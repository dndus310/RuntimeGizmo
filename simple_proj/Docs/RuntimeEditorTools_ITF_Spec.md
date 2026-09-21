# RuntimeEditorTools ITF Runtime Gizmo Spec

> 2026-09-15: RuntimeEditor 구현은 `Plugins/VTBRuntimeEditor/Source/VTBRuntimeEditor`로 이동했습니다. 설치·지원 범위는 `Plugins/VTBRuntimeEditor/README.md`를 기준으로 합니다. 아래 프로젝트 소스 배치와 과거 검증 결과는 이전 버전 기록입니다.

문서 버전: v0.3
대상 프로젝트: `simple_proj`, Unreal Engine 5.7.4

## 목표

런타임에서 선택된 액터의 movable root component를 `InteractiveToolsFramework`의 `UTransformProxy`와 `UCombinedTransformGizmo`에 연결한다. 현재 범위는 Transform Gizmo 중심이며, 별도 Tool registry, Render bridge, Input bridge, Gizmo manager wrapper는 두지 않는다. 입력은 Enhanced Input asset과 `AVTBEditorSpectatorPawn`에서 처리하고, pointer event는 stock `UInputRouter`로 전달한다.

## 구조

| 타입 | 책임 |
|---|---|
| `UVTBEditorSubsystem` | 월드 단위 세션, 선택 소스 구독, pending 선택 전달, Context view/tick 순서와 세션 종료 |
| `UVTBEditorInteractiveToolsContext` | stock `UInputRouter`, `UInteractiveGizmoManager`, `UInteractiveToolManager`, `UToolTargetManager`, `UContextObjectStore` 수명, runtime Query/Transaction 연결, Builder/Gizmo 생성 |
| `UVTBEditorTransformGizmo` | 약한 액터 선택, movable root 검증, `UTransformProxy` 생성·갱신, target/handle 상태 및 runtime behavior/시각 Actor 재구성 |
| `UVTBEditorTransformGizmoBuilder` | 기존 `UCombinedTransformGizmoBuilder` 기반 기즈모 생성과 stock Combined actor 구성 |
| `AVTBEditorTransformGizmoActor` | 프로젝트별 핸들 커스터마이징 확장 지점 |
| `UVTBEditorTransformGizmoBehavior` | Axis/Plane/Angle stock 서브 기즈모의 드래그 modifier 정책 |
| `AVTBEditorGameMode` / `IVTBSelectionSource` | 선택 스냅샷 공급 예시 |
| `AVTBEditorSpectatorPawn` | Enhanced Input 바인딩, pointer state 생성, Router 전달, TRS/Local-World 입력 처리 |

## Subsystem API

```cpp
bool BindSelectionSource(UObject* Source);
void ReceiveSelection(const TArray<TWeakObjectPtr<AActor>>& Actors);
```

Subsystem은 테스트 전용으로 쓰이던 mode/input/cancel/undo/redo façade를 제공하지 않는다. `FVTBEditorSettings` 같은 공개 설정 구조체도 사용하지 않는다. Mode와 CoordinateSystem은 `UVTBEditorInteractiveToolsContext`의 Query 구현이 소유하고, Subsystem은 선택 변경을 pending으로 보관해 Context와 Gizmo에 전달한 뒤 view/tick 순서만 조정한다.

## 런타임 규칙

- Dedicated server에서는 subsystem을 생성하지 않는다.
- Game/PIE world만 지원한다.
- GameMode가 `IVTBSelectionSource`를 구현하면 BeginPlay에서 자동 바인딩한다.
- GameMode가 없는 월드에서는 직접 전달된 선택을 유지한다.
- Context Query가 `NoGizmo`를 반환하면 다음 subsystem Tick에서 활성 대상과 입력 핸들을 해제하고 숨긴다. 선택과 proxy를 소유한 gizmo 객체는 유지하며, 모드 복귀 시 다시 연결한다.
- 빈 선택은 proxy와 활성 대상을 해제한다. gizmo 객체는 세션 종료까지 유지한다.
- Context는 하나의 `UVTBEditorTransformGizmo`만 생성하고 세션 동안 유지한다. Subsystem은 소스 전환이나 Gizmo 생성 상태를 보관하지 않으며 stock modifier 정책을 사용한다.
- 다중 선택에서는 XRCreative `TransformInteraction`과 같은 방향으로 non-uniform scale handle을 만들지 않고 uniform scale만 제공한다. 단일 선택도 World 좌표계에서는 Unreal Editor와 같이 uniform scale만 표시하고, Local 좌표계에서 non-uniform scale을 표시한다. Scale 모드는 항상 Local 좌표계를 사용한다.
- 기본 GizmoMode는 `Translation`이다.
- W/E/R은 각각 Translation/Rotation/Scale 모드를 지정하고, T는 TRS를 순환한다.
- 선택된 모드의 handle만 표시한다. 회전축 드래그 중에는 선택된 회전축 하나와 나머지 회전축 표시를 분리해 에디터 기즈모처럼 표시한다.
- Local/World 전환은 Context Query의 `CoordinateSystem`을 변경하고, stock `UCombinedTransformGizmo`가 모든 축의 frame을 갱신한다. Scale 모드에서 World 요청이 들어오면 Context가 Local로 고정한다. stock factory에서 빠지는 full axis scale mesh에도 동일한 좌표계 갱신을 전달한다.

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
   │  ├─ VTBEditorTransformGizmo.h
   │  ├─ VTBEditorTransformGizmo.cpp
   │  ├─ VTBEditorTransformGizmoActor.cpp
   │  └─ VTBEditorTransformGizmoBehavior.cpp
   ├─ Selection/VTBSelectionSource.h
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
- manager/Gizmo 조합을 노출하는 내부 façade
- Subsystem의 입력, 모드 변경, 취소, Undo/Redo façade

## 검증

최종 검증에서 UE 5.7.4 Editor 빌드와 런타임 GameWorld 시작을 통과했다. 프로젝트 소스에는 자동화 테스트 등록이 없어 새 테스트는 추가하지 않았고, 엔진 자동화 테스트 `StringPrefixTree.Insert` 실행도 성공했다.
