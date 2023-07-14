// Copyright RealEye, Inc. All Rights Reserved.

//UI
#include "UI/FRealBandUIManager.h"
#include "UI/FRealBandStyle.h"
#include "UI/FRealBandAssetLoader.h"
#include "UI/FRealBandMessageHandler.h"
// Settings
#include "Settings/RealBandImportSettings.h"
//Importers
#include "Importer/FRealBandAssetImporter.h"
//Widgets
#include "Widgets/SWindow.h"
#include "Widgets/SWidget.h"
#include "Widgets/Layout/SConstraintCanvas.h"
#include "Widgets/Layout/SSeparator.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SWidgetSwitcher.h"
#include "Widgets/Images/SSpinningImage.h"
#include "Layout/ChildrenBase.h"
#include "Widgets/Layout/SGridPanel.h"
#include "NodeFactory.h"
#include "Brushes/SlateBoxBrush.h"

#include "ImageUtils.h"
#include "Brushes/SlateImageBrush.h"
#include "Styling/SlateStyle.h"
#include "Styling/StyleColors.h"

//List of Project files
#include "Interfaces/IProjectManager.h"
#include "EditorDirectories.h"
//File picker includes
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"

// WebBrowser
#include "SWebBrowser.h"
#include "WebBrowserModule.h"
#include "IWebBrowserSingleton.h"
#include "IWebBrowserCookieManager.h"
// Json
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"

//AssetView
#include "AssetRegistryModule.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"
#include "SAssetView.h"

#include "ExternalSourceModule.h"
#include "ExternalSource.h"
#include "SourceUri.h"
// ContentBrowser
#include "ContentBrowserModule.h"
#include "ContentBrowserItem.h"

//Editor
#include "LevelEditor.h"
#include "PropertyEditor/Private/SDetailCategoryTableRow.h"

//Collections
#include "CollectionManagerTypes.h"
#include "CollectionManagerModule.h"
#include "ICollectionManager.h"
//#include "AssetManagerEditorModule.h"
//core
#include "CoreGlobals.h"


#include "Styling/SlateStyleMacros.h"

//Tools Menus
#include "ToolMenus.h"
#include "ToolMenuSection.h"

#include "Misc/FileHelper.h"


//#include "RealBandImportSettings.h"
//uint16 FRealBandUIManagerImpl::ActiveTypeBitset = TNumericLimits<uint16>::Max();
//#include "Cascade/Private/CascadeActions.h"


DEFINE_LOG_CATEGORY(LogManager);

#define LEVELEDITOR_MODULE_NAME TEXT("LevelEditor")
TSharedPtr<FRealBandUIManagerImpl> FRealBandUIManager::Instance;
const FName RealBandTabName = "RealBandTab";

#define LOCTEXT_NAMESPACE "RealBand"


FRealBandUIManagerImpl::FRealBandUIManagerImpl(TSharedPtr<FRealBandAssetImporter> inFRealBandAssetImporter):
	                    pFRealBandAssetImporter(inFRealBandAssetImporter)
{
//	ObjUserPreference.ActiveTypeBitset = TNumericLimits<uint16>::Max();
//	ObjUserPreference.ActiveTextypeBitset = TNumericLimits<uint16>::Max();
	ObjUserPreference.Format = TNumericLimits<uint8>::Max();
}

FRealBandUIManager::~FRealBandUIManager()
{
	Instance.Reset();
}


