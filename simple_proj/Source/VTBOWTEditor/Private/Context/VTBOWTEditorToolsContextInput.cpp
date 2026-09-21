#include "Context/VTBOWTEditorToolsContextInput.h"

#include "Context/VTBOWTEditorToolsContext.h"
#include "Context/VTBOWTEditorTransactionHistory.h"
#include "Framework/Application/SlateApplication.h"
#include "InputRouter.h"
#include "InputState.h"

FVTBOWTEditorToolsContextInput::FVTBOWTEditorToolsContextInput(UVTBOWTEditorToolsContext& InOwner,
	IVTBOWTEditorTransactionHistory& InHistory)
	: Owner(InOwner)
	, History(InHistory)
{
}

void FVTBOWTEditorToolsContextInput::BindApplicationFocus()
{
	if (FSlateApplication::IsInitialized())
	{
		auto& ActivationChanged = FSlateApplication::Get().OnApplicationActivationStateChanged();
		ActivationChanged.RemoveAll(Owner.InputRouter);
		ApplicationFocusHandle = ActivationChanged.AddWeakLambda(&Owner, [this](bool bFocused)
		{
			if (!bFocused)
			{
				CancelActiveInteraction();
			}
		});
	}
}

void FVTBOWTEditorToolsContextInput::UnbindApplicationFocus()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().OnApplicationActivationStateChanged().Remove(ApplicationFocusHandle);
	}
	ApplicationFocusHandle.Reset();
}

bool FVTBOWTEditorToolsContextInput::PostPointerInput(const FInputDeviceState& Input, bool bHover)
{
	return Owner.RunContextUpdate([&]
	{
		{
			TGuardValue<bool> InputGuard(bProcessingInput, true);
			if (bHover)
			{
				Owner.InputRouter->PostHoverInputEvent(Input);
			}
			else
			{
				Owner.InputRouter->PostInputEvent(Input);
			}
		}
		FinishPendingCancellation();
	});
}

void FVTBOWTEditorToolsContextInput::CancelActiveInteraction()
{
	if (!Owner.IsRuntimeReady() || History.IsReplaying())
	{
		return;
	}
	History.BeginCancellation();
	FinishPendingCancellation();
}

void FVTBOWTEditorToolsContextInput::FinishPendingCancellation()
{
	if (!History.IsCancelling() || bProcessingInput)
	{
		return;
	}
	Owner.RunGuardedContextUpdate([&]
	{
		TGuardValue<bool> InputGuard(bProcessingInput, true);
		Owner.InputRouter->ForceTerminateAll();
		History.EndCancellation();
	});
}

bool FVTBOWTEditorToolsContextInput::HasActiveMouseCapture() const
{
	return Owner.InputRouter && Owner.InputRouter->HasActiveMouseCapture();
}
