# Runtime AttributeEditor 사용법

대상: Unreal Engine 5.7 · `OWTRuntimeEditing` 플러그인 · 2026-10-07

이 문서는 구현 구조와 사용 계약을 설명한다. 테스트 결과와 실행 환경은 [요구사항 문서](RuntimeAttributeEditor_Requirements.md)의 검증 기록을 확인한다. 아래 설치·실행 예시는 검증 통과 보고가 아니다.

## 플러그인 설치와 모듈 선택

`Plugins/OWTRuntimeEditing` 안에 세 Runtime 모듈이 있다.

| 모듈 | 책임 | 다른 기능 없이 직접 사용 |
|---|---|---|
| `OWTEventCore` | 소유 객체별 JSON 이벤트 전달, weak 구독, 제한된 진단 이력 | 가능. AttributeEditor나 월드가 필요하지 않은 UObject 소유자도 허용 |
| `OWTRuntimeDuplication` | 같은 standalone 월드의 Actor 설정과 지원 객체 그래프 복제 | 가능. 월드가 있는 소유 UObject 필요 |
| `VTBOWTEditor` | typed 관찰 상태, 선택·TRS·복제 요청, Gizmo, 입력, Details/Events UI | 앞의 두 모듈을 조합 |

다른 UE5.7 C++ 프로젝트의 `Plugins/OWTRuntimeEditing`으로 다음 항목을 함께 복사한다.

- `OWTRuntimeEditing.uplugin`
- `Source/`
- `Config/` — 이전 모듈의 클래스·delegate 참조를 위한 Core Redirect 포함
- `Content/` — 런타임 Gizmo 재질 등 플러그인 에셋
- `README.md`

원본 프로젝트의 `Binaries/`, `Intermediate/`, `Saved/` 또는 `/Game/VTBOWT` 샘플 에셋은 설치 항목이 아니다. 대상 프로젝트에서 플러그인을 활성화하고 그 프로젝트의 Editor/Game target을 빌드한다. descriptor가 선언한 Enhanced Input 의존성도 활성화된다. 엔진 버전과 대상 플랫폼에 맞게 소스에서 다시 빌드한다.

소비하는 C++ 모듈의 `.Build.cs`에 사용하는 모듈만 추가한다. 예를 들어 범용 이벤트·복제 기능만 사용하면 다음과 같다.

```csharp
PrivateDependencyModuleNames.AddRange(new string[]
{
    "OWTEventCore", "OWTRuntimeDuplication"
});
```

소비 모듈의 **public 헤더**에서 이 모듈들의 헤더/타입을 노출하면 해당 의존성을 `PublicDependencyModuleNames`에 둔다. AttributeEditor를 사용할 때는 `VTBOWTEditor`를 추가한다. 소비 모듈의 기존 Core/CoreUObject/Engine 의존성은 유지한다.

## 샘플 및 native 구성으로 실행

기존 프로젝트에서는 `/Game/VTBOWT/Maps/L_OWTEditSample`을 연 뒤 Play/Standalone으로 실행한다. 새 프로젝트에서는 실행할 맵의 GameMode를 native **VTBOWTEditorGameMode**로 지정한다. 이 GameMode가 `AVTBOWTSpectator`, `AVTBOWTEditorPlayerController`, `AVTBAttributeEditor`를 연결한다. 클릭할 Actor에는 적절한 선택 trace 충돌이 있어야 한다.

별도 BP Pawn·입력 에셋·UMG 위젯 없이 native 구성으로 시작할 수 있다. Spectator의 `MappingContext`가 없으면 transient Enhanced Input action/매핑을 만들고 편집·카메라 동작을 native로 바인딩한다. Controller는 EnhancedPlayerInput을 직접 선택한다. 기존 BP가 MappingContext를 제공하면 그 입력 경로를 유지하므로, 커스텀 매핑의 편집 context 전달과 카메라 action 설정은 해당 BP/호스트의 책임이다.

