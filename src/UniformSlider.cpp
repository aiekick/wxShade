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

#include "UniformSlider.h"
#include "DocShaderArt.h"
#include "UniformViewItem.h"

#include "MainFrame.h"

#include "../icons/20x20/Add_small_icon.xpm"
#include "../icons/20x20/Delete_small_icon.xpm"

wxBEGIN_EVENT_TABLE(UniformSlider, UniformAbstract)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_TIME_VALUE, UniformSlider::OnTimeValueChange)
	//EVT_TOGGLEBUTTON(ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, UniformSlider::OnPlayPauseTime)
	//EVT_BUTTON(ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, UniformSlider::OnResetTime)
	//EVT_TIMER(ID_UNIFORM_TIMER, UniformSlider::OnTimer)
	//EVT_CHECKBOX(ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, UniformSlider::OnInvCheck)
	//EVT_TEXT(ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, UniformSlider::OnMultCoefChange)
wxEND_EVENT_TABLE()

const char SlidersLabels[] = { 'x', 'y', 'z', 'w' };

UniformSlider::UniformSlider(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager)
{
	IsMergeInCodeAvailable = true;

	wxBoxSizer* bSizer28;
	bSizer28 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer(wxVERTICAL);

	m_bpButton10 = new wxBitmapButton(this, wxID_ANY, Add_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton10->SetMaxSize(wxSize(24, 24));

	bSizer30->Add(m_bpButton10, 0, 0, 1);

	bSizer28->Add(bSizer30, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 1);

	m_staticline7 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer28->Add(m_staticline7, 0, wxEXPAND | wxLEFT, 1);

	containerSizer = new wxBoxSizer(wxVERTICAL);

	SliderControlStruct st = AddSlider();

	bSizer28->Add(containerSizer, 1, wxEXPAND, 5);

	this->SetSizer(bSizer28);
	this->Layout();

	// Connect Events
	m_bpButton10->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformSlider::OnAddSlider), NULL, this);
}


UniformSlider::~UniformSlider()
{
	m_bpButton10->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformSlider::OnAddSlider), NULL, this);
}

