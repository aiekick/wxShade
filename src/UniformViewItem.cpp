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

#include "UniformViewItem.h"
#include "DocShaderArt.h"

#include "MainFrame.h"

wxBEGIN_EVENT_TABLE(UniformViewItem, wxPanel)
EVT_CHECKBOX(ID_UNIFORM_WIDGET_EDIT_CHECKBOX, UniformViewItem::OnEditUniformNameCheck)
//EVT_TEXT_ENTER(ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX, UniformViewItem::OnChangeUniformName)
wxEND_EVENT_TABLE()

UniformViewItem* UniformViewItem::Create(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, UniformManager *vUniformManager)
{
	UniformViewItem *res = 0;

	if (vDocShaderArt != 0 && parent != 0)
	{
		res = new UniformViewItem(vDocShaderArt, parent, id, pos, size, style, vUniformManager);
		if (res != 0)
		{

		}
	}

	return res;
}

UniformViewItem::UniformViewItem(DocShaderArt* vDocShaderArt, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, UniformManager *vUniformManager)
: wxPanel(parent, id, pos, size, style), m_UniformManager(vUniformManager), bSizer4(0), m_DocShaderArt(vDocShaderArt)
{

}


UniformViewItem::~UniformViewItem()
{
	m_MergeBtn->Disconnect(ID_MERGE_IN_CODE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewItem::OnMergeInCodeUniformItem), 0, this);
	// on laisse faire wx pour detruire tout les enfants
}

