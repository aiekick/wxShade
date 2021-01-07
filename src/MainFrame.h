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

#pragma once

#include "Globals.h"

#include "..\BuildInc\Build.h"

#include <sstream>
#include <string>
#include <map>
using namespace std;

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/ribbon/buttonbar.h>
#include <wx/ribbon/panel.h>
#include <wx/ribbon/page.h>
#include <wx/ribbon/control.h>
#include <wx/ribbon/art.h>
#include <wx/ribbon/bar.h>
#include <wx/ribbon/toolbar.h>
#include <wx/aui/auibook.h>
#include <wx/frame.h>
#include <wx/aui/aui.h>
#include <wx/fs_mem.h>
#include <wx/fs_inet.h>
#include <wx/html/htmlwin.h>
#include "wx/imagpng.h"
#include "wx/wxhtml.h"
#include "wx/statline.h"
#include <wx/statusbr.h>
#include <wx/dnd.h>

#include "..\framework\Config.h"
#include "DocShaderArt.h"
#include "OutputWin.h"
#include "SettingsDlg.h"

//#include <wx/msw/wx.rc>

#include <wx/filefn.h>

class HelpDlg;
class MainFrame;

class myFileDropTarget : public wxFileDropTarget
{
public:
	myFileDropTarget();
	~myFileDropTarget();
	virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames);
};

class myHtmlWindow : public wxHtmlWindow
{// pour appeler le browser externe plutot que charger la page dans la fenetre
public:
	myHtmlWindow(wxWindow *parent, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
		long style = wxHW_SCROLLBAR_AUTO, const wxString& name = _T("htmlWindow"))
		: wxHtmlWindow(parent, id, pos, size, style, name)
	{

	}
	void OnLinkClicked(const wxHtmlLinkInfo& link)
	{
		if (link.GetHref().StartsWith(_T("http://")) || link.GetHref().StartsWith(_T("https://")) || link.GetHref().StartsWith(_T("mailto")))
			wxLaunchDefaultBrowser(link.GetHref());
		else
			wxHtmlWindow::OnLinkClicked(link);
	}
};

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

	DocShaderArt* GetCurrentView();
	void SetSavedStatusOFCurrentPage(bool vIsSaved);

	bool CloseAllDocuments(wxCloseEvent& event);

	void ToggleLayoutButton(ShaderArtLayoutEnum vLay);

	bool CheckSavedDocument(DocShaderArt *view);

	void SetStatusBarText(string vMsg, int vField);

	void Openfiles(wxArrayString vPaths, bool vCompilAfterLoating);

	void CheckUsedMemory();

	void UpdateTitleWidthExtraInfos(string vInfos);

private:
	void InitControls();
	void UnInitControls();
	void OnSizeEvent(wxSizeEvent& evt);
	
private:
	HelpDlg *m_HelpDlg;
	myFileDropTarget *m_RibbonFileDropTarget;
	myFileDropTarget *m_BookFileDropTarget;
	wxTimer	m_CheckUsedMemoryTimer; // timer fro check current memory used

