# RuntimeGizmo Playground 체험 및 검증

이 Playground는 실제 프로젝트에 저장된 Blueprint와 레벨을 사용하는 Runtime 편집 체험 환경이다. 기본 시작 맵은 밝은 전시형 `L_OWTPlayground`이며, 별도의 `L_OWTStress`에서 많은 Actor가 있는 상태를 확인한다. 이전 `L_OWTEditSample`도 보존한다.

현재 체험 대상은 v3의 선택·Transform·Details·복제·이벤트·PCG 기능이다. [v4 피드백 명세](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Docs/AttributeEditMode_Feedback_Spec.md)의 다중 선택, 삭제, Undo/Redo, Current Tree/JSON 등은 이 콘텐츠 작업으로 구현되는 기능이 아니다.

## 실행

패키지 위치는 `Saved/Packaged/OWTPlayground/Windows/simple_proj.exe`다. Unreal Editor를 실행하지 않고 이 파일을 실행할 수 있다. 함께 배포할 때는 exe 하나가 아닌 `Windows` 폴더 전체를 복사한다.

PowerShell에서 프로젝트 폴더를 기준으로 실행할 수도 있다.

```powershell
./Scripts/Launch-Playground.ps1
./Scripts/Launch-Playground.ps1 -Level Stress
```

Editor에서 프로젝트를 열면 새 전시 레벨이 시작 맵이다. Content Browser의 `/Game/OWTPlayground/Blueprints`에서 생성한 Blueprint를 열고 component 구성과 변수를 직접 수정할 수 있다.

## 조작

| 입력 | 기능 |
|---|---|
| Actor 클릭 | 선택 및 Details 표시 |
| W / E / R | 이동 / 회전 / 크기 Gizmo |
| Details 숫자 입력·드래그 | 선택 객체 Transform 변경 |
| Ctrl+D | 현재 선택 복제; v3 기본 X 방향 100 cm offset |
| F2 | 편집 활성 전환 |
| F3 | 이벤트 모니터 전환 |
| 마우스 오른쪽 버튼 + 마우스 / WASD | 카메라 방향 / 이동 |
| F6 | 전시 레벨 로드 |
| F7 | 스트레스 레벨 로드 |
| F8 | 상세 도움말 표시 전환 |

레벨에 들어가면 편집과 초기 선택이 준비된다. 좌측 HUD에는 실제 Actor·component·mesh/instance 개수와 최근 FPS·프레임 시간이 표시된다. 우측 Details와 겹치지 않도록 배치했다.

F6/F7은 현재 레벨을 다시 로드하므로 임시 편집 결과가 초기화된다. 입력 필드 편집, Gizmo 드래그, 복제 처리 중에는 전환을 막고 사유를 표시한다. 마우스 버튼을 놓고 포인터를 sidebar 밖으로 옮긴 다음 전환한다.

## 저장된 콘텐츠

| 레벨 | 편집 Actor | 구성 |
|---|---:|---|
| `/Game/OWTPlayground/Maps/L_OWTPlayground` | 54 | Blueprint 9종을 전시 구역별로 6개씩 배치 |
| `/Game/OWTPlayground/Maps/L_OWTStress` | 600 | 같은 Blueprint들을 사용하되 light·CAC·PCG 등 무거운 종류의 수를 제한 |

바닥·전시대·간판·조명·카메라 시작점은 환경 Actor다. 위 숫자에는 환경 Actor, CAC의 소유 자식, PCG 생성물, Runtime 편집 서비스/Gizmo가 포함되지 않는다. 따라서 HUD의 전체 Actor 수는 더 크다. 환경 mesh는 Visibility 선택 trace를 막지 않게 구성했다.

| Blueprint | 주요 구성 및 확인할 점 |
|---|---|
| `BP_OWTColorBlock` | 이동 가능한 StaticMesh, collision, 색상 material |
| `BP_OWTStackedSculpture` | 중첩 SceneComponent와 서로 다른 상대 Transform |
| `BP_OWTLightRig` | PointLight·SpotLight, `Accent`를 적용하는 Construction graph |
| `BP_OWTInstanceArray` | ISM과 6개 instance Transform |
| `BP_OWTHierarchicalArray` | HISM과 9개 instance Transform |
| `BP_OWTSplineDisplay` | 4개 spline point 및 mesh marker |
| `BP_OWTChildAssembly` | 별도 Blueprint를 소유하는 ChildActorComponent |
| `BP_OWTPhysicsProp` | 질량·충돌 등 물리 설정. 초기 simulation은 꺼진 편집용 상태 |
| `BP_OWTPCGDisplay` | 기존 plugin PCG graph를 사용하는 독립 component 구성 |

