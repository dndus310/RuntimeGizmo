# OWT Runtime Editing

Unreal Engine 5.7용 Runtime 플러그인이다. 선택 Actor의 상태/Transform 편집, Gizmo와 Events 모니터를 제공한다. 범용 이벤트와 Actor 복제 기능은 별도 모듈로 직접 사용할 수 있다.

| 모듈 | 용도 |
|---|---|
| `OWTEventCore` | 소유 객체별 JSON Pub/Sub와 제한된 진단 이력 |
| `OWTRuntimeDuplication` | standalone 월드 Actor 설정·component·관리 ChildActor 계층 복제 |
| `VTBOWTEditor` | typed 관찰 상태, 선택/TRS/복제 요청, Gizmo, 입력, Details/Events UI |

## 다른 프로젝트에 설치

대상 UE5.7 C++ 프로젝트의 `Plugins/OWTRuntimeEditing`에 이 폴더의 **descriptor, Source, Config, Content, README**를 함께 복사한다. Content에는 런타임 Gizmo 재질이 있고 Config에는 이전 모듈 이름의 Core Redirect가 있다. 원본의 Binaries/Intermediate나 샘플 프로젝트 `/Game/VTBOWT`를 복사할 필요는 없다.

플러그인을 활성화하고 대상 프로젝트의 Editor/Game target을 빌드한다. Enhanced Input 의존성은 descriptor에 선언되어 있다. 코드에서 사용하는 모듈을 소비 모듈의 Build.cs에 추가한다.

```csharp
PrivateDependencyModuleNames.AddRange(new string[]
{
    "OWTEventCore", "OWTRuntimeDuplication"
});
```

편집기까지 사용하면 `VTBOWTEditor`를 추가한다. 소비 모듈의 public 헤더가 의존 타입을 노출하면 해당 의존성은 PublicDependencyModuleNames에 둔다. 기존 Core/CoreUObject/Engine 의존성은 유지한다.

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
| 월드 Ctrl+D / Duplicate actor | 기본 월드 +X 100cm 복제 후 복제본 선택 |
| 우클릭 + WASD / 마우스 | 카메라 이동 / 회전 |
| F3 / Events 탭 | 편집 OFF에서도 Events 모니터 열기 |

상태 요약은 편집 활성, 선택 이름·ID, 모드/좌표계, 수정 중 여부, baseline 대비 변경, revision을 보여준다. Events는 최근 256개를 최신 순으로 보여주며 sequence/UTC/topic/방향/source/requestId/operationId/실패 사유와 펼칠 수 있는 JSON이 있다. Pause는 이력 표시만 멈춘다. Clear view는 현재 위젯에서 과거 항목을 숨기며 원본 journal을 지우지 않는다. Filter는 topic/ID/payload/sequence를 검색한다. 편집 OFF의 모니터가 수정 권한을 켜지는 않는다.

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

## 이벤트와 복제를 독립 서비스로 사용

범용 헤더는 `Events/OWTNotificationCenter.h`, `Duplication/OWTRuntimeActorDuplicator.h`다. AttributeEditor 인스턴스가 필요하지 않다. 소유 클래스에서 서비스 수명을 유지한다.

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

실제 호출부에서는 Initialize/Publish 반환값과 DuplicateActor의 nullptr/Error를 처리한다. NewObject의 정확한 Outer와 Initialize의 Owner가 같아야 한다. 소유자는 weak로 보관하므로 **Outer만 지정하지 말고 서비스 참조를 UPROPERTY로 유지**한다. 모든 호출은 게임 스레드에서 한다. 종료 시 이벤트 구독 해제와 Shutdown, 복제 서비스 Deinitialize를 호출한다.

범용 center는 arbitrary JSON topic을 전달하며 업무 상태를 해석하지 않는다. Publish는 전달+OUT journal, RecordEvent는 전달 없이 IN/OUT journal만 기록한다. 구독자는 weak reference로 관리한다. 기본 이력 256개, 최대 설정 1024개, journal payload 최대 16,384 문자이며 truncated/invalid flag를 제공한다. ClearEventHistory 후에도 sequence는 되돌리지 않는다. 별도 UI polling은 GetHistoryRevision으로 clear/용량 변경까지 감지할 수 있다.

