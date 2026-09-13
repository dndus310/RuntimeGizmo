# OWT 관전자 Pawn 입력

## 생성된 에셋

- `/Game/VTBOWT/Blueprints/BP_VTBOWTSpectator` (부모: `AVTBOWTSpectator`)
- `/Game/VTBOWT/Input/IMC_OWTEdit`
- `/Game/VTBOWT/Input/IA_*` 14개 (편집 11개, 카메라 3개)

직접 작성했던 AbilityBindingComponent는 제거했다. Pawn의 기본 UEnhancedInputComponent가 입력 바인딩을 담당하고 MappingContext에 IMC_OWTEdit를 지정한다. IMC 등록/해제는 Pawn에서 엔진 UEnhancedInputLocalPlayerSubsystem을 통해 수행한다.

| IA | 키 | 편집 명령 |
|---|---|---|
| IA_ToggleEditing | F2 | 편집모드 켜기/끄기 |
| IA_Selectable | Left Mouse Button | SelectObject |
| IA_Undo | Ctrl+Z (Shift 제외) | Undo |
| IA_Redo | Ctrl+Shift+Z / Ctrl+Y | Redo |
| IA_TRSGizmoCoordinate | Ctrl+` | ToggleCoordinateSystem |
| IA_GizmoMode | Ctrl+T | ToggleTransformSpline |
| IA_GizmoTranslation | W | SetTranslation |
| IA_GizmoRotation | E | SetRotation |
| IA_GizmoScale | R | SetScale |
| IA_SelectCancel | Escape | HideSelectionGizmo |
| IA_Duplicate | Ctrl+D | DuplicateSelection |

편집 매핑은 Pressed 트리거와 필요 시 OWT Modifier Keys 트리거를 사용한다.
BP에서는 Triggered 핀을 연결하여 누른 순간 한 번만 전달한다.
좌/우 Ctrl과 Shift를 모두 지원한다. Ctrl+Shift+Z는 Undo 매핑의 Shift 금지 조건 때문에 Undo와 중복 실행되지 않는다.

## 입력과 편집의 경계

```text
Pawn의 Enhanced Input 이벤트
  → Make 명령별 Context 구조체 → Make Instanced Struct
  → SendEditContext(FInstancedStruct)
  → IOWTEditContextReceiver::ReceiveEditContext
  → UVTBOWTEditorSubsystem
  → ActiveEditMode (기본: UOWTObjectEditMode)
  → SelectObject / Undo / Redo / ... / DuplicateSelection 이벤트