bool UniformViewItem::SetUniformAbstractPane(UniformAbstract *vUniformAbstract)
{
	if (vUniformAbstract == 0) return false;

	m_UniformAbstract = vUniformAbstract;
	
	m_UniformAbstract->SetDocShaderArt(m_DocShaderArt);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	//wxBoxSizer* bSizer3;
	//bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_RemoveBtn = new wxButton(this, ID_REMOVE_UNIFORM_ABSTRACT_BTN, wxT("X"), wxDefaultPosition, wxDefaultSize, 0);
	m_RemoveBtn->SetMaxSize(wxSize(20, 20));
	
	bSizer2->Add(m_RemoveBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 2);

	m_MergeBtn = new wxButton(this, ID_MERGE_IN_CODE_UNIFORM_ABSTRACT_BTN, wxT(">"), wxDefaultPosition, wxDefaultSize, 0);
	m_MergeBtn->SetHelpText(wxT("Merge In Code : Uniform Name in Code is Replaced by is corresponding value "));
	m_MergeBtn->SetMaxSize(wxSize(20, 20));
	m_MergeBtn->Connect(ID_MERGE_IN_CODE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewItem::OnMergeInCodeUniformItem), 0, this);
	m_MergeBtn->Enable(m_UniformAbstract->IsMergeInCodeAvailable);
	bSizer2->Add(m_MergeBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 2);

	//m_HelpBtn = new wxButton(this, ID_TYPE_UNIFORM_ABSTRACT_BTN, wxT("?"), wxDefaultPosition, wxDefaultSize, 0);
	//m_HelpBtn->SetMaxSize(wxSize(20, 20));

	//bSizer3->Add(m_HelpBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 2);

	//bSizer2->Add(bSizer3, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	bSizer4 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer *bSizer6 = new wxBoxSizer(wxHORIZONTAL);

	wxButton *m_SearchBtn = new wxButton(this, ID_SEARCH_IN_CODE_UNIFORM_ABSTRACT_BTN, wxT("|>"), wxDefaultPosition, wxDefaultSize, 0);
	m_SearchBtn->SetHelpText(wxT("Search Uniform Name in Code"));
	m_SearchBtn->SetMaxSize(wxSize(20, 20));
	m_SearchBtn->Connect(ID_SEARCH_IN_CODE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewItem::OnSearchInCodeUniformItem), 0, this);
	bSizer6->Add(m_SearchBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	wxTextCtrl *ctrl = new wxTextCtrl(this, ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX, "", wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_PROCESS_ENTER /* | wxTE_READONLY*/);
	ctrl->SetHelpText(wxT("Uniform Name / Color Indicate if the uniform is used by GPU"));
	ctrl->Connect(ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(UniformViewItem::OnChangeUniformName), 0, this);
	ctrl->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 75, 90, 92, false, wxT("Comic Sans MS")));
	bSizer6->Add(ctrl, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	m_SearchBtn->SetClientData(ctrl);

	bSizer4->Add(bSizer6, 0, wxLEFT | wxRIGHT, 1);

	bSizer2->Add(bSizer4, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

	//m_checkBox1 = new wxCheckBox(this, ID_UNIFORM_WIDGET_EDIT_CHECKBOX, wxT("Edit"), wxDefaultPosition, wxDefaultSize, 0);
	//m_checkBox1->SetMaxSize(wxSize(35, -1));

	//bSizer2->Add(m_checkBox1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	EditorView*editor = m_DocShaderArt->GetEditorView();
	if (editor != 0)
	{
		string selection = editor->GetSelection();
		string replaceStr = m_UniformAbstract->Init(selection); // on lance l'init du control
		if (replaceStr.size() > 0)
		{
			editor->ReplaceSelectionBy(replaceStr);
		}
	}
	else
	{
		m_UniformAbstract->Init(""); // on lance l'init du control
	}

	if (m_UniformAbstract->InitIsFailed == true)
		return false;

	list<string> name = m_UniformAbstract->GetUniformNames();
	string first = *name.begin();
	ctrl->SetValue(first);
	m_textCtrls[first] = ctrl;

	bSizer2->Add(m_UniformAbstract, 1, wxALL | wxEXPAND, 2);

	this->SetSizer(bSizer2);
	this->Layout();

	UpdateTypeName();

	return true;
}

UniformAbstract* UniformViewItem::GetUniformAbstractPane()
{
	return m_UniformAbstract;
}

// declenche le emrge in code de l'uniform
// cad va convertir le controle en format pour le mettre dans le ceo
// ne fait ca appelle la methode de chaque widget qui va so'ccuper de formater ca donnée
void UniformViewItem::OnMergeInCodeUniformItem(wxCommandEvent& /*event*/)
{
	if (m_UniformAbstract != 0)
	{
		list<string> name = m_UniformAbstract->GetUniformNames();

		EditorView *editor = m_DocShaderArt->GetEditorView();
		if (editor != 0)
		{
			string code = editor->GetCode();
			string modifiedCode = m_UniformAbstract->ReplaceUniformInCode(code);
			if (modifiedCode.size() > 0)
				editor->SetCode(modifiedCode);
		}
	}
}

// declenche la recherche in code du nom de l'uniform
void UniformViewItem::OnSearchInCodeUniformItem(wxCommandEvent& event)
{
	if (m_UniformAbstract != 0)
	{
		wxButton *btn = dynamic_cast<wxButton*>(event.GetEventObject());
		if (btn != 0)
		{
			void *data = btn->GetClientData();
			if (data != 0)
			{
				wxTextCtrl *ctrl = static_cast<wxTextCtrl*>(data);
				if (ctrl != 0)
				{
					string name = ctrl->GetValue();
					EditorView *editor = m_DocShaderArt->GetEditorView();
					if (editor != 0)
					{
						if (name != m_LastSearchUniformName)
						{
							m_Occurences = editor->GetOccurencesOfWord(name, true);
							if (m_Occurences.size() > 0)
							{
								m_LastSearchUniformName = name;
								m_LastSearch = m_Occurences.begin();
							}
						}

						if (m_Occurences.size() > 0)
						{
							string code = editor->GetCode();

							if (m_LastSearch == m_Occurences.end())
								m_LastSearch = m_Occurences.begin();

							int wordStart = *m_LastSearch;

							editor->SetVisibleWord(wordStart, (int)name.size());

							if (m_LastSearch != m_Occurences.end())
								m_LastSearch++;
						}
					}
				}
			}
		}
	}
}

void UniformViewItem::OnEditUniformNameCheck(wxCommandEvent& event)
{
	for (std::map<string, wxTextCtrl*>::iterator it = m_textCtrls.begin(); it != m_textCtrls.end(); ++it)
	{
		string key = it->first;
		wxTextCtrl *ctrl = it->second;

		if (ctrl != 0)
		{
			ctrl->Enable(event.IsChecked());
		}
	}
}

// apres avoir changé le nom de l'uniform et avoir tapé sur enter pour appliquer la modif
void UniformViewItem::OnChangeUniformName(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		// on va montrer une boite de dialogue pour avertir que le changement va affecter le code
		string msg = "Are you sure to change the name of this Unifrom ?\n";
		int answer = wxMessageBox(msg, "ATTENTION !!", wxYES_NO | wxCANCEL, this);
		if (answer == wxYES)
		{
			// le nom ne doit avoir que : absdefghijklmnopqrstuvwxyz0123456789
			// apres on va donc checker la validité du nom
			string newName = txtCtrl->GetValue();
			if (newName.size() > 0)
			{
				if (newName.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") == string::npos) // pas trouvé donc c'est ok
				{
					// et apres on va appliquer les changement dans le manager 
					// et dans le code ( et relancer la compilation .. )
					// quoique faire que le user doive declencher lui meme la recompilation pour appliquer les changements est une bonne chose.. a voir

					// on va trouver l'ancien nom affecté a ce controle
					string currentName;
					for (std::map<string, wxTextCtrl*>::iterator it = m_textCtrls.begin(); it != m_textCtrls.end(); ++it)
					{
						string key = it->first;
						wxTextCtrl *ctrl = it->second;

						if (ctrl != 0)
						{
							if (ctrl == txtCtrl)
							{
								currentName = key;
								break;
							}
						}
					}

					// il s'agit d'etre sur de notre coup avant de demarrer la maj des nom sinon c'est le merdier
					// car il sera quasiement impossible de revenir en arriere correctement, ca necesseiterait de sauver avant les donné et de savoir 
					// comment ca merdé pour savoir quoi changer.. => trop complexe et je pense pas que ce soit necessaire
					if (m_UniformAbstract != 0)
					{
						if (m_UniformManager != 0)
						{
							if (currentName.size() > 0) // nom actuel de l'uniform trouvé
							{
								EditorView *editor = m_DocShaderArt->GetEditorView();
								if (editor != 0)
								{
									// on demande au manager de remplacer le nom dans la bd du manager
									string err = m_UniformManager->ChangeUniformName(currentName, newName);
									if (err.size() == 0)
									{
										// on laisse le widget changer lui meme le nom vu qu'on ignore comment il s'est organisé en interne avec les noms
										m_UniformAbstract->ChangeUniformName(currentName, newName);

										// on va maintenant remplacer le nom de l'uniform dans le code
										string code = editor->GetCode();
										if (code.size() > 0)
										{
											bool change = false;
											size_t pos = 0;
											while ((pos = code.find(currentName, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
											{
												string num = code.substr(pos + currentName.length(), 1); // 2
												size_t pos2 = num.find_first_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
												if (pos2 == string::npos) // non trouve donc c'est bien le nom qu'on cherche
												{
													code.replace(pos, currentName.length(), newName);
													change = true;
												}
												pos += newName.length();
											}
											if (change == true)
											{
												editor->SetCode(code);
											}
										}
										else
										{
											msg = "The change in code has failed ! The code is empty !";
											g_pMainFrame->SetStatusBarText(msg, 0);
											wxMessageBox(msg, "FAIL !!");
											return;
										}
											
										// derneire etape modifier la clef pour ce controle
										wxTextCtrl *ctrl = m_textCtrls[currentName];
										m_textCtrls.erase(currentName);
										m_textCtrls[newName] = ctrl;
									}
									else
									{
										msg = err;
										g_pMainFrame->SetStatusBarText(msg, 0);
										wxMessageBox(msg, "FAIL !!");
										return;
									}
								}
								else
								{
									msg = "Cant find the editor.. !";
									g_pMainFrame->SetStatusBarText(msg, 0);
									wxMessageBox(msg, "FAIL !!");
									return;
								}
							}
							else
							{
								msg = "Weird, but the old uniform name is unfindable... !";
								g_pMainFrame->SetStatusBarText(msg, 0);
								wxMessageBox(msg, "FAIL !!");
								return;
							}
						}
						else
						{
							msg = "Cant find the uniform manager... !";
							g_pMainFrame->SetStatusBarText(msg, 0);
							wxMessageBox(msg, "FAIL !!");
							return;
						}
					}
					else
					{
						msg = "Weird, but not any widget for this item !";
						g_pMainFrame->SetStatusBarText(msg, 0);
						wxMessageBox(msg, "FAIL !!");
						return;
					}
				}
				else // bad name
				{
					msg = "the name is not correct !\nThe name must contain only these symbols :\n abcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n0123456789 \n";
					g_pMainFrame->SetStatusBarText(msg, 0);
					wxMessageBox(msg, "FAIL !!");
					return;
				}
			}
			else // le nom ne puet aps etre vide
			{
				msg = "the name cant be empty ! \n";
				g_pMainFrame->SetStatusBarText(msg, 0);
				wxMessageBox(msg, "FAIL !!");
				return;
			}

			
		}
	}
}

// met a jout le type de l'uniform actuel
// en base et aussi quand le type mute comme le slider qui passe de float à vec2 d vec2 à vec3 et de vec3 à vec4
// et ineversement
// maj le tooltip du control du text de l'uniform
void UniformViewItem::UpdateTypeName()
{
	if (m_UniformManager != 0)
	{
		for (std::map<string, wxTextCtrl*>::iterator it = m_textCtrls.begin(); it != m_textCtrls.end(); ++it)
		{
			string key = it->first;
			wxTextCtrl *ctrl = it->second;

			if (ctrl != 0)
			{
				string name = ctrl->GetValue();
				UniformStruct uni = m_UniformManager->GetUniformStructByName(name);
				if (uni.name == name)
				{
					string type;
					if (uni.uType != UniformsTypeEnum::UTYPENONE)
					{
						type = m_UniformManager->ConvertUniformsTypeEnumToString(uni.uType);
					}
					else
					{
						UniformsTypeEnum glslType = m_UniformManager->GetGlslTypeForUniformsWidgetTypeEnum(uni.wType, uni.value.count);
						type = m_UniformManager->ConvertUniformsTypeEnumToString(glslType);

					}
					ctrl->SetToolTip(type);
				}
			}
		}
	}
}

// ajoute un unfirom dans le manager et dans le widget actuel
// cad un deuxieme uniform comme le size des texture par exemple ou les point des bvh
string UniformViewItem::AddUniform(string vName, UniformsTypeEnum vType, bool vCanBeSaved)
{
	string uniformName;

	if (m_UniformManager != 0)
	{
		uniformName = m_UniformManager->AddUniformTypeFromWidget(vName, vType, m_UniformAbstract, vCanBeSaved);

		m_UniformAbstract->AddUniformName(uniformName);

		wxBoxSizer *bSizer6 = new wxBoxSizer(wxHORIZONTAL);

		wxButton *m_SearchBtn = new wxButton(this, ID_SEARCH_IN_CODE_UNIFORM_ABSTRACT_BTN, wxT("|>"), wxDefaultPosition, wxDefaultSize, 0);
		m_SearchBtn->SetHelpText(wxT("Search Uniform Name in Code"));
		m_SearchBtn->SetMaxSize(wxSize(20, 20));
		m_SearchBtn->Connect(ID_SEARCH_IN_CODE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewItem::OnSearchInCodeUniformItem), 0, this);
		bSizer6->Add(m_SearchBtn, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

		wxTextCtrl *ctrl = new wxTextCtrl(this, ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX, uniformName, wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_PROCESS_ENTER /* | wxTE_READONLY*/);
		ctrl->SetHelpText(wxT("Uniform Name / Color Indicate if the uniform is used by GPU"));
		ctrl->Connect(ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX, wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler(UniformViewItem::OnChangeUniformName), 0, this);
		ctrl->SetFont(wxFont(wxNORMAL_FONT->GetPointSize(), 75, 90, 92, false, wxT("Comic Sans MS")));
		m_textCtrls[vName] = ctrl;
		bSizer6->Add(ctrl, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 1);

		m_SearchBtn->SetClientData(ctrl);

		bSizer4->Add(bSizer6, 0, wxLEFT | wxRIGHT, 1);

		this->Layout();

		UpdateTypeName();
	}

	return uniformName;
}

// enleve le widget de l'uniform lié à l'item et enleve aussi le uniform du manager
string UniformViewItem::RemoveUniform(string vName)
{
	string uniformName = vName;

	if (m_UniformManager != 0)
	{
		if (m_UniformManager->RemoveUniformByName(vName) == true)
		{
			uniformName = "";

			if (m_textCtrls.find(vName) != m_textCtrls.end()) // item trouvé
			{
				wxTextCtrl *ctrl = m_textCtrls[vName];
				if (ctrl != 0)
				{
					if (ctrl->GetValue() == vName)
					{
						size_t countErased = m_textCtrls.erase(vName);
						if (countErased == 1)
						{
							bSizer4->Detach(ctrl);

							delete ctrl;
						}
						else
						{

						}
					}
				}
			}

			this->Layout();

			UpdateTypeName();
		}
	}

	return uniformName;
}

// verifie si l'e unifrom est utilisé par le sahder (si le loc est different de -1)
// et affiche vis la couleur verte si utilsié ou rouge si pas utilisé dans le control de text du nom de l'uniform
void UniformViewItem::CheckUniformWidgetCurrentlyUsed()
{
	if (m_UniformManager != 0)
	{
		for (std::map<string, wxTextCtrl*>::iterator it = m_textCtrls.begin(); it != m_textCtrls.end(); ++it)
		{
			string key = it->first;
			wxTextCtrl *ctrl = it->second;

			if (ctrl != 0)
			{
				string name = ctrl->GetValue();
				UniformStruct uni = m_UniformManager->GetUniformStructByName(name);
				if (uni.name == name)
				{
					if (uni.loc > -1)
					{
						ctrl->SetBackgroundColour(wxColour(151, 240, 204));
					}
					else
					{
						ctrl->SetBackgroundColour(wxColour(245, 177, 177));
					}
				}
				ctrl->Refresh(true);
			}
		}
	}
}