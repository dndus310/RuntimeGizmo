# VTBOWT 런타임 편집

`VTBRuntimeEditor` 플러그인은 비활성화되어 있습니다. 실제 구현은 두 Runtime 모듈인
`Source/VTBOWTEditor`와 `Source/simple_proj`에만 있습니다.

## 역할과 확장 지점

```text
simple_proj (입력·카메라·게임별 선택 정책·데모)
  └─ IVTBOWTEditorSelectionSource / Context 서비스 인터페이스
       └─ VTBOWTEditor (재사용 가능한 ITF 실행 환경)
            ├─ ModeSubsystem : UTickableWorldSubsystem
            ├─ ToolsContext : UInteractiveToolsContext
            │    ├─ cpp 내부 Queries / Transactions / Render 구현
            │    ├─ ContextInput : 포인터 입력·캡처 종료·포커스 구독
            │    ├─ ContextViewport : 뷰 갱신·Render 호출·렌더 카운터
            │    ├─ SceneState : Queries의 선택·좌표계·모드 갱신
            │    └─ TransactionHistory : 변경 기록·Undo/Redo·취소 rollback
            ├─ GizmoManager : UInteractiveGizmoManager
            ├─ RepositionalGizmo : URepositionableTransformGizmo
            │    ├─ GizmoSelection : 선택·TransformProxy 관리
            │    ├─ GizmoInteraction : 입력 조건·상호작용 강조 상태
            │    └─ RepositionalGizmoBuilder.cpp : Gizmo·Actor Factory 구성
            └─ CombinedTransformGizmoActor : 엔진 기즈모 액터
                 └─ GizmoVisualComponent : 재질·표시·배치
```

기존 엔진 상속 계층을 유지합니다. ToolsContext는 ITF Router·Manager·ContextObjectStore의
생성·연결·Tick·종료를 조율합니다. 입력·뷰·선택·Undo/Redo 작업은 `GetInput()`,
`GetViewport()`, `GetSceneState()`, `GetUndoRedo()`가 반환하는 담당 인터페이스로 실행합니다.
Context에는 이 작업을 다시 전달하는 공개 함수를 두지 않습니다.

세 가지 ITF API 구현은 cpp에 숨기고 각각 `TUniquePtr`로 소유하며,
`IToolsContextQueriesAPI`, `IToolsContextTransactionsAPI`, `IToolsContextRenderAPI`로 노출합니다.
SceneState는 QueriesImpl의 기존 상태를 갱신하며 별도 선택 목록을 저장하지 않습니다.
TransactionsImpl은 ITF의 기록 요청을 단일 TransactionHistory에 전달합니다.
외부 Transaction API를 주입하면 그쪽에만 기록하고 로컬 이력을 중복 생성하지 않습니다.

ModeSubsystem이 월드마다 Context 하나를 만들고 초기화·Tick·종료합니다.
`CreateToolsContext()`에서 엔진의 `SetCreateGizmoManagerFunc()`로 매니저 구성을 지정합니다.
기본 팩토리는 VTBOWTEditorGizmoManager를 연결합니다.
ModeSubsystem은 뷰를 갱신할 PlayerController를 고릅니다. Viewport 인터페이스에는
명시한 플레이어를 사용하는 `UpdateView(PlayerController)`만 있습니다.
`SetActorSelection(Actors)`는 루트 컴포넌트를 수집하고,
`SetSelection(Actors, Components)`는 빈 목록을 포함해 명시한 컴포넌트 목록을 그대로 사용합니다.

GizmoManager가 선택 기즈모 생성과 대상 동기화를 담당합니다.
RepositionalGizmo가 소유하는 GizmoSelection UObject는 선택 컴포넌트와 TransformProxy를 관리하고,
GizmoInteraction 값 멤버는 입력 조건과 상호작용 강조 상태를 관리합니다.
재질·배치·표시는 실제 `ACombinedTransformGizmoActor`가 소유하는 GizmoVisualComponent가 맡습니다.
기본 Actor Factory가 이 컴포넌트를 설치하며, 외부 Factory가 주입되면 반환된 Actor에 연결합니다.
RepositionalGizmo는 `Super::Tick()` 이후 모드·좌표계·활성 핸들 정보를 컴포넌트에 전달합니다.

