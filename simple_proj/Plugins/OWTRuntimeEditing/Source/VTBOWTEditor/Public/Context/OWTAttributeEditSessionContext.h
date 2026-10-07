#pragma once

#include "CoreMinimal.h"
#include "OWTAttributeEditSessionContext.generated.h"

class UOWTAttributeEditMode;

UCLASS(Transient)
class VTBOWTEDITOR_API UOWTAttributeEditSessionContext : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(UOWTAttributeEditMode& InMode);
	UOWTAttributeEditMode* GetMode() const;

private:
	UPROPERTY(Transient)
	TWeakObjectPtr<UOWTAttributeEditMode> Mode;
};
