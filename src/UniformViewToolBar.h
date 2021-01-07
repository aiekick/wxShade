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

#ifndef __UNIFORMVIEWTOOLBAR_H__
#define __UNIFORMVIEWTOOLBAR_H__

#include "Globals.h"

#include "wx/wxprec.h"

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/aui/auibook.h>
#include <wx/frame.h>
#include <wx/tglbtn.h>
#include <wx/aui/aui.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statline.h>

#include "wx/timer.h"
#include "wx/math.h"
#include "wx/window.h"

#include "..\framework\Config.h"
#include "..\framework\Tools\Tools.hpp"
#include "..\framework\GLShaderRenderer.h"

#include <map>

class UniformView;

class UniformViewToolBar : public wxPanel
{
public:
	UniformViewToolBar(UniformView *vUniformView, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style);

	virtual ~UniformViewToolBar();

	bool Init();

	void CreateFilterButtons(UniformsWidgetTypeEnum vUniformType);
	void DestroyFilterButtons(UniformsWidgetTypeEnum vUniformType);

private:
	wxBoxSizer* bSizerTogglesButtons;
	std::map<UniformsWidgetTypeEnum, int> m_UniformVisiblity; // bool => toggle of corresponding button
	std::map<wxBitmapToggleButton*, UniformsWidgetTypeEnum> m_ToggleBtns;
private:
	UniformView *m_UniformView;

protected:
	virtual void DoFilters(wxCommandEvent& event);
	virtual void DoFiltersAll(wxCommandEvent& event);
	virtual void DoFiltersNone(wxCommandEvent& event);

protected:
	wxDECLARE_NO_COPY_CLASS(UniformViewToolBar);
	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMVIEWTOOLBAR_H__