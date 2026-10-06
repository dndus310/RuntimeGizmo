#pragma once

class AActor;
class ACombinedTransformGizmoActor;
class AVTBOWTSpectator;
class UVTBOWTEditorSubsystem;

bool ValidateOWTGizmoSnapping(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target);
bool ValidateOWTCameraNavigation(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub);
void PrepareOWTGizmoHeadlessHandles(ACombinedTransformGizmoActor& Actor);
bool ValidateOWTBaseGizmoMaterials(UVTBOWTEditorSubsystem& Hub);
bool ValidateOWTCustomGizmo(AVTBOWTSpectator& Pawn, UVTBOWTEditorSubsystem& Hub, AActor& Target);
