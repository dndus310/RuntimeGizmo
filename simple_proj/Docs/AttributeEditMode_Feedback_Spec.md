# AttributeEditMode 피드백 보완 요구사항 명세

작성일: 2026-10-08 · 대상: `simple_proj`, Unreal Engine 5.7 · 버전: v4 계획 · 상태: 요구사항 및 설계, 구현 전

v3 Runtime ITF 구조를 유지하면서 좌표계, 다중 선택, 복제·삭제·Undo/Redo, 실시간 상태 모니터, Blueprint로 구성하는 CustomGizmo, 드래그 중 Gizmo 강조를 보완한다. 이 문서는 이번 피드백의 구현 기준이다. 기존 v3 및 Playground의 빌드·자동화 통과 기록은 아래 신규 기능의 검증 결과가 아니다.

Unreal Editor와의 비교 기준은 **UE 5.7 기본 Level Editor의 Actor 편집**이다. `UModelingToolsEditorMode`에서는 Tool 등록·수명·Context 서비스·transaction 연결 구조를 참조한다. Modeling Tool별 고유한 pivot 동작이나 모든 Editor 기능을 기본 Actor 편집 동작으로 혼합하지 않는다. Editor 전용 코드는 비교 근거로만 사용하며 Runtime에 링크하지 않는다.

## 1. 피드백과 현재 차이

| 요구 ID | 사용자 피드백 | v3에서 확인한 상태 | 이번 보완 |
|---|---|---|---|
| COORD | T/R/S의 Local/World 허용을 Editor와 동일하게 | 요청 좌표계를 Context로 그대로 전달 | 사용자 선택 좌표계와 실제 적용 좌표계를 분리하고 Scale은 Local 고정 |
| SELECT | 다중 선택과 다중 TransformGizmo | 단일 Actor 선택·단일 target | 순서가 있는 선택 집합, 활성 객체, 공통 Gizmo, 계층 중복 적용 방지 |
| DUP | 다중 선택 Duplicate | 단일 source와 authored hierarchy 복제 | 선택 집합을 하나의 계획·참조 매핑·transaction으로 복제 |
| DELETE | 복제 후 삭제 | 편집 명령으로 제공하지 않음 | 원본·복제본 공통 삭제 명령, 선택 정리, PCG 정리, 복원 정보 확보 |
| HISTORY | 객체별 히스토리 및 Undo/Redo | JSON event journal만 존재. Undo/Redo 및 ITF transaction은 빈 구현 | 객체별 변경 조회와 세션 전체 실행 취소 스택을 연결 |
| CURRENT | 실시간 Current TreeView/JsonView와 변경 식별 | 단일 선택 snapshot, operation/PCG 요약, 이벤트 JSON | 동일 typed snapshot의 Tree/JSON 화면, 필드별 변경·baseline·preview 표시 |
| DRAG | 드래그 중 축을 벗어나면 강조 재질 해제 | hover 재질과 interacting 표시가 별도 경로 | 포인터 hover와 capture된 조작 대상을 구분하여 종료까지 강조 |
| CUSTOM | BP에서 셰이프를 수정하고 실제 Gizmo로 사용 | native Actor가 9개 셰이프 생성. Custom 등록은 있으나 편집 Tool은 기본 Gizmo 고정 | BP visual Actor·명시적 handle 역할·설정 주입·hit test/state adapter 연결 |

`Actor 관찰값 → typed 상태 → UI/JSON projection` 방향은 유지한다. JSON 이벤트 수신 내용을 현재 상태나 Undo 복원 정보로 직접 저장하지 않는다.

## 2. 좌표계와 T/R/S

### COORD-01. Viewport Gizmo 좌표계

| 도구 | World | Local | 실제 적용 및 UI |
|---|---:|---:|---|
| Translation / W | 허용 | 허용 | 선택된 좌표계 사용 |
| Rotation / E | 허용 | 허용 | 선택된 좌표계 사용 |
| Scale / R | 비허용 | 허용 | 항상 Local. 좌표계 전환을 비활성화하고 사유 표시 |

UE 5.7의 `FEditorModeTools::GetCoordSystem()`은 Scale에서 Local을 반환하고, raw 좌표계 설정은 보존한다.[E1] 따라서 `PreferredCoordinateSystem`과 `EffectiveCoordinateSystem`을 별도 상태로 둔다. 예를 들어 World Translation → Scale → Rotation 순서에서는 `World → Local → World`가 된다. Scale을 선택했다는 이유로 사용자의 World 선호값을 Local로 덮어쓰지 않는다.

UI, BP/C++ 요청, JSON 요청, ITF Queries, Gizmo 생성·Tick이 동일한 좌표계 정책을 사용한다. Scale 중 World를 강제 요청해도 World Scale을 실행하지 않으며, 현재 제한 사유를 반환한다. 코드 경로별로 예외 규칙을 중복 작성하지 않는다.

조작 시작 시 도구 종류, 좌표계, 축/평면, pivot, 선택 ID와 시작 Transform을 고정한다. 드래그 도중 도구·좌표계 전환 요청은 조작 종료까지 비활성화한다. 외부에서 대상이나 모드가 무효화되면 해당 조작을 취소한다.

### COORD-02. Details의 Relative/World는 별도 의미

Viewport의 Local은 Gizmo 축 방향이고, Details의 Relative/World는 각 Transform 항목의 부모 상속 설정이다. 둘을 하나의 토글로 묶지 않는다. Editor의 Details 메뉴는 단순 표시 변환이 아니라 RootComponent의 `bAbsoluteLocation`, `bAbsoluteRotation`, `bAbsoluteScale`을 변경한다.[E3]

- Location/Rotation/Scale 행마다 실제 상대값과 Absolute 플래그에 맞는 값을 표시한다. World 관찰값은 Current에서 항상 별도로 제공한다.
- Relative/World 변경은 해당 항목의 Absolute 플래그와 필요한 상대값을 함께 변경하고, 변경 직전의 world pose를 보존한다. 부모 socket Transform과 이미 Absolute인 다른 항목도 고려한다.
- 이 변경은 실제 편집이므로 Undo/Redo 대상이다. 표시만 바꾸는 모니터의 World/Relative 보기 선택과 구분한다.
- 다중 선택의 값이나 상속 플래그가 다르면 `Mixed`로 표시한다. 지정한 행·축만 변경하며 다른 항목은 객체별 값을 보존한다.
- 기존 `RequestTransformField`의 World 의미는 호환 API에서 유지한다. 신규 API에는 `ValueSpace`와 Absolute 변경 여부를 명시하여 기존 소비자의 결과를 바꾸지 않는다.

### COORD-03. Scale 및 수치 기준

기본 비교 설정은 UE 5.7의 additive scaling이다. 각 Actor의 상대 scale에 같은 축 delta를 적용하고, 공통 pivot에 대한 위치 보정은 UE Actor 편집 수식을 따른다.[E4] `UTransformProxy`의 공통 비율 곱셈을 그대로 사용하면 서로 다른 초기 scale의 결과가 달라질 수 있으므로 동일성 검증 없이 채택하지 않는다.

회전된 Actor, 서로 다른 초기 scale, 음수·0 scale, 비균일 scale을 가진 부모와 자식을 검증한다. 0으로 나누거나 NaN을 생성하지 않아야 한다. shear를 새로 표현하는 별도 변환 체계는 도입하지 않고 UE의 `FTransform` 표현과 Actor 편집 결과를 기준으로 한다. Runtime component의 이동 가능 여부에 따른 제한은 대상별 사유로 표시한다.

기존 위치·각도·크기 snapping 설정은 유지하며 같은 조작 중 양자화 기준이 바뀌지 않게 한다. 과거 percentage scaling, 사용자 임시 pivot 편집, vertex snapping의 신규 구현은 이번 필수 범위가 아니다.

## 3. 다중 선택과 TransformGizmo

### SELECT-01. 선택 모델

선택은 `OrderedObjectIds`, `ActiveObjectId`, `SelectionRevision`으로 표현한다. UObject 주소나 이름을 선택 항목의 영속 키로 사용하지 않는다. 활성 객체는 마지막으로 선택된 유효한 편집 대상이며, 해제되면 남은 선택 중 가장 최근 대상을 사용한다.

| 입력 | 동작 |
|---|---|
| Actor 좌클릭 | 기존 선택을 대체 |
| Ctrl+좌클릭 | 미선택 Actor 추가, 이미 선택된 Actor 제거 |
| 빈 공간 좌클릭 | 선택 해제 |
| Ctrl+빈 공간 클릭 | 기존 선택 유지 |
| Gizmo 클릭·드래그 | Actor 선택을 변경하지 않고 Gizmo가 입력 소유 |
| UI 조작·텍스트 입력 | 월드 선택·편집 단축키로 전파하지 않음 |

