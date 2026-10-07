# Runtime AttributeEditor 사용법

대상: Unreal Engine 5.7 · `OWTRuntimeEditing` 플러그인 · 2026-10-07

이 문서는 v3의 구현 구조와 사용 계약을 설명한다. 현재 설계 기준은 [AttributeEditMode_ITF_Spec.md](AttributeEditMode_ITF_Spec.md)이며 [기존 요구사항 문서](RuntimeAttributeEditor_Requirements.md)의 v2 결과는 과거 기록이다. 검증 결과는 각 기록의 대상 버전·실행 환경을 확인한다. 아래 설치·실행 예시는 검증 통과 보고가 아니다.

## 플러그인 설치와 모듈 선택

`Plugins/OWTRuntimeEditing` 안에 두 Runtime 모듈이 있다. Duplication은 `VTBOWTEditor`의 내부 기능으로 통합되어 있다.

| 모듈 | 책임 | 다른 기능 없이 직접 사용 |
|---|---|---|
| `OWTEventCore` | 소유 객체별 JSON 이벤트 전달, weak 구독, 제한된 진단 이력 | 가능. AttributeEditor나 월드가 필요하지 않은 UObject 소유자도 허용 |
| `VTBOWTEditor` | Runtime AttributeEditMode, ITF 도구 등록·수명, 선택/TRS, 내부 Duplication·PCG adapter, Details/Events | EventCore·ITF·EnhancedInput·PCG를 사용 |

다른 UE5.7 C++ 프로젝트의 `Plugins/OWTRuntimeEditing`으로 다음 항목을 함께 복사한다.

- `OWTRuntimeEditing.uplugin`
- `Source/`
- `Config/` — `Engine.ini`에 이전 모듈의 클래스·delegate 참조를 위한 Core Redirect 포함
- `Content/` — 런타임 Gizmo 재질 등 플러그인 에셋
- `README.md`

원본 프로젝트의 `Binaries/`, `Intermediate/`, `Saved/` 또는 `/Game/VTBOWT` 샘플 에셋은 설치 항목이 아니다. 대상 프로젝트에서 플러그인을 활성화하고 그 프로젝트의 Editor/Game target을 빌드한다. descriptor가 선언한 Enhanced Input과 PCG 의존성도 활성화된다. 엔진 버전과 대상 플랫폼에 맞게 소스에서 다시 빌드한다. `VTBOWTEditor`라는 기존 이름은 BP 참조 호환을 위해 유지하며 모듈 타입은 Runtime이다. `ModelingToolsEditorMode`를 Game target에 링크하거나 `UEdMode`를 상속하지 않는다.

소비하는 C++ 모듈의 `.Build.cs`에 사용하는 모듈을 추가한다. 편집기·복제를 사용하면 다음과 같다.

```csharp
PrivateDependencyModuleNames.AddRange(new string[]
{
    "VTBOWTEditor"
});
```

소비 모듈의 **public 헤더**에서 이 모듈의 헤더/타입을 노출하면 해당 의존성을 `PublicDependencyModuleNames`에 둔다. 범용 이벤트만 필요하면 `OWTEventCore`만 사용할 수 있다. 소비 모듈의 기존 Core/CoreUObject/Engine 의존성은 유지한다. 구 버전의 `OWTRuntimeDuplication` dependency는 제거하고 `VTBOWTEditor`로 바꾼다. `Duplication/...` include 경로와 클래스 이름은 유지하며 이전 `/Script/OWTRuntimeDuplication` class 경로와 이동한 이벤트 타입은 `Plugins/OWTRuntimeEditing/Config/Engine.ini`의 `[CoreRedirects]`로 이전한다. 표준 Engine 설정에 들어가야 독립 소비 프로젝트에서도 이전 클래스 경로를 해석할 수 있다. `Config/DefaultOWTRuntimeEditing.ini`는 위치 안내 주석만 남긴 파일이므로 실제 설정인 `Config/Engine.ini`를 함께 배포한다. Core Redirect가 C++ Build.cs 변경을 대신하지는 않는다.

## 샘플 및 native 구성으로 실행

기존 프로젝트에서는 `/Game/VTBOWT/Maps/L_OWTEditSample`을 연 뒤 Play/Standalone으로 실행한다. 새 프로젝트에서는 실행할 맵의 GameMode를 native **VTBOWTEditorGameMode**로 지정한다. 이 GameMode가 `AVTBOWTSpectator`, `AVTBOWTEditorPlayerController`, `AVTBAttributeEditor`를 연결한다. 클릭할 Actor에는 적절한 선택 trace 충돌이 있어야 한다.

별도 BP Pawn·입력 에셋·UMG 위젯 없이 native 구성으로 시작할 수 있다. Spectator의 `MappingContext`가 없으면 transient Enhanced Input action/매핑을 만들고 편집·카메라 동작을 native로 바인딩한다. Controller는 EnhancedPlayerInput을 직접 선택한다. 기존 BP가 MappingContext를 제공하면 그 입력 경로를 유지하므로, 커스텀 매핑의 편집 context 전달과 카메라 action 설정은 해당 BP/호스트의 책임이다.

1. 시작 시 편집은 꺼져 있다. **F2**를 누르면 오른쪽 Details 패널이 열린다.
2. Actor를 클릭하면 이름·ID·클래스·World Transform을 볼 수 있다.
3. 수치를 클릭하여 입력 후 Enter로 확정하거나 좌우로 드래그한다. 이동은 cm, 회전은 degree, 크기는 배율이다.
4. **Tools** 목록의 복제 도구 또는 월드 입력 상태의 **Ctrl+D**로 복제를 요청한다. AttributeEditor 기본 오프셋은 월드 +X 100cm이며 authored 구성 복제가 완료되면 복제본을 선택한다. PCG 생성 결과의 준비 상태는 별도로 확인한다.
5. **F3** 또는 **Events / F3**를 누르면 상태와 이벤트 이력을 함께 본다. 편집이 꺼진 상태에서도 F3으로 열 수 있다.
6. F2로 편집을 끄면 선택·Gizmo를 정리한다. Events를 열어 둔 경우 모니터는 남는다. F3을 다시 누르면 편집 중에는 Details로 돌아가고, 편집이 꺼져 있으면 패널을 닫는다.

