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

#ifndef __DOCSHADERART_H__
#define __DOCSHADERART_H__

#include "Globals.h"

#include "DocVisualControlAbstract.h"

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
#include <wx/aui/auibook.h>
#include <wx/frame.h>
#include <wx/tglbtn.h>
#include <wx/aui/aui.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statline.h>

#include "ShaderArtDocument.h"
#include "UniformManager.h"

#include "ShaderView.h"
#include "EditorView.h"
#include "UniformView.h"

#include "UniformManager.h"

#include "UniformAbstract.h"

#include "ShaderViewToolBar.h"

#include "UniformViewToolBar.h"

#include "..\framework\Config.h"
#include "..\framework\Tools\Tools.hpp"

#include <string>
using namespace std;

enum ImportTypeEnum
{
	INPORT_SHADERTOY = 0,
	INPORT_GLSLSANDBOX = 1
};

enum ExportTypeEnum
{
	EXPORT_SHADERTOY = 0,
	EXPORT_GLSLSANDBOX = 1,
	EXPORT_COCOS2DX = 2
};

class DocShaderArt : public DocVisualControlAbstract
{
private:
	string m_FragmentShader;
	bool m_IsInit;
	ShaderArtLayoutEnum m_DefaultLayout;
	ShaderArtLayoutEnum m_CurrentLayout;

protected:
	virtual void OnSize(wxSizeEvent& event);
	virtual void DoSwitchPerfMode(wxCommandEvent& event);
	virtual void OnPageClose(wxAuiNotebookEvent& event);
	virtual void OnPageClosed(wxAuiNotebookEvent& event);

	bool Init();

	bool ReplaceUniformNameInStringAndCreateUniformForShaderToyImport(string vText, UniformsWidgetTypeEnum vEnum);

protected:
	wxAuiNotebook *m_auinotebook3;
	wxPanel *m_panel3;
	ShaderArtDocument *m_Document;
	UniformManager *m_Uniforms_Mgr;

	wxAuiNotebook *m_auinotebookEditor;

	EditorView *m_EditorMusicView;
	EditorView *m_EditorVertexView;
	EditorView *m_EditorFragmentView;

	cVec2 EditorViewSize;
	cVec2 ShaderViewSize;
	cVec2 UniformViewSize;

	wxPanel *m_uniformpane;
	UniformView *m_UniformView;
	UniformViewToolBar *m_UniformViewToolBar;

	wxButton *m_PerfBtn;
	
	wxPanel *m_shaderpane;
	ShaderView *m_ShaderView;
	ShaderViewToolBar *m_ShaderViewToolBar;

public:
	virtual void OnCompilAndRunShaderClick(wxRibbonButtonBarEvent& event);

public:
	DocShaderArt(wxWindow* parent, ShaderArtTypeEnum vShaderType = ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(630, 355), long style = wxTAB_TRAVERSAL, string vFileName = "");
	~DocShaderArt();

	bool NewDocument(string vFilePathName);
	bool LoadDocument(string vFilePathName, DocumentFileFormat vFileFormat, bool vCompilAfterLoating);
	bool ImportDocumentString(string vDocumentString, ImportTypeEnum vImportType);
	bool ExportDocumentString(string vDocumentString, ExportTypeEnum vImportType);
	bool SaveDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat);
	void CompilAndRunShader(bool vPerfAnalysisMode);
	void RefreshSizeAndDisplay();
	void DoAutoLayout(ShaderArtLayoutEnum vLay);
	void RefreshShaderView();
	void DoAutoIndent();
	void DoAnalyseShaderCode();
	void DoShaderPerfAnalyse();
	void OnAddMusicShader();
	bool OnLoadMusicShader();

	void SetCurrentShader(int vShaderId);

	void RecordGif(float vTime0, float vTime1, float vFps, string vFilepathName);

	ShaderArtLayoutEnum GetCurrentLayout();

	bool AddUniformWidget(UniformsWidgetTypeEnum vType);

	string ValidateWithGlslang();

	ShaderArtDocument* GetDocument(){ return m_Document; }
	ShaderView* GetShaderView(){ return m_ShaderView; }
	ShaderViewToolBar* GetShaderViewToolBar(){ return m_ShaderViewToolBar; }
	UniformViewToolBar* GetUniformViewToolBar(){ return m_UniformViewToolBar; }
	EditorView* GetEditorView() { return static_cast<EditorView*>(m_auinotebookEditor->GetCurrentPage()); }
	UniformView* GetUniformView(){ return m_UniformView; }
	UniformManager* GetUniformManager(){ return m_Uniforms_Mgr; }
	
	wxDECLARE_EVENT_TABLE();
};

#endif //__DOCSHADERART_H__
