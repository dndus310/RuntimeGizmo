#include "Modes/OWTObjectEditMode.h"
#include "VTBOWTEditorSubsystem.h"
#include "UObject/StructOnScope.h"
#include "UObject/UnrealType.h"

// Stack-only result: ResolveContextHandler guarantees all members before invocation.
struct FOWTResolvedContextHandler
{
public:
	FOWTResolvedContextHandler() : Receiver(nullptr), Function(nullptr), Parameter(nullptr)
	{
	}
	FOWTResolvedContextHandler(UObject* InReceiver, UFunction* InFunction, FStructProperty* InParameter)
	    : Receiver(InReceiver), Function(InFunction), Parameter(InParameter)
	{
	}

public:
	UObject* Receiver;
	UFunction* Function;
	FStructProperty* Parameter;
};

namespace
{
FStructProperty* FindContextParameter(UFunction* Function, const UScriptStruct* ContextType)
{
	if (!Function || Function->NumParms != 1 || Function->HasAnyFunctionFlags(FUNC_Static | FUNC_Net))
	{
		return nullptr;
	}

	for (TFieldIterator<FProperty> It(Function); It; ++It)
	{
		if (!It->HasAnyPropertyFlags(CPF_Parm))
		{
			continue;
		}

		FStructProperty* Parameter = CastField<FStructProperty>(*It);
		if (!Parameter || Parameter->Struct != ContextType || Parameter->HasAnyPropertyFlags(CPF_ReturnParm) ||
		    (Parameter->HasAnyPropertyFlags(CPF_OutParm) && !Parameter->HasAnyPropertyFlags(CPF_ConstParm)))
		{
			return nullptr;
		}

		return Parameter;
	}

	return nullptr;
}
} // namespace

UOWTObjectEditMode::UOWTObjectEditMode() : ContextHandlers(), bContextBindingsInitialized(false)
{
}

bool UOWTObjectEditMode::ReceiveEditContext_Implementation(const FInstancedStruct& Context)
{
	check(IsInGameThread());

	if (!Context.IsValid())
	{
		return false;
	}

	EnsureContextBindings();

	const FOWTResolvedContextHandler Handler = ResolveContextHandler(Context.GetScriptStruct());
	if (!Handler.Receiver)
	{
		return false;
	}

	InvokeContextHandler(Context, Handler);

	return true;
}

void UOWTObjectEditMode::InitializeContextBindings_Implementation()
{
	BindContext<FOWTSelectObjectContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, SelectObject));
	BindContext<FOWTUndoContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, Undo));
	BindContext<FOWTRedoContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, Redo));
	BindContext<FOWTToggleCoordinateSystemContext>(
	    this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, ToggleCoordinateSystem));
	BindContext<FOWTToggleTransformSplineContext>(this,
	                                              GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, ToggleTransformSpline));
	BindContext<FOWTSetTranslationContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, SetTranslation));
	BindContext<FOWTSetRotationContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, SetRotation));
	BindContext<FOWTSetScaleContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, SetScale));
	BindContext<FOWTHideSelectionGizmoContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, HideSelectionGizmo));
	BindContext<FOWTDuplicateSelectionContext>(this, GET_FUNCTION_NAME_CHECKED(UOWTObjectEditMode, DuplicateSelection));
}

bool UOWTObjectEditMode::BindContextHandler(UScriptStruct* ContextType, UObject* Receiver, FName FunctionName)
{
	check(IsInGameThread());

	// This fallible registration API may be used to probe compatibility.
	// Invalid external registrations are rejected without an assertion.
	if (!ContextType || !IsValid(Receiver))
	{
		return false;
	}

	UFunction* Function = Receiver->FindFunction(FunctionName);
	if (!FindContextParameter(Function, ContextType))
	{
		return false;
	}

	EnsureContextBindings();
	ContextHandlers.Add(ContextType, FOWTContextHandlerBinding(Receiver, FunctionName));

	return true;
}

void UOWTObjectEditMode::SelectObject_Implementation(const FOWTSelectObjectContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->SetSelectedObject(Context.SelectedObject.Get());
}