```

편집 코드에는 UInputAction, IMC, FInputActionValue, 키, TriggerEvent를 전달하지 않는다.
`FInstancedStruct` 안에는 명령별 구조체(FOWTUndoContext, FOWTRedoContext 등)를 넣는다. 선택 요청은 FOWTSelectObjectContext.SelectedObject에 weak reference로 전달한다.
활성 모드는 UScriptStruct를 키로 하는 핸들러 테이블로 분기 없이 호출한다. 등록되지 않은 타입은 false를 반환한다.

Play 직후에는 편집모드가 꺼져 있다. F2를 누르면 기본 Translation 상태로 활성화되며, 다시 누르면 Gizmo와 선택을 정리한다. 활성화 후 큐브를 클릭하면 Gizmo가 표시되고 W/E/R로 이동/회전/크기 축을 전환한다.

선택 그래프는 `IA_Selectable → TraceSelectableObject → SendEditContext`이며 Trace의 Actor 반환값이 Make FOWTSelectObjectContext.SelectedObject에 연결된다.
Trace는 Pawn 쪽에서 마우스 좌표를 월드로 역투영하여 Visibility 채널로 검사한다. 빈 공간 또는 유효한 마우스 좌표가 없으면 nullptr를 전달한다.
PlayerController는 좌표 조회, LocalPlayer 접근, 커서·입력 모드·회전 상태 제어에 사용한다. 입력 바인딩과 핸들러는 Pawn에만 작성했다.

Subsystem은 공통 SelectedObject를 보관하고 인터페이스로 활성 모드에 요청을 전달한다.
사용하지 않던 `OnEditContextReceived` 관찰용 델리게이트는 제거했다.
`ReceiveEditContext`의 true는 라우팅 수락을 의미하며, 실제 작업 성공이나 Undo 저장을 의미하지 않는다.

## 편집 기능 연결 지점

`UOWTObjectEditMode`에는 10개의 BlueprintNativeEvent가 있다. 선택, Gizmo 숨김, Local/World 및 이동/회전/크기 전환의 기본 구현은 Subsystem의 관리 API를 호출한다.
Spline 조작, 객체 복제, Undo/Redo 알고리즘은 아직 스켈레톤이다.
기존 AVTBAttributeEditor의 히스토리도 스켈레톤 상태이다.

Subsystem은 월드마다 `UVTBOWTEditorToolsContext`를 초기화하고 매 프레임 ToolManager/GizmoManager 및 GizmoViewContext를 갱신한다.
Subsystem은 GizmoManager에 `UVTBOWTBaseTransformGizmoBuilder`를 등록하고 `UVTBOWTBaseTransformGizmo`를 생성한다. Gizmo는 선택 액터의 Movable 루트 컴포넌트를 자신이 소유하는 TransformProxy에 연결한다.
루트가 없거나 Static인 액터는 선택만 유지한다. Gizmo는 UCombinedTransformGizmo의 수명 관리와 변환 계산을 재사용한다. 기본 Base 구성은 엔진 ACombinedTransformGizmoActor와 기본 핸들 세트를 사용한다. 기존 AVTBOWTTransformGizmoActor의 X/Y/Z 이동 화살표·회전 원·크기 상자는 Custom 구성으로 유지한다. 두 구성 모두 UVTBOWTTransformGizmoBehavior와 축 서브기즈모를 공유한다. 런타임 InteractiveToolsFramework를 사용하며 LightGizmos나 UnrealEd 모듈에는 의존하지 않는다.
선택 변경/숨김/대상 파괴 시 Manager를 통해 Gizmo와 Proxy 연결을 해제한다. 숨김은 선택을 유지한다.
OnWorldEndPlay/Deinitialize에서는 Subsystem이 입력 캡처 종료 → Gizmo 제거 → ToolsContext Shutdown을 요청한다. Context가 Queries/Transactions를 소유하고 내부 Manager 종료가 끝난 뒤 어댑터를 해제한다. Context 자체도 중복 종료를 허용한다.
Pawn은 포인터 ray/누름/유지/해제 상태를 FOWTGizmoPointerContext로 전달한다. Subsystem은 이를 InputRouter에 전달한다. 핸들 캡처가 성립하면 BP의 선택 요청을 소비해 드래그 중 Gizmo가 재생성되지 않게 한다. 선택과 포인터 입력 모두 편집 비활성 상태에서는 처리하지 않는다.
Transactions 어댑터의 이력 저장은 비어 있으며 Transform 변경의 Undo/Redo는 제공하지 않는다.

실제 모드 구현은 UOWTObjectEditMode의 C++ 또는 Blueprint 자식에서 처리 이벤트를 재정의한다.
모드 객체를 Subsystem을 Outer로 생성한 뒤 SetActiveEditMode에 등록한다. 기본 모드는 Subsystem 초기화 시 자동 생성된다.
HideSelectionGizmo는 선택 해제가 아니므로 공통 SelectedObject를 지우지 않는다.
ToggleTransformSpline은 객체 전체 Transform 편집 도구와 Spline 편집 도구의 전환이며 SetTranslation은 Transform 도구 내부의 이동 축 선택이다.

## 플레이에 연결

기본 게임맵과 에디터 시작맵은 `/Game/VTBOWT/Maps/L_OWTEditSample`이다.
맵의 World Settings는 기존 `AVTBOWTEditorGameMode`를 사용한다.
GameMode의 DefaultPawnClass/SpectatorClass는 `BP_VTBOWTSpectator`, PlayerControllerClass는 `AVTBOWTEditorPlayerController`, GameStateClass는 `AVTBOWTEditorGameState`이다.
일반 Play에서도 관전자 Pawn을 조종하도록 DefaultPawnClass도 함께 지정했다. Controller는 마우스 커서 표시 설정만 추가하고 입력을 처리하지 않는다.
맵에는 `/Game/VTBOWT/Blueprints/BP_OWTEditableCube` 인스턴스, 큐브를 바라보는 PlayerStart, DirectionalLight가 있다.
큐브의 메시 컴포넌트는 Movable 및 BlockAllDynamic으로 설정되어 Visibility 선택 트레이스에 반응한다.
Pawn이 시작되거나 로컬 제어권을 얻으면 IMC를 등록하고, 제어권 해제/EndPlay에서 자신이 등록한 IMC를 해제한다.
기본 Spectator WASD 이동 바인딩은 W/E/R 편집 키와 충돌하여 비활성화했다. 대신 같은 IMC에 카메라 액션을 등록하고 Pawn의 SetupPlayerInputComponent에서 바인딩한다.

| IA | 키 | 동작 |
|---|---|---|
| IA_CameraNavigate | Right Mouse Button | 누르는 동안 카메라 조작, 해제 시 종료 |
| IA_CameraMove | WASD | 시선 방향 전후 이동 및 좌우 이동 |
| IA_CameraLook | Mouse XY | 시점 회전 |

우클릭을 누른 동안에만 이동·회전 입력을 적용하며 편집 명령과 Gizmo 포인터 전달을 차단한다. 따라서 우클릭+W는 전진이고, 우클릭 없이 W/E/R을 누르면 편집 축이 전환된다. 편집모드 활성 여부와 무관하게 카메라를 움직일 수 있다.
카메라 조작을 시작하면 커서를 숨기고 GameOnly 입력 모드로 전환한다. 우클릭 해제/취소 또는 제어권 해제/EndPlay에서는 이동 속도·대기 이동·회전 입력을 초기화하고 커서와 GameAndUI 모드를 복원한다. CameraLookSensitivity로 마우스 감도를 조절한다.
Gizmo가 마우스를 캡처한 동안에는 카메라 조작을 시작할 수 없고 이동·회전 입력도 적용하지 않는다. Gizmo 드래그가 끝난 뒤 우클릭을 다시 누르면 카메라를 조작할 수 있다.
PIE의 Escape 중지 단축키나 콘솔의 ` 키가 입력을 먼저 소비하는 환경에서는 해당 에디터 단축키 설정을 조정하거나 Standalone에서 확인한다.

