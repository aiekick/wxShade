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

#ifndef __DOCUMENTABSTRACT_H__
#define __DOCUMENTABSTRACT_H__

#include <string>

#include "Globals.h"

enum ShaderEditorTemplateEnum
{
	TEMPLATE_QUAD = 0,
	TEMPLATE_POINTS = 1,
	TEMPLATE_MUSIC = 2
};

enum ShaderTypeEnum
{
	SHADER_TYPE_VERTEX = 0,
	SHADER_TYPE_FRAGMENT = 1,
	SHADER_TYPE_GEOMETRY = 2
};

enum ShaderArtTypeEnum
{
	SHADER_TYPE_UNKNOW = 0,
	SHADER_TYPE_FOR_QUAD = 1,
	SHADER_TYPE_FOR_POINTS = 2,
	SHADER_TYPE_FOR_CUSTOM_SHAPE = 3
};

class DocumentAbstract
{
public:
	DocumentAbstract(std::string vFileName)
	{

	}

	virtual bool NewDocument(ShaderEditorTemplateEnum vShaderEditorTemplate, std::string vFilePathName) = 0;
	virtual bool LoadDocumentFile(std::string vFilePathName, DocumentFileFormat vFileFormat, bool vGetOnlyType) = 0;
	virtual bool ImportDocumentFile(std::string vFilePathName) = 0;
	virtual bool SaveDocumentFile(std::string vFilePathName, DocumentFileFormat vFileFormat) = 0;

	virtual bool IsModified() = 0;
	virtual void SetModified(bool vIsModified) = 0;

private:
	bool m_Modified;
	bool m_NewDocument;

};

#endif //__DOCUMENTABSTRACT_H__