1. 시작 시 편집은 꺼져 있다. **F2**를 누르면 오른쪽 Details 패널이 열린다.
2. Actor를 클릭하면 이름·ID·클래스·World Transform을 볼 수 있다.
3. 수치를 클릭하여 입력 후 Enter로 확정하거나 좌우로 드래그한다. 이동은 cm, 회전은 degree, 크기는 배율이다.
4. **Duplicate actor** 또는 월드 입력 상태의 **Ctrl+D**로 복제한다. AttributeEditor 기본 오프셋은 월드 +X 100cm이며 복제본을 선택한다.
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

Events는 최근 **256개** 진단 레코드를 최신 순으로 보여준다. 각 행에는 sequence, IN/OUT, topic, UTC 시각, source, requestId/operationId의 앞부분과 실패 사유가 나타난다. 행을 펼치면 전체 상관 ID, recipient, JSON payload를 볼 수 있다.

- **Pause display / Resume**: 이력 표시만 멈춘다. 위쪽 상태와 실제 편집·이벤트 기록은 계속 진행된다. Resume 시 남아 있는 최근 이력을 읽는다.
- **Clear view**: 현재 sequence까지 이 위젯에서 숨긴다. 원본 이벤트 journal은 지우지 않는다.
- **Filter**: topic, source, request/operation ID, JSON 문자열 또는 sequence로 필터링한다.
- JSON은 journal에서 최대 **16,384 문자**만 보관한다. 잘린 항목은 표시되며, 확장 화면은 저장된 부분만 보여준다. `bValidJson`은 잘리기 전 원본의 유효성을 뜻한다.

F3은 편집을 켜지 않는다. 편집 OFF 상태의 모니터를 열어도 Transform/복제 입력은 활성화되지 않는다. Blueprint에서는 Controller의 `ToggleEventMonitor()` 또는 위젯의 `SetMonitorVisible(bool)`, `ToggleMonitor()`, `IsMonitorVisible()`을 사용한다.

## 실제 Actor → typed 상태 → 알림

선택과 편집 활성 상태는 `UVTBOWTEditorSubsystem`, Transform과 이름·클래스는 실제 Actor가 원본이다. `AVTBAttributeEditor`가 이를 관찰하여 내부 `UOWTAttributeStateStore`에 저장한다. 저장소는 Actor weak reference, 세션별 stable ID, baseline과 typed `FOWTAttributeSnapshot`을 관리한다. 변경 권한은 소유 AttributeEditor에만 있다.

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

요청마다 requestId는 새 GUID로 만든다. 드래그 동안에는 operationId만 유지한다. 같은 requestId의 재전송은 재실행하지 않고 종료된 operationId를 재사용하지 않는다. 요청 bool 반환도 확인한다. 서비스 준비 전/종료 후나 이미 처리한 ID의 재전송은 상태 이벤트 없이 false가 될 수 있다. 상태 콜백에서 다시 명령을 제출하는 재진입은 Busy로 거부될 수 있다.

| 이벤트 | payload |
|---|---|
| `EditorStateChanged` | 편집 활성·모드·도구·입력 가능 상태와 snapshot |
| `SelectionChanged` | 선택 snapshot; 해제 시 hasSelection=false, transform=null |
| `TransformChanged` | 실제 적용·관찰 snapshot, phase, operationId |
| `ObjectDuplicated` | 복제 후 snapshot, originalObjectId, duplicateObjectId |
| `RequestRejected` | schemaVersion, editorId, requestId, source, code, reason; snapshot 아님 |

Transform JSON은 `transform.location.{x,y,z}`, `transform.rotation.{roll,pitch,yaw}`, `transform.scale.{x,y,z}`다. 상태에는 editingEnabled/hasSelection/canEditTransform/disabledReason/activeMode/gizmoMode/gizmoCoordinateSystem/isModifying/hasChanges/selectionRevision/stateRevision이 포함된다.

실패 코드는 EditingDisabled, StaleSelection, Busy, OperationEnded, InvalidValue, DuplicateFailed 등이다. 요청·거부 payload도 IN/OUT journal에 남지만 이것으로 typed 상태를 갱신하지 않는다.

## 복제 서비스를 AttributeEditor 없이 사용

