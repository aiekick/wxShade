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

#include "UniformTime.h"

#include "UniformManager.h"

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/ControlStartBlue_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformTime, UniformAbstract)
	EVT_TEXT(ID_UNIFORM_TIME_WIDGET_TIME_VALUE, UniformTime::OnTimeValueChange)
	EVT_TOGGLEBUTTON(ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, UniformTime::OnPlayPauseTime)
	EVT_BUTTON(ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, UniformTime::OnResetTime)
	EVT_TIMER(ID_UNIFORM_TIMER, UniformTime::OnTimer)
	EVT_CHECKBOX(ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, UniformTime::OnInvCheck)
	EVT_TEXT(ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, UniformTime::OnMultCoefChange)
wxEND_EVENT_TABLE()

UniformTime::UniformTime(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), 
m_timer(this, ID_UNIFORM_TIMER), m_lastTick(0), m_currentTime(0), m_lastDeltaTick(0), m_multCoef(1), m_TimeDir(1), m_lastInvertTick(0), m_currentTick(0), m_Modified(false)
{
	IsMergeInCodeAvailable = true;

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_textCtrl1 = new wxTextCtrl(this, ID_UNIFORM_TIME_WIDGET_TIME_VALUE, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl1->SetLabelText(toStr(m_currentTime));
	bSizer2->Add(m_textCtrl1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_bpButton1 = new wxBitmapToggleButton(this, ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, wxBitmap(ControlPlayBlue_small_icon), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton1->SetBitmapPressed(wxBitmap(ControlPauseBlue_small_icon));
	m_bpButton1->SetMaxSize(wxSize(32, 32));
	bSizer2->Add(m_bpButton1, 0, wxALL, 5);

	m_bpButton2 = new wxBitmapButton(this, ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, wxBitmap(ControlStartBlue_small_icon), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	bSizer2->Add(m_bpButton2, 0, wxALL, 5);

	m_checkBox2 = new wxCheckBox(this, ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, wxT("Inv"), wxDefaultPosition, wxDefaultSize, 0);
	m_checkBox2->SetMaxSize(wxSize(35, -1));
	bSizer2->Add(m_checkBox2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_textCtrl3 = new wxTextCtrl(this, ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, wxT(""), wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl3->SetValue(toStr(m_multCoef));
	bSizer2->Add(m_textCtrl3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	this->SetSizer(bSizer2);
	this->Layout();
}


UniformTime::~UniformTime()
{

}

string UniformTime::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string time = "TIME=\"" + m_textCtrl1->GetValue().ToStdString() + "\"";
			//string invCheck = "INV=\"" + toStr(m_checkBox2->IsChecked()) + "\"";
			//string coef = "COEF=\"" + m_textCtrl3->GetValue().ToStdString() + "\"";
			//string playState = "PLAYED=\"" + toStr(m_bpButton1->GetNormalState() == wxBitmapToggleButton::State::State_Pressed) + "\"";

			if (attrib == "TIME")
			{
				ResetTimer(value.getF());
			}

			if (attrib == "INV")
			{
				if (value.getB() == true)
				{
					m_checkBox2->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
				}
				else
				{
					m_checkBox2->Set3StateValue(wxCheckBoxState::wxCHK_UNCHECKED);
				}
			}

			if (attrib == "COEF")
			{
				m_textCtrl3->SetValue(value.getS());
			}

			if (attrib == "PLAYED")
			{
				bool mode = value.getB();
				m_bpButton1->SetValue(mode);
			}
		}

		// on declenche les events pour declencher les actions liées à la maj des controls
		wxTimerEvent t; // event vide
		OnTimer(t);
		wxCommandEvent e; // event vide
		OnTimeValueChange(e);
		OnInvCheck(e);
		OnMultCoefChange(e);
		OnPlayPauseTime(e);
		m_Modified = false; // init ==> 1er chargement
	}
	else
	{

	}

	return res;
}

void UniformTime::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
	ResetTimer(0.0f);//force uniform set and view refresh
}

// declenche quand le test change... pas seulement quand c'est le user qui le change
void UniformTime::OnTimeValueChange(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		if (txtCtrl->IsEnabled() == true)
		{
			m_Modified = true;
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
					ResetTimer(t);//force uniform set and view refresh
				}
			}
			else // ss cant be conerted to float
			{
				txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
				txtCtrl->Refresh(false, 0);
			}
		}
	}
}
	

void UniformTime::OnPlayPauseTime(wxCommandEvent& /*event*/)
{
	m_Modified = true;
	UpdateUniformByName(m_mainUniformName, m_uniTime, m_Modified); // on transfere 'letat modifié ou non

	if (m_bpButton1->GetValue() == true)
	{
		PlayTimer();
	}
	else
	{
		PauseTimer();
	}
}

