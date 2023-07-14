//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "../Importer/FRealBandAssetImporter.h"
#include "Misc/AutomationTest.h"
#include "ContentBrowserModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "IContentBrowserDataModule.h"
#include "ContentBrowserCommands.h"
#include "ContentBrowserUtils.h"
#include "IContentBrowserSingleton.h"
#include "Settings/ContentBrowserSettings.h"

//Collections
#include "CollectionManagerTypes.h"
#include "CollectionManagerModule.h"
#include "ICollectionManager.h"
//Assets
#include "AssetRegistryModule.h"
#include "AssetViewUtils.h"
#include "ObjectTools.h"
using namespace  ObjectTools;
//filesystem
#include <filesystem>
namespace fs = std::filesystem;

#define LOCTEXT_NAMESPACE "FRealBandAssetImporterTests"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FRealBandAssetImporterTest, "RealBandGroup.AssetImporter.Placeholder Test", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)


bool FRealBandAssetImporterTest::RunTest(const FString& Parameters)
{
    {
        TSharedPtr<FRealBandAssetImporter> FRealBandAsstImp = MakeShareable(new FRealBandAssetImporter);
        TestNotNull(TEXT("ContentBrowserData"), FRealBandAsstImp.Get());
        FRealBandAsstImp->Init();

        TArray<FName> PackagePaths;
        PackagePaths.Add(FName("/Engine/RealBand/Textures/Prespective"));
        bool bAdded = false;
        TArray<FAssetData> AssetsInPackages;
        FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
        IAssetRegistry* AssetRegistry = &AssetRegistryModule.Get();
        FARFilter Filter;
        Filter.bIncludeOnlyOnDiskAssets = false;
        Filter.PackagePaths = PackagePaths;
        TArray<FAssetData> AsstInPackages;
        AssetRegistry->GetAssets(Filter, AsstInPackages);
        TArray<FString> OutPathList;
        AssetRegistry->GetSubPaths(FString("/Engine/RealBand"), OutPathList, true);
        bool bFolderExists = false;
        int iCmp = -1;
        for (const FString packageFolder : OutPathList)
        {
            iCmp = packageFolder.Compare(FString(TEXT("/Engine/RealBand/Textures/Prespective")));
            if (!iCmp)
                bFolderExists = true;
        }
        TestTrue(TEXT("Prespective Folder Exists"),bFolderExists == true);

        bool bClassName = false;
        FAssetPickerConfig ConfigPicker;
        FRealBandAsstImp->GetAssetConfig(ConfigPicker);
        TArray<FName> AssetClasses = ConfigPicker.Filter.ClassNames;
        if (AssetClasses.Num() > 0)
        {
            if (AssetClasses[0] == "Texture2D")
                bClassName = true;
        }
        TestTrue(TEXT("AssetViewer Config ClassName"), bClassName == true);

        FString projectDir = FPackageName::FilenameToLongPackageName(FPaths::ProjectContentDir());
        FString ProjectTestDir = FPaths::ProjectPluginsDir() / TEXT("RealBandImporter") / TEXT("Source")
                                                            / TEXT("RealBandImporter") / TEXT("Private") / TEXT("Tests") / TEXT("TestData") / TEXT("Zero");
        FRealBandAsstImp->CreateTexturesFromAssets(FText::FromString(ProjectTestDir));
        AssetRegistry->GetAssets(Filter, AsstInPackages);
        TestTrue(TEXT("Empty Texture Folder"), AsstInPackages.Num() == 0);

        ProjectTestDir = FPaths::ProjectPluginsDir() / TEXT("RealBandImporter") / TEXT("Source")
            / TEXT("RealBandImporter") / TEXT("Private") / TEXT("Tests") / TEXT("TestData") / TEXT("Textures");
        FRealBandAsstImp->CreateTexturesFromAssets(FText::FromString(ProjectTestDir));
        AssetRegistry->GetAssets(Filter, AsstInPackages);
        TestTrue(TEXT("Prespective Texture Created"), AsstInPackages.Num() == 1);

     
        ProjectTestDir = FPaths::ProjectPluginsDir() / TEXT("RealBandImporter") / TEXT("Source")
            / TEXT("RealBandImporter") / TEXT("Private") / TEXT("Tests") / TEXT("TestData") 
            / TEXT("매우 긴 이름 매우 긴 이름 매우");

        if (FName(ProjectTestDir).GetDisplayNameEntry()->IsWide())
        {
            WIDECHAR NewTextureNameString[NAME_SIZE] = { 0 };
            FName ConvertedFString = FName(ProjectTestDir);
            ConvertedFString.GetPlainWIDEString(NewTextureNameString);
            wstring FolderPath = StringCast<WIDECHAR>(NewTextureNameString).Get();
            FText AssetFolderPath = FText::FromString(FolderPath.c_str());
            FRealBandAsstImp->CreateTexturesFromAssets(AssetFolderPath);
            AssetRegistry->GetAssets(Filter, AsstInPackages);
            TestTrue(TEXT("Prespective Texture Created"), AsstInPackages.Num() == 1);
        }

        // Try a system or a hidden folder
        ProjectTestDir = FPaths::ProjectPluginsDir() / TEXT("RealBandImporter") / TEXT("Source")
            / TEXT("RealBandImporter") / TEXT("Private") / TEXT("Tests") / TEXT("TestData")
            / TEXT("매우 긴 이름 매우 긴 이름 매우");


    }
    // Check if we can enable the Engine Data folder in the content browser
    {
        //GetMutableDefault<UContentBrowserSettings>()->SetDisplayEngineFolder(true, true);
        bool bEngineFolder = GetMutableDefault<UContentBrowserSettings>()->GetDisplayEngineFolder();
        TestTrue(TEXT("Enable Engine Folder in ContenBrowser is enabled"),
            bEngineFolder == true);
    }

    {
        UContentBrowserDataSubsystem* ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
        TestNotNull(TEXT("ContentBrowserData"), ContentBrowserData);
    }
    
    //check(bEngineFolder==true);
    return true;
}

#undef LOCTEXT_NAMESPACE