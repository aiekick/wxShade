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

#include "GLShaderRenderer.h"

#include "../src/OutputWin.h" // OutputWin* g_pOutputWin

#include "../src/UniformManager.h"

const char* const GLShaderRenderer::_ErrorStrings[] = {
	{ "No Error" },                     // 0
	{ "Unable to get a DC" },           // 1
	{ "ChoosePixelFormat failed" },     // 2
	{ "SelectPixelFormat failed" },     // 3
	{ "wglCreateContext failed" },      // 4
	{ "wglMakeCurrent failed" },        // 5
	{ "wglDeleteContext failed" },      // 6
	{ "SwapBuffers failed" },           // 7
};

// class
GLShaderRenderer::GLShaderRenderer(string vShaderName, UniformManager *vUMgr, GLShaderRendererTypeEnum vRendererType)
: lastShader(0), currentShader(0), m_vbo(0), m_vao(0), m_ibo(0), m_hrc(0),
m_ShaderName(vShaderName), m_UniformManager(vUMgr), m_Points(0), mRendererType(vRendererType), m_RenderMode(GL_TRIANGLES)
{
	//https://en.wikipedia.org/wiki/OpenGL_Shading_Language
	/*
	2.0 ES #version 100
	3.0 es #version 300 es
	1.10.59[1] 	2.0 	April 2004 		#version 110
	1.20.8[2] 	2.1 	September 2006 	#version 120
	1.30.10[3] 	3.0 	August 2008 	#version 130
	1.40.08[4] 	3.1 	March 2009 		#version 140
	1.50.11[5] 	3.2 	August 2009 	#version 150
	3.30.6[6] 	3.3 	February 2010 	#version 330
	4.00.9[7] 	4.0 	March 2010 		#version 400
	4.10.6[8] 	4.1 	July 2010 		#version 410
	4.20.11[9] 	4.2 	August 2011 	#version 420
	4.30.8[10] 	4.3 	August 2012 	#version 430
	4.40[11] 	4.4 	July 2013 		#version 440
	4.50[12] 	4.5 	August 2014 	#version 450
	*/
	GlVersionList["2.0 ES"] = "#version 100";
	GlVersionList["3.0 ES"] = "#version 300 es";
	GlVersionList["2.0"] = "#version 110";
	GlVersionList["2.1"] = "#version 120";
	GlVersionList["3.0"] = "#version 130";
	GlVersionList["3.1"] = "#version 140";
	GlVersionList["3.2"] = "#version 150";
	GlVersionList["3.3"] = "#version 330";
	GlVersionList["4.0"] = "#version 400";
	GlVersionList["4.1"] = "#version 410";
	GlVersionList["4.2"] = "#version 420";
	GlVersionList["4.3"] = "#version 430";
	GlVersionList["4.4"] = "#version 440";
	GlVersionList["4.5"] = "#version 450";
}


GLShaderRenderer::~GLShaderRenderer()
{

}

void GLShaderRenderer::LogToOutput(string vBuild, bool vErase)
{
	LogStr(vBuild);
	
	if (g_pOutputWin != 0)
	{
		if (vErase == true)
		{
			g_pOutputWin->ClearContent();
		}

		if (vBuild != "")
		{
			// on pete la ligne via les \n pour recuperer plusieurs lignes
			std::vector<std::string> vec = DebugLogger::splitString(vBuild, '\n');
			for (std::vector<std::string>::const_iterator i = vec.begin(); i != vec.end(); ++i)
			{
				string word = *i;
				if (word != L"")
				{
					g_pOutputWin->AddToOutput(word);
				}
			}
		}
	}
}

