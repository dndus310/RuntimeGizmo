#include "Duplication/OWTDuplicationAdapterProvider.h"

IOWTDuplicationAdapterProvider::~IOWTDuplicationAdapterProvider() = default;

FName IOWTDuplicationAdapterProvider::GetFeatureName()
{
	return TEXT("OWT.DuplicationAdapterProvider");
}
