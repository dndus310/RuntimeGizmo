# 런타임 편집 도구 코드 검토 및 수정

검토 범위는 `Source/simple_proj`의 입력, 선택, 기즈모, 서브시스템 및 트랜잭션 코드입니다. 아래 내용은 소스와 로컬 UE 5.7 엔진 구현을 대조한 결과입니다. 사용자 요청에 따라 빌드, 컴파일, 에디터 실행 및 자동화 테스트 실행은 하지 않았습니다.

## 수정한 문제

| 문제와 발생 조건 | 수정 내용 | 주요 파일 |
|---|---|---|
| 입력·취소·Undo 콜백이 선택이나 기즈모 종류를 변경하면 실행 중인 기즈모를 교체할 수 있음 | 입력 처리와 트랜잭션 재생 중 변경 요청을 보관하고 다음 Tick에 적용. 기즈모 종류 변경도 같은 갱신 경로로 통합 | `RuntimeEditor/VTBEditorSubsystem.cpp`, `VTBEditorSpectatorPawn.cpp` |
| 취소 또는 Undo/Redo 콜백에서 다시 취소하거나 Context를 종료하면 실행 중인 변경 객체·매니저가 해제될 수 있음 | 재생 중 취소 차단, 종료 요청 지연, 중복 종료 방어. 매니저 해체 전에 런타임 콜백 비활성화 | `RuntimeEditor/Context/VTBEditorInteractiveToolsContext.cpp`, `Context/Private/VTBEditorTransactionsAPI.cpp` |
| 선택 해제·재선택 후 이전 Proxy의 Undo/Redo를 실행하거나 외부 코드가 액터 Transform을 변경하면 현재 Proxy의 피벗·상대 Transform이 오래된 값으로 남음 | 컴포넌트별 Transform을 비교해 외부 변경을 감지하고 새 Proxy로 연결. 활성 Proxy의 정상 드래그에서는 캐시만 갱신 | `RuntimeEditor/Selection/VTBEditorTargetSelection.cpp` |
| 기본 ITF 기즈모는 다중 선택에서도 축·평면별 크기 조절 핸들을 생성함 | 기본/커스텀 기즈모가 같은 선택 수 정책을 사용. 다중 선택은 균일 크기 조절, 단일 선택은 축·평면 조절 허용 | `RuntimeEditor/VTBEditorSubsystem.cpp` |
| 이미 생성된 핸들에 `UpdateBehavior()`의 새 Alt/Ctrl/Shift 제한 설정이 반영되지 않음 | 약한 기즈모 참조를 통해 현재 설정을 조회 | `RuntimeEditor/Gizmo/VTBEditorTransformGizmo.cpp` |
| 드래그 중 Pawn의 기본 Tick이 생략되고 포인터 위치 이중 갱신으로 이동량이 사라짐 | 기본 Tick을 항상 호출하고 포인터 이벤트 생성·전송 경로를 정리. press/release 플래그의 다음 이벤트 유출 방지 | `VTBEditorSpectatorPawn.cpp` |
| 소유 해제 시 매핑·마우스 설정·드래그 상태가 남거나 입력 취소 시 카메라 잠금이 유지될 수 있음 | `UnPossessed()` 정리 경로 추가. 직접 설치한 매핑과 잠금만 해제하고 이전 마우스 설정 복원. Canceled 및 누락된 release 처리 | `VTBEditorSpectatorPawn.h`, `VTBEditorSpectatorPawn.cpp` |

## 검증 상태

- UE 5.7 엔진 소스에서 사용하는 API·델리게이트 서명, Transform 갱신 순서, 기즈모 생성 및 Context 종료 순서를 확인했습니다.
- 수정 전 복사본과 파일별 차이를 검토하고 변경 부분의 공백 오류를 확인했습니다.
- 기존 테스트 소스를 보강하고 입력 수명, 외부 Transform 변경, 취소·종료 재진입에 대한 회귀 테스트 소스를 추가했습니다. **이 테스트들은 실행하지 않았으며 컴파일 성공도 확인하지 않았습니다.**

## 빌드 후 확인할 동작

1. `VTB.RuntimeGizmo` 자동화 테스트를 실행합니다. 다중 선택 테스트는 기본/커스텀 기즈모 모두를 검사하고, 재선택 후 Undo/Redo 테스트는 강제 재선택 없이 Tick만으로 피벗 갱신을 확인합니다.
2. PIE에서 이동·회전·크기 조절을 한 뒤 선택 해제, 재선택, Undo/Redo를 수행하고 액터와 기즈모 위치가 일치하는지 확인합니다.
3. 드래그 중 Escape, 창 포커스 이동, Pawn 소유 해제 후 카메라 입력과 마우스 설정이 정상 복구되는지 확인합니다.
4. 드래그 종료·취소 콜백에서 선택과 기즈모 종류를 변경하여 콜백 종료 후 최신 요청이 반영되는지 확인합니다.