Blueprint마다 `FixtureLabel`, `Revision`, `Accent` 변수가 있다. 색상·환경 material instance 11종과 공통 surface material을 프로젝트에 저장한다. 구성은 런타임에만 임시 생성하는 mock이 아니므로 Editor에서도 그대로 열 수 있다.

PCG 복제는 authored graph·parameter·seed·설정의 독립 복원과 재생성을 뜻한다. 움직인 위치의 입력이 다르면 결과가 원본과 달라질 수 있다. PhysicsProp은 설정 보존을 검증하는 fixture이며, 초기부터 모든 물체를 떨어뜨리는 물리 시뮬레이션 부하 레벨은 아니다.

전시 레벨의 PCG 6개는 OnLoad로 실제 결과를 자동 생성한다. Blueprint 기본값과 스트레스 레벨의 PCG는 OnDemand이며, 자동화 stress가 생성·복제·정리 경로를 명시적으로 실행한다.

## 재현 가능한 생성·빌드

다음 명령은 Editor target을 빌드하고, assets를 생성하거나 검증한 뒤 두 레벨을 포함한 Windows Development 패키지를 만든다.

```powershell
./Scripts/Build-Playground.ps1
```

기존 Playground가 있으면 검증만 실행한다. 생성된 assets를 의도적으로 초기 상태로 되돌릴 때만 아래 옵션을 사용한다. 먼저 직접 수정한 Blueprint와 레벨을 백업한다.

```powershell
./Scripts/Build-Playground.ps1 -RegenerateAssets
```

생성 commandlet은 `/Game/OWTPlayground`의 고정 생성 목록과 `Content/OWTPlayground/OWTPlayground.generated.json` manifest를 확인한다. manifest에 등록되지 않은 기존 assets는 덮어쓰지 않는다. 원래 sample 레벨과 plugin 콘텐츠는 생성자의 삭제 범위에 포함하지 않는다.

이번 작업 전 Source·Config·Content·Docs·Scripts는 `Saved/Backups/BeforePlayground_20261008_004359`에 백업했다. Playground 콘텐츠와 실행용 host는 `simple_proj`에 있으며, 재사용 편집 plugin의 배포 ZIP과 구분한다.

## 테스트의 의미

```powershell
./Scripts/Test-Playground.ps1
./Scripts/Test-Playground.ps1 -Suite Smoke
./Scripts/Test-Playground.ps1 -Suite Stress
./Scripts/Test-Playground.ps1 -Suite Navigation
```

테스트는 packaged 게임에서 실제 저장된 맵과 cooked Blueprint를 사용한다. `RenderOffscreen`은 창을 화면 밖에서 렌더링하는 옵션이며 NullRHI가 아니다. 결과 report와 별도 metrics, 실제 UI가 포함된 PNG를 남긴다.

- Smoke: Blueprint 9종의 구성과 변수, 실제 selection/Details 변환, 복제본의 component·참조·이름·PCG 독립성을 반복 확인한다.
- Stress: 600개 Actor 레벨에서 같은 Blueprint들의 복제·관찰·정리를 여러 프레임에 걸쳐 반복한다. 작업 프레임 시간, request→commit/ready 시간, process 메모리, 이벤트 보관 수와 잔여 임시 Actor 수를 기록한다.
- Navigation: packaged 환경에서 전시→스트레스→전시 레벨 전환, GameMode/HUD 재초기화와 단축키 바인딩을 확인한다.
- 기존 회귀: v3 편집·Pub-Sub·BP/CAC·PCG 관련 자동화 검사를 별도로 실행한다.

반복 복제 수는 동시에 살아 있는 복제본 수와 다르다. 기본 stress는 600개 배경 Actor를 유지한 채 임시 복제본을 순차 생성·검증·정리한다. 작업 종료 시 GC 전후 메모리와 복제 Actor/component의 weak 참조를 별도로 검사한다. GC 및 이후 정리 프레임은 작업 시간 측정에서 제외한다. 측정 메모리 변화에는 renderer cache와 이력 할당도 포함되므로 그 수치만으로 메모리 누수라고 판정하지 않는다. 성능 값은 해상도·GPU·실행 옵션·warmup 조건과 함께 읽어야 한다.

