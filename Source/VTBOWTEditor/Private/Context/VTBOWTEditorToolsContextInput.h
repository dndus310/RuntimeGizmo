#pragma once

#include "CoreMinimal.h"
#include "Context/IVTBOWTEditorInput.h"

class UVTBOWTEditorToolsContext;
class IVTBOWTEditorTransactionHistory;
struct FInputDeviceState;

class FVTBOWTEditorToolsContextInput : public IVTBOWTEditorInput
{
public:
	FVTBOWTEditorToolsContextInput(UVTBOWTEditorToolsContext& InOwner, IVTBOWTEditorTransactionHistory& InHistory);

	void BindApplicationFocus();
	void UnbindApplicationFocus();
	virtual bool PostPointerInput(const FInputDeviceState& Input, bool bHover) override;
	virtual void CancelActiveInteraction() override;
	void FinishPendingCancellation();
	virtual bool HasActiveMouseCapture() const override;

private:
	UVTBOWTEditorToolsContext& Owner;
	IVTBOWTEditorTransactionHistory& History;
	FDelegateHandle ApplicationFocusHandle;
	bool bProcessingInput = false;
};