Ctrl 토글은 [기본 Level Editor 선택 동작](https://dev.epicgames.com/documentation/en-us/unreal-engine/selecting-actors-in-unreal-engine?application_version=5.7)을 따른다. 선택된 모든 Actor는 표시하고 활성 객체는 추가 표시로 구분한다. 선택 정책은 service actor, Gizmo actor, PCG generated object 등을 판별하며 구체 BP 클래스명이나 자산 경로 목록에 의존하지 않는다. CAC 관리 자식을 직접 hit한 경우 독립 편집 가능 여부와 소유 Actor로의 해석을 한 정책에서 결정한다.

ITF의 `GetCurrentSelectionState`와 `RequestSelectionChange`에 전체 선택 집합 및 Add/Remove/Replace/Clear를 연결한다. 기존 `GetSelectedObject`는 활성 객체를 반환하는 호환 조회, `SetSelectedObject`는 단일 선택으로 대체하는 호환 호출로 유지한다.

Marquee, Outliner 범위 선택, GroupActor, BSP, component/instance 직접 편집은 별도 확장 범위다. 이번 필수 다중 선택은 viewport의 Actor 선택과 공개 selection API로 완결한다.

### SELECT-02. Gizmo 기준과 적용 대상

기본 pivot은 **정규화된 조작 목록의 마지막 Actor**의 pivot이고, Local 축 방향도 해당 객체 기준이다.[E2] 독립된 Actor끼리는 마지막 선택 객체에 해당한다. 부모·자식 동시 선택처럼 정규화로 자식이 제외되는 경우 `ActiveObjectId`와 `GizmoReferenceObjectId`가 다를 수 있으므로 두 값을 구분한다. 선택 bounding box 중심이나 평균 위치를 기본 pivot으로 쓰지 않는다. `UTransformProxy`의 기본 다중 target 평균 위치/identity rotation 동작은 그대로 사용하지 않고 Runtime 선택 정책으로 보정한다.[E5]

Actor 원점 외에 편집 pivot offset이 필요한 경우 Runtime에 유지되는 typed pivot provider로 공급한다. cooked build에서 사라지는 Editor 전용 pivot 정보가 자동으로 보존된다고 가정하지 않는다. 별도 pivot offset이 없는 Actor는 Actor 원점을 사용한다.

- Translation: 모든 조작 대상에 같은 world delta를 적용한다.
- Rotation: 공통 pivot 주위에서 위치와 방향을 함께 회전한다. 각자의 원점에서만 회전하는 모드는 기본값이 아니다.
- Scale: 공통 pivot을 사용하되 각 Actor의 scale과 위치 보정은 COORD-03의 Editor 동작을 따른다.
- 부모와 자식이 함께 선택되면 선택 표시에는 둘 다 남는다. Gizmo의 실제 transform 적용 집합은 선택된 상위 attachment가 이미 움직이는 대상을 정규화하여 같은 delta가 두 번 적용되지 않게 한다. Absolute 상속 예외의 결과는 Editor 비교 fixture로 검증한다.
- 다중 Gizmo의 모양과 기준은 조작 중 안정적으로 유지한다. Actor 변경 callback이나 PCG 생성 결과로 target 배열을 중간에 교체하지 않는다.

Details의 숫자 입력은 Gizmo delta와 구분한다. `Mixed`인 X에 숫자를 입력하면 선택 객체 모두의 X를 그 값으로 설정하고 각각의 Y/Z는 유지한다. 선택 부모·자식 모두에 값 설정이 요청된 경우 시작 snapshot에서 최종 목표를 계산한 후 attachment 순서에 맞춰 적용한다. 부모 이동 후 자식에 누적 delta를 다시 더하지 않는다.

Details의 한 번의 숫자 확정 또는 slider press→release, Gizmo press→release는 객체 수와 무관하게 transaction 하나다. 시작 시 대상과 before 값을 고정하고, 실시간 preview는 Actor에서 다시 관찰한다. 취소 또는 실질적인 값 변화가 없는 조작은 Undo 항목을 만들지 않는다.

## 4. 다중 Duplicate

### DUP-01. 선택 복제와 계층 복제의 구분

새 `DuplicateSelection`은 선택 Actor와 그 객체가 소유한 CAC 계층을 복제한다. 선택하지 않은 일반 attachment 자식을 자동 포함하지 않는다. 기존 v3의 `AuthoredHierarchy` 요청은 명시적인 계층 복제 정책으로 유지한다. 두 범위를 UI·API·operation snapshot에서 구분한다.

선택 A가 선택 B의 일반 attachment 부모라면 A와 B를 각각 한 번 복제하고 A′–B′ 관계를 복원한다. 부모를 발견했다는 이유로 선택 B를 복제 대상에서 제거하지 않는다. 반면 소유 CAC 계층에 이미 포함된 객체는 중복 생성하지 않는다. 선택 집합과 계층 방문 집합, 생성할 Actor 집합은 서로 구분한다.

모든 선택 루트에 **하나의 전체 ObjectMapping**을 사용한다. A가 B를 참조하면 A′는 B′를 참조해야 한다. 루트마다 기존 단일 복제 API를 반복 호출해서 외부 참조처럼 남기는 구현은 허용하지 않는다. 배치 밖의 자산·Actor 참조는 기존 외부 참조 정책을 따른다.

선택 안의 attachment 부모는 복제 부모로 연결한다. 새 선택 복제의 외부 부모 정책은 `PreserveOriginalParentSameWorld`로 정의하며, 부모·socket이 commit 전에 사라지면 실패시킨다. 현재 v3는 map 밖의 부모를 분리하므로 신규 명령에서 별도 보완이 필요하다. 기존 단일 API의 분리 동작은 호환 정책으로 유지한다.[E8]

같은 World의 여러 Level에 걸친 선택은 원래 Level별 생성 계획과 하나의 전체 매핑·commit을 사용한다. 다른 World의 객체는 한 배치에 포함하지 않는다. 대상 Level이 로드 해제되거나 편집 불가능하면 전체 배치를 거부한다. 배치 전체에 공통 world offset을 한 번 적용한다. Ctrl+D의 기본 offset은 0이며, 이동은 별도 Transform 또는 Alt+Translation drag로 수행한다. 기존 offset 인자를 사용하는 호출의 동작은 보존한다.

일반 reflected 외부 참조와 특수 Actor 관계를 구분한다. 호환 기본값에서 Owner는 내부 대상이면 remap하고 외부면 clear, Instigator는 clear다. 이 예외를 operation 정책에 명시하며 별도 provider가 보존을 지원할 수 있다. **삭제 Undo의 Restore**는 Clone 정책과 달리 기록된 Owner/Instigator 관계를 ID resolver로 복원한다. PCG managed output은 일반 외부 참조처럼 유지하지 않고 adapter의 논리 대응이 없으면 사전에 거부한다.

### DUP-02. 이름, 선택, 실행 경계

- 이름은 기존 family 규칙을 유지한다: `Chair → Chair_1 → Chair_2`, `Chair_4 → Chair_5`. 배치 내 예약 이름과 같은 Level의 충돌을 함께 확인한다.
- 숫자 규칙은 직접 생성하는 Actor의 Runtime object name에 적용한다. CAC 소유 자식의 엔진 내부 이름은 소유 component 계약을 따른다. Editor의 ActorLabel 자동 이름과 완전히 같은 기능이라고 표현하지 않는다.
- 삭제·Undo가 family 증가 이력을 되돌리지 않는다. 실패로 번호가 건너뛰는 것은 허용하되 같은 살아 있는 이름을 중복 발급하지 않는다.
- 정상 복제는 새 논리 ID와 `SourceObjectId`를 가진다. Redo는 같은 논리 복제본을 복원하며 새 Duplicate로 취급하지 않는다.
- 성공 후 선택은 원래 선택 목록에 대응하는 복제본으로 바뀌고 활성 객체도 매핑된다. 실패·취소는 기존 선택을 유지한다.
- batch preflight → capture → 생성·복원 → 참조/attachment 연결 → authored commit → PCG 활성화 순서를 따른다. commit 전 일부만 성공한 상태를 완료로 알리지 않는다.
- 배치 전체가 transaction 하나다. Ctrl+D와 UI 버튼은 같은 명령을 호출한다. Alt+Translation drag는 실제 이동이 시작될 때 한 번만 복제하고 이동까지 하나의 transaction으로 기록한다. 클릭만 하고 놓으면 복제하지 않는다.
- Alt drag 취소는 생성한 배치를 정리하고 원래 선택을 복원한다. 일반 Duplicate 완료 뒤 별도 이동은 별도 transaction이다.

Duplicate 결과는 선택 source→destination 대응과 **실제 생성한 전체 객체 집합**을 기록한다. Undo는 이 생성 집합을 제거하며, 나중에 외부에서 붙인 일반 자식을 재귀 탐색으로 추가 삭제하지 않는다. Alt+Translation의 사용자 동작은 [Epic Transforming Actors](https://dev.epicgames.com/documentation/en-us/unreal-engine/transforming-actors-in-unreal-engine?application_version=5.7)를 참고하고, 하나의 Runtime transaction으로 묶는 것은 이번 설계 계약이다.

PCG graph·parameter·seed·policy는 기존 adapter로 독립 복원한다. generated Actor/ISM, task, cache, managed resource는 복제 또는 history payload에 공유하지 않는다. authored commit과 PCG Ready는 별도 상태이며 PCG 실패가 이미 commit된 복제를 자동으로 취소하지 않는다.

## 5. 삭제와 객체 수명

### DELETE-01. 삭제 범위

Delete 키와 UI 버튼은 같은 `DeleteSelection` 명령을 실행한다. 복제본뿐 아니라 편집 정책상 삭제 가능한 Actor에 공통 적용한다. 일반 입력 필드에서 Delete를 누를 때는 텍스트 편집을 우선한다.

기본 범위는 선택 Actor 및 소유 CAC 객체다. 일반 미선택 attachment 후손은 삭제하지 않고 world Transform을 유지한 채 분리한다. 엔진의 Actor 파괴 경로도 일반 attached 자식을 KeepWorld로 분리한다.[E6] 별도의 계층 삭제를 제공한다면 `DeleteHierarchy`로 범위를 명시하며 기본 Delete에 숨겨 넣지 않는다.

삭제 transaction은 삭제 대상뿐 아니라 **살아남는 자식의 attachment 변경**도 저장한다. Undo는 원래 부모·component·socket·상대/Absolute 설정과 Transform을 복원한다. 삭제된 대상만 저장해서 자식을 분리된 상태로 남기는 것은 실패다.

### DELETE-02. 복원 가능성 및 실행

1. 대상 전체의 편집 권한, world/level, 수명, 복원 adapter, 참조 변경 범위, history 메모리 예산을 사전 검증한다. 실패 대상과 이유를 반환하며 기본 정책은 배치 전체 거부다.
2. 반영 가능한 authored 상태, 인스턴스 속성·소유 subobject·component·관계·이름·논리 ID와 선택 before/after를 복원 payload에 저장한다. JSON event journal이나 weak Actor 참조만으로 복원을 구현하지 않는다.
3. 해당 객체에 대한 신규 명령을 막고 활성 조작을 종료한다. PCG 작업을 취소하고 managed resource 정리를 완료한 뒤 authored 삭제를 확정한다.
4. 선택·Gizmo·관찰 참조를 갱신하고 실제 삭제 결과를 확인한 후 transaction을 commit한다. current 상태에는 삭제된 객체의 tombstone을 남긴다.
5. 실패 시 지원되는 객체 상태와 관계를 보상 복원한다. 복원까지 실패한 예외는 `RecoveryFailed`로 명시하고 성공 이벤트나 history cursor 이동을 내보내지 않는다.

PCG의 기존 cleanup-only 작업은 `CancelGeneration()`으로 중단되지 않을 수 있다. `WaitingForCleanup`으로 접근 가능 상태를 기다린 뒤 original/local component의 자원을 정리한다. 임의의 즉시 cleanup 또는 unregister는 실행 중 자원에 대한 assert를 유발할 수 있으므로 `CancelGeneration → 즉시 Destroy`를 공통 절차로 사용하지 않는다. timeout은 Busy/실패 사유로 반환하며 실제 정리 상태를 숨기지 않는다. EndPlay만으로 전체 PCG 자원이 정리된다고 가정하지 않는다.[E9]

`DestroyActor`의 반환값만으로 완료를 판단하지 않는다. 삭제 거부, 지연된 파괴, 사용자 Destroyed callback의 재진입을 확인해야 한다. 지원 범위에서는 논리적 배치 원자성을 목표로 하되 임의 callback의 외부 부수 효과까지 엔진이 rollback해 주지는 않는다. `RecoveryFailed`에서는 객체별 실제 생존·복원 결과를 다시 관찰하여 보여주고 충돌하는 후속 편집을 차단한다. 과거 상태가 완전히 복원된 것처럼 표시하지 않는다.

Undo 복원은 새 UObject 인스턴스일 수 있다. 논리 `ObjectId`는 같고 `InstanceGeneration`은 증가해야 한다. Destroy된 Actor를 다시 사용하거나 일반 Duplicate 이름 발급을 통해 `Chair_2`를 `Chair_3`으로 복원하지 않는다. 원래 이름은 history가 예약하며 외부 Actor가 그 이름을 선점한 경우 `NameConflict`로 실패시킨다. 다른 객체를 자동 삭제하거나 덮어쓰지 않는다.

Destroy 직후 GC 전의 이전 인스턴스가 내부 이름을 점유하는 경우도 복원 경로에서 처리해야 한다. 사용자가 GC를 기다리거나 수동 실행해야 Undo/Redo가 되는 구현은 불가하다. 사용자의 살아 있는 객체 이름을 바꾸는 방식으로 충돌을 피하지 않는다.

편집 세션이 관리하는 객체의 반영 가능한 inbound 참조와 살아남는 자식 관계는 변경·복원 계획에 포함한다. 삭제 후 외부 raw pointer·native cache·delegate까지 새 인스턴스로 자동 연결된다고 보장하지 않는다. 등록된 reference provider/adapter가 담당 범위를 선언하며 복원 불가능한 필수 관계는 삭제 전에 거부한다.

삭제된 객체의 tombstone은 ID, 마지막 이름·클래스·상태, 삭제 사유·시각·transaction을 표시하는 진단 정보다. 살아 있는 Actor를 강하게 보유하지 않으며 tombstone만으로 복원 가능하다는 뜻이 아니다.

## 6. Undo/Redo와 객체별 히스토리

### HISTORY-01. 한 세션의 실행 순서

세션에는 순서와 cursor가 있는 Undo/Redo stack 하나를 둔다. 객체별 히스토리는 이 stack 및 관찰 변경 레코드의 조회 화면이다. A만 필터해서 보고 있어도 A 변경 후 B 변경을 했다면 Undo는 B부터 실행한다. 임의 과거 객체 항목 하나만 건너뛰어 Undo하는 기능은 제공하지 않는다.

| 작업 | 기록 단위 | Undo / Redo |
|---|---|---|
| Gizmo 조작 | 한 capture의 전체 선택 | 각 객체의 실제 before / after Transform 복원 |
| Details 값·slider | 한 확정 또는 한 slider 조작 | 지정 필드와 실제로 함께 바뀐 항목 복원 |
| Details Relative/World | 전체 대상의 한 설정 변경 | Absolute 플래그·상대값·관계상 필요한 값 복원 |
| Duplicate | 전체 batch, Alt drag면 이동 포함 | 생성 계층 제거 / 같은 논리 계층 복원 |
| Delete | 선택 배치와 살아남는 객체의 관계 변경 | authored 상태·관계 복원 / 다시 삭제 |
| 선택만 변경·탭·필터·hover | 진단/selection 기록 | 사용자 Undo stack에는 넣지 않음 |
| 외부 시스템이 Actor 변경 | 관찰 변경 기록 | 기본은 Undo 불가. 협력 명령으로 등록된 경우에만 지원 |
| PCG 진행률·Ready·cleanup | operation/절차 상태 기록 | 독립적인 Undo 항목을 만들지 않음 |

각 command에는 `TransactionId`, 명령 종류·설명, affected IDs, before/after 상태, 선택 before/after, 실행 origin, 복원 capability, payload 소유권을 저장한다. 트랜잭션의 component·subobject 참조도 stable ID와 instance 세대를 통해 해석한다.

기존 복제기의 임시 capture 구조는 살아 있는 source Actor와 property 메모리를 참조하므로 그 구조를 그대로 stack에 장기 보관하지 않는다. session memento에는 복원 가능한 값·class/level·소유 관계·ID 기반 참조·adapter 설정을 저장한다. command 내부의 적용 순서는 Redo 정순, Undo 역순이며 일부 target이 만료됐다고 조용히 skip하지 않는다.

Undo 후 새로운 사용자 편집이 commit되면 redo branch를 폐기한다. Undo/Redo 결과는 관찰 이력과 이벤트에 기록하지만 사용자 command를 새로 push하지 않는다. 실패한 가장 최근 항목을 자동으로 건너뛰거나 cursor만 이동하지 않는다.

### HISTORY-02. ITF와 비동기 실행

ITF `BeginUndoTransaction`, `AppendChange`, `EndUndoTransaction`을 같은 Runtime history service에 연결한다. 중첩 transaction은 바깥 사용자 조작 하나로 묶고 Cancel/실패/no-op을 처리한다. Gizmo가 이미 ITF change를 제출하는 경우 facade가 같은 조작을 중복 기록하지 않게 한다.

`FToolCommandChange`의 동기 `Apply/Revert`와 PCG cleanup이 필요한 scene command의 비동기 완료를 구분한다. 비동기 history executor는 `Pending → Applying/Reverting → Committed/Failed` 상태로 실행하고 실제 authored 완료 전에 cursor를 이동하지 않는다. 동기 ITF callback 안에서 작업을 시작한 뒤 즉시 성공으로 간주하는 구현은 허용하지 않는다.

변환 history가 일시적인 Gizmo/TransformProxy 포인터만 보유하면 도구를 재생성했을 때 복원되지 않는다. 기본 Actor 명령은 stable ID로 현재 인스턴스를 찾아 적용한다. 외부 Tool의 change는 유효한 target 수명 또는 자체 복원 계약을 제공해야 한다. 조건을 충족하는 도구에만 history capability를 제공한다.

`ToolManager::EmitObjectChange`는 Tool이 종료되면 만료되는 wrapper를 사용할 수 있다.[E10] 즉시 종료되는 Duplicate/Delete Tool의 영속 command는 session history 또는 Context transactions API에 직접 기록한다. Tool 내부 preview용 change와 영속 scene change의 수명을 구분한다.

실행 규칙은 다음과 같다.

- Undo/Redo 명령은 한 번에 하나만 실행한다. pending 상태에서는 충돌하는 편집과 다음 replay를 비활성화하고 이유를 보여준다.
- 미확정 드래그/slider 중 Undo나 Escape는 현재 조작부터 취소하며 이전 transaction까지 한 번에 되돌리지 않는다. focus 상실·모드 종료로 취소될 때도 같은 정리 경로를 사용한다.
- before/after의 예상 값과 실제 Actor 상태가 충돌하면 `Conflict`를 반환하고 cursor를 유지한다. 현재 선택과 관계없이 원래 명령 대상에 적용한다.
- PCG 작업 중 Duplicate Undo 또는 Delete를 실행하면 먼저 취소·cleanup을 완료한다. 이전 attempt의 늦은 callback은 generation/operation token으로 무시한다.
- Delete Undo 및 Duplicate Redo는 authored 상태를 먼저 복원하고 PCG를 기록된 생성 정책으로 새로 활성화한다. PCG task/cache/이전 출력 포인터를 되살리지 않는다.
- 복원 replay의 완료는 authored commit이다. 이후 generation source를 기다리는 PCG 때문에 Undo/Redo cursor를 계속 Busy로 두지 않는다. 삭제 replay는 삭제 전 cleanup barrier가 완료되어야 commit한다.
- F2 OFF/ON과 Mode/Tool 교체로 history를 지우지 않는다. World/편집 세션 종료는 history 수명의 경계이며, 프로젝트 재실행·저장/로드 사이의 history 영속화는 범위 밖이다.

단축키는 `Ctrl+Z`, `Ctrl+Y`, 기존 `Ctrl+Shift+Z`를 지원한다. 버튼과 단축키는 `CanUndo/CanRedo`, 다음 명령명, 대상 수, 비활성 사유를 공유한다.

### HISTORY-03. 보관 한도

초기 기본값은 최대 256개 transaction과 복원 payload 64 MiB이며 설정 가능하게 한다. 관찰 이력과 event journal 한도는 별개다. 보관 중인 payload가 참조하는 class/asset/subobject의 GC 수명을 명시적으로 관리하되 삭제된 원본 Actor를 복원본 대신 붙잡아 두지 않는다.

완결된 오래된 항목을 제거할 때 필요한 identity·이름 예약·adapter lease를 참조 수에 맞게 해제한다. 실행 중인 transaction은 제거하지 않는다. 한 명령이 예산을 넘으면 **변경 전에** 거부하고 사유를 표시한다. 기록 없이 삭제를 실행하는 방식으로 조용히 우회하지 않는다. 한도 초과로 사라진 과거 Undo 경계는 화면에 표시한다.

## 7. Current·History·Events 모니터

### CURRENT-01. 화면 구획

기존 Details를 유지하고 Monitor 내부에 `Current`, `History`, `Events`를 둔다. 상단에는 편집 활성, Mode/Tool, 선택 수·활성 객체, pending command, Undo/Redo, 현재 snapshot revision을 공통 표시한다. F3로 편집 모드가 꺼져 있어도 조회할 수 있다.

| 화면 | 기준 데이터 | 주요 기능 |
|---|---|---|
| Current | 실제 Actor 관찰값 및 typed Mode/selection/operation 상태 | TreeView, JsonView, Changed only, Pin/Watch, baseline 비교 |
| History | typed 변경 레코드 및 실행 가능한 세션 stack | 객체 필터, before/after, transaction 연결, Undo/Redo cursor·불가 사유 |
| Events | 소유 Pub-Sub의 입출력 journal | topic/방향/ID 필터, payload, pause, clear view, 잘림 표시 |

Current의 TreeView와 JsonView는 같은 불변 snapshot에서 생성하며 동일 revision을 표시한다. JsonView는 현재 상태의 읽기 전용 직렬화 결과다. Events의 JSON payload와 구분하며 JSON을 편집해서 StateStore에 직접 대입하는 경로는 만들지 않는다.

기본 트리의 의미 구조는 다음과 같다. 각 항목은 provider의 typed descriptor로 구성하고 UI에 특정 BP 필드를 하드코딩하지 않는다.

```text
Session
  Mode / ActiveTool / Capabilities
  Selection: OrderedIds / ActiveId / Revision
  Gizmo: Mode / PreferredSpace / EffectiveSpace / ReferenceObjectId / Pivot / Hover / Capture
  Objects
    ObjectId: Name / Class / InstanceGeneration / Lifecycle
      Transform: World / Relative / AbsoluteFlags
      Attachment: ParentId / ComponentId / Socket
      Components: ComponentId / Type / SupportedProperties
      Change: ObjectRevision / Baseline / Preview
  Operations: Duplication / Deletion / HistoryReplay
  Procedural: ComponentId / OperationId / Attempt / State / Reason
  History: Cursor / CanUndo / CanRedo / Pending / Retention
```

필수 Actor 관찰 항목은 이름·클래스·ID·수명, world/relative TRS, Absolute 플래그, attachment, 선택 여부와 편집 capability다. component/PCG의 알려진 상태를 포함한다. 추가 BP/native 속성은 read-only 관찰 provider로 확장한다. 임의 UObject graph를 무한 반사 순회하거나 모든 BP 실행 상태를 자동 지원한다고 표시하지 않는다. 객체 참조는 ID 링크로 표시하고 배열은 stable key 또는 변경 범위와 함께 표시한다.

### CURRENT-02. 실시간 관찰과 변경 표시

기본 관찰 대상은 현재 선택, 사용자가 Pin/Watch한 객체, 진행 중인 편집·복제·삭제·replay의 대상이다. history의 복원 대상은 replay 전 반드시 재관찰한다. 선택 해제된 객체를 계속 감시하지 않는 경우 `NotWatching`, 마지막 관찰 시각, 재선택 시 관찰 공백을 명시한다. 전체 World를 매 프레임 반사 스캔하지 않는다.

명령 적용·Undo/Redo·외부 변화 후 Actor에서 실제 값을 다시 읽고 snapshot을 갱신한다. 요청한 Desired 값이 그대로 적용됐다고 추정하지 않는다. game thread의 관찰과 화면 표시를 분리하고 UI 갱신은 변경된 행 단위로 합칠 수 있다.

| 표시 | 의미 | 해제 규칙 |
|---|---|---|
| Changed | 직전 관찰 이후 값 변경 | 기본 1초 강조 후 해제, 최근 변경 시각 유지 |
| Modified from baseline | 명시적 비교 기준과 다른 값 | 값이 기준으로 돌아오거나 baseline 재설정 시 해제 |
| Preview | 미확정 조작 중인 값 | commit/cancel 시 해제 |
| Added / Deleted / Restored | 객체 또는 속성의 수명 변화 | lifecycle·history에서 조회 가능 |
| Mixed / Unavailable / Unsupported | 다중 값 차이 / 해당 항목 없음 / 제공자 미지원 | 실제 typed 상태 변화에 따라 갱신 |

색상만으로 구분하지 않고 배지·기호·텍스트, property path, before/after를 함께 제공한다. `Changed only`는 baseline 차이와 최근 변경 중 어느 기준인지 표시한다. 비교 기준 재설정은 저장·Undo 제거 기능이 아니다.

기본 비교 오차는 위치 0.001 cm, 회전 등가각 0.001°, scale 0.000001로 두고 설정 가능하게 한다. quaternion 부호 및 Euler 360° 표기 차이를 실제 방향 변화로 오인하지 않는다. 누적 변화는 마지막 확정 관찰값과 비교하여 프레임별 작은 변화가 영구히 누락되지 않게 한다. 현재 실제 수치는 표시하고 변경 판정 오차와 구분한다.

Tree/JSON 간 전환, 값 갱신, 필터 변경 시 노드 펼침·스크롤·선택을 ID+property path로 유지한다. 모니터 노드 선택은 월드 선택을 자동 변경하지 않으며 `Select in world`를 별도 동작으로 제공한다. 긴 목록은 가상화하고 JSON이 생략되면 범위와 사유를 명시한다. 잘린 JSON을 완전한 snapshot이나 복원 payload로 제공하지 않는다.

### CURRENT-03. 표시 제어 및 시간 기준

- `Pause events`는 이벤트 목록 표시만 멈춘다. Current, Actor 적용, history 기록, PCG는 계속 진행한다.
- `Clear view`는 해당 화면의 표시 기준만 변경한다. Undo stack·Actor·기록 service를 삭제하지 않는다.
- 필터는 표시만 바꾸고 명령 대상·이벤트 전달·기록 여부를 바꾸지 않는다.
- Current를 고정하는 기능을 제공할 경우 `Frozen`, 캡처 시각/revision을 표시하며 그 고정 화면에서 편집을 시작하지 않는다.
- journal 보관 한도를 넘긴 누락, payload 잘림, history eviction, 관찰하지 않은 기간을 각각 구분한다.

검증 기준은 실제 RHI 60 FPS 환경에서 조작 중인 선택 객체의 변경이 Current에 100 ms 이내 나타나는 것이다. 선택 객체 100개·관찰 property 행 1,000개를 기준 fixture로 측정한다. Tree/JSON 전체 재생성을 매 Tick 반복하지 않고 관찰·projection·paint 시간을 별도로 기록한다. 이 수치는 구현 후 측정할 목표이며 현재 성능 결과가 아니다.

## 8. 식별자·상태·JSON 계약

`SessionId`, `ObjectId`, `ComponentId`, `InstanceGeneration`, `SelectionRevision`, `ObjectRevision`, `SnapshotRevision`, `HistoryRevision`, `OperationId`, `TransactionId`를 구분한다. 이벤트 Sequence는 전달 순서이며 객체 revision이 아니다. 조회·구독·JSON 직렬화만으로 ObjectRevision을 증가시키지 않는다. 64비트 revision/sequence는 JSON에서 십진 문자열로 표현하여 외부 JS 소비자의 정밀도 손실을 피한다.

객체 이름 변경, 선택 변경, F2 전환으로 ID가 바뀌지 않는다. 일반 생성·Duplicate는 새 ID, Undo/Redo 복원은 같은 ID와 새 instance generation이다. 삭제 후 같은 이름으로 외부에서 만든 Actor는 별도 객체다. 생존 중인 component/subobject도 이름만으로 재바인딩하지 않는다.

typed 변경 레코드에는 다음을 포함한다.

```text
ChangeId, TransactionId?, OperationId?, SessionId
AffectedObjectIds, ComponentId?, PropertyPath
Kind: Created / Modified / Deleted / Restored
Origin: Details / Gizmo / Tool / External / Undo / Redo
BeforeRevision, AfterRevision, BeforeValue, AfterValue, ValueType
ObservedAtUtc, CommandLabel, Outcome, UndoEligibility, UnavailableReason
```

외부 변경의 before는 마지막 관찰값이다. 관찰하지 않은 중간 상태를 생성하지 않으며 before를 알 수 없으면 `UnknownBefore`로 표시한다. continuous preview는 Current로 갱신하고 사용자 transaction은 commit 때 한 건으로 확정한다. 외부 변화의 이력은 관찰 구간을 표시하여 이벤트 발생 횟수와 동일한 것으로 오해하지 않게 한다.

신규 JSON schema 3에서는 **실제 영향 대상**과 **당시 선택 context**를 분리한다.

```json
{
  "schemaVersion": 3,
  "category": "Observation",
  "sessionId": "<session-id>",
  "snapshotRevision": "125",
  "targets": [{ "objectId": "<actual-owner-id>", "componentId": "<component-id>" }],
  "context": { "selectionIds": ["<selected-id>"], "activeObjectId": "<selected-id>" },
  "correlation": { "operationId": "<operation-id>", "transactionId": null, "changeId": "<change-id>" },
  "changedPaths": ["procedural.state"]
}
```

이 예시는 envelope 구조이며 실제 property 변경 데이터는 위 typed 레코드에서 직렬화한다. `Command`, `Observation`, `Selection`, `Lifecycle`, `Procedural`, `Diagnostic` category를 구분한다. PCG Ready를 Actor TRS 변경이나 Undo 가능한 사용자 명령으로 기록하지 않는다.

현재 v3의 일부 이벤트는 선택 snapshot을 공통 payload로 사용하므로 최상위 objectId가 이벤트 대상과 다를 수 있다.[P4] 객체별 이력은 JSON objectId를 역해석하지 않고 typed `AffectedObjectIds`로 인덱싱한다. schema 1/2의 기존 요청은 호환 adapter로 계속 수신하며 단일 선택/World 의미를 보존한다. 새 응답·이벤트 schema는 구독 시 버전을 선택할 수 있게 하여 기존 payload를 조용히 바꾸지 않는다.

명령 요청은 대상 ID 집합, 선택 revision, 필요한 객체별 예상 revision과 requestId를 검증한다. 처리 중 선택이 바뀌었다고 새 선택에 오래된 명령을 적용하지 않는다. 같은 접수 requestId의 재전달은 같은 operation 결과를 반환하고 작업을 중복 실행하지 않는다. 중복 요청 보관 기간은 세션의 bounded request ledger 정책으로 명시한다.

## 9. Gizmo 구성 및 드래그 중 축 강조

### DRAG-01. 코드에서 확인한 원인과 남은 재현

ITF InputRouter는 capture 시작 시 hover를 종료할 수 있다. Axis Gizmo는 interacting 상태를 별도로 전달하지만, 현재 mesh component 계열의 hover 재질은 hover boolean으로 결정된다.[E7] 따라서 hover 종료와 조작 종료를 같은 시각 상태로 취급하면 드래그가 계속되어도 강조가 해제될 수 있다.

이는 코드 경로에서 확인한 구조적 원인이다. 사용자가 본 정확한 “축을 벗어난 프레임”의 입력·material 변화는 아직 계측 재현하지 않았다. 구현 첫 단계에서 `HoveredPart`, `CapturedPart`, capture 시작/종료, material 변경을 함께 기록하여 동일 증상인지 확인한다. hover callback 하나를 교체하는 것만으로 해결됐다고 판정하지 않는다.

### DRAG-02. 표시 우선순위와 종료

표시 우선순위는 `Active interaction > Hover > Idle`이다. 활성 조작은 hit test 결과가 아니라 InputRouter의 capture 수명에 연결한다. 조작 중 포인터가 축을 벗어나거나 다른 축 위를 지나도 원래 축/평면이 강조되며 새로운 축으로 조작 대상이 바뀌지 않는다.

`HoveredPartId`, `CapturedPartId`, `bInteracting`, capture 종료 사유를 별도 typed 상태로 제공한다. 축·평면·회전 ring·uniform scale handle 모두 같은 규칙을 사용하고, interacting용 대체 mesh·가시성 전환은 유지한다. material을 매 Tick 새로 만들거나 공유 원본 material을 변경하지 않는다.

정상 release는 값을 commit한 후 capture와 active 강조를 해제하고 현재 포인터 위치의 hover를 재계산한다. Escape·focus 상실·편집 종료·대상 파괴·Tool 교체는 지정된 취소/종료 경로로 입력, preview, history, 강조를 함께 정리한다. UI 위로 포인터가 이동한 것만으로 이미 시작한 Gizmo capture를 끊지 않으며 앱 focus 상실과 구분한다. 다음 조작에 이전 강조·축 제한이 남지 않아야 한다.

modern Gizmo 초기화가 component interface에 직접 상태를 전달하는 경로와 회전용 substitute full-circle mesh를 함께 수정 대상으로 검토한다. `SetUpdateHoverFunction`만 교체하거나 원래 ring에만 material을 지정해서는 대체 mesh에서 강조가 누락될 수 있다.[E7]

### CUSTOM-01. ITF 책임 분리와 현재 코드 정리

정돈 전 `VTBOWTTransformGizmoBehavior.h`에는 입력 Behavior 1개, 축 위치·회전 Gizmo 2개, 각 Builder 2개가 함께 선언되어 있었다.[P5] Behavior가 Builder를 소유하는 구조는 아니다. Builder가 Gizmo를 생성하고, Gizmo가 입력 Behavior를 등록하여 InputRouter의 capture를 받는 관계다. Builder와 Gizmo가 존재하는 것은 정상적인 ITF 구성이다.

2026-10-08 가독성 정리에서 AxisPosition/AxisAngle의 header·구현을 각각 분리했다. UClass 이름과 입력 동작은 유지하고, 기존 Behavior header에는 이전 include 소비자를 위한 forwarding include를 남겼다. 이 파일 정리는 아래 Blueprint visual/provider 기능의 구현 완료를 의미하지 않는다.

현재 `UVTBOWTTransformGizmoBehavior`는 왼쪽 마우스와 기본 Gizmo 입력 우선순위만 설정한다. 이 값은 UE 5.7의 기본 Axis Gizmo 및 AnyButtonInputBehavior에서도 이미 설정하므로 현재 고유 입력 기능이 없다.[E11] 의미 있는 추가 동작은 `UVTBOWTAxisAngleGizmo`의 회전 드래그 중 다른 회전 handle 숨김·복원이다. 이것은 입력 인식이 아닌 Gizmo 표시 정책이다.

보완 구현에서는 다음 책임을 적용한다. 불필요한 Behavior 상속·기본 MouseBehavior 교체는 제거하거나 실제 고유 입력 정책이 있을 때만 유지한다. 기존 공개 UClass를 제거·이름 변경하는 경우 BP/직렬화 참조를 조사하고 필요한 호환 처리를 한다. 파일을 분리하는 것만으로 동작이 개선됐다고 판정하지 않는다.

| 구성 | 책임 |
|---|---|
| AttributeEditMode / Context | Tool·Gizmo provider 등록, 설정·서비스 수명 |
| AttributeEditTool | 선택 target binding, Gizmo 생성 요청·해제, 공통 편집 transaction 연결 |
| GizmoBuilder | 설정을 읽어 Gizmo와 ActorFactory 구성 |
| CombinedTransformGizmo / 축·평면 sub-Gizmo | ITF 변환 제약·capture·TransformProxy 연결 |
| InputBehavior | 입력 인식, capture 시작·갱신·종료 |
| Blueprint visual Actor / handle component | 셰이프·재질·표시 상태 및 hit geometry |

축 Gizmo 및 Builder는 `VTBOWTAxisAngleGizmo.h/.cpp`처럼 책임이 드러나는 단위로 이동한다. 짧고 전용인 Gizmo와 해당 Builder는 같은 파일에 있어도 된다. Behavior 파일에 여러 Gizmo 구현을 모으는 관행은 유지하지 않는다. 회전 focus 정책은 `PartId`와 공통 표시 상태를 통해 적용하며 임의 HitTarget·Actor를 강제 cast해야만 동작하는 확장 계약을 만들지 않는다.

`UModelingToolsEditorMode`의 Tool 등록, Context 서비스, GizmoManager 수명을 Runtime에서 재현한다.[E12] 해당 Editor 클래스를 직접 상속하거나 `UnrealEd`·`ModelingToolsEditorMode`를 Runtime dependency로 추가하지 않는다. 현재 Runtime Mode 구조와 GizmoManager 기반 수명은 유지한다.

엔진 Mode가 transform Gizmo Context를 등록하고 Context가 builder들을 등록하는 방식에 맞춰, 프로젝트도 `UOWTTransformGizmoContext` 같은 전용 Context에 Gizmo 등록·factory·BP 설정을 모은다. Mode는 이 Context를 설치·해제하고 Tool은 조회하여 생성 요청을 한다. Context와 Mode 양쪽에서 동일 builder를 중복 등록하지 않는다.

### CUSTOM-02. Blueprint 작성 계약

최종 사용자 흐름은 **Gizmo용 Blueprint 생성 → component의 mesh·재질·상대 배치 편집 → 각 handle의 역할 지정 → 편집 Mode 설정에 BP class 지정 → packaged Runtime에서 선택한 Actor를 조작**하는 것이다. 셰이프 변경에 C++ 재컴파일이 필요하지 않아야 한다. BP 클래스가 변경되면 콘텐츠 재저장·cook/package는 필요하다.

현재 custom factory는 `AVTBOWTTransformGizmoActor` native class를 고정 생성한 뒤 Arrow/Circle/Box를 코드에서 추가한다. 엔진 부모는 `NotBlueprintable/NotBlueprintType`이며 현재 파생 클래스는 이를 재정의하지 않는다. 또한 실제 Tool의 `CreateGizmo` ID는 `OWT.Transform`으로 고정되어 있다.[P6] 따라서 현재 Custom 등록만으로 이 사용자 흐름이 제공되지는 않는다.

구현 API 이름은 제안이며 다음 계약을 만족하는 기존 클래스를 확장해도 된다.

- `Blueprintable`, `BlueprintType`인 native visual Actor 기반을 제공한다. `ACombinedTransformGizmoActor` 기반으로 생성·수명을 유지하고 BP의 ConstructionScript가 끝난 뒤 component들을 수집한다. 레벨에 수동 배치해야만 작동하는 구조로 만들지 않는다.
- BP에서 추가 가능한 native mesh handle component를 제공한다. 기본 구현은 `UViewAdjustedStaticMeshGizmoComponent` 및 `IGizmoBaseComponentInterface`의 runtime hit test·상태 전달 기능을 활용한다. native adapter에서 ViewContext, 카메라 거리별 화면 크기, 가시성, hit geometry를 초기화한다. 일반 StaticMeshComponent를 배치하기만 하면 모든 조작 기능이 생긴다고 가정하지 않는다.
- handle마다 enum `PartId`를 명시한다. 최소 지원은 `TranslateX/Y/Z`, `TranslateXY/XZ/YZ`, `RotateX/Y/Z`, `ScaleX/Y/Z`, `ScaleXY/XZ/YZ`, `UniformScale`이다. component 이름·배열 순서·mesh asset 경로로 의미를 추론하지 않는다.
- 각 part에는 입력을 받는 하나의 primary component와 필요한 장식·표시 component를 연결할 수 있다. mesh, material, 색상, thickness/크기, 상대 transform, Idle/Hover/Active 표시를 BP에서 설정한다. 셰이프 자체의 회전·상대 배치를 바꾸어도 의미상 X/Y/Z 조작축은 공통 ITF 좌표계 정책을 따른다.
- 보이는 geometry와 클릭 영역이 같은 화면 변환을 사용한다. 복잡한 mesh는 명시적인 collision/hit proxy를 제공하며 ring의 빈 중앙을 전체 원판처럼 판정하지 않는다. renderer 가시성 callback에서 BP를 실행하지 않는다. BP 표시 알림은 game thread에서 전달한다.

장식 component는 handle 입력 및 일반 Actor 선택 trace를 방해하지 않아야 한다. Gizmo Actor 자체는 편집 대상 선택·Duplicate·Delete·Actor StateStore에서 제외한다. 대상의 음수·비균일 scale이 시각 Actor를 뒤집거나 왜곡하지 않도록 표시 transform과 대상 transform을 분리한다.

필수 part 목록은 설정에 명시하고 검증한다. optional part가 없으면 대응 handle·capability를 비활성화한다. 중복 PartId, 잘못된 component 소유자, 지원하지 않는 hit geometry, 필수 part 누락은 오류를 반환한다. 사용자 BP 설정 오류에 `checkf`로 게임을 종료하지 않으며 부분 생성 Actor를 정리하고 설정에 지정된 기본 Gizmo fallback과 사유를 표시한다. 내부 ITF 불변 조건에만 assert를 사용한다.

### CUSTOM-03. Runtime 연결과 공통 상태

Mode 설정 또는 Context의 Gizmo provider가 사용할 BP class와 필수 part·표시 설정을 전달한다. Builder가 이 설정을 ActorFactory에 주입하고 Tool은 provider의 Gizmo ID를 요청한다. native class·프로젝트 asset 경로를 builder에 하드코딩하지 않는다. 설정 asset에서 BP class와 mesh/material을 참조하여 cook 대상이 되게 한다. soft reference를 사용하면 AssetManager/cook 및 사전 로딩 정책을 함께 제공하고 입력 capture 도중 동기 load를 수행하지 않는다.

생성 순서는 `설정 검증 → BP spawn/Construction 완료 → component resolve·role 검증 → ITF handle binding → TransformProxy target 연결`이다. 미완성 Actor를 사용자 입력에 노출하지 않는다. GizmoManager가 Setup/Shutdown과 Actor 해제를 소유하며, BP가 별도로 Mouse Tick·Actor SetTransform으로 변환 경로를 우회하지 않는다.

Custom과 기본 Gizmo는 COORD·SELECT·HISTORY·DRAG의 같은 계약을 사용한다. BP에서는 외형과 표시 반응을 바꾸고, 좌표계·pivot·snapping·Transform 수학·Undo 명령은 공통 native 경로가 결정한다. 중복 입력·중복 Transform 적용을 막는다. 스킨 변경 자체는 편집 대상 Actor의 변경 이력으로 기록하지 않는다.

표시 우선순위는 `Active > Hover > Idle`이며 Disabled/Hidden part는 hit 불가다. native adapter가 hover/interacting 상태를 분리 보관하고 BP 재질 및 대체 mesh에 동일 상태를 전달한다. BP가 이벤트를 구현하지 않아도 기본 강조가 작동해야 한다. 선택 해제·Tool 전환·모드 종료·대상 파괴에서 capture, 원래 재질·가시성, 임시 Actor를 정리한다. capture 중 스킨 교체는 요청을 거부하고 이유를 반환하여 조작 도중 component를 교체하지 않는다.

모니터에는 `GizmoProviderId`, 선택된 `VisualActorClass`, 생성 상태, 지원 part 목록, `HoveredPartId`, `CapturedPartId`, fallback/validation 사유를 typed Current로 노출하고 JSON은 그 projection으로 제공한다. 시각 asset 설정과 편집 대상 Actor 상태를 별도 노드에 표시한다.

### CUSTOM-04. 샘플과 제공 범위

모양이 서로 다른 Gizmo Blueprint 2종을 실제 content asset으로 제공한다. 동일 Actor를 두 스킨으로 T/R/S 조작하고 똑같은 논리 delta가 적용됨을 확인한다. Playground에 스킨 선택 경로를 제공하고 선택된 BP class를 표시한다. 샘플만의 component 이름 분기 없이 다른 프로젝트에서도 같은 설정으로 적용되어야 한다.

기본 지원 범위는 위 part에 대응하는 mesh 기반 Transform Gizmo다. 임의 Blueprint의 어떤 component·입력 알고리즘도 자동으로 Gizmo가 되는 범위는 아니다. 새로운 조작 의미나 렌더/hit 형식은 별도 native handle/provider adapter로 확장한다. 이 절은 요구사항이며 기존 Playground 패키지에 해당 샘플이나 선택 기능이 구현됐다는 뜻이 아니다.

## 10. Runtime 구성과 확장 경계

배포 모듈은 기존 `OWTEventCore`, `VTBOWTEditor` 두 개를 유지한다. Duplicate·Delete·history를 다른 프로젝트에서 적용할 수 있도록 서비스 경계를 두되 외부 별도 Duplicate 모듈을 다시 만들지 않는다. 범용 캡처·복원 부분은 특정 UI, Pub-Sub, BP 클래스에 의존하지 않는다.

| 책임 | 제안 구성 | 계약 |
|---|---|---|
| 세션 수명 | Subsystem 소유 `UOWTEditingSession` | Mode 재생성과 독립적인 ID·선택·history 수명 |
| ID와 선택 | Identity registry, Selection context | logical ID↔현재 weak instance, 순서·활성 객체·revision |
| 편집 실행 | Runtime scene edit service | Transform/Duplicate/Delete의 preflight·commit·rollback 조율 |
| history | Runtime history service/executor | command payload, cursor, ITF bridge, 비동기 replay |
| 객체 복원 | Duplication 내부 capture/restore core 및 adapter | Clone과 Restore의 ID·이름 정책 구분, PCG 수명 처리 |
| 도구 실행 | 기존 AttributeEditMode / ToolManager / Context | 위 서비스를 조회, Tool 수명·입력·Gizmo 관리 |
| 관찰 상태 | AttributeEditor 소유 StateStore | Actor 실제 상태와 baseline, session registry ID 사용 |
| 전송 | AttributeEditor 소유 OWTEventCore Pub-Sub | typed 결과→JSON, 기존 owner-scoped 수명 유지 |
| 화면 | Details와 Monitor view model | typed snapshot 조회 및 명령 제출, 직접 Actor 수정 금지 |

새 클래스명은 책임을 표현하는 제안이며 동등한 기존 구현은 확장해서 사용한다. session registry와 StateStore가 각각 ID를 발급하는 이중 원장은 만들지 않는다. 외부 facade 교체·Mode 교체는 동일 세션의 registry를 다시 사용한다.

`UnrealEd`, `GEditor`, `FScopedTransaction`, `UTransBuffer`, Editor typed element 구현을 Runtime 의존성으로 추가하지 않는다. 공개 header에 PCG 세부 구현을 노출하지 않는다. adapter 선택·provider 등록으로 확장하며 Actor 클래스명·graph asset 경로·샘플 레벨에 대한 분기를 만들지 않는다.

사용자 입력·외부 객체 소멸·unsupported native 상태는 구조화된 오류로 처리한다. `check/checkf`는 game-thread, 단일 명령 실행 등 내부 불변 조건에 사용한다. 조건은 의미별 guard로 나누고 과도한 `&&`/`||` 체인이나 중복된 방어 검사를 피한다. 기존 `Docs/CppStyle.md`와 `.clang-format`을 따른다.

기존 BP·CAC·PCG adapter의 지원 경계는 유지한다. 실행 중 Blueprint 스택, 타이머, 비협력 Construction의 외부 부수 효과, 임의 native 자원, 네트워크 authority/replication, 전체 World 저장·로드는 이 명세만으로 복원 가능해지지 않는다. 지원 불가를 감지하지 않은 채 “모든 BP 완전 Undo”를 표시하지 않는다.

## 11. 인수 조건

| ID | 재현 및 합격 기준 |
|---|---|
| A01 | World Translation → Scale → Rotation: effective 값이 World/Local/World, preference는 World 유지. Local 선호도 같은 방식으로 확인 |
| A02 | Scale의 World 요청은 UI·BP·JSON·ITF 경로에서 동일하게 제한. 모니터·축 방향·실제 적용값 일치 |
| A03 | 부모/socket이 있는 Actor의 각 Relative/World 전환 후 world pose 유지. Undo/Redo로 플래그와 실제 값 복원 |
| A04 | Ctrl 클릭 선택/해제, 빈 공간, 활성 객체 제거, UI 입력 차단 확인. 전체 선택과 ITF selection 일치 |
| A05 | 위치·회전·scale이 서로 다른 A/B를 두 순서로 선택하여 마지막 선택 pivot·Local 방향 확인. 평균 pivot으로 바뀌지 않음 |
| A06 | 단일/다중 T/R/S, 부모·자식 동시 선택, 음수·0·비균일 scale을 UE 5.7 기준 fixture와 비교. 이중 delta·NaN 없음 |
| A07 | 다중 Details X 변경 시 X만 지정값, 나머지 축은 개별 보존. Mixed를 0으로 처리하지 않음 |
| A08 | A↔B 참조, 선택 부모+자식, CAC, 선택 밖 부모를 포함한 batch Duplicate가 하나의 매핑으로 연결. 기본 선택 복제와 AuthoredHierarchy 범위 구분 |
| A09 | 같은 family 동시 복제·삭제·재복제의 번호 충돌 없음. Ctrl+D offset 0. Alt drag 한 번당 복제·이동 transaction 1개, 취소 시 잔여물 0 |
| A10 | Delete는 선택/CAC를 제거하고 미선택 일반 자식 KeepWorld 유지. Undo는 객체뿐 아니라 부모·socket·참조·선택까지 복원 |
| A11 | `이동 A → 복제 B → 이동 B → 삭제 B → Undo×4 → Redo×4`를 GC와 Tool 재생성 사이에 실행. 논리 ID 유지, 오래된 UObject 포인터 사용 없음 |
| A12 | 드래그 100회 update는 Current를 갱신하고 Undo 1건. Cancel/no-op은 0건. Undo 후 새 commit 시 redo branch 폐기 |
| A13 | A/B 필터와 현재 선택에 관계없이 global Undo 순서 준수. 실패·외부 변경 충돌·이름 충돌 시 cursor 유지·사유 표시 |
| A14 | PCG Generating/Ready/Waiting/CleaningUp 각각에서 삭제·복제 Undo·복원. 원본 자원 보존, stale callback 차단, 생성물·local component 누수 없음 |
| A15 | Current Tree/JSON의 revision·값 일치. 외부 Actor 변화·Undo·복원 표시, changed/baseline/preview 구분, 조회만으로 ObjectRevision 불변 |
| A16 | 선택 A 중 B의 PCG 이벤트 수신 시 B target에 연결. 손상·재전달 JSON으로 Current를 덮어쓰거나 명령을 중복 실행하지 않음 |
| A17 | 이벤트 Pause/Clear/필터가 Current·Undo·Actor에 영향 없음. 삭제 객체 이력과 tombstone 유지, retention/관찰 공백 표시 |
| A18 | 모든 축/평면/ring/중앙 handle을 잡고 축 밖·다른 축·UI 위로 드래그해도 원래 조작 강조 유지. release/cancel/focus loss 후 강조와 capture 정리 |
| A19 | 실제 RHI에서 Current 100 ms 목표 및 100객체/1,000행 fixture 측정. 작은 창·DPI 변경에서도 트리/JSON/버튼/스크롤·포커스 사용 가능 |
| A20 | 원본·샘플 없는 독립 호스트의 packaged build에서 신규 검증과 v3 BP/CAC/PCG/입력 회귀 통과. Editor-only 의존성 없음 |
| A21 | 제공 BP 2종과 사용자가 셰이프·재질·상대 배치를 수정한 BP가 C++ 수정 없이 cook 후 실제 T/R/S handle로 작동. Tool이 설정된 provider/class를 생성 |
| A22 | enum part마다 축·평면·uniform scale 의미를 검증. 동일 논리 delta에 두 스킨과 기본 Gizmo의 target Transform·snap·transaction 결과 동일 |
| A23 | 원근·직교 뷰, 거리·FOV·DPI 변경 및 대상 음수·비균일 scale에서 보이는 셰이프와 hit 영역 일치. ring 내부 오검출·장식 입력 차단 없음. 모든 part에서 축 밖 드래그의 Active 강조·release/cancel 정리 확인 |
| A24 | 누락 class/mesh·중복 role·외부 소유 component·필수 part 누락을 crash 없이 진단. 선택한 fallback 적용 및 Current 사유 표시. 부분 생성 Actor·입력 등록 잔여 없음 |
| A25 | mode/target 교체·world teardown·GC 및 반복 스킨 교체에서 Gizmo Actor/behavior 수명 정상. capture 중 교체 거부. BP 및 종속 asset이 Editor 없는 독립 packaged host에 포함됨 |

Editor 비교 fixture는 엔진 버전, pivot/좌표계, snap on/off 및 step, additive scale 설정, selection 순서를 기록한다. 동일한 논리 delta를 입력하고 world/relative TRS와 attachment 결과를 비교한다. 기본 합격 오차는 CURRENT-02와 같고 예외는 원인과 수치를 남긴다. 포인터 이동량만 같다고 논리 delta가 같다고 가정하지 않는다.

자동화는 상태·원자성·참조·history·수명 계약을 검증하고, 실제 RHI 입력 검증은 강조 재질·capture·UI 갱신을 확인한다. 화면 캡처만으로 Undo 동작을 검증했다고 하거나 NullRHI 결과만으로 시각 문제 해결을 완료 처리하지 않는다.

## 12. 병렬 구현 분담과 순서

가용 동시 슬롯은 **총 4개**, 권장 구성은 **통합 담당 1개 + 하위 에이전트 3개**다. selection/history/복원 계약을 먼저 고정한 뒤 파일 소유 범위를 나눠 병렬 작업한다. 공유 header를 각자 다른 의미로 수정하는 방식은 피한다.

| 담당 | 소유 범위 | 핵심 결과 |
|---|---|---|
| 통합 담당 | Session/identity·shared types·Mode·facade·ITF context·공용 history service·통합 검증 | 명령 수명, schema 호환, session stack, 전체 빌드·패키징 |
| 하위 1: 선택·Gizmo | selection/transform 정책 구현, AttributeEditTool, Gizmo·input·BP visual adapter와 테스트 | Editor 좌표계·pivot·다중 변환, capture 강조, Blueprint 셰이프 binding |
| 하위 2: 객체 수명 | Duplication capture/restore core·Delete·PCG adapter·scene command payload와 테스트 | batch map, 삭제/복원, PCG cleanup, undoable lifecycle command |
| 하위 3: 관찰·화면 | StateStore projection·Monitor view model·Details/Current/History/Events UI와 테스트 | 실시간 tree/json, diff, mixed 편집, 객체 history 조회 |

공유 파일 변경은 통합 담당이 수행한다. 하위 1은 입력·selection/Gizmo change 계약, 하위 2는 history executor용 비동기 command 계약, 하위 3은 typed snapshot·capability 계약을 먼저 전달받는다.

1. **계약 확정:** 세션 ID·selection·좌표계·command/history·schema 타입, 대상/계층 정책, 실제 drag 재현 및 Editor 비교 fixture.
2. **기반 구현:** identity/selection/history 골격과 ITF 연결, actor capture/restore 경계. 샘플 없는 호스트에서 컴파일 확인.
3. **병렬 기능 구현:** 다중 Transform/강조, batch Duplicate/Delete/PCG, Current/History UI를 담당별 구현.
4. **통합 검증:** 실제 command commit/replay 연결, 실패·재진입·GC·외부변경 검증, A01–A25 및 v3 회귀. Blueprint Gizmo 샘플 2종의 실제 RHI 입력·cook 포함 여부도 확인.
5. **완료 기록:** 통과·실패·미지원 항목, RHI 사용성 증거, 독립 호스트 결과와 배포 문서를 갱신. 검증 전 capability를 완료로 표시하지 않음.

## 13. 조사 근거

아래는 2026-10-08에 확인한 로컬 UE 5.7 및 v3 소스다. 라인 번호는 문서 작성 시점 기준이며 구현 후 이동할 수 있다. `[E*]`는 엔진 동작 근거, `[P*]`는 현재 구현 상태, 그 외 정책·한도·API 이름은 이번 명세의 설계 결정이다.

- [E1] [EditorModeManager.cpp:714](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/EditorModeManager.cpp:714>): Scale effective coordinate와 raw preference.
- [E2] [EditorSelectUtils.cpp:437](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/EditorSelectUtils.cpp:437>), [EditorModeManager.cpp:1136](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/EditorModeManager.cpp:1136>): 마지막 조작 가능 선택의 pivot 및 Local 방향. [ActorElementLevelEditorSelectionCustomization.cpp:406](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/LevelEditor/Private/Elements/Actor/ActorElementLevelEditorSelectionCustomization.cpp:406>): 선택 부모에 포함된 자식의 조작 목록 정규화.
- [E3] [ComponentTransformDetails.cpp:798](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/DetailCustomizations/Private/ComponentTransformDetails.cpp:798>): Absolute 플래그·부모 socket 기준 값 보존·한 transaction. [같은 파일:1185](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/DetailCustomizations/Private/ComponentTransformDetails.cpp:1185>): 상대값과 mixed cache.
- [E4] [ActorEditor.cpp:895](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/Engine/Private/ActorEditor.cpp:895>): additive/percentage scale 및 pivot 위치 보정. [ActorElementEditorViewportInteractionCustomization.cpp:50](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/Elements/Actor/ActorElementEditorViewportInteractionCustomization.cpp:50>): 공통 pivot 회전·이동·scale 적용.
- [E5] [TransformProxy.cpp:143](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/TransformProxy.cpp:143>): 기본 다중 component proxy의 중심/방향 계산.
- [E6] [LevelActor.cpp:927](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/Engine/Private/LevelActor.cpp:927>): 파괴 시 일반 attached 자식 KeepWorld 분리.
- [E7] [InputRouter.cpp:298](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/InputRouter.cpp:298>), [AxisPositionGizmo.cpp:175](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/AxisPositionGizmo.cpp:175>): capture와 hover 종료, 별도 interacting 수명. [ViewAdjustedStaticMeshGizmoComponent.cpp:391](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/ViewAdjustedStaticMeshGizmoComponent.cpp:391>), [GizmoPrivateUtil.cpp:169](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/GizmoPrivateUtil.cpp:169>), [CombinedTransformGizmo.cpp:494](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/CombinedTransformGizmo.cpp:494>): mesh 재질·interaction, 직접 interface 연결, rotation substitute 생성.
- [E8] [EditorActor.cpp:563](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/EditorActor.cpp:563>): 선택 Actor 목록 복제. [같은 파일:405](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Editor/UnrealEd/Private/EditorActor.cpp:405>): 부모·자식 offset 중복 방지와 외부 attachment 유지. [OWTRuntimeActorDuplicator.cpp:1218](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Duplication/OWTRuntimeActorDuplicator.cpp:1218): 현재 Owner/Instigator 및 외부 부모 복원 정책.
- [E9] [PCGComponent.cpp:1180](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/PCG/Source/PCG/Private/PCGComponent.cpp:1180>), [같은 파일:1575](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/PCG/Source/PCG/Private/PCGComponent.cpp:1575>), [같은 파일:1915](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/PCG/Source/PCG/Private/PCGComponent.cpp:1915>): generation 취소, 즉시 cleanup 접근성 조건, EndPlay 경계.
- [E10] [InteractiveToolManager.cpp:465](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/InteractiveToolManager.cpp:465>): Tool-local wrapper와 영속 change의 Context transactions 직접 기록.
- [E11] [AxisPositionGizmo.cpp:27](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/AxisPositionGizmo.cpp:27>), [AxisAngleGizmo.cpp:25](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseGizmos/AxisAngleGizmo.cpp:25>), [AnyButtonInputBehavior.cpp:9](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Private/BaseBehaviors/AnyButtonInputBehavior.cpp:9>): 기본 Gizmo 우선순위 및 왼쪽 버튼.
- [E12] [ModelingToolsEditorMode.cpp:360](<C:/Program Files/Epic Games/UE_5.7/Engine/Plugins/Editor/ModelingToolsEditorMode/Source/ModelingToolsEditorMode/Private/ModelingToolsEditorMode.cpp:360>): transform Gizmo Context 등록. [CombinedTransformGizmo.h:35](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Public/BaseGizmos/CombinedTransformGizmo.h:35>): ActorFactory와 component별 sub-Gizmo의 확장 계약 및 BP 메타데이터. [HitTargets.h:50](<C:/Program Files/Epic Games/UE_5.7/Engine/Source/Runtime/InteractiveToolsFramework/Public/BaseGizmos/HitTargets.h:50>): Component hit 및 hover/interacting 분리.
- [P1] [OWTAttributeEditMode.h:99](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Public/Modes/OWTAttributeEditMode.h:99): 단일 Actor 선택 API.
- [P2] [VTBOWTEditorToolsContext.cpp:129](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Context/VTBOWTEditorToolsContext.cpp:129), [VTBAttributeEditor.cpp:1752](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/VTBAttributeEditor.cpp:1752): 미구현 transaction·Undo/Redo 경로.
- [P3] [OWTAttributeStateStore.cpp:6](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/State/OWTAttributeStateStore.cpp:6): weak Actor 등록, baseline, 단일 snapshot. [OWTAttributeDetailsWidget.cpp:259](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/UI/OWTAttributeDetailsWidget.cpp:259): 기존 Details/Events 화면.
- [P4] [VTBAttributeEditor.cpp:591](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/VTBAttributeEditor.cpp:591), [같은 파일:910](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/VTBAttributeEditor.cpp:910): procedural 이벤트와 선택 snapshot의 공통 JSON 생성.
- [P5] [VTBOWTTransformGizmoBehavior.h](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Public/Gizmos/Base/VTBOWTTransformGizmoBehavior.h), [VTBOWTAxisPositionGizmo.h](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Public/Gizmos/Base/VTBOWTAxisPositionGizmo.h), [VTBOWTAxisAngleGizmo.cpp](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Gizmos/Transform/VTBOWTAxisAngleGizmo.cpp): 가독성 정리 후 입력·위치·회전 책임별 파일. 분석 당시 결합 파일은 `Saved/Backups/BeforeReadability_20261008_011448/PluginSource/VTBOWTEditor`에 보존.
- [P6] [VTBOWTTransformGizmoActor.cpp:42](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Gizmos/Transform/Custom/VTBOWTTransformGizmoActor.cpp:42): native Actor 고정 생성. [OWTAttributeEditMode.cpp:134](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Modes/OWTAttributeEditMode.cpp:134): Builder 등록. [OWTAttributeEditTool.cpp:297](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Tools/OWTAttributeEditTool.cpp:297): 기본 Gizmo ID 고정 사용.
- [W1] [Epic: Transforming Actors, UE 5.7](https://dev.epicgames.com/documentation/en-us/unreal-engine/transforming-actors-in-unreal-engine?application_version=5.7): Details 다중 값 입력, Transform 행과 Gizmo, Alt+Translation 복제의 사용자 동작 참고. 단축키 및 Scale 허용의 정확한 기준은 설치된 5.7 소스와 이 문서의 명시 계약을 우선한다.
- [W2] [Epic: Selecting Actors, UE 5.7](https://dev.epicgames.com/documentation/en-us/unreal-engine/selecting-actors-in-unreal-engine?application_version=5.7): 클릭 대체·Ctrl 토글, 다중 선택의 사용자 동작.

기존 설계·검증 기록: [v3 ITF 설계](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Docs/AttributeEditMode_ITF_Spec.md), [구현·검증 기록](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Docs/RuntimeAttributeEditor_Requirements.md), [사용법](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Docs/RuntimeAttributeEditor_Usage.md).
