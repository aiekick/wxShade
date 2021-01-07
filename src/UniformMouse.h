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

#ifndef __UNIFORMMOUSE_H__
#define __UNIFORMMOUSE_H__

#include "UniformAbstract.h"

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
#include <wx/tglbtn.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "Globals.h"

#include <string>
using namespace std;

class UniformMouse : public UniformAbstract
{
public:
	UniformMouse(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformMouse();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	void UpdateValue(cUniformVariant vValue);
	void SetDatas(cUniformVariant vValue);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);

	virtual string ReplaceUniformInCode(string vCode);

private:

protected:
	wxStaticText* m_staticText12;
	wxChoice* m_choice1;
	wxStaticText* m_staticText3;
	wxTextCtrl* m_textCtrl10;
	wxStaticText* m_staticText4;
	wxTextCtrl* m_textCtrl11;
	wxStaticText* m_staticText31;
	wxTextCtrl* m_textCtrl101;
	wxStaticText* m_staticText41;
	wxTextCtrl* m_textCtrl111;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMMOUSE_H__