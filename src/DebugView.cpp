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

#include "DebugView.h"

#include "wx/msw/private.h"

#include "DocShaderArt.h"

wxBEGIN_EVENT_TABLE(DebugView, wxWindow)
	EVT_SIZE(DebugView::OnSize)
	EVT_PAINT(DebugView::OnPaint)
	EVT_CHAR(DebugView::OnChar)
	EVT_MOUSE_EVENTS(DebugView::OnMouseEvent)
	EVT_TIMER(ID_SHADERVIEW_TIMER, DebugView::OnTimer)
	EVT_TIMER(ID_SHADERVIEW_FPSTIMER, DebugView::OnFpsTimer)
wxEND_EVENT_TABLE()

DebugView* DebugView::Create(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, UniformManager *vUniformManager)
{
	vUniformManager;

	DebugView *res = 0;

	if (vDocShaderArt != 0 && parent != 0)
	{
		res = new DebugView(vDocShaderArt, parent, id, pos, size, style);
		if (res != 0)
		{
			bool state = res->CreateWindow(parent, id, pos, size, style, name);
			if (state == true)
			{
				state = res->Init();
			}
			if (state == false)
			{
				delete res;
				res = 0;
			}
		}
	}

	return res;
}

DebugView::DebugView(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID /*id*/, const wxPoint& /*pos*/, const wxSize& /*size*/, long /*style*/, const wxString& /*name*/)
: m_hDC(0), m_DocShaderArt(vDocShaderArt), m_GLShaderRenderer(0), m_timer(this, ID_SHADERVIEW_TIMER), m_ShowPerf(false), fpsTab(0), countDisplayForFps(0), 
m_fpstimer(this, ID_SHADERVIEW_FPSTIMER), m_CropModeActive(false)
{
	parent;
}

static HRGN hrgn;
// Replaces wxWindow::Create functionality, since we need to use a different window class
bool DebugView::CreateWindow(wxWindow *parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
{
	wxCHECK_MSG(parent, false, wxT("can't create wxWindow without parent"));

	if (!CreateBase(parent, id, pos, size, style, wxDefaultValidator, name))
		return false;

	parent->AddChild(this);

	/*
	A general rule with OpenGL and Win32 is that any window that will have a
	HGLRC built for it must have two flags:  WS_CLIPCHILDREN & WS_CLIPSIBLINGS.
	You can find references about this within the knowledge base and most OpenGL
	books that contain the wgl function descriptions.
	*/
	WXDWORD exStyle = 0;
	DWORD msflags = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	msflags |= MSWGetStyle(style, &exStyle);

	if (!MSWCreate(wxApp::GetRegisteredClassName(wxT("wxGLEWCanvas"), -1, CS_OWNDC),
		NULL, pos, size, msflags, exStyle))
		return false;

	m_hDC = ::GetDC(GetHwnd());
	if (!m_hDC)
		return false;

	//hrgn = CreateRectRgn(0, 0, 100, 100);
	//int res = SelectClipRgn(m_hDC, hrgn);

	return true;
}

DebugView::~DebugView()
{
	Pause();
	m_fpstimer.Stop();
	if (m_GLShaderRenderer == 0) return;

	//DeleteObject(hrgn);

	// TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! //
	// attention bien ce mettre en contexte courant sinon on va cree le sahder sur le contexte d'une autre fenetre
	// et ca va foutre la merde à l'affichage
	m_GLShaderRenderer->SetCurrent(m_hDC);

	m_GLShaderRenderer->DestroyGLContext(m_hDC);

	SAFE_DELETE(m_GLShaderRenderer);
}

bool DebugView::Init()
{
	UniformManager *mgr = m_DocShaderArt->GetUniformManager();
	if (mgr != 0)
	{
		m_GLShaderRenderer = new GLShaderRenderer(this->GetName().ToStdString(), mgr, GLShaderRendererTypeEnum::GLShaderRendererTypeQuad);

		if (m_GLShaderRenderer == 0)
			return false;

		if (m_GLShaderRenderer->CreateGLContext(m_hDC) == false)
		{
			LogStr("Context Creation for GLShaderRenderer Failed " + toStr(GetLastError()) + "\n");
			return false;
		}

		if (m_GLShaderRenderer->PrepareSceneFor2DQuad(m_hDC) == false)
		{
			LogStr("GLShaderRenderer::PrepareScene Failed " + toStr(GetLastError()) + "\n");
			return false;
		}

		m_fpstimer.Start(1000.0);// 1 second
		Play();
	}
	
	return true;
}

void DebugView::OnPaint(wxPaintEvent& /*event*/)
{
	if (m_GLShaderRenderer == 0) return;

	// This is a dummy, to avoid an endless succession of paint messages.
	// OnPaint handlers must always create a wxPaintDC.
	//wxPaintDC dc(this);

	// http://stackoverflow.com/questions/9178100/fps-game-ping-implementation
	// QueryPerformanceCounter

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&StartingTime);

	// Activity to be timed
	m_GLShaderRenderer->SetCurrent(m_hDC);
	m_GLShaderRenderer->DrawSceneFor2DQuad(m_hDC);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	if (ElapsedMicroseconds.QuadPart > 0)
	{
		m_ExecTime = ElapsedMicroseconds.QuadPart / 1000.0f;
		countDisplayForFps += m_ExecTime;
	}

	
	//ShowPerf();
}