| 조작 | 동작 |
|---|---|
| TRS 드래그 | Begin → Update → Commit |
| 드래그 중 Shift / Ctrl | 빠르게 / 미세하게 조절 |
| Details 드래그 중 Escape | 조작 시작 Transform으로 Cancel |
| 텍스트 편집 중 Escape | 입력 텍스트 취소; 활성 슬라이더가 있으면 Cancel |
| 월드 W / E / R | 이동 / 회전 / 크기 Gizmo |
| 월드 Ctrl+` | World / Local Gizmo 좌표계 전환 |
| 월드 Escape | Gizmo 숨김; 선택·Details 유지 |
| 월드 Ctrl+D | 선택 Actor 복제 |
| 우클릭 + WASD / 마우스 이동 | 카메라 이동 / 시점 회전 |
| F2 / F3 | 편집 활성 / Events 모니터 전환 |

패널 hover·키보드 focus·수치 drag capture 중에는 월드 선택, W/E/R·Ctrl+D와 카메라 입력을 차단한다. F2/F3은 패널 내부 focus에서도 처리한다. 탭 전환은 활성 수치 조작을 마치고 capture를 해제한다. PIE가 Escape를 플레이 종료로 먼저 처리할 수 있으므로 해당 입력은 Standalone에서도 확인한다.

Transform 편집은 root component가 있고 Movable인 선택 Actor에 제공한다. 비활성 이유는 Details에 표시한다. 선택이 없으면 이전 수치 필드를 숨긴다. 위치·회전에 임의의 좁은 범위를 강제하지 않으며, scale은 0·음수·비균등 값을 허용한다. NaN/무한대 입력은 거부한다. root가 없는 Actor의 복제 가능 여부와 Transform 편집 가능 여부는 별도 조건이다.

## 상태와 Events 모니터

위쪽 상태 요약은 두 탭에서 항상 보인다. 편집 ON/OFF, modifying, 기준 대비 변경 여부, 모드, Gizmo 종류/좌표계, state/selection revision, 선택 이름·ID를 표시한다. 전체 Editor/Object ID는 선택 요약 tooltip에서도 확인할 수 있다.

Details의 Tools 영역은 mode의 등록 목록을 읽어 자동 구성한다. 도구 ID별 버튼·활성 여부·사용 불가 사유를 표시하며 위젯에 새 도구 이름을 추가할 필요가 없다. Accept/Cancel은 활성 도구가 해당 종료 방법을 제공할 때만 켜진다. 영구 선택/TRS 도구처럼 Accept/Cancel이 없는 도구는 버튼이 비활성화된다.

Events의 **Live operations and procedural components**에는 mode lifecycle/active tool과 복제 작업별 phase·operation/request ID·원본/복제본·실패 이유를 표시한다. PCG는 component별 graph·trigger·state·generation attempt·reason·상관 ID를 별도로 표시한다. `Committed`는 authored Actor 구성의 완료이며 모든 PCG component가 `Ready`라는 뜻이 아니다. Runtime generation source를 기다리는 component는 `WaitingForGenerationSource`로 남을 수 있다.

Events는 최근 **256개** 진단 레코드를 최신 순으로 보여준다. 각 행에는 sequence, IN/OUT, topic, UTC 시각, source, requestId/operationId의 앞부분과 실패 사유가 나타난다. 행을 펼치면 전체 상관 ID, recipient, JSON payload를 볼 수 있다.

- **Pause display / Resume**: 이벤트 이력 표시만 멈춘다. 위쪽 상태, Live operations/component 상태와 실제 편집·이벤트 기록은 계속 진행된다. Resume 시 남아 있는 최근 이력을 읽는다.
- **Clear view**: 현재 sequence까지 이 위젯에서 숨긴다. 원본 이벤트 journal은 지우지 않는다.
- **Filter**: topic, source, request/operation ID, JSON 문자열 또는 sequence로 필터링한다.
- JSON은 journal에서 최대 **16,384 문자**만 보관한다. 잘린 항목은 표시되며, 확장 화면은 저장된 부분만 보여준다. `bValidJson`은 잘리기 전 원본의 유효성을 뜻한다.

F3은 편집을 켜지 않는다. 편집 OFF 상태의 모니터를 열어도 Transform/복제 입력은 활성화되지 않는다. Blueprint에서는 Controller의 `ToggleEventMonitor()` 또는 위젯의 `SetMonitorVisible(bool)`, `ToggleMonitor()`, `IsMonitorVisible()`을 사용한다.

## 실제 Actor → typed 상태 → 알림

선택·편집 활성·활성 도구는 subsystem이 소유한 `UOWTAttributeEditMode`, Transform과 이름·클래스는 실제 Actor가 원본이다. `AVTBAttributeEditor`가 이를 관찰하여 내부 `UOWTAttributeStateStore`에 저장한다. 저장소는 Actor weak reference, 세션별 stable ID, baseline과 typed `FOWTAttributeSnapshot`을 관리한다. 변경 권한은 소유 AttributeEditor에만 있다. subsystem의 기존 public API는 mode로 전달하는 호환 진입점이다.

`GetSnapshot()`은 **마지막으로 관찰한 상태의 값 복사본**을 반환한다. 반환된 구조체 수정, `ParseSnapshotJson()` 호출, journal JSON 수정으로 저장소나 Actor를 바꿀 수 없다. JSON은 요청·외부 전달·진단 형식이며 저장소의 데이터 원본이 아니다. 기본 UI는 알림을 받으면 typed `GetSnapshot()`을 다시 읽는다. 알림 JSON을 상태 저장소에 역으로 적용하지 않는다.

외부 코드가 선택 Actor를 직접 변경하면 다음 Subsystem Tick에서 다시 관찰한다. 필요한 경계에서는 `RefreshSelectedTransform()`으로 관찰을 요청할 수 있다. 모든 Actor의 모든 속성 변화가 즉시 수집되는 시스템은 아니다.

`hasChanges`는 선택 Actor의 현재 Transform과 최초 등록/`MarkSelectionBaseline()` 시점의 Transform 비교다. 새 복제본은 변경된 객체로 표시된다. 기준 Transform으로 돌아가거나 현재 상태를 새 baseline으로 수락하면 false가 된다. 이 값은 디스크 저장 상태, 임의 BP 변수, 재질·물리 상태 전체의 dirty 추적을 뜻하지 않는다.

## AttributeEditor C++ / Blueprint API

헤더는 `VTBAttributeEditor.h`, `Events/OWTAttributeTypes.h`, `VTBOWTEditorSubsystem.h`다. 게임 스레드에서 호출한다.

| API | 계약 |
|---|---|
| `Subsystem->GetAttributeEditor()` | 월드에 연결된 서비스. 초기화 전에는 nullptr 가능 |
| `GetSnapshot()` | 마지막 관찰 typed 상태의 복사본 |
| `Subscribe(Receiver, Callback, true)` | native 구독; 현재 snapshot을 즉시 전달 |
| `SubscribeDynamic(Receiver, Callback, true)` | 같은 기능의 BP delegate 구독 |
| `Unsubscribe(Handle)` | 구독 해제 |
| `RequestTransformField(Expected, Field, Value, Phase, OperationId)` | typed 값으로 JSON 명령을 구성하여 검증·적용 |
| `RequestDuplicate(Expected)` | 현재 선택에 대한 복제 요청 |
| `BeginDuplicateOperation(Expected, Options)` | 접수된 작업 ID 반환. invalid ID면 접수 실패 |
| `GetModeSnapshot()` | mode/tool/lifecycle의 typed 관찰 상태 |
| `GetDuplicationOperations()` | authored 복제 작업 상태의 복사본 |
| `GetProceduralComponents()` | PCG component별 생성 상태의 복사본 |
| `GetAvailableTools()` | registry에서 얻은 도구 ID·표시명·활성 여부·거부 사유 |
| `RequestStartTool(ToolId)` | 등록 도구 시작 요청 |
| `CanAcceptActiveTool()` / `CanCancelActiveTool()` | 활성 도구의 현재 종료 가능 여부 |
| `RequestEndTool(bAccept)` | true는 Accept, false는 Cancel 요청 |
| `PublishRequest(Event, Json)` | 직접 JSON 요청 제출; 상태 알림을 주입하는 API가 아님 |
| `MarkSelectionBaseline()` | 선택 Actor의 실제 Transform을 새 변경 비교 기준으로 수락 |
| `ParseSnapshotJson(Json, Out)` | 외부 전달 JSON을 caller-owned 구조체로 파싱 |
| `GetMonitorEntries(MaximumCount=256)` | 최근 진단 레코드 복사본. 반환 배열은 오래된 순 |
| `GetLatestEventSequence()` | 최신 journal sequence |

C++ 수신 객체에서 다음처럼 구독한다. 콜백 시그니처는 `void OnAttributeEvent(FName Event, const FString& Json)`이다.

```cpp
Subscription = Editor->Subscribe(
    this,
    FOWTAttributeEventNative::CreateUObject(this, &UMyInspector::OnAttributeEvent),
    true);