Context는 Input·Viewport·SceneState·TransactionHistory를 `TUniquePtr`로 소유합니다.
이 서비스는 Context 전체 수명 동안 유지되며 Shutdown에서는 세션 데이터를 정리합니다.
따라서 종료·재초기화 후에도 같은 서비스 참조를 사용할 수 있습니다.
입력 처리 중 여부와 포커스 구독은 Input이, 렌더 카운터는 Viewport가,
트랜잭션 기록·커서·중첩 깊이·취소·재생 상태는 History가 관리합니다.
Input은 캡처 종료와 History rollback을 호출하고,
Context의 `RunContextUpdate()`는 콜백 실행 중 종료와 재진입을 제어합니다.

게임별 선택 정책은 `IVTBOWTEditorSelectionSource`로 제공합니다.
Subsystem은 특정 GameMode 클래스에 의존하지 않으며, 인터페이스를 구현한 객체를
`BindSelectionSource()`로 연결할 수 있습니다. BeginPlay에는 이 인터페이스를 구현한
현재 GameMode를 자동으로 연결합니다. simple_proj의 VTBEditorGameMode는 컴포넌트 선택 요청에서
소유 액터와 기준 컴포넌트를 함께 보존하고 선택 변경 이벤트를 내보냅니다.

## 실행 흐름

```text
SpectatorPawn → Context.GetInput().PostPointerInput → InputRouter → Gizmo → TransformProxy → 액터
GameMode 선택 이벤트 → ModeSubsystem → GizmoManager → Context.GetSceneState() → Queries
ITF 선택 변경 요청 → Transactions → Subsystem → SelectionSource → GameMode 정책
ITF 변경 기록 → Transactions → TransactionHistory ← Context.GetUndoRedo()
ModeSubsystem → Context.GetViewport().UpdateView(PlayerController)
LocalPlayer → SceneView.Drawer → ViewportClient → Context.GetViewport().Render(View, PDI)
RepositionalGizmo.Tick → CombinedTransformGizmoActor.GizmoVisualComponent
```

입력은 Pawn 한 곳에서 전달합니다. 렌더 API의 View/PDI 포인터는 Draw 호출 동안만 보관합니다.
입력·렌더·Tick 콜백 도중 Shutdown이 요청되면 호출이 끝난 뒤 정리합니다.
선택 변경도 같은 방식으로 안전한 시점에 적용합니다.

Context는 초기화 중/완료/종료 중을 별도 상태로 저장하지 않습니다.
매니저 생성이 모두 끝난 후 Render API를 생성하고, 종료를 시작할 때 먼저 해제합니다.
따라서 `IsRuntimeReady()`는 이 API의 존재와 대기 중인 종료 요청만 확인합니다.
매니저의 수명은 Context 소유권으로 보장하므로 준비 상태를 조회할 때마다 다시 검사하지 않습니다.
Context는 실행 중 콜백 보호와 종료 요청의 두 플래그만 유지합니다.
입력 캡처 처리 보호는 Input이, 취소·재생 상태는 History가 관리합니다.

`Selection`의 타입 별칭 `FVTBOWTActorSelection`은 약한 액터 참조 배열입니다.
Optional이 비어 있으면 새 요청이 없고, 값이 있는 빈 배열이면 선택 해제 요청입니다.

`InitializeContext(GameWorld)` 또는 기본 `Initialize()`는 자체 런타임 Undo/Redo를 제공합니다.
최대 128개의 트랜잭션을 보관하며, 진행 중 드래그를 취소하면 시작 상태로 되돌립니다.
`Initialize(Queries, Transactions)`에 외부 API를 전달하면 소유한 어댑터가 해당 API로 위임합니다.
외부 트랜잭션 API에는 Undo/Redo 실행 인터페이스가 없으므로 이 경우 기록 재생은 외부 호스트가 담당합니다.

## 설정과 조작

- 두 Build.cs 모두 VTBRuntimeEditor 의존성이 없습니다.
- uproject에서 해당 플러그인을 명시적으로 비활성화했습니다.
- DefaultEngine.ini에서 VTBOWTEditor의 LocalPlayer와 ViewportClient를 사용합니다.
- DefaultGame.ini에서 ITF 자산, 입력 자산, 데모 BasicShapes를 AlwaysCook에 포함합니다.

조작:

