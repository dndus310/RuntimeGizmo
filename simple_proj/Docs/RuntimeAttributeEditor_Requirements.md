# Runtime AttributeEditor 요구사항 및 병렬 작업 명세

> 첫 절은 **v3의 실제 구현·검증 기록**이다. 이후 v2 명세와 과거 결과는 비교를 위해 보존한다. 현재 설계는 [AttributeEditMode_ITF_Spec.md](AttributeEditMode_ITF_Spec.md), 설치·API·확장 방법은 [RuntimeAttributeEditor_Usage.md](RuntimeAttributeEditor_Usage.md)를 기준으로 한다.

## v3 구현 및 검증 — 2026-10-08

Unreal Engine 5.7, Windows Development에서 구현과 검증을 완료했다. 통합 담당 1명과 ITF, 복제·PCG, UI·계약 검증 담당 3명이 파일 소유 범위를 나눠 작업했다. 기존 v2 결과를 재사용하지 않고 아래 환경에서 다시 실행했다.

Runtime 모드의 소유권은 `Subsystem → UOWTAttributeEditMode → ToolsContext / ToolManager / 도구·복제 세션`으로 구성했다. `AttributeEditTool`과 `DuplicateTool`은 builder 등록으로 실행되며, 외부 Tool·TargetFactory·Context provider를 등록·해제할 수 있다. UI는 등록된 도구와 사용 가능 사유를 조회한다. 기존 `VTBOWTObjectEditMode` script 경로는 호환 wrapper로 유지한다.

`AttributeEditor`는 소유 Pub-Sub와 Actor 관찰 상태 저장소를 유지한다. Actor의 실제 값, Mode·복제 operation·PCG의 typed 상태, JSON 전달을 구분했다. 복제 요청 접수, authored 계층 commit, PCG `Ready`는 별개 상태다. 이벤트 콜백이 모드나 선택을 변경해도 이전 이벤트·자동 선택이 최신 상태를 덮어쓰지 않도록 검증한다.

독립 `OWTRuntimeDuplication` 모듈은 제거하고 구현을 `VTBOWTEditor/Duplication`으로 통합했다. 배포 플러그인은 Runtime 모듈 `OWTEventCore`, `VTBOWTEditor` 두 개이며 PCG를 필수 의존성으로 사용한다. 클래스 이관 redirect는 새 호스트에서도 로드되는 `Config/Engine.ini`에 둔다. 기존 C++ 소비자의 Build.cs는 `VTBOWTEditor` 의존성으로 변경해야 한다. 이전 bool `RequestDuplicate`의 성공은 접수를 뜻하며 완료는 operation 상태로 관찰한다.

복제는 이름의 `_숫자` 증가, authored attachment와 ChildActorComponent 계층, 지원 객체·component·instanced UObject 참조 복원을 포함한다. PCG GraphInstance·parameter·seed·generation policy·지원 custom scheduling policy를 복원하고 generated ISM/Actor·cache·task를 공유하지 않는다. PCG 실행은 authored commit 후 시작하며 partition/HiGen local component의 생성·정리·재생성을 관찰한다. 어댑터는 가장 구체적인 Primary 하나와 적용 가능한 Auxiliary를 선택하고 소유권 충돌을 거부한다.

| 검증 | 결과 | 근거 |
|---|---|---|
| 원본 프로젝트 Editor 빌드 | PASS | `Saved/Logs/V3_EditorVerifiedBuild2.log`, 추가 fixture 포함 `V3_EditorChildActorBuild.log` |
| Editor의 실제 RHI 게임 실행 | 21/21 PASS, warning/error 0 | `Saved/Automation/V3_RHIVerified/index.json` |
| 추가 cooked BP ChildActor + OnLoad | 1/1 PASS, warning/error 0 | `Saved/Automation/V3_PCGChildActor/index.json` |
| 원본 Game build·cook·stage | PASS | `Saved/Logs/V3_HostPackage.log`; 테스트 C++ 추가 후 기존 cooked content를 사용한 `V3_HostPackageFinal.log` |
| 최종 원본 packaged 실제 RHI 실행 | **22/22 PASS, warning/error 0, skip 0** | `Saved/Automation/V3_HostPackagedFinal/index.json`, `Saved/Logs/V3_HostPackagedFinalTests.log` |
| 고급 Native/BP/CAC·MID·물리·BeginPlay 회귀 | PASS, warning/error 0 | `Saved/Logs/V3_DuplicationFinalRegression.log`의 `OWT_DUPLICATION_VALIDATION: PASS` |
| 기존 BP 입력 및 샘플 레벨 회귀 | PASS | `Saved/Logs/V3_InputRegression.log`, `V3_LevelRegression.log` |
| 독립 소비 프로젝트 Editor 빌드·계약 검사 | PASS; 19개 실행 통과, viewport 2개는 NullRHI skip | `Saved/Logs/V3_ProbeEditorVerifiedBuild2.log`, `Saved/Automation/V3_ProbeEditorVerified/index.json` |
| 독립 소비 프로젝트 Game build·cook·stage | PASS | `Saved/Logs/V3_ProbePackage.log` |
| 독립 packaged 실제 RHI 실행 | **22/22 PASS, warning/error 0, skip 0** | `Saved/Automation/V3_ProbePackaged/index.json`, `Saved/Logs/V3_ProbePackagedTests.log` |
| 배포 일치 검사 | Source/Config/Content/descriptor/README **104개 파일 SHA-256 일치** | 원본·`Saved/PortabilityProbeV3` 복사본·배포 폴더·ZIP entry 비교. `Saved/Distribution/V3/manifest.sha256.json`, `archive-verification.json` |

독립 소비 프로젝트는 `/Game/VTBOWT` 샘플 없이 `/Engine/Maps/Entry`와 복사한 플러그인만 사용한다. plugin-owned cooked BP/graph는 배포 Content에 포함한다. 원본의 `OWT.Runtime.Attributes`와 독립 프로젝트의 `OWT.Portability.IndependentHost`는 각각의 host 전용 검사이며 나머지는 동일한 plugin 테스트다.

PCG 검증은 실제 CPU CreatePoints→StaticMeshSpawner graph를 사용했다. 일반 BP attachment, live BeginPlay의 cooked BP ChildActor + OnLoad, non-partition/partition/HiGen runtime scheduler, 생성 소스 이동·cleanup·복귀, GraphInstance/parameter/policy 참조, 원본과 복제본의 자원 분리를 확인했다. ChildActor 검사는 authored commit 시 PCG 비활성·미생성·출력 0을 확인하고 그 후 `Generating → Ready`와 독립 출력 2개를 확인한다. 기본 unbounded PCG 반경이 매우 크므로 거리 cleanup 테스트는 명시적 생성 반경을 사용한다.

실제 viewport의 HUD 프레임에서 Tool `Render/DrawHUD` 호출을 검증했다. packaged Details·Events PNG를 열어 필드·도구 버튼·작업 상태·스크롤 영역의 배치를 확인했다. 원본 화면 사본은 `Saved/Verification/V3/Packaged_Details.png`, `Packaged_Monitor.png`다. 독립 프로젝트도 실제 RHI에서 두 viewport 검사를 실행하고 캡처를 확인했다. 이는 마우스·키보드 전체 사용자 동선에 대한 수동 사용성 검사를 뜻하지 않는다.

배포본은 [OWTRuntimeEditing_UE5.7_v3.zip](../Saved/Distribution/OWTRuntimeEditing_UE5.7_v3.zip)이다. 소스·설정·콘텐츠·descriptor·README만 포함하며 Binaries/Intermediate 및 제거한 duplication 모듈을 포함하지 않는다. SHA-256은 `B1EF88F21E708AC36F11F24C966C93C2BF113058FB795307E56E229D62BEAEA9`다. 이전 이름의 v2 ZIP과 `Saved/PortabilityProbe`는 과거 자료이며 이번 배포본과 구분한다.

지원 경계는 유지한다. GPU PCG graph는 검증하지 않았고, material mesh 렌더가 필요한 일반 Modeling Tool과 범용 Undo history는 현재 host capability에서 비활성화된다. 임의 native/opaque 상태, Actor 소유 graph definition, 복제 억제를 무시하고 자체 PCG cleanup을 시작하는 Construction은 전용 협력 adapter가 필요하다. 일반 attached Actor의 BeginPlay가 모든 sibling 참조 연결보다 먼저 실행될 수 있으므로 전체 계층의 준비 경계는 authored commit이다. partition/runtime PCG는 구성된 PCGWorldActor가 필요하다. 자세한 API와 실패 사유는 사용법 문서에 기록했다.

## v2 기록 시작

작성일: 2026-10-07 · 대상: `simple_proj`, Unreal Engine 5.7

이 문서는 v2의 승인된 요구사항과 현재 구현된 C++ API를 함께 정리한다. `OWTRuntimeEditing` 플러그인은 범용 소유 Pub-Sub, Actor 복제, typed 관찰 상태와 Details/Events UI를 세 Runtime 모듈로 제공한다. Editor 빌드와 기능별 실행 검증은 구분하며, 최종 검증 결과는 12절에 별도로 기록한다. 기존 v1 결과는 역사적 기록으로만 유지한다. 실행법과 C++/BP 연결 예시는 [RuntimeAttributeEditor_Usage.md](RuntimeAttributeEditor_Usage.md)를 참고한다.

## 2026-10-07 추가 구현 명세: 상태·이벤트 분리와 재사용

이번 확장에서는 Actor에서 관찰한 typed 상태와 JSON 이벤트 전달을 별도 객체에 둔다. 아래 명세가 이번 확장의 구현 기준이다. 검증 전 항목을 완료로 간주하지 않는다.