FRealBandUIManagerImpl::~FRealBandUIManagerImpl()
{
	pCanvas.Reset();
	pOverlay.Reset();
	//pFRealBandAssetLoader.Reset();
	//pSettings.Reset();
	//pImport.Reset();
	//pRealBandImportSettings.Reset();
	//pApplyButton.Reset();
	FRealBandUIManager::Instance.Reset();
	ObjUserPreference.ActiveTypeBitset = TNumericLimits<uint16>::Max();
	ObjUserPreference.ActiveTextypeBitset = TNumericLimits<uint16>::Max();
//	
}
void FRealBandUIManagerImpl::Initialize()
{
	// Lists all the projects in Unreal Engine ...
	// Find all the engine installations
	//TMap<FString, FString> EngineInstallations;
	//FDesktopPlatformModule::Get()->EnumerateEngineInstallations(EngineInstallations);
	//TSet<FString> AllFoundProjectFiles;

	//for (TMap<FString, FString>::TConstIterator Iter(EngineInstallations); Iter; ++Iter)
	//{
	//	TArray<FString> ProjectFiles;

	//	if (FDesktopPlatformModule::Get()->EnumerateProjectsKnownByEngine(Iter.Key(), false, ProjectFiles))
	//	{
	//		AllFoundProjectFiles.Append(MoveTemp(ProjectFiles));
	//	}
	//}
	//for (FString& ProjectFile : AllFoundProjectFiles)
	//{
	//	FString ProjectFilename = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*ProjectFile);

	//	if (FPaths::FileExists(ProjectFilename))
	//	{
	//		FProjectStatus ProjectStatus;
	//		if (IProjectManager::Get().QueryStatusForProject(ProjectFilename, ProjectStatus))
	//		{
	//			// @todo localized project name
	//			const FText ProjectName = FText::FromString(ProjectStatus.Name);
	//			Array_Resolutions.Add(MakeShareable<FString>(new FString(ProjectStatus.Name)));
	//			
	//		}
	//		
	//	}
	//}

	// Menu Setup
	SetupMenuItem();
	//Create the MainWindow
	CreateWindow();
}



void FRealBandUIManager::Initialize(TSharedPtr<FRealBandAssetImporter> iFRealBandAssetImporter)
{
	
	if (!Instance.IsValid())
	{
		FRealBandStyle::Initialize();
		Instance = MakeShareable(new FRealBandUIManagerImpl(iFRealBandAssetImporter));
		FRealBandStyle::SetSVGIcon("SaveIcon", "saveIcon");
		//FRealBandStyle::SetIcon("Icon8", "IconsB");
		FRealBandStyle::SetIcon("Icon8", "Icon40x40");
	}
	
	Instance->Initialize();
}

bool FRealBandUIManager::DisplayStartWindow()
{
	bool bRet = false;
	if (Instance)
	{
		bRet = Instance->RestoreMainWindow();
	}
	return bRet;
}


void FRealBandUIManagerImpl::SetupMenuItem()
{
	FRealBandStyle::SetIcon("Logo", "Logo80x80");
	FRealBandStyle::SetIcon("ContextLogo", "Logo32x32");
	//FRealBandStyle::SetSVGIcon("MenuLogo", "QuixelBridgeB");
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>(LEVELEDITOR_MODULE_NAME);
	TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
	ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, nullptr, FToolBarExtensionDelegate::CreateRaw(this, &FRealBandUIManagerImpl::FillToolbar));
	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);

	// Adding Bridge entry to Quick Content menu.
	UToolMenu* AddMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.AddQuickMenu");
	FToolMenuSection& Section = AddMenu->FindOrAddSection("Content");
	Section.AddMenuEntry("OpenRealBand",
		LOCTEXT("OpenRealBand_Label", "RealBandImporter"),
		LOCTEXT("OpenRealBand_Desc", "Opens the RealBand Importer."),
		FSlateIcon(FName("RealBandImporterStyle"), "RealBand.MenuLogo"),
		FUIAction(FExecuteAction::CreateRaw(this, &FRealBandUIManagerImpl::CreateWindow), FCanExecuteAction())
	).InsertPosition = FToolMenuInsert("ImportContent", EToolMenuInsertType::After);
}

void FRealBandUIManagerImpl::FillToolbar(FToolBarBuilder& ToolbarBuilder)
{
	ToolbarBuilder.BeginSection(TEXT("RealBandImporter"));
	{
		ToolbarBuilder.AddToolBarButton(
			FUIAction(FExecuteAction::CreateRaw(this, &FRealBandUIManagerImpl::CreateWindow)),
			FName(TEXT("RealBand Importer")),
			LOCTEXT("QMSLiveLink_label", "RealBandImporter"),
			LOCTEXT("WorldProperties_ToolTipOverride", "Megascans Link"),
			FSlateIcon(FRealBandStyle::GetStyleSetName(), "RealBand.Logo"),
			EUserInterfaceActionType::Button,
			FName(TEXT("RealBandImporter"))
		);
	}
	ToolbarBuilder.EndSection();
}

