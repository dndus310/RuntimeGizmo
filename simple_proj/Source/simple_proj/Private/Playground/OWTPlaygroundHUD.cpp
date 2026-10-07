#include "Playground/OWTPlaygroundHUD.h"

#include "Components/InputComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/MeshComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerInput.h"
#include "HAL/PlatformTime.h"
#include "InputCoreTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/PackageName.h"
#include "Modes/OWTAttributeEditMode.h"
#include "VTBAttributeEditor.h"
#include "VTBOWTEditorPlayerController.h"
#include "VTBOWTEditorSubsystem.h"

AOWTPlaygroundHUD::AOWTPlaygroundHUD()
    : DebugBindingOverrides(), SceneCounts(), StatusMessage(), SampleStartedAt(0), LastSceneCountAt(0),
      StatusExpiresAt(0), SampleFrameCount(0), AverageFrameMilliseconds(0), FramesPerSecond(0), bHelpVisible(true),
      bInputBound(false), bMapChangePending(false)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AOWTPlaygroundHUD::BeginPlay()
{
	Super::BeginPlay();
	SampleStartedAt = FPlatformTime::Seconds();
	RefreshSceneCounts();
	InitializePlaygroundInput();
}

void AOWTPlaygroundHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!bInputBound)
	{
		InitializePlaygroundInput();
	}
	UpdateStatistics();
}

void AOWTPlaygroundHUD::DrawHUD()
{
	Super::DrawHUD();
	if (!Canvas)
	{
		return;
	}
	if (!GEngine)
	{
		return;
	}
	if (!bShowHUD)
	{
		return;
	}
	UFont* Font = GEngine->GetSmallFont();
	if (!Font)
	{
		return;
	}
	const float Margin = 16.0f;
	const float PanelWidth = FMath::Min(420.0f, Canvas->ClipX * 0.40f);
	const float TextWidth = PanelWidth - 24.0f;
	if (TextWidth < 96.0f)
	{
		return;
	}
	const float Scale = FMath::Clamp(Canvas->ClipY / 800.0f, 0.9f, 1.25f);
	float TextHeight = 0;
	float TestWidth = 0;
	GetTextSize(TEXT("Ag"), TestWidth, TextHeight, Font, Scale);
	const float LineHeight = TextHeight + 5.0f;
	TArray<FString> SourceLines;
	BuildOverlayLines(SourceLines);
	TArray<FString> Lines;
	for (const FString& Line : SourceLines)
	{
		WrapOverlayLine(Line, TextWidth, Font, Scale, Lines);
	}
	const float PanelHeight = 24.0f + LineHeight * Lines.Num();
	DrawRect(FLinearColor(0.025f, 0.045f, 0.075f, 0.90f), Margin, Margin, PanelWidth, PanelHeight);
	DrawRect(FLinearColor(0.16f, 0.82f, 0.76f, 1.0f), Margin, Margin, 3.0f, PanelHeight);
	float Y = Margin + 12.0f;
	for (int32 Index = 0; Index < Lines.Num(); ++Index)
	{
		const FLinearColor Color = Index == 0 ? FLinearColor(0.35f, 0.95f, 0.87f) : FLinearColor(0.92f, 0.95f, 1.0f);
		DrawText(Lines[Index], Color, Margin + 12.0f, Y, Font, Scale);
		Y += LineHeight;
	}
}

void AOWTPlaygroundHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RestoreDebugBindings();
	if (PlayerOwner)
	{
		DisableInput(PlayerOwner);
	}
	bInputBound = false;
	Super::EndPlay(EndPlayReason);
}

void AOWTPlaygroundHUD::InitializePlaygroundInput()
{
	if (!PlayerOwner)
	{
		return;
	}
	if (!PlayerOwner->IsLocalController())
	{
		return;
	}
	if (!PlayerOwner->PlayerInput)
	{
		return;
	}
	EnableInput(PlayerOwner);
	check(InputComponent);
	InputComponent->bBlockInput = false;
	InputComponent->BindKey(EKeys::F6, IE_Pressed, this, &ThisClass::OnShowroomPressed);
	InputComponent->BindKey(EKeys::F7, IE_Pressed, this, &ThisClass::OnStressPressed);
	InputComponent->BindKey(EKeys::F8, IE_Pressed, this, &ThisClass::ToggleHelp);
	TArray<FKeyBind>& Bindings = PlayerOwner->PlayerInput->DebugExecBindings;
	for (int32 Index = 0; Index < Bindings.Num(); ++Index)
	{
		FKeyBind& Binding = Bindings[Index];
		bool bPlaygroundKey = Binding.Key == EKeys::F6;
		if (Binding.Key == EKeys::F7)
		{
			bPlaygroundKey = true;
		}
		if (Binding.Key == EKeys::F8)
		{
			bPlaygroundKey = true;
		}
		if (bPlaygroundKey)
		{
			DebugBindingOverrides.Emplace(Index, Binding.Key.GetFName(), Binding.bDisabled != 0);
			Binding.bDisabled = true;
		}
	}
	bInputBound = true;
}

