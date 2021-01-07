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

#ifndef __UNIFORMFONT_H__
#define __UNIFORMFONT_H__

#include "Globals.h"

#include "UniformAbstract.h"

#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/font.h>
#include <wx/fontpicker.h>
#include <wx/gdicmn.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/clrpicker.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/spinctrl.h>
#include <wx/fontdlg.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"

#include "SettingsDlg.h"

#include <string>
using namespace std;

class UniformFont : public UniformAbstract
{
public:
	UniformFont(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformFont();

private:
	bool UpdateFontTex();
	bool loadFontImage(wxImage* img, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot);

private:
	cUniformVariant m_Font;
	string m_FilePathName;
	cUniformVariant m_FontSize;
	string m_FontSizeName;
	string m_Text;
	string m_FontName;
	wxColour m_FontColour;
	int m_FontPixelSize;
	int m_Precision;
	int m_CountRows;
	int m_CountCols;
	float m_ColSpace;
	float m_RowSpace;
	float m_ColWidth;
	float m_RowWidth;

private:
	wxFontData  m_FontData;

protected:
	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual void UpdateValue(cUniformVariant vValue);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);
	virtual string ReplaceUniformInCode(string vCode);
	virtual void ChangeUniformName(string vCurrentName, string vNewName);

protected:
	virtual void OnChangeFontBtn(wxCommandEvent& event);
	virtual void OnChangeFontColor(wxColourPickerEvent& event);
	virtual void OnChangeParams(wxSpinEvent& event);
	virtual void OnChangeText(wxCommandEvent& event);

protected:
	wxButton* m_button1;
	wxColourPickerCtrl* m_colourPicker2;
	//wxStaticText* m_staticText1;
	//wxSpinCtrl* m_spinCtrl2;
	//wxSpinCtrl* m_spinCtrl21;
	//wxStaticText* m_staticText111;
	//wxSpinCtrl* m_spinCtrl211;
	//wxSpinCtrl* m_spinCtrl212;
	wxStaticText* m_staticText1111;
	wxSpinCtrl* m_spinCtrl2111;
	//wxSpinCtrl* m_spinCtrl2121;
	wxTextCtrl* m_textCtrl1;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMFONT_H__