void UOWTObjectEditMode::Undo_Implementation(const FOWTUndoContext& Context)
{
}

void UOWTObjectEditMode::Redo_Implementation(const FOWTRedoContext& Context)
{
}

void UOWTObjectEditMode::ToggleCoordinateSystem_Implementation(const FOWTToggleCoordinateSystemContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->SetCoordinateSystem(Subsystem->GetCoordinateSystem() == EToolContextCoordinateSystem::World
	                                   ? EToolContextCoordinateSystem::Local
	                                   : EToolContextCoordinateSystem::World);
}

void UOWTObjectEditMode::ToggleTransformSpline_Implementation(const FOWTToggleTransformSplineContext& Context)
{
}

void UOWTObjectEditMode::DuplicateSelection_Implementation(const FOWTDuplicateSelectionContext& Context)
{
}

void UOWTObjectEditMode::HideSelectionGizmo_Implementation(const FOWTHideSelectionGizmoContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->HideSelectionGizmo();
}

bool UOWTObjectEditMode::UnbindContextHandler(UScriptStruct* ContextType)
{
	check(IsInGameThread());

	if (!ContextType)
	{
		return false;
	}

	EnsureContextBindings();
	const int32 RemovedCount = ContextHandlers.Remove(ContextType);

	return RemovedCount > 0;
}

void UOWTObjectEditMode::SetTranslation_Implementation(const FOWTSetTranslationContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->SetTransformGizmoMode(EToolContextTransformGizmoMode::Translation);
}

void UOWTObjectEditMode::SetRotation_Implementation(const FOWTSetRotationContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->SetTransformGizmoMode(EToolContextTransformGizmoMode::Rotation);
}

void UOWTObjectEditMode::SetScale_Implementation(const FOWTSetScaleContext& Context)
{
	UVTBOWTEditorSubsystem* Subsystem = GetTypedOuter<UVTBOWTEditorSubsystem>();
	if (!ensureMsgf(Subsystem, TEXT("OWT object edit mode requires an owning editor subsystem.")))
	{
		return;
	}

	Subsystem->SetTransformGizmoMode(EToolContextTransformGizmoMode::Scale);
}

void UOWTObjectEditMode::EnsureContextBindings()
{
	// The public entry point has already checked the game thread.
	if (bContextBindingsInitialized)
	{
		return;
	}

	bContextBindingsInitialized = true;
	InitializeContextBindings();
}

FOWTResolvedContextHandler UOWTObjectEditMode::ResolveContextHandler(const UScriptStruct* ContextType) const
{
	const FOWTContextHandlerBinding* Found = ContextHandlers.Find(ContextType);
	if (!Found)
	{
		return {};
	}

	// User callbacks may rebind/remove entries, so do not keep map pointers during dispatch.
	const FOWTContextHandlerBinding Binding = *Found;
	UObject* Receiver = Binding.Receiver.Get();
	// A weak receiver expiring is an expected lifecycle event.
	if (!Receiver)
	{
		return {};
	}

	UFunction* Function = Receiver->FindFunction(Binding.FunctionName);
	FStructProperty* Parameter = FindContextParameter(Function, ContextType);
	// Registration already validated this signature. BP reinstancing may invalidate it.
	if (!ensureMsgf(Parameter, TEXT("OWT handler %s.%s no longer accepts context %s; rebind the handler."),
	                *Receiver->GetPathName(), *Binding.FunctionName.ToString(), *ContextType->GetName()))
	{
		return {};
	}

	return FOWTResolvedContextHandler(Receiver, Function, Parameter);
}

void UOWTObjectEditMode::InvokeContextHandler(const FInstancedStruct& Context,
                                              const FOWTResolvedContextHandler& Handler) const
{
	FStructOnScope Parameters(Handler.Function);
	uint8* ParameterMemory = Parameters.GetStructMemory();
	void* ContextParameter = Handler.Parameter->ContainerPtrToValuePtr<void>(ParameterMemory);

	Handler.Parameter->CopyCompleteValue(ContextParameter, Context.GetMemory());
	Handler.Receiver->ProcessEvent(Handler.Function, ParameterMemory);
}
