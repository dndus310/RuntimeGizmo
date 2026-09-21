#include "VTBOWTEditGameState.h"

#include "Net/UnrealNetwork.h"

void AVTBOWTEditGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AVTBOWTEditGameState, bEditMode);
	DOREPLIFETIME(AVTBOWTEditGameState, ActiveGizmoMode);
}

bool AVTBOWTEditGameState::GetEditorState_Implementation() const
{
	return bEditMode;
}

EActiveGizmoMode AVTBOWTEditGameState::GetActiveGizmoMode_Implementation() const
{
	return ActiveGizmoMode;
}

void AVTBOWTEditGameState::UpdateEditorState(bool bNewState)
{
	if (!HasAuthority() || bEditMode == bNewState)
	{
		return;
	}

	bEditMode = bNewState;
	ForceNetUpdate();
	OnRep_EditMode();
}

void AVTBOWTEditGameState::UpdateActiveGizmoMode(EActiveGizmoMode NewGizmoMode)
{
	if (!HasAuthority() || ActiveGizmoMode == NewGizmoMode)
	{
		return;
	}
	if (NewGizmoMode != EActiveGizmoMode::Transform && NewGizmoMode != EActiveGizmoMode::Spline)
	{
		return;
	}

	ActiveGizmoMode = NewGizmoMode;
	ForceNetUpdate();
	OnRep_ActiveGizmoMode();
}

void AVTBOWTEditGameState::OnRep_EditMode()
{
	OnEditorStateChanged.Broadcast(bEditMode);
}

void AVTBOWTEditGameState::OnRep_ActiveGizmoMode()
{
	OnActiveGizmoModeChanged.Broadcast(ActiveGizmoMode);
}
