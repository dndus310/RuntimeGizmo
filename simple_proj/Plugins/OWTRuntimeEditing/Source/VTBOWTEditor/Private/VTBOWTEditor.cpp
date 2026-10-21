#include "VTBOWTEditor.h"

#include "Duplication/Engine/OWTEngineDuplicationProvider.h"
#include "Duplication/OWTDuplicationAdapterProvider.h"
#include "Duplication/PCG/OWTPCGDuplicationProvider.h"
#include "Features/IModularFeatures.h"

#define LOCTEXT_NAMESPACE "FVTBOWTEditorModule"

FVTBOWTEditorModule::FVTBOWTEditorModule() = default;

FVTBOWTEditorModule::~FVTBOWTEditorModule() = default;

void FVTBOWTEditorModule::StartupModule()
{
	EngineDuplicationProvider = MakeUnique<FOWTEngineDuplicationProvider>();
	PCGDuplicationProvider = MakeUnique<FOWTPCGDuplicationProvider>();
	IModularFeatures& Features = IModularFeatures::Get();
	Features.RegisterModularFeature(IOWTDuplicationAdapterProvider::GetFeatureName(), EngineDuplicationProvider.Get());
	Features.RegisterModularFeature(IOWTDuplicationAdapterProvider::GetFeatureName(), PCGDuplicationProvider.Get());
}

void FVTBOWTEditorModule::ShutdownModule()
{
	IModularFeatures& Features = IModularFeatures::Get();
	if (PCGDuplicationProvider)
	{
		Features.UnregisterModularFeature(IOWTDuplicationAdapterProvider::GetFeatureName(),
		                                  PCGDuplicationProvider.Get());
		PCGDuplicationProvider.Reset();
	}
	if (EngineDuplicationProvider)
	{
		Features.UnregisterModularFeature(IOWTDuplicationAdapterProvider::GetFeatureName(),
		                                  EngineDuplicationProvider.Get());
		EngineDuplicationProvider.Reset();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FVTBOWTEditorModule, VTBOWTEditor)
