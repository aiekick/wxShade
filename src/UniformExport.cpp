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

#include "UniformExport.h"

#include "UniformManager.h"

#include <gif_h/gif.h>

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/ControlStartBlue_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformExport, UniformAbstract)
	EVT_TEXT(ID_UNIFORM_TIME_WIDGET_TIME_VALUE, UniformExport::OnTimeValueChange)
	EVT_TOGGLEBUTTON(ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, UniformExport::OnPlayPauseTime)
	EVT_BUTTON(ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN, UniformExport::OnResetTime)
	EVT_TIMER(ID_UNIFORM_TIMER, UniformExport::OnTimer)
	EVT_CHECKBOX(ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX, UniformExport::OnInvCheck)
	EVT_TEXT(ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE, UniformExport::OnMultCoefChange)
wxEND_EVENT_TABLE()

UniformExport::UniformExport(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), 
m_timer(this, ID_UNIFORM_TIMER), m_lastTick(0), m_currentTime(0), m_lastDeltaTick(0), m_multCoef(1), m_TimeDir(1), m_lastInvertTick(0), m_currentTick(0), m_Modified(false)
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticText31 = new wxStaticText( this, wxID_ANY, wxT("Start Time :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	bSizer2->Add( m_staticText31, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl1 = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_textCtrl1->SetMaxSize( wxSize( 50,-1 ) );
	
	bSizer2->Add( m_textCtrl1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_staticText311 = new wxStaticText( this, wxID_ANY, wxT("End Time :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText311->Wrap( -1 );
	bSizer2->Add( m_staticText311, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textCtrl11 = new wxTextCtrl( this, wxID_ANY, wxT("10.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_textCtrl11->SetMaxSize( wxSize( 50,-1 ) );
	
	bSizer2->Add( m_textCtrl11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_bpButton1 = new wxBitmapToggleButton(this, ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN, wxBitmap(ControlPlayBlue_small_icon), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton1->SetBitmapPressed(wxBitmap(ControlPauseBlue_small_icon));
	m_bpButton1->SetMaxSize(wxSize(32, 32));
	bSizer2->Add( m_bpButton1, 0, wxALL, 5 );
	
	m_textCtrl111 = new wxTextCtrl( this, wxID_ANY, wxT("0.0"), wxDefaultPosition, wxDefaultSize, wxTE_CENTRE );
	m_textCtrl111->SetMaxSize( wxSize( 50,-1 ) );
	
	bSizer2->Add( m_textCtrl111, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_gauge1 = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	m_gauge1->SetValue( 0 ); 
	bSizer2->Add( m_gauge1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	this->SetSizer( bSizer2 );
	this->Layout();

	//Magick::InitializeMagick(NULL);
}

UniformExport::~UniformExport()
{

}

string UniformExport::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		/*
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string time = "TIME=\"" + m_textCtrl1->GetValue().ToStdString() + "\"";
			//string invCheck = "INV=\"" + toStr(m_checkBox2->IsChecked()) + "\"";
			//string coef = "COEF=\"" + m_textCtrl3->GetValue().ToStdString() + "\"";
			//string playState = "PLAYED=\"" + toStr(m_bpButton1->GetNormalState() == wxBitmapToggleButton::State::State_Pressed) + "\"";

			if (attrib == "TIME")
			{
				ResetTimer(value.getF());
			}

			if (attrib == "INV")
			{
				if (value.getB() == true)
				{
					m_checkBox2->Set3StateValue(wxCheckBoxState::wxCHK_CHECKED);
				}
				else
				{
					m_checkBox2->Set3StateValue(wxCheckBoxState::wxCHK_UNCHECKED);
				}
			}

			if (attrib == "COEF")
			{
				m_textCtrl3->SetValue(value.getS());
			}

			if (attrib == "PLAYED")
			{
				m_bpButton1->SetValue(value.getB());
			}
		}

		// on declenche les events pour declencher les actions liées à la maj des controls
		wxTimerEvent t; // event vide
		OnTimer(t);
		wxCommandEvent e; // event vide
		OnTimeValueChange(e);
		OnInvCheck(e);
		OnMultCoefChange(e);
		OnPlayPauseTime(e);
		m_Modified = false; // init ==> 1er chargement
		*/
	}
	else
	{

	}

	return res;
}

void UniformExport::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
	ResetTimer(0.0f);//force uniform set and view refresh
}

// declenche quand le test change... pas seulement quand c'est le user qui le change
void UniformExport::OnTimeValueChange(wxCommandEvent& event)
{
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		if (txtCtrl->IsEnabled() == true)
		{
			m_Modified = true;
			float t;
			stringstream ss = stringstream(txtCtrl->GetValue().ToStdString());
			ss >> t;
			if (ss)
			{
				if (floatIsValid(t) == true)
				{
					wxColour col = txtCtrl->GetBackgroundColour();
					if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
					{
						txtCtrl->SetBackgroundColour(wxColour("WHITE"));
						txtCtrl->Refresh(false, 0);
					}
					ResetTimer(t);//force uniform set and view refresh
				}
			}
			else // ss cant be conerted to float
			{
				txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
				txtCtrl->Refresh(false, 0);
			}
		}
	}
}

void UniformExport::OnPlayPauseTime(wxCommandEvent& /*event*/)
{
	if (m_ShaderRenderer != 0 && m_ShaderView != 0 && m_UniformManager != 0)
	{
		// faut pouvoir piloter la vue et controler frame par frame
		// la vue paint avec le renderer via le dc

		DWORD frameRate = 30;

		try
		{
			wxFileDialog fileDlg(this, _("Save As"), "", "", "video files (*.avi)|*.avi|Sequence of Png files (*.png)|*.png|Animated Gif files (*.gif)|*.gif", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (fileDlg.ShowModal() == wxID_OK)
			{
				int fileExt = fileDlg.GetFilterIndex();
		
				wxFileName filenameutil = fileDlg.GetPath().ToStdString();
				string fileext = filenameutil.GetExt();
				string filename = filenameutil.GetName();
				string filepathname = fileDlg.GetPath().ToStdString();
				string filepath = fileDlg.GetDirectory();

				cVec2 screenSize = m_ShaderView->GetScreenSize();

				// pour la video il faut que largeur et hauteur soient multiple de 4
				int w = (int(screenSize.x / 4) * 4); // w % 4 == 0
				int h = (int(screenSize.y / 4) * 4); // h % 4 == 0

				HDC hdc = m_ShaderView->GetHDC();

				m_ShaderView->SetScreenSize(w, h);

				if (fileExt == 0) // avi
				{
					frameRate = 30.0f;

					CAVIGenerator AviGen;
					BITMAPINFOHEADER lpbih;
					HRESULT hr;

					// Fill in the fields of the info header
					// https://msdn.microsoft.com/en-us/library/windows/desktop/dd183376%28v=vs.85%29.aspx
					lpbih.biSize = 40;
					lpbih.biWidth = w;
					lpbih.biHeight = h;
					lpbih.biPlanes = 1;
					lpbih.biBitCount = 32;
					lpbih.biSizeImage = 0; //This may be set to zero for BI_RGB bitmaps.
					lpbih.biCompression = BI_RGB;
					lpbih.biXPelsPerMeter = 0;
					lpbih.biYPelsPerMeter = 0;
					lpbih.biClrUsed = 0;
					lpbih.biClrImportant = 0;

					// set 15fps
					AviGen.SetRate(frameRate);

					// give info about bitmap
					AviGen.SetBitmapHeader(lpbih);

					// set filename, extension ".avi" is appended if necessary
					std::wstring uniString = DebugLogger::s2ws(filepathname);
					AviGen.SetFileName((LPCWSTR)uniString.c_str());

					// retreiving size of image
					lpbih = AviGen.GetBitmapHeader();

					// allocating memory
					BYTE* bmBits = new BYTE[4 * w * h]; // 4 car R G B A fait 4 BYTE

					m_ShaderView->Pause();

					float startTime;
					stringstream st = stringstream(m_textCtrl1->GetValue().ToStdString());
					st >> startTime;
					if (st)
					{
						float endTime;
						stringstream et = stringstream(m_textCtrl11->GetValue().ToStdString());
						et >> endTime;
						if (et)
						{
							hr = AviGen.InitEngine();
							if (FAILED(hr))
							{
								LogStr(AviGen.GetLastErrorMessage());
							}
							else
							{
								float length = endTime - startTime;
								long countFrames = length * frameRate;

								float rate = frameRate;
								for (int i = 0; i < countFrames; i++)
								{
									m_uniTime.x = startTime + i / rate;
									m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniTime, false);

									glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
									glClear(GL_COLOR_BUFFER_BIT);

									m_ShaderRenderer->SetCurrent(hdc);
									m_ShaderRenderer->DrawScene(hdc);

									glPixelStorei(GL_PACK_ALIGNMENT, 1);
									glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
									glReadPixels(0, 0, w, h, GL_BGRA, GL_UNSIGNED_BYTE, bmBits);

									// send to avi engine
									hr = AviGen.AddFrame(bmBits);
									if (FAILED(hr))
									{
										LogStr(AviGen.GetLastErrorMessage());
										break;
									}
								}

								glReadBuffer(GL_FRONT);
							}

							m_ShaderView->Play();

							// releasing engine and memory
							AviGen.ReleaseEngine();
							delete[] bmBits;

							m_ShaderView->SetScreenSize(screenSize.x, screenSize.y);
						}
					}
				}
				else if (fileExt == 1) // sequence of png
				{
					frameRate = 60.0f;

					// allocating memory
					BYTE* bmBits = new BYTE[3 * w * h];

					m_ShaderView->Pause();

					float startTime;
					stringstream st = stringstream(m_textCtrl1->GetValue().ToStdString());
					st >> startTime;
					if (st)
					{
						float endTime;
						stringstream et = stringstream(m_textCtrl11->GetValue().ToStdString());
						et >> endTime;
						if (et)
						{
							float length = endTime - startTime;
							long countFrames = length * frameRate;

							float rate = frameRate;
							for (int i = 0; i < countFrames; i++)
							{
								string count = toStr(countFrames);
								string index = toStr(i);
								while (index.size() < count.size())
									index = "0" + index;

								string file = filepath + "\\" + filename + index + "." + fileext;
									
								m_uniTime.x = startTime + i / rate;
								m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniTime, false);

								glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
								glClear(GL_COLOR_BUFFER_BIT);

								m_ShaderRenderer->SetCurrent(hdc);
								m_ShaderRenderer->DrawScene(hdc);

								glPixelStorei(GL_PACK_ALIGNMENT, 1);
								glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
								glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, bmBits);

								wxImage img(w, h, bmBits, true);
								wxImage imgMirrored = img.Mirror(false);
								imgMirrored.SaveFile(file, wxBITMAP_TYPE_PNG);
							}

							m_ShaderView->Play();

							// releasing engine and memory
							delete[] bmBits;

							m_ShaderView->SetScreenSize(screenSize.x, screenSize.y);
						}
					}
				}
				else if (fileExt == 2) // gif
				{
					frameRate = 30.0f;

					GifWriter gifStruct;
					
					// allocating memory
					BYTE* bmBits = new BYTE[4 * w * h]; // 4 car R G B A fait 4 BYTE

					m_ShaderView->Pause();

					float startTime;
					stringstream st = stringstream(m_textCtrl1->GetValue().ToStdString());
					st >> startTime;
					if (st)
					{
						float endTime;
						stringstream et = stringstream(m_textCtrl11->GetValue().ToStdString());
						et >> endTime;
						if (et)
						{
							float length = endTime - startTime;
							long countFrames = length * frameRate;

							float rate = frameRate;
							
							GifBegin(&gifStruct, filepathname.c_str(), w, h, 0);

							for (int i = 0; i < countFrames; i++)
							{
								m_uniTime.x = startTime + i / rate;
								m_UniformManager->UpdateUniformByName(m_mainUniformName, m_uniTime, false);

								glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
								glClear(GL_COLOR_BUFFER_BIT);

								m_ShaderRenderer->SetCurrent(hdc);
								m_ShaderRenderer->DrawScene(hdc);

								glPixelStorei(GL_PACK_ALIGNMENT, 1);
								glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
								glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, bmBits);

								GifWriteFrame(&gifStruct, bmBits, w, h, 0);
							}

							GifEnd(&gifStruct);
						}

						m_ShaderView->Play();

						delete[] bmBits;

						m_ShaderView->SetScreenSize(screenSize.x, screenSize.y);
					}
				}
			}
		}
		catch (std::exception &err)
		{
			string errStr = toStr(err.what());
			LogStr("UniformExport::OnPlayPauseTime exception: " + errStr);
		}
	}
}

void UniformExport::OnResetTime(wxCommandEvent& /*event*/)
{
	ResetTimer(0.0f);
	m_textCtrl1->SetLabelText(toStr(m_currentTime));
	m_textCtrl1->Refresh(false, 0);
}

void UniformExport::OnTimer(wxTimerEvent& /*event*/)
{
	m_currentTick = GetTickCount64();

	if (m_TimeDir>0)
		m_lastDeltaTick = m_currentTick - m_lastTick;
	else
		m_lastDeltaTick = (m_lastInvertTick - (m_currentTick - m_lastInvertTick)) - m_lastTick;

	m_currentTime = m_lastDeltaTick / 1000.0f * m_multCoef;

	// round à 2 chiffres
	m_textCtrl1->SetLabelText(toStr(int(m_currentTime * 100.0f) / 100.0f));

	//m_TimeDir

	m_uniTime.x = m_currentTime;

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_uniTime, m_Modified);

	//m_ShaderView->Refresh(false, 0); la sahderview s'en charge elle meme maintenant. elle a son propre timer
}

void UniformExport::PlayTimer()
{
	m_lastTick = GetTickCount64() - m_lastDeltaTick;
	m_timer.Start(20);    // 1 second interval
	m_textCtrl1->Enable(false);
}

void UniformExport::PauseTimer()
{
	m_timer.Stop();
	m_textCtrl1->Enable(true);
}

void UniformExport::ResetTimer(float vValue)
{
	m_Modified = true;
	m_lastTick = GetTickCount64();
	m_currentTick = m_lastTick;
	m_currentTime = vValue;
	m_lastDeltaTick = 0;
	m_lastInvertTick = m_currentTick;

	if (m_TimeDir < 0)
		m_lastInvertTick = m_currentTick;
	else
		m_lastInvertTick = 0;

	///////////////////////////////

	m_lastDeltaTick = m_currentTime / m_multCoef * 1000.0f;

	if (m_TimeDir>0)
		m_lastTick = m_currentTick - m_lastDeltaTick;
	else
		m_lastTick = (m_lastInvertTick - (m_currentTick - m_lastInvertTick)) - m_lastDeltaTick;

	///////////////////////////////

	m_uniTime.x = m_currentTime;

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_uniTime, m_Modified);

	if (m_ShaderView != 0)
		m_ShaderView->Refresh(false, 0);
}

void UniformExport::OnInvCheck(wxCommandEvent& /*event*/)
{
	m_Modified = true;
	/*if (m_checkBox2->Get3StateValue() == wxCheckBoxState::wxCHK_CHECKED)
	{
		m_TimeDir = -1;
		m_lastInvertTick = m_currentTick;
	}
	else
	{
		m_TimeDir = 1;
		m_lastTick += (m_currentTick - m_lastInvertTick)*2;
	}*/
	
}

void UniformExport::OnMultCoefChange(wxCommandEvent& event)
{
	m_Modified = true;
	wxTextCtrl *txtCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
	if (txtCtrl != 0)
	{
		float t;
		stringstream ss = stringstream(txtCtrl->GetValue().ToStdString());
		ss >> t;
		if (ss)
		{
			if (floatIsValid(t) == true)
			{
				wxColour col = txtCtrl->GetBackgroundColour();
				if (col != wxColour("WHITE")) // avoid flicker of continouous refresh
				{
					txtCtrl->SetBackgroundColour(wxColour("WHITE"));
					txtCtrl->Refresh(false, 0);
				}
				m_multCoef = t;
			}
		}
		else // ss cant be conerted to float
		{
			txtCtrl->SetBackgroundColour(wxColour("ORANGE"));
			txtCtrl->Refresh(false, 0);
		}
	}
}

string UniformExport::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	/*
	// les données interessantes sont :
	// le coef de demultiplication
	// l'état du player : si le timer joue ou est arretée
	// si le temps est inversé ou pas
	
	string time = "STARTTIME=\"" + toStr(m_currentTime) + "\"";
	string invCheck = "INV=\"" + toStr(m_checkBox2->IsChecked()) + "\"";
	string coef = "COEF=\"" + m_textCtrl3->GetValue().ToStdString() + "\"";
	string playState = "PLAYED=\"" + toStr(m_bpButton1->GetNormalState() == wxBitmapToggleButton::State::State_Pressed) + "\"";

	xml = time + " " + playState + " " + invCheck + " " + coef;
	*/
	m_Modified = false;

	return xml;
}

void UniformExport::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformExport::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	return code;
}
