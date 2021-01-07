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

#include "ShaderView.h"

#include "wx/msw/private.h"

#include "DocShaderArt.h"

wxBEGIN_EVENT_TABLE(ShaderView, wxWindow)
	EVT_SIZE(ShaderView::OnSize)
	EVT_PAINT(ShaderView::OnPaint)
	EVT_CHAR(ShaderView::OnChar)
	EVT_MOUSE_EVENTS(ShaderView::OnMouseEvent)
	EVT_TIMER(ID_SHADERVIEW_TIMER, ShaderView::OnTimer)
	EVT_TIMER(ID_SHADERVIEW_FPSTIMER, ShaderView::OnFpsTimer)
wxEND_EVENT_TABLE()

ShaderView* ShaderView::Create(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, UniformManager *vUniformManager)
{
	ShaderView *res = 0;

	if (vDocShaderArt != 0 && parent != 0)
	{
		res = new ShaderView(vDocShaderArt, parent, id, pos, size, style, wxT(""), vUniformManager);
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

ShaderView::ShaderView(DocShaderArt* vDocShaderArt, wxWindow* /*parent*/, wxWindowID /*id*/, const wxPoint& /*pos*/, const wxSize& /*size*/, long /*style*/, const wxString& /*name*/, UniformManager *vUniformManager)
: m_hDC(0), m_DocShaderArt(vDocShaderArt), m_GLShaderRenderer(0), m_timer(this, ID_SHADERVIEW_TIMER), m_ShowPerf(false), fpsTab(0), countDisplayForFps(0), 
m_fpstimer(this, ID_SHADERVIEW_FPSTIMER), m_CropModeActive(false), m_UniformManager(vUniformManager)
{

}

static HRGN hrgn;
// Replaces wxWindow::Create functionality, since we need to use a different window class
bool ShaderView::CreateWindow(wxWindow *parent,wxWindowID id,const wxPoint& pos,const wxSize& size,long style,const wxString& name)
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

ShaderView::~ShaderView()
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

bool ShaderView::Init()
{
	UniformManager *mgr = m_DocShaderArt->GetUniformManager();
	if (mgr != 0)
	{
		ShaderArtTypeEnum m_ShaderArtType = m_DocShaderArt->GetDocument()->GetShaderArtType();

		if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD)
		{
			m_GLShaderRenderer = new GLShaderRenderer(this->GetName().ToStdString(), mgr, GLShaderRendererTypeEnum::GLShaderRendererTypeQuad);
		}
		else if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
		{
			m_GLShaderRenderer = new GLShaderRenderer(this->GetName().ToStdString(), mgr, GLShaderRendererTypeEnum::GLShaderRendererTypePoints);
		}
		
		if (m_GLShaderRenderer == 0)
			return false;

		if (m_GLShaderRenderer->CreateGLContext(m_hDC) == false)
		{
			LogStr("Context Creation for GLShaderRenderer Failed " + toStr(GetLastError()) + "\n");
			return false;
		}

		if (m_GLShaderRenderer->PrepareScene(m_hDC) == false)
		{
			LogStr("GLShaderRenderer::PrepareScene Failed " + toStr(GetLastError()) + "\n");
			return false;
		}

		m_fpstimer.Start(1000.0);// 1 second
		Play();
	}
	
	return true;
}

void ShaderView::OnPaint(wxPaintEvent& /*event*/)
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
	m_GLShaderRenderer->DrawScene(m_hDC);

	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	if (ElapsedMicroseconds.QuadPart > 0)
	{
		m_ExecTime = ElapsedMicroseconds.QuadPart / 1000.0f;
	}

	countDisplayForFps++;

	//ShowPerf();
}

void ShaderView::SetScreenSize(int vWidth, int vHeight)
{
	if (m_GLShaderRenderer == 0) return;

	// This is normally only necessary if there is more than one wxGLEWCanvas
	// or more than one wxGLContext in the application.
	m_GLShaderRenderer->SetCurrent(m_hDC);

	m_GLShaderRenderer->Reshape(m_hDC, 0, 0, vWidth, vHeight);

	m_uScreen.x = vWidth;
	m_uScreen.y = vHeight;

	m_DocShaderArt->GetShaderViewToolBar()->SetScreen(vWidth, vHeight, false);

	UniformManager *mgr = m_DocShaderArt->GetUniformManager();
	if (mgr != 0)
		mgr->UpdateUniformByType(UniformsWidgetTypeEnum::USCREEN, m_uScreen);
}

