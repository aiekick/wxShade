/*
   Copyright 2016 Stephane Cuillerdier aka Aiekick

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "MainFrame.h"

#include "psapi.h"

MainFrame* g_pMainFrame = 0;

#include "wx\ribbon\buttonbar.h"

#include "ImportFromShaderToy.h"

#include "HelpDlg.h"

#include "Assets.h"

std::map<UniformsWidgetTypeEnum, wxBitmap> AssetsManager::gUniformIcon = std::map<UniformsWidgetTypeEnum, wxBitmap>();

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_RIBBONBUTTONBAR_CLICKED(ID_NEW_DOCUMENT, MainFrame::OnNewDocClick)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_NEW_DOCUMENT, MainFrame::OnNewDropdown)
EVT_RIBBONBUTTONBAR_CLICKED(ID_OPEN_DOCUMENT, MainFrame::OnOpenDocClick)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_OPEN_DOCUMENT, MainFrame::OnOpenDropdown)
EVT_MENU(ID_OPEN_DOCUMENT_WITHOUT_COMPILATION, MainFrame::OnOpenShaderWithoutCompilation)
EVT_MENU(ID_IMPORT_FROM_SHADERTOY_DOCUMENT, MainFrame::OnImportFromShaderToy)
EVT_MENU(ID_NEW_SHADER_FOR_QUAD, MainFrame::OnNewShaderForQuad)
EVT_MENU(ID_NEW_SHADER_FOR_POINTS, MainFrame::OnNewShaderForPoints)
EVT_MENU(ID_NEW_SHADER_FOR_CUSTOM_SHAPE, MainFrame::OnNewShaderForCustomShape)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVE_DOCUMENT, MainFrame::OnSaveDocClick)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SAVEAS_DOCUMENT, MainFrame::OnSaveAsDocClick)
EVT_RIBBONBUTTONBAR_DROPDOWN_CLICKED(ID_SAVEAS_DOCUMENT, MainFrame::OnSaveAsDropdown)
EVT_MENU(ID_EXPORT_SHADER_TO_GIF, MainFrame::OnExportToGif)
EVT_MENU(ID_EXPORT_TO_SHADERTOY, MainFrame::OnExportToShaderToy)
EVT_RIBBONBUTTONBAR_CLICKED(ID_DELETE_DOCUMENT, MainFrame::OnDeleteDocClick)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHOW_HIDE_OUTPUT_WIN, MainFrame::OnShowHideOutputWinClick)
EVT_RIBBONBUTTONBAR_CLICKED(ID_COMPIL_RUN_SHADER, MainFrame::OnCompilAndRunShaderClick)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_PERF_ANALYSE, MainFrame::OnDoShaderPerfAnalyse)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT0, MainFrame::OnDoShaderAutoLayout0)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT1, MainFrame::OnDoShaderAutoLayout1)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT2, MainFrame::OnDoShaderAutoLayout2)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT3, MainFrame::OnDoShaderAutoLayout3)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT4, MainFrame::OnDoShaderAutoLayout4)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT5, MainFrame::OnDoShaderAutoLayout5)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_LAYOUT6, MainFrame::OnDoShaderAutoLayout6)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_DO_AUTO_INDENT, MainFrame::OnDoShaderAutoIndent)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_ANALYSE_CODE, MainFrame::OnDoAnalyseShaderCode)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SHADER_MUSIC_ADD, MainFrame::OnAddMusicShader)
EVT_AUINOTEBOOK_PAGE_CHANGED(ID_SHADER_NOTEBOOK, MainFrame::OnPageChanged)
EVT_AUINOTEBOOK_PAGE_CLOSE(ID_SHADER_NOTEBOOK, MainFrame::OnPageClose)
EVT_AUINOTEBOOK_PAGE_CLOSED(ID_SHADER_NOTEBOOK, MainFrame::OnPageClosed)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_SCREEN, MainFrame::OnAddUniformScreen)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_MOUSE, MainFrame::OnAddUniformMouse)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_TEXTURE2D, MainFrame::OnAddUniformTexture2D)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_TEXTURECUBE, MainFrame::OnAddUniformTextureCube)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_VIDEO, MainFrame::OnAddUniformVideo)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_MIC, MainFrame::OnAddUniformMic)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_SLIDER, MainFrame::OnAddUniformSlider)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_CURVE, MainFrame::OnAddUniformCurve)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_WEBCAM, MainFrame::OnAddUniformWebCam)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_MUSIC, MainFrame::OnAddUniformMusic)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_TIME, MainFrame::OnAddUniformTime)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_COLOR, MainFrame::OnAddUniformColor)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_GAMEPAD360, MainFrame::OnAddUniformGamePad360)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_BVH, MainFrame::OnAddUniformBvh)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_EXPORT, MainFrame::OnAddUniformExport)
EVT_RIBBONTOOLBAR_CLICKED(ID_SHADER_UNIFORM_FONT, MainFrame::OnAddUniformFont)
EVT_RIBBONBUTTONBAR_CLICKED(ID_EXPORT_SHADER_TO_PICTURE, MainFrame::OnExportToPicture)
EVT_RIBBONBUTTONBAR_CLICKED(ID_SETTINGS, MainFrame::OnSettings)
EVT_RIBBONBUTTONBAR_CLICKED(ID_HELP, MainFrame::OnHelp)
EVT_RIBBONBUTTONBAR_CLICKED(ID_ABOUT, MainFrame::OnAbout)
EVT_HTML_LINK_CLICKED(ID_ABOUT_HTML_WIN, MainFrame::OnLinkClicked)
EVT_MAXIMIZE(MainFrame::OnMaximise)
EVT_SIZE(MainFrame::OnSize)
EVT_TIMER(ID_MEMORY_CHECK_TIMER, MainFrame::OnCheckUsedMemoryTimer)
wxEND_EVENT_TABLE()

////////////////////////////////////////////////////////////////////////////////////////////////////

myFileDropTarget::myFileDropTarget() : wxFileDropTarget()
{

}

myFileDropTarget::~myFileDropTarget()
{

}

bool myFileDropTarget::OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString &filenames)
{
	g_pMainFrame->Openfiles(filenames, true);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame()
: wxFrame(NULL, wxID_ANY, wxT(""), wxDefaultPosition, wxSize(800, 600), wxDEFAULT_FRAME_STYLE), 
m_CurrentRibbonDockDirection(wxAUI_DOCK_LEFT), m_HelpDlg(0), m_CheckUsedMemoryTimer(this, ID_MEMORY_CHECK_TIMER)
{
	xShadeVersion = "wxShade Build " + toStr(BuildNumber);

	this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));

	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UBVH] = wxBitmap(Walk_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCOLOR] = wxBitmap(ColorWheel_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCUBEMAP] = wxBitmap(FloodIt_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCURVE] = wxBitmap(DrawVertex_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UEXPORT] = wxBitmap(WrappingBetween_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UGAMEPAD360] = wxBitmap(Controller_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMIC] = wxBitmap(Microphone_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMOUSE] = wxBitmap(MouseSelectScroll_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMUSIC] = wxBitmap(Music_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::USCREEN] = wxBitmap(ScreenError_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::USLIDER] = wxBitmap(Hslider_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UTEX2D] = wxBitmap(Picture_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UTIME] = wxBitmap(Time_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UVIDEO] = wxBitmap(Film_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UWEBCAM] = wxBitmap(Webcam_small_icon);
	AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UFONT] = wxBitmap(Font_small_icon);
	
	InitControls();

	UpdateTitleWidthExtraInfos("");

	m_CheckUsedMemoryTimer.Start(1000.0);// 1 second
}

MainFrame::~MainFrame()
{
	m_CheckUsedMemoryTimer.Stop();

	UnInitControls();
}

void MainFrame::InitControls()
{
	SetIcon(wxICON(FIRE));

	bool CaptionVisible = false;

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_ALLOW_ACTIVE_PANE | wxAUI_MGR_ALLOW_FLOATING | wxAUI_MGR_DEFAULT 
		| wxAUI_MGR_HINT_FADE | wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_NO_VENETIAN_BLINDS_FADE 
		| wxAUI_MGR_RECTANGLE_HINT | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_TRANSPARENT_HINT 
		| wxAUI_MGR_VENETIAN_BLINDS_HINT);

	m_mgr.Connect(wxEVT_AUI_RENDER, wxAuiManagerEventHandler(MainFrame::OnRender), 0, this);

	m_ribbonBar1 = new wxRibbonBar(this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), 
		wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_ICONS);

	auto theme = new wxRibbonMSWArtProvider();
	m_ribbonBar1->SetArtProvider(theme);
	
	//m_ribbonBar1->GetArtProvider()->SetColour(wxRIBBON_ART_PANEL_LABEL_BACKGROUND_COLOUR, wxColour(255, 0, 0));
	//m_ribbonBar1->GetArtProvider()->SetColour(wxRIBBON_ART_PANEL_LABEL_BACKGROUND_GRADIENT_COLOUR, wxColour(0, 255, 0));
	
	m_ribbonBar1->SetMinSize(wxSize(160, 140));
	m_ribbonBar1->SetAutoLayout(true);

	m_mgr.AddPane(m_ribbonBar1, wxAuiPaneInfo().
		Top().
		Name(wxT("Ribbon")).
		CaptionVisible(CaptionVisible).
		CloseButton(false).
		PinButton(false).
		Dock().
		LeftDockable(false).
		RightDockable(false).
		Resizable().
		FloatingSize(wxSize(200, 200)).
		Row(1).
		MinSize(wxSize(95, 95)).
		Layer(1));

	m_ribbonPage1 = new wxRibbonPage(m_ribbonBar1, wxID_ANY, wxT("General"), Page_small_icon, 0);
		m_ribbonPanel1 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Files"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar1 = new wxRibbonButtonBar(m_ribbonPanel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar1->AddHybridButton(ID_NEW_DOCUMENT, wxT("New"), Page_icon, wxEmptyString);
				m_ribbonButtonBar1->AddHybridButton(ID_OPEN_DOCUMENT, wxT("Open"), PageAdd_icon, wxEmptyString);
				m_ribbonButtonBar1->AddButton(ID_SAVE_DOCUMENT, wxT("Save"), PageSave_icon, wxEmptyString);
				m_ribbonButtonBar1->AddButton(ID_SAVEAS_DOCUMENT, wxT("SaveAs"), PageSave_icon, wxEmptyString);

		m_ribbonPanel4 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Divers"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar3 = new wxRibbonButtonBar(m_ribbonPanel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				//m_ribbonButtonBar3->AddToggleButton(ID_SHOW_HIDE_OUTPUT_WIN, wxT("Output"), Error, wxEmptyString);
				//m_ribbonButtonBar3->ToggleButton(ID_SHOW_HIDE_OUTPUT_WIN, false);
				m_ribbonButtonBar3->AddButton(ID_SETTINGS, wxT("Settings"), SettingTools_icon, wxEmptyString);

		m_ribbonPanel8 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Layout"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar3 = new wxRibbonButtonBar(m_ribbonPanel8, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT0, wxT("Lay 0"), Layout0_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT1, wxT("Lay 1"), Layout1_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT2, wxT("Lay 2"), Layout2_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT3, wxT("Lay 3"), Layout3_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT4, wxT("Lay 4"), Layout4_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT5, wxT("Lay 5"), Layout5_icon, wxEmptyString);
				m_ribbonButtonBar3->AddToggleButton(ID_SHADER_DO_AUTO_LAYOUT6, wxT("Lay 6"), Layout6_icon, wxEmptyString);

		m_ribbonPanel9 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Editor"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar4 = new wxRibbonButtonBar(m_ribbonPanel9, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar4->AddButton(ID_SHADER_DO_AUTO_INDENT, wxT("Auto Indent"), AutoIndent_icon, wxEmptyString);

		m_ribbonPanel3 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Compil"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar2 = new wxRibbonButtonBar(m_ribbonPanel3, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar2->AddButton(ID_COMPIL_RUN_SHADER, wxT("Run"), GearIn_icon, wxEmptyString);

		m_ribbonPanel5 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Uniforms"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonToolBar1 = new wxRibbonToolBar(m_ribbonPanel5, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_SCREEN, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::USCREEN], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SCREEN, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_TIME, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UTIME], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TIME, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_MOUSE, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMOUSE], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_MOUSE, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_TEXTURE2D, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UTEX2D], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURE2D, true);
				m_ribbonToolBar1->AddSeparator();
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_TEXTURECUBE, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCUBEMAP], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURECUBE, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_GAMEPAD360, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UGAMEPAD360], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_GAMEPAD360, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_SLIDER, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::USLIDER], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SLIDER, true);
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_COLOR, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCOLOR], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_COLOR, true);
				m_ribbonToolBar1->AddSeparator();
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_EXPORT, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UEXPORT], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_EXPORT, true);
				m_ribbonToolBar1->AddSeparator();
				m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_FONT, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UFONT], wxEmptyString);
				m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_FONT, true);

				m_ribbonToolBar1->SetRows(2, 2);

				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_MIC, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMIC], wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_MIC, false);
				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_VIDEO, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UVIDEO], wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_VIDEO, false);

				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_BVH, Walk_small_icon, wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_BVH, true);

				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_CURVE, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UCURVE] , wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_CURVE, false);
				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_WEBCAM, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UWEBCAM], wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_WEBCAM, false);
				//m_ribbonToolBar1->AddTool(ID_SHADER_UNIFORM_MUSIC, AssetsManager::gUniformIcon[UniformsWidgetTypeEnum::UMUSIC], wxEmptyString);
				//m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_MUSIC, false);
				
		m_ribbonPane21 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Export"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
		m_ribbonButtonBar6 = new wxRibbonButtonBar(m_ribbonPane21, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar6->AddButton(ID_EXPORT_SHADER_TO_PICTURE, wxT("Picture"), PictureSave_icon, wxEmptyString);
				m_ribbonButtonBar6->EnableButton(ID_EXPORT_SHADER_TO_PICTURE, false);

		m_ribbonPane20 = new wxRibbonPanel(m_ribbonPage1, wxID_ANY, wxT("Infos"), wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxRIBBON_PANEL_DEFAULT_STYLE | wxRIBBON_PANEL_NO_AUTO_MINIMISE);
			m_ribbonButtonBar5 = new wxRibbonButtonBar(m_ribbonPane20, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
				m_ribbonButtonBar5->AddButton(ID_HELP, wxT("Help"), Help_icon, wxEmptyString);
				m_ribbonButtonBar5->EnableButton(ID_HELP, false);
				m_ribbonButtonBar5->AddButton(ID_ABOUT, wxT("About"), Information_icon, wxEmptyString);
				m_ribbonButtonBar5->EnableButton(ID_ABOUT, true);
				
	m_ribbonBar1->Realize();

	m_ribbonBar1->SetActivePage(m_ribbonPage1);

	m_statusBar = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY);
	m_statusBar->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnStatusBarClick), NULL, this);

	m_OutputWinCompil = new OutputWin(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
	g_pOutputWin = m_OutputWinCompil;

	wxAuiPaneInfo ribbonPaneInfo = m_mgr.GetPane(m_ribbonBar1);
	
	m_mgr.AddPane(m_OutputWinCompil, 
		wxAuiPaneInfo().
		Name(wxT("OutputWin")).
		Bottom().
		CaptionVisible(true).
		Caption(wxT("Compil Output")).
		CloseButton(false).
		PinButton(false).
		MaximizeButton(true).
		Dock().
		Resizable().
		FloatingSize(wxDefaultSize).
		DockFixed(false).
		MinSize(wxSize(80, 80)));

	m_ShaderBook = new wxAuiNotebook(this, ID_SHADER_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxAUI_NB_DEFAULT_STYLE);
	m_mgr.AddPane(m_ShaderBook, wxAuiPaneInfo().CenterPane().CaptionVisible(CaptionVisible).CloseButton(false).PinButton(true).Dock().Resizable().FloatingSize(wxDefaultSize).CentrePane());

	/*// load config
	string config;
	ifstream configFile("layout.conf", ios::in);
	if (configFile)
	{
		configFile >> config;
		configFile.close();

		if (config != "")
		{
			m_mgr.LoadPerspective(config, true);
		}
	}*/

	GetCurrentView();
	ToggleLayoutButton(ShaderArtLayoutEnum::NONE);

	m_mgr.Update();

	m_RibbonFileDropTarget = new myFileDropTarget();
	m_ribbonBar1->SetDropTarget(m_RibbonFileDropTarget);

	m_BookFileDropTarget = new myFileDropTarget();
	m_ShaderBook->SetDropTarget(m_BookFileDropTarget);

	this->Centre(wxBOTH);

	// on affiche ou nom la fenetre de srotie selon 'letat du bouton de commande dans le ribbon
	wxRibbonButtonBarEvent e;
	OnShowHideOutputWinClick(e);

	// load config
	SettingsDlg::LoadConfig();
}