bool GLShaderRenderer::CreateGLContext(HDC vHDC)
{
	// https://www.opengl.org/wiki/Creating_an_OpenGL_Context_(WGL)
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		32,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24,                        //Number of bits for the depthbuffer
		8,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	int nPixelFormat = ChoosePixelFormat(vHDC, &pfd);

	if (nPixelFormat == 0) return false;

	BOOL bResult = SetPixelFormat(vHDC, nPixelFormat, &pfd);

	if (!bResult) return false;

	HGLRC tempContext = wglCreateContext(vHDC);
	wglMakeCurrent(vHDC, tempContext);

	if (gladLoadGL() == 0)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return false;
	}

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

	static const int opengl_4_5[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_4_4[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 4,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_4_3[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_4_2[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_4_1[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_4_0[] =
	{ 
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_3_3[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_3_2[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 2,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_3_1[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0
	};
	static const int opengl_3_0[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,
	};
	static const int opengl_2_1[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 1,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,
	};
	static const int opengl_2_0[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		WGL_CONTEXT_FLAGS_ARB, 0,
		0,
	};
	typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
		(PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	//m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_5); 
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_4); 
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_3); 
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_2);
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_1);
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_4_0); 
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_3_3); 
	//if (m_hrc == 0) 
	//	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_3_2); 
	//if (m_hrc == 0) 
	m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_3_1);
	if (m_hrc == 0)
		m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_3_0);
	if (m_hrc == 0)
		m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_2_1);
	if (m_hrc == 0)
		m_hrc = wglCreateContextAttribsARB(vHDC, 0, opengl_2_0);

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(tempContext);
	wglMakeCurrent(vHDC, m_hrc);
	
	if (!m_hrc)
	{
		LogToOutput("Context Opengl FAIL !", false);
		return false;
	}

	glGetIntegerv(GL_MAJOR_VERSION, &OpenGLDriverVersion[0]);
	MapParams["GL_MAJOR_VERSION"] = toStr(OpenGLDriverVersion[0]);
	
	glGetIntegerv(GL_MINOR_VERSION, &OpenGLDriverVersion[1]);
	MapParams["GL_MINOR_VERSION"] = toStr(OpenGLDriverVersion[1]);

	const char *vendor = (const char*)glGetString(GL_VENDOR);
	if (vendor != 0)
		MapParams["GL_VENDOR"] = toStr(vendor);

	const char *renderer = (const char*)glGetString(GL_RENDERER);
	if (renderer != 0)
		MapParams["GL_RENDERER"] = toStr(renderer);

	const char *glslsVersionbase = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (glslsVersionbase != 0)
		MapParams["GL_GLSL_VERSION_BASE"] = toStr(glslsVersionbase);

	const char *extentions = (const char*)glGetString(GL_EXTENSIONS);
	if (extentions != 0)
		MapParams["GL_EXTENTIONS"] = toStr(extentions);

	LogToOutput("Context Opengl : " + toStr(OpenGLDriverVersion[0]) + "." + toStr(OpenGLDriverVersion[1]), false);
	
	return true;
}

void GLShaderRenderer::DestroyGLContext(HDC vHDC)
{
	if (m_hrc)
	{
		wglMakeCurrent(vHDC, m_hrc);
		
		if (currentShader != 0)
			delete currentShader;

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_vbo);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_vao);

		if (m_Points != 0)
			delete[] m_Points;

		wglDeleteContext(m_hrc);
		m_hrc = NULL;

		wglMakeCurrent(vHDC, NULL);
		wglMakeCurrent(NULL, NULL);
	}
}

bool GLShaderRenderer::SetCurrent(HDC vHDC)
{
	bool res = true;
	
	// making gl context current
	if (wglMakeCurrent(vHDC, m_hrc) == false)
	{
		string error = string("wglMakeCurrent Failed %x\r\n", GetLastError());
		LogToOutput(error, false);
		res = false;
	}
	
	return res;
}

bool GLShaderRenderer::PrepareScene(HDC vHDC)
{
	if (mRendererType == GLShaderRendererTypeQuad)
	{
		return PrepareSceneFor2DQuad(vHDC);
	}
	else if (mRendererType == GLShaderRendererTypePoints)
	{
		/*map<string, string> m_Params;
		m_Params["INF"] = "0";
		m_Params["SUP"] = "100000";
		m_Params["RATIO"] = "0.5";
		m_UniformManager->AddUniformFromScript(UniformsWidgetTypeEnum::USLIDER, m_Params);*/
		// il faudrait ajouter le uniform locké de nombre de vertex donc un uniform a ne pas modifier
		// donc un nouveau system d'uniforms a faire
		return PrepareSceneFor3DPoints(vHDC, 50000);
	}
}

bool GLShaderRenderer::PrepareSceneFor2DQuad(HDC vHDC)
{
	if (SetCurrent(vHDC) == true)
	{
		glClearColor(0.8f, 0.5f, 0.2f, 1.0f);

		float unit = 1.0f;
		float quad[12] = { -unit, -unit,
			unit, -unit,
			-unit, unit,
			unit, -unit,
			unit, unit,
			-unit, unit };

		// gen
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);

		// bind
		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// fill
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* 2 * 6, quad, GL_STATIC_DRAW);
		glVertexAttribPointer((GLuint)0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// unbind
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		return true;
	}

	return false;
}

