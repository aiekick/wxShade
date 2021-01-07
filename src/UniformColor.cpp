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

#include "UniformColor.h"

#include "UniformManager.h"

#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(UniformColor, UniformAbstract)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_TIME_VALUE, UniformColor::OnTimeValueChange)
	//EVT_TOGGLEBUTTON(ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, UniformColor::OnPlayPauseTime)
	//EVT_BUTTON(ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, UniformColor::OnResetTime)
	//EVT_TIMER(ID_UNIFORM_TIMER, UniformColor::OnTimer)
	//EVT_CHECKBOX(ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, UniformColor::OnInvCheck)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, UniformColor::OnMultCoefChange)
wxEND_EVENT_TABLE()

UniformColor::UniformColor(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager)
{
	IsMergeInCodeAvailable = true;

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxHORIZONTAL);

	m_colourPicker1 = new wxColourPickerCtrl(this, wxID_ANY, wxSystemSettings::GetColour(wxSYS_COLOUR_ACTIVECAPTION), wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	m_colourPicker1->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	m_colourPicker1->SetMinSize(wxSize(100, 65));

	bSizer10->Add(m_colourPicker1, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText11 = new wxStaticText(this, wxID_ANY, wxT("R"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText11->Wrap(-1);
	bSizer12->Add(m_staticText11, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_textCtrl19 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	m_textCtrl19->SetMaxSize(wxSize(50, -1));
	
	bSizer12->Add(m_textCtrl19, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_staticline2 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer12->Add(m_staticline2, 0, wxALL | wxEXPAND, 5);

	m_textCtrl193 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl193->SetMaxSize(wxSize(30, -1));

	bSizer12->Add(m_textCtrl193, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_slider2 = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_slider2->SetMinSize(wxSize(200, -1));

	bSizer12->Add(m_slider2, 1, wxALIGN_CENTER_VERTICAL, 1);

	m_textCtrl20 = new wxTextCtrl(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl20->SetMaxSize(wxSize(30, -1));

	bSizer12->Add(m_textCtrl20, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_textCtrl19->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider2, this);

	bSizer11->Add(bSizer12, 0, wxEXPAND, 1);

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText111 = new wxStaticText(this, wxID_ANY, wxT("G"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText111->Wrap(-1);
	bSizer121->Add(m_staticText111, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_textCtrl191 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	m_textCtrl191->SetMaxSize(wxSize(50, -1));
	
	bSizer121->Add(m_textCtrl191, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_staticline21 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer121->Add(m_staticline21, 0, wxALL | wxEXPAND, 5);

	m_textCtrl194 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl194->SetMaxSize(wxSize(30, -1));

	bSizer121->Add(m_textCtrl194, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_slider21 = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_slider21->SetMinSize(wxSize(200, -1));

	bSizer121->Add(m_slider21, 1, wxALIGN_CENTER_VERTICAL, 1);

	m_textCtrl201 = new wxTextCtrl(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl201->SetMaxSize(wxSize(30, -1));

	bSizer121->Add(m_textCtrl201, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_textCtrl191->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider21, this);

	bSizer11->Add(bSizer121, 0, wxEXPAND, 1);

	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText112 = new wxStaticText(this, wxID_ANY, wxT("B"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText112->Wrap(-1);
	bSizer122->Add(m_staticText112, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);

	m_textCtrl192 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	m_textCtrl192->SetMaxSize(wxSize(50, -1));
	
	bSizer122->Add(m_textCtrl192, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_staticline22 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer122->Add(m_staticline22, 0, wxALL | wxEXPAND, 5);

	m_textCtrl195 = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl195->SetMaxSize(wxSize(30, -1));

	bSizer122->Add(m_textCtrl195, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_slider22 = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_slider22->SetMinSize(wxSize(200, -1));

	bSizer122->Add(m_slider22, 1, wxALIGN_CENTER_VERTICAL, 1);

	m_textCtrl202 = new wxTextCtrl(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_READONLY);
	m_textCtrl202->SetMaxSize(wxSize(30, -1));

	bSizer122->Add(m_textCtrl202, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_textCtrl192->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider22, this);

	bSizer11->Add(bSizer122, 0, wxEXPAND, 1);

	bSizer10->Add(bSizer11, 1, wxALIGN_CENTER_VERTICAL, 1);

	this->SetSizer(bSizer10);
	this->Layout();

	// Connect Events
	m_slider2->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	
	m_slider21->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	
	m_slider22->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);

	m_colourPicker1->Connect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(UniformColor::OnColorChange), NULL, this);
}

UniformColor::~UniformColor()
{
	// Disconnect Events
	m_colourPicker1->Disconnect(wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler(UniformColor::OnColorChange), NULL, this);
	
	m_slider2->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	m_slider2->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnRedValueChange), NULL, this);
	
	m_slider21->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	m_slider21->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnGreenValueChange), NULL, this);
	
	m_slider22->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);
	m_slider22->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformColor::OnBlueValueChange), NULL, this);

	// l'ordre est important sinon ca cree une boucle et ca merde le remove
	m_textCtrl19->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider2, this);
	m_textCtrl191->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider21, this);
	m_textCtrl192->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformColor::OnTextChange), m_slider22, this);
}

string UniformColor::Init(string vCodeSelection)
{
	string res;

	if (vCodeSelection.size() > 0)
	{
		if (m_Params.size() == 0)
		{
			// on va parser vCodeSelection qui represente la selection dans le code
			string s = vCodeSelection;
			DebugLogger::ReplaceString(s, " ", "");
			DebugLogger::ReplaceString(s, "\t", "");
			DebugLogger::ReplaceString(s, "\n", "");
			DebugLogger::ReplaceString(s, "\r", "");

			// est ce un vec3 ?
			vector<string::size_type> lst = DebugLogger::strContains(s, ",");
			string refWord = s.substr(0, 5) + s.substr(s.size() - 1);
			if (refWord == "vec3()" || (lst.size() == 2))
			{
				if (refWord == "vec3()")
				{
					s = s.substr(5, s.size() - 6); // 6 = 5 + 1 = vec3( + )
				}
				vector<string> vec = DebugLogger::splitString(s, ',');
				// on va parser les string qui ressemble à ca "0.5,500.2,25.2,7.0"
				string name = m_mainUniformName;
				float r, g, b;
				bool IsOk = true;
				string p = "rgb";
				int idx = 0;
				if (vec.size() == 3) // vec3(0.7,0.2,0.8)
				{
					for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
					{
						string word = *it;

						float v = 0.0f;
						if (StringToFloat(word, &v) == true && idx < 3)
						{
							if (v < 0 || v > 1)
							{
								g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because one component is out of range (0,1)", 0);
								IsOk = false;
								InitIsFailed = true;
								break;
							}
							if (idx == 0) r = v;
							else if (idx == 1) g = v;
							else if (idx == 2) b = v;
						}
						else
						{
							g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because one component is not a float number or the selection is bad", 0);
							IsOk = false;
							InitIsFailed = true;
							break;
						}
						idx++;
					}

				}
				else if (vec.size() == 1) // vec3(0.5)
				{
					string word = *vec.begin();

					float v = 0.0f;
					if (StringToFloat(word, &v) == true && idx < 3)
					{
						if (v < 0 || v > 1)
						{
							g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because the component is out of range (0,1)", 0);
							IsOk = false;
							InitIsFailed = true;
						}
						else
						{
							r = v;
							g = v;
							b = v;
						}
					}
					else
					{
						g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because the component is not a float number or the selection is bad", 0);
						IsOk = false;
						InitIsFailed = true;
					}
				}
				else
				{
					g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because the component count is not correct ! must be 1 or 3", 0);
					IsOk = false;
					InitIsFailed = true;
				}

				if (IsOk == true)
				{
					m_Params["R"] = toStr(r);
					m_Params["G"] = toStr(g);
					m_Params["B"] = toStr(b);
					res = name; // le nom de l'unifrom en retour tout simplement
				}
			}
			else
			{
				g_pMainFrame->SetStatusBarText("Failed to create the UniformColor because the selection is bad", 0);
				InitIsFailed = true;
			}
		}
		else
		{
			LogStr("UniformSlider::Init : vCodeSelection est pas vide et m_Params non plus !?");
		}
	}

	if (m_Params.size() > 0)
	{
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string r = "R=\"" + toStr(m_uniColor.x) + "\"";
			//string g = "G=\"" + toStr(m_uniColor.y) + "\"";
			//string b = "B=\"" + toStr(m_uniColor.z) + "\"";

			if (attrib == "R") m_uniColor.x = value.getF();
			if (attrib == "G") m_uniColor.y = value.getF();
			if (attrib == "B") m_uniColor.z = value.getF();
		}
	}
	else
	{
		m_uniColor.x = 0.5;
		m_uniColor.y = 0.5;
		m_uniColor.z = 0.5;
	}

	m_colourPicker1->SetColour(wxColour(m_uniColor.x * 255.0f, m_uniColor.y * 255.0f, m_uniColor.z * 255.0f));

	wxColourPickerEvent e;
	OnColorChange(e);

	return res;
}

void UniformColor::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformColor::OnColorChange(wxColourPickerEvent& /*event*/)
{
	wxColour col = m_colourPicker1->GetColour();

	m_uniColor.x = (float)col.Red() / 255.0f;
	m_uniColor.y = (float)col.Green() / 255.0f;
	m_uniColor.z = (float)col.Blue() / 255.0f;

	wxScrollEvent e;
	
	m_slider2->SetValue(m_uniColor.x * 100.0f);
	OnRedValueChange(e);
	
	m_slider21->SetValue(m_uniColor.y * 100.0f);
	OnGreenValueChange(e);

	m_slider22->SetValue(m_uniColor.z * 100.0f);
	OnBlueValueChange(e);

	UpdateUniformByName(m_mainUniformName, m_uniColor, true);
}

void UniformColor::OnRedValueChange(wxScrollEvent& /*event*/)
{
	m_uniColor.x = (float)m_slider2->GetValue() / 100.0f;
	m_textCtrl19->SetValue(toStr(m_uniColor.x));
	//m_textCtrl192->Refresh(false, 0);
	
	UpdatePickerAndUniform();
}

void UniformColor::OnGreenValueChange(wxScrollEvent& /*event*/)
{
	m_uniColor.y = (float)m_slider21->GetValue() / 100.0f;
	m_textCtrl191->SetValue(toStr(m_uniColor.y));
	//m_textCtrl192->Refresh(false, 0);

	UpdatePickerAndUniform();
}

void UniformColor::OnBlueValueChange(wxScrollEvent& /*event*/)
{
	m_uniColor.z = (float)m_slider22->GetValue() / 100.0f;
	m_textCtrl192->SetValue(toStr(m_uniColor.z));
	//m_textCtrl192->Refresh(false, 0);

	UpdatePickerAndUniform();
}

void UniformColor::UpdatePickerAndUniform()
{
	wxColour col = ConvertUniformVariantToWxColour(m_uniColor);
	m_colourPicker1->SetColour(col);
	
	UpdateUniformByName(m_mainUniformName, m_uniColor, true);
}

wxColour UniformColor::ConvertUniformVariantToWxColour(cUniformVariant v)
{
	wxColour::ChannelType r = (wxColour::ChannelType)(v.x * 255.0f);
	wxColour::ChannelType g = (wxColour::ChannelType)(v.y * 255.0f);
	wxColour::ChannelType b = (wxColour::ChannelType)(v.z * 255.0f);
	return wxColour(r, g, b);
}

string UniformColor::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// les 3 valeur de la couleur
	
	wxColour col = m_colourPicker1->GetColour();

	string r = "R=\"" + toStr(m_uniColor.x) + "\"";
	string g = "G=\"" + toStr(m_uniColor.y) + "\"";
	string b = "B=\"" + toStr(m_uniColor.z) + "\"";

	xml = r + " " + g + " " + b;

	return xml;
}

void UniformColor::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

void UniformColor::OnTextChange(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		bool error = false;

		float t;
		if (StringToFloat(txtCtrl->GetValue().ToStdString(), &t) == true)
		{
			if (floatIsValid(t) == true)
			{
				if (t >= 0.0f && t <= 1.0f)
				{
					wxColour col = txtCtrl->GetBackgroundColour();
					if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
					{
						txtCtrl->SetBackgroundColour(wxColour("WHITE"));
						txtCtrl->Refresh(false, 0);
					}

					// on maj le slider
					wxSlider *slider = dynamic_cast<wxSlider*>(event.GetEventUserData());
					if (slider != 0)
					{
						slider->SetValue(t*100.0f);
					}

					error = true;
				}
			}
		}
		
		if (error == false )// ss cant be conerted to float ou t or range 0->1
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

string UniformColor::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	string src = m_mainUniformName;

	string dst;

	size_t pos = 0;
	size_t pos2 = 0;
	size_t pos3 = 0;
	while ((pos = code.find(src, pos)) != std::string::npos)
	{
		// on va chercher les unifrom du meme nom et les formater avec les valeur du slider
		// ce code ne permet pas de resoudre les code de merde du style .xzyw.zw.yx.x qui correspond a z du buf originel
		string arr = code.substr(pos + src.length(), 6); // .xyzw ou .xy, t
		if (arr[0] == '.')
		{
			pos2 = arr.find_first_of("xyzwrgbastp");
			if (pos2 != string::npos) // trouvé
			{
				pos3 = arr.find_first_not_of("xyzwrgbastp.");
				arr = arr.substr(1, pos3 - 1);
//				int len = arr.size();
				src += ".";

				dst = "vec3(" + toStr(m_uniColor.x) + "," + toStr(m_uniColor.y) + "," + toStr(m_uniColor.z) + ")";
			}
			else
			{
				return code;
			}

			code.replace(pos, src.length(), dst);
			pos += dst.length();
			src = m_mainUniformName;
			dst = "";
		}
		else
		{
			string num = code.substr(pos + src.length(), 1); // 2
			pos2 = num.find_first_of("abcdefghijklmnopqrstuvwxyz1234567890");
			if (pos2 == 0) // il y a un des charcaters autorisé derriere donc ce n'est pas le bon mot seul
			{
				pos += src.length();
				continue;
			}
			else // c'est le bon
			{
				if (m_uniColor.x == m_uniColor.y && m_uniColor.y == m_uniColor.z && m_uniColor.z == m_uniColor.w)
				{
					dst = "vec3(" + toStr(m_uniColor.x) + ")";
				}
				else
				{
					dst = "vec3(" + toStr(m_uniColor.x) + "," + toStr(m_uniColor.y) + "," + toStr(m_uniColor.z) + ")";
				}
				code.replace(pos, src.length(), dst);
				pos += dst.length();
				src = m_mainUniformName;
				dst = "";
			}
		}
	}

	// on regle les petits porblem de signe avec le remplacement des input
	DebugLogger::ReplaceString(code, "+-", "-");
	//DebugLogger::ReplaceString(code, "--", "+");
	//DebugLogger::ReplaceString(code, "++", "+");
	DebugLogger::ReplaceString(code, "-+", "-");

	return code;
}