string UniformSlider::Init(string vCodeSelection)
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

			// est ce un float ?
			if (s.find_first_not_of("0123456789.-") == string::npos)
			{
				float v = 0.0f;
				if (StringToFloat(s, &v) == true)
				{
					float _inf = 0.0f;
					float _sup = 0.0f;

					if (v < 0.0f)
					{
						_sup = 0.0f;
						_inf = v * 2.0f;
					}
					else if (v > 0.0f)
					{
						_inf = 0.0f;
						_sup = v * 2.0f;
					}
					else
					{
						_inf = 0.0f;
						_sup = 1.0f;
					}

					float ratio = v;

					m_Params["INF"] = toStr(_inf);
					m_Params["SUP"] = toStr(_sup);
					m_Params["RATIO"] = toStr(ratio);
					res = m_mainUniformName; // le nom de l'unifrom en retour tout simplement
				}
				else
				{
					g_pMainFrame->SetStatusBarText("Failed to create the UniformSlider because the selection is bad", 0);
					InitIsFailed = true;
				}
			}
			else
			{
				// est ce un vec2 ?
				vector<string::size_type> lst = DebugLogger::strContains(s, ","); // c'est le nombre de virgule pas le nombre de nombre separés
				string refWord = s.substr(0, 5) + s.substr(s.size() - 1);
				if (refWord == "vec2()" || refWord == "vec3()" || refWord == "vec4()" || (lst.size() > 0 && lst.size() < 3))
				{
					if (refWord == "vec2()" || refWord == "vec3()" || refWord == "vec4()")
					{
						s = s.substr(5, s.size() - 6);
					}
					vector<string> vec = DebugLogger::splitString(s, ',');
					// on va parser les string qui ressemble à ca "0.5,500.2,25.2,7.0"
					string infStr, supStr, ratioStr, name = m_mainUniformName;// +".";
					bool IsOk = true;
					string p = "xyzw";
					int idx = 0;
					if (vec.size() > 1)
					{
						for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
						{
							string word = *it;

							float v = 0.0f;
							if (StringToFloat(word, &v) == true && idx < 4)
							{
								float _inf = 0.0f;
								float _sup = 0.0f;

								if (v < 0.0f)
								{
									_sup = 0.0f;
									_inf = v * 2.0f;
								}
								else if (v > 0.0f)
								{
									_inf = 0.0f;
									_sup = v * 2.0f;
								}
								else
								{
									_inf = 0.0f;
									_sup = 1.0f;
								}

								float ratio = v;

								if (it != vec.begin())
								{
									infStr += ";" + toStr(_inf);
									supStr += ";" + toStr(_sup);
									ratioStr += ";" + toStr(ratio);
								}
								else
								{
									infStr += toStr(_inf);
									supStr += toStr(_sup);
									ratioStr += toStr(ratio);
								}
								//name += p[idx];
							}
							else
							{
								g_pMainFrame->SetStatusBarText("Failed to create the UniformSlider because one component is not a float number or the selection is bad", 0);
								IsOk = false;
								InitIsFailed = true;
								break;
							}
							idx++;
						}
					}
					else // cas du vec4(0.71) qui doit donner 4 slider à 0.71
					{
						string word = vec[0];
						float v = 0.0f;
						if (StringToFloat(word, &v) == true)
						{
							float _inf = 0.0f;
							float _sup = 0.0f;

							if (v < 0.0f)
							{
								_sup = 0.0f;
								_inf = v * 2.0f;
							}
							else if (v > 0.0f)
							{
								_inf = 0.0f;
								_sup = v * 2.0f;
							}
							else
							{
								_inf = 0.0f;
								_sup = 1.0f;
							}

							float ratio = v;

							if (refWord == "vec2()")
							{
								infStr += toStr(_inf) + ";" + toStr(_inf);
								supStr += toStr(_sup) + ";" + toStr(_sup);
								ratioStr += toStr(ratio) + ";" + toStr(ratio);
							}
							if (refWord == "vec3()")
							{
								infStr += toStr(_inf) + ";" + toStr(_inf) + ";" + toStr(_inf);
								supStr += toStr(_sup) + ";" + toStr(_sup) + ";" + toStr(_sup);
								ratioStr += toStr(ratio) + ";" + toStr(ratio) + ";" + toStr(ratio);
							}
							if (refWord == "vec4()")
							{
								infStr += toStr(_inf) + ";" + toStr(_inf) + ";" + toStr(_inf) + ";" + toStr(_inf);
								supStr += toStr(_sup) + ";" + toStr(_sup) + ";" + toStr(_sup) + ";" + toStr(_sup);
								ratioStr += toStr(ratio) + ";" + toStr(ratio) + ";" + toStr(ratio) + ";" + toStr(ratio);
							}
						}
						else
						{
							g_pMainFrame->SetStatusBarText("Failed to create the UniformSlider because the var is not a float number or the selection is bad", 0);
							IsOk = false;
							InitIsFailed = true;
						}
					}

					if (IsOk == true)
					{
						m_Params["INF"] = infStr;
						m_Params["SUP"] = supStr;
						m_Params["RATIO"] = ratioStr;
						res = name; // le nom de l'unifrom en retour tout simplement
					}
				}
				else
				{
					g_pMainFrame->SetStatusBarText("Failed to create the UniformSlider because the selection is bad", 0);
					InitIsFailed = true;
				}
			}
		}
		else
		{
			LogStr("UniformSlider::Init : vCodeSelection est pas vide et m_Params non plus !?");
		}
	}

	if (m_Params.size() > 0)
	{
		SliderControlStruct sliderStruct = Sliders[0];

		string infStr;
		string supStr;
		string ratioStr;
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string bInf = "INF=\"" + toStr(inf) + "\"";
			//string bSup = "SUP=\"" + toStr(sup) + "\"";
			//string value = "RATIO=\"" + toStr(ratio) + "\"";

			if (attrib == "INF") infStr = value.getS();
			if (attrib == "SUP") supStr = value.getS();
			if (attrib == "RATIO") ratioStr = value.getS();
		}

		std::size_t count = 0;
		int idx = 0;
		vector<string> vec;
		// on va parser les string qui ressemble à ca "0.5;500.2;25.2;7.0"

		//INF
		vec = DebugLogger::splitString(infStr, ';');
		if (vec.size() > count) count = vec.size();
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				float v = 0.;
				if (StringToFloat(s, &v) == true)
				{
					if (idx == 0) inf.x = v;
					else if (idx == 1) inf.y = v;
					else if (idx == 2) inf.z = v;
					else if (idx == 3) inf.w = v;
				}
				else
				{
					g_pMainFrame->SetStatusBarText("The inf attribute of the file is corrupted and not taken into account", 0);
					LogStr("error UniformSlider::Init() => attrib inf is bad ! ");
				}
				idx++;
			}
		}

		//SUP
		vec = DebugLogger::splitString(supStr, ';');
		if (vec.size() > count) count = vec.size();
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				float v = 0.;
				if (StringToFloat(s, &v) == true)
				{
					if (idx == 0) sup.x = v;
					else if (idx == 1) sup.y = v;
					else if (idx == 2) sup.z = v;
					else if (idx == 3) sup.w = v;
				}
				else
				{
					g_pMainFrame->SetStatusBarText("The sup attribute of the file is corrupted and not taken into account", 0);
					LogStr("error UniformSlider::Init() => attrib sup is bad ! ");
				}
				idx++;
			}
		}

		//RATIO
		vec = DebugLogger::splitString(ratioStr, ';');
		if (vec.size() > count) count = vec.size();
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				float v = 0.;
				if (StringToFloat(s, &v) == true)
				{
					if (idx == 0) m_uniSlider.x = v;
					else if (idx == 1) m_uniSlider.y = v;
					else if (idx == 2) m_uniSlider.z = v;
					else if (idx == 3) m_uniSlider.w = v;
				}
				else
				{
					g_pMainFrame->SetStatusBarText("The ratio attribute of the file is corrupted and not taken into account", 0);
					LogStr("error UniformSlider::Init() => attrib ratio is bad ! ");
				}

				idx++;
			}
		}

		if (count > 1) sliderStruct = this->AddSlider();
		if (count > 2) sliderStruct = this->AddSlider();
		if (count > 3) sliderStruct = this->AddSlider();

		idx = 0;
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
			//			int i = it->first;
			SliderControlStruct st = it->second;
			if (st.BorneInfextCtrl != 0)
			{
				if (idx == 0) st.BorneInfextCtrl->SetValue(toStr(inf.x));
				else if (idx == 1) st.BorneInfextCtrl->SetValue(toStr(inf.y));
				else if (idx == 2) st.BorneInfextCtrl->SetValue(toStr(inf.z));
				else if (idx == 3) st.BorneInfextCtrl->SetValue(toStr(inf.w));
			}
			if (st.BorneSupTextCtrl != 0)
			{
				if (idx == 0) st.BorneSupTextCtrl->SetValue(toStr(sup.x));
				else if (idx == 1) st.BorneSupTextCtrl->SetValue(toStr(sup.y));
				else if (idx == 2) st.BorneSupTextCtrl->SetValue(toStr(sup.z));
				else if (idx == 3) st.BorneSupTextCtrl->SetValue(toStr(sup.w));
			}
			if (st.ValueTextCtrl != 0)
			{
				if (idx == 0) st.ValueTextCtrl->SetValue(toStr(m_uniSlider.x));
				else if (idx == 1) st.ValueTextCtrl->SetValue(toStr(m_uniSlider.y));
				else if (idx == 2) st.ValueTextCtrl->SetValue(toStr(m_uniSlider.z));
				else if (idx == 3) st.ValueTextCtrl->SetValue(toStr(m_uniSlider.w));
			}
			idx++;
		}

		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniSlider, true);
	}
	else
	{
		inf.x = 0.0f;
		sup.x = 1.0f;
		m_uniSlider.x = 0.5f;
		SliderControlStruct st = Sliders[0];

		if (st.BorneInfextCtrl != 0)
			st.BorneInfextCtrl->SetValue(toStr(inf.x));

		if (st.BorneSupTextCtrl != 0)
			st.BorneSupTextCtrl->SetValue(toStr(sup.x));

		if (st.ValueTextCtrl != 0)
			st.ValueTextCtrl->SetValue(toStr(m_uniSlider.x));

		// on declenche les events pour declencher les actions liées à la maj des controls
		wxScrollEvent e;
		e.SetEventObject(st.Slider);
		OnChangeValue(e);
	}
	
	// on maj le type du uniform dans le container
	// genre float, vec2, vec3, vec4
	if (m_UniformViewItem != 0)
		m_UniformViewItem->UpdateTypeName();

	int n = Sliders.size();

	if (n == 4)
		m_bpButton10->Enable(false);

	if (n > 1)
	{
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
			SliderControlStruct sli = it->second;
			if (sli.DeleteBtn != 0)
				sli.DeleteBtn->Enable(true);
		}
	}
	else
	{
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
			SliderControlStruct sli = it->second;
			if (sli.DeleteBtn != 0)
				sli.DeleteBtn->Enable(false);
		}
	}

	return res;
}

