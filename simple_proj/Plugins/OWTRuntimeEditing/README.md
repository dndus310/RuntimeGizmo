# OWT Runtime Editing

Unreal Engine 5.7용 Runtime 플러그인이다. ITF 기반 AttributeEditMode와 등록형 도구, Actor 상태/Transform 편집, 내부 Actor·PCG 계층 복제, Gizmo와 상태 모니터 연결을 제공한다. Pub-Sub은 독립 모듈로 유지하고 상태 뷰어는 별도 `OWTStateMonitor` 플러그인을 사용한다.

| 모듈 | 용도 |
|---|---|
| `OWTEventCore` | 소유 객체별 JSON Pub/Sub와 제한된 진단 이력 |
| `VTBOWTEditor` | Runtime mode/ITF 도구, 내부 Duplication·PCG adapter, typed 관찰 상태, Gizmo, 입력, Details/Events UI |

## 다른 프로젝트에 설치

대상 UE5.7 C++ 프로젝트의 `Plugins/OWTRuntimeEditing`에 이 폴더의 **descriptor, Source, Config, Content, README**를 함께 복사한다. Content에는 런타임 Gizmo 재질이 있고 `Config/Engine.ini`에는 이전 모듈 이름의 Core Redirect가 있다. 원본의 Binaries/Intermediate나 샘플 프로젝트 `/Game/VTBOWT`를 복사할 필요는 없다.

`Plugins/OWTStateMonitor`도 함께 설치한다. 이 독립 Runtime 플러그인은 관찰 구조체를 읽어 트리·JSON·변경 이력을 표시하며 AttributeEditor·PCG·Pub-Sub에 의존하지 않는다. 모니터만 사용할 프로젝트에서는 `OWTStateMonitor`만 설치할 수 있다.

플러그인을 활성화하고 대상 프로젝트의 Editor/Game target을 빌드한다. Enhanced Input·PCG·OWTStateMonitor 의존성은 descriptor에 선언되어 있다. 코드에서 사용하는 모듈을 소비 모듈의 Build.cs에 추가한다.

```csharp
PrivateDependencyModuleNames.AddRange(new string[]
{
    "VTBOWTEditor"
});
```

범용 이벤트만 사용하면 `OWTEventCore`만 추가한다. 소비 모듈의 public 헤더가 의존 타입을 노출하면 해당 의존성은 PublicDependencyModuleNames에 둔다. 기존 Core/CoreUObject/Engine 의존성은 유지한다. `VTBOWTEditor`는 이름과 달리 Runtime 모듈이며 UEdMode/ModelingToolsEditorMode/UnrealEd를 Game target에 요구하지 않는다.

구 버전 `OWTRuntimeDuplication` Build.cs dependency는 `VTBOWTEditor`로 바꾼다. `Duplication/...` 헤더 경로와 UClass 이름은 유지한다. `Config/Engine.ini`의 `[CoreRedirects]`는 표준 Engine 설정으로 로드되어 이전 `/Script/OWTRuntimeDuplication` 참조와 이동한 이벤트 타입을 현재 모듈로 연결한다. `Config/DefaultOWTRuntimeEditing.ini`는 새 설정 위치를 안내하는 주석만 있으므로 이 파일만 복사하면 이전 클래스 경로를 해석할 수 없다. 기존 BP의 editor/subsystem 클래스 경로도 유지한다. Core Redirect가 C++ Build.cs 변경을 대신하지는 않는다.

## 기본 편집기 실행

맵의 GameMode를 native **VTBOWTEditorGameMode**로 지정한다. 기본 Spectator/Controller/AttributeEditor가 연결되며 입력 매핑이 없으면 native Enhanced Input action/매핑을 생성한다. BP Pawn, 프로젝트 입력 에셋 또는 UMG 에셋은 기본 실행의 필수 조건이 아니다. 기존 BP MappingContext를 지정한 경우 그 BP의 편집 context 전달/카메라 action 연결을 사용한다.