bool FRealBandUIManagerImpl::RestoreMainWindow()
{
	if (pDialogMainWindow)
	{
		pDialogMainWindow->BringToFront(true);
		return true;
	//	pDialogMainWindow->Restore();
	//	pDialogMainWindow->SetVisibility(EVisibility::Visible);
	}
	return false;
}

void FRealBandUIManagerImpl::CreateWindow()
{
	bool bIsVisible = false;

	if (!pDialogMainWindow )
	{
		/*FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RealBandTabName,
			FOnSpawnTab::CreateRaw(this, &FRealBandUIManagerImpl::CreatRealBandTab))
			.SetDisplayName(RealBandTabDisplay)
			.SetAutoGenerateMenuEntry(false)
			.SetTooltipText(RealBandToolTip);
		*/	


		FWebBrowserInitSettings browserInitSettings = FWebBrowserInitSettings();
		
		IWebBrowserModule::Get().CustomInitialize(browserInitSettings);
		WindowSettings.InitialURL = "https://realeye.co.kr/";
		WindowSettings.BrowserFrameRate = 60;
		
		
		if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
		{
			IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
			Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
			PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser)
				.ShowAddressBar(false)
				.ShowControls(false)
				.ShowErrorMessage(true)
				.OnLoadCompleted(FSimpleDelegate::CreateLambda([this]()
					{
					//	pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
						// Source\RealBandImporter\Private\UI
						FString FilePath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("RealBandImporter"), TEXT("Source"),
							TEXT("RealBandImporter"), TEXT("Private"), TEXT("UI"), TEXT("Translate.js"));
					

						TArray<uint8> FileData;
						if (FFileHelper::LoadFileToArray(FileData, *FilePath))
						{
							pImport->SetVisibility(EVisibility::Collapsed);
							
							FString FileContents;
							FFileHelper::BufferToString(FileContents, FileData.GetData(), FileData.Num());
						   // Browser->ExecuteJavascript(FileContents);

						}
						
					})
				);
				
			
			//	.OnUrlChanged(this, &FRealBandUIManagerImpl::HandleBrowserUrlChanged);
			    
		}


		// Get the AssetConfig from FRealBandAssetImporter
		FAssetPickerConfig ConfigPicker;
		pFRealBandAssetImporter->GetAssetConfig(ConfigPicker);

	//	pBrowserTab = CreatRealBandTab();

	//	SelectedProject = (*Array_Resolutions.begin());
		pDialogMainWindow = SNew(SWindow)
			.Title(FText::FromString("RealBand"))
			.ClientSize(FVector2D(1100, 800))
			.SupportsMaximize(false)
			.SupportsMinimize(false)
			.SizingRule(ESizingRule::UserSized)
			.AutoCenter(EAutoCenter::PreferredWorkArea)
			.SupportsMaximize(true)
			.SupportsMinimize(true)
		    
			
			
			//.ShouldPreserveAspectRatio(true)
			//.IsTopmostWindow(true)
			[
				
				SAssignNew(pOverlay, SOverlay)
			+ SOverlay::Slot()
			.VAlign(VAlign_Fill)
			.HAlign(HAlign_Fill)
			[
				SNew(SBox)
				.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			//.HeightOverride(750)
            //.WidthOverride(1000)
			
			[
				SAssignNew(pCanvas, SCanvas)
				
			+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(30.0f, 80.0f))
			[
				SNew(SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Local"))
			.OnClicked(this, &FRealBandUIManagerImpl::OnLocal)

			]
		// Asset Viewer
		+ SCanvas::Slot()
			
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(830.0f, 640.0f))
			.Position(FVector2D(150.0f, 125.0f))
			
			[
				//PluginWebBrowser.ToSharedRef()
				SAssignNew(pFRealBandAssetLoader, FRealBandAssetLoader)
			   .AssetPickerConfig(ConfigPicker)
				
			]
		+ SCanvas::Slot()

			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(830.0f, 640.0f))
			.Position(FVector2D(150.0f, 125.0f))

			[
				PluginWebBrowser.ToSharedRef()
				

			]
		
		///////////////////
		+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(30.0f, 180.0f))
			[
				SAssignNew(pSettings, SButton)
				.HAlign(HAlign_Center)
			    .VAlign(VAlign_Center)
			    .Text(FText::FromString("Settings"))
			    .OnClicked(this, &FRealBandUIManagerImpl::LaunchSettings)
			]
		 + SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			.Position(FVector2D(750.0f, 5.0f))
			[
				SAssignNew(pImport, SButton)
				.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Text(FText::FromString("Import"))
			.OnClicked(this, &FRealBandUIManagerImpl::OnImportBtnClicked)
			]
		    + SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(950.0f, 700.0f))
				.Position(FVector2D(140.0f, 45.0f))
			[
				SAssignNew(pRealBandImportSettings, RealBandImportSettingsUI)
				.ObjPreference(&ObjUserPreference)
				
				
			]
			+ SCanvas::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.Size(FVector2D(100.0f, 50.0f))
			//.Position(FVector2D(690.0f, 600.0f))
			.Position(FVector2D(450.0f, 680.0f))
			[
				SAssignNew(pApplyButton, SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.Text(FText::FromString("Apply"))
				.OnClicked(this, &FRealBandUIManagerImpl::ApplySettings)
				.ToolTipText(LOCTEXT("ApplyButtonTooltip", "Apply settings"))
			    			     
			]


			]
			
			]

			
			
			];

		//	pDialogMainWindow->SetContent(LocalBrowserDock.ToSharedRef());

		FSlateApplication::Get().AddWindow(pDialogMainWindow.ToSharedRef());
		TSharedRef<FSlateAccessibleMessageHandler> MessageHandler = FSlateApplication::Get().GetAccessibleMessageHandler();
		

		//FSlateApplication::Get().OnResizingWindow(pDialogMainWindow)
		/*if (LocalBrowserDock)
		{
			LocalBrowserDock->SetVisibility(EVisibility::Hidden);
		}*/

		pApplyButton->SetVisibility(EVisibility::Hidden);

		if (pFRealBandAssetLoader)
		{
			if (GetAssetViewCount() > 0)
				pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
			else
				pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);

			pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
		}
		

		pRealBandImportSettings->pSettingsWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
			{
				pImport->SetVisibility(EVisibility::Visible);
				pRealBandImportSettings->SetVisibility(EVisibility::Hidden);
				pApplyButton->SetVisibility(EVisibility::Hidden);
				pDialogMainWindow->BringToFront(true);;
				
			}));

		//pBrowserTab->SetVisibility(EVisibility::Collapsed);

	}
	else
	{
		if (!pDialogMainWindow->IsVisible())
		{
			pDialogMainWindow->SetVisibility(EVisibility::All);
			pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
			pRealBandImportSettings->SetVisibility(EVisibility::Visible);
		}
    }