void ShaderView::Pause()
{
	m_timer.Stop();
}

void ShaderView::Play()
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

	//int interval = 1000.0f / refreshRate;

	m_timer.Start(6, false);    // 6 correspond à 1000/6 = 166 fps max. ca suffira
}

void ShaderView::ShowPerf()
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
			m_GLShaderRenderer->DrawScene(m_hDC);

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

void ShaderView::OnSize(wxSizeEvent& event)
{
	if (!IsShownOnScreen())
		return;

	SetScreenSize(event.GetSize().x, event.GetSize().y);
}

void ShaderView::OnChar(wxKeyEvent& /*event*/)
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

void ShaderView::OnMouseEvent(wxMouseEvent& event)
{
	// Allow default processing to happen, or else the canvas cannot gain focus
	// (for key events).
	event.Skip();

	if (m_CropModeActive == true)
	{
		if (event.LeftIsDown() == true)
		{
			if (key.MouseButtonLeftDown == false) // si precedent etait appuyé
			{
				// current when clicked 
				key.old_point = cVec2(event.GetX(), m_uScreen.y - event.GetY());
			}
		}
		else if (event.LeftIsDown() == false)
		{
			if (key.MouseButtonLeftDown == true) // si precedent etait appuyé
			{
				cVec2 current = cVec2(event.GetX(), m_uScreen.y - event.GetY());

				int _w = std::abs(current.x - key.old_point.x);
				int _h = std::abs(current.y - key.old_point.y);
				int _x = std::fminf(current.x, key.old_point.x);
				int _y = std::fminf(current.y, key.old_point.y);

				if (m_GLShaderRenderer != 0 && _w > 0 && _h > 0)
				{
					// This is normally only necessary if there is more than one wxGLEWCanvas
					// or more than one wxGLContext in the application.
					m_GLShaderRenderer->SetCurrent(m_hDC);

					// on reshape juste sans toucher aux autre infos de taille de fenetre
					// ca va utilsier glViewPort
					m_GLShaderRenderer->Reshape(m_hDC, _x, _y, _w, _h);

					m_DocShaderArt->GetShaderViewToolBar()->SetScreen(_w, _h, true);
				}
			}
		}

		key.MouseButtonLeftDown = event.LeftIsDown();
	}
	else
	{
		if (event.LeftIsDown() == true)
		{
			// current when clicked 
			m_uMouse.x = event.GetX();
			m_uMouse.y = m_uScreen.y - event.GetY(); // on inverse pour avoir le 0 en bas et non en haut comme apr defaut sur windows

			if (key.MouseButtonLeftDown == false) // precedend etait non appuyé
			{
				m_uMouse.z = event.GetX();
				m_uMouse.w = m_uScreen.y - event.GetY(); // on inverse pour avoir le 0 en bas et non en haut comme apr defaut sur windows
			}

			key.old_point = cVec2(m_uMouse.x, m_uMouse.y);
		}
		else if (event.LeftIsDown() == false)
		{
			m_uMouse.z = 0;
			m_uMouse.w = 0;
		}
		key.MouseButtonLeftDown = event.LeftIsDown();

		UniformManager *mgr = m_DocShaderArt->GetUniformManager();
		if (mgr != 0)
			mgr->UpdateUniformByType(UniformsWidgetTypeEnum::UMOUSE, m_uMouse);
	}
}

void ShaderView::SetCropMode(bool vMode)
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

bool ShaderView::CompileAndUseVertexAndFragment(ComplexStringStruct vUniform, string vVertex, string vFragment, bool vPerfAnalysisMode)
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

			if (m_GLShaderRenderer->PrepareShader(vVertex, vUniform.lines, vFragment, vPerfAnalysisMode) == false)
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
				// rien a faire si ce n'est provoquer le refresh de la ShaderView
				//Refresh(false, 0);
			}
		}
	}

	return state;
}

void ShaderView::OnTimer(wxTimerEvent& /*event*/)
{
	Refresh(false, 0); // raffraichit la vue à la frequence du timer
}

void ShaderView::OnFpsTimer(wxTimerEvent& /*event*/)
{
	m_Fps = countDisplayForFps;
	countDisplayForFps = 0;
}

