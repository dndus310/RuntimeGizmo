#if WITH_DEV_AUTOMATION_TESTS

#include "OWTStateMonitorModel.h"
#include "OWTStateMonitorWidget.h"
#include "Tests/OWTStateMonitorTestTypes.h"

#include "Dom/JsonObject.h"
#include "Misc/AutomationTest.h"
#include "Serialization/JsonSerializer.h"
#include "UObject/UObjectGlobals.h"

namespace OWTStateMonitorTests
{
TSharedPtr<FOWTStateMonitorNode> FindNode(const TSharedPtr<FOWTStateMonitorNode>& Root, const FString& Path)
{
	if (Root->Path == Path)
	{
		return Root;
	}
	for (const TSharedPtr<FOWTStateMonitorNode>& Child : Root->Children)
	{
		if (TSharedPtr<FOWTStateMonitorNode> Found = FindNode(Child, Path))
		{
			return Found;
		}
	}
	return nullptr;
}

void GatherPaths(const TSharedPtr<FOWTStateMonitorNode>& Root, TArray<FString>& Paths)
{
	Paths.Add(Root->Path);
	for (const TSharedPtr<FOWTStateMonitorNode>& Child : Root->Children)
	{
		GatherPaths(Child, Paths);
	}
}

int64 CountHistoryCharacters(const FOWTStateMonitorSource& Source)
{
	int64 Count = 0;
	for (const TSharedPtr<FOWTStateMonitorHistoryEntry>& Entry : Source.History)
	{
		for (const FOWTStateMonitorChange& Change : Entry->Changes)
		{
			Count += Change.Path.Len();
			Count += Change.TypeName.Len();
			Count += Change.PreviousValue.Len();
			Count += Change.Value.Len();
		}
	}
	return Count;
}
} // namespace OWTStateMonitorTests

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorReflectionTest, "OWT.StateMonitor.Model.ReflectionCopyAndJson",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorReflectionTest::RunTest(const FString& Parameters)
{
	FOWTStateMonitorModel Model;
	FOWTStateMonitorTestState State;
	State.Nested.Position = FVector(12.5, -3.0, 7.0);
	State.Nested.Text = TEXT("quotes \" slash \\ newline\n한국어");
	State.Nested.Timestamp = FDateTime(2026, 10, 8, 12, 34, 56);
	State.Items = {4, 9};
	State.Object = NewObject<UOWTStateMonitorWidget>();
	State.SoftObject = TSoftObjectPtr<UObject>(FSoftObjectPath(TEXT("/Game/NotLoaded.NotLoaded")));
	const FString ObjectPath = State.Object->GetPathName();

	TestTrue(TEXT("A reflected snapshot is accepted"),
	         Model.SubmitSnapshot(TEXT("Current"), FText::FromString(TEXT("Current")),
	                              FOWTStateMonitorTestState::StaticStruct(), &State));
	const TSharedPtr<FOWTStateMonitorSource> Source = Model.FindSource(TEXT("Current"));
	if (!TestTrue(TEXT("Source exists"), Source.IsValid()))
	{
		return false;
	}

	const TSharedPtr<FOWTStateMonitorNode> Position =
	    OWTStateMonitorTests::FindNode(Source->Root, TEXT("$/Nested/Position/X"));
	TestTrue(TEXT("Nested reflected fields are visible"), Position.IsValid());
	if (Position.IsValid())
	{
		TestEqual(TEXT("Nested numeric value"), Position->Value, FString(TEXT("12.5")));
	}
	TestTrue(TEXT("Signed int64 remains exact JSON text"),
	         Source->CurrentJson.Contains(TEXT("\"Signed\": -9223372036854775808")));
	TestTrue(TEXT("Unsigned int64 remains exact JSON text"),
	         Source->CurrentJson.Contains(TEXT("\"Unsigned\": 18446744073709551615")));
	TestFalse(TEXT("Non-UPROPERTY data is excluded"), Source->CurrentJson.Contains(TEXT("Unreflected")));
	TestTrue(TEXT("Object reference is only a path"), Source->CurrentJson.Contains(ObjectPath));
	TestTrue(TEXT("Soft reference remains a path without loading"),
	         Source->CurrentJson.Contains(TEXT("/Game/NotLoaded.NotLoaded")));
	TestFalse(TEXT("Soft reference was not loaded"), State.SoftObject.IsValid());
	// FDateTime exposes its int64 Ticks through NoExportTypes; reflected fields take precedence over text export.
	const FString ExpectedTicks = LexToString(State.Nested.Timestamp.GetTicks());
	const TSharedPtr<FOWTStateMonitorNode> TimestampTicks =
	    OWTStateMonitorTests::FindNode(Source->Root, TEXT("$/Nested/Timestamp/Ticks"));
	TestTrue(TEXT("Native timestamp reflected field is present"), TimestampTicks.IsValid());
	if (TimestampTicks.IsValid())
	{
		TestEqual(TEXT("Native timestamp preserves the exact date as ticks"), TimestampTicks->Value, ExpectedTicks);
	}
	TestTrue(TEXT("Native timestamp remains an exact JSON integer"),
	         Source->CurrentJson.Contains(TEXT("\"Ticks\": ") + ExpectedTicks));

	TSharedPtr<FJsonObject> Parsed;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Source->CurrentJson);
	TestTrue(TEXT("Generated JSON parses with escaped values"), FJsonSerializer::Deserialize(Reader, Parsed));
	if (Parsed.IsValid())
	{
		TestEqual(TEXT("Reflection names and Unicode text survive JSON"),
		          Parsed->GetObjectField(TEXT("Nested"))->GetStringField(TEXT("Text")), State.Nested.Text);
		TestTrue(TEXT("Static array reflected"), Parsed->GetArrayField(TEXT("StaticItems")).Num() == 3);
	}

	const FString CopiedJson = Source->CurrentJson;
	State.Nested.Text = TEXT("producer changed without submission");
	State.Items.Reset();
	State.Object = nullptr;
	TestEqual(TEXT("Snapshot does not retain producer memory"), Source->CurrentJson, CopiedJson);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorChangeTest, "OWT.StateMonitor.Model.ChangesAndHistory",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorChangeTest::RunTest(const FString& Parameters)
{
	FOWTStateMonitorSettings Settings;
	Settings.MaxHistoryEntries = 3;
	FOWTStateMonitorModel Model(Settings);
	FOWTStateMonitorTestState State;
	State.Nested.Text = TEXT("before");
	State.Items = {1, 2};
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	const TSharedPtr<FOWTStateMonitorSource> Frozen = Model.FindSource(TEXT("Current"));
	const uint64 Revision = Model.GetRevision();
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	TestEqual(TEXT("Repeated values do not create revisions"), Model.GetRevision(), Revision);

	State.Nested.Text = TEXT("after");
	State.Items.Pop();
	State.Counters.Add(TEXT("new"), 7);
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	TSharedPtr<FOWTStateMonitorSource> Current = Model.FindSource(TEXT("Current"));
	const TSharedPtr<FOWTStateMonitorNode> Changed =
	    OWTStateMonitorTests::FindNode(Current->Root, TEXT("$/Nested/Text"));
	TestEqual(TEXT("Previous scalar value is retained"), Changed->PreviousValue, FString(TEXT("before")));
	TestTrue(TEXT("Scalar is marked modified"), Changed->Change == EOWTStateMonitorChange::Modified);
	TestTrue(TEXT("Parent subtree is marked modified"), Current->Root->Change == EOWTStateMonitorChange::Modified);
	TestTrue(TEXT("Removed array element is recorded"), Current->Changes.ContainsByPredicate(
	                                                        [](const FOWTStateMonitorChange& Change)
	                                                        {
		                                                        return Change.Path == TEXT("$/Items/1") &&
		                                                               Change.Change == EOWTStateMonitorChange::Removed;
	                                                        }));
	TestTrue(TEXT("Added map entry is recorded"), Current->Changes.ContainsByPredicate(
	                                                  [](const FOWTStateMonitorChange& Change)
	                                                  {
		                                                  return Change.Path.StartsWith(TEXT("$/Counters/key:")) &&
		                                                         Change.Change == EOWTStateMonitorChange::Added;
	                                                  }));
	TestEqual(TEXT("Retained source pointers remain frozen"),
	          OWTStateMonitorTests::FindNode(Frozen->Root, TEXT("$/Nested/Text"))->Value, FString(TEXT("before")));

	Model.ResetBaseline(TEXT("Current"));
	Current = Model.FindSource(TEXT("Current"));
	TestTrue(TEXT("Acknowledge clears latest changes"), Current->Changes.IsEmpty());
	TestTrue(TEXT("Acknowledge clears node highlights"), Current->Root->Change == EOWTStateMonitorChange::Unchanged);
	TestEqual(TEXT("Acknowledge preserves history"), Current->History.Num(), 2);
	for (int32 Index = 0; Index < 8; ++Index)
	{
		State.Signed = Index;
		Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	}
	TestEqual(TEXT("History count is bounded"), Model.FindSource(TEXT("Current"))->History.Num(), 3);
	Model.ClearHistory(TEXT("Current"));
	TestTrue(TEXT("ClearHistory preserves current data"), Model.FindSource(TEXT("Current"))->History.IsEmpty());
	State.Signed = 10;
	Model.SubmitSnapshot(TEXT("Events"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State, false);
	TestTrue(TEXT("Journal source can opt out of redundant history"),
	         Model.FindSource(TEXT("Events"))->History.IsEmpty());
	TestTrue(TEXT("Source can be removed"), Model.RemoveSource(TEXT("Events")));
	Model.Reset();
	TestTrue(TEXT("Reset removes all sources"), Model.GetSources().IsEmpty());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorOrderingTest, "OWT.StateMonitor.Model.StableMapSetOrdering",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorOrderingTest::RunTest(const FString& Parameters)
{
	FOWTStateMonitorModel Model;
	FOWTStateMonitorTestState State;
	State.Counters.Add(TEXT("z/path~"), 1);
	State.Counters.Add(TEXT("a"), 2);
	State.Tags.Add(TEXT("z"));
	State.Tags.Add(TEXT("a"));
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	const uint64 Revision = Model.GetRevision();
	const FString FirstJson = Model.FindSource(TEXT("Current"))->CurrentJson;
	State.Counters.Reset();
	State.Counters.Add(TEXT("a"), 2);
	State.Counters.Add(TEXT("z/path~"), 1);
	State.Tags.Reset();
	State.Tags.Add(TEXT("a"));
	State.Tags.Add(TEXT("z"));
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	TestEqual(TEXT("Insertion order does not change JSON"), Model.FindSource(TEXT("Current"))->CurrentJson, FirstJson);
	TestEqual(TEXT("Insertion order does not create false history"), Model.GetRevision(), Revision);

	State.Counters[TEXT("z/path~")] = 3;
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	const TSharedPtr<FOWTStateMonitorSource> Current = Model.FindSource(TEXT("Current"));
	TestTrue(TEXT("Map value changes keep stable entry identity"),
	         Current->Changes.ContainsByPredicate(
	             [](const FOWTStateMonitorChange& Change)
	             {
		             return Change.Path.EndsWith(TEXT("/Value")) && Change.Change == EOWTStateMonitorChange::Modified;
	             }));
	TestFalse(TEXT("Map value modification creates no false removal"), Current->Changes.ContainsByPredicate(
	                                                                       [](const FOWTStateMonitorChange& Change)
	                                                                       {
		                                                                       return Change.Change ==
		                                                                              EOWTStateMonitorChange::Removed;
	                                                                       }));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FOWTStateMonitorBoundsTest, "OWT.StateMonitor.Model.BoundsAndTruncation",
                                 EAutomationTestFlags_ApplicationContextMask | EAutomationTestFlags::EngineFilter)

bool FOWTStateMonitorBoundsTest::RunTest(const FString& Parameters)
{
	FOWTStateMonitorSettings Settings;
	Settings.MaxSources = 1;
	Settings.MaxNodesPerSnapshot = 64;
	Settings.MaxTextCharacters = 16;
	Settings.MaxHistoryCharacters = 128;
	FOWTStateMonitorModel Model(Settings);
	FOWTStateMonitorTestState State;
	State.Counters.Add(TEXT("known"), 3);
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	State.Items.Init(42, 50000);
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	TSharedPtr<FOWTStateMonitorSource> Current = Model.FindSource(TEXT("Current"));
	TestTrue(TEXT("Node budget is reported"), Current->bTruncated);
	TArray<FString> Paths;
	OWTStateMonitorTests::GatherPaths(Current->Root, Paths);
	TestTrue(TEXT("Node count respects the budget"), Paths.Num() <= Settings.MaxNodesPerSnapshot);
	TestFalse(TEXT("Missing values in a truncated capture are not falsely removed"),
	          Current->Changes.ContainsByPredicate(
	              [](const FOWTStateMonitorChange& Change)
	              {
		              return Change.Change == EOWTStateMonitorChange::Removed;
	              }));
	TestFalse(TEXT("Source capacity rejects extra source"),
	          Model.SubmitSnapshot(TEXT("Overflow"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State));
	TestFalse(TEXT("Null struct type is rejected"), Model.SubmitSnapshot(TEXT("Current"), FText(), nullptr, &State));
	TestFalse(TEXT("Null struct data is rejected"),
	          Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), nullptr));
	TestFalse(TEXT("None source id is rejected"),
	          Model.SubmitSnapshot(NAME_None, FText(), FOWTStateMonitorTestState::StaticStruct(), &State));

	State.Items.Reset();
	State.Counters.Add(TEXT("identical_prefix_but_distinct_1"), 1);
	State.Counters.Add(TEXT("identical_prefix_but_distinct_2"), 2);
	Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	Current = Model.FindSource(TEXT("Current"));
	TestTrue(TEXT("Incomplete map identities report truncation"), Current->bTruncated);
	const TSharedPtr<FOWTStateMonitorNode> Counters = OWTStateMonitorTests::FindNode(Current->Root, TEXT("$/Counters"));
	TestEqual(TEXT("Incomplete keys are omitted rather than merged"), Counters->Children.Num(), 1);
	Paths.Reset();
	OWTStateMonitorTests::GatherPaths(Current->Root, Paths);
	TSet<FString> UniquePaths;
	for (const FString& Path : Paths)
	{
		UniquePaths.Add(Path);
	}
	TestEqual(TEXT("Visible field paths remain unique"), UniquePaths.Num(), Paths.Num());

	for (int32 Index = 0; Index < 20; ++Index)
	{
		State.Signed = Index;
		Model.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	}
	TestTrue(TEXT("Retained history text obeys the source budget"),
	         OWTStateMonitorTests::CountHistoryCharacters(*Model.FindSource(TEXT("Current"))) <=
	             Settings.MaxHistoryCharacters);
	Settings.MaxDepth = 2;
	FOWTStateMonitorModel ShallowModel(Settings);
	ShallowModel.SubmitSnapshot(TEXT("Current"), FText(), FOWTStateMonitorTestState::StaticStruct(), &State);
	TestTrue(TEXT("Depth limit is reported"), ShallowModel.FindSource(TEXT("Current"))->bTruncated);
	return true;
}

#endif