void UniformSlider::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

SliderControlStruct UniformSlider::AddSlider()
{
//	int nSliders = Sliders.size();

	// on cherche le 1er index de dispo
	int idx = 0;
	while (Sliders.find(idx) != Sliders.end()) // tant que le nom est trouvé on continu
	{
		idx++;
	}

	SliderControlStruct slider;

	slider.Sizer = new wxBoxSizer(wxHORIZONTAL);

	slider.CountStatic = new wxStaticText(this, wxID_ANY, toStr(SlidersLabels[idx]), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	slider.CountStatic->Wrap(-1);
	slider.CountStatic->SetMinSize(wxSize(10, -1));
	slider.Sizer->Add(slider.CountStatic, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	slider.DeleteBtn = new wxBitmapButton(this, wxID_ANY, Delete_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	slider.DeleteBtn->SetMaxSize(wxSize(24, 24));
	slider.DeleteBtn->SetClientData(slider.CountStatic);
	slider.DeleteBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformSlider::OnRemoveSlider), NULL, this);
	slider.DeleteBtn->Enable(false);

	slider.Sizer->Add(slider.DeleteBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	wxBoxSizer* bSizer7 = new wxBoxSizer(wxHORIZONTAL);

	slider.ValueTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT("0.5"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	slider.ValueTextCtrl->SetMaxSize(wxSize(50, -1));
	slider.ValueTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
	bSizer7->Add(slider.ValueTextCtrl, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 1);

	wxStaticLine *stLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer7->Add(stLine, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	slider.BorneInfextCtrl = new wxTextCtrl(this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	slider.BorneInfextCtrl->SetMaxSize(wxSize(50, -1));
	slider.BorneInfextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
	bSizer7->Add(slider.BorneInfextCtrl, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 1);

	slider.Slider = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	slider.Slider->SetMinSize(wxSize(50, -1));
	slider.Slider->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	bSizer7->Add(slider.Slider, 1, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 1);

	slider.BorneSupTextCtrl = new wxTextCtrl(this, wxID_ANY, wxT("1.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	slider.BorneSupTextCtrl->SetMaxSize(wxSize(50, -1));
	slider.BorneSupTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
	bSizer7->Add(slider.BorneSupTextCtrl, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 1);

	slider.Sizer->Add(bSizer7, 1, wxEXPAND, 5);

	containerSizer->Insert(idx, slider.Sizer, 1, wxEXPAND | wxLEFT, 1);

	//containerSizer->Add(slider.Sizer, 1, wxEXPAND | wxLEFT, 1);

	containerSizer->Layout();

	this->Layout();

	this->GetParent()->Layout();

	this->GetParent()->GetParent()->Layout();

	slider.idx = idx;

	Sliders[idx] = slider;

	// responsable du type (float, vec2, vec3, vec4)
	m_uniSlider.count = Sliders.size();

	if (m_mainUniformName.size() > 0)
		UpdateUniformByName(m_mainUniformName, m_uniSlider, true);

	return slider;
}

void UniformSlider::RemoveSlider(int vIdx)
{
	SliderControlStruct slider = Sliders[vIdx];

	if (m_DocShaderArt != 0)
	{
		EditorView *editor = m_DocShaderArt->GetEditorView();
		if (editor != 0)
		{
			string p = "xyzw";
			string name = m_mainUniformName + "." + p[vIdx];
			vector<string::size_type> vec = editor->GetOccurencesOfWord(name, true);
			if (vec.size() > 0)
			{
				// si le MergeInCode est disponible pour cet uniform on va donner la possitilbité de merger avant de detruire l'uniform
				string msg;
				msg += "This Slider componenet is used in code.\n";
				msg += "Do you want merge in code before removing ?\n";
				int answer = wxMessageBox(msg, "ATTENTION !!", wxYES_NO | wxCANCEL, this);
				if (answer == wxYES)
				{
					string val = toStr(slider.ValueTextCtrl->GetValue());
					if (val.find_first_of(".") == string::npos) // non trouvé
						val += ".";
					editor->ReplaceOccurenceOfWordBy(name, val, true);
				}
				else if (answer == wxCANCEL)
				{
					return;
				}
			}
		}
	}

	slider.Slider->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	slider.Slider->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);

	slider.BorneInfextCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
	slider.BorneSupTextCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);

	slider.DeleteBtn->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformSlider::OnRemoveSlider), NULL, this);

	slider.ValueTextCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);

	// http://trac.wxwidgets.org/ticket/12103
	containerSizer->GetItem(slider.Sizer)->DeleteWindows();
	containerSizer->Remove(slider.Sizer);
	containerSizer->Layout();

	this->GetParent()->GetParent()->Layout();
	this->GetParent()->GetParent()->Refresh(true, 0);

	this->Update();

	Sliders.erase(vIdx);

	m_bpButton10->Enable(true);

	m_uniSlider.count = Sliders.size();

	UpdateUniformByName(m_mainUniformName, m_uniSlider, true);
}

void UniformSlider::OnAddSlider(wxCommandEvent& /*event*/)
{
	int n = Sliders.size();
	if (n < 4)
	{
		float infValue = 0.0f;
		float supValue = 1.0f;
		float ratioValue = 0.5f;

		bool res = false;

		if (m_DocShaderArt != 0)
		{
			EditorView *editor = m_DocShaderArt->GetEditorView();
			if (editor != 0)
			{
				string selection = editor->GetSelection(); // si un float est selectionné
				if (selection.size() > 0)
				{
					// est ce un float ?
					if (selection.find(',') == string::npos)
					{
						float v = 0.0f;
						if (StringToFloat(selection, &v) == true)
						{
							if (v < 0.0f)
							{
								supValue = 0.0f;
								infValue = v * 2.0f;
							}
							else if (v > 0.0f)
							{
								infValue = 0.0f;
								supValue = v * 2.0f;
							}
							else
							{
								infValue = 0.0f;
								supValue = 1.0f;
							}

							ratioValue = v;

							string p = "xyzw";
							string name = m_mainUniformName + "." + p[n];
							editor->ReplaceSelectionBy(name);

							if (n == 1) // le slider etait un float alors il faut trouver et ajouter le x au uSliderX
							{
								string newName = m_mainUniformName + ".x";
								editor->ReplaceOccurenceOfWordBy(m_mainUniformName, newName, false);
							}

							res = true;
						}
						else
						{
							g_pMainFrame->SetStatusBarText("Failed to create the Slider Component because the selection is not a float number", 0);

							res = false;
						}
					}
				}
				else
				{
					res = true;
				}
			}
		}
		
		if (res == true)
		{
			SliderControlStruct st = AddSlider();

			st.BorneSupTextCtrl->SetValue(toStr(supValue));
			st.BorneInfextCtrl->SetValue(toStr(infValue));
			st.ValueTextCtrl->SetValue(toStr(ratioValue));

			if (st.idx == 0) inf.x = infValue;
			else if (st.idx == 1) inf.y = infValue;
			else if (st.idx == 2) inf.z = infValue;
			else if (st.idx == 3) inf.w = infValue;

			if (st.idx == 0) sup.x = supValue;
			else if (st.idx == 1) sup.y = supValue;
			else if (st.idx == 2) sup.z = supValue;
			else if (st.idx == 3) sup.w = supValue;

			// on maj le type du uniform dans le container
			// genre float, vec2, vec3, vec4
			if (m_UniformViewItem != 0)
				m_UniformViewItem->UpdateTypeName();

			n = Sliders.size();

			if (n == 4)
				m_bpButton10->Enable(false);

			if (n > 1)
			{
				for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
				{
					SliderControlStruct sli = it->second;
					if (sli.DeleteBtn != 0)
						sli.DeleteBtn->Enable(true);
				}
			}

			// on declenche les events pour declencher les actions liées à la maj des controls
			wxScrollEvent e;
			e.SetEventObject(st.Slider);
			OnChangeValue(e);
		}
		
	}
	else
	{
		wxMessageBox("You can't have more than 4 sliders for one uniform slider because its a vec4 !\n But You can add another slider uniform via the toolbar if you want. ", "WARMING !!");
	}

	this->Layout();
	if (this->m_parent != 0)
		this->m_parent->Layout();
	this->GetParent()->GetParent()->Layout();
	this->GetParent()->GetParent()->GetParent()->Layout();
}

void UniformSlider::OnRemoveSlider(wxCommandEvent& event)
{
	int n = Sliders.size();

	wxBitmapButton *btn = 0;
	btn = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
	if (btn != 0)
	{
		wxStaticText *stxt = 0;
		stxt = static_cast<wxStaticText*>(btn->GetClientData());
		if (stxt != 0)
		{
			string lbl = stxt->GetLabelText().ToStdString();

			int idx = 0;
			for (int i = 0; i < 4; i++)
			{
				if (SlidersLabels[i] == lbl[0])
					idx = i;
			}

			this->RemoveSlider(idx);

			// on maj le type du uniform dans le container
			// genre float, vec2, vec3, vec4
			if (m_UniformViewItem != 0)
				m_UniformViewItem->UpdateTypeName();
		}
	}

	n = Sliders.size();

	if (n == 1)
	{
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
			SliderControlStruct sli = it->second;
			if (sli.DeleteBtn != 0)
				sli.DeleteBtn->Enable(false);
		}
	}

	this->ReOrderWizzle();

	this->Layout();
	if (this->m_parent != 0)
		this->m_parent->Layout();
	this->GetParent()->GetParent()->Layout();
	this->GetParent()->GetParent()->GetParent()->Layout();
}

string UniformSlider::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// la valeur du slider
	// la borne inf
	// la borne sup
	string infStr = "INF=\"";
	string supStr = "SUP=\"";
	string ratioStr = "RATIO=\"";

	int idx = 0;
	for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
	{
//		int i = it->first;
		SliderControlStruct st = it->second;
		if (st.Slider != 0)
		{
			if (idx > 0)
			{
				ratioStr += ";";
				infStr += ";";
				supStr += ";";
			}
			
			if (idx == 0) { infStr += toStr(inf.x); supStr += toStr(sup.x); ratioStr += toStr(m_uniSlider.x); }
			if (idx == 1) { infStr += toStr(inf.y); supStr += toStr(sup.y); ratioStr += toStr(m_uniSlider.y); }
			if (idx == 2) { infStr += toStr(inf.z); supStr += toStr(sup.z); ratioStr += toStr(m_uniSlider.z); }
			if (idx == 3) { infStr += toStr(inf.w); supStr += toStr(sup.w); ratioStr += toStr(m_uniSlider.w); }
			idx++;
		}
	}

	infStr += "\"";
	supStr += "\"";
	ratioStr += "\"";

	xml = infStr + " " + ratioStr + " " + supStr;

	return xml;
}

