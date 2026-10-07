# OWT State Monitor

UE 5.7 Runtime용 읽기 전용 구조체 모니터다. `OWTRuntimeEditing`, ITF, PCG, Pub-Sub, 프로젝트 Actor에 의존하지 않는다. `Plugins/OWTStateMonitor` 디렉터리를 다른 Unreal 프로젝트의 `Plugins`에 복사하고 플러그인을 활성화한다. C++ 소비 모듈의 `Build.cs` 의존성에는 `OWTStateMonitor`를 추가한다.

## 입력과 소유

모니터를 사용하는 시스템이 `FOWTStateMonitorModel`을 소유한다. 게임 스레드에서 `UScriptStruct`와 해당 타입의 유효한 구조체 주소를 함께 전달한다. 호출이 반환되기 전에 필드 값이 별도 문자열·트리 데이터로 복사되므로 원본 주소를 보관하지 않는다. 객체 참조는 경로 또는 null로 표시하며 참조 대상의 내부를 재귀 탐색하거나 수명을 연장하지 않는다.

```cpp
#include "OWTStateMonitorModel.h"
#include "SOWTStateMonitor.h"

// Owner의 초기화 함수에서 생성한다.
MonitorModel = MakeShared<FOWTStateMonitorModel>();

// 실제 상태를 관찰한 뒤 제출한다. Snapshot은 FMyObservedState USTRUCT 값이다.
MonitorModel->SubmitSnapshot(
    TEXT("MySystem"),
    FText::FromString(TEXT("My system")),
    FMyObservedState::StaticStruct(),
    &Snapshot);

// Slate 부모 위젯에 배치한다.
SNew(SOWTStateMonitor).Model(MonitorModel);
```

`FMyObservedState`는 소비 프로젝트에서 정의한다. `USTRUCT` 안의 `UPROPERTY`만 자동으로 열거한다. 리플렉션 정보가 없는 일반 C++ 구조체·멤버·함수의 상태는 추출하지 않는다. 생산자는 관찰용 USTRUCT를 구성하거나 기존 reflected snapshot을 그대로 전달한다. 제출은 갱신 시점에 직접 호출하며, 모듈 자체가 Actor를 찾거나 원본을 폴링하지 않는다.

중첩 구조체, 고정 배열, `TArray`, `TMap`, `TSet`, 숫자, bool, enum, 문자열, 이름, 텍스트 및 객체 참조를 표시한다. 기타 프로퍼티는 Unreal의 텍스트 표현을 사용한다. JSON의 64비트 정수는 double로 변환하지 않고 정수 토큰으로 기록한다. 외부 JavaScript 뷰어에서 읽을 때에는 그 뷰어의 숫자 정밀도 제한이 별도로 적용될 수 있다.

## Blueprint / UMG

Widget Blueprint의 팔레트에서 **State Monitor** (`UOWTStateMonitorWidget`)를 추가한다. 해당 위젯의 `Submit Snapshot`에 Source ID, 표시 이름, 구조체를 연결한다. Snapshot 핀은 구조체에 맞춰 바뀌는 wildcard이며 구조체의 reflected 필드를 읽는다. Actor나 일반 숫자를 직접 연결하는 입력은 지원하지 않는다.

Native C++에서 UMG 래퍼를 사용할 때에는 `SubmitStructSnapshot(SourceId, Label, StructType, StructData)`를 호출한다. `SubmitSnapshot`의 int32 선언은 Blueprint custom thunk를 위한 자리 표시자이므로 C++에서는 호출하지 않는다. 위젯은 자체 모니터 모델을 소유하고 Slate 재생성 시에도 수집한 값을 유지한다.

## 표시와 이력

- **Current**: 최신 구조체를 트리로 펼쳐 필드·타입·값을 확인한다. 변경된 필드는 색상과 이전 값으로 식별한다. 원본 데이터를 편집하는 기능은 없다.
- **JSON**: 같은 관찰값의 JSON 표현을 표시하고 클립보드로 복사한다.
- **History**: 상태가 달라진 시점의 경로·이전 값·새 값을 보관한다. 전체 과거 객체를 보관하거나 Undo를 수행하는 스택은 아니다.
- **Pause history**: 이력 표시를 멈춘다. 생산자의 수집과 Current 갱신은 계속된다.
- **Reset baseline**: 현재 변경 강조 표시를 지운다. 다음 변화는 직전 관찰값과 비교한다.
- **Clear history**: 모니터의 해당 이력만 비운다. 원본 시스템의 이벤트 저장소나 Actor 상태에는 영향을 주지 않는다.

Source ID는 모니터 내에서 고유하고 안정적이어야 한다. 같은 ID를 제출하면 해당 Current가 교체된다. 동일한 값의 반복 제출은 이력을 추가하지 않는다. 없어진 필드는 변경 이력의 Removed로 기록하며 Current에는 남기지 않는다. 배열의 식별 기준은 인덱스이므로 중간 삽입은 이후 원소의 값 변경으로 보일 수 있다.

노드·깊이·문자열·source 수·이력 수에 한도가 있다. 기본값 및 조절 가능한 항목은 `FOWTStateMonitorSettings`를 참고한다. 일부만 표시된 경우 뷰어에서 제한 상태를 표시한다. JSON은 원본의 완전한 저장 파일을 보장하는 직렬화 형식이 아니라 관찰용 표현이다. 설정과 이력은 메모리에 있으며 기본 동작은 파일을 생성하지 않는다.

## RuntimeGizmo 연결

`UOWTAttributeDetailsWidget`이 관찰 구조체와 별도의 이벤트 저널 구조체를 만들어 제출한다. 선택·Mode·Tool·Duplicate·PCG 상태는 편집기 typed API에서 읽는다. Events의 JSON 문자열을 상태로 역해석하지 않는다. 화면을 열었을 때 약 10Hz로 관찰하며 기존 F3 진입점을 사용한다. Events는 자체 유한 저널이므로 모니터의 변경 이력을 다시 쌓지 않는다.

## 형상관리

모듈은 형상관리 설정을 바꾸지 않는다. 개인 작업 폴더나 로그만 제외하려면 저장소의 `.git/info/exclude`를 사용하고, 팀 공통 생성 파일은 `.gitignore`에 지정한다. 이미 추적 중인 파일은 ignore 규칙만으로 제외되지 않는다.

이 플러그인 전체를 제외하면 다른 PC와 CI에도 플러그인을 별도 설치해야 한다. `OWTRuntimeEditing`에는 이 플러그인 의존성이 있으므로 소스만 숨기고 전달을 생략하면 빌드할 수 없다. 별도 저장소·버전 패키지·엔진 플러그인 설치 등으로 공급 경로를 유지한다.