## 이번 실행 결과

2026-10-08 KST 기준, UE 5.7.4 Windows Development 패키지에서 검증했다. Ryzen 9 5900X, RTX 3080, RAM 32 GB, D3D12 SM6, 1600×900 조건이다. 자동화 실행은 `-RenderOffscreen -nosound -unattended`를 사용했으며 실제 렌더러로 UI 포함 화면을 캡처했다.

- Editor 빌드 및 Blueprint·레벨 생성, Windows build/cook/stage/archive 성공. [패키지 로그](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Logs/Playground_Package.log).
- 저장 후 읽기 전용 `ValidateOnly` 성공: Blueprint 9개, material 12개, map 2개, 총 23개 asset 및 manifest 정상. [검증 로그](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Logs/Playground_ValidateAssets.log).
- 패키지 자동화 **25/25 통과**, 실패·경고·미실행 0. Smoke/Stress의 반복 횟수는 각 테스트 내부 작업 수이며 별도 자동화 case 수와 구분한다.

| 검증 | 결과 | 원본 report |
|---|---|---|
| Smoke | 9종 × 2회, 18/18 복제·구성 검증 | [Smoke](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Automation/Playground_Packaged_Smoke_20261008_010509/index.json) |
| Stress | 600개 배경 Actor, 9종 × 32회, 288/288 검증 | [Stress](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Automation/Playground_Packaged_Stress_20261008_010522/index.json) |
| Navigation | 전시 54 → 스트레스 600 → 전시 54, F8 press/hold/release 입력 검증 | [Navigation](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Automation/Playground_Packaged_Navigation_20261008_010549/index.json) |
| 기존 회귀 | `OWT.Runtime` + `OWT.EventCore`, 22/22 통과 | [Regression](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Automation/Playground_PackagedRegression/index.json) |

| 측정 | Smoke | Stress |
|---|---:|---:|
| 작업 프레임 평균 / p95 | 8.06 / 10.59 ms | 8.91 / 11.61 ms |
| 복제 요청 → commit 평균 / p95 | 12.06 / 14.16 ms | 14.86 / 17.73 ms |
| 복제 요청 → ready 평균 / p95 | 22.02 / 26.60 ms | 24.45 / 29.14 ms |
| 별도 full GC 시간 | 55.02 ms | 654.00 ms |
| GC 전 process 물리 메모리 증가 | 18.57 MiB | 346.20 MiB |
| GC 및 정리 대기 후 증가 | 11.22 MiB | 122.73 MiB |
| 종료 시 임시 Actor / GC 후 추적 복제 Actor·component | 0 / 0·0 | 0 / 0·0 |

Stress에서는 이벤트 8,224건을 관찰했고 monitor 및 operation 보관은 각각 256개로 제한됐다. 추적한 복제 Actor 320개와 component 1,120개는 full GC 후 모두 해제됐다. CAC의 소유 자식이 포함되어 Actor 추적 수가 복제 요청 수보다 많다. 원본 편집 Actor 수는 각 테스트 전후 54개·600개로 유지됐다.

위 작업 프레임 통계는 진단용 full GC와 이후 정리 대기를 제외한다. **Stress의 full GC는 약 654 ms**이므로 이 결과를 전체 실행에서 긴 멈춤이 없다는 의미로 해석하면 안 된다. GC 후 process 메모리 증가도 남아 있으며, 해당 반복 검증만으로 모든 메모리 누수의 부재를 증명하지 않는다. 이 측정은 약 18초 동안의 288회 작업이며 장시간 endurance, GPU PCG 또는 활성 물리 simulation 부하 검증은 포함하지 않는다.

원시 측정값: [Smoke metrics](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Playground/Smoke.metrics.json), [Stress metrics](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Playground/Stress.metrics.json).

실제 패키지 전시 화면:

![54 Actor 전시 레벨](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Playground/Showroom.png)

실제 패키지 스트레스 화면:

![600 Actor 스트레스 레벨](C:/Users/jkyii/Desktop/New1006/RuntimeGizmo-Version_4/simple_proj/Saved/Verification/Playground/Stress.png)
