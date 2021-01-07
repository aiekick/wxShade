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

#ifndef __UNIFORMTIME_H__
#define __UNIFORMTIME_H__

#include "UniformAbstract.h"

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
#include <wx/tglbtn.h>
#include <wx/sizer.h>
#include <wx/panel.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "Globals.h"

#include <string>
using namespace std;

class UniformTime : public UniformAbstract
{
public:
	UniformTime(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformTime();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);

	void PlayTimer();
	void PauseTimer();
	void ResetTimer(float vValue);
	
	virtual string ReplaceUniformInCode(string vCode);

private:
	wxTimer		m_timer; // timer
	int64		m_lastTick; // dernier tick
	int64		m_currentTick; // tick au dernier timer event
	int64		m_lastDeltaTick; // dernier delata en derniere tick et tick actuel // mermet de gerer les pause du timer
	float		m_currentTime; // temps actuel
	float		m_multCoef; // coef de mulitplication du temps
	int			m_TimeDir; // sens du temps
	int64		m_lastInvertTick; // nombre de tisk a la derniere inversion // pour l'inversion du sens du temps
	cUniformVariant m_uniTime;
	bool		m_Modified;

protected:
	virtual void OnTimeValueChange(wxCommandEvent& event);
	virtual void OnPlayPauseTime(wxCommandEvent& event);
	virtual void OnResetTime(wxCommandEvent& event);
	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnInvCheck(wxCommandEvent& event);
	virtual void OnMultCoefChange(wxCommandEvent& event);
	virtual void SetParamsFromXML(map<string, string> vParams);

protected:
	wxTextCtrl* m_textCtrl1;
	wxBitmapToggleButton* m_bpButton1;
	wxBitmapButton* m_bpButton2;
	wxCheckBox* m_checkBox2;
	wxTextCtrl* m_textCtrl3;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMTIME_H__