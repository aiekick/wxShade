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

#include "UniformView.h"
#include "DocShaderArt.h"

UniformView* UniformView::Create(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, UniformManager *vUniformManager)
{
	UniformView *res = 0;

	if (vDocShaderArt != 0 && parent != 0)
	{
		res = new UniformView(vDocShaderArt, parent, id, pos, size, style, vUniformManager);
		if (res != 0)
		{

		}
	}

	return res;
}

UniformView::UniformView(DocShaderArt* vDocShaderArt, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, UniformManager *vUniformManager)
: wxPanel(parent, id, pos, size, style), m_UniformManager(vUniformManager), m_DocShaderArt(vDocShaderArt)
{
	wxBoxSizer* MainSizer = new wxBoxSizer(wxVERTICAL);

	m_scrolledWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER | wxHSCROLL | wxVSCROLL);
	MainSizer->Add(m_scrolledWindow, 1, wxALL | wxEXPAND, 5);
	
	m_scrolledWindow->SetScrollRate(5, 5);
	
	m_scrollSizer = new wxBoxSizer(wxVERTICAL);

	m_scrolledWindow->SetSizer(m_scrollSizer);

	m_scrolledWindow->Layout();

	m_scrollSizer->Fit(m_scrolledWindow);

	this->SetSizer(MainSizer);
	this->Layout();
}

UniformView::~UniformView()
{
	RemoveAllItems();
}

bool UniformView::AddPane(UniformAbstract *vUPane)
{
	bool res = false;

	if (vUPane != 0)
	{
		UniformViewItem *item = UniformViewItem::Create(m_DocShaderArt, m_scrolledWindow, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxRAISED_BORDER | wxTAB_TRAVERSAL, m_UniformManager);
		item->Connect(ID_REMOVE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformView::OnRemoveUniformItem), 0, this);
		// reparent : old was MainFrame
		vUPane->Reparent(item);
		vUPane->SetUniformViewItem(item);

		if (item->SetUniformAbstractPane(vUPane) == false)
		{
			RemoveItem(item, false);
		}
		else
		{
			m_DocShaderArt->GetUniformViewToolBar()->CreateFilterButtons(vUPane->GetType());
			m_scrollSizer->Add(item, 0, wxEXPAND, 5);
			this->Layout();
			res = true;
		}
		
	}

	return res;
}

void UniformView::OnRemoveUniformItem(wxCommandEvent& event)
{
	wxButton *btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn != 0)
	{
		UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(btn->GetParent());
		this->RemoveItem(vItem, true);

		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			doc->SetModified(true);
		}
	}
}

void UniformView::RemoveItem(UniformViewItem *vItem, bool vCheckUniformUsedInCode)
{
	if (vItem != 0)
	{
		// on va d'abord verifier que le uniform n'est pas utilisé par l'editor
		EditorView *editor = m_DocShaderArt->GetEditorView();
		if (editor != 0)
		{
			if (vCheckUniformUsedInCode == true)
			{
				string name = vItem->GetUniformAbstractPane()->m_mainUniformName;
				string code = editor->GetCode();
				size_t pos = 0;
				while ((pos = code.find(name, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
				{
					string num = code.substr(pos + name.length(), 1); // 2
					size_t pos2 = num.find_first_of("1234567890");
					if (pos2 != 0) // c'est bien celui qu'on cherche
					{
						// donc on averti le user de faire attention car le uniform est dans le code

						// si le MergeInCode est disponible pour cet uniform on va donner la possitilbité de merger avant de detruire l'uniform
						string msg = "This Uniform Widget is used by the editor.\n";
						if (vItem->GetUniformAbstractPane()->IsMergeInCodeAvailable == true)
						{
							msg += "Do you want merge in code before destroying this Uniform Widget ?\n";
							int answer = wxMessageBox(msg, "ATTENTION !!", wxYES_NO | wxCANCEL, this);
							if (answer == wxYES)
							{
								string modifiedCode = vItem->GetUniformAbstractPane()->ReplaceUniformInCode(code);
								if (modifiedCode.size() > 0)
									editor->SetCode(modifiedCode);
							}
							else if (answer == wxCANCEL)
							{
								return;
							}
						}
						else // merge in code non dispo on averti juste
						{
							int answer = wxMessageBox(msg, "ATTENTION !!", wxOK | wxCANCEL, this);
							if (answer == wxCANCEL)
							{
								return;
							}

						}
						break;
					}
					pos += name.length();
				}
			}
			
			if (m_scrollSizer->Detach(vItem) == true)
			{
				ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
				if (doc != 0)
				{
					UniformManager *mgr = doc->GetUniformManager();
					if (mgr != 0)
					{
						UniformAbstract *pane = vItem->GetUniformAbstractPane();
						if (pane != 0)
						{
							list<string> uniNames = pane->GetUniformNames();
							for (list<string>::iterator it = uniNames.begin(); it != uniNames.end(); ++it)
							{
								string uniName = *it;
								if (mgr->RemoveUniformByName(uniName) == true)
								{
									if (it == uniNames.begin())// on le fait que une fois
									{
										vItem->Disconnect(ID_REMOVE_UNIFORM_ABSTRACT_BTN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformView::OnRemoveUniformItem), 0, this);

										m_DocShaderArt->GetUniformViewToolBar()->DestroyFilterButtons(pane->GetType());

										delete vItem;
										vItem = 0;
										this->Layout();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void UniformView::RemoveAllItems()
{
	wxSizerItemList lst = m_scrollSizer->GetChildren();
	wxSizerItemList::iterator iter;
	for (iter = lst.begin(); iter != lst.end(); ++iter)
	{
		wxSizerItem *cur = *iter;
		UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(cur->GetWindow());
		this->RemoveItem(vItem, false);
	}
}

void UniformView::CheckUniformWidgetCurrentlyUsed()
{
	if (m_UniformManager != 0)
	{
		wxSizerItemList lst = m_scrollSizer->GetChildren();
		wxSizerItemList::iterator iter;
		for (iter = lst.begin(); iter != lst.end(); ++iter)
		{
			wxSizerItem *cur = *iter;
			UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(cur->GetWindow());
			if (vItem != 0)
			{
				vItem->CheckUniformWidgetCurrentlyUsed();
			}
		}
	}
}