- 왼쪽 클릭: 액터 선택. 기즈모 핸들 위에서는 드래그를 우선 처리합니다.
- W / E / R: 이동 / 회전 / 크기. T: 모드 순환.
- Ctrl + ~: World / Local 전환. 크기 조절은 Local 좌표계를 사용합니다.
- Ctrl + Z / Ctrl + Y: Undo / Redo.
- Esc: 진행 중 조작 취소 및 선택 해제.
- 오른쪽 버튼 + 마우스 / WASD / QE: 카메라 회전 / 이동 / 위아래 이동.

이동 가능한(Movable) 루트 컴포넌트가 있는 액터를 변환할 수 있습니다.
변환은 현재 실행 중인 월드에 적용됩니다. 디스크 저장이나 네트워크 복제는 별도 기능입니다.
엔진 Repositionable 계층의 피벗 기능은 유지하지만, 샘플 Pawn의 입력 경로는 왼쪽 버튼 변환을 연결합니다.

## Shipping 데모

패키지의 simple_proj.exe에 다음 인자를 전달합니다.

```text
-VTBRuntimeDemo -windowed -ResX=1280 -ResY=720
```

별도 데모 umap은 없습니다. 기본 `/Engine/Maps/Templates/OpenWorld` 맵에
simple_proj의 데모 코드가 두 Actor와 바닥을 추가하고 카메라를 배치합니다.
각 Actor는 SplineRoot 아래에 상대 위치·회전이 있는 TargetMesh와 작은 SiblingMarker를 둡니다.
큰 큐브 또는 작은 메시를 선택하면 그 컴포넌트의 축으로 Actor 전체를 변환합니다.
HUD에 Actor TRS, 선택 컴포넌트의 World/Relative TRS, Context Render 호출 횟수와
Undo/Redo 가능 여부가 표시됩니다. 드래그해도 Relative TRS는 유지됩니다.
옵션을 생략하면 데모 액터와 HUD를 생성하지 않습니다.

## 자동 검증

Automation 필터: `VTBOWTEditor.ToolsContext`

- LifecycleAndQueries: API 수명과 초기화/종료.
- TransactionBackend: 외부 API 위임과 기본 자체 Undo/Redo.
- RuntimeSelectionTransformUndo: 선택, 변환, Undo/Redo, 재초기화.
- RuntimePointerDragAndDeferredShutdown: InputRouter 드래그, 콜백 중 종료·롤백, 포커스 상실 시 취소와 재초기화 후 구독 정리.
- RuntimeReentrantCancellation: 드래그 시작·종료 콜백에서 취소 요청, 캡처 해제와 후속 Undo/Redo.
- ShutdownDuringInitialization: 관리자 생성 콜백 중 종료 요청을 생성 완료 후 처리하고 재초기화.
- HostSelectionIntegration: simple_proj의 선택 정책, 인터페이스 연결, 선택 요청 지연, 호스트 분리.
- EmptyExpiredAndBranchedHistory: 빈 트랜잭션, 만료된 변경 건너뛰기, 새 편집으로 Redo 분기 교체.
- SelectionRefreshAfterTargetRemoval: 선택 중복 제거·순서 유지, 액터와 루트 컴포넌트 파괴 후 갱신.
- SelectionDuringCancellationAndShutdown: 드래그 취소 콜백에서 새 선택 요청·종료, 최신 선택 우선 적용과 롤백.
- GizmoSelectionOwnershipAndAttachment: GC 이후 선택·proxy 수명, 부모·자식 중복 변환 방지, 재선택·재구성과 Context별 선택 격리.
- GizmoHoverInteractionState: 실제 HitTarget 콜백의 드래그 강조 유지, 해제 후 강조 억제, 포인터 재진입 시 강조 복원.

포인터 자동화 테스트는 GPU 없는 환경에서 핸들 hit target만 대체합니다.
실제 InputRouter, 축 기즈모, TransformProxy와 트랜잭션은 사용합니다.

## 플러그인 제거 시 검증 결과 (2026-09-22)