void MainFrame::CheckUsedMemory() // http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
{
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));

	SIZE_T value = pmc.PagefileUsage;
	string mem = toStr(value);
	string unit = "";
	float valueReducted = value;
	if (mem.size() > 12) 
	{
		valueReducted /= pow(10.0, 12.0);
		unit = " TO";
	}
	else if (mem.size() > 9)
	{
		valueReducted /= pow(10.0, 9.0);
		unit = " GO";
	}
	else if (mem.size() > 6)
	{
		valueReducted /= pow(10.0, 6.0);
		unit = " MO";
	}
	else if (mem.size() > 3)
	{
		valueReducted /= pow(10.0,3.0);
		unit = " KO";
	}

	valueReducted = int(valueReducted * 100.0f) / 100.0f;

	string memory = "mem : " + toStr(valueReducted) + unit;

	UpdateTitleWidthExtraInfos(memory);
}

void MainFrame::OnCheckUsedMemoryTimer(wxTimerEvent& /*event*/)
{
	CheckUsedMemory();
}

void MainFrame::UpdateTitleWidthExtraInfos(string vInfos)
{
	string title = xShadeVersion + " " + vInfos;
	this->SetTitle(title);
}

void MainFrame::UnInitControls()
{
	this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainFrame::OnClose));
	
	m_statusBar->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(MainFrame::OnStatusBarClick), NULL, this);

	/*// save config
	string config = m_mgr.SavePerspective().ToStdString();
	ofstream configFile("layout.conf", ios::out);
	if (configFile)
	{
		configFile << config;
		configFile.close();
	}*/

	m_mgr.UnInit();
}