void UniformSlider::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

// on maj la valeur en focntion du slider
void UniformSlider::OnChangeValue(wxScrollEvent& event)
{
	wxSlider *slider = static_cast<wxSlider*>(event.GetEventObject());
	if (slider != 0)
	{
		SliderControlStruct st;

		// on cherche le struct qui va bien
		int idx = 0;
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
//			int i = it->first;
			st = it->second;
			if (st.Slider != 0)
			if (st.Slider == slider)
				break;
			idx++;
		}

		float v = slider->GetValue() / 100.0f;
//		float newv = 0.;
		if (idx == 0)
			m_uniSlider.x = CalcAndUpdateValue(st, inf.x, sup.x, v);
		else if (idx == 1)
			m_uniSlider.y = CalcAndUpdateValue(st, inf.y, sup.y, v);
		else if (idx == 2)
			m_uniSlider.z = CalcAndUpdateValue(st, inf.z, sup.z, v);
		else if (idx == 3)
			m_uniSlider.w = CalcAndUpdateValue(st, inf.w, sup.w, v);

		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniSlider, true);
	}
}

// on maj le slider mais la valeur reste peu importe qui modifie le texte
void UniformSlider::OnChangeText(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		SliderControlStruct st;

		// on cherche le struct qui va bien
		int idx = 0;
		for (map<int, SliderControlStruct>::iterator it = Sliders.begin(); it != Sliders.end(); ++it)
		{
//			int i = it->first;
			st = it->second;
			if (st.BorneInfextCtrl != 0)
				if (st.BorneInfextCtrl == txtCtrl)
					break;
			if (st.BorneSupTextCtrl != 0)
				if (st.BorneSupTextCtrl == txtCtrl)
					break;
			if (st.ValueTextCtrl != 0)
				if (st.ValueTextCtrl == txtCtrl)
					break;
			idx++;
		}

		wxScrollEvent e;
		e.SetEventObject(st.Slider);

		float t;
		if (StringToFloat(txtCtrl->GetValue().ToStdString(), &t) == true)
		{
			if (floatIsValid(t) == true)
			{
				wxColour col = txtCtrl->GetBackgroundColour();
				if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
				{
					txtCtrl->SetBackgroundColour(wxColour("WHITE"));
					txtCtrl->Refresh(false, 0);
				}

				if (txtCtrl == st.BorneInfextCtrl)
				{
					if (idx == 0) inf.x = t;
					else if (idx == 1) inf.y = t;
					else if (idx == 2) inf.z = t;
					else if (idx == 3) inf.w = t;
				}
				else if (txtCtrl == st.BorneSupTextCtrl)
				{
					if (idx == 0) sup.x = t;
					else if (idx == 1) sup.y = t;
					else if (idx == 2) sup.z = t;
					else if (idx == 3) sup.w = t;
				}
				else if (txtCtrl == st.ValueTextCtrl)
				{
					if (idx == 0) m_uniSlider.x = t;
					else if (idx == 1) m_uniSlider.y = t;
					else if (idx == 2) m_uniSlider.z = t;
					else if (idx == 3) m_uniSlider.w = t;
					if (m_UniformManager != 0)
						m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniSlider, true);
				}

				if (idx == 0)
					CalcAndUpdateSlider(st, inf.x, sup.x, m_uniSlider.x);
				else if (idx == 1)
					CalcAndUpdateSlider(st, inf.y, sup.y, m_uniSlider.y);
				else if (idx == 2)
					CalcAndUpdateSlider(st, inf.z, sup.z, m_uniSlider.z);
				else if (idx == 3)
					CalcAndUpdateSlider(st, inf.w, sup.w, m_uniSlider.w);
			}
		}
		else // ss cant be conerted to float
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

