#pragma once

#include "CoreMinimal.h"
#include "Rendering/OWTRuntimeToolsHUD.h"
#include "OWTPlaygroundHUD.generated.h"

class AVTBOWTEditorPlayerController;
class UFont;

USTRUCT(BlueprintType)
struct SIMPLE_PROJ_API FOWTPlaygroundSceneCounts
{
	GENERATED_BODY()

	FOWTPlaygroundSceneCounts()
	    : ActorCount(0), EditableActorCount(0), ComponentCount(0), MeshComponentCount(0), MeshInstanceCount(0)
	{
	}

	UPROPERTY(BlueprintReadOnly, Category = "OWT|Playground")
	int32 ActorCount;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Playground")
	int32 EditableActorCount;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Playground")
	int32 ComponentCount;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Playground")
	int32 MeshComponentCount;
	UPROPERTY(BlueprintReadOnly, Category = "OWT|Playground")
	int32 MeshInstanceCount;
};

/** Canvas help and sampled world statistics without covering the editor sidebar. */
UCLASS()
class SIMPLE_PROJ_API AOWTPlaygroundHUD : public AOWTRuntimeToolsHUD
{
	GENERATED_BODY()

public:
	AOWTPlaygroundHUD();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void DrawHUD() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "OWT|Playground")
	bool RequestPlaygroundMap(bool bStressMap);

	UFUNCTION(BlueprintCallable, Category = "OWT|Playground")
	void ToggleHelp();

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	bool CanChangePlaygroundMap(FString& OutReason) const;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	bool IsHelpVisible() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	bool IsMapChangePending() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	FOWTPlaygroundSceneCounts GetSceneCounts() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	float GetAverageFrameMilliseconds() const;

	UFUNCTION(BlueprintPure, Category = "OWT|Playground")
	float GetFramesPerSecond() const;

	static FName GetShowroomMapName();
	static FName GetStressMapName();

private:
	void InitializePlaygroundInput();
	void RestoreDebugBindings();
	void UpdateStatistics();
	void RefreshSceneCounts();
	void BuildOverlayLines(TArray<FString>& OutLines) const;
	void WrapOverlayLine(const FString& Text, float Width, UFont* Font, float Scale, TArray<FString>& OutLines) const;
	void ShowStatus(const FString& Message);
	void OnShowroomPressed();
	void OnStressPressed();

private:
	struct FDebugBindingOverride
	{
		FDebugBindingOverride(int32 InIndex, FName InKeyName, bool bInWasDisabled)
		    : Index(InIndex), KeyName(InKeyName), bWasDisabled(bInWasDisabled)
		{
		}
		int32 Index;
		FName KeyName;
		bool bWasDisabled;
	};

	TArray<FDebugBindingOverride> DebugBindingOverrides;
	FOWTPlaygroundSceneCounts SceneCounts;
	FString StatusMessage;
	double SampleStartedAt;
	double LastSceneCountAt;
	double StatusExpiresAt;
	int32 SampleFrameCount;
	float AverageFrameMilliseconds;
	float FramesPerSecond;
	bool bHelpVisible;
	bool bInputBound;
	bool bMapChangePending;
};
