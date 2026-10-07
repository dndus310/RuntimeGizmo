#include "OWTStateMonitorWidget.h"

#include "OWTStateMonitorModel.h"
#include "SOWTStateMonitor.h"
#include "UObject/Stack.h"
#include "UObject/UnrealType.h"

UOWTStateMonitorWidget::UOWTStateMonitorWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer), MonitorModel(), MonitorView()
{
}

void UOWTStateMonitorWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	MonitorView.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}

bool UOWTStateMonitorWidget::SubmitSnapshot(FName SourceId, FText Label, const int32& Snapshot)
{
	// Blueprint executes the custom thunk so the actual script struct is available.
	return false;
}

DEFINE_FUNCTION(UOWTStateMonitorWidget::execSubmitSnapshot)
{
	P_GET_PROPERTY(FNameProperty, SourceId);
	P_GET_PROPERTY(FTextProperty, Label);
	Stack.MostRecentProperty = nullptr;
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.StepCompiledIn<FStructProperty>(nullptr);
	const FStructProperty* StructProperty = CastField<FStructProperty>(Stack.MostRecentProperty);
	const void* StructData = Stack.MostRecentPropertyAddress;
	P_FINISH;

	P_NATIVE_BEGIN;
	bool bSubmitted = false;
	if (StructProperty)
	{
		bSubmitted = P_THIS->SubmitStructSnapshot(SourceId, Label, StructProperty->Struct, StructData);
	}
	else
	{
		FFrame::KismetExecutionMessage(TEXT("State Monitor requires a reflected struct snapshot."),
		                               ELogVerbosity::Warning);
	}
	*static_cast<bool*>(RESULT_PARAM) = bSubmitted;
	P_NATIVE_END;
}

bool UOWTStateMonitorWidget::RemoveSource(FName SourceId)
{
	return GetMonitorModel()->RemoveSource(SourceId);
}

void UOWTStateMonitorWidget::ClearHistory(FName SourceId)
{
	GetMonitorModel()->ClearHistory(SourceId);
}

void UOWTStateMonitorWidget::ResetBaseline(FName SourceId)
{
	GetMonitorModel()->ResetBaseline(SourceId);
}

void UOWTStateMonitorWidget::ResetMonitor()
{
	GetMonitorModel()->Reset();
}

bool UOWTStateMonitorWidget::SubmitStructSnapshot(FName SourceId, const FText& Label, const UScriptStruct* StructType,
                                                  const void* StructData)
{
	return GetMonitorModel()->SubmitSnapshot(SourceId, Label, StructType, StructData);
}

TSharedPtr<FOWTStateMonitorModel> UOWTStateMonitorWidget::GetMonitorModel()
{
	check(IsInGameThread());
	if (!MonitorModel.IsValid())
	{
		MonitorModel = MakeShared<FOWTStateMonitorModel>();
	}
	return MonitorModel;
}

TSharedRef<SWidget> UOWTStateMonitorWidget::RebuildWidget()
{
	return SAssignNew(MonitorView, SOWTStateMonitor).Model(GetMonitorModel());
}
