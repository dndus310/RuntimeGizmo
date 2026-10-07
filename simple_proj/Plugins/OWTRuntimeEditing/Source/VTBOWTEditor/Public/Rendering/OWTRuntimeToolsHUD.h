#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "OWTRuntimeToolsHUD.generated.h"

// Other projects can derive their HUD from this class, or call Mode.RenderTools from their own HUD.
UCLASS()
class VTBOWTEDITOR_API AOWTRuntimeToolsHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
};
