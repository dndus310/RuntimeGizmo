# Runtime Actor 복제 가이드

## 큰 그림

런타임 복제는 월드에 있는 Actor와 그 계층을 **설정(authored configuration) 기준으로** 새 Actor로 만든다. 작업자가 알아야 할 것은 세 가지다.

1. **사용**: `UOWTDuplicationLibrary::DuplicateActor` 한 번 호출로 끝난다. 수명, Tick, 정리는 월드 서브시스템이 맡는다.
2. **동작**: 내부는 `검증·캡처 → 스폰·복원 → 커밋` 세 단계다. 어느 단계에서 실패해도 월드에는 아무것도 남지 않는다.
3. **확장**: 기본으로 복제되지 않는 상태가 있으면, 필요한 깊이에 맞는 확장 지점 하나를 고른다. 가장 가벼운 확장은 람다 하나다.

```
UOWTDuplicationLibrary ─▶ UOWTDuplicationSubsystem ─▶ UOWTRuntimeActorDuplicator ─▶ FOWTDuplicationPipeline
   (한 줄 호출)              (월드별 수명·Tick)            (세션 registry·커밋)          (검증·캡처·스폰·복원)
```

## 1. 사용

### C++

```cpp
#include "Duplication/OWTDuplicationLibrary.h"

FOWTDuplicationResult Result = UOWTDuplicationLibrary::DuplicateActorAtOffset(Source, FVector(200, 0, 0));
if (!Result.bSucceeded)
{
	UE_LOG(LogTemp, Warning, TEXT("Duplicate failed: %s"), *Result.Error);
	return;
}
AActor* Copy = Result.Duplicate;            // 루트 복제본
for (AActor* Created : Result.CreatedActors) // ChildActor, 부착 Actor 포함 전체
{
}
```

옵션이 필요하면 `FOWTDuplicationOptions`를 채워 `DuplicateActor(Source, Options)`를 호출한다.

| 옵션 | 기본값 | 의미 |
|---|---|---|
| `WorldOffset` | 0 | 복제 계층 전체에 한 번 더하는 월드 오프셋 |
| `HierarchyScope` | `AuthoredHierarchy` | 부착 Actor까지 포함. `ActorAndManagedChildren`은 루트와 ChildActor만 |
| `GenerationPolicy` | `RegenerateIfSourceGenerated` | OnDemand PCG를 원본이 생성된 경우에만 재생성 |
| `IsCancellationRequested` | 없음 | 사용자 콜백 뒤마다 확인하는 취소 조건 (C++ 전용) |
| `OnAuthoredCommitted` | 없음 | 커밋 직후, PCG 생성 요청 전에 한 번 호출 (C++ 전용) |

### Blueprint

`Duplicate Actor At Offset`, `Duplicate Actor`, `Can Duplicate Actor`, `Get Procedural Components` 노드를 사용한다. 결과 구조체를 Break 해서 `bSucceeded`, `Error`, `Duplicate`, `CreatedActors`를 읽는다.

### 사전 진단

`CanDuplicateActor(Source, Options, OutError)`는 실제 복제와 같은 검증·캡처를 하되 아무것도 스폰하지 않는다. UI에서 복제 버튼을 비활성화하거나 이유를 보여 줄 때 쓴다.

### 결과 해석

- `bSucceeded`는 **설정 복제의 완료**다. PCG 생성 완료가 아니다. PCG 상태는 `GetProceduralComponents`로 컴포넌트별로 관찰한다.
- `CreatedActors`는 이 요청이 만든 Actor 전부다. 되돌리기(Undo)를 구현할 때 이 목록을 기록하면 된다.

### 세션 전용 duplicator가 필요한 경우

AttributeEditMode처럼 특정 세션의 수명과 registry를 따라야 할 때만 `UOWTRuntimeActorDuplicator`를 직접 만든다. 이 경우 `Initialize`/`Tick`/`Deinitialize`를 소유자가 전달해야 한다. 자세한 계약은 `RuntimeAttributeEditor_Usage.md`의 "내부 복제 서비스의 저수준 호환 API"를 따른다.

## 2. 동작

