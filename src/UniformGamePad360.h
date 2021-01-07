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

#ifndef __UNIFORMGAMEPAD360_H__
#define __UNIFORMGAMEPAD360_H__

#include "Globals.h"

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
#include <wx/statline.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"


#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <windows.h>
#include <XInput.h>     // XInput API
#pragma comment(lib, "xinput.lib")
#include <string>
using namespace std;

enum GamePas360Mode
{
	GamePadModeDefault = 0,
	GamePadModeAccum = 1,
	GamePadModeNav = 2
};
enum GamePad360ButtonEnum
{
	GamePad_Button_DPAD_UP = 0,
	GamePad_Button_DPAD_DOWN = 1,
	GamePad_Button_DPAD_LEFT = 2,
	GamePad_Button_DPAD_RIGHT = 3,
	GamePad_Button_START = 4,
	GamePad_Button_BACK = 5,
	GamePad_Button_LEFT_THUMB = 6,
	GamePad_Button_RIGHT_THUMB = 7,
	GamePad_Button_LEFT_SHOULDER = 8,
	GamePad_Button_RIGHT_SHOULDER = 9,
	GamePad_Button_A = 10,
	GamePad_Button_B = 11,
	GamePad_Button_X = 12,
	GamePad_Button_Y = 13,
	GamePadButton_Max = 14
};

// GamePad Indexes
enum GamePad360IndexEnum
{
	GamePadIndex_One = 0,
	GamePadIndex_Two = 1,
	GamePadIndex_Three = 2,
	GamePadIndex_Four = 3,
};

// The GamePad State Stuct, were we store the buttons positions
struct GamePadState
{
	bool _buttons[GamePadButton_Max];
	cVec2 _left_thumbstick;               // <= I'm using a Vector2 (floats) class but you can replaced it with a float X and Y or whatever your Vector2 class is
	cVec2 _right_thumbstick;
	float _left_trigger;
	float _right_trigger;
	// Just to clear all values to default
	void reset()
	{
		for (int i = 0; i<(int)GamePadButton_Max; ++i) _buttons[i] = false;
		_left_thumbstick.Set(0.0f);
		_right_thumbstick.Set(0.0f);
		_left_trigger = _right_trigger = 0.0f;
	}
};

class UniformGamePad360 : public UniformAbstract
{
public:
	UniformGamePad360(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformGamePad360();

	virtual string ReplaceUniformInCode(string vCode);

protected:
	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);
	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnStart(wxCommandEvent& /*event*/);
	virtual void OnAccumX(wxCommandEvent& event);
	virtual void OnAccumY(wxCommandEvent& event);
	virtual void OnAccumZ(wxCommandEvent& event);
	virtual void OnAccumW(wxCommandEvent& event);
	virtual void OnResetX(wxCommandEvent& event);
	virtual void OnResetY(wxCommandEvent& event);
	virtual void OnResetZ(wxCommandEvent& event);
	virtual void OnResetW(wxCommandEvent& event);

private:
	wxTimer		m_timer; // timer
	XINPUT_STATE _controllerState;
	int _controllerNum;
	GamePadState	State;
	cUniformVariant m_uniThumbLR;// xy Thumb Left, zw Thumb Right
	
	bool m_AccumModeX;
	bool m_AccumModeY;
	bool m_AccumModeZ;
	bool m_AccumModeW;

	bool m_Modified;

private:
	wxPanel* m_panel45;
	wxStaticText* m_staticText20;
	wxTextCtrl* m_textCtrl32;
	wxToggleButton* m_button63;
	wxButton* m_button631;
	wxStaticText* m_staticText201;
	wxTextCtrl* m_textCtrl321;
	wxToggleButton* m_button62;
	wxButton* m_button632;
	wxPanel* m_panel451;
	wxStaticText* m_staticText2011;
	wxTextCtrl* m_textCtrl323;
	wxToggleButton* m_button61;
	wxButton* m_button633;
	wxStaticText* m_staticText20111;
	wxTextCtrl* m_textCtrl3211;
	wxToggleButton* m_button6;
	wxButton* m_button634;

	wxBoxSizer* m_NavSizer;
	wxStaticBoxSizer* m_OriginSizer;
	wxStaticBoxSizer* m_DirectionSizer;

public:
	void CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);

	void UpdateValue(cUniformVariant vValue);
	void SetDatas(cUniformVariant vValue);

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMGAMEPAD360_H__