| 담당 | 동시 작업 수 | 구현 및 확인 범위 |
|---|---:|---|
| 통합 담당 | 1 | typed 상태 저장소, 편집 API 통합, 플러그인 구성, 빌드·패키징·별도 프로젝트 검증 |
| 이벤트 담당 | 1 | 범용 소유 Pub-Sub 추출, 이벤트 기록·수명·재진입 테스트, 샘플 BP 없는 기본 입력 |
| 복제 담당 | 1 | 이름 증가, ChildActor 그래프 복제·참조 치환·복원 훅, 복제 회귀 검증 |
| 모니터 담당 | 1 | 상태 요약, 이벤트 조회 UI, 편집 종료 상태의 모니터와 입력 차단 검증 |

가용 슬롯은 총 4개다. 현재 독립 구현 경계에 필요한 하위 에이전트는 3개이며 통합 담당까지 4개를 사용한다. 같은 파일의 동시 변경을 피하도록 소유 파일을 나눈다.

### 모듈 및 의존 방향

- 배포 단위: `Plugins/OWTRuntimeEditing`. 엔진 5.7 Runtime 플러그인, Runtime 모듈에서 UnrealEd를 참조하지 않는다.
- `OWTEventCore`: 특정 편집기 클래스에 의존하지 않는다. 외부 소유 UObject가 생성·초기화·강한 참조·종료를 책임진다. singleton과 소유자 없는 서비스는 제공하지 않는다.
- `OWTRuntimeDuplication`: 특정 편집기나 이벤트 시스템에 의존하지 않는다. 월드를 제공하는 소유 UObject에 붙여 단독 호출할 수 있다.
- `VTBOWTEditor`: 위 두 모듈을 사용한다. 기존 모듈 및 편집기 클래스의 script 경로를 유지하고 옮긴 서비스에는 CoreRedirects를 제공한다.
- `UOWTAttributeStateStore`: 선택 Actor의 GUID/weak 참조, 비교 기준 Transform, 마지막 관찰 snapshot을 소유한다. JSON을 파싱하거나 이벤트를 발행하지 않는다. 변경은 소유 편집기만 가능하며 조회는 값 복사본이다.
- `AVTBAttributeEditor`: 요청 검증 → 실제 Actor 적용 → 상태 재관찰 → JSON 결과 발행 순서를 조율한다. JSON 요청을 상태 저장소에 직접 대입하지 않는다.

### 모니터 계약

상단에 편집 활성, 대상 이름·ID, 선택/상태 revision, 편집 모드, Gizmo 종류·좌표계, 수정 중/변경 여부를 표시한다. 클래스와 Transform은 Details에서 확인한다. Details와 Events 탭을 제공한다. F2는 편집 전환, F3는 편집이 꺼져 있어도 모니터를 연다.

이벤트 기록에는 증가 sequence, UTC timestamp, inbound/outbound 방향, 이벤트명, 원래 JSON, JSON 유효성 및 기록 잘림 여부를 보관한다. 기본 256건, 최대 1024건이며 한 기록의 JSON은 16,384 문자로 제한한다. 전송 한도는 별도로 65,536 문자다. JSON 요청은 파싱 전에 inbound로 기록하여 거부된 입력도 확인한다.

UI는 최신순 256건, 필터, 펼쳐 보는 payload, 일시 정지, 표시 지우기를 지원한다. 일시 정지해도 현재 상태는 갱신한다. 표시 지우기는 UI의 기준 sequence만 바꾸며 서비스 기록을 삭제하지 않는다. 모니터를 연 상태의 UI hover/focus/drag는 편집이 꺼져 있어도 월드 입력을 차단한다.

### 복제 계약

- 이름은 마지막 `_숫자`를 제거한 family 기준으로 증가한다. 예: `Chair → Chair_1 → Chair_2`, `Chair_4 → Chair_5`. 같은 Level의 기존 이름·충돌·소유 서비스의 사용 이력을 고려하며 복제본을 다시 복제해도 같은 family를 쓴다.
- 런타임에 생성 가능한 cooked BP 및 C++ Actor의 반영 가능한 구성, 컴포넌트, instanced UObject, ChildActorComponent 계층을 복제한다. 그래프 내부 참조는 복제본으로 바꾸고 외부 객체 참조는 유지한다.
- Construction은 엔진의 정상 경로로 실행하고 그래프 복원을 루트/자식 BeginPlay 전에 완료한다. 사용자 Construction 코드의 외부 부수 효과까지 롤백하지는 않는다.
- native 비반영 데이터나 외부 리소스는 `IOWTRuntimeDuplicationParticipant`의 복원 훅으로 확장한다. 지원되지 않는 구성은 사유와 함께 실패시키고 부분 복제 Actor를 정리한다.
- 서비스 Actor/Gizmo의 복제 금지는 편집 모듈의 정책이다. 범용 복제 모듈에 편집기 타입 의존성을 넣지 않는다.
- 기존 네트워크/영구 저장/완전한 Undo·Redo 제외 범위는 유지한다.

### 완료 확인

1. Editor 및 Windows 게임 빌드에서 새 Runtime 모듈이 링크된다.
2. 상태 조회 복사본·거부 요청·실제 외부 Actor 변경·baseline·선택 파괴 테스트를 통과한다.
3. 소유자 격리·수명·콜백 중 구독 변경·재발행·기록 제한을 검증한다.
4. 이름 증가·복제본 재복제·컴포넌트 및 BP 구성·ChildActor 내부 참조·BeginPlay 관찰값·실패 정리를 검증한다.
5. 모니터 pause/resume/filter/clear와 편집 종료 후 표시·입력 차단을 검증한다.
6. 원래 `/Game/VTBOWT` 콘텐츠가 없는 별도 소비 프로젝트에서 플러그인 빌드 및 실행 경로를 확인한다.

## 1. 목표와 범위

기존 RuntimeGizmo를 런타임 편집 기능으로 확장한다. `AVTBAttributeEditor`가 편집 요청을 조율하고 실제 Actor를 관찰한 typed StateStore를 유지한다. UI는 typed 상태를 읽고 JSON Pub-Sub은 변경 알림·외부 통합·진단을 담당한다. 범용 이벤트와 복제 서비스는 다른 소유 UObject에서 편집기 없이 사용할 수 있다.

| ID | 필수 요구사항 | 완료 기준 |
|---|---|---|
| R1 | AttributeEditor 소유 Pub-Sub | 소유자 없이 동작하지 않으며 C++와 BP에서 구독·해제·요청 가능 |
| R2 | 편집 상태 조회 | 편집 활성 여부, 선택 대상, 활성 모드, Gizmo 도구·좌표계, 조작 중 여부, 변경 여부 조회 가능 |
| R3 | 런타임 디테일뷰 | 선택 Actor의 위치·회전·크기 표시, 수치 입력 및 슬라이더/드래그 조절 가능 |
| R4 | 양방향 동기화 | UI·Gizmo·외부 코드 어느 쪽에서 변경해도 실제 Actor 상태를 기준으로 동일 알림 수신 |
| R5 | Runtime Duplicate | C++ Actor와 BP 기반 Actor 인스턴스를 실행 중 복제하고 복제본을 선택 |
| R6 | 명확한 C++ 코드 | 실패 사유별 조건 분리, 내부 불변 조건 assert, 외부 입력의 정상 실패 처리 |
| R7 | Runtime 검증 | 게임 빌드·패키징 경로에서 동작하고 Runtime 모듈에 UnrealEd 의존성 없음 |
| R8 | 상태/이벤트 분리 | 실제 Actor 관찰값만 typed 저장소를 갱신하고 JSON·조회 복사본은 상태를 덮어쓰지 않음 |
| R9 | 이벤트 모니터 | 상태 요약과 제한된 이벤트 이력을 함께 조회하며 편집 OFF에서도 F3으로 접근 |
| R10 | 플러그인 재사용 | 샘플 BP 에셋 없이 native 편집기를 구성하고 범용 이벤트·복제를 독립 모듈에서 소비 |

현재 구현의 범위와 기본값은 다음과 같다.

- 현재 프로젝트처럼 단일 선택, 로컬 런타임 편집을 대상으로 한다.
- 디테일뷰는 World Transform을 표시한다. 위치는 cm, 회전은 degree, 크기는 무단위다.
- “스크롤바로 수정”은 수치 필드의 드래그/슬라이더 조절로 해석한다. 패널 전체에는 별도 ScrollBox를 제공한다.
- “BP 복제”는 이미 로드되거나 쿠킹된 BP 클래스의 월드 Actor 인스턴스 복제를 의미한다.
- 새 Blueprint 자산 생성·컴파일·저장, 임의 속성 전체를 편집하는 범용 Details 패널, 다중 선택, 네트워크 동기화, 영구 저장은 별도 범위다.
- 완전한 Undo/Redo는 미구현이다. 변경 시작·갱신·확정·취소 경계는 구현했으며 기존 SaveHistory/Undo/Redo 골격은 후속 히스토리 연결을 위해 남아 있다.

## 2. 구현 반영 상태

