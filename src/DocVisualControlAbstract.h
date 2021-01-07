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

#ifndef __DOCVISUALCONTROLABSTRACT_H__
#define __DOCVISUALCONTROLABSTRACT_H__

#include <wx\wx.h>
#include "DocumentAbstract.h"

#include <string>
using namespace std;

class DocVisualControlAbstract : public wxPanel
{
private:
	bool m_IsInit;

protected:
	virtual void OnSize(wxSizeEvent& event) = 0;
	virtual bool Init() = 0;

public:
	DocVisualControlAbstract(wxWindow* parent,
		wxWindowID id = wxID_ANY,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxSize(630, 355),
		long style = wxTAB_TRAVERSAL,
		string vFileName = "") :
		wxPanel(parent, id, pos, size, style)
	{

	}

	virtual  DocumentAbstract* GetDocument() = 0;
};

#endif //__DOCVISUALCONTROLABSTRACT_H__