- Editor Win64 Development, Game Win64 Development 빌드 성공.
- Shipping Win64 Build / Cook / Stage / Pak / IoStore / Archive 성공.
- 자동화 테스트 6개 통과, 테스트 경고 0, 오류 0.
- 세 타깃의 빌드 결과 목록과 Shipping 배포 목록에서 VTBRuntimeEditor 의존성 0개 확인.
- 새 Shipping 실행 창에서 Context Ready, Render 호출, 선택 기즈모와 이동·회전 값 변경 확인.
- 실행 창에서는 사용자 조작이 이어져 추가 자동 입력을 중단했습니다.
  UI의 Undo/Redo 전후 상태는 분리해서 검증하지 않았으며, Undo/Redo 결과는 자동화 테스트에 근거합니다.

최종 패키지: `Packaged/VTBOWTStandaloneShipping/Windows`.
`RunRuntimeDemo.cmd`로 데모를 실행합니다.
이전 `VTBOWTRuntimeShipping` 폴더는 이번 수정의 최종 패키지가 아닙니다.

자동화 보고서: `Saved/Automation/VTBOWTStandalone/index.json`.
패키징 로그: `Saved/Logs/VTBOWTStandalonePackaging.log`.

## 상태 단순화 후 검증 (2026-09-22)

Context의 플래그를 8개에서 3개로 줄인 소스로 Editor Development와 Game Shipping 빌드를 완료했습니다.
자동화 테스트 7개가 경고·오류 없이 통과했습니다.
보고서: `Saved/Automation/VTBOWTContextSimplification/index.json`.

## 중복 조건 정리 후 검증 (2026-09-22)

`Source/simple_proj`와 `Source/VTBOWTEditor` 전체를 점검하고 실행 코드 11개 파일을 수정했습니다.
테스트를 제외한 `if` 분기는 264개에서 246개로 줄었습니다. 이는 코드 구조 비교이며 실행 시간 측정값은 아닙니다.

- 선택 입력의 유효성 검사와 중복 제거는 최종 선택 저장 지점에서 처리합니다.
- Context가 직접 준비 상태를 확인하는 Render·Undo·Redo·Tick 호출부의 중복 검사를 줄였습니다.
- 대기 선택은 `RunContextUpdate`가 업데이트를 수락한 뒤 꺼냅니다. 재진입 요청은 대기 상태로 유지합니다.
- 내부 API 수명과 저장된 Change의 유효성이 이미 보장되는 지점의 검사를 제거했습니다.
- 하위 기즈모 설정·타깃 상태 비교 순회를 합치고, Hover 처리에서 이미 확인한 참조를 재사용합니다.
- 함께 변경되던 카메라 이동·시점 입력 잠금 상태 두 개를 하나로 합쳤습니다.
- 빈 조건문에서만 읽던 ModeSubsystem의 초기화 플래그를 제거했습니다.

외부 입력 검증, 약한 참조의 소멸 확인, 취소·종료 콜백 이후의 상태 재확인은 유지합니다.
선택 소스 바인딩·교체·파괴와 Add/Remove/Replace/Clear의 결과 및 이벤트 횟수도 기존 테스트에 추가했습니다.

검증 결과:

- 수정 전 기존 자동화 테스트 7개 통과.
- 수정 후 기존 7개와 신규 3개, 총 10개 통과. 테스트 경고 0, 오류 0.
- Editor Win64 Development와 Game Win64 Shipping 컴파일·링크 성공.
- Source에 VTBRuntimeEditor 참조, IsInGameThread 어서트, 한 줄로 압축한 본문 없음.

수정 전 보고서: `Saved/Automation/VTBOWTConditionBaseline/index.json`.
수정 후 보고서: `Saved/Automation/VTBOWTConditionAuditFinal/index.json`.
이번 확인은 소스 빌드와 자동화 테스트이며 위의 Shipping 데모 패키지는 다시 생성하지 않았습니다.

## 기즈모·컨텍스트 책임 분리 후 검증 (2026-09-22)

RepositionalGizmo의 선택·TransformProxy 관리는 GizmoSelection으로,
재질·표시·상호작용 강조는 GizmoVisuals로 분리하고 Builder 구현도 별도 cpp로 옮겼습니다.
기존 Gizmo cpp는 엔진 호출과 내부 구성요소 연결을 담당합니다.
ToolsContext의 입력·Tick과 뷰·Render 함수는 각각 Input.cpp와 Viewport.cpp로 옮겼습니다.

