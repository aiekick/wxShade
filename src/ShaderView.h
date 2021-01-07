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

#ifndef __SHADERVIEW_H__
#define __SHADERVIEW_H__

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

class KeyState
{
public:
	KeyState()
	{
		MouseButtonLeftDown = false;
		MouseButtonMiddleDown = false;
		MouseButtonRightDown = false;
		KeyBoardAltDown = false;
		old_point.Set(0, 0);
		osc_x = osc_y = osc_z = 0.0;
	}
	bool MouseButtonLeftDown;
	bool MouseButtonMiddleDown;
	bool MouseButtonRightDown;
	bool KeyBoardAltDown;
	cVec2 old_point;
	double osc_x;
	double osc_y;
	double osc_z;
};

#define ShaderViewName "ShaderView"
class ShaderView : public wxWindow
{
public:
	static ShaderView* Create(DocShaderArt* vDocShaderArt, wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = ShaderViewName,
		UniformManager *vUniformManager = 0);
public:
	ShaderView(DocShaderArt* vDocShaderArt, wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = ShaderViewName,
		UniformManager *vUniformManager = 0);

	virtual ~ShaderView();

	bool Init();

	// get the HDC used for OpenGL rendering
	HDC GetHDC() const { return m_hDC; }

	void OnPaint(wxPaintEvent& /*event*/);
	void OnSize(wxSizeEvent& /*event*/);
	void OnChar(wxKeyEvent& /*event*/);
	void OnMouseEvent(wxMouseEvent& /*event*/);

	bool CompileAndUseVertexAndFragment(ComplexStringStruct vUniform, string vVertex, string vFragment, bool vPerfAnalysisMode = false);

	void Pause();
	void Play();

	float GetFPS(){ return m_Fps; }
	float GetExecTime(){ return m_ExecTime; }

	cVec2  GetScreenSize(){ return cVec2(m_uScreen.x, m_uScreen.y); }

	void SetCropMode(bool vMode);

	void SetRenderMode(string vRenderModeString);
	string GetRenderMode();

	void ExportToPictureFile(string vPictureFileName);
	void ExportToPictureFileWithAlpha(string vPictureFileName);

public:
	KeyState key;
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

	UniformManager *m_UniformManager;

	float m_Fps;
	float m_ExecTime;

	bool m_CropModeActive;

protected:
	bool CreateWindow(wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = ShaderViewName);

	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnFpsTimer(wxTimerEvent& event);

	wxDECLARE_NO_COPY_CLASS(ShaderView);
	wxDECLARE_EVENT_TABLE();
};

#endif //__SHADERVIEW_H__