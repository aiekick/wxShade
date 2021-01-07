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

#ifndef __HELPDLG_H__
#define __HELPDLG_H__

#include "Globals.h"

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/html/htmlwin.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/listbook.h>
#include <wx/listctrl.h>
#include <wx/frame.h>

#include "..\framework\Config.h"

#include <algorithm>
#include <string>
using namespace std;

class DocShaderArt;

class HelpDlg : public wxDialog
{
public:

private:
	wxListbook* m_listbook2;

	DocShaderArt *m_DocShaderArt;

protected:

public:
	HelpDlg(wxWindow* parent, DocShaderArt *vDocShaderArt, wxWindowID id = wxID_ANY,
		const wxString& title = wxT("Help"), const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~HelpDlg();

public:
	void AddPaneHtml(string vTabName, string vHtmlString);

};

#endif //__HELPDLG_H__