세 API 구현 클래스(RenderImpl, QueriesImpl, TransactionImpl)는 수정 전 스냅샷과
문자열 단위로 일치합니다. 공개 함수 시그니처와 기존 상태 플래그도 유지합니다.

- Editor Win64 Development, Game Win64 Shipping 빌드 성공 (`-DisableUnity`).
- 기존 10개와 신규 2개, 총 12개 자동화 테스트 통과. 테스트 보고서의 경고·오류 0.
- 신규 테스트에서 GC 이후 선택 관리 수명, 부모·자식 변환, 독립 Context의 선택 상태와 Hover 전이를 확인했습니다.
- NullRHI 실행이므로 화면의 실제 픽셀 결과는 검증하지 않았습니다. 데모 패키지도 다시 생성하지 않았습니다.

자동화 보고서: `Saved/Automation/VTBOWTGizmoContextSplit/index.json`.
원시 실행 로그에는 테스트 시작 전 `Condition failed` 4건이 있으며, 분리 전 실행 로그에도 동일하게 존재합니다.
위의 경고·오류 수는 해당 12개 테스트의 결과 집계입니다.

## ToolsContext 객체 책임 분리 후 검증 (2026-09-22)

앞선 cpp 분리에서 더 나아가 입력과 뷰 처리를 실제 내부 객체로 분리했습니다.

- `FVTBOWTEditorToolsContextInput`: 포인터 입력, 캡처 취소, 포커스 구독과 입력 처리 중 상태.
- `FVTBOWTEditorToolsContextViewport`: 플레이어 뷰 구성, ITF 렌더 호출과 렌더 카운터.
- `UVTBOWTEditorToolsContext`: 세 API 소유, 초기화·종료·Tick 순서와 업데이트 중 재진입 제어.

기존 공개 함수는 유지하고 내부 객체에 작업을 전달합니다. API 구현 3개는 이번 수정 직전
스냅샷과 줄바꿈까지 일치합니다. 상태 플래그의 전체 개수는 늘리지 않았습니다.

검증 결과:

- Editor Win64 Development, Game Win64 Shipping 빌드 성공 (`-DisableUnity`).
- NullRHI 자동화 테스트 12개 통과. 테스트 보고서의 경고·오류 0.
- 기존 포인터 테스트에 포커스 상실 시 드래그 취소·rollback·Redo 보존 검증을 추가했습니다.
- 종료 후 재초기화와 중복 Initialize 이후에도 취소가 한 번 실행되며, 종료 시 포커스 구독이 제거됨을 확인했습니다.

자동화 보고서: `Saved/Automation/VTBOWTContextResponsibilities/index.json`.
이번 검증은 소스 빌드와 자동화 테스트이며 데모 패키지는 다시 생성하지 않았습니다.

## 엔진 예제를 기준으로 한 상태·기능 경계 (2026-09-22)

설치된 UE 5.7 소스를 직접 비교해 다음 경계를 적용했습니다.

| 엔진에서 참고한 구조 | 이번 구현에 적용한 내용 |
| --- | --- |
| InteractiveToolsFramework의 Context가 Router·Manager 수명을 조율 | Context만 업데이트·종료 상태를 변경하고, 취소 시 입력 캡처 종료와 트랜잭션 rollback 순서를 조율합니다. |
| EditorInteractiveToolsFramework의 BehaviorSource와 Interaction 분리 | 입력 어댑터는 입력 처리 중 상태와 포커스 구독을 소유합니다. Context의 상태를 직접 변경하지 않습니다. |
| TransformMeshesTool의 관련 대상 데이터를 묶는 구조 | 선택 대상의 Component와 캐시 Transform을 `FTargetState` 한 항목으로 묶었습니다. 병렬 배열과 배열 길이 동기화 조건을 제거했습니다. |
| DirectionalLightGizmo의 Builder·입력 Behavior·표시 구성 구분 | `GizmoVisuals`는 재질·표시·배치만 처리하고, `GizmoInteraction`은 입력 조건과 기존 상호작용 강조 상태 두 개를 소유합니다. |

현재 변환 기즈모는 `Super::SetActiveTarget()`이 구성하는 ITF SubGizmo의
ParameterSource·HitTarget·StateTarget을 계속 사용합니다. 새 캡처 상태나 변경 기록 시스템은 추가하지 않습니다.
DirectionalLightGizmo 자체가 ParameterSource·StateTarget 분리 예제인 것은 아니며,
그 예제의 Editor `Modify()` 경로를 런타임에 가져오지도 않았습니다.

