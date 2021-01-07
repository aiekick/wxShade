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

#include "UniformViewToolBar.h"

#include "wx/msw/private.h"

#include "UniformView.h"

#include "Assets.h"

wxBEGIN_EVENT_TABLE(UniformViewToolBar, wxWindow)
wxEND_EVENT_TABLE()

UniformViewToolBar::UniformViewToolBar(UniformView *vUniformView, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxPanel(parent, id, pos, size, style), m_UniformView(vUniformView)
{
	bSizerTogglesButtons = new wxBoxSizer(wxHORIZONTAL);

	wxButton *toggleNoneBtn = new wxButton(this, wxID_ANY, wxT("None"), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	toggleNoneBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewToolBar::DoFiltersNone), 0, this);
	toggleNoneBtn->SetMaxSize(wxSize(35, 26));
	toggleNoneBtn->SetMinSize(wxSize(35, 26));
	bSizerTogglesButtons->Add(toggleNoneBtn, 0, wxALL, 0);

	wxButton *toggleAllBtn = new wxButton(this, wxID_ANY, wxT("All"), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	toggleAllBtn->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(UniformViewToolBar::DoFiltersAll), 0, this);
	toggleAllBtn->SetMaxSize(wxSize(26, 26));
	toggleAllBtn->SetMinSize(wxSize(26, 26));
	bSizerTogglesButtons->Add(toggleAllBtn, 0, wxALL, 0);

	wxStaticLine *staticLine = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizerTogglesButtons->Add(staticLine, 0, wxALL | wxEXPAND, 5);

	wxStaticText *filters = new wxStaticText(this, wxID_ANY, wxT("Filters : "), wxDefaultPosition, wxDefaultSize, 0);
	filters->Wrap(-1);
	bSizerTogglesButtons->Add(filters, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	
	this->SetSizer(bSizerTogglesButtons);
	this->Layout();
}

UniformViewToolBar::~UniformViewToolBar()
{

}

bool UniformViewToolBar::Init()
{
	return true;
}

void UniformViewToolBar::CreateFilterButtons(UniformsWidgetTypeEnum vUniformType)
{
	if (m_UniformVisiblity.find(vUniformType) == m_UniformVisiblity.end()) // non trouvé
	{
		wxBitmapToggleButton *toggleBtn = new wxBitmapToggleButton(this, wxID_ANY, AssetsManager::gUniformIcon[vUniformType], wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
		toggleBtn->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, wxCommandEventHandler(UniformViewToolBar::DoFilters), 0, this);
		toggleBtn->SetBitmapPressed(AssetsManager::gUniformIcon[vUniformType]);
		toggleBtn->SetMaxSize(wxSize(26, 26));
		toggleBtn->SetMinSize(wxSize(26, 26));
		toggleBtn->SetValue(true);

		m_UniformVisiblity[vUniformType] = 1;
		
		m_ToggleBtns[toggleBtn] = vUniformType;

		bSizerTogglesButtons->Add(toggleBtn, 0, wxALL, 0);

		this->Layout();
	}
	else
	{
		m_UniformVisiblity[vUniformType] ++;
	}
}

void UniformViewToolBar::DestroyFilterButtons(UniformsWidgetTypeEnum vUniformType)
{
	if (m_UniformVisiblity.find(vUniformType) != m_UniformVisiblity.end()) // trouvé
	{
		m_UniformVisiblity[vUniformType]--;
		if (m_UniformVisiblity[vUniformType] <= 0) // plus de item du type recherche donc on detruit le bouton
		{
			wxBitmapToggleButton *ref = 0;
			for (std::map<wxBitmapToggleButton*, UniformsWidgetTypeEnum>::iterator it = m_ToggleBtns.begin(); it != m_ToggleBtns.end(); ++it)
			{
				wxBitmapToggleButton *btn = it->first;
				UniformsWidgetTypeEnum type = it->second;

				if (type == vUniformType)
				{
					ref = btn;
				}
			}
			if (ref != 0)
			{
				if (bSizerTogglesButtons->Detach(ref) == true)
				{
					m_ToggleBtns.erase(ref);
					m_UniformVisiblity.erase(vUniformType);
					delete ref;
					m_UniformView->Layout();
				}
			}
		}

		this->Layout();
	}
}

void UniformViewToolBar::DoFilters(wxCommandEvent& event)
{
	wxBitmapToggleButton *btn = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());
	if (btn != 0)
	{
		bool visibility = btn->GetValue();
		if (m_UniformView != 0)
		{
			wxBoxSizer *container = m_UniformView->GetContainer();
			wxSizerItemList lst = container->GetChildren();
			wxSizerItemList::iterator iter;
			for (iter = lst.begin(); iter != lst.end(); ++iter)
			{
				wxSizerItem *cur = *iter;
				UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(cur->GetWindow());
				if (vItem != 0)
				{
					UniformsWidgetTypeEnum type = vItem->GetUniformAbstract()->GetType();
					if (m_ToggleBtns[btn] == type)
					{
						if (visibility == true)
						{
							//vItem->ShowWithEffect(wxShowEffect::wxSHOW_EFFECT_SLIDE_TO_LEFT);
							vItem->Show();
						}
						else
						{
							//vItem->HideWithEffect(wxShowEffect::wxSHOW_EFFECT_SLIDE_TO_RIGHT);
							vItem->Hide();
						}
					}
				}
			}
		}
	}
	m_UniformView->Layout();
}

void UniformViewToolBar::DoFiltersAll(wxCommandEvent& event)
{
	wxButton *btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn != 0)
	{
		if (m_UniformView != 0)
		{
			wxBoxSizer *container = m_UniformView->GetContainer();
			wxSizerItemList lst = container->GetChildren();
			wxSizerItemList::iterator iter;
			for (iter = lst.begin(); iter != lst.end(); ++iter)
			{
				wxSizerItem *cur = *iter;
				UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(cur->GetWindow());
				if (vItem != 0)
				{
					//vItem->ShowWithEffect(wxShowEffect::wxSHOW_EFFECT_SLIDE_TO_RIGHT);
					vItem->Show();
					for (std::map<wxBitmapToggleButton*, UniformsWidgetTypeEnum>::iterator it = m_ToggleBtns.begin(); it != m_ToggleBtns.end(); ++it)
					{
						wxBitmapToggleButton *togBtn = it->first;
//						UniformsWidgetTypeEnum type = it->second;

						togBtn->SetValue(true);
					}
				}
			}
		}
	}
	m_UniformView->Layout();
}

void UniformViewToolBar::DoFiltersNone(wxCommandEvent& event)
{
	wxButton *btn = dynamic_cast<wxButton*>(event.GetEventObject());
	if (btn != 0)
	{
		if (m_UniformView != 0)
		{
			wxBoxSizer *container = m_UniformView->GetContainer();
			wxSizerItemList lst = container->GetChildren();
			wxSizerItemList::iterator iter;
			for (iter = lst.begin(); iter != lst.end(); ++iter)
			{
				wxSizerItem *cur = *iter;
				UniformViewItem *vItem = dynamic_cast<UniformViewItem*>(cur->GetWindow());
				if (vItem != 0)
				{
					//vItem->HideWithEffect(wxShowEffect::wxSHOW_EFFECT_SLIDE_TO_RIGHT);
					vItem->Hide();
					for (auto it = m_ToggleBtns.begin(); it != m_ToggleBtns.end(); ++it)
					{
						wxBitmapToggleButton * togBtn = it->first;
//						UniformsWidgetTypeEnum type = it->second;

						togBtn->SetValue(false);
					}
				}
			}
		}
	}
	m_UniformView->Layout();
}