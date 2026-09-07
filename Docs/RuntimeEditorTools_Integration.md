# RuntimeEditorTools Integration

## Ownership

- `UVTBEditorSubsystem` owns one local editing session per Game/PIE world.
- `UVTBEditorInteractiveToolsContext` owns the stock ITF managers and private Query/Transaction implementations.
- `UVTBEditorTargetAdapter` converts weak actor selections into a `UTransformProxy`.
- `UVTBEditorTransformGizmoBuilder` creates `UVTBEditorTransformGizmo` and `AVTBEditorTransformGizmoActor`.
- `AVTBEditorGameMode` is only an example `IVTBSelectionSource`.
- `AVTBEditorSpectatorPawn` installs `/Game/RuntimeEditor/Input/IMC_VTBEditor`, converts mouse state to `FInputDeviceState`, and forwards pointer events to the stock ITF `UInputRouter`.

There is no separate input bridge, render bridge, gizmo manager wrapper, selection manager, or public settings object in the current implementation.

## PlayerController Connection

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

The lightweight subsystem does not expose mode switching. `AVTBEditorSpectatorPawn` owns the default runtime editor commands: W/E/R select Translation/Rotation/Scale, T cycles TRS, Escape cancels selection, and Ctrl + ` toggles Local/World space.

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

Subclass `AVTBEditorTransformGizmoActor` if you need different handle components. Override `BuildComponents` or the `Create*Handle` functions and wire that actor class from a real gizmo builder/facade when the game needs customization.

The default actor uses stock ITF primitive gizmo components, screen-space sizing through `UGizmoViewContext`, and no gameplay collision.

## Boundaries

- Runtime code has no `UnrealEd`, `EditorInteractiveToolsFramework`, or `GEditor` dependency.
- The subsystem does not install Enhanced Input mappings; the pawn/controller layer owns them.
- Network replication and authority policy belong to the game layer.
- Blueprint-facing wrapper functions should live in the owning GameMode, Controller, or project facade when needed.
