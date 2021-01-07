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

#include "DebugViewToolBar.h"

#include "wx/msw/private.h"

#include "DebugView.h"

#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/Bug_small_icon.xpm"
#include "../icons/20x20/TransformCrop_small_icon.xpm"
#include "../icons/20x20/Cog_small_icon.xpm"

wxBEGIN_EVENT_TABLE(DebugViewToolBar, wxWindow)
EVT_TIMER(ID_FPS_TIMER, DebugViewToolBar::OnTimer)
EVT_TOGGLEBUTTON(ID_PAUSEPLAY_BTN, DebugViewToolBar::DoPausePlayView)
EVT_TOGGLEBUTTON(ID_TOGGLE_CROP_MODE_BTN, DebugViewToolBar::DoCropMode)
EVT_TOGGLEBUTTON(ID_TOGGLE_DEBUG_MODE_BTN, DebugViewToolBar::DoDebugMode)
EVT_BUTTON(ID_CONFIG_SHADER_VIEW_BTN, DebugViewToolBar::DoConfig)
wxEND_EVENT_TABLE()

DebugViewToolBar::DebugViewToolBar(DebugView *vDebugView, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxPanel(parent, id, pos, size, style), m_DebugView(vDebugView), m_FpsComputeTimer(this, ID_FPS_TIMER), m_LastFps(0), m_CropModeActive(false)
{
	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer(wxHORIZONTAL);

	m_ConfigBtn = new  wxBitmapButton(this, ID_CONFIG_SHADER_VIEW_BTN, Cog_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_ConfigBtn->SetMaxSize(wxSize(32, 32));
	bSizer37->Add(m_ConfigBtn, 0, wxALL, 0);

	m_TogglePlayPauseBtn = new  wxBitmapToggleButton(this, ID_PAUSEPLAY_BTN, ControlPlayBlue_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_TogglePlayPauseBtn->SetBitmapPressed(wxBitmap(ControlPauseBlue_small_icon));
	m_TogglePlayPauseBtn->SetMaxSize(wxSize(32, 32));
	bSizer37->Add(m_TogglePlayPauseBtn, 0, wxALL, 0);

	m_ToggleCropBtn = new  wxBitmapToggleButton(this, ID_TOGGLE_CROP_MODE_BTN, TransformCrop_small_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_ToggleCropBtn->SetBitmapPressed(wxBitmap(TransformCrop_small_icon));
	m_ToggleCropBtn->SetMaxSize(wxSize(32, 32));
	bSizer37->Add(m_ToggleCropBtn, 0, wxLEFT, 2);

	/*m_ToggleDebugBtn = new  wxBitmapToggleButton(this, ID_TOGGLE_DEBUG_MODE_BTN, Bug_small, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_ToggleDebugBtn->SetBitmapPressed(wxBitmap(Bug_small));
	m_ToggleDebugBtn->SetMaxSize(wxSize(32, 32));
	bSizer37->Add(m_ToggleDebugBtn, 0, wxLEFT, 2);*/

	m_staticline8 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer37->Add(m_staticline8, 0, wxEXPAND | wxALL, 5);

	m_StaticTextFps = new wxStaticText(this, wxID_ANY, wxT("Fps"), wxDefaultPosition, wxDefaultSize, 0);
	m_StaticTextFps->Wrap(-1);
	m_StaticTextFps->SetMinSize(wxSize(80, -1));
	bSizer37->Add(m_StaticTextFps, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_staticline81 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer37->Add(m_staticline81, 0, wxEXPAND | wxALL, 5);

	m_StaticTextSize = new wxStaticText(this, wxID_ANY, wxT("800x600"), wxDefaultPosition, wxDefaultSize, 0);
	m_StaticTextSize->Wrap(-1);
	m_StaticTextSize->SetMinSize(wxSize(120, -1));
	bSizer37->Add(m_StaticTextSize, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_staticline811 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer37->Add(m_staticline811, 0, wxEXPAND | wxALL, 5);

	m_StaticTextRefreshTime = new wxStaticText(this, wxID_ANY, wxT("0.253s"), wxDefaultPosition, wxDefaultSize, 0);
	m_StaticTextRefreshTime->Wrap(-1);
	m_StaticTextRefreshTime->SetMinSize(wxSize(80, -1));
	bSizer37->Add(m_StaticTextRefreshTime, 0, wxALIGN_CENTER_VERTICAL, 5);

	this->SetSizer(bSizer37);
	this->Layout();
}

DebugViewToolBar::~DebugViewToolBar()
{
	m_FpsComputeTimer.Stop();
}

bool DebugViewToolBar::Init()
{
	m_TogglePlayPauseBtn->SetValue(true);
	wxCommandEvent e;
	e.SetEventObject(m_TogglePlayPauseBtn);
	DoPausePlayView(e);// peremt de demarrer ou non le label de fps

	return true;
}

void DebugViewToolBar::DoPausePlayView(wxCommandEvent& event)
{
	if (m_DebugView != 0)
	{
		wxBitmapToggleButton *btn = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());
		if (btn != 0)
		{
			if (btn->GetValue() == true)
			{
				m_DebugView->Play();
				m_FpsComputeTimer.Start(200);
			}
			else
			{
				m_DebugView->Pause();
				m_FpsComputeTimer.Stop();
				m_DebugView->Refresh(false, 0);
			}
		}
	}
}

void DebugViewToolBar::DoCropMode(wxCommandEvent& event)
{
	wxBitmapToggleButton *btn = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());
	if (btn != 0)
	{
		m_CropModeActive = btn->GetValue();
		if (m_DebugView != 0)
		{
			m_DebugView->SetCropMode(m_CropModeActive);
		}
	}
}

void DebugViewToolBar::DoDebugMode(wxCommandEvent& event)
{
	wxBitmapToggleButton *btn = dynamic_cast<wxBitmapToggleButton*>(event.GetEventObject());
	if (btn != 0)
	{
		m_DebugModeActive = btn->GetValue();
	}
}

void DebugViewToolBar::DoConfig(wxCommandEvent& /*event*/)
{
	
}

void DebugViewToolBar::OnTimer(wxTimerEvent& /*event*/)
{
	if (m_DebugView != 0)
	{
		float fps = m_DebugView->GetFPS();
		SetFps(fps * 0.9f + m_LastFps * 0.1f);
		m_LastFps = fps;

		float exec = m_DebugView->GetExecTime();
		SetExecTime(exec * 0.9f + m_LastExec * 0.1f);
		m_LastExec = exec;
	}
}

void DebugViewToolBar::SetFps(float vFps)
{
	if (vFps >= 0)
	{
		string lbl = "FPS : " + toStr(int(vFps * 100.0f) / 100.0f) + " f/s";

		int w, h;
		m_StaticTextFps->GetTextExtent(lbl, &w, &h);
		m_StaticTextFps->SetMinSize(wxSize(w, -1));
		this->Layout();

		m_StaticTextFps->SetLabel(lbl);
		m_StaticTextFps->Refresh(false, 0);
	}
}

void DebugViewToolBar::SetScreen(int vWidth, int vHeight, bool vIsCrop)
{
	string lbl;
	if (vIsCrop == true)
		lbl = "crop ";
	lbl += toStr(vWidth) + "x" + toStr(vHeight) + " : " + toStr(vWidth*vHeight) + " px";

	int w, h;
	m_StaticTextSize->GetTextExtent(lbl, &w, &h);
	m_StaticTextSize->SetMinSize(wxSize(w, -1));
	this->Layout();

	m_StaticTextSize->SetLabel(lbl);
	m_StaticTextSize->Refresh(false, 0);
}

void DebugViewToolBar::SetExecTime(float vExec)
{
	if (vExec >= 0)
	{
		string lbl = "Exec : " + toStr(int(vExec * 100.0f) / 100.0f) + " ms ( fps Max : " + toStr(int(1000.0f / vExec * 100.0f) / 100.0f) + "f/s )";

		int w, h;
		m_StaticTextRefreshTime->GetTextExtent(lbl, &w, &h);
		m_StaticTextRefreshTime->SetMinSize(wxSize(w, -1));
		this->Layout();

		m_StaticTextRefreshTime->SetLabel(lbl);
		m_StaticTextRefreshTime->Refresh(false, 0);
	}
}