| 단계 | 하는 일 | 실패 시 |
|---|---|---|
| 검증·캡처 (`Prepare`) | 계층 수집과 원본 검증, 어댑터 캡처, 스냅샷 작성, 참조 검증 | 월드 변경 없음 |
| 스폰·복원 (`Execute`) | 이름 예약, 지연 스폰, 구성 스크립트 실행, BeginPlay 전 상태 복원 | 만든 Actor 전부 파괴, 어댑터 Rollback |
| 커밋 | `PrepareCommit` → `OnAuthoredCommitted` → `Commit` (PCG 생성 요청) | 취소 불가 |

검사는 두 종류만 있다.

- **사전 검사**는 캡처 전에 한 번만 한다. 원본 규칙, 계층, 지원하지 않는 구조체, 관리 출력 참조가 대상이다.
- **콜백 경계 검사**는 사용자 코드(구성 스크립트, 컴포넌트 등록, Participant, 콜백)가 실행된 직후에만 한다. 대상은 원본 생존, 취소 요청, 소유자 생존, 복제 객체 생존이다.

## 3. 확장

필요한 깊이에 맞춰 위에서부터 고른다. 모든 등록은 provider(`IOWTDuplicationAdapterProvider`) 또는 세션 registry(`Duplicator->GetAdapterRegistry()`)에서 한다.

| 레벨 | 언제 | 등록 위치 |
|---|---|---|
| L1 프로퍼티 정책 | 특정 프로퍼티를 복사에서 넣거나 뺄 때 | `Registry.GetPropertyPolicies()` |
| L2 원본 규칙 | 기본 거부(Controller, 물리 Constraint 등)를 허용하거나 새 거부를 추가할 때 | `Registry.GetSourceRules()` |
| L3 Participant | 내 클래스의 네이티브(비 리플렉션) 상태를 복원할 때 | 클래스에 인터페이스 구현 |
| L4 어댑터 | 내가 소유하지 않은 타입이나 절차적 시스템을 지원할 때 | `Registry.Register(Class, Factory, Role)` |

### Provider 등록

provider는 모듈 시작 시 modular feature로 등록한다. 각 duplicator는 초기화될 때 provider id 순서로 registry를 채운다.

```cpp
class FMyDuplicationProvider final : public IOWTDuplicationAdapterProvider
{
public:
	virtual FName GetProviderId() const override { return TEXT("MyGame.Duplication"); }
	virtual void RegisterAdapters(FOWTDuplicationAdapterRegistry& Registry) const override;
};

// StartupModule
IModularFeatures::Get().RegisterModularFeature(IOWTDuplicationAdapterProvider::GetFeatureName(), &Provider);
```

### L1 프로퍼티 정책

결과를 비워 두면(unset) 상위 클래스 규칙으로 넘어간다. 더 구체적인 클래스의 규칙이 등록 순서와 관계없이 우선한다.

```cpp
Registry.GetPropertyPolicies().Register(AMyActor::StaticClass(),
    [](const UObject& Object, const FProperty& Property) -> TOptional<bool>
    {
	    if (Property.GetFName() == TEXT("RuntimeCache"))
	    {
		    return false;
	    }
	    return {};
    });
```

### L2 원본 규칙

엔진 provider가 기본 거부 규칙을 등록한다(AInfo, AController, ABrush, 빙의된 Pawn, PhysicsConstraint, StaticMesh가 아닌 시뮬레이션 물리 등). 어댑터나 Participant로 그 상태를 직접 복원한다면 내 하위 클래스에 `Allow`를 등록해 덮어쓴다.

```cpp
Registry.GetSourceRules().Register(AMyVehiclePawn::StaticClass(),
    [](const UObject& Object)
    {
	    // 빙의 상태는 MyVehicleParticipant가 다시 연결한다.
	    return FOWTSourceDecision::Allow();
    });
```

`Defer()`는 상위 클래스 규칙에 맡기고, `Reject(Reason)`은 그 이유로 복제를 거부한다.

### L3 Participant

내 클래스가 리플렉션되지 않은 상태(네이티브 핸들, 캐시)를 갖고 있다면 `IOWTRuntimeDuplicationParticipant`를 구현한다. 모든 계층의 매핑이 끝난 뒤, BeginPlay 전에 복제본 쪽에서 호출된다. `DuplicatedObjects`로 원본 내부 참조를 복제본 참조로 바꾼다. 실시간 핸들을 복사하지 말고 다시 만든다.

### L4 어댑터