## 생성·검증

Editor 빌드 후 `UnrealEditor-Cmd.exe <프로젝트.uproject> -run=CreateOWTInput -unattended -nullrhi`로 생성한다.
기존 에셋은 덮어쓰지 않는다. 검증만 하려면 `-ValidateOnly`를 추가한다.

검증은 BP 컴파일, SCS 컴포넌트/IMC 연결, 11개 실제 BP 입력 바인딩 실행과 인터페이스/허브/모드 전달, 선택 객체 전달, 선택 유지, modifier 조합, 키 반복 방지를 검사한다.
카메라 검증은 Pawn의 Enhanced Input 바인딩을 실행하여 우클릭 전 이동·회전 차단, 우클릭 중 이동·회전, 해제 시 즉시 정지, 카메라 조작 중 편집 차단, 실제 InputRouter의 Gizmo 캡처 중 카메라 차단 및 WASD 매핑 방향을 검사한다.
화면에서 실제 마우스 클릭/키 입력을 하는 PIE 테스트를 대신하지는 않는다.

`-run=CreateOWTLevel`은 샘플 큐브 BP와 맵을 생성하며 기존 맵을 덮어쓰지 않는다. `-ValidateOnly`는 저장된 맵의 World Settings, GameMode 클래스 연결, 큐브 메시/충돌/이동 가능 상태를 검증한다.
CreateOWTInput 검증에는 실제 Gizmo 생성, Proxy의 액터 Transform 반영, 숨김 후 선택 유지, 대상 파괴, 중복 Shutdown 및 재초기화 검사도 포함된다.