| 키/조작 | 동작 |
|---|---|
| F2 | 편집 ON/OFF |
| Actor 클릭 | 선택 및 Details/Gizmo 표시 |
| 수치 입력·Enter / 수치 드래그 | World Transform 편집 |
| Shift / Ctrl + 수치 드래그 | 빠르게 / 미세하게 조절 |
| 수치 드래그 중 Escape | 조작 취소 |
| 월드 W / E / R | 이동 / 회전 / 크기 Gizmo |
| 월드 Ctrl+` | World / Local 좌표계 |
| 월드 Ctrl+D / Tools의 복제 도구 | 기본 월드 +X 100cm 복제 요청. authored 구성 commit 후 복제본 선택 |
| 우클릭 + WASD / 마우스 | 카메라 이동 / 회전 |
| F3 / Monitor 탭 | 편집 OFF에서도 상태 모니터 열기 |

상태 요약은 편집 활성, 선택 이름·ID, mode lifecycle/active tool, Gizmo 좌표계, 수정 중 여부, baseline 대비 변경, revision을 보여준다. Details의 Tools 목록은 registry의 ID·표시명·가능 여부·비활성 이유로 자동 구성한다. Accept/Cancel은 도구가 해당 종료 방식을 제공할 때만 켜진다.

모니터의 Source에서 **Current editor state** 또는 **JSON event journal**을 선택한다. Current는 Selection·Mode·Tools·DuplicationOperations·ProceduralComponents 구조체 필드를 표시하고 실제 typed 저장소에서 읽는다. 복제의 `Committed`는 PCG 결과 준비 완료를 뜻하지 않는다. Events는 최근 256개 이벤트의 reflected 기록과 JSON payload 문자열을 그대로 표시하며 Current 상태의 원본으로 사용하지 않는다.

필드를 트리로 펼치고 검색하거나 같은 관찰값을 JSON으로 전환·복사한다. 변경된 값은 기호·색상과 이전 값 tooltip으로 구분한다. Pause history는 필드 변화의 이력 표시만 멈추며 Current는 계속 갱신한다. Clear history는 모니터 이력만 지우고 Actor 상태·원본 이벤트 저널을 유지한다. Acknowledge는 현재 변경 강조를 지운다. 이력은 필드 변화 기록으로서 과거 Actor를 복원하는 Undo 스택과 구별된다. 편집 OFF에서도 모니터는 계속 사용할 수 있다.

패널 hover/focus/drag 중에는 월드 선택·편집·카메라 입력을 막고 F2/F3은 유지한다. F3을 다시 누르면 편집 중에는 Details, 편집 OFF이면 패널 닫기로 전환한다. Blueprint에는 위젯 `SetMonitorVisible`, `ToggleMonitor`, Controller `ToggleEventMonitor`가 있다.

## AttributeEditor 상태/API

헤더: `VTBAttributeEditor.h`, `Events/OWTAttributeTypes.h`, `VTBOWTEditorSubsystem.h`.

```cpp
AVTBAttributeEditor* Editor = World->GetSubsystem<UVTBOWTEditorSubsystem>()->GetAttributeEditor();
if (Editor)
{
    const FOWTAttributeSnapshot Expected = Editor->GetSnapshot();
    Editor->RequestTransformField(Expected, EOWTTransformField::LocationX, 120.0,
                                  EOWTTransformEditPhase::Commit, FGuid::NewGuid());
}
```

실제 Actor/SubSystem 상태를 관찰한 내부 typed StateStore가 UI의 원본이다. GetSnapshot은 마지막 관찰값의 복사본이다. JSON 파싱·복사본 변경·거부 요청으로 저장소를 갱신할 수 없다. 선택 Actor를 외부에서 직접 변경하면 다음 Subsystem Tick 또는 RefreshSelectedTransform에서 관찰한다.

`Subscribe`/`SubscribeDynamic`은 `Event(FName), Json(FString)` 알림과 현재 snapshot을 전달한다. 상태 알림을 받으면 GetSnapshot을 읽고 RequestRejected는 실패로 처리한다. Subscribe의 초기 콜백은 함수 반환 전에 실행될 수 있다. 종료할 때 Unsubscribe(handle)를 호출한다.

명령은 RequestTransformField/RequestDuplicate 또는 PublishRequest다. 직접 JSON 요청은 TransformEditRequested/DuplicateRequested이며 schemaVersion=1, editorId, objectId, selectionRevision, 새 requestId GUID, source를 요구한다. Transform 요청에는 operationId/phase/space=World/property/value도 포함한다. 드래그는 시작 snapshot과 operationId를 Begin/Update/Commit/Cancel 동안 유지한다. 반환 bool도 확인한다. JSON은 최대 65,536 문자다.

`hasChanges`는 Transform baseline 비교와 새 복제본 표시다. MarkSelectionBaseline으로 현재 실제 Transform을 기준으로 수락한다. 파일 저장 상태나 모든 BP 변수의 dirty 여부는 아니다. GetMonitorEntries와 GetLatestEventSequence는 읽기용 진단 API다.

`GetModeSnapshot()`, `GetDuplicationOperations()`, `GetProceduralComponents()`는 서로 다른 상태를 typed 복사본으로 반환한다. UI는 JSON 알림을 받으면 이 API를 다시 조회한다. `RequestDuplicate`의 bool은 접수 여부다. 옵션과 작업 ID가 필요하면 `BeginDuplicateOperation(Expected, Options)`를 사용하고 작업 phase를 관찰한다. 생성 결과가 필요한 코드는 해당 PCG component가 `Ready`가 될 때까지 기다린다.

직접 JSON에서 복제 옵션을 지정하려면 schemaVersion=2와 hierarchyScope/generationPolicy/worldOffset.{x,y,z}를 사용한다. schema 1에 옵션 필드를 추가하면 SchemaRequired로 거부한다. typed BeginDuplicateOperation은 schema 2 요청을 구성하며 NaN/무한대와 enum sentinel/범위 밖 값은 접수하지 않는다.

## Runtime mode와 도구 확장

`UVTBOWTEditorSubsystem`이 `UOWTAttributeEditMode`를 소유하고 mode가 ITF context, session context, selection, tool registry와 내부 duplicator를 관리한다. `AVTBAttributeEditor`는 기존 BP/API facade로 남아 Notifications·StateStore를 소유하고 실제 Actor/mode 상태를 관찰한다. `UVTBOWTObjectEditMode` 경로는 기존 context/BP 호환을 위해 유지한다.

`Extensions/OWTToolDescriptor.h`의 `FOWTToolDescriptor`에는 ToolId, Label, Category, Description, BuilderClass와 history/mesh rendering 요구가 있다. mode의 `RegisterTool(Descriptor, ProviderId, OutToken, OutError)`로 등록하고 종료 시 `UnregisterProvider(ProviderId, OutError)`로 제거한다. 등록·제거 반환값과 오류를 확인한다. 도구는 ITF `UInteractiveToolBuilder`/`UInteractiveTool` 생명주기를 따르며 host 서비스는 context store로 전달한다. 지원되지 않는 capability를 요구하는 도구는 활성화하지 않는다.

Enter된 mode에 `RegisterTargetFactory(Factory, ProviderId, OutError)`와 `RegisterContextObject(Service, ProviderId, OutError)`도 등록할 수 있다. context service는 `NewObject<UMyService>(Mode)`처럼 해당 mode에 속해야 하며 같은 service type 중복은 거부한다. 도구는 `ToolManager->GetContextObjectStore()->FindContext<UMyService>()`로 조회한다. 기본 `UOWTAttributeEditSessionContext::GetMode()`는 정확한 소유 mode를 반환한다.

모듈 provider는 `Extensions/OWTAttributeModeExtension.h`의 `IOWTAttributeModeExtension`에 `GetProviderId()`/`RegisterTools(Mode)`를 구현하고, `IModularFeatures`에 `GetModularFeatureName()`으로 등록한다. 이후 mode Enter에서 등록 callback을 호출한다. 이미 Enter된 mode에는 명시적으로 등록을 적용해야 한다. mode는 provider가 등록한 builder·도구·factory·service UObject를 소유한다.

provider 종료 시 modular feature 등록과 각 mode의 `UnregisterProvider`를 해제한다. `CanUnloadProvider(ProviderId)`가 false라면 mode가 추적하는 builder/tool/target factory/context service가 아직 살아 있으므로 코드를 유지한다. 외부 참조·GC 전 객체 때문에 등록 해제 직후에도 false일 수 있다. true는 provider가 별도로 만든 target/native 객체·외부 callback까지 모두 사라졌다는 보장이 아니며 이 수명도 provider가 관리한다. `OWT.Core`는 해제 대상이 아니다.

facade `GetAvailableTools()`는 표시와 실행 가능 상태를, `RequestStartTool(ToolId)`는 시작 요청을 제공한다. `CanAcceptActiveTool()`/`CanCancelActiveTool()` 확인 후 `RequestEndTool(bAccept)`를 호출한다. 기존 선택/TRS와 복제도 같은 도구 실행 경로를 사용한다. 새로운 기능을 추가할 때 UI나 입력 분기에 특정 tool 클래스·asset 경로를 추가하지 않는다.

native GameMode의 `AOWTRuntimeToolsHUD`가 실제 game viewport에서 mode Render/DrawHUD를 전달한다. 커스텀 HUD는 이 클래스를 상속하거나 DrawHUD에서 `Mode->RenderTools(Canvas, PlayerOwner)`를 연결한다. `OWT.Runtime.ViewportRendering`은 active RHI의 실제 게임 프레임에서 두 callback을 확인하며 NullRHI에서는 명시적으로 skip한다. headless skip은 화면 연결의 성공 검증이 아니다.

## 범용 이벤트와 저수준 복제 API

범용 이벤트 헤더는 `Events/OWTNotificationCenter.h`다. 복제의 기본 사용 경로는 AttributeEditor → mode → Duplicate tool이다. `VTBOWTEditor` 안의 `Duplication/OWTRuntimeActorDuplicator.h`는 기존 소유 UObject 기반 저수준 API도 제공한다. 직접 사용할 때는 소유 클래스가 서비스 수명과 Tick을 관리한다.

```cpp
UPROPERTY(Transient)
TObjectPtr<UOWTNotificationCenter> Events;
UPROPERTY(Transient)
TObjectPtr<UOWTRuntimeActorDuplicator> Duplicator;
```

초기화/사용 예시 (`this`는 소유 UObject):

```cpp
Events = NewObject<UOWTNotificationCenter>(this);
Events->Initialize(this);
// Subscribe(this, FOWTAttributeEventNative::CreateUObject(this, &UMyService::OnEvent))
Events->Publish(TEXT("MyTopic"), TEXT("{\"value\":42}"));

