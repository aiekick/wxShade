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

#include "GLShader.h"

#include "../src/OutputWin.h" // OutputWin* g_pOutputWin

GLShader::GLShader(string vShaderName) : _isValid(false), m_ShaderName(vShaderName)
{
	LogToOutput(vShaderName + " : ", true, ShaderMsg::SHADER_MSG_OK); // on efface et on ecrit que dalle
}

GLShader::~GLShader()
{
	//glDeleteShader(vertexShader);
	//CHECK_GL_ERROR_DEBUG();
	
	//glDeleteShader(fragmentShader);
	//CHECK_GL_ERROR_DEBUG();
	
	if (m_uProgram)
	{
		glDeleteProgram(m_uProgram);
		CHECK_GL_ERROR_DEBUG();
	}
}

bool GLShader::InitShaderProgram(string vtx, string frag, string uniformSrc, string version, string precision)
{
	_isValid = false;

	bool state = false;

	// Load the vertex/fragment shaders
	GLuint vertexShader = LoadFromString(GL_VERTEX_SHADER, vtx, uniformSrc, version, precision);
	GLuint fragmentShader = LoadFromString(GL_FRAGMENT_SHADER, frag, uniformSrc, version, precision);

	if (vertexShader != 0 && fragmentShader != 0)
	{
		m_uProgram = glCreateProgram();
		LogToOutput("New Shader Program => " + toStr(m_uProgram), false, ShaderMsg::SHADER_MSG_OK);

		if (m_uProgram != 0)
		{
			glAttachShader(m_uProgram, vertexShader);
			glDeleteShader(vertexShader);
			CHECK_GL_ERROR_DEBUG();

			glAttachShader(m_uProgram, fragmentShader);
			glDeleteShader(fragmentShader);
			CHECK_GL_ERROR_DEBUG();

			state = true;
		}
	}
	else
	{

	}

	return state;
}

GLuint GLShader::LoadFromString(GLenum type, string shaderSrc, string uniformSrc, string version, string precision)
{
	GLuint shader;
	GLint compiled;

	
	if (type == GL_FRAGMENT_SHADER)
	{
		// si precision est indefini on essai de le determiner
		// comme c'est un outil de dev on desactive la detection
		// si demandé par les utilisateurs j'en ferai une option
		//if (precision == "")
		//	m_Precision = this->DetermineShaderPrecision(version);
		//else 
			m_Precision = precision;

		if (m_Precision != "")
		{
			if (m_Precision == "highp" || m_Precision == "mediump" || m_Precision == "lowp") // on teste pas le undefined sinon ca posera porbleme avec la localisation dans une future version
				m_Precision = "#ifdef GL_ES\nprecision " + m_Precision + " float;\n#endif\n";
			else
				m_Precision = "";
		}
	}

	m_Version = version;
	m_Uniforms = uniformSrc;
	m_Header = m_Version + m_Precision + m_Uniforms;
	string fullShaderSrc = m_Header + shaderSrc;

	const int countItems = 1;
	const GLchar *sources = fullShaderSrc.c_str();

	// Create the shader object
	shader = glCreateShader(type);
	CHECK_GL_ERROR_DEBUG();

	if (shader == 0)
		return 0;

	//const GLint len = strlen((char*)sources);

	// Load the shader source
	glShaderSource(shader, countItems, &sources, 0);
	CHECK_GL_ERROR_DEBUG();

	// Compile the shader
	glCompileShader(shader);
	CHECK_GL_ERROR_DEBUG();

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	CHECK_GL_ERROR_DEBUG();

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			
			string typeStr = (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
			typeStr += " SHADER COMPILING => FAIL !!!";
			LogToOutput(typeStr, false, ShaderMsg::SHADER_MSG_ERROR);

			LogToOutput(infoLog, false, ShaderMsg::SHADER_MSG_ERROR);

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}
	else
	{
		string typeStr = (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
		typeStr += " SHADER COMPILING +> OK";
		LogToOutput(typeStr, false, ShaderMsg::SHADER_MSG_OK);

		// warnings

		GLint infoLen = 0;

		glGetProgramiv(m_uProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];

			glGetShaderInfoLog(m_uProgram, infoLen, NULL, infoLog);

			LogToOutput(infoLog, false, ShaderMsg::SHADER_MSG_WARNING);

			delete[] infoLog;
		}
	}

	return shader;
}

