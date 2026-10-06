#if WITH_DEV_AUTOMATION_TESTS

#include "Events/OWTNotificationCenter.h"
#include "Tests/OWTEventTestObject.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTEventOwnershipTest, "OWT.EventCore.OwnerIsolation",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTEventOwnershipTest::RunTest(const FString& Parameters)
{
	UObject* FirstOwner = NewObject<UOWTEventTestObject>();
	UObject* SecondOwner = NewObject<UOWTEventTestObject>();
	UOWTNotificationCenter* First = NewObject<UOWTNotificationCenter>(FirstOwner);
	UOWTNotificationCenter* Second = NewObject<UOWTNotificationCenter>(SecondOwner);
	TestFalse(TEXT("Uninitialized center rejects publication"), First->Publish(TEXT("Changed"), TEXT("{}")));
	TestFalse(TEXT("A different Outer cannot initialize the center"), First->Initialize(SecondOwner));
	TestFalse(TEXT("A null owner cannot initialize the center"), First->Initialize(nullptr));
	TestTrue(TEXT("Plain UObject owner initializes without an editor or world"), First->Initialize(FirstOwner));
	TestTrue(TEXT("A second owner gets an independent center"), Second->Initialize(SecondOwner));

	int32 FirstCount = 0;
	int32 SecondCount = 0;
	const FOWTAttributeEventNative FirstCallback = FOWTAttributeEventNative::CreateLambda(
	    [&FirstCount](FName, const FString&)
	    {
		    ++FirstCount;
	    });
	const FGuid FirstHandle = First->Subscribe(FirstOwner, FirstCallback);
	Second->Subscribe(SecondOwner, FOWTAttributeEventNative::CreateLambda(
	                                   [&SecondCount](FName, const FString&)
	                                   {
		                                   ++SecondCount;
	                                   }));
	TestTrue(TEXT("Native subscription returns a handle"), FirstHandle.IsValid());
	TestFalse(TEXT("Null receiver is rejected"), First->Subscribe(nullptr, FirstCallback).IsValid());
	TestFalse(TEXT("Unbound delegate is rejected"), First->Subscribe(FirstOwner, FOWTAttributeEventNative()).IsValid());
	TestFalse(TEXT("Unbound dynamic delegate is rejected"),
	          First->SubscribeDynamic(FirstOwner, FOWTAttributeEventDynamic()).IsValid());
	UOWTEventTestObject* DynamicReceiver = NewObject<UOWTEventTestObject>();
	FOWTAttributeEventDynamic DynamicCallback;
	DynamicCallback.BindDynamic(DynamicReceiver, &UOWTEventTestObject::OnEvent);
	TestFalse(TEXT("Dynamic receiver must match the bound delegate object"),
	          First->SubscribeDynamic(FirstOwner, DynamicCallback).IsValid());
	const FGuid DynamicHandle = First->SubscribeDynamic(DynamicReceiver, DynamicCallback);
	TestTrue(TEXT("Bound dynamic subscription returns a handle"), DynamicHandle.IsValid());
	TestTrue(TEXT("Dynamic subscription receives targeted publication"),
	         First->Publish(TEXT("Dynamic"), TEXT("{\"value\":7}"), DynamicHandle));
	TestEqual(TEXT("Dynamic callback runs exactly once"), DynamicReceiver->InvocationCount, 1);
	TestEqual(TEXT("Dynamic callback receives the topic"), DynamicReceiver->LastEvent, FName(TEXT("Dynamic")));
	TestEqual(TEXT("Dynamic callback receives the original JSON"), DynamicReceiver->LastJson,
	          FString(TEXT("{\"value\":7}")));
	TestEqual(TEXT("Targeted dynamic event does not broadcast to native receiver"), FirstCount, 0);
	First->Unsubscribe(DynamicHandle);
	TestTrue(TEXT("Repeated initialization preserves existing subscription"), First->Initialize(FirstOwner));
	TestTrue(TEXT("Valid JSON publishes"), First->Publish(TEXT("Changed"), TEXT("{}")));
	TestEqual(TEXT("First owner receives its event"), FirstCount, 1);
	TestEqual(TEXT("Second owner does not receive first owner's event"), SecondCount, 0);
	TestFalse(TEXT("Malformed JSON does not publish"), First->Publish(TEXT("Changed"), TEXT("{broken")));
	TestFalse(TEXT("Empty topic does not publish"), First->Publish(NAME_None, TEXT("{}")));
	TestFalse(TEXT("Oversized transport does not publish"),
	          First->Publish(TEXT("Changed"), TEXT("\"") + FString::ChrN(65536, TEXT('x')) + TEXT("\"")));
	TestEqual(TEXT("Rejected payloads did not reach subscriptions"), FirstCount, 1);

	TestTrue(TEXT("JSON array payload is supported"),
	         First->Publish(TEXT("Array"), TEXT("[1,true,null]"), FirstHandle));
	TestTrue(TEXT("JSON scalar payload is supported"), First->Publish(TEXT("Scalar"), TEXT("42"), FirstHandle));
	TestTrue(TEXT("Valid recipient targets its subscription"),
	         First->Publish(TEXT("Targeted"), TEXT("{}"), FirstHandle));
	TestEqual(TEXT("Targeted publications reach the receiver"), FirstCount, 4);
	TestTrue(TEXT("Unknown recipient is accepted without broadcasting"),
	         First->Publish(TEXT("Targeted"), TEXT("{}"), FGuid::NewGuid()));
	TestEqual(TEXT("Unknown recipient does not broadcast"), FirstCount, 4);
	TestTrue(TEXT("Subscription can be removed"), First->Unsubscribe(FirstHandle));
	TestFalse(TEXT("Unknown subscription is a normal absence"), First->Unsubscribe(FirstHandle));
	First->Publish(TEXT("Changed"), TEXT("{}"));
	TestEqual(TEXT("Removed subscription receives no later event"), FirstCount, 4);

	FirstOwner->MarkAsGarbage();
	TestFalse(TEXT("Weak owner invalidation disables the center"), First->IsReady());
	TestFalse(TEXT("An invalid owner cannot publish"), First->Publish(TEXT("Changed"), TEXT("{}")));
	TestFalse(TEXT("An invalid owner cannot journal"), First->RecordEvent(TEXT("Request"), TEXT("{}")));
	First->Shutdown();
	Second->Shutdown();
	return !HasAnyErrors();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTEventMutationTest, "OWT.EventCore.CallbackMutation",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTEventMutationTest::RunTest(const FString& Parameters)
{
	UObject* Owner = NewObject<UOWTEventTestObject>();
	UObject* Receiver = NewObject<UOWTEventTestObject>();
	UOWTNotificationCenter* Center = NewObject<UOWTNotificationCenter>(Owner);
	Center->Initialize(Owner);
	int32 SelfCount = 0;
	int32 StableCount = 0;
	int32 AddedCount = 0;
	FGuid SelfHandle;
	FGuid AddedHandle;
	SelfHandle = Center->Subscribe(Receiver, FOWTAttributeEventNative::CreateLambda(
	                                             [&](FName, const FString&)
	                                             {
		                                             ++SelfCount;
		                                             Center->Unsubscribe(SelfHandle);
		                                             AddedHandle = Center->Subscribe(
		                                                 Owner, FOWTAttributeEventNative::CreateLambda(
		                                                            [&AddedCount](FName, const FString&)
		                                                            {
			                                                            ++AddedCount;
		                                                            }));
		                                             Center->Publish(TEXT("Nested"), TEXT("{}"));
	                                             }));
	const FGuid StableHandle = Center->Subscribe(Owner, FOWTAttributeEventNative::CreateLambda(
	                                                        [&StableCount](FName, const FString&)
	                                                        {
		                                                        ++StableCount;
	                                                        }));
	Center->Publish(TEXT("Start"), TEXT("{}"));
	TestEqual(TEXT("Self unsubscribe is safe during delivery"), SelfCount, 1);
	TestEqual(TEXT("Nested publication is drained after the active event"), StableCount, 2);
	TestEqual(TEXT("New subscription begins with the next queued event"), AddedCount, 1);
	Center->Unsubscribe(StableHandle);
	Center->Unsubscribe(AddedHandle);

	int32 DeadCount = 0;
	Center->Subscribe(Receiver, FOWTAttributeEventNative::CreateLambda(
	                                [&DeadCount](FName, const FString&)
	                                {
		                                ++DeadCount;
	                                }));
	Receiver->MarkAsGarbage();
	Center->Publish(TEXT("Changed"), TEXT("{}"));
	TestEqual(TEXT("Weak receiver invalidation suppresses the lambda"), DeadCount, 0);

	int32 LoopCount = 0;
	const FGuid LoopHandle = Center->Subscribe(Owner, FOWTAttributeEventNative::CreateLambda(
	                                                      [&](FName, const FString&)
	                                                      {
		                                                      ++LoopCount;
		                                                      Center->Publish(TEXT("Loop"), TEXT("{}"));
	                                                      }));
	AddExpectedError(TEXT("OWT event dispatch limit exceeded"), EAutomationExpectedErrorFlags::Contains, 1);
	Center->Publish(TEXT("Loop"), TEXT("{}"));
	TestEqual(TEXT("Recursive publishing is bounded"), LoopCount, 256);
	Center->Unsubscribe(LoopHandle);

	bool bReinitializedDuringDelivery = true;
	Center->Subscribe(Owner, FOWTAttributeEventNative::CreateLambda(
	                             [&](FName, const FString&)
	                             {
		                             Center->Publish(TEXT("QueuedBeforeShutdown"), TEXT("{}"));
		                             Center->Shutdown();
		                             bReinitializedDuringDelivery = Center->Initialize(Owner);
	                             }));
	Center->Publish(TEXT("Shutdown"), TEXT("{}"));
	TestFalse(TEXT("Reinitialization cannot replace an active dispatch session"), bReinitializedDuringDelivery);
	TestFalse(TEXT("Shutdown during a callback disables the center"), Center->IsReady());
	TestTrue(TEXT("Center can initialize after delivery has unwound"), Center->Initialize(Owner));
	TestTrue(TEXT("A fresh session can publish"), Center->Publish(TEXT("Fresh"), TEXT("{}")));
	Center->Shutdown();
	return !HasAnyErrors();
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTEventHistoryTest, "OWT.EventCore.BoundedHistory",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTEventHistoryTest::RunTest(const FString& Parameters)
{
	UObject* Owner = NewObject<UOWTEventTestObject>();
	UOWTNotificationCenter* Center = NewObject<UOWTNotificationCenter>(Owner);
	Center->Initialize(Owner);
	Center->SetHistoryCapacity(2);
	int32 DeliveryCount = 0;
	int32 DeliveredCharacters = 0;
	Center->Subscribe(Owner, FOWTAttributeEventNative::CreateLambda(
	                             [&](FName, const FString& Json)
	                             {
		                             ++DeliveryCount;
		                             DeliveredCharacters = Json.Len();
	                             }));
	TestTrue(TEXT("Malformed inbound request is journaled"), Center->RecordEvent(TEXT("Request"), TEXT("{invalid")));
	TestEqual(TEXT("Journal-only requests never reach subscribers"), DeliveryCount, 0);
	TArray<FOWTEventRecord> Records = Center->GetRecentEvents();
	TestEqual(TEXT("Request creates one record"), Records.Num(), 1);
	if (Records.Num() != 1)
	{
		Center->Shutdown();
		return false;
	}
	TestFalse(TEXT("Malformed request is marked invalid"), Records[0].bValidJson);
	TestEqual(TEXT("RecordEvent defaults to inbound"), Records[0].Direction, EOWTEventDirection::Inbound);
	TestTrue(TEXT("Record timestamp is set"), Records[0].TimestampUtc.GetTicks() > 0);
	TestEqual(TEXT("First record has sequence one"), Records[0].Sequence, int64(1));

	const FString LargeJson = TEXT("{\"text\":\"") + FString::ChrN(20000, TEXT('x')) + TEXT("\"}");
	Center->Publish(TEXT("Large"), LargeJson);
	Center->Publish(TEXT("Last"), TEXT("{}"));
	Records = Center->GetRecentEvents();
	TestEqual(TEXT("History discards oldest record at capacity"), Records.Num(), 2);
	if (Records.Num() != 2)
	{
		Center->Shutdown();
		return false;
	}
	TestEqual(TEXT("Returned history stays oldest to newest"), Records[0].Event, FName(TEXT("Large")));
	TestEqual(TEXT("Journal copy has a bounded character count"), Records[0].Json.Len(), 16384);
	TestTrue(TEXT("Truncated copy is marked"), Records[0].bPayloadTruncated);
	TestTrue(TEXT("Original JSON validity survives journal truncation"), Records[0].bValidJson);
	TestEqual(TEXT("Published events have outbound direction"), Records[0].Direction, EOWTEventDirection::Outbound);
	TestEqual(TEXT("Latest event sequence"), Records[1].Sequence, int64(3));
	TestEqual(TEXT("Latest N includes only the newest records"), Center->GetRecentEvents(1)[0].Sequence, int64(3));
	TestEqual(TEXT("Zero requested records returns an empty snapshot"), Center->GetRecentEvents(0).Num(), 0);
	Records[0].Json = TEXT("mutated caller copy");
	TestEqual(TEXT("History is returned by copy"), Center->GetRecentEvents()[0].Json.Len(), 16384);

	Center->Publish(TEXT("LargeAgain"), LargeJson);
	TestEqual(TEXT("Transport retains payload beyond journal cutoff"), DeliveredCharacters, LargeJson.Len());
	const int64 BeforeClear = Center->GetLatestSequence();
	const int64 BeforeClearRevision = Center->GetHistoryRevision();
	Center->ClearEventHistory();
	TestEqual(TEXT("Clear removes retained records"), Center->GetRecentEvents().Num(), 0);
	TestEqual(TEXT("Clear preserves the sequence cursor"), Center->GetLatestSequence(), BeforeClear);
	TestTrue(TEXT("Clear changes the history revision for monitors"),
	         Center->GetHistoryRevision() > BeforeClearRevision);
	Center->SetHistoryCapacity(0);
	Center->RecordEvent(NAME_None, TEXT("malformed unnamed request"));
	TestEqual(TEXT("Zero capacity disables retention"), Center->GetRecentEvents().Num(), 0);
	TestEqual(TEXT("Sequence advances while retention is disabled"), Center->GetLatestSequence(), BeforeClear + 1);
	Center->SetHistoryCapacity(1000000);
	TestEqual(TEXT("Capacity is bounded"), Center->GetHistoryCapacity(), 1024);
	Center->Shutdown();
	return !HasAnyErrors();
}

#endif