`OWTRuntimeDuplication`과 `Duplication/OWTRuntimeActorDuplicator.h`만으로 사용할 수 있다. 살아 있는 월드를 제공하는 UObject가 소유하며 정확한 Outer/Owner 일치, `UPROPERTY` 보관, 게임 스레드 호출 규칙을 적용한다.

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

복제본은 원본과 같은 Actor 클래스·Level에 생성한다. 외부 부모에서 분리하며 caller의 world offset을 루트와 그 관리 계층에 한 번 적용한다. 루트의 Owner/Instigator는 비우고 내부 child 소유 관계는 재연결한다. 원본 객체는 유지한다. 반환 nullptr이면 Error를 확인한다. 같은 helper에 대한 재진입 복제는 허용하지 않는다.

이름은 Actor `GetName()`의 마지막 `_숫자`를 떼어 family를 구하고 같은 Level의 기존 이름과 해당 helper counter보다 큰 번호를 쓴다. 예: `Chair → Chair_1`, 기존 `Chair_4 → Chair_5`, 복제본 `Chair_5 → Chair_6`. 이미 더 큰 번호가 있으면 그 다음 번호를 사용한다. 삭제로 counter가 되돌아가지 않으며 Deinitialize에서 초기화된다. 이는 UObject 이름 규칙으로 Actor Label과 별개다. 루트 이름에 적용하고 ChildActor 이름은 해당 component 생성 규칙을 따른다.

## 복제 범위와 클래스별 확장

다음 상태를 캡처·복원한다.

- 지원 nontransient reflected Actor 변수/컨테이너, 편집 가능한 component 설정, component 내부 부착·상대 Transform, runtime instance component와 instanced UObject.
- self·component·owned object 참조와 관리되는 child 사이의 참조를 복제 그래프로 재매핑. 외부 Actor·공유 에셋·미로드 soft path는 외부 참조로 유지.
- `ChildActorComponent`가 소유하는 재귀 child 계층. 부모 Actor와 함께 복제하며 독립적인 외부 attached Actor 트리는 자동 포함하지 않는다.
- ISM의 instance Transform/custom data와 지원 dynamic material instance 값. 소유된 기본 `UMaterialInstanceDynamic`의 scalar/vector/double-vector/texture/font/texture-collection/RVT/global sparse-volume 값과 PhysMaterial/PhysicalMaterialMap override를 복원하고 native 자원을 다시 만든다.
- 용접되지 않은 일반 `UStaticMeshComponent` 단일 body의 지원 물리 설정, 속도·각속도, awake 상태. 실행 중 전체 물리 세계의 복제는 아니다.

루트가 없는 Actor도 복제할 수 있다. Pawn은 미점유 상태이고 자동 player/AI possession이 모두 Disabled인 경우에 한해 설정 복제를 허용한다. managed ChildActor만 직접 넘기지 말고 그 부모 Actor를 복제한다.

객체 그래프의 reflected 값과 참조 복원 뒤, 복제 계층의 BeginPlay 전에 각 복제 대상의 `IOWTRuntimeDuplicationParticipant` 확장점을 호출한다. public 헤더는 `Duplication/OWTRuntimeDuplicationParticipant.h`다.

```cpp
bool RestoreRuntimeDuplicateState_Implementation(
    UObject* SourceObject,
    const TMap<UObject*, UObject*>& DuplicatedObjects,
    FString& OutError) override;
```

Actor/component/owned UObject가 이 BlueprintNativeEvent interface를 구현할 수 있다. destination에서 SourceObject의 class별 native 설정을 읽고, 내부 참조는 DuplicatedObjects로 바꾸고, 필요한 resource는 새로 생성한다. 살아 있는 handle/pointer를 그대로 복사하지 않는다. 기본 구현은 성공이며 false를 반환하면 새 루트와 관리 child 계층을 정리하고 실패를 반환한다.

Construction과 일부 native 초기화/child PostInitializeComponents는 이 hook보다 먼저 실행될 수 있다. 사용자 Construction·초기화·hook이 만든 임의 외부 부작용은 rollback하지 못한다. BeginPlay는 복원 완료 뒤 실행하지만 그 코드가 값을 다시 바꾸는 경우는 해당 클래스의 동작이다.

