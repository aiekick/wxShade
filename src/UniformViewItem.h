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

#ifndef __UNIFORMVIEWITEM_H__
#define __UNIFORMVIEWITEM_H__

#include <wx/wx.h>

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/bmpbuttn.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"

#include "Globals.h"
#include "UniformAbstract.h"

#include <map>
#include <string>
using namespace std;

// histoire de recuperer la perte du focus
class MyTextCtrl : public wxTextCtrl
{
	virtual void SetFocus()
	{

	}
};

//#define COUNT_UNIFORMS 100
class UniformViewItem : public wxPanel
{
public:
	static UniformViewItem* Create(DocShaderArt* vDocShaderArt, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, UniformManager *vUniformManager = 0);

public:
	UniformViewItem(DocShaderArt* vDocShaderArt, wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, UniformManager *vUniformManager = 0);
	~UniformViewItem();

	bool SetUniformAbstractPane(UniformAbstract *vUniformAbstract);
	UniformAbstract* GetUniformAbstractPane();

	void UpdateTypeName();

	string AddUniform(string vName, UniformsTypeEnum vType, bool vCanBeSaved);
	string RemoveUniform(string vName);

	UniformAbstract* GetUniformAbstract(){ return m_UniformAbstract; }

	void CheckUniformWidgetCurrentlyUsed();

protected:
	virtual void OnEditUniformNameCheck(wxCommandEvent& event);
	virtual void OnMergeInCodeUniformItem(wxCommandEvent& event);
	virtual void OnSearchInCodeUniformItem(wxCommandEvent& event);
	virtual void OnChangeUniformName(wxCommandEvent& event);

private:
	UniformAbstract *m_UniformAbstract;
	UniformManager *m_UniformManager;
	DocShaderArt *m_DocShaderArt;

public:

	wxButton *m_RemoveBtn;
	wxButton *m_HelpBtn;
	wxButton *m_MergeBtn;
	
	map<string, wxTextCtrl*> m_textCtrls;
	wxCheckBox* m_checkBox1;
	wxBoxSizer* bSizer4;

	// search
	vector<std::string::size_type>::iterator m_LastSearch;
	vector<std::string::size_type> m_Occurences;
	string m_LastSearchUniformName;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMVIEWITEM_H__