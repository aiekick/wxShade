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

#include "UniformScreen.h"

#include "UniformManager.h"

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ArrowRefresh_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformScreen, UniformAbstract)
	EVT_TEXT(ID_UNIFORM_SCREEN_WIDGET_WIDTH_VALUE, UniformScreen::OnScreenWidthValueChange)
	EVT_TEXT(ID_UNIFORM_SCREEN_WIDGET_HEIGHT_VALUE, UniformScreen::OnScreenHeightValueChange)
	EVT_BUTTON(ID_UNIFORM_SCREEN_REFRESH_BTN, UniformScreen::OnRefreshScreenBtn)
wxEND_EVENT_TABLE()

UniformScreen::UniformScreen(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager)
{
	IsMergeInCodeAvailable = true;

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("Screen Width :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	bSizer5->Add(m_staticText3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_textCtrl10 = new wxTextCtrl(this, ID_UNIFORM_SCREEN_WIDGET_WIDTH_VALUE, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(m_textCtrl10, 0, wxALL, 5);

	m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("Screen Height :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer5->Add(m_staticText4, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_textCtrl11 = new wxTextCtrl(this, ID_UNIFORM_SCREEN_WIDGET_HEIGHT_VALUE, wxT("0.0"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer5->Add(m_textCtrl11, 0, wxALL, 5);

	m_bpButton4 = new wxBitmapButton(this, ID_UNIFORM_SCREEN_REFRESH_BTN, ArrowRefresh_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	bSizer5->Add(m_bpButton4, 0, wxALL, 5);

	m_staticText5 = new wxStaticText(this, wxID_ANY, wxT("Ratio : "), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText5->Wrap(-1);
	bSizer5->Add(m_staticText5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	this->SetSizer(bSizer5);
	this->Layout();
}

UniformScreen::~UniformScreen()
{

}

string UniformScreen::Init(string vCodeSelection)
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

void UniformScreen::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformScreen::OnScreenWidthValueChange(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		float t;
		stringstream ss = stringstream(txtCtrl->GetValue().ToStdString());
		ss >> t;
		if (ss)
		{
			if (floatIsValid(t) == true)
			{
				wxColour col = txtCtrl->GetBackgroundColour();
				if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
				{
					txtCtrl->SetBackgroundColour(wxColour("WHITE"));
					txtCtrl->Refresh(false, 0);
				}
			}
		}
		else // ss cant be conerted to float
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

void UniformScreen::OnScreenHeightValueChange(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		float t;
		stringstream ss = stringstream(txtCtrl->GetValue().ToStdString());
		ss >> t;
		if (ss)
		{
			if (floatIsValid(t) == true)
			{
				wxColour col = txtCtrl->GetBackgroundColour();
				if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
				{
					txtCtrl->SetBackgroundColour(wxColour("WHITE"));
					txtCtrl->Refresh(false, 0);
				}
			}
		}
		else // ss cant be conerted to float
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

void UniformScreen::OnRefreshScreenBtn(wxCommandEvent& /*event*/)
{
	m_value.x = atof(m_textCtrl10->GetValue().ToStdString().c_str());
	m_value.y = atof(m_textCtrl11->GetValue().ToStdString().c_str());

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_value, false); // false car GetParamsToXMLString renvoie rien

	m_ShaderView->Refresh(false, 0);
}

void UniformScreen::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;

	SetScreenValue(m_value.x, m_value.y);
}

void UniformScreen::SetScreenValue(float w, float h)
{
	m_textCtrl10->SetLabelText(toStr(w));
	m_textCtrl10->Refresh(false, 0);

	m_textCtrl11->SetLabelText(toStr(h));
	m_textCtrl11->Refresh(false, 0);

	m_staticText5->SetLabelText("Ratio : " + toStr(w / h));
	m_staticText5->Refresh(false, 0);
}

string UniformScreen::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// rien

	return xml;
}

void UniformScreen::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformScreen::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	string src = m_mainUniformName;

	string dst = "vec2(" + toStr(m_value.x) + "," + toStr(m_value.y) + ")";

	DebugLogger::ReplaceString(code, src, dst);

	// on regle les petits porblem de signe avec le remplacement des input
	DebugLogger::ReplaceString(code, "+-", "-");
	//DebugLogger::ReplaceString(code, "--", "+");
	//DebugLogger::ReplaceString(code, "++", "+");
	DebugLogger::ReplaceString(code, "-+", "-");

	return code;
}