void ShaderView::SetRenderMode(string vRenderModeString)
{
	if (m_GLShaderRenderer != 0)
	{
		if (vRenderModeString == "POINTS") m_GLShaderRenderer->SetRenderMode(GL_POINTS);
		else if (vRenderModeString == "LINE_STRIP") m_GLShaderRenderer->SetRenderMode(GL_LINE_STRIP);
		else if (vRenderModeString == "LINE_LOOP") m_GLShaderRenderer->SetRenderMode(GL_LINE_LOOP);
		else if (vRenderModeString == "LINES") m_GLShaderRenderer->SetRenderMode(GL_LINES);
		else if (vRenderModeString == "TRI_STRIP") m_GLShaderRenderer->SetRenderMode(GL_TRIANGLE_STRIP);
		else if (vRenderModeString == "TRI_FAN") m_GLShaderRenderer->SetRenderMode(GL_TRIANGLE_FAN);
		else if (vRenderModeString == "TRIANGLES") m_GLShaderRenderer->SetRenderMode(GL_TRIANGLES);

		m_DocShaderArt->GetDocument()->SetRenderMode(vRenderModeString);
	}
}

string ShaderView::GetRenderMode()
{
	return m_DocShaderArt->GetDocument()->GetRenderMode();
}

void ShaderView::ExportToPictureFile(string vPictureFileName)
{
	try
	{
		HDC hdc = GetHDC();

		cVec2 size = GetScreenSize();

		int bufW = size.x;
		int bufH = size.y;
		int bufSize = 3 * bufW * bufH; // 4 car RGB

		// allocating memory
		BYTE* bmBits = new BYTE[bufSize]; 

		glReadBuffer(GL_FRONT);

		m_GLShaderRenderer->SetCurrent(hdc);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_GLShaderRenderer->DrawScene(hdc);

		glReadBuffer(GL_FRONT);
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glReadPixels(0, 0, bufW, bufH, GL_RGB, GL_UNSIGNED_BYTE, bmBits);

		// Save pixels to file
		wxImage img(bufW, bufH, bmBits, true);
		wxImage imgMirrored = img.Mirror(false);
		if (imgMirrored.SaveFile(vPictureFileName) == false)
		{
			// If the extension wasn't recognized, use PNG
			imgMirrored.SaveFile(vPictureFileName, wxBITMAP_TYPE_PNG);
		}

		delete[] bmBits;
	}
	catch (std::exception &err)
	{
		string errStr = toStr(err.what());
		LogStr("ShaderView::ExportToPictureFile : " + errStr);
	}
}

// necessite le rerendu dans un FBO donc efface la vue
void ShaderView::ExportToPictureFileWithAlpha(string vPictureFileName)
{
	try
	{
		HDC hdc = GetHDC();

		m_GLShaderRenderer->SetCurrent(hdc);

		cVec2 size = GetScreenSize();

		int bufW = size.x;
		int bufH = size.y;
		int bufSize = 4 * bufW * bufH; // 4 car RGBA

		// allocating memory
		BYTE* bmBits = new BYTE[bufSize];

		////////////////////////////////////////
		// FBO /////////////////////////////////
		////////////////////////////////////////

		GLuint fbo = 0;
		GLuint colorTexture = 0; // color texture for color attachment
		//GLuint depthRBO = 0; // render buffer object for depth buffer
		//GLuint viewFramebuffer = 0;

		// generate a texture id
		glGenTextures(1, &colorTexture);

		// bind the texture
		glBindTexture(GL_TEXTURE_2D, colorTexture);

		// set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// create the texture in the GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		// unbind the texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// create a frame buffer object
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

		// attach the color texture to the frame buffer
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);

		// attach depth buffer
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

		// check the frame buffer
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			LogStr("Frame buffer cannot be generated! Status : " + toStr(status));
		}
		else
		{
			
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_GLShaderRenderer->DrawScene(hdc);

			glReadBuffer(GL_FRONT);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glReadPixels(0, 0, bufW, bufH, GL_RGBA, GL_UNSIGNED_BYTE, bmBits);

			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

			// Save pixels to file
			wxImage img(bufW, bufH, bmBits, true);
			wxImage imgMirrored = img.Mirror(false);
			if (imgMirrored.SaveFile(vPictureFileName) == false)
			{
				// If the extension wasn't recognized, use PNG
				imgMirrored.SaveFile(vPictureFileName, wxBITMAP_TYPE_PNG);
			}

		}

		delete[] bmBits;

		
		//glBindFramebuffer(GL_FRAMEBUFFER, viewFramebuffer);
	}
	catch (std::exception &err)
	{
		string errStr = toStr(err.what());
		LogStr("ShaderView::ExportToPictureFile : " + errStr);
	}
}