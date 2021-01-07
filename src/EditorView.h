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

#ifndef __EDITORVIEW_H__
#define __EDITORVIEW_H__

#include <wx/wx.h>
#include <wx/stc/stc.h> // wxStyledTextCtrl

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
#include <wx/aui/auibook.h>
#include <wx/frame.h>
#include <wx/tglbtn.h>
#include <wx/aui/aui.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statline.h>

#include "..\framework\Config.h"

#include "ShaderArtDocument.h"

#include <string>
using namespace std;

class DocShaderArt;

#define ID_STYLED_TEXT_CTRL 4578
class EditorView :	public wxPanel
{
public:
	static EditorView* Create(DocShaderArt *vDocShaderArt, ShaderTypeEnum vShaderType, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL);

public:
	EditorView(DocShaderArt *vDocShaderArt, ShaderTypeEnum vShaderType, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL);
	~EditorView();

	void LoadFileMemoryToEditor();
	void SaveEditorToFileMemory();

	void ShowErrorInCode(string vError, int vOffset, ShaderMsg vMsg);

	void SetCode(string vCode);
	string GetCode(){ return m_scintillaCode->GetText().ToStdString(); }
	string GetHeader(){ UpdateHeaderText(); return m_scintillaHeader->GetText().ToStdString(); }

	void DoAutoIndent();
	void DoAnalyseShaderCode();

	ShaderTypeEnum GetShaderType(){ return m_ShaderType; }

	void UpdateHeaderText();

	void UpdateGlslVersionAndFloatPrecisionComboBoxs();

	void ShowHideToolBar(bool vMode);

	bool Init();

	string GetSelection();
	void ReplaceSelectionBy(string vReplaceString);

	vector<std::string::size_type> GetOccurencesOfWord(string vWord, bool vMayWizzled);
	void ReplaceOccurenceOfWordBy(string vCurrentWord, string vNewWord, bool vMayWizzled);

	void SetVisibleWord(int vCharStart, int vCountChar);

private:
	wxStyledTextCtrl *CreateScintillaControl();

	enum mark_Type
	{
		marginError = 1,
		marginBreak = 1,

		markerError = 1,
		markerBreak
	};

	bool m_IsInit;

private:
	DocShaderArt* m_DocShaderArt;

	wxStyledTextCtrl* m_scintillaHeader;
	wxStyledTextCtrl* m_scintillaCode;
	ShaderTypeEnum m_ShaderType;
	long m_CurrentHeaderSize;

	wxPanel* m_panel1;
	wxStaticText* m_staticText1;
	wxChoice* m_choice1;
	wxChoice* m_choice2;
	wxStaticLine* m_staticline1;
	wxToggleButton *m_ShowHideHeaderBtn;

protected:
	virtual void OnCharAdded(wxStyledTextEvent &event);
	virtual void OnTextChange(wxStyledTextEvent &event);
	virtual void OnMarginClick(wxStyledTextEvent &event);
	virtual void OnGLSLVersionChoice(wxCommandEvent& event);
	virtual void OnFloatPrecisionChoice(wxCommandEvent& event);
	virtual void OnShowHideHeader(wxCommandEvent& event);

	wxDECLARE_NO_COPY_CLASS(EditorView);
	wxDECLARE_EVENT_TABLE();
};

#endif //__EDITORVIEW_H__