bool GLShader::LinkShaderProgram()
{
	bool state = false;

	_isValid = false;

	cAssert(m_uProgram != 0, "program not created");

	// Link the program
	glLinkProgram(m_uProgram);
	CHECK_GL_ERROR_DEBUG();

	// Check the link status
	glGetProgramiv(m_uProgram, GL_LINK_STATUS, &linked);
	CHECK_GL_ERROR_DEBUG();

	if (!linked)
	{
		GLint infoLen = 0;

		glGetProgramiv(m_uProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];

			glGetShaderInfoLog(m_uProgram, infoLen, NULL, infoLog);

			LogToOutput("PROGRAM LINKING => FAIL !!!", false, ShaderMsg::SHADER_MSG_ERROR);
			LogToOutput(infoLog, false, ShaderMsg::SHADER_MSG_ERROR);

			delete[] infoLog;
		}

		glDeleteProgram(m_uProgram);
		state = false;
	}
	else
	{
		string typeStr = "PROGRAM LINKING => OK";
		LogToOutput(typeStr, false, ShaderMsg::SHADER_MSG_OK);

		// warnings

		GLint infoLen = 0;

		glGetProgramiv(m_uProgram, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];

			glGetShaderInfoLog(m_uProgram, infoLen, NULL, infoLog);

			LogToOutput(infoLog, false, ShaderMsg::SHADER_MSG_WARNING);

			delete[] infoLog;
		}

		state = true;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	_isValid = state;

	return state;
}

string GLShader::DetermineShaderPrecision(string version)
{
	string hp = "#ifdef GL_ES\nprecision highp float;\n#endif\n";
	string mp = "#ifdef GL_ES\nprecision mediump float;\n#endif\n";
	string lp = "#ifdef GL_ES\nprecision lowp float;\n#endif\n";

	string vstr = "void main() { gl_Position = vec4(1.0); }\n";
	string fstr = "void main() { gl_FragColor = vec4(1.0); }\n";

	if (TestCreateShaderWithPrecision(hp, version) == true)
		return "highp";

	if (TestCreateShaderWithPrecision(mp, version) == true)
		return "mediump";

	if (TestCreateShaderWithPrecision(lp, version) == true)
		return "lowp";

	return "";
}

bool GLShader::TestCreateShaderWithPrecision(string prec, string version)
{
	GLint compiled;
	
	string vtxStr = version + prec + "void main() { gl_Position = vec4(1.0); }\n";
	const char* vtx = vtxStr.c_str();
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);  
	if (vs == 0) return false;
	glShaderSource(vs, 1, &vtx, 0);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compiled);
	if (!compiled) return false;

	string frgStr = version + prec + "void main() { gl_FragColor = vec4(1.0); }\n";
	const char* frg = frgStr.c_str();
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	if (fs == 0) return false;
	glShaderSource(fs, 1, &frg, 0);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compiled);
	if (!compiled) return false;

	GLint linked;

	GLuint pg = glCreateProgram();
	if (pg == 0) return false;
	glAttachShader(pg, vs);	glDeleteShader(vs);
	glAttachShader(pg, fs);	glDeleteShader(fs);
	glLinkProgram(pg);
	glGetProgramiv(pg, GL_LINK_STATUS, &linked);

	glDeleteProgram(pg);

	return (bool)linked;
}

void GLShader::LogToOutput(string vBuild, bool vErase, ShaderMsg vMsg)
{
	LogStr(vBuild);

	if (vMsg == ShaderMsg::SHADER_MSG_ERROR)
		m_Error = vBuild;

	if (vMsg == ShaderMsg::SHADER_MSG_WARNING)
	{
		vector<std::string::size_type> vec = DebugLogger::strContains(vBuild, "warning");
		if (vec.size() > 0)
		{
			m_Warnings = vBuild;
		}
	}

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

void GLShader::Use()
{
	glUseProgram(m_uProgram);
	CHECK_GL_ERROR_DEBUG();
}

void GLShader::UnUse()
{
	glUseProgram(0);
	//CHECK_GL_ERROR_DEBUG();
}

void GLShader::addAttribute(const char* attributeName, GLuint index)
{
	glBindAttribLocation(m_uProgram, index, attributeName);
	CHECK_GL_ERROR_DEBUG();
}

GLint GLShader::getUniformLocationForName(const char* name)
{
	cAssert(name != NULL, "Invalid uniform name");
	cAssert(m_uProgram != 0, "Invalid operation. Cannot get uniform location when program is not initialized");

	GLint ret = glGetUniformLocation(m_uProgram, name); 
	CHECK_GL_ERROR_DEBUG();

	return ret;
}

bool GLShader::IsValid()
{
	return _isValid;
}