```

`UMyInspector`는 호출자 클래스 예시다. 콜백의 `RequestRejected`는 실패로 처리하고, 상태 알림에서는 보관한 Editor의 `GetSnapshot()`을 읽는다. JSON만 받는 외부 통합에서는 `ParseSnapshotJson()` 반환값을 확인한다. initial snapshot 콜백은 Subscribe가 반환되기 전에 실행될 수 있다. UI 값을 갱신하는 도중 수정 콜백이 명령을 재발행하지 않도록 구분한다. NativeDestruct/EndPlay에서 handle로 명시적으로 구독을 해제한다.

단일 수치 편집 예시:

```cpp
const FOWTAttributeSnapshot Expected = Editor->GetSnapshot();
const bool bApplied = Editor->RequestTransformField(
    Expected, EOWTTransformField::LocationX, 120.0,
    EOWTTransformEditPhase::Commit, FGuid::NewGuid());
```

슬라이더는 Begin 시점의 `Expected`와 새 OperationId를 저장하고 Update/Commit/Cancel에 계속 사용한다. 조작 중에 새 선택 snapshot으로 대상을 교체하지 않는다. 새 조작에는 새 OperationId를 쓴다.

`RequestDuplicate`의 true는 요청 접수다. 반환 즉시 새 Actor가 존재하거나 PCG 생성이 완료됐다고 가정하지 않는다. mode/tool Tick을 거쳐 `GetDuplicationOperations()`의 작업 phase와 `GetProceduralComponents()`의 component별 state를 확인한다. 명시적인 옵션과 상관 ID가 필요하면 `BeginDuplicateOperation`을 사용한다.

BP는 World Subsystem → AttributeEditor 조회 후 Subscribe Dynamic으로 연결한다. 상태 갱신에서는 Get Snapshot → Break OWTAttributeSnapshot, 명령에서는 Request Transform Field/Request Duplicate를 사용하면 JSON 문자열을 직접 조립할 필요가 없다. `Event(Name), Json(String)` signature를 갖는 자신의 callback과 Self를 Receiver로 전달한다.

## 범용 이벤트 모듈을 별도 소유 객체에서 사용

`UOWTNotificationCenter`는 `OWTEventCore`의 독립 UObject다. 별도 Actor/Component/UObject에서 생성할 수 있으며 AttributeEditor가 필요하지 않다. 헤더는 `Events/OWTNotificationCenter.h`, 레코드/enum/delegate는 `Events/OWTEventTypes.h`다. `FOWTAttributeEventNative/Dynamic` 이름은 기존 소스 호환을 위해 유지되지만 이벤트 topic은 범용이다.

소유 클래스 헤더의 멤버 예시:

```cpp
UPROPERTY(Transient)
TObjectPtr<UOWTNotificationCenter> Events;

FGuid EventSubscription;
```

초기화와 사용 예시 (`this`는 해당 서비스의 소유 UObject):

```cpp
Events = NewObject<UOWTNotificationCenter>(this);
if (!Events->Initialize(this))
{
    return;
}
EventSubscription = Events->Subscribe(
    this, FOWTAttributeEventNative::CreateUObject(this, &UMyService::OnEvent));