ModeSubsystem의 중복 `EditingWorld`, `EditorGizmoManager` 참조는 제거했습니다.
월드는 BeginPlay 인수와 Subsystem에서, 매니저는 Context에서 조회합니다.
세 API Impl과 Context·Gizmo·Manager·Subsystem의 기존 공개 함수는 유지합니다.

참고한 엔진 소스:

- [InteractiveToolsContext 생성·종료](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/InteractiveToolsContext.cpp:120>)
- [ViewportInteractionsBehaviorSource](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/Experimental/EditorInteractiveToolsFramework/Public/ViewportInteractions/ViewportInteractionsBehaviorSource.h:57>)
- [TransformMeshesTool의 설정과 대상 데이터](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/Experimental/MeshModelingToolsetExp/Source/MeshModelingToolsExp/Public/TransformMeshesTool.h:96>)
- [DirectionalLightGizmo의 입력 전달](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/Experimental/GizmoEdMode/Source/LightGizmos/Private/DirectionalLightGizmo.cpp:444>)

EditorInteractiveToolsFramework와 LightGizmos는 설계 참고로 사용했으며 모듈 의존성을 추가하지 않았습니다.

검증 결과:

- Editor Win64 Development와 Game Win64 Shipping 컴파일·링크 성공 (`-DisableUnity`).
- NullRHI 자동화 테스트 12개 통과. 테스트 보고서의 경고·오류 0.
- 기존 테스트에 외부 Transform 변경 후 다중 선택의 동일 이동량 적용, Alt 입력의 캡처·변환·이력 차단 검증을 추가했습니다.
- 기존 Hover 전이, GC 수명, Undo/Redo, 포커스 취소, 종료 중 재진입 테스트도 통과했습니다.
- 세 API Impl은 수정 직전 스냅샷과 원문이 일치하고 주요 공개 함수 선언도 동일합니다.

자동화 보고서: `Saved/Automation/VTBOWTEnginePatternSeparation/index.json`.
이번 확인은 소스 빌드와 자동화 테스트이며 데모 재패키징과 실제 화면 렌더링 검증은 포함하지 않습니다.

## Context 인터페이스와 Gizmo Actor 구성 (2026-09-22)

앞 절의 공개 전달 함수와 `FVTBOWTEditorGizmoVisuals` 구조를 다음과 같이 변경했습니다.

| 담당 객체 | 책임과 호출 경로 |
| --- | --- |
| `UVTBOWTEditorToolsContext` | 기존 ITF Router·Manager·ContextObjectStore 생성과 종료, Tick 순서, 콜백 실행 중 종료 지연 |
| `IVTBOWTEditorInput` | `GetInput()`으로 포인터 전달, 캡처 조회·취소 |
| `IVTBOWTEditorViewport` | `GetViewport()`으로 명시한 PlayerController의 뷰 구성과 Render |
| `IVTBOWTEditorSceneState` | `GetSceneState()`로 기존 QueriesImpl의 선택·좌표계·기즈모 모드 갱신 |
| `IVTBOWTEditorUndoRedo` | `GetUndoRedo()`로 단일 History 객체의 Undo/Redo 실행·가능 여부 조회 |
| `UVTBOWTEditorGizmoVisualComponent` | 실제 `ACombinedTransformGizmoActor`에 등록되어 재질·배치·핸들 가시성 관리 |

Context의 `UpdateView`, `SetSelection`, 입력·뷰·Undo/Redo 전달 함수를 제거했습니다.
플레이어를 고르는 정책은 ModeSubsystem에 두고 뷰 인터페이스에는 `UpdateView(PlayerController)`만 남겼습니다.
선택 인터페이스의 `SetActorSelection(Actors)`는 RootComponent를 자동 수집합니다.
`SetSelection(Actors, Components)`는 명시한 컴포넌트 목록을 그대로 사용하며 빈 목록도 유지합니다.

