# VTB 런타임 Transform Gizmo

> 이전 구현 기록입니다. 2026-09-22부터 VTBRuntimeEditor 플러그인을 사용하지 않고 Source/VTBOWTEditor와 Source/simple_proj에서 구현합니다. 현재 구조와 실행 방법은 [VTBOWT_Runtime.md](VTBOWT_Runtime.md)를 참고하세요.

대상: Unreal Engine 5.7.4. 게임 측 C++ 코드가 `UVTBEditorSubsystem`에 선택을 전달하고 실제 Gizmo 구성은 InteractiveToolsFramework 기본 Manager와 Router를 사용합니다. Enhanced Input 자산은 `/Game/RuntimeEditor/Input`에 있고, 기본 `AVTBEditorSpectatorPawn`이 입력을 Context/Router로 전달합니다.

## Subsystem API

`UVTBEditorSubsystem`은 런타임 선택 세션의 입구와 수명 순서만 담당합니다. 선택 소스를 구독하고 pending 선택을 Context와 Gizmo에 전달한 뒤 Context의 view/tick 순서를 조정합니다. ITF manager, Router, Gizmo Builder와 target 연결은 각각 Context와 Gizmo가 소유합니다.

| API | 역할 |
|---|---|
| `BindSelectionSource(Source)` | `IVTBSelectionSource`를 구현한 GameMode, Controller, UObject를 구독합니다. `nullptr`이면 구독 해제와 선택 해제를 함께 수행합니다. |
| `ReceiveSelection(WeakActors)` | 선택 소스 없이 선택된 액터를 직접 전달합니다. |
| `GetRuntimeContext()` | Pawn 같은 런타임 입력 계층이 Context API를 사용할 때만 Context를 반환합니다. manager와 Gizmo 조합은 노출하지 않습니다. |

Context Query가 반환하는 `Mode`는 `NoGizmo`, `Translation`, `Rotation`, `Scale`, `Combined`를 그대로 사용합니다. `NoGizmo`는 Gizmo만 비활성화하고 선택은 유지합니다. `CoordinateSystem`은 World/Local만 허용합니다.

## 연결 예시

```cpp
#include "RuntimeEditor/VTBEditorSubsystem.h"

UVTBEditorSubsystem* Editor = GetWorld()->GetSubsystem<UVTBEditorSubsystem>();
if (!Editor)
{
	return;
}

Editor->ReceiveSelection({TargetActor});
```

월드 BeginPlay에서 GameMode가 있으면 자동으로 `IVTBSelectionSource` 바인딩을 시도합니다. GameMode 없는 클라이언트 월드는 이미 전달된 선택을 유지합니다. 인터페이스는 [VTBSelectionSource.h](C:/Users/jkyii/Desktop/simple_proj/Source/simple_proj/RuntimeEditor/Selection/VTBSelectionSource.h)에 있으며, `AVTBEditorGameMode`가 구현합니다. `SetSelectedActors(Actors)` 또는 `ClearSelection()`으로 변경하면 스냅샷을 갱신한 뒤 `OnSelectionChanged()` 델리게이트가 알림을 보냅니다. 조회는 `GetSelectionSnapshot(OutActors)`를 사용합니다.

## 입력 위치

테스트 전용으로 쓰이던 `PerformPointerInput`, `PerformCancel`, `PerformUndo`, `PerformRedo`는 Subsystem에서 제거했습니다. 런타임 입력은 [VTBEditorSpectatorPawn](/C:/Users/jkyii/Desktop/simple_proj/Source/simple_proj/VTBEditorSpectatorPawn.cpp:1)이 소유합니다. Pawn은 `FInputDeviceState`를 구성해 `UVTBEditorInteractiveToolsContext`의 stock `UInputRouter`에 pointer press/drag/release/hover를 전달하고, 키 입력은 Context의 `GizmoMode`와 `CoordinateSystem`을 갱신합니다.