복제는 원본과 같은 클래스/Level을 사용하고 루트를 외부 부모에서 분리해 caller의 world offset을 한 번 적용한다. reflected 사용자 값, component/instanced UObject, 내부 참조, 관리 ChildActorComponent 계층을 복원한다. 외부 Actor/공유 에셋은 공유 참조로 유지한다. 이름은 끝 `_숫자`를 family로 묶어 Level 내 최대 번호 다음으로 만든다 (`Chair → Chair_1`, `Chair_4 → Chair_5`, 복제본 → `Chair_6`). helper counter는 삭제로 감소하지 않고 Deinitialize에서 초기화된다.

rootless Actor도 지원한다. Pawn은 미점유이며 자동 player/AI possession이 모두 Disabled여야 한다. managed child만 직접 복제하지 말고 부모를 복제한다. 지원 adapter는 소유 기본 MID의 일반 parameter와 PhysMaterial/PhysicalMaterialMap override, unwelded 비-instanced StaticMesh 단일 body의 물리 설정/속도/awake 상태를 복원한다.

class별 native 설정은 `Duplication/OWTRuntimeDuplicationParticipant.h`의 `IOWTRuntimeDuplicationParticipant`로 확장한다. destination의 BlueprintNativeEvent `RestoreRuntimeDuplicateState(SourceObject, DuplicatedObjects, OutError)`는 전체 그래프 재매핑 후, 계층 BeginPlay 전에 호출된다. map으로 내부 참조를 바꾸고 자원을 새로 생성한다. false는 새 루트/관리 child를 정리한다. Construction/초기화/hook의 외부 부작용은 rollback하지 않는다.

네트워크 월드, 관리 Actor(AInfo/Controller/Brush), 다른 월드/파괴 중 객체, 지원 instancing 계약 없는 소유 객체, simulated skeletal/instanced/welded body와 live constraint handle을 가진 UPhysicsConstraintComponent는 거부한다. custom MID subclass 및 parameter-collection/UserSceneTexture/Nanite override/layered sparse-volume 상태에는 별도 adapter가 필요하다. timer/delegate/latent 실행·임의 native 메모리·숨은 참조가 있는 native 직렬화 컨테이너·외부 handle 전체는 snapshot하지 않는다. participant hook은 이 거부 조건을 우회하지 않는다. 일반 attachment 트리와 ChildActorComponent 관리 계층은 구분된다.

opaque FInstancedStruct/FInstancedPropertyBag 및 raw FBodyInstance/FConstraintInstance/FTickFunction을 포함한 reflected 속성도 사전 검사에서 거부한다. 해당 타입은 모듈의 전용 adapter가 필요하다. 일반 primitive component의 BodyInstance 설정은 안전한 별도 adapter로 지원한다. participant hook은 거부된 구조를 통과시키는 방법이 아니라 클래스의 추가 native 설정을 복원하는 확장점이다.

## 범위 및 추가 자료

기본 UI는 단일 Actor의 World Transform용이다. 다중 선택·임의 속성 전체 UI·범용 Undo stack·저장/로드·새 BP asset 생성·멀티플레이 정책은 포함하지 않는다. 이벤트 journal은 제한된 진단 이력이며 영구 감사 로그가 아니다.

플러그인 자체 automation은 OWT.EventCore, OWT.Runtime.AttributeContracts, OWT.Runtime.AttributeDetailsContracts, OWT.Runtime.ObservedState, OWT.Runtime.NativeInput으로 실행한다. 원본 프로젝트의 OWT.Runtime.Attributes는 샘플 BP를 요구하므로 plugin-only 소비 프로젝트의 검사와 구분한다. 위 테스트 이름은 실행 진입점 안내이며 결과 보고가 아니다.

원본 저장소에는 [상세 사용법](../../Docs/RuntimeAttributeEditor_Usage.md)과 [구현/검증 기록](../../Docs/RuntimeAttributeEditor_Requirements.md)이 있다. 위 설명은 구현 계약이며 특정 환경에서의 테스트 통과 주장과는 별개다. 이 README만 복사해도 설치·native 실행·기본 API 사용 정보를 확인할 수 있다.
