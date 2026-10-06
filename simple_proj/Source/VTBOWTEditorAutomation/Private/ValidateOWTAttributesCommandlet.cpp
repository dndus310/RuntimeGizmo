#include "ValidateOWTAttributesCommandlet.h"
#include "OWTDuplicationValidation.h"

UValidateOWTAttributesCommandlet::UValidateOWTAttributesCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UValidateOWTAttributesCommandlet::Main(const FString& Params)
{
	const bool bPassed = ValidateOWTRuntimeDuplication();
	return bPassed ? 0 : 1;
}
