#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class IOWTDuplicationAdapterProvider;

class FVTBOWTEditorModule : public IModuleInterface
{
public:
	FVTBOWTEditorModule();

	virtual ~FVTBOWTEditorModule() override;

	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

private:
	TUniquePtr<IOWTDuplicationAdapterProvider> EngineDuplicationProvider;
	TUniquePtr<IOWTDuplicationAdapterProvider> PCGDuplicationProvider;
};
