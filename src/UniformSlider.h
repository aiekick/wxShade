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

#ifndef __UNIFORMSLIDER_H__
#define __UNIFORMSLIDER_H__

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
#include <wx/statline.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "Globals.h"

#include <cmath>
#include <string>
using namespace std;

struct SliderControlStruct
{
public:
	wxStaticText* CountStatic; // contient le numero de l'arr
	wxBitmapButton* DeleteBtn; // contient le bouton pour enlever un slider
	wxTextCtrl* ValueTextCtrl; // contient la valeur du slider
	wxTextCtrl* BorneInfextCtrl; // contient la borne inf
	wxTextCtrl* BorneSupTextCtrl; // contient la borne sup
	wxSlider* Slider; // contient le sliders
	wxBoxSizer* Sizer; // continet le sizer de l'ensemble
	int idx;

public:
	SliderControlStruct()
	{
		CountStatic = 0;
		DeleteBtn = 0;
		ValueTextCtrl = 0;
		BorneInfextCtrl = 0;
		BorneSupTextCtrl = 0;
		Slider = 0;
		Sizer = 0;
		idx = 0;
	}
};

class UniformSlider : public UniformAbstract
{
public:
	UniformSlider(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformSlider();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);

	virtual void CalcAndUpdateSlider(SliderControlStruct vSliderStruct, float vInf, float vSup, float vTextVal);
	virtual float CalcAndUpdateValue(SliderControlStruct vSliderStruct, float vInf, float vSup, float vSliderVal);

	virtual string ReplaceUniformInCode(string vCode);

	void ReOrderWizzle();

private:
	cUniformVariant m_uniSlider;
	cVec4 inf;
	cVec4 sup;

private:
	SliderControlStruct AddSlider();
	void RemoveSlider(int vIdx);

protected:
	virtual void OnChangeValue(wxScrollEvent& /*event*/);
	virtual void OnChangeText(wxCommandEvent& /*event*/);
	virtual void OnAddSlider(wxCommandEvent& /*event*/);
	virtual void OnRemoveSlider(wxCommandEvent& /*event*/);

protected:
	wxBitmapButton* m_bpButton10;
	wxStaticLine* m_staticline7;

	wxBoxSizer* containerSizer;

	map<int, SliderControlStruct> Sliders; 

	UniformViewItem *m_ParentContainer;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMSLIDER_H__