Events->RecordEvent(TEXT("ImportRequested"), TEXT("{\"asset\":\"Chair\"}"));
Events->Publish(TEXT("ImportCompleted"), TEXT("{\"count\":1}"));
```

`NewObject`의 **정확한 Outer와 Initialize의 Owner가 같아야 한다**. Owner는 weak로 저장된다. Outer 지정만으로 자식 UObject의 GC 생존을 보장하지 않으므로 서비스 참조를 `UPROPERTY`로 유지한다. 테스트·짧은 native scope에는 `TStrongObjectPtr`도 사용할 수 있다.

`Publish`는 유효한 JSON 값과 nonempty topic을 받으며 최대 65,536 문자다. 모든 구독자에게 전달하거나 optional Recipient에 특정 subscription handle을 지정할 수 있다. `RecordEvent`는 전달하지 않고 journal에만 남긴다. 잘못된 JSON 요청도 유효성 flag와 함께 진단용으로 보존할 수 있다. 범용 center는 application state나 schemaVersion을 해석하지 않는다.

구독자는 weak reference로 보관하고, BP dynamic delegate는 지정한 Receiver와 동일 객체를 가리켜야 한다. native delegate도 Receiver 수명으로 전달을 제한하므로 수신 객체 자신의 함수에 바인딩하는 구성을 기본으로 사용한다. 콜백 중 구독 변경/중첩 publish는 queue로 처리한다. 무한 재발행을 제한하기 위해 한 dispatch 흐름의 이벤트 수는 256개로 제한된다. `Publish` true는 queue 수락이지 모든 수신자가 업무를 성공했다는 뜻은 아니다.

기본 journal 용량은 256개이고 `SetHistoryCapacity(0..1024)`로 바꿀 수 있다. `GetRecentEvents()`는 오래된 순의 복사본, `GetLatestSequence()`는 단조 증가 sequence, `GetHistoryRevision()`은 추가·clear·용량 변경을 추적한다. `ClearEventHistory()`는 이력을 지워도 sequence를 되돌리지 않는다. 종료 시 `Unsubscribe(EventSubscription)` 후 `Shutdown()`을 호출하고 보관 참조를 정리한다. 모든 공개 사용은 게임 스레드에서 수행한다.

AttributeEditor의 내부 center는 외부에 쓰기용으로 노출하지 않는다. 다른 업무에 필요한 center는 그 업무 소유 객체가 별도로 만든다.

## 요청 JSON 및 상태 이벤트

지원 요청 topic은 `TransformEditRequested`, `DuplicateRequested`다. Event는 API 인자로 전달하며 JSON 내부의 `event` 필드로 선택하지 않는다. 다음 GUID와 selectionRevision은 실제 수신 snapshot/새 요청 ID로 교체한다.

```json
{
  "schemaVersion": 1,
  "editorId": "11111111-1111-4111-8111-111111111111",
  "requestId": "22222222-2222-4222-8222-222222222222",
  "source": "MyTool",
  "objectId": "33333333-3333-4333-8333-333333333333",
  "selectionRevision": 12,
  "operationId": "44444444-4444-4444-8444-444444444444",
  "phase": "Commit",
  "space": "World",
  "property": "Location.X",
  "value": 120.0
}
```

property는 `Location.X/Y/Z`, `Rotation.Roll/Pitch/Yaw`, `Scale.X/Y/Z`다. phase는 Begin/Update/Commit/Cancel이다. DuplicateRequested는 공통 필드 schemaVersion/editorId/requestId/source/objectId/selectionRevision을 사용한다. source는 1~64자다. 요청 JSON은 객체 형식이며 최대 65,536 문자다.

명시적인 복제 옵션은 `schemaVersion: 2`에서 `hierarchyScope`, `generationPolicy`, `worldOffset: {x,y,z}`로 전달한다. scope/policy 문자열은 위 옵션 enum의 실제 값 이름을 사용한다. schema 1에 이 세 필드를 넣으면 조용히 무시하지 않고 `SchemaRequired`로 거부한다. `BeginDuplicateOperation(Expected, Options)`는 이 schema 2 요청을 구성하며 유한하지 않은 offset과 enum sentinel/범위 밖 값을 거부한다.

요청마다 requestId는 새 GUID로 만든다. 드래그 동안에는 operationId만 유지한다. 같은 requestId의 재전송은 재실행하지 않고 종료된 operationId를 재사용하지 않는다. 요청 bool 반환도 확인한다. 서비스 준비 전/종료 후나 이미 처리한 ID의 재전송은 상태 이벤트 없이 false가 될 수 있다. 상태 콜백에서 다시 명령을 제출하는 재진입은 Busy로 거부될 수 있다.

| 이벤트 | payload |
|---|---|
| `EditorStateChanged` | 편집 활성·모드·도구·입력 가능 상태와 snapshot |
| `SelectionChanged` | 선택 snapshot; 해제 시 hasSelection=false, transform=null |
| `TransformChanged` | 실제 적용·관찰 snapshot, phase, operationId |
| `ObjectDuplicated` | 복제 후 snapshot, originalObjectId, duplicateObjectId |
| `ToolStarted` / `ToolEnded` | mode가 관찰한 도구 전환 |
| `DuplicateOperationChanged` | operationId에 대한 authored 복제 phase 변경 |
| `ProceduralGenerationChanged` | operationId/componentId별 PCG 생성 상태 변경 |
| `RequestRejected` | schemaVersion, editorId, requestId, source, code, reason; snapshot 아님 |

Transform JSON은 `transform.location.{x,y,z}`, `transform.rotation.{roll,pitch,yaw}`, `transform.scale.{x,y,z}`다. 상태에는 editingEnabled/hasSelection/canEditTransform/disabledReason/activeMode/gizmoMode/gizmoCoordinateSystem/isModifying/hasChanges/selectionRevision/stateRevision이 포함된다.

실패 코드는 EditingDisabled, StaleSelection, Busy, OperationEnded, InvalidValue, DuplicateFailed 등이다. 요청·거부 payload도 IN/OUT journal에 남지만 이것으로 typed 상태를 갱신하지 않는다.

## Mode 구성과 등록형 도구

`UVTBOWTEditorSubsystem`이 `UOWTAttributeEditMode`를 소유하고 mode가 ITF context, session context, selection, tool registry, 내부 duplication 서비스를 소유한다. Actor facade는 JSON 요청 검증과 observed StateStore/Notifications를 유지한다. 이 구조는 Modeling mode의 도구 등록·활성화·종료·context 제공 책임을 Runtime 환경에 적용한 것이며 Editor 전용 UEdMode를 상속하는 구조가 아니다.

도구 metadata는 `Extensions/OWTToolDescriptor.h`의 `FOWTToolDescriptor`다. ToolId, Label, Category, Description, BuilderClass와 history/mesh rendering 요구를 선언한다. mode `RegisterTool(Descriptor, ProviderId, OutToken, OutError)`로 등록하면 native Details UI의 Tools 영역에 표시된다. 등록은 Enter된 mode에서 수행하며 도구·target·service는 동일한 ProviderId로 묶는다.

| 등록 API | 책임 |
|---|---|
| `RegisterTool(Descriptor, ProviderId, OutToken, OutError)` | 고유 ToolId와 builder 등록 |
| `RegisterTargetFactory(Factory, ProviderId, OutError)` | ToolTargetManager에서 사용할 factory 등록 |
| `RegisterContextObject(Service, ProviderId, OutError)` | mode에 속하는 UObject를 ContextObjectStore에 등록. 중복 service type 거부 |
| `UnregisterProvider(ProviderId, OutError)` | 해당 활성 도구 종료 및 도구·target factory·context service 등록 해제 |
| `CanUnloadProvider(ProviderId)` | mode가 추적하는 builder/tool/target factory/context service 객체의 생존 여부 확인 |

컨텍스트 service는 `NewObject<UMyContextService>(Mode)`처럼 해당 mode의 소유 그래프에서 생성하고 `RegisterContextObject`로 등록한다. builder/tool은 `ToolManager->GetContextObjectStore()->FindContext<UMyContextService>()`로 조회한다. 기본 `UOWTAttributeEditSessionContext`는 `GetMode()`로 정확한 소유 세션을 제공한다. 전역 singleton이나 다른 월드의 context를 공유하지 않는다.

모듈 단위 확장은 `Extensions/OWTAttributeModeExtension.h`의 `IOWTAttributeModeExtension`을 구현한다. `GetProviderId()`와 `RegisterTools(Mode)`를 제공하고 `IModularFeatures::Get().RegisterModularFeature(IOWTAttributeModeExtension::GetModularFeatureName(), Provider)`로 provider를 등록하면 이후 mode Enter에서 호출한다. 이미 Enter된 mode에는 provider가 명시적으로 등록을 적용한다. modular feature 추가만으로 기존 모든 mode에 자동 적용되지는 않는다.

해제는 future Enter용 modular feature 등록을 제거하고, 이미 적용한 각 mode에 `UnregisterProvider`를 호출한 뒤, `CanUnloadProvider`가 true가 될 때까지 해당 모듈 코드를 유지한다. 등록 해제 성공과 즉시 코드 unload 가능은 다르다. 외부에서 보관한 tool/factory/service 참조와 GC 전 생존 객체 때문에 false일 수 있다. provider가 별도로 생성한 target·native 객체나 외부 callback은 provider 자체의 수명 책임이며 이 조회가 모든 모듈 객체를 열거하는 것은 아니다. `OWT.Core`는 해제·unload 대상이 아니다. 종료 과정에서도 반환값과 오류를 확인하며 강제로 살아 있는 provider 코드를 제거하지 않는다.

도구 구현은 `UInteractiveToolBuilder`와 `UInteractiveTool`의 Setup/Tick/Shutdown 계약을 따른다. 필요한 session 서비스는 context store에서 조회한다. 위젯·입력 코드에 특정 BP 이름이나 에셋 경로를 넣지 않는다. `GetAvailableTools()`의 비활성 사유와 capability를 기준으로 시작 가능 여부를 판단한다. 현재 Runtime host에 없는 history/mesh rendering 기능을 요구하는 도구를 지원하는 것처럼 활성화하지 않는다.

native GameMode는 `AOWTRuntimeToolsHUD`를 사용해 실제 game viewport의 HUD 프레임에서 mode의 Render/DrawHUD를 전달한다. 다른 프로젝트의 커스텀 HUD를 사용할 때는 이 HUD를 상속하거나 해당 HUD의 DrawHUD에서 `Mode->RenderTools(Canvas, PlayerOwner)`를 전달한다. synthetic Canvas에서 함수를 직접 호출한 검사는 실제 viewport 연결 검사를 대신하지 않는다. `OWT.Runtime.ViewportRendering`은 active RHI의 실제 게임 맵에서 실행하며 NullRHI에서는 명시적으로 skip한다. skip은 viewport 표시 성공 결과가 아니다.

## 내부 복제 서비스의 저수준 호환 API

일반 편집 흐름은 AttributeEditor → AttributeEditMode → Duplicate tool을 사용한다. `VTBOWTEditor`의 `Duplication/OWTRuntimeActorDuplicator.h`에는 기존 소유 UObject 기반 API도 유지한다. 직접 사용할 때는 살아 있는 월드를 제공하는 UObject가 소유하며 정확한 Outer/Owner 일치, `UPROPERTY` 보관, 게임 스레드 호출 규칙을 적용한다. 소유자가 서비스 `Tick`과 종료 `Deinitialize`를 전달해야 procedural 상태를 계속 관찰하고 정리할 수 있다.

```cpp
// 소유 Actor/Component의 헤더 멤버
UPROPERTY(Transient)
TObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;

