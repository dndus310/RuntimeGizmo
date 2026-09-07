#if WITH_DEV_AUTOMATION_TESTS

#include "InputAction.h"
#include "InputMappingContext.h"
#include "InputCoreTypes.h"
#include "InputTriggers.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FVTBEditorInputAssetsTest,
	"VTB.RuntimeGizmo.Input.MappingContextAssets",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FVTBEditorInputAssetsTest::RunTest(const FString& Parameters)
{
	const UInputMappingContext* Context = LoadObject<UInputMappingContext>(
		nullptr, TEXT("/Game/RuntimeEditor/Input/IMC_VTBEditor.IMC_VTBEditor"));
	if (!TestNotNull(TEXT("Runtime editor mapping context asset"), Context))
	{
		return false;
	}

	const TMap<FString, EInputActionValueType> ExpectedTypes =
	{
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSelect.IA_EditSelect"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditTranslation.IA_EditTranslation"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditRotation.IA_EditRotation"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditScale.IA_EditScale"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_ChagneGizmoMode.IA_ChagneGizmoMode"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSelectCancel.IA_EditSelectCancel"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSpace.IA_EditSpace"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditCtrl.IA_EditCtrl"), EInputActionValueType::Boolean },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EInputActionValueType::Axis3D },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraLook.IA_CameraLook"), EInputActionValueType::Axis2D },
	};

	const TArray<TPair<FString, FKey>> ExpectedKeys =
	{
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSelect.IA_EditSelect"), EKeys::LeftMouseButton },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditTranslation.IA_EditTranslation"), EKeys::W },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditRotation.IA_EditRotation"), EKeys::E },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditScale.IA_EditScale"), EKeys::R },
		{ TEXT("/Game/RuntimeEditor/Input/IA_ChagneGizmoMode.IA_ChagneGizmoMode"), EKeys::T },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSelectCancel.IA_EditSelectCancel"), EKeys::Escape },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditSpace.IA_EditSpace"), EKeys::Tilde },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditCtrl.IA_EditCtrl"), EKeys::LeftControl },
		{ TEXT("/Game/RuntimeEditor/Input/IA_EditCtrl.IA_EditCtrl"), EKeys::RightControl },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::W },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::S },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::D },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::A },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::E },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraMove.IA_CameraMove"), EKeys::Q },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraLook.IA_CameraLook"), EKeys::MouseX },
		{ TEXT("/Game/RuntimeEditor/Input/IA_CameraLook.IA_CameraLook"), EKeys::MouseY },
	};

	const UInputAction* EditSpaceAction = nullptr;
	const UInputAction* EditCtrlAction = nullptr;
	TSet<FString> ActualKeys;
	bool bEditSpaceUsesCtrlChord = false;
	for (const FEnhancedActionKeyMapping& Mapping : Context->GetMappings())
	{
		if (const UInputAction* Action = Mapping.Action.Get())
		{
			const FString ActionPath = Action->GetPathName();
			ActualKeys.Add(FString::Printf(TEXT("%s|%s"), *ActionPath, *Mapping.Key.GetFName().ToString()));

			if (const EInputActionValueType* ExpectedType = ExpectedTypes.Find(ActionPath))
			{
				TestEqual(FString::Printf(TEXT("%s value type"), *Action->GetName()), Action->ValueType, *ExpectedType);
			}

			if (ActionPath == TEXT("/Game/RuntimeEditor/Input/IA_EditSpace.IA_EditSpace"))
			{
				EditSpaceAction = Action;
				for (const UInputTrigger* Trigger : Mapping.Triggers)
				{
					const UInputTriggerChordAction* ChordTrigger = Cast<UInputTriggerChordAction>(Trigger);
					if (ChordTrigger && ChordTrigger->ChordAction)
					{
						bEditSpaceUsesCtrlChord = ChordTrigger->ChordAction->GetPathName()
							== TEXT("/Game/RuntimeEditor/Input/IA_EditCtrl.IA_EditCtrl");
					}
				}
			}
			else if (ActionPath == TEXT("/Game/RuntimeEditor/Input/IA_EditCtrl.IA_EditCtrl"))
			{
				EditCtrlAction = Action;
			}
		}
	}

	for (const TPair<FString, FKey>& Expected : ExpectedKeys)
	{
		const FString ExpectedKey = FString::Printf(TEXT("%s|%s"), *Expected.Key, *Expected.Value.GetFName().ToString());
		TestTrue(FString::Printf(TEXT("%s is mapped to %s"), *Expected.Key, *Expected.Value.ToString()),
			ActualKeys.Contains(ExpectedKey));
	}
	TestNotNull(TEXT("IA_EditSpace exists"), EditSpaceAction);
	TestNotNull(TEXT("IA_EditCtrl exists"), EditCtrlAction);
	TestTrue(TEXT("IA_EditSpace is triggered by Ctrl chord"), bEditSpaceUsesCtrlChord);
	TestEqual(TEXT("Only expected editor mappings are exposed"), ActualKeys.Num(), ExpectedKeys.Num());
	return true;
}

#endif // WITH_DEV_AUTOMATION_TESTS