void MainFrame::OnClose(wxCloseEvent& event)
{
	if (CloseAllDocuments(event) == true)
	{
		wxFrame::OnCloseWindow(event);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//////////////////// EVT_RIBBONBUTTONBAR_CLICKED ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

// new doc => doc template de base
void MainFrame::OnNewDocClick(wxRibbonButtonBarEvent& event)
{ 
	OnNewShaderForQuad(event);
}

void MainFrame::OnNewDropdown(wxRibbonButtonBarEvent& event)
{
	wxMenu menu;
	menu.Append(ID_NEW_SHADER_FOR_QUAD, wxT("New Quad Shader"));
	menu.Append(ID_NEW_SHADER_FOR_POINTS, wxT("New Points Shader"));
	event.PopupMenu(&menu);
}

void MainFrame::OnOpenDocClick(wxRibbonButtonBarEvent& /*event*/)
{ 
	string xsha = "XSha File (*.xsha)|*.xsha";
	string xshb = "XShb Embedded Ressource File(*.xshb) | *.xshb";
	wxFileDialog fileDlg(this, _("Open"), SettingsDlg::ShadersDirPath, "", xsha + "|" + xshb, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxArrayString arr;
		fileDlg.GetPaths(arr);
		Openfiles(arr, true);
	}
}

void MainFrame::OnOpenShaderWithoutCompilation(wxCommandEvent& /*event*/)
{
	string xsha = "XSha File (*.xsha)|*.xsha";
	string xshb = "XShb Embedded Ressource File(*.xshb) | *.xshb";
	wxFileDialog fileDlg(this, _("Open"), SettingsDlg::ShadersDirPath, "", xsha + "|" + xshb, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_MULTIPLE);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxArrayString arr;
		fileDlg.GetPaths(arr);
		Openfiles(arr, false);
	}
}

void MainFrame::Openfiles(wxArrayString vPaths, bool vCompilAfterLoating)
{
	if (vPaths.size() > 0)
	{
		if (vPaths.size() > 1)
		{
			string msg = "You are on the way to open " + toStr(vPaths.size()) + " XShade files !\n It may use a long time !\nAre you sure ?";
			int answer = wxMessageBox(msg, "WARNING !!", wxYES | wxNO, this);
			if (answer == wxNO)
			{
				return;// on abandonne
			}
		}

		for (std::size_t i = 0; i < vPaths.size(); i++)
		{
			string filepathname = vPaths.Item(i).ToStdString();
			DocumentFileFormat format = DocumentFileFormat::DOC_FILE_FORMAT_NOTHING;
			if (filepathname.find(".xsha") != string::npos) // un fichier a été trouvé et c'est bien la bonne extention
				format = DocumentFileFormat::DOC_FILE_FORMAT_ASCII;
			if (filepathname.find(".xshb") != string::npos) // un fichier a été trouvé et c'est bien la bonne extention
				format = DocumentFileFormat::DOC_FILE_FORMAT_EMBEDDED;
			if (format != DocumentFileFormat::DOC_FILE_FORMAT_NOTHING) // un fichier a été trouvé et c'est bien la bonne extention
			{
				wxFileName wxFN(filepathname);
				if (wxFN.Exists() == true)
				{
					string filename = wxFN.GetName().ToStdString(); // nom du fichier sans le chemin et l'extention pour le nom de l'onglet

					ShaderArtDocument *docAnalyser = new ShaderArtDocument(filepathname, 0);
					if (docAnalyser != 0)
					{
						docAnalyser->LoadDocumentFile(filepathname, format, true);
						ShaderArtTypeEnum shaderArtType = docAnalyser->GetShaderArtType();
						if (shaderArtType == ShaderArtTypeEnum::SHADER_TYPE_UNKNOW)
							shaderArtType = ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD;

						// il va falloir parser le fichier en dehors pour trouver le type de shader avant de le charger completement
						DocShaderArt *doc = new DocShaderArt(m_ShaderBook, shaderArtType, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, filename);
						if (doc->LoadDocument(filepathname, format, vCompilAfterLoating) == true)
						{
							filename = doc->GetDocument()->GetFileName();

							m_ShaderBook->AddPage(doc, filename, true, wxNullBitmap);

							doc->GetDocument()->SetModified(false);
						}
						else
						{
							// message box to show error
							SAFE_DELETE(doc);
						}
						delete docAnalyser;
					}
				}
				
			}
		}
	}
}

void MainFrame::OnOpenDropdown(wxRibbonButtonBarEvent& event)
{
	wxMenu menu;
	menu.Append(ID_OPEN_DOCUMENT_WITHOUT_COMPILATION, wxT("Open Without Direct Compilation"));
	menu.Append(ID_IMPORT_FROM_SHADERTOY_DOCUMENT, wxT("Import From ShaderToy"));
	event.PopupMenu(&menu);
}

void MainFrame::OnSaveAsDropdown(wxRibbonButtonBarEvent& event)
{
	wxMenu menu;
	menu.Append(ID_EXPORT_TO_SHADERTOY, wxT("Export To ShaderToy"));
	event.PopupMenu(&menu);
}

void MainFrame::OnExportToGif(wxCommandEvent& /*event*/)
{
	/*	char *buffer = (char*) calloc(imgWidth * imgHeight * 4, sizeof(char)); 
		glReadPixels(0, 0, imgWidth, imgHeight, GL_RGB, GL_UNSIGNED_BYTE, buffer); 
		Blob b(buffer, 4 * imgWidth * imgHeight); 
		Image svimage(imgWidth, imgHeight, "RGB", CharPixel, buffer);*/

	/*std::vector<Magick::Image> frames;
	Magick::Image img1("100x100", "white");
	frames.push_back(img1);

	Magick::Image img2("100x100", "red");
	frames.push_back(img2);

	img1.animationDelay(2000);
	img2.animationDelay(2000);

	Magick::writeImages(frames.begin(), frames.end(), "2.gif");*/
}

void MainFrame::OnImportFromShaderToy(wxCommandEvent& /*event*/)
{
	ShaderToyImportWizard *wiz = new ShaderToyImportWizard(this, wxID_ANY);
	wiz->SetShaderBook(m_ShaderBook);
	if (wiz->RunWizard(wiz->m_pages.Item(0)) == true)
	{

	}
	SAFE_DELETE(wiz);
}

// un sahder de type de base avec un quad genre shadertoy ou glslsandbox
void MainFrame::OnNewShaderForQuad(wxCommandEvent& /*event*/)
{
	int count = m_ShaderBook->GetPageCount();
	string filename = "Shader " + toStr(count);
	string currentPath = wxGetCwd();
	DocShaderArt *doc = new DocShaderArt(m_ShaderBook, ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, filename);
	if (doc->NewDocument(currentPath + "\\" + filename) == true)
	{
		m_ShaderBook->AddPage(doc, filename, true, wxNullBitmap);
	}
	else
	{
		// message box to show error
		SAFE_DELETE(doc);
	}
}

// un sahder du type de vertexshaderart avec 50k points a manipuler par le vertex
void MainFrame::OnNewShaderForPoints(wxCommandEvent& /*event*/)
{
	int count = m_ShaderBook->GetPageCount();
	string filename = "Shader " + toStr(count);
	string currentPath = wxGetCwd();
	DocShaderArt *doc = new DocShaderArt(m_ShaderBook, ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, filename);
	if (doc->NewDocument(currentPath + "\\" + filename) == true)
	{
		m_ShaderBook->AddPage(doc, filename, true, wxNullBitmap);
	}
	else
	{
		// message box to show error
		SAFE_DELETE(doc);
	}
}

// un shader avec un vertex et un fragment mais avec des models 3d a charger
void MainFrame::OnNewShaderForCustomShape(wxCommandEvent& /*event*/)
{

}

void MainFrame::OnExportToShaderToy(wxCommandEvent& /*event*/)
{

}

void MainFrame::OnSaveDocClick(wxRibbonButtonBarEvent& /*event*/)
{ 
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		ShaderArtDocument *doc = view->GetDocument();
		if (doc->IsModified() == true)
		{
			if (doc->GetFilePathName() == "")
			{
				wxRibbonButtonBarEvent e;
				OnSaveAsDocClick(e);
			}
			else
			{
				if (view->SaveDocumentFile("", DocumentFileFormat::DOC_FILE_FORMAT_NOTHING) == true)
				{
					SetSavedStatusOFCurrentPage(true);
				}
			}
		}
	}
}