Duplicator = NewObject<UOWTRuntimeActorDuplicator>(this);
Duplicator->Initialize(this); // 소유자가 살아 있는 UWorld를 제공해야 한다.
FString Error;
AActor* Copy = Duplicator->DuplicateActor(Source, FVector(100, 0, 0), Error);
```

실제 호출부에서는 Initialize/Publish 반환값과 DuplicateActor의 nullptr/Error를 처리한다. 저수준 DuplicateActor의 반환은 authored 구성의 동기 결과이며 PCG 준비 완료가 아니다. NewObject의 정확한 Outer와 Initialize의 Owner가 같아야 한다. 소유자는 weak로 보관하므로 **Outer만 지정하지 말고 서비스 참조를 UPROPERTY로 유지**한다. 모든 호출은 게임 스레드에서 한다. procedural 상태 관찰을 위해 소유 Tick에서 Duplicator->Tick(DeltaTime)을 호출하고 종료 시 이벤트 구독 해제와 Shutdown, 복제 서비스 Deinitialize를 호출한다.

범용 center는 arbitrary JSON topic을 전달하며 업무 상태를 해석하지 않는다. Publish는 전달+OUT journal, RecordEvent는 전달 없이 IN/OUT journal만 기록한다. 구독자는 weak reference로 관리한다. 기본 이력 256개, 최대 설정 1024개, journal payload 최대 16,384 문자이며 truncated/invalid flag를 제공한다. ClearEventHistory 후에도 sequence는 되돌리지 않는다. 별도 UI polling은 GetHistoryRevision으로 clear/용량 변경까지 감지할 수 있다.

복제는 원본과 같은 클래스/Level을 사용하고 루트를 외부 부모에서 분리해 caller의 world offset을 한 번 적용한다. reflected 사용자 값, component/instanced UObject, 내부 참조, 관리 ChildActorComponent 계층을 복원한다. 외부 Actor/공유 에셋은 공유 참조로 유지한다. 이름은 끝 `_숫자`를 family로 묶어 Level 내 최대 번호 다음으로 만든다 (`Chair → Chair_1`, `Chair_4 → Chair_5`, 복제본 → `Chair_6`). helper counter는 삭제로 감소하지 않고 Deinitialize에서 초기화된다.

rootless Actor도 지원한다. Pawn은 미점유이며 자동 player/AI possession이 모두 Disabled여야 한다. managed child만 직접 복제하지 말고 부모를 복제한다. 지원 adapter는 소유 기본 MID의 일반 parameter와 PhysMaterial/PhysicalMaterialMap override, unwelded 비-instanced StaticMesh 단일 body의 물리 설정/속도/awake 상태를 복원한다.

class별 native 설정은 `Duplication/OWTRuntimeDuplicationParticipant.h`의 `IOWTRuntimeDuplicationParticipant`로 확장한다. destination의 BlueprintNativeEvent `RestoreRuntimeDuplicateState(SourceObject, DuplicatedObjects, OutError)`에서 map으로 내부 참조를 바꾸고 자원을 새로 생성한다. 기존 root + 관리 ChildActor는 해당 복원 뒤 BeginPlay를 실행한다. false는 새 루트/관리 child를 정리한다. Construction/초기화/hook의 외부 부작용은 rollback하지 않는다. 일반 authored attached Actor의 BeginPlay 경계는 아래 설명을 따른다.

복제 factory와 필드 정책은 `IOWTDuplicationAdapterProvider`를 Modular Feature에 등록하여 확장한다. 각 Duplicator는 Initialize에서 Provider를 찾아 owner registry에 설치하고 작업 시작 시 registry를 복사한다. 진행 중인 복제의 factory callback이 owner registry를 바꿔도 해당 복제는 기존 설정을 사용하고 다음 작업부터 변경을 반영한다. Provider ID는 모듈별로 고유하고 안정적인 이름을 사용한다.

`GetAdapterRegistry().GetPropertyPolicies()`에 프로젝트 파생 클래스 정책을 등록할 수 있다. 정책은 `TOptional<bool>`을 반환하며 true는 일반 필드 복사, false는 제외, unset은 상위 클래스 정책으로 이어진다. 공통 transient·delegate 제외는 항상 먼저 적용된다. Engine Provider가 Actor·Component·Scene·MID의 수명 규칙을 공급하고 PCG Provider가 PCG adapter를 설치한다. PCG authoring 설정은 CPF_Edit 기반 reflection으로 전송하며 Graph·Policy·생성 자원은 전용 엔진 API 계약으로 다룬다.

Provider factory와 정책이 Provider 객체의 주소를 캡처하지 않도록 작성하고, 소비 세션 동안 해당 모듈을 유지한다. Modular Feature 해제는 새 세션의 발견을 막으며 이미 등록된 세션의 factory를 자동 제거하지 않는다. 동일한 클래스의 기존 정책 등록은 false를 반환하므로 기본 엔진 클래스를 대체할 때에는 해당 수명 규칙 전체를 책임져야 한다. 일반 확장은 프로젝트 파생 클래스의 정책을 추가하고 unset으로 기본 규칙에 이어지는 구성을 사용한다.

## Authored 계층과 PCG

`FOWTDuplicationOptions` (`Duplication/OWTDuplicationRequest.h`)는 `WorldOffset`, `HierarchyScope`, `GenerationPolicy`를 제공한다. mode의 기본 `AuthoredHierarchy`는 원본 + 관리 ChildActor + 작성자가 부착한 Actor 계층을 포함하고 PCG managed output을 제외한다. `ActorAndManagedChildren`는 기존 root + 관리 ChildActor 범위다. 저수준 `DuplicateActor`는 이 기존 범위를 유지하며, 명시적인 새 옵션은 `DuplicateActorWithOptions(Source, Options, OperationId, Error)`로 전달한다.

PCG 외부 graph asset, 소유 GraphInstance·지원 parameter, component 설정을 복원하고 생성 자원은 새로 만든다. custom scheduling policy의 reflected·instanced 참조는 복제 그래프로 재매핑하며 opaque native payload에는 별도 adapter가 필요하다. 원본의 출력 ISM/spawned Actor·cache·task ID를 복제본의 소유물로 공유하지 않는다. PCG adapter는 편집 모듈의 Private/Duplication 안에 있으며 별도 bridge 플러그인을 설치할 필요가 없다. PCG는 현재 descriptor의 필수 의존성이다.

구성 복원 뒤 adapter의 `PrepareCommit`에서 component observation·generation delegate와 초기 상태를 먼저 설치한다. authored commit 알림 이후 `Commit`이 활성화·scheduler 등록·generation을 요청한다. `ObjectDuplicated` callback에서도 이미 component별 상태를 조회할 수 있고 빠르게 완료되는 generation 이벤트를 놓치지 않게 하는 순서다. 자체 adapter도 이 관찰 설치 경계를 사용한다.

GenerateOnLoad는 authored commit 후 생성한다. GenerateOnDemand는 원본이 generated인 경우 기본 `RegenerateIfSourceGenerated` 정책으로 요청하고, `KeepUnGenerated`는 OnDemand 자동 요청만 생략한다. Runtime generation은 PCG scheduler에 등록·갱신하며 source가 없으면 `WaitingForGenerationSource`로 유지된다. 이 상태를 고정 timeout 실패나 준비 완료로 바꾸지 않는다. partition/runtime 정책에는 해당 world에 등록된 PCGWorldActor가 필요하며 없으면 `MissingPCGWorldActor`로 거부한다. 비활성 component는 비활성을 보존한다.

모니터의 authored `Committed`와 component별 `Ready`는 별개다. `GetProceduralComponents()`의 operation/component ID, state, generation attempt, reason을 관찰한다. Ready는 현재 등록된 generation workload와 관련 local component의 생성 완료·idle 상태이며 미래 scheduler cell 전체의 완료 보장은 아니다. source 이동에 따른 cleanup은 `CleaningUp`/`Cleaned`, 재진입 생성은 새 generation attempt로 관찰한다. 같은 Actor의 한 PCG component가 Ready여도 다른 component는 대기·실패일 수 있다. 서비스 `Deinitialize`는 관찰을 종료하며 commit된 Actor를 삭제하지 않는다.

생성/cleanup 중인 source, 지원 밖 native parameter, authored 참조가 PCG managed output을 가리키는 경우, 관리 생성물 아래 authored 부착, 소유 graph definition 자체는 구체적 사유로 거부한다. 외부 graph asset과 소유 GraphInstance는 지원한다. Construction에서 복원 전에 PCG generation이나 cleanup을 직접 실행하는 클래스에는 협력하는 초기화/복제 adapter가 필요하다. 대상의 조기 cleanup을 감지하면 `DestinationPCGLifecycleViolation`으로 거부하며 사용자 코드의 외부 부작용은 rollback하지 않는다. 일반 authored attached Actor의 BeginPlay는 전체 sibling graph 연결보다 먼저 실행될 수 있으므로 전체 준비는 operation commit으로 판단한다. PCG generation은 전체 authored commit까지 억제한다.

별도 native 복제 확장은 `Duplication/OWTDuplicationAdapter.h`의 `IOWTDuplicationAdapter`와 `Duplicator->GetAdapterRegistry().Register(SupportedClass, Factory, Role)`로 등록한다. Role 기본값은 `EOWTDuplicationAdapterRole::Primary`다. 해제는 `Unregister(SupportedClass, Role)`이며 동일 Class+Role 중복 등록은 false다. 같은 클래스에 Primary와 Auxiliary를 각각 하나 등록할 수 있다.

core는 authored Actor/component마다 가장 구체적인 Primary 하나와 적용 가능한 모든 Auxiliary를 선택해 `ValidateObject`/`CaptureObject`를 호출한다. 기존 Actor 훅은 기본 구현을 통해 유지된다. Primary만 `OwnsProperty`/`OwnsObject`로 복원 책임을 선언하며 Auxiliary의 ownership 또는 Primary 간 중복 ownership은 `AdapterOwnershipConflict`로 거부한다. Auxiliary는 검증·관찰·정책을 함께 적용하는 확장점이다. 선택되지 않은 adapter는 world ownership indexing에만 참여한다. adapter는 작업마다 생성되며 capture/restore/reference validation/commit/rollback을 담당한다. 실패 정리는 대상 작업의 새 객체·자원에 한정하고 원본 PCG 결과를 cleanup하지 않는다.

네트워크 월드, 관리 Actor(AInfo/Controller/Brush), 다른 월드/파괴 중 객체, 지원 instancing 계약 없는 소유 객체, simulated skeletal/instanced/welded body와 live constraint handle을 가진 UPhysicsConstraintComponent는 거부한다. custom MID subclass 및 parameter-collection/UserSceneTexture/Nanite override/layered sparse-volume 상태에는 별도 adapter가 필요하다. timer/delegate/latent 실행·임의 native 메모리·숨은 참조가 있는 native 직렬화 컨테이너·외부 handle 전체는 snapshot하지 않는다. participant hook은 이 거부 조건을 우회하지 않는다. 일반 attachment 트리와 ChildActorComponent 관리 계층은 구분된다.

전용 adapter가 없는 opaque FInstancedStruct/FInstancedPropertyBag 및 raw FBodyInstance/FConstraintInstance/FTickFunction 속성은 사전 검사에서 거부한다. PCG의 지원 parameter bag은 PCG adapter가 참조를 점검·재매핑하며 live handle까지 허용하지 않는다. 일반 primitive component의 BodyInstance 설정은 안전한 별도 adapter로 지원한다. participant hook은 거부된 구조를 통과시키는 방법이 아니라 클래스의 추가 native 설정을 복원하는 확장점이다.

## 범위 및 추가 자료

기본 UI는 단일 Actor의 World Transform용이다. 다중 선택·임의 속성 전체 UI·범용 Undo stack·저장/로드·새 BP asset 생성·멀티플레이 정책은 포함하지 않는다. 이벤트 journal은 제한된 진단 이력이며 영구 감사 로그가 아니다.

현재 plugin automation 이름은 다음과 같다. 실행 진입점 안내이며 결과 보고가 아니다.

| 그룹 | 테스트 |
|---|---|
| 이벤트 | `OWT.EventCore.OwnerIsolation`, `OWT.EventCore.CallbackMutation`, `OWT.EventCore.BoundedHistory` |
| 상태·UI·입력 | `OWT.Runtime.AttributeContracts`, `OWT.Runtime.AttributeDetailsContracts`, `OWT.Runtime.ObservedState`, `OWT.Runtime.NativeInput` |
| mode·복제 | `OWT.Runtime.ModeLifecycle`, `OWT.Runtime.AuthoredHierarchy`, `OWT.Runtime.DuplicationFacade`, `OWT.Runtime.AdapterRegistryResolution`, `OWT.Runtime.AdapterRegistryDispatch` |
| PCG | `OWT.Runtime.PCGConfiguration`, `OWT.Runtime.PCGRegeneration`, `OWT.Runtime.PCGRuntimePolicy`, `OWT.Runtime.PCGSchedulerHierarchy`, `OWT.Runtime.PCGPolicyReferences`, `OWT.Runtime.PCGCookedBlueprintHierarchy`, `OWT.Runtime.PCGChildActorOnLoad` |
| 실제 viewport | `OWT.Runtime.ViewportRendering`, `OWT.Runtime.ViewportMonitorCapture` — active RHI 게임 프레임 필요, NullRHI에서는 skip |

`ViewportMonitorCapture`는 실제 native Details·Monitor UI를 `Saved/Screenshots/OWT_Details_<ID>.png`와 `OWT_Monitor_<ID>.png`로 저장한다. 파일 생성은 시각적 잘림·가독성 검사를 대신하지 않으므로 PNG를 열어 확인한다.

cooked Blueprint PCG 검사는 `Content/Tests`와 Config/Game.ini의 cook 설정을 필요로 한다. 배포할 때 Content·Config를 함께 복사한다. 원본 개발 프로젝트의 Editor commandlet `CreateOWTInput -CreatePCGFixture`로 fixture를 재생성할 수 있으며 이 commandlet은 소비 프로젝트 Runtime 의존성이 아니다. 원본 host의 `OWT.Runtime.Attributes`는 `/Game/VTBOWT` 샘플 BP를 요구하므로 plugin-only 검사와 구분한다. `OWT.Portability.IndependentHost`는 별도 소비 프로젝트 fixture의 테스트다.

원본 저장소의 현재 독립 소비 프로젝트 fixture는 `Saved/PortabilityProbeV3/OWTPortabilityProbe.uproject`다. 새 목적지에는 같은 폴더의 `CopyPlugin.ps1`로 plugin을 복사하고 해당 프로젝트의 Editor/Game target을 빌드한다. helper는 기존 plugin 목적지를 덮어쓰지 않으며 `Saved/PortabilityProbe`의 이전 결과를 v3 결과로 사용하지 않는다. fixture 경로 안내 자체가 검증 통과를 뜻하지는 않는다.

원본 저장소에는 [v3 설계 명세](../../Docs/AttributeEditMode_ITF_Spec.md), [상세 사용법](../../Docs/RuntimeAttributeEditor_Usage.md), [v2 명세·과거 검증 기록](../../Docs/RuntimeAttributeEditor_Requirements.md)이 있다. 위 설명은 구현 계약이며 특정 환경에서의 테스트 통과 주장과는 별개다. 이 README만 복사해도 설치·native 실행·기본 API 사용 정보를 확인할 수 있다.