void AOWTPlaygroundHUD::RestoreDebugBindings()
{
	if (!IsValid(PlayerOwner))
	{
		return;
	}
	if (!IsValid(PlayerOwner->PlayerInput))
	{
		return;
	}
	TArray<FKeyBind>& Bindings = PlayerOwner->PlayerInput->DebugExecBindings;
	for (const FDebugBindingOverride& Previous : DebugBindingOverrides)
	{
		if (!Bindings.IsValidIndex(Previous.Index))
		{
			continue;
		}
		FKeyBind& Binding = Bindings[Previous.Index];
		if (Binding.Key.GetFName() != Previous.KeyName)
		{
			continue;
		}
		Binding.bDisabled = Previous.bWasDisabled;
	}
	DebugBindingOverrides.Reset();
}

void AOWTPlaygroundHUD::UpdateStatistics()
{
	const double Now = FPlatformTime::Seconds();
	++SampleFrameCount;
	const double Elapsed = Now - SampleStartedAt;
	if (Elapsed >= 0.5)
	{
		AverageFrameMilliseconds = static_cast<float>(Elapsed * 1000.0 / SampleFrameCount);
		FramesPerSecond = static_cast<float>(SampleFrameCount / Elapsed);
		SampleFrameCount = 0;
		SampleStartedAt = Now;
	}
	if (Now - LastSceneCountAt >= 1.0)
	{
		RefreshSceneCounts();
	}
}

void AOWTPlaygroundHUD::RefreshSceneCounts()
{
	SceneCounts = FOWTPlaygroundSceneCounts();
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		const AActor* Actor = *It;
		if (Actor->IsActorBeingDestroyed())
		{
			continue;
		}
		++SceneCounts.ActorCount;
		if (!Actor->GetParentActor())
		{
			if (Actor->ActorHasTag(TEXT("OWT.Playground.Editable")))
			{
				++SceneCounts.EditableActorCount;
			}
		}
		TInlineComponentArray<UActorComponent*> Components(Actor);
		for (const UActorComponent* Component : Components)
		{
			if (!IsValid(Component))
			{
				continue;
			}
			++SceneCounts.ComponentCount;
			if (!Component->IsA<UMeshComponent>())
			{
				continue;
			}
			++SceneCounts.MeshComponentCount;
			if (const UInstancedStaticMeshComponent* Instances = Cast<UInstancedStaticMeshComponent>(Component))
			{
				SceneCounts.MeshInstanceCount += Instances->GetInstanceCount();
			}
			else
			{
				++SceneCounts.MeshInstanceCount;
			}
		}
	}
	LastSceneCountAt = FPlatformTime::Seconds();
}

void AOWTPlaygroundHUD::BuildOverlayLines(TArray<FString>& OutLines) const
{
	const FString MapName = UGameplayStatics::GetCurrentLevelName(this, true);
	const bool bStress = MapName == TEXT("L_OWTStress");
	OutLines.Add(bStress ? TEXT("OWT / STRESS LAB") : TEXT("OWT / RUNTIME PLAYGROUND"));
	OutLines.Add(TEXT("F6 Showroom   F7 Stress   F8 Help"));
	OutLines.Add(FString::Printf(TEXT("%.0f FPS  |  %.1f ms/frame"), FramesPerSecond, AverageFrameMilliseconds));
	OutLines.Add(FString::Printf(TEXT("Editable props: %d"), SceneCounts.EditableActorCount));
	OutLines.Add(
	    FString::Printf(TEXT("World: %d actors / %d components"), SceneCounts.ActorCount, SceneCounts.ComponentCount));
	OutLines.Add(FString::Printf(TEXT("Meshes: %d components / %d instances"), SceneCounts.MeshComponentCount,
	                             SceneCounts.MeshInstanceCount));
	if (bHelpVisible)
	{
		OutLines.Add(TEXT("Click a prop to select it"));
		OutLines.Add(TEXT("F2 Edit ON/OFF   F3 Events"));
		OutLines.Add(TEXT("W Move   E Rotate   R Scale"));
		OutLines.Add(TEXT("Ctrl+D Duplicate (+X 100 cm)"));
		OutLines.Add(TEXT("RMB + WASD / mouse: fly"));
		OutLines.Add(TEXT("Details: type or drag a value"));
		OutLines.Add(TEXT("Esc cancels a value drag"));
	}
	if (FPlatformTime::Seconds() < StatusExpiresAt)
	{
		OutLines.Add(StatusMessage);
	}
}

void AOWTPlaygroundHUD::WrapOverlayLine(const FString& Text, float Width, UFont* Font, float Scale,
                                        TArray<FString>& OutLines) const
{
	TArray<FString> Words;
	Text.ParseIntoArrayWS(Words);
	FString Line;
	for (const FString& Word : Words)
	{
		const FString Candidate = Line.IsEmpty() ? Word : Line + TEXT(" ") + Word;
		float MeasuredWidth = 0;
		float MeasuredHeight = 0;
		GetTextSize(Candidate, MeasuredWidth, MeasuredHeight, Font, Scale);
		if (MeasuredWidth > Width)
		{
			if (!Line.IsEmpty())
			{
				OutLines.Add(Line);
				Line = Word;
				continue;
			}
		}
		Line = Candidate;
	}
	if (!Line.IsEmpty())
	{
		OutLines.Add(Line);
	}
}

