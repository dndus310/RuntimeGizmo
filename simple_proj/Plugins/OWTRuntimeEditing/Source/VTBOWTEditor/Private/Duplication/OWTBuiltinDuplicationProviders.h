#pragma once

#include "CoreMinimal.h"

class IOWTDuplicationAdapterProvider;

TUniquePtr<IOWTDuplicationAdapterProvider> CreateOWTEngineDuplicationProvider();

TUniquePtr<IOWTDuplicationAdapterProvider> CreateOWTPCGDuplicationProvider();
