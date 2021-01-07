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

#include "UniformGamePad360.h"

#include "UniformManager.h"

#include "DocShaderArt.h"

#include "MainFrame.h"

#include <kazmath/kazmath/kazmath.h>
#include <kazmath/kazmath/mat3.h>

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/ControlStartBlue_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformGamePad360, UniformAbstract)
EVT_TIMER(ID_UNIFORM_TIMER, UniformGamePad360::OnTimer)
EVT_TOGGLEBUTTON(ID_UNIFORM_GAMEPAD360_ACCUM_X, UniformGamePad360::OnAccumX)
EVT_TOGGLEBUTTON(ID_UNIFORM_GAMEPAD360_ACCUM_Y, UniformGamePad360::OnAccumY)
EVT_TOGGLEBUTTON(ID_UNIFORM_GAMEPAD360_ACCUM_Z, UniformGamePad360::OnAccumZ)
EVT_TOGGLEBUTTON(ID_UNIFORM_GAMEPAD360_ACCUM_W, UniformGamePad360::OnAccumW)
EVT_BUTTON(ID_UNIFORM_GAMEPAD360_RESET_X, UniformGamePad360::OnResetX)
EVT_BUTTON(ID_UNIFORM_GAMEPAD360_RESET_Y, UniformGamePad360::OnResetY)
EVT_BUTTON(ID_UNIFORM_GAMEPAD360_RESET_Z, UniformGamePad360::OnResetZ)
EVT_BUTTON(ID_UNIFORM_GAMEPAD360_RESET_W, UniformGamePad360::OnResetW)
wxEND_EVENT_TABLE()
UniformGamePad360::UniformGamePad360(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), m_timer(this, ID_UNIFORM_TIMER),
m_OriginSizer(0), m_DirectionSizer(0), m_AccumModeX(false), m_AccumModeY(false), m_AccumModeZ(false), m_AccumModeW(false)
{
	IsMergeInCodeAvailable = true;

	wxBoxSizer* bSizer30;
	bSizer30 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer35;
	bSizer35 = new wxBoxSizer(wxHORIZONTAL);

	m_panel45 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(m_panel45, wxID_ANY, wxT("Left Stick")), wxVERTICAL);

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText20 = new wxStaticText(m_panel45, wxID_ANY, wxT("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_staticText20->Wrap(-1);
	m_staticText20->SetMinSize(wxSize(10, -1));

	bSizer20->Add(m_staticText20, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_textCtrl32 = new wxTextCtrl(m_panel45, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl32->SetMinSize(wxSize(100, -1));
	m_textCtrl32->SetMaxSize(wxSize(50, -1));

	bSizer20->Add(m_textCtrl32, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_button63 = new wxToggleButton(m_panel45, ID_UNIFORM_GAMEPAD360_ACCUM_X, wxT("A"), wxDefaultPosition, wxDefaultSize, 0);
	m_button63->SetMinSize(wxSize(25, -1));

	bSizer20->Add(m_button63, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_button631 = new wxButton(m_panel45, ID_UNIFORM_GAMEPAD360_RESET_X, wxT("R"), wxDefaultPosition, wxDefaultSize, 0);
	m_button631->SetMinSize(wxSize(25, -1));

	bSizer20->Add(m_button631, 0, 0, 5);


	sbSizer1->Add(bSizer20, 1, wxEXPAND, 1);

	wxBoxSizer* bSizer201;
	bSizer201 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText201 = new wxStaticText(m_panel45, wxID_ANY, wxT("Y"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_staticText201->Wrap(-1);
	m_staticText201->SetMinSize(wxSize(10, -1));

	bSizer201->Add(m_staticText201, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_textCtrl321 = new wxTextCtrl(m_panel45, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl321->SetMinSize(wxSize(100, -1));
	m_textCtrl321->SetMaxSize(wxSize(50, -1));

	bSizer201->Add(m_textCtrl321, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_button62 = new wxToggleButton(m_panel45, ID_UNIFORM_GAMEPAD360_ACCUM_Y, wxT("A"), wxDefaultPosition, wxDefaultSize, 0);
	m_button62->SetMinSize(wxSize(25, -1));

	bSizer201->Add(m_button62, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_button632 = new wxButton(m_panel45, ID_UNIFORM_GAMEPAD360_RESET_Y, wxT("R"), wxDefaultPosition, wxDefaultSize, 0);
	m_button632->SetMinSize(wxSize(25, -1));

	bSizer201->Add(m_button632, 0, 0, 5);


	sbSizer1->Add(bSizer201, 1, wxEXPAND, 5);


	m_panel45->SetSizer(sbSizer1);
	m_panel45->Layout();
	sbSizer1->Fit(m_panel45);
	bSizer35->Add(m_panel45, 0, wxALL, 1);

	m_panel451 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxStaticBoxSizer* sbSizer11;
	sbSizer11 = new wxStaticBoxSizer(new wxStaticBox(m_panel451, wxID_ANY, wxT("Right Stick")), wxVERTICAL);

	wxBoxSizer* bSizer203;
	bSizer203 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText2011 = new wxStaticText(m_panel451, wxID_ANY, wxT("Z"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_staticText2011->Wrap(-1);
	m_staticText2011->SetMinSize(wxSize(10, -1));

	bSizer203->Add(m_staticText2011, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_textCtrl323 = new wxTextCtrl(m_panel451, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl323->SetMinSize(wxSize(100, -1));
	m_textCtrl323->SetMaxSize(wxSize(50, -1));

	bSizer203->Add(m_textCtrl323, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_button61 = new wxToggleButton(m_panel451, ID_UNIFORM_GAMEPAD360_ACCUM_Z, wxT("A"), wxDefaultPosition, wxDefaultSize, 0);
	m_button61->SetMinSize(wxSize(25, -1));

	bSizer203->Add(m_button61, 0, wxALIGN_CENTER_VERTICAL, 5);

	m_button633 = new wxButton(m_panel451, ID_UNIFORM_GAMEPAD360_RESET_Z, wxT("R"), wxDefaultPosition, wxDefaultSize, 0);
	m_button633->SetMinSize(wxSize(25, -1));

	bSizer203->Add(m_button633, 0, 0, 5);


	sbSizer11->Add(bSizer203, 1, wxEXPAND, 1);

	wxBoxSizer* bSizer2011;
	bSizer2011 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText20111 = new wxStaticText(m_panel451, wxID_ANY, wxT("W"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_staticText20111->Wrap(-1);
	m_staticText20111->SetMinSize(wxSize(10, -1));

	bSizer2011->Add(m_staticText20111, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_textCtrl3211 = new wxTextCtrl(m_panel451, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl3211->SetMinSize(wxSize(100, -1));
	m_textCtrl3211->SetMaxSize(wxSize(50, -1));

	bSizer2011->Add(m_textCtrl3211, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	m_button6 = new wxToggleButton(m_panel451, ID_UNIFORM_GAMEPAD360_ACCUM_W, wxT("A"), wxDefaultPosition, wxDefaultSize, 0);
	m_button6->SetMinSize(wxSize(25, -1));

	bSizer2011->Add(m_button6, 0, wxALIGN_CENTER_VERTICAL, 0);

	m_button634 = new wxButton(m_panel451, ID_UNIFORM_GAMEPAD360_RESET_W, wxT("R"), wxDefaultPosition, wxDefaultSize, 0);
	m_button634->SetMinSize(wxSize(25, -1));

	bSizer2011->Add(m_button634, 0, 0, 5);


	sbSizer11->Add(bSizer2011, 1, wxEXPAND, 5);


	m_panel451->SetSizer(sbSizer11);
	m_panel451->Layout();
	sbSizer11->Fit(m_panel451);
	bSizer35->Add(m_panel451, 0, wxALL, 1);


	bSizer37->Add(bSizer35, 0, 0, 5);


	bSizer14->Add(bSizer37, 0, 0, 5);


	bSizer30->Add(bSizer14, 1, wxEXPAND, 5);


	this->SetSizer(bSizer30);
	this->Layout();
}


UniformGamePad360::~UniformGamePad360()
{
	
}

string UniformGamePad360::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		string THUMB;
		string ACCUM;

		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			if (attrib == "ACCUM") ACCUM = value.getS();
			if (attrib == "THUMB") THUMB = value.getS();
		}

		//m_radioBox1->SetSelection((int)m_CurrentMode);

		std::size_t count = 0;
		int idx = 0;
		vector<string> vec;
		// on va parser les string qui ressemble à ca "0.5;500.2;25.2;7.0"

		//THUMB
		vec = DebugLogger::splitString(THUMB, ';');
		if (vec.size() > count) count = vec.size();
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				float v = 0.;
				stringstream ss = stringstream(s);
				ss >> v;
				if (ss)
				{
					if (idx == 0) m_uniThumbLR.x = v;
					else if (idx == 1) m_uniThumbLR.y = v;
					else if (idx == 2) m_uniThumbLR.z = v;
					else if (idx == 3) m_uniThumbLR.w = v;
				}
				else
				{
					g_pMainFrame->SetStatusBarText("The THUMB attribute of the file is corrupted and not taken into account", 0);
					LogStr("error UnifromGamePad360::Init() => attrib THUMB is bad ! ");
				}
					
				idx++;
			}
		}

		//ACCUM
		vec = DebugLogger::splitString(ACCUM, ';');
		if (vec.size() > count) count = vec.size();
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				bool v = false;
				stringstream ss = stringstream(s);
				ss >> v;
				if (ss)
				{
					if (idx == 0) {	m_button63->SetValue(v); m_AccumModeX = v;}
					else if (idx == 1) { m_button62->SetValue(v); m_AccumModeY = v; }
					else if (idx == 2){ m_button61->SetValue(v); m_AccumModeZ = v; }
					else if (idx == 3){ m_button6->SetValue(v); m_AccumModeW = v; }
				}
				else
				{
					g_pMainFrame->SetStatusBarText("The ACCUM attribute of the file is corrupted and not taken into account", 0);
					LogStr("error UnifromGamePad360::Init() => attrib ACCUM is bad ! ");
				}
				idx++;
			}
		}

		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniThumbLR, false);
	}
	else
	{
		m_uniThumbLR.x = 0.0f;
		m_uniThumbLR.y = 0.0f;
		m_uniThumbLR.z = 0.0f;
		m_uniThumbLR.w = 0.0f;
	}


	m_timer.Start(10); //100ms
	CXBOXController(1);

	return res;
}

void UniformGamePad360::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformGamePad360::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
	SetDatas(vValue);
}

void UniformGamePad360::SetDatas(cUniformVariant/* vValue*/)
{

}

string UniformGamePad360::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// le mode accum
	// le vec4 du thumb

	string accum = "ACCUM=\"" + toStr(m_AccumModeX) + ";" + toStr(m_AccumModeY) + ";" + toStr(m_AccumModeZ) + ";" + toStr(m_AccumModeW) + "\"";
	string thumb = "THUMB=\"" + toStr(m_uniThumbLR.x) + ";" + toStr(m_uniThumbLR.y) + ";" + toStr(m_uniThumbLR.z) + ";" + toStr(m_uniThumbLR.w) + "\"";

	xml = accum + " " + thumb;

	return xml;
}

void UniformGamePad360::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

//mat3 getRotXMat(float a){ return mat3(1., 0., 0., 0., cos(a), -sin(a), 0., sin(a), cos(a)); }
//mat3 getRotYMat(float a){ return mat3(cos(a), 0., sin(a), 0., 1., 0., -sin(a), 0., cos(a)); }
//mat3 getRotZMat(float a){ return mat3(cos(a), -sin(a), 0., sin(a), cos(a), 0., 0., 0., 1.); }
static inline kmMat3* kmRotatefX(kmMat3* pOut, float angle, float x, float y, float z)
{
	kmVec3 axis;
	kmMat3 rotation;
	kmVec3Fill(&axis, x, y, z);
	kmMat3FromRotationAxisAngle(&rotation, &axis, kmDegreesToRadians(angle));
	kmMat3MultiplyMat3(pOut, pOut, &rotation);
	return pOut;
}
void UniformGamePad360::OnTimer(wxTimerEvent& /*event*/)
{
	if (IsConnected() == true)
	{
		GetState();

		bool modifiedValue = false;

		/*kmVec3 vec3Org;
		kmVec3 vec3Dir;
		kmVec3Fill(&vec3Org, m_NavOrigin.x, m_NavOrigin.y, m_NavOrigin.z);
		kmVec3Fill(&vec3Dir, m_NavDirection.x, m_NavDirection.y, m_NavDirection.z);

		kmMat3 mat3Dir;
		kmMat3Fill(&mat3Dir, {});
		*/
		//////////////////////////////////////////////////////////////
		// joystick gauche
		//////////////////////////////////////////////////////////////
		cVec2 LeftThumb(_controllerState.Gamepad.sThumbLX, _controllerState.Gamepad.sThumbLY);
		cVec2 LeftThumbSign = LeftThumb.Sign();
		float LeftThumbMagnitude = abs(LeftThumb.x);
		if (LeftThumbMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (LeftThumbMagnitude > 32767) LeftThumbMagnitude = 32767;
			LeftThumbMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			LeftThumbMagnitude /= (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); // normalize
			if (m_AccumModeX == true)
				m_uniThumbLR.x += LeftThumbSign.x * LeftThumbMagnitude;
			else
				m_uniThumbLR.x = LeftThumbSign.x * LeftThumbMagnitude;
			modifiedValue = true;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			if (m_AccumModeX == false)
				m_uniThumbLR.x = 0.0;
		}
		m_textCtrl32->SetLabelText(toStr(m_uniThumbLR.x));

		LeftThumbMagnitude = abs(LeftThumb.y);
		if (LeftThumbMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (LeftThumbMagnitude > 32767) LeftThumbMagnitude = 32767;
			LeftThumbMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			LeftThumbMagnitude /= (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); // normalize
			if (m_AccumModeY == true)
				m_uniThumbLR.y += LeftThumbSign.y * LeftThumbMagnitude;
			else
				m_uniThumbLR.y = LeftThumbSign.y * LeftThumbMagnitude;
			modifiedValue = true;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			if (m_AccumModeY == false)
				m_uniThumbLR.y = 0.0;
		}
		m_textCtrl321->SetLabelText(toStr(m_uniThumbLR.y));

		//////////////////////////////////////////////////////////////
		// joystick droit
		//////////////////////////////////////////////////////////////
		cVec2 RightThumb(_controllerState.Gamepad.sThumbRX, _controllerState.Gamepad.sThumbRY);
		cVec2 RightThumbSign = RightThumb.Sign();
		float RightThumbMagnitude = abs(RightThumb.x);
		if (RightThumbMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (RightThumbMagnitude > 32767) RightThumbMagnitude = 32767;
			RightThumbMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			RightThumbMagnitude /= (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); // normalize
			if (m_AccumModeZ == true)
				m_uniThumbLR.z += RightThumbSign.y * RightThumbMagnitude;
			else
				m_uniThumbLR.z = RightThumbSign.y * RightThumbMagnitude;
			modifiedValue = true;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			if (m_AccumModeZ == false)
				m_uniThumbLR.z = 0.0;
		}
		m_textCtrl323->SetLabelText(toStr(m_uniThumbLR.z));
		
		RightThumbMagnitude = abs(RightThumb.y);
		if (RightThumbMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
		{
			if (RightThumbMagnitude > 32767) RightThumbMagnitude = 32767;
			RightThumbMagnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
			RightThumbMagnitude /= (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE); // normalize
			if (m_AccumModeW == true)
				m_uniThumbLR.w += RightThumbSign.y * RightThumbMagnitude;
			else
				m_uniThumbLR.w = RightThumbSign.y * RightThumbMagnitude;
			modifiedValue = true;
		}
		else //if the controller is in the deadzone zero out the magnitude
		{
			if (m_AccumModeW == false)
				m_uniThumbLR.w = 0.0;
		}
		m_textCtrl3211->SetLabelText(toStr(m_uniThumbLR.w));

		// Update Uniform
		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniThumbLR, modifiedValue);
	}
}

void UniformGamePad360::CXBOXController(int playerNumber)
{
	// Set the Controller Number
	_controllerNum = playerNumber - 1;
}

XINPUT_STATE UniformGamePad360::GetState()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	XInputGetState(_controllerNum, &_controllerState);

	return _controllerState;
}

bool UniformGamePad360::IsConnected()
{
	// Zeroise the state
	ZeroMemory(&_controllerState, sizeof(XINPUT_STATE));

	// Get the state
	DWORD Result = XInputGetState(_controllerNum, &_controllerState);

	if (Result == ERROR_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// vibrate range 0 to 65535
void UniformGamePad360::Vibrate(int leftVal, int rightVal)
{
	// Create a Vibraton State
	XINPUT_VIBRATION Vibration;

	// Zeroise the Vibration
	ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

	// Set the Vibration Values
	Vibration.wLeftMotorSpeed = leftVal;
	Vibration.wRightMotorSpeed = rightVal;

	// Vibrate the controller
	XInputSetState(_controllerNum, &Vibration);
}

void UniformGamePad360::OnStart(wxCommandEvent& /*event*/)
{
	if (IsConnected() == true)
	{
		//Vibrate(25000, 50000);
	}
}

void UniformGamePad360::OnResetX(wxCommandEvent& /*event*/)
{
	m_textCtrl32->SetValue("0");
	m_uniThumbLR.x = 0;
	UpdateUniformByName(m_mainUniformName, m_uniThumbLR, true);
}
void UniformGamePad360::OnResetY(wxCommandEvent& /*event*/)
{
	m_textCtrl321->SetValue("0");
	m_uniThumbLR.y = 0;
	UpdateUniformByName(m_mainUniformName, m_uniThumbLR, true);
}
void UniformGamePad360::OnResetZ(wxCommandEvent& /*event*/)
{
	m_textCtrl323->SetValue("0");
	m_uniThumbLR.z = 0;
	UpdateUniformByName(m_mainUniformName, m_uniThumbLR, true);
}
void UniformGamePad360::OnResetW(wxCommandEvent& /*event*/)
{
	m_textCtrl3211->SetValue("0");
	m_uniThumbLR.w = 0;
	UpdateUniformByName(m_mainUniformName, m_uniThumbLR, true);
}
void UniformGamePad360::OnAccumX(wxCommandEvent& /*event*/)
{
	m_AccumModeX = m_button63->GetValue();
}
void UniformGamePad360::OnAccumY(wxCommandEvent& /*event*/)
{
	m_AccumModeY = m_button62->GetValue();
}
void UniformGamePad360::OnAccumZ(wxCommandEvent& /*event*/)
{
	m_AccumModeZ = m_button61->GetValue();
}
void UniformGamePad360::OnAccumW(wxCommandEvent& /*event*/)
{
	m_AccumModeW = m_button6->GetValue();
}

string UniformGamePad360::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	string src = m_mainUniformName;

	string dst;

	cUniformVariant var = m_uniThumbLR;

	size_t pos = 0;
	size_t pos2 = 0;
	while ((pos = code.find(src, pos)) != std::string::npos)
	{
		// ce code ne permet pas de resoudre les code de merde du style .xzyw.zw.yx.x qui correspond a z du buf originel
		string arr = code.substr(pos + src.length(), 6); // .xyzw
		if (arr[0] == '.')
		{
			pos2 = arr.find_first_of("xyzwrgbastp");
			if (pos2 == 5)
			{
				return code;
			}
			else
			{
				string params = ".";
				int count = 0;
				for (int i = 0; i < 4; i++)
				{
					char c = arr[i + 1];

					pos2 = toStr(c).find_first_of("xyzwrgbastp");
					if (pos2 == 0)
					{
						if (count == 0)
						{
							if (c == 'x' || c == 'r' || c == 's') var.x = m_uniThumbLR.x;
							else if (c == 'y' || c == 'g' || c == 't') var.x = m_uniThumbLR.y;
							else if (c == 'z' || c == 'b' || c == 'p') var.x = m_uniThumbLR.z;
							else if (c == 'w' || c == 'a') var.x = m_uniThumbLR.w;
							params += c;
						}
						else if (count == 1)
						{
							if (c == 'x' || c == 'r' || c == 's') var.y = m_uniThumbLR.x;
							else if (c == 'y' || c == 'g' || c == 't') var.y = m_uniThumbLR.y;
							else if (c == 'z' || c == 'b' || c == 'p') var.y = m_uniThumbLR.z;
							else if (c == 'w' || c == 'a') var.y = m_uniThumbLR.w;
							params += c;
						}
						else if (count == 2)
						{
							if (c == 'x' || c == 'r' || c == 's') var.z = m_uniThumbLR.x;
							else if (c == 'y' || c == 'g' || c == 't') var.z = m_uniThumbLR.y;
							else if (c == 'z' || c == 'b' || c == 'p') var.z = m_uniThumbLR.z;
							else if (c == 'w' || c == 'a') var.z = m_uniThumbLR.w;
							params += c;
						}
						else if (count == 3)
						{
							if (c == 'x' || c == 'r' || c == 's') var.w = m_uniThumbLR.x;
							else if (c == 'y' || c == 'g' || c == 't') var.w = m_uniThumbLR.y;
							else if (c == 'z' || c == 'b' || c == 'p') var.w = m_uniThumbLR.z;
							else if (c == 'w' || c == 'a') var.w = m_uniThumbLR.w;
							params += c;
						}
						count++;
					}
				}
				var.count = count;
				src += params;
			}
		}
		else
		{
			string num = code.substr(pos + src.length(), 1); // 2
			pos2 = num.find_first_of("1234567890");
			if (pos2 == 0) // il y a un nuero derriere le uniform ce qui signifie donc que ce n'est pas le bon
			{
				pos += src.length();
				continue;
			}
		}
		if (var.count == 1)
		{
			dst = toStr(var.x);
			if (dst.find_first_of(".") == string::npos)
				dst += ".";
		}

		if (var.count == 2)
		{
			dst = "vec2(" + toStr(var.x) + "," + toStr(var.y) + ")";
			if (var.x == var.y)
			{
				dst = "vec2(" + toStr(var.x) + ")";
			}
		}

		if (var.count == 3)
		{
			dst = "vec3(" + toStr(var.x) + "," + toStr(var.y) + "," + toStr(var.z) + ")";
			if (var.x == var.y && var.y == var.z)
			{
				dst = "vec3(" + toStr(var.x) + ")";
			}
		}

		if (var.count == 4)
		{
			dst = "vec4(" + toStr(var.x) + "," + toStr(var.y) + "," + toStr(var.z) + "," + toStr(var.w) + ")";
			if (var.x == var.y && var.y == var.z && var.z == var.w)
			{
				dst = "vec4(" + toStr(var.x) + ")";
			}
		}

		code.replace(pos, src.length(), dst);
		pos += dst.length();
		src = m_mainUniformName;
//		break; // pour debug => exec remplacement par remplacement
	}

	// on regle les petits porblem de signe avec le remplacement des input
	DebugLogger::ReplaceString(code, "+-", "-");
	//DebugLogger::ReplaceString(code, "--", "+");
	//DebugLogger::ReplaceString(code, "++", "+");
	DebugLogger::ReplaceString(code, "-+", "-");

	return code;
}
