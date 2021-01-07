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

#ifndef __SHADER_H__
#define __SHADER_H__

#include "Config.h"
#include "Tools\Tools.hpp"

// ATTENTION
/*
1) InitShaderProgram
2) ajoute les attribute par addAttribute
3) link => LinkShaderProgram
4) updateUniforms // init les matrices
*/
// glGetUniformLocation peut renvoyer -1 si un uniform est déclaré dans un shader mais jamais utilisé. opengl fait ces simplifications

class GLShader
{
private:
	//GLint m_uUniforms[kUniform_MAX];
	GLenum err;
	bool _isValid;

public:
	GLuint m_uProgram;
	GLint linked;
	string m_ShaderName;

public:
	GLShader(string vShaderName);
	~GLShader();

	bool IsValid();
	bool InitShaderProgram(string vtx, string frag, string uniformSrc, string version, string precision);
	bool LinkShaderProgram();
	GLuint LoadFromString(GLenum type, string shaderSrc, string uniformSrc, string version, string precision);

	void Use();
	void UnUse();

	void LogToOutput(string vBuild, bool vErase, ShaderMsg vMsg);

	GLint getUniformLocationForName(const char* name);
	
	void addAttribute(const char* attributeName, GLuint index);

	string GetLastShaderErrorString(){ return m_Error; }
	string GetLastShaderWarningString(){ return m_Warnings; }
	string GetUsedPrecisionString(){ return m_Precision; }
	string GetUsedGlslVersionString(){ return m_Version; }
	string GetUsedUniformsString(){ return m_Uniforms; }
	string GetUsedHeaderString(){ return m_Header; }

private:
	bool TestCreateShaderWithPrecision(string prec, string version);
	string DetermineShaderPrecision(string version);
	string m_Error;
	string m_Warnings;
	string m_Precision;
	string m_Version;
	string m_Uniforms;
	string m_Header;
};
#endif /* __SHADER_H__ */
