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

#ifndef __SHADERARTDOCUMENT_H__
#define __SHADERARTDOCUMENT_H__

#include "DocumentAbstract.h"

#include "Globals.h"

#include "..\framework\Config.h"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <sstream>
#include <exception>
using namespace std;

#include <zlib.h>

class UniformManager;

struct ShaderInfosStruct
{
	string m_FragmentString;
	string m_VertexString;
	UniformManager *m_UniformManager;
	string m_GlslVersion;
	string m_FloatPrecision;
	int m_BuildNumber;
	ShaderArtTypeEnum m_ShaderArtType;
	string m_RenderMode;
};

class ShaderArtDocument : public DocumentAbstract
{
public:
	ShaderArtDocument(string vFileName, UniformManager *vUniformManager);
	~ShaderArtDocument();

	// affiche le template de base
	bool NewDocument(ShaderEditorTemplateEnum vShaderEditorTemplate, string vFilePathName);

	bool LoadDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat, bool vGetOnlyType);

	bool ImportDocumentFile(string vFilePathName);
	bool SaveDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat);

	DocumentFileFormat GetFormat() { return m_Format; }
	void SetFormat(DocumentFileFormat vFormat) { m_Format = vFormat; }

	string GetFileMemoryToString(ShaderTypeEnum vShaderTypeEnum);
	void SetStringToFileMemory(ShaderTypeEnum vShaderTypeEnum, string vTxt);

	bool IsModified();
	void SetModified(bool vIsModified);

	string GetFileName() { return m_FileName; }
	string GetFilePathName() { return m_FilePathName; }
	
	DocumentFragmentParseErrorEnum parse(const char* datas);
	DocumentFragmentParseErrorEnum RecursParsing(tinyxml2::XMLElement* vElem);
	
	void SetUniformManager(UniformManager *vMgr){ m_UniformManager = vMgr; }
	UniformManager* GetUniformManager(){return m_UniformManager;}
	
	// https://panthema.net/2007/0328-ZLibString.html
	string compress_string(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION);
	string decompress_string(const std::string& str);

	// http://www.adp-gmbh.ch/cpp/common/base64.html
	void SetGlslVersion(string vVersion) { m_GlslVersion = vVersion;}
	string GetGlslVersion() { return m_GlslVersion; }
	
	void SetFloatPrecision(string vFloatPrecision) { m_FloatPrecision = vFloatPrecision; }
	string GetFloatPrecision() { return m_FloatPrecision; }

	void SetShaderArtType(ShaderArtTypeEnum vShaderArtType) { m_ShaderArtType = vShaderArtType; }
	ShaderArtTypeEnum GetShaderArtType() { return m_ShaderArtType; }

	void SetRenderMode(string vRenderMode) { m_RenderMode = vRenderMode ; }
	string GetRenderMode() { return m_RenderMode; }

	void IncBuildNumber() { m_BuildNumber++; }
	int GetBuildNumber() { return m_BuildNumber; }

private:
	map<string, ShaderInfosStruct> m_Shaders;
	string m_CurrentShaderId;

private:
	string m_FragmentString;
	string m_VertexString;
	string m_MusicShaderString;
	UniformManager *m_UniformManager;
	string m_FilePathName;
	string m_FileName;
	bool m_Modified;
	bool m_NewDocument;
	string m_GlslVersion;
	string m_FloatPrecision;
	DocumentFileFormat m_Format;
	ShaderArtTypeEnum m_ShaderArtType;
	string m_RenderMode;
	bool m_GetOnlyShaderArtType;
	int m_BuildNumber;
};

#endif //__SHADERARTDOCUMENT_H__