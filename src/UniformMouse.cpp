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

#include "UniformMouse.h"

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/ControlStartBlue_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformMouse, UniformAbstract)
	//EVT_CHECKBOX(ID_UNIFORM_WIDGET_EDIT_CHECKBOX, UniformMouse::OnEditUniformNameCheck)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_TIME_VALUE, UniformMouse::OnTimeValueChange)
	//EVT_TOGGLEBUTTON(ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, UniformMouse::OnPlayPauseTime)
	//EVT_BUTTON(ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, UniformMouse::OnResetTime)
	//EVT_TIMER(ID_UNIFORM_TIMER, UniformMouse::OnTimer)
	//EVT_CHECKBOX(ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, UniformMouse::OnInvCheck)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, UniformMouse::OnMultCoefChange)
wxEND_EVENT_TABLE()

UniformMouse::UniformMouse(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager)
{
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	/*m_staticText12 = new wxStaticText(this, wxID_ANY, wxT("Button Tracked :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText12->Wrap(-1);
	bSizer5->Add(m_staticText12, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	wxString m_choice1Choices[] = { wxT("Left"), wxT("Middle"), wxT("Right"), wxEmptyString, wxEmptyString, wxEmptyString, wxEmptyString };
	int m_choice1NChoices = sizeof(m_choice1Choices) / sizeof(wxString);
	m_choice1 = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 0);
	m_choice1->SetSelection(0);
	m_choice1->SetMaxSize(wxSize(100, -1));
	bSizer5->Add(m_choice1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);*/

	m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("Pos (x,y):"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	bSizer5->Add(m_staticText3, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_textCtrl10 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl10->SetMaxSize(wxSize(40, -1));

	bSizer5->Add(m_textCtrl10, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_staticText4 = new wxStaticText(this, wxID_ANY, wxT(","), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer5->Add(m_staticText4, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_textCtrl11 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl11->SetMaxSize(wxSize(40, -1));

	bSizer5->Add(m_textCtrl11, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_staticText31 = new wxStaticText(this, wxID_ANY, wxT("Click (z,w):"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText31->Wrap(-1);
	bSizer5->Add(m_staticText31, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_textCtrl101 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl101->SetMaxSize(wxSize(40, -1));

	bSizer5->Add(m_textCtrl101, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_staticText41 = new wxStaticText(this, wxID_ANY, wxT(","), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText41->Wrap(-1);
	bSizer5->Add(m_staticText41, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_textCtrl111 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl111->SetMaxSize(wxSize(40, -1));

	bSizer5->Add(m_textCtrl111, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);


	this->SetSizer(bSizer5);
	this->Layout();
}


UniformMouse::~UniformMouse()
{

}

string UniformMouse::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;
		}

		// on declenche les events pour declencher les actions liées à la maj des controls
		//wxCommandEvent e;
	}
	else
	{

	}

	return res;
}

void UniformMouse::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformMouse::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
	SetDatas(vValue);
}

void UniformMouse::SetDatas(cUniformVariant vValue)
{
	// mouse move position
	m_textCtrl10->SetValue(toStr(vValue.x));
	m_textCtrl10->Refresh(false, 0);
	m_textCtrl11->SetValue(toStr(vValue.y));
	m_textCtrl11->Refresh(false, 0);

	// mouse last click position 
	m_textCtrl101->SetValue(toStr(vValue.z));
	m_textCtrl101->Refresh(false, 0);
	m_textCtrl111->SetValue(toStr(vValue.w));
	m_textCtrl111->Refresh(false, 0);
}

string UniformMouse::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// rien

	return xml;
}

void UniformMouse::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformMouse::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	return code;
}