protected: // ribbon bar click
	virtual void OnCheckUsedMemoryTimer(wxTimerEvent& event);
	virtual void OnNewDocClick(wxRibbonButtonBarEvent& event);
	virtual void OnNewDropdown(wxRibbonButtonBarEvent& event);
	virtual void OnOpenDocClick(wxRibbonButtonBarEvent& event);
	virtual void OnOpenDropdown(wxRibbonButtonBarEvent& event);
	virtual void OnOpenShaderWithoutCompilation(wxCommandEvent& event);
	virtual void OnImportFromShaderToy(wxCommandEvent& event);
	virtual void OnNewShaderForQuad(wxCommandEvent& event);
	virtual void OnNewShaderForPoints(wxCommandEvent& event);
	virtual void OnNewShaderForCustomShape(wxCommandEvent& event);
	virtual void OnExportToShaderToy(wxCommandEvent& event);
	virtual void OnSaveDocClick(wxRibbonButtonBarEvent& event);
	virtual void OnSaveAsDocClick(wxRibbonButtonBarEvent& event);
	virtual void OnSaveAsDropdown(wxRibbonButtonBarEvent& event);
	virtual void OnDeleteDocClick(wxRibbonButtonBarEvent& event);
	virtual void OnShowHideOutputWinClick(wxRibbonButtonBarEvent& event);
	virtual void OnCompilAndRunShaderClick(wxRibbonButtonBarEvent& event);
	virtual void OnExportToGif(wxCommandEvent& event);
	virtual void OnRender(wxAuiManagerEvent& event);
	virtual void OnDoShaderAutoLayout0(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout1(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout2(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout3(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout4(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout5(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoLayout6(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderAutoIndent(wxRibbonButtonBarEvent& event);
	virtual void OnDoAnalyseShaderCode(wxRibbonButtonBarEvent& event);
	virtual void OnDoShaderPerfAnalyse(wxRibbonButtonBarEvent& event);
	virtual void OnAddMusicShader(wxRibbonButtonBarEvent& event);
	virtual void OnPageChanged(wxAuiNotebookEvent& event);
	virtual void OnPageClose(wxAuiNotebookEvent& event);
	virtual void OnPageClosed(wxAuiNotebookEvent& event);
	virtual void OnAddUniformScreen(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformMouse(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformTexture2D(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformTextureCube(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformVideo(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformMic(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformSlider(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformCurve(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformWebCam(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformMusic(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformTime(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformColor(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformGamePad360(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformBvh(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformExport(wxRibbonToolBarEvent& event);
	virtual void OnAddUniformFont(wxRibbonToolBarEvent& event);
	virtual void OnSettings(wxRibbonButtonBarEvent& event);
	virtual void OnHelp(wxRibbonButtonBarEvent& event);
	virtual void OnAbout(wxRibbonButtonBarEvent& event);
	virtual void OnLinkClicked(wxHtmlLinkEvent& event);
	virtual void OnMaximise(wxMaximizeEvent& event);
	virtual void OnSize(wxSizeEvent& event);
	virtual void OnStatusBarClick(wxMouseEvent& event);
	virtual void OnExportToPicture(wxRibbonButtonBarEvent& event);

protected: 
	virtual void OnClose(wxCloseEvent& event);

protected:
	wxAuiManager m_mgr;

	// Ribbon
	int m_CurrentRibbonDockDirection;
	wxRibbonBar* m_ribbonBar1;
	wxRibbonPage* m_ribbonPage1;
	wxRibbonPanel* m_ribbonPanel1;
	wxRibbonButtonBar* m_ribbonButtonBar1;
	wxRibbonPanel* m_ribbonPanel4;
	wxRibbonButtonBar* m_ribbonButtonBar3;
	wxRibbonPage* m_ribbonPage10;
	wxRibbonPanel* m_ribbonPanel3;
	wxRibbonButtonBar* m_ribbonButtonBar2;
	wxRibbonPanel* m_ribbonPanel5;
	wxRibbonToolBar* m_ribbonToolBar1;
	wxRibbonPanel* m_ribbonPanel8;
	wxRibbonPanel* m_ribbonPanel9;
	wxRibbonButtonBar* m_ribbonButtonBar4;
	wxRibbonPanel* m_ribbonPane21;
	wxRibbonButtonBar* m_ribbonButtonBar6;
	wxRibbonPanel* m_ribbonPane20;
	wxRibbonButtonBar* m_ribbonButtonBar5;

	// NoteBook
	wxAuiNotebook* m_ShaderBook;

	// Output Window
	wxAuiNotebook* m_OutputBook;

	string xShadeVersion;

public:
	OutputWin *m_OutputWinCompil;
	wxStatusBar* m_statusBar;

	wxDECLARE_EVENT_TABLE();
};