void MainFrame::OnSaveAsDocClick(wxRibbonButtonBarEvent& /*event*/)
{ 
	string xsha = "XSha File (*.xsha)|*.xsha";
	string xshb = "XShb Embedded Ressource File(*.xshb) | *.xshb";
	wxFileDialog fileDlg(this, _("Save As"), SettingsDlg::ShadersDirPath, "", xsha + "|" + xshb, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		string filepathname = fileDlg.GetPath().ToStdString();
		DocShaderArt *view = GetCurrentView();
		if (view != 0)
		{
			int filterIdx = fileDlg.GetFilterIndex();
			
			DocumentFileFormat format = DocumentFileFormat::DOC_FILE_FORMAT_NOTHING;
			if (filterIdx == 0)
				format = DocumentFileFormat::DOC_FILE_FORMAT_ASCII;
			if (filterIdx == 1)
				format = DocumentFileFormat::DOC_FILE_FORMAT_EMBEDDED;

			if (view->SaveDocumentFile(filepathname, format) == true)
			{
				SetSavedStatusOFCurrentPage(true);
			}
		}
	}
}

void MainFrame::OnDeleteDocClick(wxRibbonButtonBarEvent& event)
{ 
	event.Skip(); 
}

void MainFrame::OnShowHideOutputWinClick(wxRibbonButtonBarEvent& /*event*/)
{ 
	wxAuiPaneInfo& pi = m_mgr.GetPane(wxT("OutputWin")); // le & est important sinon le block ne peut rien piloter
	if (pi.IsOk()) 
	{
		if (pi.IsShown() == true)
		{
			pi.Hide();
		}
		else
		{
			pi.Show();
		}

		m_ribbonButtonBar3->ToggleButton(ID_SHOW_HIDE_OUTPUT_WIN, pi.IsShown());
		m_ribbonButtonBar3->Realize();
		m_mgr.Update();
	}
}