void UniformSlider::CalcAndUpdateSlider(SliderControlStruct vSliderStruct, float vInf, float vSup, float vTextVal)
{
	// same as glsl mix func => https://www.opengl.org/sdk/docs/man/html/mix.xhtml
	/*
	vt = iv * (1 - vs) + sv * vs
	vt/vs = iv/vs*(1-vs) + sv/vs *vs
	vt/vs = iv/vs-iv*vs/vs + sv
	vt/vs = iv/vs-iv*1 + sv
	vt/vs = iv/vs - iv + sv
	vt/vs - iv/vs = -iv +sv
	(vt - iv)/vs = -iv +sv
	vt - iv = (-iv + sv) * vs
	(vt - iv) / (-iv + sv ) = vs
	(vt - iv) / (sv - iv) = vs
	*/

	float val = (vTextVal - vInf) / (vSup - vInf);

	if (vSliderStruct.Slider != 0)
	{
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);

		vSliderStruct.Slider->SetValue(val * 100);

		vSliderStruct.Slider->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
		vSliderStruct.Slider->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformSlider::OnChangeValue), NULL, this);
	}
}

float UniformSlider::CalcAndUpdateValue(SliderControlStruct vSliderStruct, float vInf, float vSup, float vSliderVal)
{
	float val = 0;

	val = vInf * (1 - vSliderVal) + vSup * vSliderVal; // same as glsl mix func => https://www.opengl.org/sdk/docs/man/html/mix.xhtml

	if (vSliderStruct.ValueTextCtrl != 0)
	{
		// pour eviter de declencher les event
		vSliderStruct.ValueTextCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
		vSliderStruct.ValueTextCtrl->SetValue(toStr(val));
		vSliderStruct.ValueTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(UniformSlider::OnChangeText), NULL, this);
	}

	return val;
}

