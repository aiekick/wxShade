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

#ifndef __UNIFORMVIEW_H__
#define __UNIFORMVIEW_H__

#include <wx/wx.h>

#include "..\framework\Config.h"
#include "UniformViewItem.h"
#include "UniformAbstract.h"

#include <string>
using namespace std;

class DocShaderArt;
class UniformView : public wxPanel
{
public:
	static UniformView* Create(DocShaderArt* vDocShaderArt, wxWindow *parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, UniformManager *vUniformManager = 0);

public:
	UniformView(DocShaderArt* vDocShaderArt, wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, UniformManager *vUniformManager = 0);
	~UniformView();

	bool AddPane(UniformAbstract *vUPane);

	void OnRemoveUniformItem(wxCommandEvent& event);
	void RemoveAllItems();
	void RemoveItem(UniformViewItem *vItem, bool vCheckUniformUsedInCode);
	
	void CheckUniformWidgetCurrentlyUsed();

	wxBoxSizer* GetContainer(){ return m_scrollSizer; }

private:
	wxScrolledWindow* m_scrolledWindow;
	wxBoxSizer* m_scrollSizer;
	DocShaderArt* m_DocShaderArt;
	UniformManager *m_UniformManager;
};

#endif //__UNIFORMVIEW_H__