//	pDialogMainWindow->SetOnWindowClosed(FRequestDestroyWindowOverride::CreateSP(this, &FRealBandUIManagerImpl::OnDialogClosed));

    if (pRealBandImportSettings && GetAssetViewCount() == 0)
    {
	    pRealBandImportSettings->SetVisibility(EVisibility::Visible);
		pApplyButton->SetVisibility(EVisibility::Visible);
		pImport->SetVisibility(EVisibility::Hidden);
	}

	pDialogMainWindow->SetOnWindowClosed(FOnWindowClosed::CreateLambda([this](const TSharedRef<SWindow>& Window)
		{
			pRealBandImportSettings->SaveSettings();
			pDialogMainWindow->SetVisibility(EVisibility::Collapsed);
			pDialogMainWindow.Reset();
		//	pFRealBandAssetImporter.Reset();
			//FRealBandUIManager::Instance.Reset();
		}));

	PluginWebBrowser->SetVisibility(EVisibility::Collapsed);
}


void FRealBandUIManagerImpl::TranslateWebPage()
{
	if (PluginWebBrowser)
	{
		FString FilePath = TEXT("Translate.js");

		TArray<uint8> FileData;
		if (FFileHelper::LoadFileToArray(FileData, *FilePath))
		{
			FString FileContents;
			FFileHelper::BufferToString(FileContents, FileData.GetData(), FileData.Num());
			PluginWebBrowser->ExecuteJavascript(FileContents);
		
		}
		else
		{
			// Failed to load the file
			// Handle the error
		}

		
	}
}