| Asset | Key | 처리 |
|---|---|---|
| `IA_EditSelect` | Left Mouse | 먼저 ITF Router에 전달하고 capture가 없으면 액터 선택 |
| `IA_EditTranslation` | W | Translation 모드 |
| `IA_EditRotation` | E | Rotation 모드 |
| `IA_EditScale` | R | Scale 모드 |
| `IA_ChagneGizmoMode` | T | Translation → Rotation → Scale 순환 |
| `IA_EditSelectCancel` | Escape | active interaction 취소와 선택 해제 |
| `IA_EditSpace` | Ctrl + ` | Translation/Rotation/Combined 모드에서 Local/World 전환. Scale 모드에서는 Local 좌표계가 고정됩니다. IMC 키는 `Tilde`이고 핸들러에서 Ctrl modifier를 검증합니다. |

## 내부 책임

| 구성 요소 | 책임 |
|---|---|
| `UVTBEditorSubsystem` | 월드 수명, 선택 구독, pending 선택 전달, Context/Gizmo tick 순서 |
| `AVTBEditorSpectatorPawn` | Enhanced Input IMC 설치, pointer event 생성, Router 전달, TRS/Space command 적용 |
| `UVTBEditorInteractiveToolsContext` | 기본 ITF 서비스와 manager/router 수명, 런타임 Gizmo Builder/생성, LocalPlayer SceneView, Queries/Transactions, 취소와 Undo/Redo |
| `FVTBEditorQueriesAPI` | Context cpp 내부의 월드, 선택, 카메라, 모드, 좌표계 조회 |
| `FVTBEditorTransactionsAPI` | 런타임 전용 그룹 Undo/Redo, 취소 rollback, TransformProxy GC 보존 |
| `UVTBEditorTransformGizmo` | 약한 액터 선택, movable root 검증, `UTransformProxy` 생성·갱신, 선택 수·모드에 따른 target/서브 기즈모 연결과 회전축 표시 |
| `UVTBEditorTransformGizmoBuilder` | 런타임 기즈모 생성과 stock Combined actor factory의 활성 핸들 구성 전달 |
| `AVTBEditorTransformGizmoActor` | 프로젝트별 핸들 커스터마이징을 위한 확장 지점 |
| `UVTBEditorTransformGizmoBehavior` | stock 드래그의 Alt/Ctrl/Shift 입력 정책 |

`DirectionalLightGizmo`의 Builder/Actor/Gizmo/Behavior 역할 구분을 적용합니다. Builder와 Actor는 기존 Combined 기즈모 생성 흐름을 구성하고, 기즈모 본체는 런타임 `UCombinedTransformGizmo`를 상속해 TRS 계산, 좌표계, 스냅, TransformProxy와 런타임 Undo 연결을 재사용합니다. Behavior는 stock Axis/Plane/Angle 서브 기즈모의 드래그 정책만 구성합니다. `LightGizmos`, `UnrealEd`, `EditorInteractiveToolsFramework` 모듈은 사용하지 않습니다.

현재는 `FCombinedTransformGizmoActorFactory`가 제공하는 stock `ACombinedTransformGizmoActor`를 사용합니다. `AVTBEditorTransformGizmoActor`는 이후 프로젝트 전용 factory가 spawn해 핸들을 교체할 수 있는 확장 지점으로 남겨두었습니다. Axis/Plane/Angle 서브 기즈모 자체는 기존 ITF stock builder를 사용하고, 별도 Behavior 객체가 각 서브 기즈모의 기존 드래그 정책을 구성합니다. 기본 ITF Router가 캡처와 hover를 처리합니다.

`Subsystem`은 선택 요청만 pending 상태로 보관합니다. 입력/Undo/취소 콜백 중 들어온 선택 요청은 Context의 update guard가 풀린 다음 Tick에서 적용됩니다. Context는 Gizmo Builder와 생성된 Gizmo를 보유하고, Gizmo는 선택·Proxy·target/handle 구성을 보유합니다. Mode, CoordinateSystem, PlayerController, ActorClass 요청 상태는 Subsystem에 보관하지 않습니다.

XRCreative `TransformInteraction`처럼 선택 수에 따라 생성할 sub-gizmo를 조정합니다. 단일 선택은 축/평면/균일 scale handle을 만들고, 다중 선택은 non-uniform scale을 피하기 위해 uniform scale handle만 만듭니다. non-uniform scale은 Unreal Editor와 같은 Local 좌표계에서만 표시하며 World 좌표계에서는 균일 scale만 표시합니다. Scale 모드는 항상 Local 좌표계를 사용하고 Space 입력은 무시합니다. scale plane은 stock ITF의 Combined `120` / Scale `75` 오프셋과 회전 basis를 사용하고, 메시는 `GizmoComponentMaterial_NotDimmed` 기반 동적 머티리얼을 사용합니다. 선택 자체는 XRCreative `SelectionInteraction`의 click behavior를 가져오지 않고 기존 `IVTBSelectionSource` 경로로 유지합니다.

선택과 Proxy 관리가 기즈모에 통합되어 `UVTBEditorTargetSelection`은 제거했습니다. `UVTBEditorTransformGizmo` 하나를 세션 동안 유지하고 stock modifier 정책을 사용합니다. 선택 수에 따라 시각 Actor와 핸들만 재구성하며 기즈모 객체는 유지합니다. 빈 선택은 Proxy를 해제하고 숨기며, `NoGizmo`는 다음 subsystem Tick에서 활성 연결만 해제하고 선택·Proxy를 보존합니다. 기존 Proxy를 보관한 Undo/Redo는 계속 원래 대상에 적용됩니다.

폴더 구조는 TransformGizmo 상위 개념 아래에서 역할별로 나눴습니다.

```text
Source/simple_proj/
├─ VTBEditorGameMode.h / .cpp
├─ VTBEditorSpectatorPawn.h / .cpp
└─ RuntimeEditor/
   ├─ VTBEditorSubsystem.h / .cpp
   ├─ Context/
   │  ├─ VTBEditorInteractiveToolsContext.h / .cpp
   │  └─ Private/VTBEditorTransactionsAPI.h / .cpp
   ├─ Gizmo/
   │  ├─ VTBEditorTransformGizmo.h
   │  ├─ VTBEditorTransformGizmo.cpp
   │  ├─ VTBEditorTransformGizmoActor.cpp
   │  └─ VTBEditorTransformGizmoBehavior.cpp
   ├─ Selection/VTBSelectionSource.h
   └─ Tests/
      ├─ VTBEditorGizmoTests.cpp
      ├─ VTBEditorInputTests.cpp
      ├─ VTBEditorSelectionTests.cpp
      ├─ VTBEditorSubsystemTests.cpp
      └─ VTBEditorTransactionsTests.cpp