void DebugView::SetScreenSize(int vWidth, int vHeight)
{
	if (m_GLShaderRenderer == 0) return;

	// This is normally only necessary if there is more than one wxGLEWCanvas
	// or more than one wxGLContext in the application.
	m_GLShaderRenderer->SetCurrent(m_hDC);

	m_GLShaderRenderer->Reshape(m_hDC, 0, 0, vWidth, vHeight);

	m_uScreen.x = vWidth;
	m_uScreen.y = vHeight;

	//m_DocShaderArt->GetDebugViewToolBar()->SetScreen(vWidth, vHeight, false);

	UniformManager *mgr = m_DocShaderArt->GetUniformManager();
	if (mgr != 0)
		mgr->UpdateUniformByType(UniformsWidgetTypeEnum::USCREEN, m_uScreen);
}

void DebugView::Pause()
{
	m_timer.Stop();
}

void DebugView::Play()
{
	DEVMODE lpDevMode;
	memset(&lpDevMode, 0, sizeof(DEVMODE));
	lpDevMode.dmSize = sizeof(DEVMODE);
	lpDevMode.dmDriverExtra = 0;

	float refreshRate = 60.0f;
	if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &lpDevMode) != 0)
	{
		refreshRate = lpDevMode.dmDisplayFrequency;
	}

	int interval = 1.0f / refreshRate * 1000.0f;

	m_timer.Start(interval, false);    // 1.0/60.0*1000.0 second interval
}

void DebugView::ShowPerf()
{
	if (m_ShowPerf == false) return;

	m_GLShaderRenderer->SetCurrent(m_hDC);

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;

	int cellsizewidth = 1;
	int cellsizeheight = 1;

	w = m_uScreen.x;
	h = m_uScreen.y;

	int nstepw = w / cellsizewidth;
	int nsteph = h / cellsizeheight;

	fpsTab = 0;
	fpsTab = new float[nstepw*nsteph];

	float fpsmin = 1e6;
	float fpsmax = 0;
	for (int i = 0; i < nstepw; i++)
	{
		int x = i * nstepw;
		
		for (int j = 0; j < nsteph; j++)
		{
			int y = j * nsteph;
			
			glViewport(x, y, cellsizewidth, cellsizeheight);

			QueryPerformanceFrequency(&Frequency);
			QueryPerformanceCounter(&StartingTime);

			// Activity to be timed
//			m_GLShaderRenderer->SetCurrent(m_hDC);
			m_GLShaderRenderer->DrawSceneFor2DQuad(m_hDC);

			QueryPerformanceCounter(&EndingTime);
			ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

			ElapsedMicroseconds.QuadPart *= 1000000;
			ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

			if (ElapsedMicroseconds.QuadPart > 0)
			{
				double fps = 1e6 / ElapsedMicroseconds.QuadPart;
				fps = int(fps * 100) / 100.0f; // 2 decimals

				// min max
				if (fps > fpsmax) fpsmax = fps;
				if (fps < fpsmin) fpsmin = fps;

				// on stock dans un tab la valeur
				int id = j * nstepw + i;
				fpsTab[id] = fps;

				LogStr("fpsTab[" + toStr(i) + "][" + toStr(j) + "]=" + toStr(fps));

//				int ii = id % nstepw;
//				int jj = (id - ii) / nstepw;

				//int xx = 2;
			}
		}
	}

	delete[] fpsTab;
	fpsTab = 0;

	m_GLShaderRenderer->Reshape(m_hDC, 0, 0, w, h);
}

