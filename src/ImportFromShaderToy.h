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

#ifndef __SHADERTOYIMPORTWIZARD_H__
#define __SHADERTOYIMPORTWIZARD_H__

#include "Globals.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/aui/aui.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/stc/stc.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listctrl.h>
#include <wx/wizard.h>
#include <wx/dynarray.h>

#include "..\framework\Config.h"
#include "..\framework\Tools\Tools.hpp"
#include "..\framework\GLShaderRenderer.h"

#include "ShaderArtDocument.h"

#include <algorithm>
#include <string>
using namespace std;

class EditorView;

WX_DEFINE_ARRAY_PTR(wxWizardPageSimple*, WizardPages);

class ShaderToyImportWizard : public wxWizard
{
private:
	wxAuiNotebook* m_AuiNotebook;

protected:
	virtual void OnPageChange(wxWizardEvent& event);

protected:
	wxStaticText* m_staticText1;
	EditorView* m_EditorFragmentView;
	wxStaticText* m_staticText2;
	wxListCtrl* m_listCtrl1;

public:

	ShaderToyImportWizard(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("Import code from ShaderToy"), const wxBitmap& bitmap = wxNullBitmap, 
		const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	WizardPages m_pages;
	~ShaderToyImportWizard();

	void SetShaderBook(wxAuiNotebook *vAuiNotebook) { m_AuiNotebook = vAuiNotebook; }
	
	wxDECLARE_EVENT_TABLE();
};

#endif //__SHADERTOYIMPORTWIZARD_H__