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

#include "ImportFromShaderToy.h"

#include "UniformView.h"

#include "UniformManager.h"

#include "EditorView.h"

#include "DocShaderArt.h"

wxBEGIN_EVENT_TABLE(ShaderToyImportWizard, wxWizard)
	EVT_WIZARD_PAGE_CHANGED(wxID_ANY, ShaderToyImportWizard::OnPageChange)
wxEND_EVENT_TABLE()

ShaderToyImportWizard::ShaderToyImportWizard(wxWindow* parent, wxWindowID id, const wxString& title, const wxBitmap& bitmap, const wxPoint& pos, long style)
{
	this->Create(parent, id, title, bitmap, pos, style);
	this->SetSizeHints(wxSize(800, 600), wxSize(800, 600));

	wxWizardPageSimple* m_wizPage1 = new wxWizardPageSimple(this);
	m_pages.Add(m_wizPage1);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_staticText1 = new wxStaticText(m_wizPage1, wxID_ANY, wxT("Put the code from Shadertoy In the editor under"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer3->Add(m_staticText1, 0, wxALL | wxEXPAND, 5);

	m_EditorFragmentView = EditorView::Create(0, ShaderTypeEnum::SHADER_TYPE_FRAGMENT, m_wizPage1, ID_IMPORT_FROM_SHADERTOY_EDITOR, wxDefaultPosition, wxDefaultSize, 0);
	m_EditorFragmentView->ShowHideToolBar(false);

	bSizer3->Add(m_EditorFragmentView, 1, wxEXPAND | wxALL, 5);

	m_wizPage1->SetSizer(bSizer3);
	m_wizPage1->Layout();
	bSizer3->Fit(m_wizPage1);

	wxWizardPageSimple* m_wizPage2 = new wxWizardPageSimple(this);
	m_pages.Add(m_wizPage2);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	m_staticText2 = new wxStaticText(m_wizPage2, wxID_ANY, wxT("This is what we replace for the conversion :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer4->Add(m_staticText2, 0, wxALL | wxEXPAND, 5);

	m_listCtrl1 = new wxListCtrl(m_wizPage2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_AUTOARRANGE | wxLC_REPORT | wxLC_SINGLE_SEL);
	bSizer4->Add(m_listCtrl1, 1, wxALL | wxEXPAND, 5);

	m_wizPage2->SetSizer(bSizer4);
	m_wizPage2->Layout();
	bSizer4->Fit(m_wizPage2);

	this->Centre(wxBOTH);

	for (unsigned int i = 1; i < m_pages.GetCount(); i++)
	{
		m_pages.Item(i)->SetPrev(m_pages.Item(i - 1));
		m_pages.Item(i - 1)->SetNext(m_pages.Item(i));
	}
}

ShaderToyImportWizard::~ShaderToyImportWizard()
{
	m_pages.Clear();
}

void ShaderToyImportWizard::OnPageChange(wxWizardEvent& /*event*/)
{
	wxWizardPage *page = this->GetCurrentPage();
	if (page == m_pages.Item(1))
	{
		bool res = false;

		// new doc
		DocShaderArt *doc = 0;
		if (m_AuiNotebook != 0)
		{
			int count = m_AuiNotebook->GetPageCount();
			string filename = "Shader " + toStr(count);
			string currentPath = wxGetCwd();
			string filepathname = currentPath + "\\" + filename;

			string script = m_EditorFragmentView->GetCode();
			if (script.size() > 0)
			{
				doc = new DocShaderArt(m_AuiNotebook, ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, filename);

				doc->ImportDocumentString(script, ImportTypeEnum::INPORT_SHADERTOY);

				m_AuiNotebook->AddPage(doc, filename, true, wxNullBitmap);
			}
		}
		else
		{
			res = false;
		}
	}
}