`IOWTDuplicationAdapter`를 구현한다. 한 인스턴스는 한 작업에만 쓰이며 원본을 수정하지 않는다. 콜백 순서는 다음과 같다.

```
Initialize → ValidateObject → CaptureObject → VisitCapturedReferences → ValidateReferences
→ PrepareDestination → Restore → PrepareCommit → Commit   (실패 시 Rollback)
```

- **Primary**는 클래스마다 가장 구체적인 하나만 선택된다. `OwnsProperty`/`OwnsObject`로 복사 책임을 가져갈 수 있다.
- **Auxiliary**는 해당하는 것이 모두 선택되지만 소유권을 가질 수 없다. 검증과 관찰 용도로 쓴다.
- **Managed**(`IsManagedObject`)는 어댑터가 다시 생성하는 출력이다. 복사되지 않고, 다른 객체가 참조하면 거부된다.
- 커밋 뒤에도 관찰이 필요하면 `GetProceduralComponents`가 비어 있지 않게 유지한다. 그동안 duplicator가 `Tick`을 전달한다.

참고 구현은 `Private/Duplication/PCG/OWTPCGDuplicationAdapter.h`이다. 설정 스냅샷(`FOWTPCGConfigurationSnapshot`), 그래프 복제(`FOWTPCGGraphCloner`), 참조 재매핑(`FOWTPCGReferenceRemapper`), 생성 상태 추적(`FOWTPCGGenerationTracker`)이 각각 별도 클래스로 나뉘어 있다.

## 4. 소스 지도

폴더 이름이 곧 단계입니다. `Capture`가 원본을 읽고, `Restore`가 복제본을 만들고, `Pipeline`이 두 단계를 이어 줍니다.

```
Public/Duplication/
  OWTDuplicationLibrary.h             한 줄 API (BP/C++)
  OWTDuplicationSubsystem.h           월드별 duplicator 수명·Tick
  OWTDuplicationResult.h              결과 구조체
  OWTDuplicationRequest.h             옵션
  OWTRuntimeActorDuplicator.h         세션 단위 파사드
  OWTDuplicationAdapter.h             어댑터 인터페이스 + registry
  OWTDuplicationPropertyPolicy.h      L1 프로퍼티 정책
  OWTDuplicationSourceRules.h         L2 원본 규칙
  OWTRuntimeDuplicationParticipant.h  L3 Participant
  OWTDuplicationAdapterProvider.h     provider 등록
Private/Duplication/
  Pipeline/    FOWTDuplicationPipeline(작업 흐름), FOWTDuplicationContext(공유 서비스), FOWTAdapterDispatcher(어댑터 호출·소유권)
  Properties/  FOWTPropertyFilter(복사 대상 판정), FOWTPropertyValue(값 복사본), FOWTReferenceVisitor(참조 순회),
               FOWTUnsupportedStructCache(미지원 구조체 탐지)
  Capture/     FOWTHierarchyCollector(계층 수집), FOWTSourceValidator(원본 검증), FOWTSnapshotBuilder(스냅샷 작성),
               FOWT*Snapshot(스냅샷 데이터)
  Restore/     FOWTSnapshotRestorer(스폰·복원), FOWTActorNameAllocator(이름 할당), UOWTRestoreHookComponent(구성 직후 훅)
  Engine/      FOWTEngineDuplicationProvider, FOWTEnginePropertyPolicies, FOWTEngineSourceRules
  PCG/         FOWTPCGDuplicationProvider, FOWTPCGDuplicationAdapter, FOWTPCGConfigurationSnapshot, FOWTPCGGraphCloner,
               FOWTPCGReferenceRemapper, FOWTPCGGenerationTracker, FOWTPCGAuthoredProperties
```

## 5. 알려진 한계

- Standalone 월드만 지원한다.
- `FInstancedStruct`, `FInstancedPropertyBag`, `FBodyInstance`, `FTickFunction`을 담은 프로퍼티는 어댑터 없이는 거부된다.
- PCG 생성 결과는 복사하지 않고 다시 생성한다. Actor에 내장된 `UPCGGraph` 정의는 지원하지 않는다.
- Undo/Redo 기록은 이 기능 밖이다. `FOWTDuplicationResult::CreatedActors`를 히스토리 항목에 기록해 되돌리기에 사용한다.