| 확인 대상 | 구현 상태 | 범위 및 연결 |
|---|---|---|
| `AVTBAttributeEditor` | 서비스 소유, 요청 검증, 실제 적용·재관찰·알림 조율 | SaveHistory/Undo/Redo는 여전히 골격 |
| `UOWTAttributeStateStore` | Actor GUID/weak 참조·baseline·typed snapshot | 소유 편집기만 갱신하고 GetSnapshot은 값 복사본 |
| `UOWTNotificationCenter` | 범용 owner-bound JSON Pub-Sub과 제한된 진단 이력 | OWTEventCore의 독립 UObject; 편집 상태 저장 기능 없음 |
| `UOWTRuntimeActorDuplicator` | 같은 standalone 월드의 지원 객체 그래프 복제 | 독립 owner API, 이름 family, ChildActor, native participant 확장 |
| `AVTBOWTEditorGameMode::GetAttributeEditor` | 기존 인스턴스 탐색 또는 생성·캐시 | BeginPlay에서 Subsystem 연결 |
| `UVTBOWTEditorSubsystem` | 편집 활성·선택·Gizmo 수명 및 상태 알림 | 선택 Actor의 외부 Transform 변경 비교, Proxy 재동기화 |
| `UVTBOWTObjectEditMode` | 기존 typed context 라우팅 유지 | DuplicateSelection이 AttributeEditor의 RequestDuplicate 호출 |
| Gizmo 연결 | 기본 선택 Gizmo의 TransformProxy 시작·변경·종료 delegate 연결 | 외부 변경에는 ReinitializeGizmoTransform 사용; 별도 생성 Custom Gizmo는 기존 수명·스냅 회귀 대상 |
| Details / Events | `UOWTAttributeDetailsWidget` | native Slate 9개 double SpinBox·복제 버튼·상태 요약·256건 이벤트 모니터; 별도 Widget 자산 불필요 |
| PlayerController / Spectator | 위젯 수명, F3, UI 입력 차단, native Enhanced Input fallback | 패널 hover/focus/drag만 월드 입력 차단; 프로젝트 BP 없이 기본 입력 구성 |
| ToolsContext transactions | Begin/End/AppendChange 골격 유지 | 이번 변경 이벤트를 실제 Undo 기록으로 보고하지 않음 |
| Runtime Build.cs | OWTEventCore: Core/CoreUObject + private Json; Duplication: Core/CoreUObject/Engine; Editor: 두 모듈+UMG/입력/도구/Slate | 세 모듈 모두 Runtime; UnrealEd 의존성 없음 |

기존 `Pawn → FInstancedStruct → Subsystem → EditMode` 입력 흐름은 유지한다. 기본 매핑이 없으면 native action을 만든다. 이 입력 context의 FInstancedStruct 사용과 8절의 Actor 속성 복제에서 opaque FInstancedStruct를 거부하는 정책은 서로 다른 경계다.

## 3. 책임과 데이터 흐름

```text
Pawn의 편집 Context ──→ Subsystem / ObjectEditMode
                              │ 선택·편집 상태·Gizmo 변경
사이드바 / 외부 명령 ──→ AVTBAttributeEditor ──→ 실제 월드 Actor
                         │ 검증·적용             │ 관찰
                         │                      ▼
                         │             UOWTAttributeStateStore
                         │             GUID / baseline / typed snapshot
                         │                      │ GetSnapshot 복사본
                         │                      └────────→ UI
                         ├─ NotificationCenter: JSON 알림·진단 journal
                         └─ RuntimeActorDuplicator: 지원 그래프 복제

다른 소유 UObject ──→ OWTEventCore / OWTRuntimeDuplication 독립 사용
```

- `AVTBAttributeEditor`는 UI와 외부 코드에 제공하는 편집 API 및 Pub-Sub의 소유자다.
- 기존 Subsystem의 편집 활성 상태와 선택 참조를 원본으로 유지한다. AttributeEditor에 별도의 쓰기 가능한 선택 상태를 중복 보관하지 않는다.
- Transform의 원본은 실제 Actor다. UI 표시값이나 요청 JSON을 실제 상태로 간주하지 않는다.
- typed StateStore에는 JSON이 들어가지 않는다. UI는 이벤트를 상태 무효화 신호로 사용하고 GetSnapshot을 다시 읽는다. JSON만 받는 외부 소비자는 ParseSnapshotJson으로 자신의 복사본을 만들 수 있다.
- `ReceiveEditContext`의 수락과 실제 편집 성공은 구분한다. 성공은 적용 후 상태 이벤트, 실패는 요청 실패 이벤트로 전달한다.
- native GameMode 제공 경로를 기본으로 사용하며 기존 BP 호스트도 유지한다. 네트워크 클라이언트용 소유·복제 정책은 따로 설계한다.

## 4. 소유 Pub-Sub 계약

구현 클래스는 `UOWTNotificationCenter`다. 전역 singleton이나 독립 WorldSubsystem이 아니며, Outer인 UObject가 초기화·강한 참조·종료를 책임진다. 편집기에서는 AttributeEditor가 버스를 내부 소유하고 외부에는 검증된 편집 API만 공개한다. 다른 모듈에서는 자체 소유자를 두고 범용 버스 API를 직접 호출한다.

1. 소유 UObject가 `NewObject<UOWTNotificationCenter>(Owner)`로 생성하고 같은 Owner로 Initialize한다. 서비스는 `UPROPERTY` 등 강한 참조로 유지한다. 편집 통합에서는 AttributeEditor가 소유자다. Outer 지정만으로 GC 수명 보장이 끝났다고 간주하지 않는다.
2. 유효한 소유자와 초기화 없이 발행·구독할 수 없도록 한다. 소유자 없는 생성 경로와 Blueprint 생성 팩토리는 노출하지 않는다.
3. 편집 통합의 외부 소비자는 AttributeEditor의 Subscribe/Unsubscribe/PublishRequest를 사용한다. 내부 편집 버스의 쓰기 권한은 공개하지 않는다. 별도 업무의 소유자는 자신의 범용 center에서 Publish/RecordEvent를 직접 사용한다.
4. C++와 BP 구독을 지원한다. BP 경계에서는 이벤트 종류와 JSON `FString`을 사용하고, BP가 문자열을 직접 이어 붙이지 않도록 요청 생성·snapshot 해석 함수를 제공한다.
5. 구독마다 해제 handle을 반환하고 수신 객체는 weak reference로 추적한다. 위젯 재생성·구독자 파괴·EndPlay에서 정리한다.
6. 콜백 중 구독 추가·해제와 재발행을 처리한다. handle 목록을 순회하고 실제 구독을 다시 조회하며 중첩 발행은 큐로 처리한다. 범용 center는 한 dispatch에 256개 제한을 적용한다. requestId 재실행 방지와 동일 값 알림 생략은 편집 서비스의 별도 정책이다.
7. 월드와 AttributeEditor 인스턴스별로 이벤트를 격리한다. 서로 다른 PIE 월드의 ID나 구독이 섞이면 안 된다.
8. Actor 접근과 콜백 실행은 게임 스레드 전용이다. 별도 비동기 진입점은 제공하지 않으므로 호출자가 게임 스레드로 전환해야 한다.
9. 늦게 열린 UI는 AttributeEditor 구독 직후 현재 snapshot을 받는다. 기본 UI는 동기 초기 콜백 중 detach/rebind되어도 generation을 검사해 오래된 handle을 정리한다. 범용 center의 Subscribe 자체는 snapshot을 생성하거나 과거 이벤트를 재생하지 않는다.

