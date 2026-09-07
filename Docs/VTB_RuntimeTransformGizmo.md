# VTB 런타임 Transform Gizmo

대상: Unreal Engine 5.7.4. 게임 측 C++ 코드가 `UVTBEditorSubsystem`에 선택을 전달하고 실제 Gizmo 구성은 InteractiveToolsFramework 기본 Manager와 Router를 사용합니다. Enhanced Input 자산은 `/Game/RuntimeEditor/Input`에 있고, 기본 `AVTBEditorSpectatorPawn`이 입력을 Context/Router로 전달합니다.

## Subsystem API

`UVTBEditorSubsystem`은 런타임 선택 세션의 입구 역할만 합니다. 테스트나 디버깅을 위해 내부 Context/Gizmo를 반환하던 함수, mode/input/cancel/undo/redo façade, `FVTBEditorSettings` 공개 타입은 두지 않습니다.

| API | 역할 |
|---|---|
| `BindSelectionSource(Source)` | `IVTBSelectionSource`를 구현한 GameMode, Controller, UObject를 구독합니다. `nullptr`이면 구독 해제와 선택 해제를 함께 수행합니다. |
| `ReceiveSelection(WeakActors)` | 선택 소스 없이 선택된 액터를 직접 전달합니다. |

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

월드 BeginPlay에서 GameMode가 있으면 자동으로 `IVTBSelectionSource` 바인딩을 시도합니다. GameMode 없는 클라이언트 월드는 이미 전달된 선택을 유지합니다. `IVTBSelectionSource`와 예시 `AVTBEditorGameMode`는 [VTBEditorGameMode.h](/C:/Users/jkyii/Desktop/simple_proj/Source/simple_proj/VTBEditorGameMode.h:1)에 함께 있습니다.

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
| `IA_EditSpace` | Ctrl + ` | Local/World 전환. IMC 키는 `Tilde`이고 핸들러에서 Ctrl modifier를 검증합니다. |

## 내부 책임

| 구성 요소 | 책임 |
|---|---|
| `UVTBEditorSubsystem` | 월드 수명, 선택 구독, 선택 지연 적용, Gizmo 타깃 연결 |
| `AVTBEditorSpectatorPawn` | Enhanced Input IMC 설치, pointer event 생성, Router 전달, TRS/Space command 적용 |
| `UVTBEditorInteractiveToolsContext` | 기본 ITF 서비스, LocalPlayer SceneView, Queries/Transactions, 취소와 Undo/Redo |
| `FVTBEditorQueriesAPI` | Context cpp 내부의 월드, 선택, 카메라, 모드, 좌표계 조회 |
| `FVTBEditorTransactionsAPI` | 런타임 전용 그룹 Undo/Redo, 취소 rollback, TransformProxy GC 보존 |
| `UVTBEditorTargetAdapter` | 약한 액터 선택을 Movable root component와 `UTransformProxy`로 변환 |
| `UVTBEditorTransformGizmo` / Builder / Actor | 커스텀 Actor 생성, 핸들 외형, 기본 ITF behavior 정책 |

`Subsystem`은 선택 요청만 pending 상태로 보관합니다. 입력/Undo/취소 콜백 중 들어온 선택 요청은 다음 Tick에서 한 번에 적용됩니다. Mode, CoordinateSystem, PlayerController, ActorClass 요청 상태는 Subsystem에 보관하지 않습니다.

XRCreative `TransformInteraction`처럼 선택 수에 따라 생성할 sub-gizmo를 조정합니다. 단일 선택은 축/평면/균일 scale handle을 모두 만들고, 다중 선택은 non-uniform scale을 피하기 위해 uniform scale handle만 만듭니다. 선택 자체는 XRCreative `SelectionInteraction`의 click behavior를 가져오지 않고 기존 `IVTBSelectionSource` 경로로 유지합니다.

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
   │  ├─ VTBEditorTransformGizmo.h / .cpp
   │  └─ VTBEditorTransformGizmoActor.cpp
   ├─ Selection/VTBEditorTargetAdapter.h / .cpp
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
| `GetToolsContext()` / `GetTransformGizmo()` / `HasActiveMouseCapture()` | 제거. 운영 API에서 내부 ITF 객체와 디버깅 상태를 노출하지 않습니다. |
| `UVTBEditorInteractiveToolsContext::GetGizmoViewContext()` / `ClearHistory()` / `SetSnappingSettings()` | 제거. 필요한 경우 Context 내부 책임으로 추가합니다. |
| `UVTBEditorTargetAdapter::ClearSelection()` | 제거. `SetSelection(World, {})`로 선택 해제를 표현합니다. |
| `AVTBEditorGameMode::GetSelectedActors()` | 제거. 선택 조회는 `IVTBSelectionSource::GetSelectionSnapshot()` 계약으로 통일합니다. |

## 검증

이번 정리 후 Win64 Development Editor/Game 빌드를 모두 통과했습니다. `VTB.RuntimeGizmo` 자동화 테스트는 18개 모두 성공했고 실패/미실행은 0개입니다. 독립 테스트 월드 종료 과정에서 발생하는 `World has no context` 경고는 기존과 같은 테스트 환경 경고입니다. 로그에는 기존 `Content/NewMap.umap` Blueprint 컴파일 에러가 함께 출력되지만, `VTB.RuntimeGizmo` 리포트 실패로 집계되지는 않습니다.

```powershell
& 'C:/Program Files/Epic Games/UE_5.7/Engine/Build/BatchFiles/Build.bat' simple_projEditor Win64 Development '-Project=C:/Users/jkyii/Desktop/simple_proj/simple_proj.uproject' -WaitMutex -NoHotReloadFromIDE
& 'C:/Program Files/Epic Games/UE_5.7/Engine/Build/BatchFiles/Build.bat' simple_proj Win64 Development '-Project=C:/Users/jkyii/Desktop/simple_proj/simple_proj.uproject' -WaitMutex
& 'C:/Program Files/Epic Games/UE_5.7/Engine/Binaries/Win64/UnrealEditor-Cmd.exe' 'C:/Users/jkyii/Desktop/simple_proj/simple_proj.uproject' -unattended -nop4 -nosplash -nosound -NullRHI '-ExecCmds=Automation RunTests VTB.RuntimeGizmo' '-TestExit=Automation Test Queue Empty' '-ReportExportPath=C:/Users/jkyii/Desktop/simple_proj/Saved/Automation/VTBRuntimeGizmoInputMapping3' '-abslog=C:/Users/jkyii/Desktop/simple_proj/Saved/Logs/VTBRuntimeGizmoInputMapping3Tests.log'
```
