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

#ifndef __GLRenderer_H__
#define __GLRenderer_H__

#include "wx/window.h"

#include "../src/ShaderArtDocument.h"

#include <glad/glad.h>
#include "GLShader.h"

#include <map>
#include <list>
#include <string>

enum GLShaderRendererTypeEnum
{
	GLShaderRendererTypeQuad = 0,
	GLShaderRendererTypePoints = 1
};

#define VERTEX_POS_SIZE 3
#define VERTEX_IDX_SIZE 1

#define VERTEX_POS_INDX 0
#define VERTEX_IDX_INDX 1

struct VTX_POINTS
{
	float x, y, z;
	float i;
};

class GLShaderRenderer
{
public:
	std::map<std::string, std::string> MapParams;

public:
	GLShaderRenderer(string vShaderName, UniformManager *vUMgr, GLShaderRendererTypeEnum vRendererType);
	virtual ~GLShaderRenderer(void);
	
	bool CreateGLContext(HDC vHDC); 	// Creates OpenGL Rendering Context
	void DestroyGLContext(HDC vHDC);		// Cleanup
	
	bool PrepareScene(HDC vHDC);		// Scene preparation stuff
	bool PrepareSceneFor2DQuad(HDC vHDC);		// Scene preparation stuff
	bool PrepareSceneFor3DPoints(HDC vHDC, int vCountPoints); // Scene preparation stuff for points mode ( like vertexshaderart )
	
	void DrawScene(HDC vHDC);		// Draws the scene
	void DrawSceneFor2DQuad(HDC vHDC);		// Draws the scene
	void DrawSceneFor3DPoints(HDC vHDC);		// Draws the scene
	void DrawSceneWithoutSwap(HDC vHDC); // Draws the scene without the final swap for exec time analysis
	
	void Reshape(HDC vHDC, int x, int y, int w, int h);	// Changing viewport
	
	bool SetCurrent(HDC vHDC);
	void LogToOutput(string vBuild, bool vErase);

	UniformManager* GetUniformManager(){ return m_UniformManager; }

	string GetGlslVersionDefine(string vVersion);
	std::map<string, string> GetGlslVersionMap() { return GlVersionList; }

	void SetGlslVersion(string vVersion);
	string GetGlslVersion(){ return m_GlslVersion; }

	void SetFloatPrecision(string vFloatPrecision){ m_Precision = vFloatPrecision; }
	string GetFloatPrecision(){ return m_Precision; }

	string GetLastHeader(){	return m_LastHeader;	}

	GLShaderRendererTypeEnum GetRendererType(){ return mRendererType; }

	void SetRenderMode(GLenum vRenderMode){ m_RenderMode = vRenderMode; }
	GLenum GetRenderMode(){ return m_RenderMode; }

public:
	bool PrepareShader(string vVertex, string vUniform, string vFragment, bool vPerfAnalysisMode);
	bool UseShader(bool vUse);
	string GetLastShaderErrorString(){ return m_Error; }
	string GetLastShaderWarningString(){ return m_Warnings; }

// Render Buffer
public:
	//bool AddRenderBuffer(string vId);
	//bool PrepareRenderBuffer(string vId, string vVertex, string vUniform, string vFragment, bool vPerfAnalysisMode);
	//bool RemoveRenderBuffer(string vId);

private:
	string m_LastHeader;
	GLShader *currentShader;
	GLShader *lastShader;
	int attPosition;
	string m_Error;
	string m_Warnings;
	string m_Precision;
	std::map<string, string> GlVersionList;
	string m_GlslVersion;
	int OpenGLDriverVersion[2];
	GLShaderRendererTypeEnum mRendererType;
	GLenum m_RenderMode; // GL_TRIANGLES, GL_POINTS, etc...

// Uniform
private:
	string m_ShaderName;
	UniformManager *m_UniformManager;
	VTX_POINTS *m_Points;
	int m_CountPoints;

protected:
	HGLRC m_hrc;                        // OpenGL Rendering Context 
	GLuint m_vbo;
	GLuint m_ibo;
	GLuint m_vao;

private:
	void SetError(int e);
	static const char* const _ErrorStrings[];
	const char* m_ErrorString;
};

#endif /* __GLRenderer_H__ */