참고한 [NotificationCenter.h](https://github.com/dndus310/ProjectT/blob/main/Source/ProjectT/System/Core/Managers/NotificationCenter.h)는 이벤트별 C++/BP observer 등록·해제·Post 구조를 제공한다. 해당 헤더의 delegate에는 payload 인자가 없으므로, JSON payload와 소유·수명 계약은 이번 요구에 맞춰 확장한다.

## 5. 이벤트와 JSON

| 종류 | 구현 이벤트명 | 의미 |
|---|---|---|
| 요청 | `TransformEditRequested` | 대상·필드·값·조작 단계를 포함한 변경 요청 |
| 요청 | `DuplicateRequested` | 선택 Actor 복제 요청 |
| 상태 | `EditorStateChanged` | 편집 활성·모드·도구·좌표계·입력 가능 상태 변경 |
| 상태 | `SelectionChanged` | 선택 교체·해제·파괴 및 새 대상 snapshot |
| 상태 | `TransformChanged` | 실제 적용 또는 관찰된 Actor Transform과 변경 단계 |
| 결과 | `ObjectDuplicated` | 원본·복제본 ID, 복제본의 최종 상태 |
| 결과 | `RequestRejected` | 요청 ID, 실패 코드, 사용자에게 표시할 이유 |

공통 필드는 `schemaVersion`, `editorId`, `requestId`, `source`다. 대상 요청에는 `objectId`, `selectionRevision`을 넣고, 상태 알림에는 증가하는 `stateRevision`을 넣는다. 요청과 관계없는 외부 변경에서는 requestId를 비워 둘 수 있다.

- 객체 ID는 이 편집 세션에서 관리하는 GUID와 weak Actor 참조로 해석한다. JSON에 메모리 주소를 넣거나 에디터 전용 Actor GUID를 전제로 하지 않는다.
- 선택 revision은 선택이 바뀔 때 증가한다. A를 조절하던 오래된 요청이 B 또는 재선택된 A에 적용되지 않도록 검사한다.
- 수치 변경 요청은 바뀐 필드만 전달한다. 한 축을 고칠 때 오래된 나머지 필드로 Actor 전체 Transform을 덮어쓰지 않는다.
- JSON 파싱·타입·필수 필드·지원 버전·대상 월드·선택 일치·편집 가능 상태·유한수 여부를 검사한다. 실패하면 Actor는 변경하지 않는다.
- 위치는 `x/y/z`, 회전은 `roll/pitch/yaw`, 크기는 `x/y/z`로 명시한다. 회전 Euler 축과 Quaternion 내부 계산을 혼용하지 않는다.
- 디테일뷰의 `space: World`는 Gizmo의 Local/World 축 모드와 별개다. 상대 Transform 지원은 명시적인 별도 기능으로 추가한다.
- 스케일은 기존 Gizmo와 같이 0·음수·비균등 값을 허용한다. UI는 임의 최소값을 강제하지 않으며 JSON 요청의 NaN/무한대는 거부한다.

변경 요청 예시:

```json
{
  "schemaVersion": 1,
  "editorId": "<editor-guid>",
  "requestId": "<request-guid>",
  "source": "DetailsView",
  "objectId": "<actor-guid>",
  "selectionRevision": 12,
  "operationId": "<interaction-guid>",
  "phase": "Update",
  "space": "World",
  "property": "Location.X",
  "value": 120.0
}
```

상태 snapshot은 `editingEnabled`, `activeMode`, `gizmoMode`, `gizmoCoordinateSystem`, 선택 ID/이름/클래스, Transform, `canEditTransform`, 비활성 사유, `isModifying`, `hasChanges`를 포함한다. 선택이 없으면 대상과 Transform을 비운다.

`isModifying`는 현재 드래그 등의 진행 상태다. `hasChanges`는 선택 Actor의 현재 Transform이 해당 Actor를 등록할 때의 기준 Transform과 다른지를 의미한다. 과거에 한 번 변경했다는 이력 플래그가 아니다.

- 기준값으로 되돌리면 false가 된다. Cancel은 조작 시작값으로 돌아간 뒤 기준 snapshot과 다시 비교한다.
- 선택 교체 시 새 대상의 기준으로 계산하고, 미선택이면 false다. 복제본은 새로 생성된 객체이므로 기준 상태로 수락하기 전까지 true다.
- 이는 디스크 저장 여부가 아니다. `MarkSelectionBaseline()`이 현재 선택 Actor의 Transform을 새 기준으로 수락한다. 임의 BP 변수 변경은 이 Transform 변경 플래그에 포함되지 않는다.

## 6. Transform 동기화와 변경 단계

처리 순서는 `요청 수신·진단 기록 → 검증 → 실제 적용 → Actor 재조회 → typed StateStore 갱신 → JSON 알림 → typed 상태를 읽어 UI 표시`다.

1. 선택 시 Actor 상태를 읽어 snapshot을 제공하고 외부 Transform 변경 감시를 연결한다.
2. Subsystem의 선택 Gizmo는 TransformProxy의 `OnBeginTransformEdit`, `OnTransformChanged`, `OnEndTransformEdit`에 연결했다.
3. 외부 코드의 SetActorTransform 등은 Subsystem Tick에서 선택 Actor 하나의 상태를 비교해 감지한다. 전체 월드를 매 프레임 검색하지 않는다. Root 교체·이동 가능 상태 변경·대상 파괴도 선택 및 Gizmo 수명 처리에 반영한다.
4. 외부 변경 후 Gizmo Proxy와 표시 위치도 갱신한다. 갱신용 API가 다시 Actor를 변경해 재귀하거나 오래된 Proxy 상태가 Actor를 되돌리지 않는지 검증한다.
5. UI 수신값 반영 중 입력 콜백은 요청을 재발행하지 않는다. 동일 값은 건너뛰고, 적용 중 발생한 중복 알림은 모아 최종 상태를 한 번 발행한다.
6. `Begin/Update/Commit/Cancel`과 operationId를 사용한다. 숫자 입력 확정은 단일 Commit으로 처리할 수 있고, 슬라이더는 Begin→여러 Update→Commit으로 처리한다. Cancel은 해당 조작의 시작값으로 복원한다.
7. 선택 교체·편집 종료·포커스 상실은 유효한 현재 조작을 정리한 뒤 상태를 전환한다. 기본 정책은 마지막 적용값을 Commit하는 것이다. 대상 파괴·월드 종료는 복원 시도 없이 중단하고 콜백을 해제한다.
8. Gizmo·UI 슬라이더 등 어떤 조작이든 활성 operation이 있으면 다른 Transform·복제 요청을 `Busy`로 거부한다. 같은 operationId의 Update/Commit/Cancel만 연속 요청으로 허용한다. 선택·모드 변경은 7번의 종료 정책을 따른다. 외부 코드가 직접 Actor를 변경했다면 진행 중 조작을 종료하고 외부의 최종 값을 재조회한다.
9. Update의 base revision을 매번 이전 snapshot과 비교해 정상 연속 드래그를 거부하지 않는다. 활성 operationId와 대상·선택 revision으로 연속 조작을 검증한다.
10. 히스토리를 후속 연결할 때는 조작 하나당 before/after 한 쌍만 기록한다. ToolsContext transaction과 별도 SaveHistory를 동시에 기록하지 않는다.

## 7. 사이드바와 입력 충돌

구현은 BP 파생 가능한 `UOWTAttributeDetailsWidget` 한 클래스다. 상단 상태 요약과 Details/Events 탭으로 구성하고 Transform 필드는 native Slate SpinBox를 쓴다. PlayerController가 BeginPlay에서 위젯을 만들고 `SetAttributeEditor()` 뒤 Viewport에 추가한다. 패널은 편집 활성 또는 모니터 열기 상태에서 보인다. F2로 편집을 꺼도 열린 Events 모니터는 유지하며, 편집 OFF에서 F3으로 모니터를 닫을 때 전체 패널을 숨긴다.

- 위치 3축, 회전 3축, 크기 3축의 수치 입력 및 드래그/슬라이더 조작, Actor 이름·클래스, 복제 버튼을 제공한다.
- 위치·회전 수치 입력을 임의의 좁은 슬라이더 범위로 제한하지 않는다. 입력 가능한 범위와 드래그 감도는 구분한다.
- 편집모드 off·미선택·파괴된 대상에서는 이전 값을 제거하고 입력을 비활성화한다.
- 루트가 없거나 Static인 대상은 기존 선택 정책에 따라 정보를 표시하되 Transform 수정 가능 여부와 사유를 표시한다. 자동으로 Movable로 바꾸지 않는다.
- `HideSelectionGizmo`는 선택 해제가 아니다. 이 명령만으로 디테일뷰를 비우지 않는다.
- 새 위젯은 초기 snapshot을 받아 복원되며 NativeDestruct에서 구독을 해제한다.
- Events는 최신순 256건의 sequence/UTC/topic/방향/source/상관 ID/실패 요약과 펼칠 수 있는 JSON을 표시한다. Pause/Resume은 이력 표시만 멈추고 typed 상태는 계속 갱신한다. Clear view는 현재 sequence까지 UI에서 숨기며 원본 journal을 삭제하지 않는다.
- 위젯은 Actor에 직접 SetTransform을 호출하지 않고 모든 수정·복제를 같은 Pub-Sub 요청으로 보낸다.

현재 Spectator는 Tick에서 LMB 상태를 직접 읽는다. 따라서 UMG 이벤트를 소비하는 것만으로 월드 입력 차단이 보장되지 않는다.

- 포인터가 사이드바에 있거나 UI가 드래그를 캡처한 동안 월드 선택·Gizmo 캡처를 차단한다.
- 수치/검색 입력에 포커스가 있을 때 W/E/R, Ctrl+D 등 편집 단축키를 차단한다. F2/F3은 focused widget의 preview 경로에서도 허용하고 반복 keydown은 소비만 하여 한 번 누를 때 한 번만 전환한다.
- 사이드바 조작과 우클릭 카메라 조작이 동시에 시작되지 않도록 한다.
- UI 열기·닫기·포커스 전환·마우스 해제 시 차단 상태와 기존 입력 캡처를 반드시 정리한다.

## 8. Runtime Duplicate

참조 경로는 UE5.7의 `UEditorActorSubsystem::DuplicateActor` → `UUnrealEdEngine::DuplicateActors` → 복사/붙여넣기 처리다. 이 경로는 Editor 모듈과 트랜잭션·팩토리 등에 의존한다. Runtime에서는 이 함수를 호출하지 않고 Actor 생성 수명 주기를 사용하는 복제 서비스를 만든다.

구현 클래스는 `OWTRuntimeDuplication` 모듈의 `UOWTRuntimeActorDuplicator`다. 월드를 제공하는 임의 소유 UObject가 exact Outer/Owner와 강한 참조를 유지하여 단독 사용할 수 있다. 편집 통합에서는 AttributeEditor가 서비스를 소유하고 Ctrl+D와 UI 복제 버튼이 같은 요청 처리 함수를 사용한다. Initialize/DuplicateActor/Deinitialize는 게임 스레드에서 호출한다.

### 복제 성공의 의미

- 원본과 같은 런타임 클래스의 새 Actor가 같은 standalone 월드·Level에 존재한다.
- 원본 Transform과 지원하는 인스턴스 속성·컴포넌트 상태를 보존한다. BP 클래스 기본값으로만 다시 생성하는 것은 성공으로 보지 않는다.
- 원본 Actor는 변경하지 않는다. 편집 통합에서 복제본은 새 객체 ID를 받는다. 루트 이름은 마지막 `_숫자`를 제거한 family 기준이며 같은 Level의 최대 번호와 helper counter보다 큰 번호를 쓴다. 복제본을 다시 복제해도 같은 family를 유지한다.
- 원본 소유 컴포넌트 및 지원하는 instanced UObject는 별도 인스턴스로 생성한다. 복제 그래프 내부 참조는 복제본으로 연결하고, 외부 Actor·공유 애셋 참조는 외부 참조로 유지한다.
- 루트는 외부 부모에서 분리하고 World Transform에 caller offset을 한 번 더한다. `ChildActorComponent`가 관리하는 재귀 child 계층은 포함하고, 임의의 외부 attached Actor 트리는 포함하지 않는다. 관리 child만 직접 복제하지 말고 부모를 복제한다.
- 기본 위치 오프셋은 월드 +X 100cm다. AttributeEditor의 `DuplicateWorldOffset` 속성으로 설정한다.
- 생성 Level은 원본 Level이며 루트 Owner/Instigator는 비운다. 관리 계층 내부 소유·참조는 복제 객체로 재매핑한다. `AlwaysSpawn`으로 충돌에 따른 위치 자동 이동 없이 생성한다.
- 스폰 Transform의 scale과 template root scale이 중복 곱해지지 않도록 TransformScaleMethod를 명시하고 비균등 scale을 검증한다.
- 성공 후 기존 선택 관리 경로를 통해 복제본을 선택하고 `ObjectDuplicated`, 선택 snapshot을 발행한다. 실패 시 기존 선택을 유지한다.

### 구현한 생성 및 복원 경로

원본 Actor를 Spawn Template로 넘기거나 `DuplicateObject<AActor>`로 통째로 복사하지 않는다. 클래스 기본값에서 deferred SpawnActor를 수행하고, 따로 캡처한 지원 속성·컴포넌트·instanced UObject·관리 child 상태를 복원한다. `UOWTDuplicationRestoreComponent`와 계층 복원 순서가 Construction 이후 루트/child BeginPlay 이전 복원 시점을 제공한다.

복제 검증 fixture는 다음 보존 조건을 확인하도록 구성한다. 실행 결과는 12절에서 별도로 관리한다.

| Fixture | 확인할 내용 |
|---|---|
| C++ Actor | 생성 후 변경된 UPROPERTY, Transform, 기본 컴포넌트 상태 |
| BP Actor | BP 변수의 인스턴스 값과 SCS 컴포넌트 속성 |
| Construction Script BP | 생성 스크립트가 다시 설정하는 값의 복원 시점 |
| 지원 대상 instanced UObject | 원본과 포인터 분리 및 내부 참조 재연결 |
| 참조 보유 Actor | 원본 self/컴포넌트 참조와 외부 Actor/애셋 참조의 구별 |
| 이름 family | 기존 숫자 suffix·충돌·삭제 후 증가·복제본 재복제 |
| ChildActorComponent 계층 | 재귀 child 구성, 내부 교차 참조, BeginPlay 관찰값과 실패 시 계층 정리 |
| Native participant | reflected 범위 밖 class별 설정 복원, 내부 참조 map, 실패 반환 정리 |
| 재질·물리 adapter | 소유 MID parameter/physical-material override, 일반 StaticMesh 단일 body 설정·속도·awake 상태 |

구현 순서는 `지원 계층·속성 사전 검사/캡처 → deferred SpawnActor → 생성 전 지원 상태 적용 → FinishSpawning → Construction 이후 계층 복원 → 컴포넌트 등록·그래프 참조 재연결 → adapter/native participant 복원 → 계층 BeginPlay 허용 → 보조 객체 정리`다. 원본 소유 컴포넌트끼리의 attachment는 유지하고 외부 부모 attachment는 분리한다.

- BeginPlay 이전에 보존돼야 할 값과 이후에 복원 가능한 값을 구분한다. FinishSpawning 뒤 무조건 모든 값을 덮어쓰는 방법을 일반해로 사용하지 않는다.
- Timer/delegate/latent 실행, transient·duplicate-transient·editor-only 속성, 네트워크 연결과 임의 native 실행 상태는 자동 snapshot 대상이 아니다. nested reflected 필드에도 해당 제외 규칙을 적용한다.
- 지원 재질 adapter는 소유된 기본 UMaterialInstanceDynamic의 일반 parameter 값과 PhysMaterial/PhysicalMaterialMap override를 복원한다. unwelded 비-instanced StaticMesh 단일 body의 지원 물리 설정·속도·awake 상태는 별도 adapter로 복원한다. 이는 전체 물리 세계나 임의 GPU/native handle 복제가 아니다.
- `IOWTRuntimeDuplicationParticipant::RestoreRuntimeDuplicateState(SourceObject, DuplicatedObjects, OutError)`는 destination Actor/component/owned UObject에 대한 BlueprintNativeEvent다. 전체 그래프 재매핑 뒤 계층 BeginPlay 전에 class별 native 설정을 복원한다. 내부 참조는 map으로 치환하고 자원은 새로 생성한다. false이면 새 루트와 관리 child를 정리한다.
- 지원하지 않는 유형은 복제 전에 거부하거나 실패 사유를 돌려준다. 불완전한 결과를 성공으로 알리지 않는다.
- 중간 실패 시 새로 생성한 객체를 정리하고 성공 이벤트를 보내지 않는다. Construction/일부 native 초기화/child PostInitializeComponents는 복원 hook보다 먼저 실행될 수 있으며 그 외부 부작용을 자동 되돌리지 않는다. 복원 뒤 BeginPlay가 값을 다시 바꾸는 동작도 해당 클래스의 정책이다.
- standalone 이외 네트워크 월드, AInfo/Controller/Brush, CDO·archetype, 파괴 중이거나 다른 월드의 Actor는 거부한다. Pawn은 미점유 상태이며 자동 player/AI possession이 모두 Disabled일 때 지원한다. root 없는 Actor와 ChildActorComponent 계층을 지원한다. 편집 관리 Actor와 Gizmo 금지는 AttributeEditor의 정책이다.
- simulated skeletal/instanced/welded body와 UPhysicsConstraintComponent는 거부한다. custom MID subclass, parameter collection/UserSceneTexture/Nanite override/layered sparse-volume 상태에는 전용 adapter가 필요하다.
- opaque FInstancedStruct/FInstancedPropertyBag 또는 raw FBodyInstance/FConstraintInstance/FTickFunction이 포함된 reflected 속성은 숨은 참조/live handle을 일반 복사하지 않도록 사전 거부한다. 일반 primitive의 BodyInstance 설정은 안전한 adapter 대상이다. participant hook은 사전 거부 정책을 우회하지 않으며 거부된 타입에는 모듈 내부의 전용 adapter가 필요하다.
- 원본이 참조하는 소유 객체가 지원하는 instanced UObject가 아니거나 컴포넌트 타입·그래프를 안전하게 복원할 수 없으면 복제를 거부한다. 부분 성공으로 처리하지 않고 `RequestRejected`의 `DuplicateFailed` 사유로 알린다. 전체 지원 범위는 사용법 문서의 복제 제한을 참고한다.

로컬 엔진에서 확인한 근거는 `Editor/UnrealEd`의 복제 경로, `Runtime/Engine/Private/LevelActor.cpp`의 생성·레벨 등록, `Actor.cpp`의 ExecuteConstruction/PostActorConstruction/BeginPlay, `SCS_Node.cpp`의 BP 컴포넌트 생성이다. Runtime 코드는 UE5.7 설치 소스를 기준으로 구성했다.

## 9. 코드 작성 규칙

기존 `Docs/CppStyle.md`를 기본으로 아래 사용자 요구를 적용한다.

- 서로 다른 실패 사유를 긴 `&&`·`||` 한 줄로 묶지 않는다. 조건별 if와 early return으로 풀어 쓴다.
- 단순 조건도 무조건 함수를 만들지는 않는다. 의미 있는 도메인 검사만 이름 있는 함수로 분리한다.
- if/for/while에 중괄호를 사용하고 return/continue/break는 별도 줄에 쓴다.
- 공개 진입점에서 입력·대상·수명을 확인한다. 같은 동기 호출에서 이미 보장한 조건을 내부 함수마다 반복하지 않는다.
- 소유 관계, 게임 스레드, 내부 자료구조 불변 조건에는 `check`/`checkf`를 사용한다.
- 잘못된 JSON, 선택 없음, 대상 파괴, 지원하지 않는 복제와 스폰 실패는 처리 가능한 실패로 반환한다. 사용자 입력 오류로 assert를 발생시키지 않는다.
- 복구 가능한 내부 설정 오류는 `ensureMsgf` 후 실패를 반환할 수 있다.
- `check` 안에서 생성·구독·상태 변경 등 필수 부작용을 실행하지 않는다. Shipping에서 assert가 비활성화되어도 필수 검증과 동작이 남아야 한다.
- 함수 하나는 한 역할을 맡고 상위 함수는 검증·조회·적용·통지 순서를 드러낸다. 불필요한 래퍼와 반복 주석은 추가하지 않는다.
- 이 규칙은 모든 신규 코드와 실제 수정하는 코드에 적용한다. 무관한 기존 파일 전체를 포맷 변경하지 않는다.

조건 분리 예시:

```cpp
if (!IsValid(Target))
{
    return RejectRequest(RequestId, EOWTEditError::InvalidTarget);
}

if (Target->GetWorld() != GetWorld())
{
    return RejectRequest(RequestId, EOWTEditError::DifferentWorld);
}

if (!Subsystem->IsEditingEnabled())
{
    return RejectRequest(RequestId, EOWTEditError::EditingDisabled);
}
```

위 예시는 흐름을 설명하는 의사 코드이며 해당 API가 현재 구현돼 있다는 뜻은 아니다. Assert 동작은 [Epic Asserts 문서](https://dev.epicgames.com/documentation/unreal-engine/asserts-in-unreal-engine?lang=en-US)를 참고한다.

## 10. 가용 수와 담당 작업

현재 세션의 동시 실행 한도는 루트 포함 4개다. 권장 구성은 **통합 담당 1 + 하위 에이전트 3 = 총 4개**다. 이는 이 세션의 한도이며 Codex 전체의 공통 한도는 아니다.

계산 근거는 계약 확정 뒤 독립적으로 진행할 수 있는 구현 묶음이 코어, UI, 복제의 3개이고, 공유 파일 연결과 통합 검증은 루트가 맡기 때문이다. 기능의 개수만큼 에이전트를 늘릴 필요가 없다.

| 담당 | 소유 파일/영역 | 산출물 |
|---|---|---|
| 루트: 설계·통합 | AttributeEditor, StateStore, 플러그인/모듈 구성, 머티리얼, 요구 명세 | 상태 분리, 통합, 빌드·패키징·이식 검증 |
| A: Pub-Sub | OWTEventCore, Spectator·GameMode 입력 fallback | 범용 소유 버스와 기록, 독립 수명 검증, 샘플 BP 없는 입력 |
| B: Runtime 복제 | 신규 `Duplication/OWTRuntimeActorDuplicator.*`, 복제 전용 fixture·검증 파일 | 엔진 경로 분석, 보존 범위, 복제·참조·실패 정리 |
| C: 모니터 | `UI/OWTAttributeDetailsWidget.*`, PlayerController, UI/상태 테스트, 별도 소비 프로젝트 fixture, 사용법 | 상태 요약·이벤트 기록 UI, F3 입력, 소비자 문서 |

재사용 C++ 파일은 `Plugins/OWTRuntimeEditing/Source/{모듈}/Public` 및 `Private` 아래에 역할별로 배치했다. 모듈별 Runtime 테스트는 `Private/Tests`에, 프로젝트 BP 자산을 요구하는 테스트는 `Source/simple_proj/Private/Tests`에 둔다. Editor 전용 fixture와 커맨드렛은 `Source/VTBOWTEditorAutomation/Private`에 유지한다.

공유 헤더와 public API는 0단계에서 루트가 확정한다. 다른 담당의 파일 변경이 필요하면 필요한 API와 변경안을 루트에 전달한다. Build.cs와 기존 대형 파일은 여러 에이전트가 동시에 수정하지 않는다.

현재 Git 탐색 결과 저장소 루트는 프로젝트 폴더가 아닌 `C:/Users/jkyii`다. 이 상태에서 자동으로 worktree를 생성하지 않는다. 현재 분담은 공유 작업 폴더에서 파일 소유권을 지키는 방식이며, 독립 worktree를 사용할 때는 먼저 프로젝트의 의도된 저장소 경계를 확인해야 한다.

## 11. 실행 순서와 에이전트 지시문

1. 루트가 세 Runtime 모듈의 public API, typed 상태 소유권, 이벤트 기록 형식, 모니터 facade를 확정한다.
2. 이벤트 담당은 범용 Pub-Sub과 native 입력 fallback, 복제 담당은 그래프·이름·확장 훅, UI 담당은 상태 요약·이벤트 모니터를 병렬 구현한다. 담당 밖의 파일은 조율 후 변경한다.
3. 루트는 Actor 재관찰 → StateStore 갱신 → JSON 알림 흐름을 연결하고 plugin content/config를 포함해 통합한다.
4. 빌드는 같은 출력 디렉터리에서 중복 실행하지 않는다. Editor 테스트, 고급 복제 커맨드렛, 기존 입력·Gizmo 회귀를 수행한다.
5. 샘플 콘텐츠 없이 플러그인만 복사한 별도 프로젝트에서 공개 API 소비와 native 편집기 테스트를 수행한다. 게임 cook·stage 후 packaged 테스트 및 화면 확인을 별도로 기록한다.

## 12. 검증 및 완료 기준

| 영역 | 필수 검증 |
|---|---|
| 소유·구독 | 소유자 미초기화 요청 거부, 구독자 파괴, 구독 중 해제/재발행, 위젯 재생성 후 중복 수신 없음 |
| 초기 상태 | Play 직후 편집 off, 선택 직후 snapshot 일치, 늦은 구독에서도 현재 상태 수신 |
| 선택 수명 | 선택 교체·해제·대상 파괴·F2 off·월드 종료가 UI와 Gizmo에 동일하게 반영 |
| Transform | UI 수치/슬라이더→Actor/Gizmo, Gizmo→UI, 외부 SetTransform→UI/Gizmo의 세 경로 일치 |
| 입력 검증 | 잘못된 JSON·타입·ID·revision·NaN/Inf·다른 월드 요청 거부 및 Actor 미변경 |
| 조작 경계 | 연속 Update가 정상 처리되고 Commit/Cancel 후 isModifying 해제, 오래된 선택 요청 거부 |
| 변경 상태 | 기준값 복원·Cancel·선택 교체·새 복제본·기준 재설정에서 hasChanges의 정의 유지 |
| 편집 제약 | Static/루트 없음/지원하지 않는 대상의 사유 표시, Gizmo 캡처와 UI 요청 동시 적용 없음 |
| UI 입력 | 슬라이더/텍스트 포커스 중 월드 선택·Gizmo·W/E/R·Ctrl+D·카메라 입력 충돌 없음 |
| 복제 | C++/BP 인스턴스 값과 지원 컴포넌트 보존, 원본 독립, 내부/외부 참조 정책, 복제본 선택 |
| 복제 실패 | 지원 불가 사유와 중간 생성물 정리, 원본·기존 선택 보존, 성공 이벤트 미발행 |
| 회귀 | 기존 선택·스냅·Base/Custom Gizmo·카메라·Shutdown 검증 유지 |
| Runtime | 게임 Target 빌드 및 cooked/packaged 실행에서 동작, Runtime UnrealEd 의존성 없음 |

기존 검증은 `CreateOWTInput` 및 `CreateOWTLevel` 커맨드렛의 `-ValidateOnly` 경로를 활용한다. 이 검증은 BP 바인딩·Gizmo 계산·상태 전달을 검사하지만 실제 UI 픽셀 배치와 마우스 포커스를 증명하지 않으므로 PIE/Standalone에서 UI 조작을 따로 확인한다.

플러그인 Runtime 테스트는 `OWT.EventCore.OwnerIsolation`, `OWT.EventCore.CallbackMutation`, `OWT.EventCore.BoundedHistory`, `OWT.Runtime.AttributeContracts`, `OWT.Runtime.AttributeDetailsContracts`, `OWT.Runtime.ObservedState`, `OWT.Runtime.NativeInput`이다. DetailsContracts는 초기 외부 변경 뒤의 드래그, 편집 OFF 모니터, pause/filter/clear, F2/F3 repeat와 동기 구독 중 detach/rebind를 검사한다. ObservedState는 typed 상태와 JSON의 분리, NativeInput은 샘플 BP 없는 기본 입력을 검사한다.

프로젝트 모듈의 `OWT.Runtime.Attributes`는 `/Game/VTBOWT` 샘플 BP를 요구한다. 고급 Native/BP 복제 fixture는 Editor 커맨드렛 `ValidateOWTAttributes`에 있으며, 별도 소비 프로젝트 fixture의 공개 모듈 API 검사는 `OWT.Portability.IndependentHost`다. Editor 빌드 통과만으로 Runtime·패키징·이식 지원 완료로 보고하지 않는다.

### v2 최종 실행 결과 (2026-10-07, UE5.7)

| 검사 | 결과 | 근거 |
|---|---|---|
| 최종 Editor Win64 Development 빌드 | PASS | `Saved/Logs/V2_InputPipelineBuild.log` |
| Editor 자동 테스트 | 8/8 Success, warning/error 0 | `Saved/Automation/V2_InputPipeline/index.json`, `Saved/Logs/V2_InputPipelineTests.log` |
| 고급 Native/BP 복제 fixture | PASS, warning/error 0 | `Saved/Logs/V2_DuplicationFinal.log`; 이름, ChildActor 그래프, 내부 참조, BeginPlay 이전 복원, native 훅과 실패 정리, MID/물리 상태 |
| 기존 입력·Gizmo / 샘플 맵 회귀 | 각각 PASS, warning/error 0 | `Saved/Logs/V2_InputRegression.log`, `Saved/Logs/V2_LevelRegression.log` |
| 독립 소비 프로젝트 Editor 빌드 | PASS | `Saved/Logs/V2_ProbeFinalBuild.log`; `Saved/PortabilityProbe`에는 원래 `/Game/VTBOWT` 콘텐츠가 없음 |
| 독립 소비 프로젝트 자동 테스트 | 8/8 Success, warning/error 0 | `Saved/Automation/V2_ProbeFinal/index.json`; AttributeEditor 없이 일반 Actor 소유 이벤트/복제 API도 검증 |
| Windows Game 빌드·cook·stage | PASS | `Saved/Logs/V2_FinalPackage.log` |
| 최종 패키징 게임 자동 테스트 | 8/8 Success, warning/error 0 | `Saved/Automation/V2_PackagedFinal/index.json`, `Saved/Logs/V2_PackagedFinalTests.log` |
| 실제 패키징 화면 | 선택, BP `_0 → _1 → _2` 연속 복제, 요청/결과 JSON 펼침, 편집 OFF 모니터 유지 확인 | `Saved/Logs/V2_UI.log`; 최종 빌드 F3 재확인 및 viewmode 부작용 없음은 `Saved/Logs/V2_FinalUI.log`; 화면 확인과 자동 테스트는 서로 다른 검증 |
| 플러그인 배포 ZIP | 66개 파일의 원본·소비 프로젝트·배포본 SHA-256 일치 | `Saved/Distribution/OWTRuntimeEditing_UE5.7.zip`; Source/Config/Content/descriptor/README 포함, Binaries/Intermediate 제외 |

`OWT.Runtime.NativeInput`은 엔진의 mapping rebuild, InputKey, ProcessInputStack을 통과해 F2 누름·유지·해제·재누름을 검증한다. 한 프레임 안의 즉시 누름/해제는 UE5.7 Enhanced Input 키보드 Pressed trigger가 발생하지 않는 것을 재현했다. UI 자동화 도구는 키를 누른 채 유지하는 API가 없어 이 도구의 순간 입력으로 월드 단축키 전체를 검증했다고 주장하지 않는다. F2/F3의 엔진 기본 viewmode debug binding 충돌은 해당 Controller의 PlayerInput에서만 비활성화했다. 실제 마우스 드래그 검증 제한은 아래 v1 기록과 동일하며 드래그 조작 경계는 자동 테스트로 검증했다.

### 기존 v1 실행 결과 기록 (이번 확장 이전)

아래 표는 v1 당시 코드와 환경의 결과를 보존한 것이다. v2의 새 모듈·모니터·typed 저장소·확장 복제·별도 소비 프로젝트 검증 결과로 재사용하지 않는다.

| 검사 | 최종 결과 | 근거 로그 / 제한 |
|---|---|---|
| Editor Win64 Development 빌드, UE5.7 | PASS | `Saved/Logs/AttributeEditor_Build.log`, 최종 `AttributeEditor_FinalPackage.log` |
| Editor 자동 테스트: Attributes / AttributeContracts / AttributeDetailsContracts | 3/3 Success, warning/error 0 | `Saved/Automation/Attributes/index.json`, `Saved/Logs/AttributeEditor_FinalEditor.log` |
| ValidateOWTAttributes | PASS, warning/error 0 | `Saved/Logs/AttributeEditor_Duplication.log`; 고급 Native/BP 복제 fixture |
| CreateOWTInput -ValidateOnly | PASS | `Saved/Logs/AttributeEditor_InputRegression.log`; 기존 입력·Gizmo 회귀 |
| CreateOWTLevel -ValidateOnly | PASS | `Saved/Logs/AttributeEditor_LevelRegression.log`; 기존 샘플 맵 검증 |
| Game Win64 Development 빌드·cook·stage | PASS | `Saved/Logs/AttributeEditor_Package.log`; 마지막 UI 수정 후 `AttributeEditor_FinalPackage.log`로 다시 빌드·stage |
| 패키징된 게임 자동 테스트 3종 | 3/3 Success, warning/error 0 | `Saved/Automation/AttributesPackaged/index.json`, 플랫폼 Windows; `Saved/Logs/AttributeEditor_Packaged.log` |
| Standalone 실제 화면 | 선택, X 수치 입력, Actor/Gizmo 이동, 복제 버튼, 텍스트 포커스 중 F2 종료 확인 | `Saved/Logs/AttributeEditor_UI.log`; 드래그 조작은 UI 자동화의 이동 이벤트로 값 변경이 재현되지 않아 실마우스 검증은 별도로 남음 |

## 13. 소스 근거

- 프로젝트: `simple_proj.uproject` — EngineAssociation 5.7, OWTRuntimeEditing 플러그인 활성화 및 프로젝트/Editor 자동화 모듈.
- `Plugins/OWTRuntimeEditing/OWTRuntimeEditing.uplugin` — OWTEventCore/OWTRuntimeDuplication/VTBOWTEditor Runtime 모듈, EnhancedInput 의존성, plugin content.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/VTBAttributeEditor.cpp` — 소유 Pub-Sub, JSON 계약, snapshot, Transform 및 복제 요청 처리.
- `Plugins/OWTRuntimeEditing/Source/OWTEventCore/Private/Events/OWTNotificationCenter.cpp` — 구독 수명과 이벤트 전달.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/State/OWTAttributeStateStore.cpp` — 실제 Actor 관찰값, GUID/weak 참조, baseline, 읽기 복사본.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/UI/OWTAttributeDetailsWidget.cpp` — 런타임 사이드바와 UI 조작 단계.
- `Plugins/OWTRuntimeEditing/Source/OWTRuntimeDuplication/Private/Duplication/OWTRuntimeActorDuplicator.cpp` — 상태 캡처와 Runtime 복제.
- `Plugins/OWTRuntimeEditing/Source/OWTRuntimeDuplication/Public/Duplication/OWTRuntimeDuplicationParticipant.h` — class별 native 설정 복원 확장점.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/VTBOWTEditorGameMode.cpp` — AttributeEditor 생성·접근.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/VTBOWTEditorSubsystem.cpp` — 선택·편집 활성·Gizmo 수명.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Modes/VTBOWTObjectEditMode.cpp` — typed 명령과 Runtime DuplicateSelection 연결.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Gizmos/Transform/VTBOWTBaseTransformGizmo.cpp` — 선택 컴포넌트와 Proxy 연결.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/Context/VTBOWTEditorToolsContext.cpp` — 런타임 transaction 골격.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/Private/VTBOWTSpectator.cpp` — 포인터 상태 직접 전달과 카메라 입력.
- `Plugins/OWTRuntimeEditing/Source/VTBOWTEditor/VTBOWTEditorPlayerController.cpp` — 위젯 수명, EnhancedPlayerInput 기본값, F3 모니터 입력.
- `Plugins/OWTRuntimeEditing/Config/DefaultOWTRuntimeEditing.ini`, `Content/`, `README.md` — script redirect, 런타임 재질, 독립 설치 안내.
- `Saved/PortabilityProbe/` — plugin-only 별도 소비 프로젝트 fixture. 프로젝트 BP 의존 검사는 이 fixture에 포함하지 않는다.
- `Docs/CppStyle.md`, `Docs/OWTInput.md` — 기존 스타일·입력·검증 규약.
- [사용자 지정 NotificationCenter 참고](https://github.com/dndus310/ProjectT/blob/main/Source/ProjectT/System/Core/Managers/NotificationCenter.h).
- [Epic DuplicateActor API](https://dev.epicgames.com/documentation/unreal-engine/API/Editor/UnrealEd/Subsystems/UEditorActorSubsystem/DuplicateActor/1?application_version=5.5) — Editor API 참고. 실제 설계 검토는 설치된 UE5.7 소스를 기준으로 했다.
- [Epic FActorSpawnParameters](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/FActorSpawnParameters) — Template, OverrideLevel, 충돌 및 TransformScaleMethod 옵션.

## 14. 가독성 정리 기록 (2026-10-08)

이번 변경은 기존 Runtime 동작을 보존하면서 소스 28개 파일(추가 6, 수정 22)의 책임과 읽는 순서를 정리한 작업이다. v4 기능이나 Blueprint CustomGizmo 기능의 신규 구현으로 분류하지 않는다.

참고한 `vit-app` 소스는 [variables.pool.h](D:/Repo-W/CL_PROJ/vit-app/src/vsfx/model/variables.pool.h), [module.manager.h](D:/Repo-W/CL_PROJ/vit-app/src/vsfx/module/module.manager.h), [module.manager.cpp](D:/Repo-W/CL_PROJ/vit-app/src/vsfx/module/module.manager.cpp), [notification.center.h](D:/Repo-W/CL_PROJ/vit-app/src/vsfx/comm/notification.center.h), [page.controller.h](D:/Repo-W/CL_PROJ/vit-app/src/vsfx/ui/page.controller.h)다. 작은 클래스 책임, 함수·데이터 영역의 구분, 상위 함수가 처리 순서를 조율하는 구성을 적용했다. Unreal의 타입·표기법·컨테이너와 reflection 매크로는 유지하며 C 스타일이나 `std::` 사용으로 전환하지 않았다.

| 영역 | 정리한 내용 | 보존한 계약 |
|---|---|---|
| AttributeEditor | JSON 변환·요청 해석을 `Private/AttributeEditor/OWTAttributeJson`으로 분리. 요청 context, 대상 해석, Transform operation 검증을 각각 구분 | 공개 UFUNCTION, JSON 필드·오류·이벤트, callback 전후 수명·선택 검증 |
| Duplicate | 계층 수집을 `FAuthoredActorHierarchy`로 분리. `FDuplicateOperation` 선언과 구현, adapter commit 단계를 구분 | CAC 우선 순회, 참조 매핑, 실패 rollback, cancel·commit 알림 순서 |
| PCG adapter | 긴 class 본문을 선언과 구현으로 구분. 설정 복원·관찰 연결·생성 정책·생성 요청 단계에 이름 부여 | 모든 component 매핑 후 참조 복원, 관찰 연결 후 생성, 원본 자원·세대 수명 |
| Mode / Tool | Context 초기화, Gizmo·설정 Tool·Extension 등록, 선택 Behavior 초기화를 구분 | 기존 진입·종료 순서, Extension 재진입 검사, selection/capture 동작 |
| Sidebar | 헤더·탭·live operation·monitor controls 구성 및 Tool 변경 감지, JSON 파싱·검색을 분리 | Slate 구성·LOCTEXT, focus·slider·Duplicate callback 순서. 기존 사용자 Slate 포맷 유지 |
| Gizmo | 위치·회전 Gizmo와 전용 Builder를 각 header/cpp로 분리. Behavior는 입력 설정만 담음 | UClass 이름 및 입력 정책 유지, 기존 Behavior include의 forwarding 호환 |
| Subsystem / 공통 | 기본 Mode 생성과 Context handler 등록 분리, 반복되는 활성 Tool 조회 정리, 선택 조회 guard 명시 | UPROPERTY 저장 순서, 설정·BP 참조 및 공통 수명 |

대표적으로 `VTBAttributeEditor.cpp`는 1,768줄에서 1,318줄로 줄었고, `ProcessTransformRequest`는 151줄에서 65줄, `ProcessDuplicateRequest`는 125줄에서 50줄로 정리됐다. 전체 코드 줄 수 자체를 줄이는 목표는 아니다. 이동한 JSON 코드와 helper가 추가되며, 상위 진입점에서 처리 순서를 읽을 수 있게 하는 것이 기준이다.

작업 전 원본은 `Saved/Backups/BeforeReadability_20261008_011448`에 보존했다. 변경 파일 목록은 `Saved/ReadabilityChangedFiles.json`, 공백을 제외한 검토 diff는 `Saved/Verification/Readability.diff`에 있다. 스타일 기준은 [CppStyle.md](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Docs/CppStyle.md)에 반영했다.

정적 검토에서 공개 reflection 이름·멤버 저장 순서, JSON 문자열과 오류 우선순위, 재진입·cleanup 순서를 대조했다. 테스트 내용은 바꾸지 않았다. 검증 실행 결과는 아래에 기록한다.

| 검사 | 결과 | 근거 |
|---|---|---|
| Editor Win64 Development | PASS | `Saved/Logs/Readability_EditorBuild.log` |
| 기존 Runtime + EventCore 회귀 | 22/22 PASS, warning/failure/not-run 0 | `Saved/Automation/Readability_Regression/index.json` |
| Playground Smoke / Navigation / Stress | 3/3 PASS, warning/failure/not-run 0. Smoke 18회·Stress 288회 복제 검증 | `Saved/Automation/Readability_SmokeNavigation/index.json`, `Saved/Automation/Readability_Stress/index.json` |
| 기존 입력·기본/Custom Gizmo 검증 | PASS, warning/error 0. 회전 focus·원복, snapping, 입력·수명 검증 | `Saved/Logs/Readability_GizmoValidation.log` |
| Game Win64 Development | PASS | `Saved/Logs/Readability_GameBuild.log` |

자동화 25개는 새 Editor DLL의 `-game -RenderOffscreen` 환경에서 실제 RHI로 실행했다. Stress의 편집 Actor 수는 600개로 유지됐으며 종료 시 임시 Actor와 GC 후 추적 복제 Actor/component는 0이었다. 기본·Custom Gizmo의 추가 입력 검사는 `CreateOWTInput -ValidateOnly -nullrhi`에서 수행했다. Details·Events 화면은 별도 실제 RHI 캡처로 확인했으며 [전시 화면](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Readability/Showroom.png), [Details](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Readability/Details.png), [Monitor](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Readability/Monitor.png)에 보존한다. 기존 패키지 실행 등 동시 작업이 있었으므로 이번 프레임 시간은 이전 단독 패키지 성능과 비교하지 않는다.

이번 가독성 작업의 결과는 위 새 빌드와 검사에서 판단한다. 기존 실행 중인 Playground 패키지와 배포 ZIP은 교체하지 않았으며, 해당 검증 기록은 그때의 빌드 기록으로 보존한다. 현재 프로젝트의 Editor DLL과 Game 실행 파일은 정리한 소스로 빌드됐다.

## 15. 범용 Runtime 상태 모니터 (2026-10-08)

모니터는 `Plugins/OWTStateMonitor`의 독립 Runtime 모듈로 구성한다. 이 모듈은 AttributeEditor·ITF·PCG·이벤트 전송 모듈을 참조하지 않으며, 소비 시스템이 소유하는 `FOWTStateMonitorModel`에 관찰용 구조체 값을 제출한다. `UScriptStruct`와 해당 타입의 구조체 주소를 함께 전달하고 호출 중 reflected 필드를 복사한다. 일반 C++ 구조체에는 자동 필드 열거 계약이 없으므로 `USTRUCT`·`UPROPERTY`를 입력 조건으로 사용한다.

기본 표시 방식은 필드·타입·값을 비교하기 쉬운 가상화 트리뷰다. 같은 값의 JSON 표현과 복사 기능, source 선택·필드 검색, 변경 전후 값의 이력도 제공한다. 변경 필드는 색상과 기호로 구분하고 이전 값을 tooltip에 표시한다. 이력은 전체 과거 Actor를 유지하는 Undo 스택이 아니라 관찰된 필드 변화의 기록이다. 이력을 일시정지하거나 과거 기록을 선택해도 Current 관찰은 계속되며, 펼침·선택·스크롤 상태를 보존한다.

| 계약 | 구현 기준 |
|---|---|
| 입력 | 중첩 USTRUCT, 고정 배열·Array·Map·Set, 숫자·bool·enum·문자열·이름·텍스트·객체 참조 |
| 원본 수명 | 원본 구조체 주소를 보관하지 않음. UObject는 경로/null로 표시하며 내부 재귀 탐색·강한 참조 보관 없음 |
| Current | 동일 Source ID의 최신 관찰값을 교체. 동일 값 반복 제출은 이력 증가 없음 |
| 변경 | 안정적인 경로로 Added·Modified·Removed 비교. Map/Set 정렬. 배열은 인덱스 기반 |
| 제한 | source·노드·깊이·문자·키 검사·컨테이너 순회·이력 수와 이력 전체 문자 예산. 잘린 상태를 표시하고 거짓 추가/삭제 추론 억제 |
| History | 필드 경로·이전 값·새 값·sequence 기록. 일시정지는 이력 표시만 동결 |
| 초기화 | 변경 강조 확인 및 이력 삭제는 모니터 데이터에만 적용. 원본 상태와 이벤트 저널 유지 |
| C++ 소비 | `SOWTStateMonitor`의 Model 인자로 공유 모델을 주입 |
| Blueprint 소비 | `UOWTStateMonitorWidget`의 구조체 wildcard Submit Snapshot. UMG 재생성 시 모델 유지 |
| 엔진 경계 | Runtime 모듈만 사용. UnrealEd·프로젝트 콘텐츠 의존성 없음 |

프로젝트 연결은 `UOWTAttributeDetailsWidget`의 작은 adapter에 둔다. `FOWTAttributeMonitorSnapshot`의 Selection·Mode·Tools·DuplicationOperations·ProceduralComponents는 실제 typed 저장소에서 읽는다. 별도 `FOWTAttributeMonitorEvents`의 Journal에는 기존 JSON 이벤트 문자열을 전달한다. Events를 역해석해 Current를 만들지 않는다. 표시 중 약 10Hz로 구조체를 제출하고 Events는 이미 유한 저널이므로 모니터 변경 이력을 중복 저장하지 않는다. F3 진입점과 기존 UI 입력 차단 계약을 유지한다.

설치·C++·Blueprint 사용 예시는 [OWTStateMonitor README](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTStateMonitor/README.md)에 기록한다. 모니터의 기본 설정과 이력은 메모리에 있다. Git 설정을 변경하거나 로그 파일을 자동 저장하는 기능은 없다.

## 16. 복제 정책·Provider 확장 (2026-10-08)

`UOWTRuntimeActorDuplicator` 생성자에서 PCG를 직접 등록하던 연결을 제거한다. 모듈이 `IOWTDuplicationAdapterProvider`를 Modular Feature에 등록하고, 각 소유 Duplicator가 Initialize에서 Provider를 탐색하여 자신의 `FOWTDuplicationAdapterRegistry`에 factory와 필드 정책을 등록한다. Provider ID 순서로 설치하며 세션 중에는 owner registry를 사용한다. 복제 시작 시 Registry 전체를 값으로 복사하므로 factory나 검증 callback이 owner의 등록을 바꿔도 진행 중인 작업에는 이미 확보한 factory·필드 정책을 사용한다.

`FOWTDuplicationPropertyPolicySet`은 클래스별 정책을 등록하고 가장 구체적인 클래스부터 검사한다. 판단이 unset이면 상위 클래스 정책으로 이어진다. 공통 transient·delegate 제외는 복제 코어가 먼저 적용한다. 기본 Actor·Component·Scene·MID의 엔진 수명 규칙은 별도 Engine Provider에 둔다. MID renderer 자원과 Tick·BodyInstance·Transform cache는 기존 전용 엔진 API로 복원하며 일반 필드 복사 범위에 넣지 않는다. 엔진 버전 계약으로 남는 필드 허용 목록은 이 Provider에서 관리한다.

PCG authoring 값은 `OWTPCGComponentConfiguration`의 공통 reflection 전송으로 캡처·복원한다. `UPCGComponent` 선언 범위의 CPF_Edit 필드를 읽고 CPF_EditConst·transient·editor-only·delegate 등은 제외한다. VisibleAnywhere도 CPF_Edit를 가질 수 있으므로 CPF_EditConst를 제외하는 것이 필수다. 미래에 편집 가능한 PCG 필드가 추가되어도 필드별 대입을 추가하지 않는다. Graph instance·parameter bag·SchedulingPolicy·ToolData·활성화 억제·생성 요청·partition 등록은 PCG 전용 수명 처리에 유지한다. PCG 파생 클래스의 사용자 필드는 기존 공통 복제 경로가 맡는다.

Provider의 factory·규칙은 Provider 객체 주소에 의존하지 않는 함수여야 하며 소비 세션과 진행 중인 작업 동안 해당 모듈이 로드된 상태여야 한다. Modular Feature unregister는 새 세션의 탐색을 막는 동작이다. 이미 설치된 세션의 정책을 철회하거나 실행 중인 모듈을 강제로 unload하는 계약은 아니다.

## 17. 형상관리 제외 방법

Git에서 개인 PC에만 적용하는 제외 규칙은 `.git/info/exclude`, 팀과 공유할 생성 파일 규칙은 `.gitignore`에 둔다. ignore는 아직 추적하지 않는 파일에 적용된다. 이미 커밋한 파일은 ignore 규칙만으로 추적이 해제되지 않는다. [Git ignore 공식 문서](https://git-scm.com/docs/gitignore)

현재 `git rev-parse --show-toplevel` 결과는 프로젝트 폴더가 아니라 `C:/Users/jkyii`다. 따라서 현재 저장소의 `.git/info/exclude`는 `C:/Users/jkyii/.git/info/exclude`이며 아래 규칙은 저장소 루트 기준이다.

```gitignore
# 모니터 플러그인 전체를 개인 PC에서 제외할 경우
/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Plugins/OWTStateMonitor/

# 해당 프로젝트의 실행 기록·검증 산출물만 제외할 경우
/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/
/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Intermediate/
/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Binaries/
```

프로젝트 폴더 자체를 별도 Git 저장소로 관리하는 환경에서는 규칙이 다음처럼 짧아진다.

```gitignore
/Plugins/OWTStateMonitor/
```

플러그인 소스는 공유하고 Unreal 생성 파일만 제외하려면 프로젝트의 `.gitignore`에 다음과 같이 지정한다. 맨 앞 `/` 없는 디렉터리 패턴은 플러그인 내부의 같은 생성 디렉터리에도 적용된다.

```gitignore
Binaries/
Intermediate/
DerivedDataCache/
Saved/
.vs/
```

이미 추적 중인 플러그인을 앞으로 저장소에서 제외하기로 결정한 경우에는 먼저 ignore 규칙을 추가하고 프로젝트 디렉터리에서 다음 명령을 사용한다. `--cached`는 로컬 파일을 유지하고 Git index에서 제거한다. 변경을 커밋하면 다른 checkout에서도 해당 경로가 저장소에서 제거되므로 플러그인 배포 경로를 별도로 준비해야 한다. [Git rm 공식 문서](https://git-scm.com/docs/git-rm)

```powershell
git rm -r --cached -- Plugins/OWTStateMonitor
```

이번 작업에서는 Git 설정·index·추적 파일을 변경하지 않았다. 현재 확인한 새 모니터 플러그인은 untracked 상태다. 플러그인 전체를 제외할 경우 `OWTRuntimeEditing.uplugin` 및 `VTBOWTEditor.Build.cs`의 의존성이 남으므로 다른 PC/CI에 별도 설치해야 한다. 독립 플러그인 패키지나 별도 저장소로 전달하고 사용 버전을 고정하는 방식을 사용한다.
