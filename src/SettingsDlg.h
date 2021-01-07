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

#ifndef __SETTINGSDLG_H__
#define __SETTINGSDLG_H__

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
#include <wx/propdlg.h>
#include <wx/filepicker.h>

#include "..\framework\Config.h"

#include <algorithm>
#include <string>
using namespace std;

class SettingsDlg : public wxPropertySheetDialog 
{
public:
	static string AssetsDirPath;
	static string ShadersDirPath;

private:
	wxPanel* m_PaneDirectory;
	wxStaticText* m_staticText1;
	wxDirPickerCtrl* m_dirPicker2;
	wxStaticText* m_staticText11;
	wxDirPickerCtrl* m_dirPicker3;

	wxPanel* m_PaneGeneral;

protected:

public:
	SettingsDlg(wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("Settings"), const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);
	~SettingsDlg();

public:
	static bool LoadConfig();
	static DocumentFragmentParseErrorEnum parse(const char* datas);
	static DocumentFragmentParseErrorEnum RecursParsing(tinyxml2::XMLElement* vElem);

	bool SaveConfig();
};

#endif //__SETTINGSDLG_H__