## 구조체 타입으로 핸들러 확장

수신 함수는 enum/switch 또는 명령별 if 체인을 사용하지 않는다. 등록은 정확한 구조체 타입 기준이며 상속 타입에 대한 암묵적 fallback은 하지 않는다.

1. 새 USTRUCT(BlueprintType)를 정의한다.
2. 그 구조체 하나를 입력으로 받는 UFUNCTION 또는 Blueprint 이벤트를 구현한다.
3. InitializeContextBindings에서 타입과 함수 이름을 등록한다. 부모 구현을 호출하면 기본 10개 바인딩이 유지된다.
4. 송신 측에서 구조체를 Make Instanced Struct로 감싸 기존 SendEditContext에 전달한다.

```cpp
// 헤더: FMyEditContext는 별도로 정의한 BlueprintType UStruct
UFUNCTION()
void HandleMyEdit(const FMyEditContext& Context);

// InitializeContextBindings_Implementation 내부
Super::InitializeContextBindings_Implementation();
BindContext<FMyEditContext>(
    this, GET_FUNCTION_NAME_CHECKED(UMyObjectEditMode, HandleMyEdit));
```

Blueprint에서는 BindContextHandler에 ContextType, 수신 객체, 이벤트/함수 이름을 전달한다.
대상 함수는 해당 구조체 타입의 입력 인자 하나만 가지고 반환값은 없어야 한다. 인자 없는 함수, 다른 타입, 반환값이나 쓰기 가능한 out 인자는 등록을 거부한다.
동일 타입 재등록은 기존 핸들러를 교체하고, UnbindContextHandler로 해제한다.
수신 객체는 weak reference로 저장하므로 객체가 파괴되면 호출하지 않고 false를 반환한다.
바인딩은 첫 등록/수신 시 지연 초기화하므로 C++ 생성자 안에서 Blueprint 이벤트를 호출하지 않는다.

C++의 자체 멤버 기본값은 생성자 초기화 리스트로 정의한다. 기본값이 없는 컨테이너·weak reference도 명시적으로 초기화한다.
상속받은 엔진 멤버(IsClient, bAddDefaultMovementBindings, OverrideInputComponentClass 등)는 C++ 문법상 파생 클래스 초기화 리스트에 넣을 수 없어 생성자 본문에서 설정한다.


런타임 Gizmo의 엔진 메시/머티리얼 경로 로딩에 대비해 /Engine/InteractiveToolsFramework 디렉터리를 쿠킹 대상에 포함했다. 이 패키징 설정의 엔진 섹션명은 UnrealEd이지만 게임 코드의 에디터 모듈 의존성을 추가하는 것은 아니다.


추가 검증: F2 BP 이벤트 실행, 비활성 중 명령 차단, W/E/R BP 이벤트 실행 후 핸들의 Visibility, 기본 Actor 생성, InputRouter를 통한 축 누름/드래그/해제와 액터 위치 변경. 헤드리스 테스트는 실제 엔진 메시의 충돌을 사용하되 뷰에 따른 크기·방향 보정만 끈다. 실제 화면의 픽셀 크기와 마우스 조작을 검증하는 테스트와는 구분한다.

## Base / Custom Gizmo

Public/Private 양쪽의 Gizmos 디렉터리를 다음과 같이 구성한다.

```text
Gizmos/
  Base/
    VTBOWTBaseTransformGizmo.h/.cpp
    VTBOWTTransformGizmoBehavior.h/.cpp
  Custom/
    VTBOWTCustomTransformGizmo.h/.cpp
    VTBOWTTransformGizmoActor.h/.cpp
```

