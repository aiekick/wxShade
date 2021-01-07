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

#ifndef __DEBUGVIEW_H__
#define __DEBUGVIEW_H__

#include "Globals.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/timer.h"
#include "wx/math.h"
#include "wx/window.h"

#include "..\framework\Config.h"
#include "..\framework\Tools\Tools.hpp"
#include "..\framework\GLShaderRenderer.h"

#include "ShaderArtDocument.h"

class DocShaderArt;
struct ComplexStringStruct;

#define DebugViewName2 "DebugView"
class DebugView : public wxWindow
{
public:
	static DebugView* Create(DocShaderArt* vDocShaderArt, wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = DebugViewName2,
		UniformManager *vUniformManager = 0);
public:
	DebugView(DocShaderArt* vDocShaderArt, wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = DebugViewName2);

	virtual ~DebugView();

	bool Init();

	// get the HDC used for OpenGL rendering
	HDC GetHDC() const { return m_hDC; }

	void OnPaint(wxPaintEvent& /*event*/);
	void OnSize(wxSizeEvent& /*event*/);
	void OnChar(wxKeyEvent& /*event*/);
	void OnMouseEvent(wxMouseEvent& /*event*/);

	bool CompileAndUseFragment(ComplexStringStruct vUniform, string vFrgament, bool vPerfAnalysisMode = false);

	void Pause();
	void Play();

	float GetFPS(){ return m_Fps; }
	float GetExecTime(){ return m_ExecTime; }

	cVec2  GetScreenSize(){ return cVec2(m_uScreen.x, m_uScreen.y); }

	void SetCropMode(bool vMode);

public:
//	KeyState key;
	GLfloat m_rQuad;
	GLfloat m_rTri;
	double zoom;
	double rot_y;
	double rot_x;
	int h, w;
	cUniformVariant m_uMouse;
	cUniformVariant m_uScreen;

// performance
public:
	void ShowPerf();
	float *fpsTab;
	bool m_ShowPerf;
	wxTimer	m_timer; // timer
	wxTimer	m_fpstimer; // timer
	int countDisplayForFps;

// Uniform
public:
	void SetScreenSize(int vWidth, int vHeight);
	GLShaderRenderer* GetShaderRenderer() { return m_GLShaderRenderer; }
	HDC GetHDC(){ return m_hDC; }

private:
	DocShaderArt* m_DocShaderArt;

	HGLRC m_hRC;// opengl context
	HDC m_hDC;// HDC for this window, we keep it all the time
	GLShaderRenderer *m_GLShaderRenderer;

	float m_Fps;
	float m_ExecTime;

	bool m_CropModeActive;

protected:
	bool CreateWindow(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = DebugViewName2);

	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnFpsTimer(wxTimerEvent& event);

	wxDECLARE_NO_COPY_CLASS(DebugView);
	wxDECLARE_EVENT_TABLE();
};

#endif //__DEBUGVIEW_H__