void DebugView::OnSize(wxSizeEvent& event)
{
	if (!IsShownOnScreen())
		return;

	SetScreenSize(event.GetSize().x, event.GetSize().y);
}

void DebugView::OnChar(wxKeyEvent& /*event*/)
{
	/*
	switch (event.GetKeyCode())
	{
	case WXK_ESCAPE:
		wxTheApp->ExitMainLoop();
		return;

	default:
		event.Skip();
		return;
	}
	*/
}

void DebugView::OnMouseEvent(wxMouseEvent& /*event*/)
{
	
}

void DebugView::SetCropMode(bool vMode)
{
	m_CropModeActive = vMode;

	if (m_CropModeActive == false)
	{
		if (m_GLShaderRenderer != 0)
		{
			// This is normally only necessary if there is more than one wxGLEWCanvas
			// or more than one wxGLContext in the application.
			m_GLShaderRenderer->SetCurrent(m_hDC);

			// on reshape juste sans toucher aux autre infos de taille de fenetre
			// ca va utilsier glViewPort
			m_GLShaderRenderer->Reshape(m_hDC, 0, 0, m_uScreen.x, m_uScreen.y);
		}
	}
}

bool DebugView::CompileAndUseFragment(ComplexStringStruct vUniform, string vFragment, bool vPerfAnalysisMode)
{
	bool state = false;

	// TRES IMPORTANT !!! // // TRES IMPORTANT !!! // // TRES IMPORTANT !!! //
	// TRES IMPORTANT !!! // // TRES IMPORTANT !!! // // TRES IMPORTANT !!! //
	// attention bien ce mettre en contexte courant sinon on va cree le shader sur le contexte d'une autre fenetre
	// et ca va foutre la merde à l'affichage
	m_GLShaderRenderer->SetCurrent(m_hDC);

	EditorView *editor = m_DocShaderArt->GetEditorView(); // get current editor view ( l'editeur courant esta ctive par le docshaderart seulement avant la compilation
	if (editor != 0)
	{
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			m_GLShaderRenderer->SetGlslVersion(doc->GetGlslVersion());
			m_GLShaderRenderer->SetFloatPrecision(doc->GetFloatPrecision());

			string vertexShader = "attribute vec2 pos; void main() { gl_Position = vec4(pos.xy, 0.0, 1.0); }";
			if (m_GLShaderRenderer->PrepareShader(vertexShader, vUniform.lines, vFragment, vPerfAnalysisMode) == false)
			{
				// error => mode debug
				string error = m_GLShaderRenderer->GetLastShaderErrorString();
				if (error != "")
				{
					int sizeHeader = vUniform.count;
					if (m_GLShaderRenderer->GetFloatPrecision() != "")	sizeHeader += 3;
					if (m_GLShaderRenderer->GetGlslVersion() != "") sizeHeader += 1;
					editor->ShowErrorInCode(error, sizeHeader, ShaderMsg::SHADER_MSG_ERROR);
				}
			}
			else
			{
				state = true;

				string warnings = m_GLShaderRenderer->GetLastShaderWarningString();
				if (warnings != "")
				{
					int sizeHeader = vUniform.count;
					if (m_GLShaderRenderer->GetFloatPrecision() != "")	sizeHeader += 3;
					if (m_GLShaderRenderer->GetGlslVersion() != "") sizeHeader += 1;
					editor->ShowErrorInCode(warnings, sizeHeader, ShaderMsg::SHADER_MSG_WARNING);
				}
				else
				{
					editor->ShowErrorInCode(warnings, 0, ShaderMsg::SHADER_MSG_OK);
				}
				// rien a faire si ce n'est provoquer le refresh de la DebugView
				//Refresh(false, 0);
			}
		}
	}

	return state;
}

void DebugView::OnTimer(wxTimerEvent& /*event*/)
{
	Refresh(false, 0); // raffraichit la vue à la frequence du timer
}

void DebugView::OnFpsTimer(wxTimerEvent& /*event*/)
{
	m_Fps = countDisplayForFps;
	countDisplayForFps = 0;
}