string UniformSlider::ReplaceUniformInCode(string vCode)
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
				int len = arr.size();
				src += ".";

				if (len == 2) dst = "vec2(";
				if (len == 3) dst = "vec3(";
				if (len == 4) dst = "vec4(";

				for (int i = 0; i < len; i++)
				{
					char c = arr[i];

				//	float val = 0.0f;

					if (i > 0)
						dst += ",";

					if (c == 'x' || c == 'r' || c == 's') dst += toStr(m_uniSlider.x);
					else if (c == 'y' || c == 'g' || c == 't') dst += toStr(m_uniSlider.y);
					else if (c == 'z' || c == 'b' || c == 'p') dst += toStr(m_uniSlider.z);
					else if (c == 'w' || c == 'a') dst += toStr(m_uniSlider.w);

					if (i == len - 1 && len > 1)
						dst += ")";

					if (len == 1)
						if (dst.find_first_of(".") == string::npos) // non trouvé
							dst += ".";

					src += c;
				}
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
		else // simple float
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
				if (m_uniSlider.count == 1)
				{
					dst = toStr(m_uniSlider.x);
					if (dst.find_first_of(".") == string::npos) // non trouvé
						dst += ".";
				}
				if (m_uniSlider.x == m_uniSlider.y && m_uniSlider.y == m_uniSlider.z && m_uniSlider.z == m_uniSlider.w)
				{
					if (m_uniSlider.count == 2) dst = "vec2(" + toStr(m_uniSlider.x) + ")";
					if (m_uniSlider.count == 3) dst = "vec3(" + toStr(m_uniSlider.x) + ")";
					if (m_uniSlider.count == 4) dst = "vec4(" + toStr(m_uniSlider.x) + ")";
				}
				else
				{
					if (m_uniSlider.count == 2) dst = "vec2(" + toStr(m_uniSlider.x) + "," + toStr(m_uniSlider.y) + ")";
					if (m_uniSlider.count == 3) dst = "vec3(" + toStr(m_uniSlider.x) + "," + toStr(m_uniSlider.y) + "," + toStr(m_uniSlider.z) + ")";
					if (m_uniSlider.count == 4) dst = "vec4(" + toStr(m_uniSlider.x) + "," + toStr(m_uniSlider.y) + "," + toStr(m_uniSlider.z) + "," + toStr(m_uniSlider.w) + ")";
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

void UniformSlider::ReOrderWizzle()
{
//	int n = Sliders.size();

	string p = "xyzw";
	int arrSize;
	SliderControlStruct *arr = getNewBufferFromMap(Sliders, 0, 0, &arrSize);
	for (int i = 0; i < arrSize; i++)
	{	
		if (arr[i].idx != i) // idx pas à la bonne place
		{
			Sliders[i] = Sliders[arr[i].idx];
			Sliders.erase(arr[i].idx);

			if (i == 0) { inf.x = inf.y; sup.x = sup.y; m_uniSlider.x = m_uniSlider.y; }
			if (i == 1) { inf.y = inf.z; sup.y = sup.z; m_uniSlider.y = m_uniSlider.z; }
			if (i == 2) { inf.z = inf.w; sup.z = sup.w; m_uniSlider.z = m_uniSlider.w; }
			//if (i == 3) { inf.w = inf.x; sup.x = sup.y; m_uniSlider.x = m_uniSlider.y }

			if (m_DocShaderArt != 0)
			{
				EditorView *editor = m_DocShaderArt->GetEditorView();
				if (editor != 0)
				{
					string src = m_mainUniformName + "." + arr[i].CountStatic->GetLabel().ToStdString();
					string dst = m_mainUniformName + "." + p[i];

					editor->ReplaceOccurenceOfWordBy(src, dst, true);
				}
			}

			Sliders[i].idx = i;
			Sliders[i].CountStatic->SetLabel(p[i]);
		}
	}
}
