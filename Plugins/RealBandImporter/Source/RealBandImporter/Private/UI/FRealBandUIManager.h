//Copyright(C) 2022 VAXPORT all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/SCanvas.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Docking/SDockTab.h"

#include "Framework/Docking/TabManager.h"
#include "WebBrowser/Public/IWebBrowserWindow.h"
#include "ContentBrowserDelegates.h"

#include "DatasmithGLTFImportOptions.h"
#include "DatasmithGLTFImporter.h"
#include "Importer/FRealBandAssetImporter.h"
#include "RealBandCommon.h"

class FRealBandAssetLoader;
class SGridPanel;
class RealBandImportSettingsUI;

struct FFileChangeData;


//#include "SWebBrowser.h"
//#include "UI/BrowserBinding.h"
//#include "Framework/MultiBox/MultiBoxBuilder.h"
//#include "Framework/Docking/TabManager.h"
DECLARE_LOG_CATEGORY_EXTERN(LogManager, Log, All);



class FRealBandUIManagerImpl : public TSharedFromThis<FRealBandUIManagerImpl>
{

private:
	TSharedPtr<SCanvas> pCanvas;
	TSharedPtr<SWindow> pBrowserWindow;
	TSharedPtr<SOverlay> pOverlay;
	TSharedPtr<SButton> pSettings;
	TSharedPtr<SBox> pSettingsBox;
	TSharedPtr<SButton> pLocal;
	TSharedPtr<SButton> pImport;
	TSharedPtr<SWindow> pDialogMainWindow;
	TSharedPtr<SWindow> pSettingsWindow;
	TSharedPtr<SBox> pBox;
	TSharedPtr<SBorder> pBorder;
	TSharedPtr<SComboBox<TSharedPtr<FString>>> ProjectsCombo;
	TSharedPtr<STextBlock> pTextBlock;
	TSharedPtr<SWebBrowser> WebBrowserWidget;
	TSharedPtr<SEditableTextBox> pAssetPath;
	TSharedPtr<SEditableTextBox> pAssetFolderPath;
	TSharedPtr <SCanvas> pSettingsCanvasBox;
	TSharedPtr <SOverlay> pSettingsOverlay;
	TArray< TSharedPtr <SCheckBox>> OptionsFormat;
	TSharedPtr<SHorizontalBox> pHBox;
	TSharedPtr<SCheckBox> pChkBox;
	TSharedPtr<SDockTab> pBrowserTab;
	TSharedPtr<SWebBrowser> PluginWebBrowser;
	TSharedPtr <RealBandImportSettingsUI> pRealBandImportSettings;
	FReply LaunchSettings();
	FReply OnLocal();
	FReply ApplySettings();
	FReply ResetSettings();
	FReply OnImportBtnClicked();
	FReply LaunchOpenFileDialog();
	TArray<TSharedPtr <FString>> Array_Resolutions; 
	TSharedPtr<FString> SelectedProject;
	TSharedRef<SDockTab> CreatRealBandTab();
	TSharedPtr<FString> FolderSelected;
	FSetPathPickerPathsDelegate SetPathsDelegate;

	TStrongObjectPtr<UDatasmithGLTFImportOptions> ImportOptions;
	TSharedPtr<class FDatasmithGLTFImporter> Importer;
	TSharedPtr<FRealBandAssetLoader> pFRealBandAssetLoader;
	TSharedPtr<FRealBandAssetImporter> pFRealBandAssetImporter;
	TSharedPtr<SButton> ExpanderArrow;
	TSharedPtr<SButton> pApplyButton;
	int GetAssetViewCount();
	USRPREFERENCE ObjUserPreference;
	void SetupMenuItem();
	void FillToolbar(FToolBarBuilder& ToolbarBuilder);
	void OnDialogClosed(const TSharedRef<SWindow>& Window);
	TSharedRef<SDockTab> CreatRealBandTab(const FSpawnTabArgs& Args);
	void TranslateWebPage();
public:

	FRealBandUIManagerImpl(TSharedPtr<FRealBandAssetImporter> );
	virtual ~FRealBandUIManagerImpl();
	void Initialize();
	void CreateWindow();
	bool RestoreMainWindow();
	TSharedPtr<SDockTab> LocalBrowserDock;
	TSharedPtr<SWindow> DragDropWindow;
	TSharedPtr<IWebBrowserWindow> Browser;
	FCreateBrowserWindowSettings WindowSettings;
	uint16 ActiveTypeBitset;
	void HandleBrowserUrlChanged(const FText& Url);
	void TranslatePages();

protected:
	const FText RealBandTabDisplay = FText::FromString("RealBand");
	const FText RealBandToolTip = FText::FromString("Launch RealEye");
};


class FRealBandUIManager
{
public:
	static void Initialize(TSharedPtr<FRealBandAssetImporter>);
	static bool DisplayStartWindow();
	static TSharedPtr<FRealBandUIManagerImpl> Instance;
	~FRealBandUIManager();
};