void FRealBandUIManagerImpl::HandleBrowserUrlChanged(const FText& iUrl)
{
	
}

void FRealBandUIManagerImpl::OnDialogClosed(const TSharedRef<SWindow>& Window)
{
	pDialogMainWindow->BringToFront(true);;
}


FReply FRealBandUIManagerImpl::OnLocal()
{
	//pSettingsBox->SetVisibility(EVisibility::Collapsed);
	pRealBandImportSettings->SetVisibility(EVisibility::Collapsed);
	pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
	pImport->SetVisibility(EVisibility::Visible);
	pApplyButton->SetVisibility(EVisibility::Hidden);
	PluginWebBrowser->SetVisibility(EVisibility::Collapsed);
	return FReply::Handled();
}

FReply FRealBandUIManagerImpl::LaunchSettings()
{
	
//	pSettingsWindow->SetVisibility(EVisibility::Visible);
	//pSettingsBox->SetVisibility(EVisibility::Visible);
	pImport->SetVisibility(EVisibility::Collapsed);
	pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);
	pRealBandImportSettings->SetVisibility(EVisibility::Visible);
	pApplyButton->SetVisibility(EVisibility::Visible);

	//pAssetFolderPath->SetText(FText::FromString("C:\\Assets\\RealBand_bunddle_Asset\\RealBand_bunddle_Asset"));
	return FReply::Handled();
}


FReply FRealBandUIManagerImpl::OnImportBtnClicked()
{
	// Get the selected asset and add it to content browser
	pImport->SetVisibility(EVisibility::Visible);
	const TSharedPtr<SAssetView>& pAssetView = pFRealBandAssetLoader->GetAssetView();

	TArray<FContentBrowserItem> AssetsSelected = pAssetView->GetSelectedItems();

	TArray<FName> AssetList;
	for (FContentBrowserItem Item : AssetsSelected)
	{
		if (Item.IsValid())
		{
			FName AssetPath = Item.GetItemName();
			AssetList.Add(AssetPath);
			//PendingSyncItems.SelectedVirtualPaths.Add(Item->GetItem().GetVirtualPath());
		}
	}

	pFRealBandAssetImporter->ImportSelectedAssets(AssetList, ObjUserPreference);
	pDialogMainWindow->BringToFront();
	
	return FReply::Handled();
}


FReply FRealBandUIManagerImpl::ApplySettings()
{
//	if(pFRealBandAssetLoader->GetVisibility() == EVisibility::Visible)
//		return FReply::Handled();
	pApplyButton->SetVisibility(EVisibility::Hidden);
	pRealBandImportSettings->SetVisibility(EVisibility::Hidden);
	//pSettingsBox->SetVisibility(EVisibility::Collapsed);
	pImport->SetVisibility(EVisibility::Visible);
	//FText AssetFolderPath = pAssetFolderPath->GetText();
	//FString fPath = AssetFolderPath.ToString();
	FText AssetFolderPath = FText::FromString(ObjUserPreference.FolderPath.c_str());

	FText UriPath = FText::FromString(ObjUserPreference.ConnectPath.c_str());

	
	if (!UriPath.IsEmpty() && !pBrowserTab)
	{
		PluginWebBrowser->LoadURL(ObjUserPreference.ConnectPath.c_str());
		
		PluginWebBrowser->SetVisibility(EVisibility::Visible);
//		return FReply::Handled();
//		TranslatePages();
	}

	//FText AssetFolderPath = FText::FromString("C:\\Assets\\RealBand_bunddle_Asset\\RealBand_bunddle_Asset");
	pFRealBandAssetImporter->CreateTexturesFromAssets(AssetFolderPath);
	pDialogMainWindow->BringToFront();
	pDialogMainWindow->Restore();
	pDialogMainWindow->EnableWindow(true);
	if (PluginWebBrowser && PluginWebBrowser->IsLoaded())
	{
		pFRealBandAssetLoader->SetVisibility(EVisibility::Collapsed);
	}
	else
	{
		pFRealBandAssetLoader->SetVisibility(EVisibility::Visible);
	}
	pRealBandImportSettings->SaveConfig(ObjUserPreference);
	
	return FReply::Handled();
}



