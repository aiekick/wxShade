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

#ifndef __UNIFORMABSTRACT_H__
#define __UNIFORMABSTRACT_H__

#include "Globals.h"

#include <wx/wx.h>

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "..\framework\Tools\Tools.hpp"

#include "ShaderView.h"

class UniformManager;
class cUniformVariant;
class UniformViewItem;
class DocShaderArt;

#include <string>
using namespace std;

class UniformAbstract : public wxPanel
{
public:
	UniformAbstract(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);

	~UniformAbstract();

	// fonctions virtuelles pures
	virtual string Init(string vCodeSelection) = 0;
	virtual string ReplaceUniformInCode(string vCode) = 0;
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/) = 0;
	virtual void SetParamsFromXML(map<string, string> vParams) = 0;
	
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual void SetShaderView(ShaderView* vShaderView);
	virtual void SetDocShaderArt(DocShaderArt* vDocShaderArt);

	virtual void SetType(UniformsWidgetTypeEnum vType);
	virtual UniformsWidgetTypeEnum GetType();

	virtual void UpdateValue(cUniformVariant vValue);
	virtual cUniformVariant GetValue();
	
	virtual void AddUniformName(string vName) 
	{ 
		if (vName.size() > 0)
		{
			m_UniformNames.push_back(vName);
			m_mainUniformName = *m_UniformNames.begin();
		}
		
	}
	virtual list<string> GetUniformNames() 
	{
		return m_UniformNames;
	};

	virtual void ChangeUniformName(string vCurrentName, string vNewName);

	virtual void SetUniformViewItem(UniformViewItem *vUniformViewItem);

	virtual void UpdateUniformByName(string vUniformName, cUniformVariant vValue, bool vModified);

public:
	string m_mainUniformName;
	list<string> m_UniformNames;
	cUniformVariant m_value;
	UniformsWidgetTypeEnum m_type;
	int m_typeId; // id du type / reprenste le nom de uniform du meme type. permet de savoir si c'est le 0 ou  le 1, le 2 etc...
	map<string, string> m_Params;
	bool IsMergeInCodeAvailable;
	bool InitIsFailed;

protected:
	GLShaderRenderer *m_ShaderRenderer;
	UniformManager *m_UniformManager;
	ShaderView *m_ShaderView;
	UniformViewItem *m_UniformViewItem;
	DocShaderArt *m_DocShaderArt;
};

#endif //__UNIFORMABSTRACT_H__