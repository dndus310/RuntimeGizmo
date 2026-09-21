# RuntimeEditorTools Integration

> Historical implementation notes. Since 2026-09-22, the plugin is disabled and implementation lives in Source/VTBOWTEditor and Source/simple_proj. See [VTBOWT_Runtime.md](VTBOWT_Runtime.md) for the current architecture and runtime setup.

## Ownership

- `UVTBEditorSubsystem` owns one local editing session per Game/PIE world, subscribes to the selection source, and orders pending selection, view, and tick work.
- `UVTBEditorInteractiveToolsContext` owns the stock ITF managers/router, private Query/Transaction implementations, and the runtime Gizmo Builder/Gizmo lifetime.
- `UVTBEditorTransformGizmo` owns the weak actor selection, validates movable roots, creates and refreshes its `UTransformProxy`, and applies target/handle state.
- `UVTBEditorTransformGizmoBuilder` keeps the runtime `UCombinedTransformGizmoBuilder` flow and uses ITF's stock actor factory for the session.
- `AVTBEditorTransformGizmoActor` is the reserved extension point for project-specific handle customization.
- `UVTBEditorTransformGizmoBehavior` handles modifier filtering on the axis, plane, and angle sub-gizmos.
- `AVTBEditorGameMode` is only an example `IVTBSelectionSource`.
- `AVTBEditorSpectatorPawn` installs `/Game/RuntimeEditor/Input/IMC_VTBEditor`, converts mouse state to `FInputDeviceState`, and forwards pointer events to the stock ITF `UInputRouter`.

There is no separate input bridge, render bridge, gizmo manager wrapper, selection manager, or public settings object in the current implementation.

The context creates the runtime gizmo on the first selection request and keeps it
registered until the session ends. Empty selection clears the proxy and active target.
`NoGizmo` clears the active target and hides the actor while retaining the selection and
proxy for the next enabled mode. The subsystem forwards these transitions before the
context tick, after the context update guard has accepted the work. Changing between
single and multiple targets rebuilds only the visual actor and its handles, preserving the
selection owner. The runtime gizmo uses the stock modifier policy and keeps the same actor,
proxy, and behavior for the session.
Retired proxies remain available to the transaction history for Undo/Redo.

## PlayerController Connection

### Editor mode state

`AVTBEditorGameMode` handles mode commands through `IVTBOWTEditorModeControl` and forwards
them to `AVTBOWTEditGameState`. GameState stores `bEditMode` (initially `false`) and
`ActiveGizmoMode` (initially `Transform`). It updates the value before broadcasting
`OnEditorStateChanged(bool)` or `OnActiveGizmoModeChanged(EActiveGizmoMode)`. Repeating
the current value does not broadcast. `Count` and invalid gizmo modes are ignored.

Use `VTBEditorModeInterface.h` to issue commands on the server and query GameState:

```cpp
if (AGameModeBase* Mode = GetWorld()->GetAuthGameMode();
	Mode && Mode->Implements<UVTBOWTEditorModeControl>())
{
	IVTBOWTEditorModeControl::Execute_SetEditorState(Mode, true);
	IVTBOWTEditorModeControl::Execute_SetActiveGizmoMode(Mode, EActiveGizmoMode::Spline);
}

if (AGameStateBase* State = GetWorld()->GetGameState();
	State && State->Implements<UVTBOWTEditorModeState>())
{
	const bool bEditing = IVTBOWTEditorModeState::Execute_GetEditorState(State);
	const EActiveGizmoMode Mode = IVTBOWTEditorModeState::Execute_GetActiveGizmoMode(State);
}
```

In Blueprint, call the same interface functions on GameMode/GameState and bind to
GameState's two event dispatchers. Bind first, then query the current values to
initialize a newly created UI; events report subsequent changes. GameState replicates
both properties and broadcasts the same events through RepNotify on clients. Client
commands must reach the server through the game's owning PlayerController RPC.
`InitGameState()` also applies commands received before GameState was created.

Use the subsystem from the local controller, pawn, or game object that owns selection.

```cpp
UVTBEditorSubsystem* Editor = GetWorld()->GetSubsystem<UVTBEditorSubsystem>();
if (!Editor)
{
	return;
}

Editor->ReceiveSelection({SelectedActor});
```

If a GameMode implements `IVTBSelectionSource`, the subsystem binds it on BeginPlay. If selection is owned somewhere else, either pass that object to `BindSelectionSource(Source)` or directly call `ReceiveSelection(WeakActors)` whenever selection changes.

```cpp
Editor->ReceiveSelection({SelectedActor});
Editor->ReceiveSelection({});
```

The lightweight subsystem does not expose mode switching. `AVTBEditorSpectatorPawn` obtains
the Context through `GetRuntimeContext()` and owns the default runtime editor commands:
W/E/R select Translation/Rotation/Scale, T cycles TRS, Escape cancels selection, and Ctrl +
` toggles Local/World space for Translation, Rotation, and Combined modes; Scale always uses Local space.

## Pointer Input

The subsystem no longer exposes `PerformPointerInput`, `PerformCancel`, `PerformUndo`, or `PerformRedo`. Tests exercise those responsibilities through `UVTBEditorInteractiveToolsContext` and the stock `UInputRouter`. Runtime mouse routing lives in `AVTBEditorSpectatorPawn`: left mouse is sent to the Router first, and actor selection only runs when no gizmo capture starts.

The input assets are:

| Asset | Key |
|---|---|
| `IMC_VTBEditor` | Mapping context |
| `IA_EditSelect` | Left Mouse |
| `IA_EditTranslation` | W |
| `IA_EditRotation` | E |
| `IA_EditScale` | R |
| `IA_ChagneGizmoMode` | T |
| `IA_EditSelectCancel` | Escape |
| `IA_EditSpace` | Tilde, with Ctrl checked by the Pawn handler |

## Custom Handles

The current runtime path uses `FCombinedTransformGizmoActorFactory` and its stock
`ACombinedTransformGizmoActor` components. `AVTBEditorTransformGizmoActor` remains
available as the project customization hook; a future custom factory can spawn it and
populate the inherited handle fields while preserving the same element flags.

This follows the Builder/Actor/Gizmo/Behavior separation used by `DirectionalLightGizmo`, while retaining runtime `UCombinedTransformGizmo` for TRS, snapping, and proxy transactions. The existing ITF axis/plane/angle builders remain in use. `UVTBEditorTransformGizmoBehavior` configures their existing drag behaviors after target creation, so hover and capture termination retain the ITF behavior lifecycle.

## Boundaries

- Runtime code has no `UnrealEd`, `EditorInteractiveToolsFramework`, or `GEditor` dependency.
- The subsystem does not install Enhanced Input mappings; the pawn/controller layer owns them.
- Network replication and authority policy belong to the game layer.
- Blueprint-facing wrapper functions should live in the owning GameMode, Controller, or project facade when needed.