FReply FRealBandUIManagerImpl::ResetSettings()
{

	pAssetPath->ClearContent();
//	pAssetFolderPath->ClearContent();
	pAssetFolderPath->SetText(FText::FromString(""));
	
	for (TSharedPtr<SCheckBox> ptrChkBox : OptionsFormat)
	{
		ptrChkBox->SetIsChecked(ECheckBoxState::Unchecked);
	}
	
	ObjUserPreference.ActiveTypeBitset &= ~(0xFF);
	ObjUserPreference.ActiveTextypeBitset &= ~(0xFF);

	return FReply::Handled();
}



int FRealBandUIManagerImpl::GetAssetViewCount()
{
	TArray<FName> PackagePaths;
	PackagePaths.Add(FName("/Engine/RealBand/Textures/Prespective"));
	TArray<FAssetData> AssetsInPackages;
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	IAssetRegistry* AssetRegistry = &AssetRegistryModule.Get();
	FARFilter Filter;
	Filter.bIncludeOnlyOnDiskAssets = false;
	Filter.PackagePaths = PackagePaths;
	TArray<FAssetData> AsstInPackages;
	AssetRegistry->GetAssets(Filter, AsstInPackages);
	TArray<FString> OutPathList;
	AssetRegistry->GetSubPaths(FString("/Engine/RealBand"), OutPathList,true);
	return AsstInPackages.Num();
}


FReply FRealBandUIManagerImpl::LaunchOpenFileDialog()
{

	
	//		void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (pDialogMainWindow)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 represents multiple file selection
				FString FolderPath;
				FString DefaultPath("C:\\Temp");
				// set a default path
				FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_EXPORT, DefaultPath);
				DesktopPlatform->OpenDirectoryDialog(pDialogMainWindow->GetNativeWindow()->GetOSWindowHandle(),
					                                 DefaultPath,
					                                 DefaultPath, 
					                                 FolderPath);
				FEditorDirectories::Get().SetLastDirectory(ELastDirectory::GENERIC_EXPORT, FolderPath);
				if (pAssetPath)
				{
					pAssetPath->SetText(FText::FromString(FolderPath));
					pAssetFolderPath->SetText(FText::FromString(FolderPath));
				}

				
	//			DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
			}
	//	}
	//}

	return FReply::Handled();
}



