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

#include "OutputWin.h"

OutputWin* g_pOutputWin = 0;

OutputWin::OutputWin(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxPanel(parent, id, pos, size, style)
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	m_ListBox = new wxListBox(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0);
	bSizer2->Add(m_ListBox, 1, wxALL | wxEXPAND, 5);
	
	this->SetSizer(bSizer2);
	this->Layout();
}


OutputWin::~OutputWin()
{

}

void OutputWin::AddToOutput(string vTxt)
{
	m_ListBox->AppendString(vTxt.c_str());
	LogStr(vTxt);
}

void OutputWin::ClearContent()
{
	m_ListBox->Clear();
}