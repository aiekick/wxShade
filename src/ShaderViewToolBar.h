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

#ifndef __SHADERVIEWTOOLBAR_H__
#define __SHADERVIEWTOOLBAR_H__

#include "Globals.h"

#include "wx/wxprec.h"

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
#include <wx/stattext.h>
#include <wx/statline.h>
#include <wx/bmpbuttn.h>
#include <wx/choice.h>

#include "wx/timer.h"
#include "wx/math.h"
#include "wx/window.h"

#include "..\framework\Config.h"
#include "..\framework\Tools\Tools.hpp"
#include "..\framework\GLShaderRenderer.h"

class ShaderView;

//#define ShaderViewName "ShaderViewToolBar"
class ShaderViewToolBar : public wxPanel
{
public:
	ShaderViewToolBar(ShaderView *vShaderView, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);

	virtual ~ShaderViewToolBar();

	bool Init();

	void SetFps(float vFps);
	void SetScreen(int vWidth, int vHeight, bool vIsCrop);
	void SetExecTime(float vExec);

	bool IsCropModeActive(){ return m_CropModeActive; }
	bool IsDebugModeActive(){ return m_DebugModeActive; }

	void SetRenderMode(string vRenderMode);

private:
	wxBitmapButton* m_ConfigBtn;
	wxChoice *m_RenderMode;
	wxBitmapToggleButton* m_TogglePlayPauseBtn;
	wxBitmapToggleButton* m_ToggleCropBtn;
	wxBitmapToggleButton* m_ToggleDebugBtn;
	wxStaticLine* m_staticline8;
	wxStaticText* m_StaticTextFps;
	wxStaticLine* m_staticline81;
	wxStaticText* m_StaticTextSize;
	wxStaticLine* m_staticline811;
	wxStaticText* m_StaticTextRefreshTime;

private:
	ShaderView *m_ShaderView;

	wxTimer	m_FpsComputeTimer; // timer

	float m_LastFps;
	float m_LastExec;

	bool m_CropModeActive;
	bool m_DebugModeActive;

protected:
	virtual void DoPausePlayView(wxCommandEvent& event);
	virtual void DoCropMode(wxCommandEvent& event);
	virtual void DoDebugMode(wxCommandEvent& event);
	virtual void DoConfig(wxCommandEvent& event);
	virtual void OnRenderModeChoice(wxCommandEvent& event);

	virtual void OnTimer(wxTimerEvent& event);

protected:
	wxDECLARE_NO_COPY_CLASS(ShaderViewToolBar);
	wxDECLARE_EVENT_TABLE();
};

#endif //__SHADERVIEWTOOLBAR_H__