bool GLShaderRenderer::PrepareSceneFor3DPoints(HDC vHDC, int vCountPoints)
{
	if (SetCurrent(vHDC) == true)
	{
		glClearColor(0.8f, 0.5f, 0.2f, 1.0f);

		m_CountPoints = vCountPoints;

		if (m_Points != 0) 
			delete[] m_Points;
		m_Points = new VTX_POINTS[m_CountPoints];
		for (int i = 0; i < m_CountPoints; i++)
		{
			m_Points[i].x = 0.0f;
			m_Points[i].y = 0.0f;
			m_Points[i].z = 0.0f;
			m_Points[i].i = i;
		}

		// gen
		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		
		// vbo : bind and fill
		glGenBuffers(1, &m_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		
		glBufferData(GL_ARRAY_BUFFER, sizeof(VTX_POINTS)* m_CountPoints, m_Points, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(VERTEX_POS_INDX);
		glVertexAttribPointer(VERTEX_POS_INDX, VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, sizeof(VTX_POINTS), (void*)0);                       //send positions on pipe 0
		glDisableVertexAttribArray(VERTEX_POS_INDX);

		glEnableVertexAttribArray(VERTEX_IDX_INDX);
		glVertexAttribPointer(VERTEX_IDX_INDX, VERTEX_IDX_SIZE, GL_FLOAT, GL_FALSE, sizeof(VTX_POINTS), (void*)(sizeof(float) * VERTEX_POS_SIZE));       //send normals on pipe 1
		glDisableVertexAttribArray(VERTEX_IDX_INDX);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// unbind
		glBindVertexArray(0);
		
		return true;
	}

	return false;
}

void GLShaderRenderer::DrawScene(HDC vHDC)
{
	if (mRendererType == GLShaderRendererTypeQuad)
	{
		DrawSceneFor2DQuad(vHDC);
	}
	else if (mRendererType == GLShaderRendererTypePoints)
	{
		DrawSceneFor3DPoints(vHDC);
	}
}

void GLShaderRenderer::DrawSceneFor2DQuad(HDC vHDC)
{
	if (SetCurrent(vHDC) == true)
	{
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use Shader
		UseShader(true);

		// bind
		glBindVertexArray(m_vao);		// select first VAO

		// draw
		glDrawArrays(m_RenderMode, 0, 6);	// draw first object 6 => decalage 3 coord * 2 (float)

		// unbind
		glBindVertexArray(0);

		// Un Use Shader
		UseShader(false);

		// swap
		glFlush();
		SwapBuffers(vHDC);
	}
}
void GLShaderRenderer::DrawSceneFor3DPoints(HDC vHDC)
{
	if (SetCurrent(vHDC) == true)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Use Shader
		UseShader(true);

		glEnable(GL_DEPTH);
		glEnable(GL_POINT_SPRITE); // accelere le rendu des gros points
		glEnable(GL_VERTEX_PROGRAM_POINT_SIZE); // permet au veretx shader de modifier la taille des points
		glEnable(GL_CULL_FACE); 
		
		glCullFace(GL_BACK);

		// bind
		glBindVertexArray(m_vao);		// select first VAO
		glEnableVertexAttribArray(VERTEX_POS_INDX);
		glEnableVertexAttribArray(VERTEX_IDX_INDX);
		//glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		// draw
		glDrawArrays(m_RenderMode, 0, m_CountPoints);

		// unbind
		glDisableVertexAttribArray(VERTEX_IDX_INDX);
		glDisableVertexAttribArray(VERTEX_POS_INDX);
		glBindVertexArray(0);

		glDisable(GL_CULL_FACE);
		glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		glDisable(GL_POINT_SPRITE);
		glDisable(GL_DEPTH);

		// Un Use Shader
		UseShader(false);

		// swap
		glFlush();
		SwapBuffers(vHDC);
	}
}

// Draws the scene without the final swap for exec time analysis
// disable also the clear color
void GLShaderRenderer::DrawSceneWithoutSwap(HDC vHDC)
{
	if (SetCurrent(vHDC) == true)
	{//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use Shader
		UseShader(true);

		// bind
		glBindVertexArray(m_vao);		// select first VAO

		// draw
		glDrawArrays(GL_TRIANGLES, 0, 6);	// draw first object 6 => decalage 3 coord * 2 (float)

		// unbind
		glBindVertexArray(0);

		// Un Use Shader
		UseShader(false);

		// swap
		//SwapBuffers(vHDC);
	}
}

void GLShaderRenderer::Reshape(HDC vHDC, int x, int y, int w, int h)
{
	if (SetCurrent(vHDC) == true)
	{
		glViewport(x, y, w, h);
	}
}

void GLShaderRenderer::SetError(int e)
{
	// if there was no previous error,
	// then save this one
	if (_ErrorStrings[0] == m_ErrorString)
	{
		m_ErrorString = _ErrorStrings[e];
	}
}

bool GLShaderRenderer::PrepareShader(string vVertex, string vUniform, string vFragment, bool vPerfAnalysisMode)
{
	bool state = false;
	
	m_Error = "";

	// on sauve le sahder actuel au cas ou la modif ne passerai pas sur le nouveau
	lastShader = currentShader;

	// on cree un nouveau shader
	currentShader = 0;
	currentShader = new GLShader(m_ShaderName);

	//LogToOutput("New Shader to try compile before showing", false);
	
	if (currentShader != 0)
	{
		if (vPerfAnalysisMode == true)
		{
			// on ajoute le uniform de test qui doit remplacer gl_FragCoord.xy
			//vUniform = "uniform vec4 PerfCoord;" + vUniform;
			//DebugLogger::ReplaceString(vFragment, "gl_FragCoord", "PerfCoord"); // on remplace les deux termes
		}

		//ajout de la version
		string version = GetGlslVersionDefine(m_GlslVersion);
		if (version != "")
			version += "\n";
		
		if (currentShader->InitShaderProgram(vVertex, vFragment, vUniform, version, m_Precision) == true)
		{
			currentShader->addAttribute("pos", VERTEX_POS_INDX);

			if (mRendererType == GLShaderRendererTypePoints)
				currentShader->addAttribute("vertexId", VERTEX_IDX_INDX);

			currentShader->LinkShaderProgram();
	
			if (m_UniformManager->CompleteMapWithLocFromShader(currentShader) == true)
			{
				if (vPerfAnalysisMode == true)
				{
					ComplexStringStruct uniforms = m_UniformManager->GetUniformsToString();
					int line = uniforms.count;
					int idx = currentShader->getUniformLocationForName("PerfCoord");
					if (idx == -1)
					{
						m_Error += "0(" + toStr(++line) + ")   Cant find PerfCoord in code for perf analysis. this var is only wanted for perf analysis\n";
						m_Error += "0(" + toStr(++line) + ")   PerfCoord is a vec4 and replace gl_FragCoord\n";
						state = false;
					}
					else
						state = true;

					idx = currentShader->getUniformLocationForName("CellSize");
					if (idx == -1)
					{
						m_Error += "0(" + toStr(++line) + ")   Cant find vec2 CellSize in code for perf analysis. this var is only wanted for perf analysis\n";
						m_Error += "0(" + toStr(++line) + ")   CellSize is a vec2 and represent the size of the cell(x,y)\n";
						state = false;
					}
					else
						state = true;
				}
				else
					state = true;
			}

			m_Warnings = currentShader->GetLastShaderWarningString();
			m_Precision = currentShader->GetUsedPrecisionString();
		}
		else // error
		{
			m_Error = currentShader->GetLastShaderErrorString();
			m_Precision = currentShader->GetUsedPrecisionString();
		}

		m_LastHeader = currentShader->GetUsedHeaderString();
	}

	if (state == true)
	{
		delete lastShader;
		lastShader = 0;
	}
	else
	{
		if (currentShader != 0)
			LogToOutput("Fail Compiling Of Shader " + toStr(currentShader->m_uProgram), false);

		if (lastShader != 0)
			LogToOutput("Return to Current Shader " + toStr(lastShader->m_uProgram), false);

		delete currentShader;
		currentShader = 0;
		
		currentShader = lastShader;
		lastShader = 0;
	}

	return state;
}

bool GLShaderRenderer::UseShader(bool vUse)
{
	bool state = false;

	if (currentShader != 0)
	{
		if (currentShader->IsValid() == true)
		{
			if (vUse == true)
			{
				currentShader->Use();

				m_UniformManager->UploadUniforms();

				state = true;
			}
			else
			{
				currentShader->UnUse();
			}
		}
	}

	return state;
}

string GLShaderRenderer::GetGlslVersionDefine(string vVersion)
{
	if (GlVersionList.find(vVersion) != GlVersionList.end())
	{
		return GlVersionList[vVersion];
	}
	return "";
}

void GLShaderRenderer::SetGlslVersion(string vVersion)
{
	m_GlslVersion = vVersion; 
}