DocShaderArt* MainFrame::GetCurrentView()
{
	DocShaderArt *res = 0;

	// redirect to active DocShaderArt
	wxWindow *win = m_ShaderBook->GetCurrentPage();
	if (win != 0)
	{
		DocShaderArt *view = dynamic_cast<DocShaderArt*>(win);
		if (view != 0)
		{
			res = view;
			m_ribbonButtonBar1->EnableButton(ID_SAVE_DOCUMENT, view->GetDocument()->IsModified());
			m_ribbonButtonBar1->EnableButton(ID_SAVEAS_DOCUMENT, true);
			m_ribbonButtonBar5->EnableButton(ID_HELP, true);
			m_ribbonButtonBar4->EnableButton(ID_SHADER_DO_AUTO_INDENT, true);
			m_ribbonButtonBar2->EnableButton(ID_COMPIL_RUN_SHADER, true);
			m_ribbonButtonBar6->EnableButton(ID_EXPORT_SHADER_TO_PICTURE, true);

			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SCREEN, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TIME, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_MOUSE, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURE2D, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURECUBE, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_GAMEPAD360, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SLIDER, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_COLOR, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_EXPORT, true);
			m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_FONT, true);
		}
	}
	else
	{
		m_ribbonButtonBar1->EnableButton(ID_SAVE_DOCUMENT, false);
		m_ribbonButtonBar1->EnableButton(ID_SAVEAS_DOCUMENT, false);
		m_ribbonButtonBar5->EnableButton(ID_HELP, false);
		m_ribbonButtonBar4->EnableButton(ID_SHADER_DO_AUTO_INDENT, false);
		m_ribbonButtonBar2->EnableButton(ID_COMPIL_RUN_SHADER, false);
		m_ribbonButtonBar6->EnableButton(ID_EXPORT_SHADER_TO_PICTURE, false);

		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SCREEN, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TIME, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_MOUSE, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURE2D, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_TEXTURECUBE, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_GAMEPAD360, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_SLIDER, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_COLOR, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_EXPORT, false);
		m_ribbonToolBar1->EnableTool(ID_SHADER_UNIFORM_FONT, false);
	}
	return res;
}