void UniformTime::OnResetTime(wxCommandEvent& /*event*/)
{
	ResetTimer(0.0f);
	m_textCtrl1->SetLabelText(toStr(m_currentTime));
	m_textCtrl1->Refresh(false, 0);
}

void UniformTime::OnTimer(wxTimerEvent& /*event*/)
{
	m_currentTick = GetTickCount64();

	if (m_TimeDir>0)
		m_lastDeltaTick = m_currentTick - m_lastTick;
	else
		m_lastDeltaTick = (m_lastInvertTick - (m_currentTick - m_lastInvertTick)) - m_lastTick;

	m_currentTime = m_lastDeltaTick / 1000.0f * m_multCoef;

	// round à 2 chiffres
	m_textCtrl1->SetLabelText(toStr(int(m_currentTime * 100.0f) / 100.0f));

	//m_TimeDir

	m_uniTime.x = m_currentTime;

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_uniTime, m_Modified);

	//m_ShaderView->Refresh(false, 0); la sahderview s'en charge elle meme maintenant. elle a son propre timer
}

void UniformTime::PlayTimer()
{
	m_lastTick = GetTickCount64() - m_lastDeltaTick;
	m_timer.Start(20);    // 1 second interval
	m_textCtrl1->Enable(false);
}

void UniformTime::PauseTimer()
{
	m_timer.Stop();
	m_textCtrl1->Enable(true);
}

void UniformTime::ResetTimer(float vValue)
{
	m_Modified = true;
	m_lastTick = GetTickCount64();
	m_currentTick = m_lastTick;
	m_currentTime = vValue;
	m_lastDeltaTick = 0;
	m_lastInvertTick = m_currentTick;

	if (m_TimeDir < 0)
		m_lastInvertTick = m_currentTick;
	else
		m_lastInvertTick = 0;

	///////////////////////////////

	m_lastDeltaTick = m_currentTime / m_multCoef * 1000.0f;

	if (m_TimeDir>0)
		m_lastTick = m_currentTick - m_lastDeltaTick;
	else
		m_lastTick = (m_lastInvertTick - (m_currentTick - m_lastInvertTick)) - m_lastDeltaTick;

	///////////////////////////////

	m_uniTime.x = m_currentTime;

	// Update Uniform
	if (m_mainUniformName.size() > 0)
		UpdateUniformByName(m_mainUniformName, m_uniTime, m_Modified);

	if (m_ShaderView != 0)
		m_ShaderView->Refresh(false, 0);
}

void UniformTime::OnInvCheck(wxCommandEvent& /*event*/)
{
	m_Modified = true;
	if (m_checkBox2->Get3StateValue() == wxCheckBoxState::wxCHK_CHECKED)
	{
		m_TimeDir = -1;
		m_lastInvertTick = m_currentTick;
	}
	else
	{
		m_TimeDir = 1;
		m_lastTick += (m_currentTick - m_lastInvertTick)*2;
	}
	
}

void UniformTime::OnMultCoefChange(wxCommandEvent& event)
{
	m_Modified = true;
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
				m_multCoef = t;
			}
		}
		else // ss cant be conerted to float
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

string UniformTime::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// le coef de demultiplication
	// l'état du player : si le timer joue ou est arretée
	// si le temps est inversé ou pas

	string time = "TIME=\"" + toStr(m_currentTime) + "\"";
	string invCheck = "INV=\"" + toStr(m_checkBox2->IsChecked()) + "\"";
	string coef = "COEF=\"" + m_textCtrl3->GetValue().ToStdString() + "\"";
	
	string playMode; 
	if (m_bpButton1->GetNormalState() == wxBitmapToggleButton::State::State_Pressed)
		playMode = "true";
	else
		playMode = "false";
	string playState = "PLAYED=\"" + playMode + "\"";

	xml = time + " " + playState + " " + invCheck + " " + coef;

	m_Modified = false;

	return xml;
}

void UniformTime::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformTime::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	string name = m_mainUniformName;

	float val = m_uniTime.x;

	float coef;
	stringstream ss = stringstream(m_textCtrl3->GetValue().ToStdString());
	ss >> coef;
	if (ss)
	{
		string coefStr = toStr(coef);
		size_t pos = coefStr.find_first_of(".");
		if (pos == std::string::npos) // pas trouvé
		{
			coefStr += ".";
		}

		string src = name;
		string dst = toStr(val);
		if (dst.find_first_of(".") == std::string::npos) // pas trouvé
			dst += ".";

		if (coef != 1.0f)
			dst += "*" + coefStr;

		DebugLogger::ReplaceString(code, src, dst);
	}

	// on regle les petits porblem de signe avec le remplacement des input
	DebugLogger::ReplaceString(code, "+-", "-");
	//DebugLogger::ReplaceString(code, "--", "+");
	//DebugLogger::ReplaceString(code, "++", "+");
	DebugLogger::ReplaceString(code, "-+", "-");

	return code;
}