TSharedRef<SDockTab> FRealBandUIManagerImpl::CreatRealBandTab(const FSpawnTabArgs& Args)
{
	FString PluginPath = FPaths::Combine(FPaths::EnginePluginsDir(), TEXT("RealBandImporter"));
	FString IndexUrl = FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginPath, TEXT("RealEye"), TEXT("index.html")));
	FString TokenPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(PluginPath,TEXT("RealEye"), TEXT("token")));

	FString IndexFileContent;
	FFileHelper::LoadFileToString(IndexFileContent, *IndexUrl);
	FString Token;
	FFileHelper::LoadFileToString(Token, *TokenPath);

	FString FinalUrl;
	if (Token.Len() > 0)
	{
		TSharedPtr<FJsonObject> JsonParsed;
		TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Token);
		if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
		{
			FString AccessToken = JsonParsed->GetStringField("token");
			FString RefreshToken = JsonParsed->GetStringField("refreshToken");
			const FString FileUrl = FPaths::Combine(TEXT("file:///"), IndexUrl);
			FinalUrl = FString::Printf(TEXT("%s?token=%s&refreshToken=%s"), *FileUrl, *AccessToken, *RefreshToken);
		}
	}
	else
	{
		FinalUrl = FPaths::Combine(TEXT("file:///"), IndexUrl);
	}
	
	FWebBrowserInitSettings browserInitSettings = FWebBrowserInitSettings();
	IWebBrowserModule::Get().CustomInitialize(browserInitSettings);
	WindowSettings.InitialURL = FinalUrl;
	WindowSettings.BrowserFrameRate = 60;
	
	if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
	{
		IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
		Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
		PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser)
			.ShowAddressBar(false)
			.ShowControls(true);
	}
	else
	{
		const FText Title = FText::FromString(TEXT("Enable Web Browser Plugin"));
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("RealBandPlugin requires the “Web Browser” plugin, which is disabled. Go to Edit > Plugins and search for “Web Browser” to enable it.")), &Title);

		return SAssignNew(LocalBrowserDock, SDockTab)
			.TabRole(ETabRole::PanelTab);
	}

	SAssignNew(LocalBrowserDock, SDockTab)
		.OnTabClosed_Lambda([](TSharedRef<class SDockTab> InParentTab)
			{
				// Kill node process if bound
				//FBridgeUIManager::BrowserBinding->OnExitDelegate.ExecuteIfBound("Plugin Window Closed");
				//FBridgeUIManager::BrowserBinding = NULL;

				//// Clean up browser
				//FBridgeUIManager::Instance->LocalBrowserDock = NULL;
				//if (FBridgeUIManager::Instance->WebBrowserWidget.IsValid())
				//{
				//	FBridgeUIManager::Instance->WebBrowserWidget.Reset();
				//	FBridgeUIManager::Instance->Browser.Reset();
				//}
			})
		.TabRole(ETabRole::NomadTab)
				[
					PluginWebBrowser.ToSharedRef()
				];

			LocalBrowserDock->SetOnTabDraggedOverDockArea(
				FSimpleDelegate::CreateLambda([IndexUrl]()
					{
						//FBridgeUIManager::Instance->WebBrowserWidget->Invalidate(EInvalidateWidgetReason::Layout);
					})
			);

			LocalBrowserDock->SetOnTabRelocated(
				FSimpleDelegate::CreateLambda([IndexUrl]()
					{
						//FBridgeUIManager::Instance->WebBrowserWidget->Invalidate(EInvalidateWidgetReason::Layout);
					})
			);
			if (WebBrowserWidget.IsValid())
			{
			/*	UNodePort* NodePortInfo = NewObject<UNodePort>();
				FBridgeUIManager::BrowserBinding = NewObject<UBrowserBinding>();
				FBridgeUIManager::Instance->WebBrowserWidget->BindUObject(TEXT("NodePortInfo"), NodePortInfo, true);
				FBridgeUIManager::Instance->WebBrowserWidget->BindUObject(TEXT("BrowserBinding"), FBridgeUIManager::BrowserBinding, true);*/
			}

			if (LocalBrowserDock.IsValid())
			{
				return LocalBrowserDock.ToSharedRef();
			}

			return SAssignNew(LocalBrowserDock, SDockTab);


	////////////////////////////////////////////////////////////////////////////////////
	//FWebBrowserInitSettings browserInitSettings = FWebBrowserInitSettings();
	//IWebBrowserModule::Get().CustomInitialize(browserInitSettings);
	//TSharedPtr<SWebBrowser> PluginWebBrowser;
	//WindowSettings.InitialURL = ObjUserPreference.ConnectPath.c_str();
	//WindowSettings.BrowserFrameRate = 60;

	//if (IWebBrowserModule::IsAvailable() && IWebBrowserModule::Get().IsWebModuleAvailable())
	//{
	//	IWebBrowserSingleton* WebBrowserSingleton = IWebBrowserModule::Get().GetSingleton();
	//	Browser = WebBrowserSingleton->CreateBrowserWindow(WindowSettings);
	//	PluginWebBrowser = SAssignNew(WebBrowserWidget, SWebBrowser, Browser)
	//		.ShowAddressBar(false)
	//		.ShowControls(false);
	//}

	////
	//return SAssignNew(LocalBrowserDock, SDockTab);
}





#undef LOCTEXT_NAMESPACE