bool MainFrame::CloseAllDocuments(wxCloseEvent& /*event*/)
{
	DocShaderArt *doc = GetCurrentView();
	while (doc != 0)
	{
		if (CheckSavedDocument(doc) == true)
		{
			if (m_ShaderBook->RemovePage(m_ShaderBook->GetPageIndex(doc)) == true)
			{
				SAFE_DELETE(doc);
			}
			else
			{
				string msg = "Due to unknow error, the document can't be closed !\n\r";
				if (m_ShaderBook->GetPageCount() > 1) // si d'autres documents existent
				{
					msg += "If you force the close, the other Documents will not been saved.\nYou can try to save each document themselves for avoid lost !\n\r";
				}
				msg += "Do you want to force the close ?\n";
				// une page n'arrive pas à être enlevée. il faut stopper la procedure et avertir le user
				int res = wxMessageBox(msg, "Problem !", wxYES_NO, this);
				if (res == wxYES)
				{
					// on fait rien et la procedure va continuer
					// ATTENTION !!!
					// ATTENTION !!!
					// ATTENTION !!!
					// ATTENTION !!!
					// faudrait parcourir les autres documents
					return true; // la on quiite au 1er qui merde.......mais on averti le user de ce qu'on va faire donc.... ca passe...
				}
				else
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		doc = GetCurrentView();
	}
	return true;
}

void MainFrame::SetSavedStatusOFCurrentPage(bool vIsSaved)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		string viewName = view->GetDocument()->GetFileName();
		int idx = m_ShaderBook->GetPageIndex(view);
		if (vIsSaved == true)
		{
			m_ShaderBook->SetPageText(idx, viewName);
		}
		else
		{
			m_ShaderBook->SetPageText(idx, viewName + " *");
		}
	}
	
}

void MainFrame::OnCompilAndRunShaderClick(wxRibbonButtonBarEvent& event)
{ 
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		m_OutputWinCompil->AddToOutput(view->GetDocument()->GetFileName() + " view");
		view->OnCompilAndRunShaderClick(event);
	}
}

void MainFrame::OnDoShaderAutoLayout0(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::EL_ST_UT);
	}
}

void MainFrame::OnDoShaderAutoLayout1(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::EL_STR_UBR);
	}
}

void MainFrame::OnDoShaderAutoLayout2(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::ER_STL_UBL);
	}
}

void MainFrame::OnDoShaderAutoLayout3(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::ETR_STL_UB);
	}
}

void MainFrame::OnDoShaderAutoLayout4(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::ETL_STR_UB);
	}
}

void MainFrame::OnDoShaderAutoLayout5(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::ETL_SR_UBL);
	}
}