ITF Render/Queries API 구현은 이번 수정 직전 원문을 유지했습니다.
SceneState는 이 API의 기존 상태를 수정하는 접근 인터페이스이며 별도 선택 목록을 저장하지 않습니다.
Transaction API의 ITF 함수 계약은 유지하고, 내부 기록·중첩 깊이·커서·취소·재생·GC 수집은
`FVTBOWTEditorTransactionHistory` 한 객체로 이동했습니다. 외부 Transaction API가 주입되면
기존처럼 그쪽으로 기록을 전달하며 로컬 History에 중복 기록하지 않습니다.
History와 입력·뷰 서비스는 Context 수명 동안 유지하고 Shutdown에서는 세션 데이터만 정리합니다.
따라서 Undo/Redo 콜백이 Shutdown을 요청해도 실행 중인 History가 파괴되지 않습니다.

`ContextObjectStore`는 도구와 매니저가 공유 객체를 찾는 엔진 저장소로 계속 사용합니다.
예를 들어 Builder와 Viewport는 여기에 등록된 `UGizmoViewContext`를 함께 사용합니다.
Store가 입력 전달·Render·서비스 Shutdown을 자동 실행하지 않으므로,
이러한 host 기능을 등록하려고 별도 UObject 래퍼를 추가하지 않았습니다.

기본 Gizmo Actor Factory는 Visual Component를 설치합니다. 외부 Factory를 주입한 경우에는
그 Factory가 반환한 Actor에 같은 컴포넌트를 연결하므로 Actor와 Mesh를 교체하지 않습니다.
`Super::Tick` 후 현재 모드·좌표계·활성 핸들 정보를 전달하고 컴포넌트에는 같은 상태를 중복 저장하지 않습니다.
Actor를 재생성하면 새 Actor가 자신의 Visual Component를 소유합니다.

참고한 Runtime 엔진 구조:

- [InteractiveToolsContext의 소유 범위](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Public/InteractiveToolsContext.h:30>)
- [ContextObjectStore의 공유 객체 역할](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Public/ContextObjectStore.h:17>)
- [TransformGizmoUtil의 Context 등록](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/TransformGizmoUtil.cpp:35>)

최종 검증 결과:

- Editor Win64 Development와 Game Win64 Shipping 컴파일·링크 성공 (`-DisableUnity`).
- NullRHI 자동화 테스트 14개 통과. 해당 테스트 보고서의 경고·오류 0.
- 기존 드래그·취소·선택·Hover 검증에 중첩 트랜잭션, 선택 해제 후 GC와 Undo/Redo, 외부 backend 이력 분리 검증을 추가했습니다.
- Undo/Redo 콜백에서 재진입과 Shutdown을 요청한 뒤, 같은 서비스 참조로 재초기화해 사용할 수 있음을 확인했습니다.
- 기본·주입 Factory의 Visual Component 단일 등록, Actor·Mesh 보존, 요소 재생성, World/Local 및 모드별 표시를 검증했습니다.
- 좌표 갱신 콜백이 활성 컴포넌트 배열을 변경한 뒤에도 후속 갱신이 최신 배열을 조회하는 회귀 테스트를 포함했습니다.

보고서: `Saved/Automation/VTBOWTContextServices/index.json`.
로그: `Saved/Logs/VTBOWTContextServices.log`.
이번 검증에는 화면 픽셀 비교와 데모 재패키징이 포함되지 않습니다.

## 하위 컴포넌트 축으로 Actor 전체 변환 (2026-09-22)

선택 컴포넌트는 기즈모의 기준 World Transform을 제공하고, 실제 변경 대상은 선택 Actor의 RootComponent입니다.
루트부터 Spline, 선택 메시, 형제·자손 컴포넌트가 ActorTransform 변경처럼 함께 움직이며
각 컴포넌트의 Relative Transform은 바꾸지 않습니다.

게임 호스트에서 다음처럼 지정할 수 있습니다. Blueprint에서도 같은 함수를 호출할 수 있습니다.

```cpp
GameMode->SetSelectedComponent(StaticMeshComponent);
```

클릭 선택은 HitResult의 컴포넌트를 사용합니다. StaticMesh, DynamicMesh, InstancedStaticMesh,
HierarchicalInstancedStaticMesh 모두 같은 USceneComponent 계약으로 처리합니다.
ISM/HISM의 인스턴스별 Local Transform이나 인스턴스 수는 변경하지 않습니다.
개별 인스턴스의 축을 선택하는 기능은 이번 컴포넌트 선택 경로에 포함하지 않습니다.

