# RuntimeEditorTools ITF Runtime Gizmo Spec

문서 버전: v0.3  
대상 프로젝트: `simple_proj`, Unreal Engine 5.7.4

## 목표

런타임에서 선택된 액터의 movable root component를 `InteractiveToolsFramework`의 `UTransformProxy`와 `UCombinedTransformGizmo`에 연결한다. 현재 범위는 Transform Gizmo 중심이며, 별도 Tool registry, Render bridge, Input bridge, Gizmo manager wrapper는 두지 않는다. 입력은 Enhanced Input asset과 `AVTBEditorSpectatorPawn`에서 처리하고, pointer event는 stock `UInputRouter`로 전달한다.

## 구조

| 타입 | 책임 |
|---|---|
| `UVTBEditorSubsystem` | 월드 단위 세션, 선택 소스 구독, target adapter refresh, gizmo 생성/해제 |
| `UVTBEditorInteractiveToolsContext` | stock `UInputRouter`, `UInteractiveGizmoManager`, `UInteractiveToolManager`, `UToolTargetManager`, `UContextObjectStore` 수명과 runtime Query/Transaction 연결 |
| `UVTBEditorTargetAdapter` | 약한 액터 선택을 검증하고 `UTransformProxy`를 생성 또는 갱신 |
| `UVTBEditorTransformGizmo` | stock combined gizmo에 runtime behavior 정책을 적용 |
| `UVTBEditorTransformGizmoBuilder` | custom gizmo와 visual actor를 생성 |
| `AVTBEditorTransformGizmoActor` | stock ITF handle component 배치와 외형 설정 |
| `AVTBEditorGameMode` / `IVTBSelectionSource` | 선택 스냅샷 공급 예시 |
| `AVTBEditorSpectatorPawn` | Enhanced Input 바인딩, pointer state 생성, Router 전달, TRS/Local-World 입력 처리 |

## Subsystem API

```cpp
bool BindSelectionSource(UObject* Source);
void ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors);
```

Subsystem은 테스트 전용으로 쓰이던 mode/input/cancel/undo/redo façade를 제공하지 않는다. `FVTBEditorSettings` 같은 공개 설정 구조체도 사용하지 않는다. Mode와 CoordinateSystem은 `UVTBEditorInteractiveToolsContext`의 Query 구현이 소유하고, 현재 경량화 범위에서 Subsystem은 선택 변경과 target 연결만 처리한다.

## 런타임 규칙

- Dedicated server에서는 subsystem을 생성하지 않는다.
- Game/PIE world만 지원한다.
- GameMode가 `IVTBSelectionSource`를 구현하면 BeginPlay에서 자동 바인딩한다.
- GameMode가 없는 월드에서는 직접 전달된 선택을 유지한다.
- Context Query가 `NoGizmo`를 반환하면 gizmo를 숨기고 파괴하지만 선택은 유지한다.
- 다중 선택에서는 XRCreative `TransformInteraction`과 같은 방향으로 non-uniform scale handle을 만들지 않고 uniform scale만 제공한다.
- 기본 GizmoMode는 `Translation`이다.
- W/E/R은 각각 Translation/Rotation/Scale 모드를 지정하고, T는 TRS를 순환한다.
- 선택된 모드의 handle만 표시한다. 회전축 드래그 중에는 선택된 회전축 하나와 나머지 회전축 표시를 분리해 에디터 기즈모처럼 표시한다.
- Local/World 전환은 Context Query의 `CoordinateSystem`을 변경하고, stock `UCombinedTransformGizmo`가 모든 축의 frame을 갱신한다.

## 파일 배치

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
   │  ├─ VTBEditorTransformGizmo.h / .cpp
   │  └─ VTBEditorTransformGizmoActor.cpp
   ├─ Selection/VTBEditorTargetAdapter.h / .cpp
   └─ Tests/

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

## 제외

- 범용 runtime tool registry
- 별도 input/router bridge 클래스
- 별도 gizmo manager wrapper
- editor-only `UnrealEd`, `EditorInteractiveToolsFramework`, `GEditor` transaction 의존성
- Blueprint 노출용 subsystem 함수
- 테스트 전용 내부 객체 getter
- Subsystem의 입력, 모드 변경, 취소, Undo/Redo façade

## 검증

`simple_projEditor`와 `simple_proj` Win64 Development 빌드가 통과해야 한다. 자동화 테스트는 `Automation RunTests VTB.RuntimeGizmo`로 실행하며, 현재 테스트는 selection filtering, gizmo composition/input, input asset mapping, mode visibility, rotation axis focus, transaction rollback/history, subsystem reentrancy, 다중 선택 scale handle 정책을 검증한다.