void MainFrame::OnDoShaderAutoLayout6(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoLayout(ShaderArtLayoutEnum::ETR_SL_UBR);
	}
}

void MainFrame::OnDoShaderAutoIndent(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAutoIndent();
	}
}

void MainFrame::OnDoAnalyseShaderCode(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoAnalyseShaderCode();
	}
}

void MainFrame::OnDoShaderPerfAnalyse(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->DoShaderPerfAnalyse();
	}
}

void MainFrame::OnAddMusicShader(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		view->OnAddMusicShader();
	}
}

void MainFrame::OnPageChanged(wxAuiNotebookEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		ToggleLayoutButton(view->GetCurrentLayout());
		
		SetSavedStatusOFCurrentPage(!view->GetDocument()->IsModified());

		// on resize et on rafraichit
		view->RefreshSizeAndDisplay();
	}
}

void MainFrame::OnPageClosed(wxAuiNotebookEvent& /*event*/) // page fermée
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		ToggleLayoutButton(view->GetCurrentLayout());
	}
	else
	{
		ToggleLayoutButton(ShaderArtLayoutEnum::NONE);
	}
}

void MainFrame::OnPageClose(wxAuiNotebookEvent& event) // page va etre fermée
{
	if (CheckSavedDocument(GetCurrentView()) == false)
	{
		event.Veto();
	}
}

bool MainFrame::CheckSavedDocument(DocShaderArt *vView)
{
	if (vView != 0)
	{
		//on check si le document doit etre sauvé
		if (vView->GetDocument()->IsModified() == true)
		{
			int res = wxMessageBox("Do you want to save the document ?", "Document not saved !", wxYES_NO | wxCANCEL, this);
			if (res == wxYES)
			{
				wxRibbonButtonBarEvent e;
				OnSaveDocClick(e);
				return true;
			}
			else if (res == wxCANCEL)
			{
				return false; // l'op a echoué et doit etre stopé
			}
		}
	}
	return true;
}

void MainFrame::ToggleLayoutButton(ShaderArtLayoutEnum vLay)
{
	DocShaderArt *view = GetCurrentView();
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT0, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT1, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT2, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT3, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT4, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT5, view != 0);
	m_ribbonButtonBar3->EnableButton(ID_SHADER_DO_AUTO_LAYOUT6, view != 0);

	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT0, vLay == ShaderArtLayoutEnum::EL_ST_UT);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT1, vLay == ShaderArtLayoutEnum::EL_STR_UBR);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT2, vLay == ShaderArtLayoutEnum::ER_STL_UBL);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT3, vLay == ShaderArtLayoutEnum::ETR_STL_UB);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT4, vLay == ShaderArtLayoutEnum::ETL_STR_UB);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT5, vLay == ShaderArtLayoutEnum::ETL_SR_UBL);
	m_ribbonButtonBar3->ToggleButton(ID_SHADER_DO_AUTO_LAYOUT6, vLay == ShaderArtLayoutEnum::ETR_SL_UBR);
}

void MainFrame::OnAddUniformScreen(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::USCREEN) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformMouse(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UMOUSE) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformTexture2D(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UTEX2D) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformTextureCube(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UCUBEMAP) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformVideo(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UVIDEO) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformMic(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UMIC) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformSlider(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::USLIDER) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformCurve(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UCURVE) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformWebCam(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UWEBCAM) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformMusic(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UMUSIC) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformTime(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UTIME) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformColor(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UCOLOR) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformGamePad360(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UGAMEPAD360) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformBvh(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UBVH) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformExport(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UEXPORT) == false)
		{
			// error
		}
	}
}

void MainFrame::OnAddUniformFont(wxRibbonToolBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (view->AddUniformWidget(UniformsWidgetTypeEnum::UFONT) == false)
		{
			// error
		}
	}
}

void MainFrame::OnExportToPicture(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		string png = "png file (*.png)|*.png";

		string viewName = view->GetDocument()->GetFileName();
		DebugLogger::ReplaceString(viewName, " ", "_");
		viewName += ".png";
		wxFileDialog fileDlg(this, _("Save As"), wxEmptyString, viewName, png, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (fileDlg.ShowModal() == wxID_OK)
		{
			string filepathname = fileDlg.GetPath().ToStdString();
			view->GetShaderView()->ExportToPictureFile(filepathname);
		}
	}
}

// trick pour changer la config du ribbon quand il est docké a un nouveau bord que le precedent
//https://groups.google.com/forum/#!topic/wx-users/X7-pkbjW51g
void MainFrame::OnRender(wxAuiManagerEvent& event)
{
	event.Skip();
	//wxAuiPaneInfo &pi = event.GetManager()->GetPane(wxT("Ribbon"));
	/*
	if (pi.IsOk() && pi.IsDocked() && pi.dock_direction != m_CurrentRibbonDockDirection)
	{
		// on drie si le ribbon doit etre vertical ou horizontal selon ou il est docké
		int dir = pi.dock_direction;
		if (dir == wxAUI_DOCK_LEFT || dir == wxAUI_DOCK_RIGHT)
			m_ribbonBar1->SetWindowStyleFlag(wxRIBBON_BAR_FLOW_VERTICAL | wxRIBBON_BAR_SHOW_PAGE_ICONS);
		if (dir == wxAUI_DOCK_TOP || dir == wxAUI_DOCK_BOTTOM)
			m_ribbonBar1->SetWindowStyleFlag(wxRIBBON_BAR_FLOW_HORIZONTAL | wxRIBBON_BAR_SHOW_PAGE_ICONS);
		m_ribbonBar1->Layout();
		m_ribbonBar1->Update();
		m_CurrentRibbonDockDirection = dir;
	}
	*/
}

