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

#ifndef __OUTPUTWIN_H__
#define __OUTPUTWIN_H__

#include <wx/wx.h>

#include "..\framework\Config.h"
#include "Globals.h"

#include <string>
using namespace std;

class OutputWin : public wxPanel
{
public:
	OutputWin(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);
	~OutputWin();

	void AddToOutput(string vTxt);
	void ClearContent();

private:
	wxListBox *m_ListBox;

};


#endif //__OUTPUTWIN_H__