// Copyright 2022, Aquanox.

#include "Model/SubsystemBrowserCategory.h"

#include "SubsystemBrowserModule.h"
#include "SubsystemBrowserFlags.h"
#include "Subsystems/EngineSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "EditorSubsystem.h"

#define LOCTEXT_NAMESPACE "SubsystemBrowser"

namespace SubsystemCategoryHelpers
{
	void SelectEngineSubsystems(UWorld* InContext, TArray<UObject*>& OutData);
	void SelectEditorSubsystems(UWorld* InContext, TArray<UObject*>& OutData);
	void SelectGameInstanceSubsystems(UWorld* InContext, TArray<UObject*>& OutData);
	void SelectWorldSubsystems(UWorld* InContext, TArray<UObject*>& OutData);
	void SelectPlayerSubsystems(UWorld* InContext, TArray<UObject*>& OutData);
}

void FSubsystemBrowserModule::RegisterDefaultCategories()
{
	auto CategoryEngine = MakeShared<FSimpleSubsystemCategory>();
	CategoryEngine->Name = TEXT("EngineSubsystemCategory");
	CategoryEngine->Label = LOCTEXT("SubsystemBrowser_Engine", "Engine Subsystems");
	CategoryEngine->Selector = FEnumSubsystemsDelegate::CreateStatic(&SubsystemCategoryHelpers::SelectEngineSubsystems);
	CategoryEngine->SortOrder = 100;
	RegisterCategory(CategoryEngine);

	auto CategoryEditor = MakeShared<FSimpleSubsystemCategory>();
	CategoryEditor->Name = TEXT("EditorSubsystemCategory");
	CategoryEditor->Label = LOCTEXT("SubsystemBrowser_Editor", "Editor Subsystems");
	CategoryEditor->Selector = FEnumSubsystemsDelegate::CreateStatic(&SubsystemCategoryHelpers::SelectEditorSubsystems);
	CategoryEditor->SortOrder = 200;
	RegisterCategory(CategoryEditor);

	auto CategoryGameInstance = MakeShared<FSimpleSubsystemCategory>();
	CategoryGameInstance->Name = TEXT("GameInstanceCategory");
	CategoryGameInstance->Label = LOCTEXT("SubsystemBrowser_GameInstance", "Game Instance Subsystems");
	CategoryGameInstance->Selector = FEnumSubsystemsDelegate::CreateStatic(&SubsystemCategoryHelpers::SelectGameInstanceSubsystems);
	CategoryGameInstance->SortOrder = 300;
	RegisterCategory(CategoryGameInstance);

	auto CategoryWorld = MakeShared<FSimpleSubsystemCategory>();
	CategoryWorld->Name = TEXT("WorldSubsystemCategory");
	CategoryWorld->Label = LOCTEXT("SubsystemBrowser_World", "World Subsystems");
	CategoryWorld->Selector = FEnumSubsystemsDelegate::CreateStatic(&SubsystemCategoryHelpers::SelectWorldSubsystems);
	CategoryWorld->SortOrder = 400;
	RegisterCategory(CategoryWorld);

	auto CategoryPlayer = MakeShared<FSimpleSubsystemCategory>();
	CategoryPlayer->Name = TEXT("PlayerCategory");
	CategoryPlayer->Label = LOCTEXT("SubsystemBrowser_Player", "Player Subsystems");
	CategoryPlayer->Selector = FEnumSubsystemsDelegate::CreateStatic(&SubsystemCategoryHelpers::SelectPlayerSubsystems);
	CategoryPlayer->SortOrder = 500;
	RegisterCategory(CategoryPlayer);
}

void SubsystemCategoryHelpers::SelectEngineSubsystems(UWorld* CurrentWorld, TArray<UObject*>& OutData)
{
	if (GEngine)
	{
		OutData.Append(GEngine->GetEngineSubsystemArray<UEngineSubsystem>());
	}
}

void SubsystemCategoryHelpers::SelectEditorSubsystems(UWorld* CurrentWorld, TArray<UObject*>& OutData)
{
	if (GEditor)
	{
		OutData.Append(GEditor->GetEditorSubsystemArray<UEditorSubsystem>());
	}
}

void SubsystemCategoryHelpers::SelectGameInstanceSubsystems(UWorld* CurrentWorld, TArray<UObject*>& OutData)
{
	if (IsValid(CurrentWorld) && CurrentWorld->GetGameInstance())
	{
		OutData.Append(CurrentWorld->GetGameInstance()->GetSubsystemArray<UGameInstanceSubsystem>());
	}
}

void SubsystemCategoryHelpers::SelectWorldSubsystems(UWorld* CurrentWorld, TArray<UObject*>& OutData)
{
	if (IsValid(CurrentWorld))
	{
		OutData.Append(CurrentWorld->GetSubsystemArray<UWorldSubsystem>());
	}
}

void SubsystemCategoryHelpers::SelectPlayerSubsystems(UWorld* CurrentWorld, TArray<UObject*>& OutData)
{
	if (IsValid(CurrentWorld) && CurrentWorld->GetGameInstance())
	{
		for (ULocalPlayer* const LocalPlayer : CurrentWorld->GetGameInstance()->GetLocalPlayers())
		{
			OutData.Append(LocalPlayer->GetSubsystemArray<ULocalPlayerSubsystem>());
		}
	}
}

FSubsystemCategory::FSubsystemCategory(const FName& Name, const FText& Label, int32 SortOrder)
	: Name(Name), Label(Label), SortOrder(SortOrder)
{
}

FSimpleSubsystemCategory::FSimpleSubsystemCategory(const FName& Name, const FText& Label, const FEnumSubsystemsDelegate& Selector, int32 SortOrder)
	: FSubsystemCategory(Name, Label, SortOrder), Selector(Selector)
{
	ensure(Selector.IsBound());
}

TArray<UObject*> FSimpleSubsystemCategory::Select(UWorld* InContext) const
{
	TArray<UObject*> OutResult;
	Selector.Execute(InContext, OutResult);
	return OutResult;
}

#undef LOCTEXT_NAMESPACE

#if ENABLE_SUBSYSTEM_BROWSER_EXAMPLES

// 2. Call this in your editor modules StartupModule to register a new category
void RegisterCategoryExample()
{
	// Get a reference to Subsystem Browser module instance or load it
	FSubsystemBrowserModule& Module = FModuleManager::LoadModuleChecked<FSubsystemBrowserModule>(TEXT("SubsystemBrowser"));
	// Construct category
	auto SampleCategory = MakeShared<FSimpleSubsystemCategory>();
	SampleCategory->Name = TEXT("Sample");
	SampleCategory->Label = INVTEXT("Sample Subsystems");
	SampleCategory->SortOrder = 50;
	SampleCategory->Selector = FEnumSubsystemsDelegate::CreateLambda([](UWorld* InContext, TArray<UObject*>& OutData)
	{
		// Fill Data
	});
	// Register category in module
	Module.RegisterCategory(SampleCategory);
}

#endif