// 초기화
Duplicator = NewObject<UOWTRuntimeActorDuplicator>(this);
if (!Duplicator->Initialize(this))
{
    return;
}

// Source는 소유자와 같은 standalone 월드의 실제 Actor
FString Error;
AActor* Duplicate = Duplicator->DuplicateActor(Source, FVector(100, 0, 0), Error);
if (!Duplicate)
{
    // Error를 호출자에게 표시한다.
}

// 소유자 종료 시
Duplicator->Deinitialize();
```

저수준 `DuplicateActor` 반환은 authored 구성의 동기 복제 결과다. PCG generation 완료를 뜻하지 않는다. 복제본은 원본과 같은 Actor 클래스·Level에 생성한다. 외부 부모에서 분리하며 caller의 world offset을 루트와 포함 계층에 한 번 적용한다. 루트의 Owner/Instigator는 비우고 내부 child 소유 관계는 재연결한다. 원본 객체는 유지한다. 반환 nullptr이면 Error를 확인한다. 같은 helper에 대한 재진입 복제는 허용하지 않는다.

이름은 Actor `GetName()`의 마지막 `_숫자`를 떼어 family를 구하고 같은 Level의 기존 이름과 해당 helper counter보다 큰 번호를 쓴다. 예: `Chair → Chair_1`, 기존 `Chair_4 → Chair_5`, 복제본 `Chair_5 → Chair_6`. 이미 더 큰 번호가 있으면 그 다음 번호를 사용한다. 삭제로 counter가 되돌아가지 않으며 Deinitialize에서 초기화된다. 이는 UObject 이름 규칙으로 Actor Label과 별개다. 루트 이름에 적용하고 ChildActor 이름은 해당 component 생성 규칙을 따른다.

## 복제 범위와 PCG 생성

`Duplication/OWTDuplicationRequest.h`의 `FOWTDuplicationOptions`로 범위를 지정한다.

| 옵션 | 계약 |
|---|---|
| `WorldOffset` | 포함 Actor 계층에 적용하는 월드 오프셋. 구조체 기본값은 0이며 facade의 기본 복제 요청은 설정된 +X 100cm 사용 |
| `HierarchyScope = AuthoredHierarchy` | 기본값. 원본 Actor, 관리 ChildActor와 작성자가 부착한 Actor 하위 계층 포함. PCG 관리 생성물 제외 |
| `HierarchyScope = ActorAndManagedChildren` | 기존 root + ChildActorComponent 범위. 일반 attached Actor 하위 트리 제외 |
| `GenerationPolicy = RegenerateIfSourceGenerated` | 기본값. OnDemand component는 원본이 생성된 경우 재생성 요청 |
| `GenerationPolicy = KeepUnGenerated` | OnDemand component의 자동 생성 요청만 생략. OnLoad/Runtime trigger 정책은 유지 |

기존 저수준 `DuplicateActor(Source, Offset, Error)`는 root + 관리 ChildActor 범위를 유지한다. 범위가 필요한 호출은 `DuplicateActorWithOptions(Source, Options, OperationId, Error)`를 사용한다. 일반 attached Actor는 같은 Level이어야 하며 부착 cycle과 PCG 관리 생성물 아래에 달린 authored 객체는 명시적 실패로 처리한다.

PCG는 외부 graph asset 참조, 소유 GraphInstance와 사용자 parameter, component의 authored 설정을 복원한다. custom scheduling policy의 reflected·instanced 참조도 복제 그래프에 맞게 재매핑하며 opaque native payload에는 별도 adapter가 필요하다. 복제본의 관리 출력·task ID·cache·생성 자원은 원본과 공유하지 않고 PCG가 다시 생성한다. ISM이나 spawned Actor가 눈에 보인다는 이유만으로 authored component/Actor로 이중 복제하지 않는다. graph asset은 외부 공유 자산이며 새 BP/graph asset을 저장하는 기능은 아니다.

adapter 실행 순서는 구성 복원·참조 검사 → `PrepareCommit` → authored commit 알림 → `Commit`이다. PCG `PrepareCommit`은 component별 observation과 generation delegate를 먼저 설치하고 초기 Scheduled/Waiting 상태를 준비한다. 따라서 `ObjectDuplicated` 구독자가 `GetProceduralComponents()`를 읽을 때 component 목록을 얻을 수 있고, 빠르게 완료되는 generation callback도 놓치지 않는다. 실제 활성화·scheduler 등록·generation 요청은 `Commit`에서 수행한다. 자체 adapter도 작업이 시작된 뒤 관찰자를 늦게 붙이지 말고 이 경계를 사용한다.

생성은 component의 trigger에 따라 달라진다.

- **GenerateOnLoad**: authored 복제 commit 후 생성 요청을 보낸다.
- **GenerateOnDemand**: 원본 generated 상태와 GenerationPolicy에 따라 요청하거나 `NotRequested`로 유지한다.
- **GenerateAtRuntime**: PCG Runtime Generation System에 등록·갱신한다. generation source가 없으면 `WaitingForGenerationSource`가 정상 상태이며 고정 시간 경과를 실패로 판정하지 않는다.
- 비활성 원본 component는 비활성 설정을 보존하고 `NotRequested` 및 이유를 표시한다.

partition/runtime 정책에는 해당 world에 등록된 PCGWorldActor가 필요하며 없으면 `MissingPCGWorldActor`로 거부한다.

작업 phase `Committed`와 component state `Ready`는 별개다. component마다 NotRequested/Scheduled/WaitingForGenerationSource/Generating/Ready/Failed/Cancelled/Cleaned/CleaningUp 및 generation attempt를 관찰한다. `Ready`는 현재 등록된 generation workload와 관련 local component가 생성 완료·idle 상태라는 뜻이다. 앞으로 scheduler가 만들 모든 cell의 완료를 보장하지 않는다. source 이동에 따른 cleanup은 CleaningUp/Cleaned로, 재진입 생성은 새 generation attempt로 관찰한다. 한 component의 성공을 다른 component의 성공으로 취급하지 않는다. 생성 task 수락 실패나 component 파괴도 해당 component 상태와 reason에 남긴다.

생성·cleanup 중인 원본 또는 접근할 수 없는 관리 자원은 `SourceBusy`로 거부한다. PCG managed output을 향한 authored/parameter 참조, opaque native parameter 상태, 소유 graph definition 자체, 생성물 아래 authored 부착은 전용 adapter 없이 얕게 복사하지 않는다. 외부 graph asset과 소유 GraphInstance는 지원하되 임의 native graph 구현 전체를 자동 복제하지 않는다. Construction이 복원 전에 PCG generation이나 cleanup을 직접 시작하는 클래스는 협력하는 초기화/복제 adapter가 필요하다. 대상 Construction에서 시작한 cleanup이 감지되면 `DestinationPCGLifecycleViolation`으로 거부한다. 사용자 코드가 만든 외부 부작용은 rollback 대상이 아니다.

클래스별 확장은 `Duplication/OWTDuplicationAdapter.h`의 `IOWTDuplicationAdapter`와 소유 세션의 registry를 사용한다. `Duplicator->GetAdapterRegistry().Register(SupportedClass, Factory, Role)`로 작업별 adapter factory를 등록하고 `Unregister(SupportedClass, Role)`로 해제한다. `Role` 기본값은 `EOWTDuplicationAdapterRole::Primary`다. 동일 UClass와 Role의 중복 등록은 false를 반환하지만 같은 클래스에 Primary와 Auxiliary를 각각 하나씩 등록할 수 있다.

core는 authored Actor와 component마다 상속 관계상 가장 구체적인 Primary 하나와 해당 클래스에 적용되는 모든 Auxiliary를 선택한다. `ValidateObject`/`CaptureObject`를 구현하며 기존 Actor용 `ValidateActor`/`CaptureActor`는 기본 구현을 통해 호출된다. 선택되지 않은 adapter는 world ownership indexing에만 참여하고 capture/restore/commit 대상에서 제외된다. `AcceptsObject`/`IsAuxiliary`는 적용 범위 확인용이며 core의 capture 추적 대신 사용자 capture 구현은 `CaptureObject`에 둔다.

Primary만 `OwnsProperty`/`OwnsObject`로 복원 책임을 선언한다. Auxiliary의 ownership 선언 또는 여러 Primary의 같은 대상 ownership은 `AdapterOwnershipConflict`로 실패한다. Auxiliary는 검증·관찰·정책 훅을 함께 적용할 수 있지만 같은 구성의 별도 복원을 소유하면 안 된다. adapter는 capture/restore/reference validation/commit/rollback과 managed output 식별을 담당하며 원본을 수정하지 않는다. 일반 reflection 복사에 PCG 타입별 property 이름 분기를 계속 추가하는 방법으로 확장하지 않는다.

`Deinitialize()`는 서비스와 관찰 delegate를 정리한다. 이미 commit된 Actor를 삭제하는 API가 아니다. 실패 rollback은 해당 작업에서 새로 만든 대상과 adapter 자원을 정리하며 원본의 PCG 출력을 cleanup하지 않는다.

## 복제 범위와 클래스별 확장

다음 상태를 캡처·복원한다.

- 지원 nontransient reflected Actor 변수/컨테이너, 편집 가능한 component 설정, component 내부 부착·상대 Transform, runtime instance component와 instanced UObject.
- self·component·owned object 참조와 관리되는 child 사이의 참조를 복제 그래프로 재매핑. 외부 Actor·공유 에셋·미로드 soft path는 외부 참조로 유지.
- `ChildActorComponent`가 소유하는 재귀 child 계층. `AuthoredHierarchy` 요청에서는 일반 authored attached Actor 트리도 포함한다. PCG managed output은 다시 생성한다.
- ISM의 instance Transform/custom data와 지원 dynamic material instance 값. 소유된 기본 `UMaterialInstanceDynamic`의 scalar/vector/double-vector/texture/font/texture-collection/RVT/global sparse-volume 값과 PhysMaterial/PhysicalMaterialMap override를 복원하고 native 자원을 다시 만든다.
- 용접되지 않은 일반 `UStaticMeshComponent` 단일 body의 지원 물리 설정, 속도·각속도, awake 상태. 실행 중 전체 물리 세계의 복제는 아니다.

루트가 없는 Actor도 복제할 수 있다. Pawn은 미점유 상태이고 자동 player/AI possession이 모두 Disabled인 경우에 한해 설정 복제를 허용한다. managed ChildActor만 직접 넘기지 말고 그 부모 Actor를 복제한다.

객체 그래프의 reflected 값과 참조 복원 경계에서 각 복제 대상의 `IOWTRuntimeDuplicationParticipant` 확장점을 호출한다. 기존 root + 관리 ChildActor의 BeginPlay는 해당 복원 뒤 실행한다. public 헤더는 `Duplication/OWTRuntimeDuplicationParticipant.h`다.

```cpp
bool RestoreRuntimeDuplicateState_Implementation(
    UObject* SourceObject,
    const TMap<UObject*, UObject*>& DuplicatedObjects,
    FString& OutError) override;