- `OWT.Transform`: 기본 등록. UVTBOWTBaseTransformGizmoBuilder가 엔진 FCombinedTransformGizmoActorFactory의 기본 핸들 세트로 생성한다. 축·평면 이동, 축 회전, 축·평면·균등 스케일을 포함한다.
- `OWT.CustomTransform`: UVTBOWTCustomTransformGizmoBuilder가 기존 AVTBOWTTransformGizmoActor를 생성한다. UVTBOWTCustomTransformGizmo는 Base를 상속하여 대상 컴포넌트와 TransformProxy 연결을 재사용한다.
- 회전 축 집중 표시는 공통 UVTBOWTAxisAngleGizmo가 담당하므로 특정 Custom Actor 타입에 의존하지 않는다. 스냅, Local/World 좌표계, 입력 캡처, 카메라와의 상호 배제는 두 구성에서 공통으로 유지한다.
- Base의 재질은 `/Engine/InteractiveToolsFramework/Materials/GizmoComponentMaterial_NotDimmed`를 부모로 하는 `/Game/VTBOWT/Materials/MI_OWTGizmo_NotOccluded`에서 MID를 생성한다. 축 색상과 호버 색상은 엔진 원래 재질의 uniform 파라미터를 복사한다. 기본·호버·상호작용 대체 메시 모두 적용한다.
- `OccludeByCustomDepth`는 Static Switch다. MI에 False를 저장하고 쿠킹하며, 런타임에는 MID로 색상을 유지한다. MID에서 SetScalarParameterValue로 이 스위치를 바꾸는 방식은 사용하지 않는다. 엔진 원본은 수정하지 않는다.
- 재질 생성 스크립트: `Scripts/CreateOWTGizmoMaterial.py`. `UnrealEditor-Cmd.exe <프로젝트.uproject> -run=pythonscript -script=<스크립트 절대 경로> -EnablePlugins=PythonScriptPlugin -unattended -nullrhi`로 재생성할 수 있다. Python은 에셋 준비 시에만 필요하다.
- 쿠킹 대상에 엔진 InteractiveToolsFramework 콘텐츠와 프로젝트 VTBOWT/Materials를 포함한다.



## Gizmo 스냅과 회전 축 집중 표시

- 기본 스냅: 이동 10cm, 회전 15도, 크기 0.1. 세 가지 모두 기본 활성화한다.
- Blueprint에서 Get VTBOWTEditorSubsystem → Get Gizmo Snap Settings → 원하는 필드 변경 → Set Gizmo Snap Settings로 적용한다.
- FOWTGizmoSnapSettings의 TranslationStep, RotationStepDegrees, ScaleStep은 각각 단위를 지정하고 bTranslationEnabled, bRotationEnabled, bScaleEnabled는 각각의 스냅을 켜거나 끈다.
- 설정은 ToolsContext가 소유한다. Queries가 회전·크기 설정을 제공하고, ContextObjectStore의 런타임 UOWTGridSnappingManager가 이동 그리드 쿼리를 처리한다.
- 현재 스냅은 드래그 시작값에 대한 증분이다. 이동 10cm는 시작 위치에서 10cm씩, 회전 15도는 시작 방향에서 15도씩 변경한다. 크기 0.1은 배율을 더하는 방식으로 1.0 → 1.1 → 1.2로 변경한다.
- 0 이하 또는 NaN/무한대 단위는 거부한다. 드래그 중 설정 변경은 기존 캡처를 종료한 뒤 적용한다.
- 회전 축 드래그 중에는 선택한 회전 축만 보인다. 숨겨진 나머지 회전 축은 엔진 HitTarget의 Visibility 검사로 클릭/호버 대상에서도 제외된다.
- 마우스 해제, 입력 캡처 강제 종료, W/E/R 전환 시 회전 축 표시 상태를 복원한다. 다른 모드로 전환한 경우에는 그 모드의 표시 규칙이 적용된다.
- 헤드리스 검증: 양/음수 이동 스냅, 단위 변경, 스냅 해제, X/Y/Z 회전 스냅·나머지 축 입력 차단·복원, 강제 모드 전환, 크기 스냅 및 잘못된 단위 거부.
