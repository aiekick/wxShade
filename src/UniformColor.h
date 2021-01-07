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

#ifndef __UNIFORMCOLOR_H__
#define __UNIFORMCOLOR_H__

#include "UniformAbstract.h"

#include <wx/wx.h>
#include <wx/clrpicker.h>
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
#include <wx/statline.h>
#include <wx/statusbr.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "Globals.h"

#include <string>
using namespace std;

class UniformColor : public UniformAbstract
{
public:
	UniformColor(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformColor();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	
	virtual void UpdatePickerAndUniform();
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);

	virtual string ReplaceUniformInCode(string vCode);

private:
	cUniformVariant m_uniColor;

private:
	virtual wxColour ConvertUniformVariantToWxColour(cUniformVariant v);

protected:
	virtual void OnColorChange(wxColourPickerEvent& /*event*/);
	virtual void OnRedValueChange(wxScrollEvent& /*event*/);
	virtual void OnGreenValueChange(wxScrollEvent& /*event*/);
	virtual void OnBlueValueChange(wxScrollEvent& /*event*/);
	virtual void OnTextChange(wxCommandEvent& /*event*/);

protected:
	wxColourPickerCtrl* m_colourPicker1;
	wxStaticText* m_staticText11;
	wxTextCtrl* m_textCtrl193;
	wxStaticLine* m_staticline2;
	wxTextCtrl* m_textCtrl19;
	wxSlider* m_slider2;
	wxTextCtrl* m_textCtrl20;
	wxStaticText* m_staticText111;
	wxTextCtrl* m_textCtrl191;
	wxStaticLine* m_staticline21;
	wxTextCtrl* m_textCtrl194;
	wxSlider* m_slider21;
	wxTextCtrl* m_textCtrl201;
	wxStaticText* m_staticText112;
	wxTextCtrl* m_textCtrl192;
	wxStaticLine* m_staticline22;
	wxTextCtrl* m_textCtrl195;
	wxSlider* m_slider22;
	wxTextCtrl* m_textCtrl202;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMCOLOR_H__