bool AOWTPlaygroundHUD::CanChangePlaygroundMap(FString& OutReason) const
{
	OutReason.Reset();
	if (bMapChangePending)
	{
		OutReason = TEXT("A map is already loading.");
		return false;
	}
	const UWorld* World = GetWorld();
	if (!World)
	{
		OutReason = TEXT("The playground world is unavailable.");
		return false;
	}
	if (World->bIsTearingDown)
	{
		OutReason = TEXT("The playground world is closing.");
		return false;
	}
	const AVTBOWTEditorPlayerController* Controller = Cast<AVTBOWTEditorPlayerController>(PlayerOwner);
	if (!Controller)
	{
		OutReason = TEXT("The playground controller is unavailable.");
		return false;
	}
	if (Controller->IsAttributeUIBlockingInput())
	{
		OutReason = TEXT("Finish field editing and move the pointer off the sidebar before changing maps.");
		return false;
	}
	if (Controller->IsInputKeyDown(EKeys::LeftMouseButton))
	{
		OutReason = TEXT("Release the mouse before changing maps.");
		return false;
	}
	if (Controller->IsInputKeyDown(EKeys::RightMouseButton))
	{
		OutReason = TEXT("Release the camera navigation button before changing maps.");
		return false;
	}
	const UVTBOWTEditorSubsystem* Hub = World->GetSubsystem<UVTBOWTEditorSubsystem>();
	if (!Hub)
	{
		OutReason = TEXT("The editing session is unavailable.");
		return false;
	}
	if (Hub->HasGizmoCapture())
	{
		OutReason = TEXT("Finish the gizmo drag before changing maps.");
		return false;
	}
	if (const AVTBAttributeEditor* Editor = Hub->GetAttributeEditor())
	{
		if (Editor->GetSnapshot().bIsModifying)
		{
			OutReason = TEXT("Finish the current edit before changing maps.");
			return false;
		}
	}
	if (const UOWTAttributeEditMode* Mode = Hub->GetAttributeEditMode())
	{
		if (Mode->HasPendingDuplicate())
		{
			OutReason = TEXT("Wait for the duplicate operation before changing maps.");
			return false;
		}
	}
	return true;
}

bool AOWTPlaygroundHUD::RequestPlaygroundMap(bool bStressMap)
{
	FString Reason;
	if (!CanChangePlaygroundMap(Reason))
	{
		ShowStatus(Reason);
		return false;
	}
	const FName Destination = bStressMap ? GetStressMapName() : GetShowroomMapName();
	const FString CurrentMap = UGameplayStatics::GetCurrentLevelName(this, true);
	if (CurrentMap == FPackageName::GetShortName(Destination.ToString()))
	{
		ShowStatus(TEXT("You are already in this map."));
		return false;
	}
	if (!FPackageName::DoesPackageExist(Destination.ToString()))
	{
		ShowStatus(TEXT("This playground map is missing from the build."));
		return false;
	}
	bMapChangePending = true;
	ShowStatus(bStressMap ? TEXT("Loading Stress Lab...") : TEXT("Loading Showroom..."));
	UGameplayStatics::OpenLevel(this, Destination);
	return true;
}

void AOWTPlaygroundHUD::ShowStatus(const FString& Message)
{
	StatusMessage = Message;
	StatusExpiresAt = FPlatformTime::Seconds() + 4.0;
}

void AOWTPlaygroundHUD::OnShowroomPressed()
{
	RequestPlaygroundMap(false);
}

void AOWTPlaygroundHUD::OnStressPressed()
{
	RequestPlaygroundMap(true);
}

void AOWTPlaygroundHUD::ToggleHelp()
{
	bHelpVisible = !bHelpVisible;
}

bool AOWTPlaygroundHUD::IsHelpVisible() const
{
	return bHelpVisible;
}

bool AOWTPlaygroundHUD::IsMapChangePending() const
{
	return bMapChangePending;
}

FOWTPlaygroundSceneCounts AOWTPlaygroundHUD::GetSceneCounts() const
{
	return SceneCounts;
}

float AOWTPlaygroundHUD::GetAverageFrameMilliseconds() const
{
	return AverageFrameMilliseconds;
}

float AOWTPlaygroundHUD::GetFramesPerSecond() const
{
	return FramesPerSecond;
}

FName AOWTPlaygroundHUD::GetShowroomMapName()
{
	return TEXT("/Game/OWTPlayground/Maps/L_OWTPlayground");
}

FName AOWTPlaygroundHUD::GetStressMapName()
{
	return TEXT("/Game/OWTPlayground/Maps/L_OWTStress");
}