Content/RuntimeEditor/Input/
├─ IMC_VTBEditor.uasset
├─ IA_EditSelect.uasset
├─ IA_EditTranslation.uasset
├─ IA_EditRotation.uasset
├─ IA_EditScale.uasset
├─ IA_ChagneGizmoMode.uasset
├─ IA_EditSelectCancel.uasset
└─ IA_EditSpace.uasset
```

## 변경된 API

| 이전 | 현재 |
|---|---|
| `FVTBEditorSettings` + `Configure(Settings)` | 제거. Subsystem은 설정 façade를 소유하지 않습니다. |
| `ApplySelection(...)` | `ReceiveSelection(...)` |
| `SubmitPointerEvent(...)` | 제거. 입력 전달은 별도 runtime editor façade에서 구성합니다. |
| `CancelInteraction()` | 제거. Context 취소 경로를 직접 검증합니다. |
| `Undo()` / `Redo()` | 제거. Context transaction API를 직접 검증합니다. |
| `GetSettings()` | 제거. Subsystem은 설정 상태를 갖지 않습니다. |
| `GetRuntimeContext()` | Pawn의 pointer 입력과 capture 조회를 위해 추가. Context가 제공하는 좁은 runtime API만 사용합니다. |
| `UVTBEditorInteractiveToolsContext::GetGizmoViewContext()` / `ClearHistory()` / `SetSnappingSettings()` | 제거. 필요한 경우 Context 내부 책임으로 추가합니다. |
| `UVTBEditorTargetSelection` / `UVTBEditorTargetAdapter` | 제거. 선택 검증·Proxy 관리는 `UVTBEditorTransformGizmo`가 담당하며 `SetSelection(World, {})`로 선택을 해제합니다. |
| `AVTBEditorGameMode::GetSelectedActors()` | 제거. 선택 조회는 `IVTBSelectionSource::GetSelectionSnapshot()` 계약으로 통일합니다. |

## 검증

최종 검증에서 UE 5.7.4 Editor 빌드와 런타임 GameWorld 시작을 통과했습니다. 프로젝트 소스에는 자동화 테스트 등록이 없어 새 테스트는 추가하지 않았고, 엔진 자동화 테스트 `StringPrefixTree.Insert` 실행도 성공했습니다.