추상화된 선택 소스는 기존 GetSelection()으로 변환할 Actor 목록을 제공하고,
GetSelectionFrame()으로 선택한 기준 컴포넌트를 제공합니다.
GetSelectionFrame()을 구현하지 않는 기존 소스는 RootComponent 기준으로 동작합니다.
새 기준 컴포넌트를 선택하면 Local 축으로 시작하고, 이후 사용자의 World/Local 전환은 유지합니다.
SetSelectedActors()를 호출하면 컴포넌트 기준을 해제하고 기존 Actor 선택으로 돌아갑니다.

ITF 연결은 MeshModelingTools의 TransformMeshesTool처럼 UTransformProxy::AddComponentCustom을 사용합니다.
기준 컴포넌트의 World TRS를 읽되 setter가 루트의 World TRS를 계산합니다.
중간 Spline의 회전과 비균등 Scale 때문에 계층을 단일 Relative Transform으로 미리 합치지 않고,
각 부착 단계의 변환을 엔진 순서대로 합성하여 기준점의 이동을 보정합니다.
같은 Actor 안에서 기준 컴포넌트만 바뀌어도 Proxy가 갱신됩니다.
기준 컴포넌트가 파괴되거나 계층에서 분리되면 남은 Actor의 Root 기준으로 복귀합니다.
선택된 부모·자식 Actor는 최상위 대상만 변환하여 자식을 두 번 움직이지 않습니다.

세 Context API 구현에는 기능을 추가하지 않았습니다. 기존 ITF TransformProxyChange와
단일 History의 Undo/Redo 경로를 사용합니다.
각 Proxy는 생성 시점의 부착 변환 관계를 보관하므로, 나중에 기준 컴포넌트를 수정하거나 삭제해도
그 Proxy의 이전 Undo/Redo는 기록된 Actor 변환을 복원합니다. 바뀐 계층의 현재 편집은 새 Proxy가 담당합니다.
컴포넌트 기준의 TRS도 Unreal FTransform 표현을 따르며, 메시를 베이크하거나 전단 변형을 만들지 않습니다.
Absolute Transform 채널, 0인 중간 Relative Scale처럼 Root로 제어할 수 없는 채널은 이 동작의 전제에서 제외됩니다.
음수 Scale이 있는 미러 계층은 엔진의 행렬 분해 경로를 사용하므로 이번 양수 Scale 계층 검증 결과를 그대로 적용할 수 없습니다.

참고한 설치 엔진 소스:

- [TransformMeshesTool의 Custom Proxy 등록](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/Experimental/MeshModelingToolsetExp/Source/MeshModelingToolsExp/Private/TransformMeshesTool.cpp:254>)
- [TransformProxy의 사용자 정의 변환과 변경 기록](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/TransformProxy.cpp:42>)
- [SceneComponent의 계층 합성](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/Engine/Private/Components/SceneComponent.cpp:715>)

검증 결과:

- Editor Win64 Development·Game Win64 Shipping 빌드 성공 (`-DisableUnity`).
- 기존 14개와 신규 4개, 총 18개 NullRHI 자동화 테스트 통과. 테스트 경고·오류 0.
- 실제 StaticMesh·DynamicMesh·ISM·HISM 컴포넌트와 회전·비균등 Scale을 가진 Spline 계층에서 이동·회전·비균등 Scale·복합 TRS 검증.
- Root Scale을 0으로 만든 뒤 Undo/Redo 복원, 자식 Relative Transform·인스턴스 Local Transform 보존 검증.
- 같은 Actor의 기준 컴포넌트 교체, 외부 변경·파괴 후 이전 이력 재생, 선택 해제 후 GC, 부모·자식 중복 변환 방지 검증.
- 호스트의 클릭 선택과 같은 선택 인터페이스를 통해 컴포넌트 정보 및 Local/World 선택 정책 검증.
- 세 Context API 구현 파일은 이번 수정 직전과 원문이 일치합니다.

보고서: `Saved/Automation/VTBOWTComponentFrame/index.json`.
로그: `Saved/Logs/VTBOWTComponentFrame.log`.
이번 변경은 소스 빌드와 자동화 테스트로 검증했으며, 기존 실행 중인 데모 패키지는 갱신하지 않았습니다.