void MainFrame::OnSettings(wxRibbonButtonBarEvent& /*event*/)
{
	SettingsDlg *settings = new SettingsDlg(this, wxID_ANY);
	if (settings->ShowModal() == wxID_OK)
	{
		settings->SaveConfig();
	}
	settings->Destroy();
	SAFE_DELETE(settings);
}

void MainFrame::OnHelp(wxRibbonButtonBarEvent& /*event*/)
{
	DocShaderArt *view = GetCurrentView();
	if (view != 0)
	{
		if (m_HelpDlg == 0)
		{
			m_HelpDlg = new HelpDlg(this, view);
			if (m_HelpDlg->ShowModal() == wxID_OK)
			{

			}
			m_HelpDlg->Destroy();
			SAFE_DELETE(m_HelpDlg);
		}
	}
}

void MainFrame::OnAbout(wxRibbonButtonBarEvent& /*event*/)
{
	wxBoxSizer *topsizer;
	myHtmlWindow *html;
	wxDialog dlg(this, wxID_ANY, wxString(_("About")));

	topsizer = new wxBoxSizer(wxVERTICAL);

	html = new myHtmlWindow(&dlg, ID_ABOUT_HTML_WIN, wxDefaultPosition, wxSize(500, 280), wxHW_SCROLLBAR_NEVER);
	html->SetBorders(0);

	string content = "<html><body bgcolor=\"#FFFFFF\"><table cellspacing=3 cellpadding=4 width=\"100%\">";
	content += "<tr><td bgcolor = \"#101010\"><center>";
	content += "<font size = +2 color = \"#FFFFFF\"><b><br>" + xShadeVersion + "<br>< / b>";
	content += "</font></center></td></tr><tr><td bgcolor = \"#73A183\"><b><font size = -1>Copyright(C) 2015 Stephane Cuillerdier All rights reserved.</font></b><p>";
	content += "<font size = -1><table cellpadding = 0 cellspacing = 0 width = \"100%\">";
	content += "<tr><td width = \"65%\">Web : <a href=\"http://www.funparadigm.com/xshade\">www.funparadigm.com/xshade</a></td></tr>";
	content += "<tr><td width = \"65%\">Twitter : <a href=\"https://twitter.com/aiekick\">@aiekick</a></td></tr>";
	content += "<tr><td width = \"65%\">Mail : <a href=\"mailto:mail@funparadigm.com\">mail@funparadigm.com</a><p></td></tr>";
	content += "<tr><td width = \"65%\">Use : <a href=\"https://www.wxwidgets.org\">wxWidget 3.0.2</a> / <a href=\"http://www.grinninglizard.com/tinyxml2\">TinyXml2</a> / <a href=\"http://glm.g-truc.net/0.9.7/index.html\">Glm</a> / <a href=\"http://glew.sourceforge.net\">Glew</a> / <a href=\"http://www.zlib.net\">Zlib</a> </td></tr>"; 
	content += "<tr><td width = \"65%\">Many assets come from <a href=\"https://www.shadertoy.com\">www.ShaderToy.com</a> <p></td></tr>";
	content += "<tr><td width = \"65%\">This soft is inspired from ShaderToy <p></td></tr>";
	content += "<tr><td width = \"65%\">SoShade is licenced and distributed under the <a href = \"http://opensource.org/licenses/bsd-license.php\">BSD 2-Clause License</a><p></td></tr>";
	content += "</td></tr></table></font></td>";
	content += "</tr></table></body></html>"; 

	html->AppendToPage(content);
	html->SetSize(
		html->GetInternalRepresentation()->GetWidth(),
		html->GetInternalRepresentation()->GetHeight());

	topsizer->Add(html, 1, wxALL, 10);

	topsizer->Add(new wxStaticLine(&dlg, wxID_ANY), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	wxButton *bu1 = new wxButton(&dlg, wxID_OK, _("OK"));
	bu1->SetDefault();

	topsizer->Add(bu1, 0, wxALL | wxALIGN_RIGHT, 15);

	dlg.SetSizer(topsizer);
	topsizer->Fit(&dlg);

	dlg.ShowModal();
}

void MainFrame::OnLinkClicked(wxHtmlLinkEvent& /*event*/)
{

}

void MainFrame::OnMaximise(wxMaximizeEvent& /*event*/)
{
	/*int count = m_ShaderBook->GetPageCount();
	for (int idx = 0; idx < count; idx++)
	{
		wxWindow *win = m_ShaderBook->GetPage(idx);
		if (win != 0)
		{
			DocShaderArt *view = dynamic_cast<DocShaderArt*>(win);
			if (view != 0)
			{
				ShaderArtLayoutEnum lay = view->GetCurrentLayout();
				view->DoAutoLayout(lay);
			}
		}
	}*/
}

void MainFrame::OnSize(wxSizeEvent& /*event*/)
{
	/*int count = m_ShaderBook->GetPageCount();
	for (int idx = 0; idx < count; idx++)
	{
		wxWindow *win = m_ShaderBook->GetPage(idx);
		if (win != 0)
		{
			DocShaderArt *view = dynamic_cast<DocShaderArt*>(win);
			if (view != 0)
			{
				ShaderArtLayoutEnum lay = view->GetCurrentLayout();
				view->DoAutoLayout(lay);
			}
		}
	}*/
}

void MainFrame::SetStatusBarText(string vMsg, int vField)
{
	m_statusBar->SetStatusText(vMsg, vField);
	g_pOutputWin->AddToOutput(vMsg);
}

void MainFrame::OnStatusBarClick(wxMouseEvent& /*event*/)
{
	wxRibbonButtonBarEvent e;
	OnShowHideOutputWinClick(e);
}