```

Actor/component/owned UObject가 이 BlueprintNativeEvent interface를 구현할 수 있다. destination에서 SourceObject의 class별 native 설정을 읽고, 내부 참조는 DuplicatedObjects로 바꾸고, 필요한 resource는 새로 생성한다. 살아 있는 handle/pointer를 그대로 복사하지 않는다. 기본 구현은 성공이며 false를 반환하면 새 루트와 관리 child 계층을 정리하고 실패를 반환한다.

Construction과 일부 native 초기화/child PostInitializeComponents는 이 hook보다 먼저 실행될 수 있다. 사용자 Construction·초기화·hook이 만든 임의 외부 부작용은 rollback하지 못한다. 기존 root + 관리 ChildActor의 BeginPlay는 해당 구성 복원 뒤 실행하지만 그 코드가 값을 다시 바꾸는 경우는 해당 클래스의 동작이다. 일반 attached Actor 전체를 포함한 요청에서는 엔진 공개 spawn 수명 때문에 일부 Actor의 BeginPlay가 모든 sibling 복제본의 연결 완료보다 먼저 실행될 수 있다. 전체 계층 준비가 필요한 외부 작업은 operation의 `Committed`를 기다린다. PCG generation은 전체 authored commit까지 억제한다.

다음 경계에서는 구체적인 실패 사유를 반환한다.

- 다른 월드·네트워크 월드, teardown 중 월드, CDO/archetype, 파괴 중 Actor 또는 유한하지 않은 Transform.
- AInfo/Controller/Brush·volume 등 별도 애플리케이션 정책이 필요한 관리 Actor, 위 조건을 만족하지 않는 Pawn.
- Instanced/DefaultToInstanced 계약 없이 분리 복원을 요구하는 소유 UObject 또는 생성 과정에서 예상 클래스/그래프가 달라지는 경우.
- 전용 adapter가 없는 reflected 속성의 opaque `FInstancedStruct`/`FInstancedPropertyBag`와 raw `FBodyInstance`/`FConstraintInstance`/`FTickFunction` 구조체. PCG의 지원 parameter bag은 PCG adapter가 참조를 점검·재매핑하며 raw handle까지 허용하지 않는다. 일반 primitive component의 BodyInstance 설정은 별도 안전한 adapter를 거쳐 지원한다.
- 시뮬레이션 중 skeletal/instanced/welded body, 지원 범위 밖 native 물리 자원과 `UPhysicsConstraintComponent`. ConstraintInstance의 live handle은 일반 reflected 복사로 안전하게 분리할 수 없다.
- custom MID subclass, parameter collection/UserSceneTexture/Nanite override/layered sparse-volume 등 전용 재질 어댑터가 없는 상태.

timer 실행 상태, delegate 바인딩, coroutine/latent 실행, native 비-UPROPERTY 데이터 전체, 숨은 참조를 포함한 native 직렬화 컨테이너, 외부 파일·네트워크·GPU handle은 자동 snapshot 대상이 아니다. interface hook은 class별 **설정 복원** 확장점이며 거부된 구조·물리·재질 종류를 무조건 통과시키는 옵션이 아니다. 이 서비스는 Unreal Editor 전체 DuplicateActor 동작 또는 임의 프로세스 메모리 복제를 보장하지 않는다.

## 현재 편집기 범위와 검증 진입점

- 단일 Actor 선택과 World Transform 편집 UI를 제공한다. 다중 선택, 상대 Transform UI, 임의 reflected 속성 전체 편집은 없다.
- Undo/Redo 입력 context·확장 함수는 있지만 범용 undo stack, 프로젝트 저장/로드, 새 BP asset 생성·컴파일·저장은 제공하지 않는다.
- 복제는 runtime Actor instance 작업이다. mode API의 접수·commit과 PCG의 비동기 readiness는 분리된다. 멀티플레이 권한·replication·클라이언트 소유 정책은 제공하지 않는다.
- 모니터는 제한된 최근 진단 이력이다. 영구 감사 로그·전체 변경 이력 저장소가 아니다.

현재 plugin automation 진입점은 다음과 같다. 목록은 실행 방법이며 통과 결과가 아니다.

| 테스트 | 확인 대상 |
|---|---|
| `OWT.EventCore.OwnerIsolation` | owner·receiver 수명 및 격리 |
| `OWT.EventCore.CallbackMutation` | 구독 변경·중첩 callback 전달 |
| `OWT.EventCore.BoundedHistory` | 이벤트 journal 용량·sequence |
| `OWT.Runtime.AttributeContracts` | 요청·snapshot 계약 |
| `OWT.Runtime.AttributeDetailsContracts` | Details, 등록형 Tools UI, typed monitor, pause·OFF 상태 |
| `OWT.Runtime.ObservedState` | Actor 관찰값과 JSON 전달 분리 |
| `OWT.Runtime.NativeInput` | native 입력 매핑·편집 경로 |
| `OWT.Runtime.ModeLifecycle` | ITF 도구·입력·target/context/provider 수명 |
| `OWT.Runtime.AuthoredHierarchy` | authored Actor 계층·참조 복제 |
| `OWT.Runtime.AdapterRegistryResolution` | 가장 구체적인 Primary 선택·Auxiliary 병행·등록 충돌 |
| `OWT.Runtime.AdapterRegistryDispatch` | 실제 복제 경로의 adapter 실행·소유권 충돌·PCG adapter 교체 |
| `OWT.Runtime.DuplicationFacade` | schema 2 접수·commit·취소·stale source·결과 단일 발행 |
| `OWT.Runtime.PCGConfiguration` | PCG 구성·GraphInstance·참조 복원 |
| `OWT.Runtime.PCGRegeneration` | 생성 자원 분리·재생성·관찰 |
| `OWT.Runtime.PCGRuntimePolicy` | runtime scheduler와 생성 대기 상태 |
| `OWT.Runtime.PCGSchedulerHierarchy` | partition/HiGen 실제 생성, local workload·cleanup·재생성 관찰 |
| `OWT.Runtime.PCGPolicyReferences` | custom scheduling policy의 참조 복제·재매핑 |
| `OWT.Runtime.PCGCookedBlueprintHierarchy` | plugin BP/graph asset이 포함된 cooked 계층 복제 |
| `OWT.Runtime.PCGChildActorOnLoad` | live BeginPlay의 cooked BP ChildActor, authored commit 이후 OnLoad 생성과 자원 분리 |
| `OWT.Runtime.ViewportRendering` | active RHI 실제 game viewport의 Render/DrawHUD. NullRHI에서는 skip |
| `OWT.Runtime.ViewportMonitorCapture` | 실제 native Details/Monitor UI를 Saved/Screenshots에 PNG로 저장. NullRHI에서는 skip; 생성 후 시각 검사 필요 |

`PCGCookedBlueprintHierarchy`에는 plugin의 `Content/Tests` asset이 필요하다. 배포 시 Content와 `Config/Game.ini`를 함께 복사해야 해당 cook 경로가 유지된다. 원본 개발 프로젝트에서 fixture를 다시 만들 때는 Editor commandlet `CreateOWTInput -CreatePCGFixture`를 사용한다. fixture 생성 commandlet은 host의 Editor 전용 automation 모듈에 있고 소비 프로젝트 Runtime 실행 의존성이 아니다.

프로젝트 모듈의 `OWT.Runtime.Attributes`는 `/Game/VTBOWT` 샘플 BP를 요구한다. 독립 소비 fixture의 `OWT.Portability.IndependentHost`는 복사한 소스와 현재 module dependency가 일치할 때 별도로 실행한다. 고급 복제 fixture는 Editor 커맨드렛 `ValidateOWTAttributes`, 기존 BP 입력/맵 회귀는 `CreateOWTInput`/`CreateOWTLevel -ValidateOnly`를 사용한다.

현재 독립 소비 프로젝트 fixture는 `Saved/PortabilityProbeV3`다. 새 probe의 `Plugins/OWTRuntimeEditing`이 아직 없을 때 `Saved/PortabilityProbeV3/CopyPlugin.ps1`을 실행하여 Source/Config/Content/descriptor/README를 복사한다. 이 helper는 기존 목적지를 덮어쓰지 않는다. 기존 probe를 재검증할 때는 복사본과 현재 plugin의 소스·설정·콘텐츠가 일치하는지 먼저 확인하고 소비 모듈의 dependency를 `VTBOWTEditor`와 필요한 `OWTEventCore`로 맞춘다. `Saved/PortabilityProbeV3/OWTPortabilityProbe.uproject`를 대상으로 해당 Editor/Game target을 빌드한 뒤 `OWT.Portability.IndependentHost`와 plugin 테스트를 실행한다. 이전 `Saved/PortabilityProbe`의 결과를 이번 구조의 검증으로 사용하지 않는다. 화면상의 수치 입력, 슬라이더, 포커스, F2/F3, 카메라와 expanded JSON은 headless 계약 검사 외에 실제 창에서도 확인한다.