다음 경계에서는 구체적인 실패 사유를 반환한다.

- 다른 월드·네트워크 월드, teardown 중 월드, CDO/archetype, 파괴 중 Actor 또는 유한하지 않은 Transform.
- AInfo/Controller/Brush·volume 등 별도 애플리케이션 정책이 필요한 관리 Actor, 위 조건을 만족하지 않는 Pawn.
- Instanced/DefaultToInstanced 계약 없이 분리 복원을 요구하는 소유 UObject 또는 생성 과정에서 예상 클래스/그래프가 달라지는 경우.
- reflected 속성에 포함된 opaque `FInstancedStruct`/`FInstancedPropertyBag`와 raw `FBodyInstance`/`FConstraintInstance`/`FTickFunction` 구조체. 숨은 참조나 live handle의 안전한 복제에는 모듈 내부의 해당 타입 전용 adapter가 필요하다. 일반 primitive component의 BodyInstance 설정은 별도 안전한 adapter를 거쳐 지원한다.
- 시뮬레이션 중 skeletal/instanced/welded body, 지원 범위 밖 native 물리 자원과 `UPhysicsConstraintComponent`. ConstraintInstance의 live handle은 일반 reflected 복사로 안전하게 분리할 수 없다.
- custom MID subclass, parameter collection/UserSceneTexture/Nanite override/layered sparse-volume 등 전용 재질 어댑터가 없는 상태.

timer 실행 상태, delegate 바인딩, coroutine/latent 실행, native 비-UPROPERTY 데이터 전체, 숨은 참조를 포함한 native 직렬화 컨테이너, 외부 파일·네트워크·GPU handle은 자동 snapshot 대상이 아니다. interface hook은 class별 **설정 복원** 확장점이며 거부된 구조·물리·재질 종류를 무조건 통과시키는 옵션이 아니다. 이 서비스는 Unreal Editor 전체 DuplicateActor 동작 또는 임의 프로세스 메모리 복제를 보장하지 않는다.

## 현재 편집기 범위와 검증 진입점

- 단일 Actor 선택과 World Transform 편집 UI를 제공한다. 다중 선택, 상대 Transform UI, 임의 reflected 속성 전체 편집은 없다.
- Undo/Redo 입력 context·확장 함수는 있지만 범용 undo stack, 프로젝트 저장/로드, 새 BP asset 생성·컴파일·저장은 제공하지 않는다.
- 복제는 runtime Actor instance 작업이다. 멀티플레이 권한·replication·클라이언트 소유 정책이나 비동기 API는 제공하지 않는다.
- 모니터는 제한된 최근 진단 이력이다. 영구 감사 로그·전체 변경 이력 저장소가 아니다.

관련 automation 이름은 `OWT.EventCore`, `OWT.Runtime.AttributeContracts`, `OWT.Runtime.AttributeDetailsContracts`, `OWT.Runtime.ObservedState`, `OWT.Runtime.NativeInput`이다. 프로젝트 모듈의 `OWT.Runtime.Attributes`는 `/Game/VTBOWT` 샘플 BP를 요구한다. 고급 복제 fixture는 Editor 커맨드렛 `ValidateOWTAttributes`, 기존 BP 입력/맵 회귀는 `CreateOWTInput`/`CreateOWTLevel -ValidateOnly`를 사용한다.

`Saved/PortabilityProbe`는 독립 소비 프로젝트 fixture다. 자체 게임 모듈이 `OWTEventCore`·`OWTRuntimeDuplication`만 직접 의존하고 plain Actor를 소유자로 쓰는 `OWT.Portability.IndependentHost` 테스트를 포함한다. `CopyPlugin.ps1`은 준비된 plugin Source/Config/Content/descriptor/README만 새 목적지에 복사한다. fixture README의 실행 순서와 요구사항 문서의 실제 로그를 구분해 확인한다. 화면상의 수치 입력, 슬라이더, 포커스, F2/F3, 카메라와 expanded JSON은 headless 계약 검사 외에 실제 창에서도 확인한다.
