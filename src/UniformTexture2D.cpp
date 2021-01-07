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

#include "UniformTexture2D.h"

#include "UniformManager.h"

#include "DocShaderArt.h"

#include <wx/wfstream.h>
#include <wx/base64.h> 
#include <wx/buffer.h>
#include <wx/mstream.h>

///////////////////////////////////////////////////////////////////////////
#include "../icons/PictureAdd_icon.xpm"
#include "../icons/Delete_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformTexture2D, UniformAbstract)
EVT_BUTTON(ID_UNIFORM_TEX2D_WIDGET_DIR_BTN, UniformTexture2D::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEX2D_WIDGET_TRASH_BTN, UniformTexture2D::OnResetTexBtn)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_MAG, UniformTexture2D::OnChoiceMag)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_MIN, UniformTexture2D::OnChoiceMin)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_WRAPS, UniformTexture2D::OnChoiceWrapS)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_WRAPT, UniformTexture2D::OnChoiceWrapT)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_GENERATE_MIPMAP, UniformTexture2D::OnChoiceGenerateMipMap)
EVT_SPINCTRL(ID_UNIFORM_TEX2D_CHOICE_MIPMAP_BASE_LEVEL, UniformTexture2D::OnSpinMipMapBaseLevel)
EVT_SPINCTRL(ID_UNIFORM_TEX2D_CHOICE_MIPMAP_MAX_LEVEL, UniformTexture2D::OnSpinMipMapMaxLevel)
EVT_CHOICE(ID_UNIFORM_TEX2D_CHOICE_TEXTURE_INVERT_Y, UniformTexture2D::OnChoiceTexInvertY)
wxEND_EVENT_TABLE()

UniformTexture2D::UniformTexture2D(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), m_BaseLevel(0), m_MaxLevel(4)
{
	m_btnSize = wxSize(100, 100);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_bpBtnTrash = new wxBitmapButton(this, ID_UNIFORM_TEX2D_WIDGET_TRASH_BTN, Delete_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);

	//m_bpBtnTrash->SetMinSize(m_btnSize);
	//m_bpBtnTrash->SetMaxSize(m_btnSize);

	bSizer2->Add(m_bpBtnTrash, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	m_bpButton1 = new wxBitmapButton(this, ID_UNIFORM_TEX2D_WIDGET_DIR_BTN, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);

	m_bpButton1->SetMinSize(m_btnSize);
	m_bpButton1->SetMaxSize(m_btnSize);

	bSizer2->Add(m_bpButton1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

	//////////////////////////////////////////////////////////////////////////////////
	wxBoxSizer* bSizerParamaters;
	bSizerParamaters = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizerMag;
	bSizerMag = new wxBoxSizer(wxHORIZONTAL);

	m_staticTextMag = new wxStaticText(this, wxID_ANY, wxT("GL_TEXTURE_MAG_FILTER"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextMag->Wrap(-1);
	bSizerMag->Add(m_staticTextMag, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	wxString m_ChoiceMagChoices[] = 
	{ 
		wxT("GL_NEAREST"),
		wxT("GL_LINEAR")
	};
	int m_ChoiceMagNChoices = sizeof(m_ChoiceMagChoices) / sizeof(wxString);
	m_ChoiceMag = new wxChoice(this, ID_UNIFORM_TEX2D_CHOICE_MAG, wxDefaultPosition, wxDefaultSize, m_ChoiceMagNChoices, m_ChoiceMagChoices, 0);
	m_ChoiceMag->SetSelection(1);
	m_Mag = UniformTexture2DMAGEnum::magLINEAR;
	bSizerMag->Add(m_ChoiceMag, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 0);

	bSizerParamaters->Add(bSizerMag, 1, wxEXPAND, 0);

	wxBoxSizer* bSizerMin;
	bSizerMin = new wxBoxSizer(wxHORIZONTAL);

	m_staticTextMin = new wxStaticText(this, wxID_ANY, wxT("GL_TEXTURE_MIN_FILTER"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextMin->Wrap(-1);
	bSizerMin->Add(m_staticTextMin, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	wxString m_ChoiceMinChoices[] =
	{ 
		wxT("GL_NEAREST"), 
		wxT("GL_NEAREST_MIPMAP_NEAREST"),
		wxT("GL_LINEAR_MIPMAP_NEAREST"),
		wxT("GL_NEAREST_MIPMAP_LINEAR"),
		wxT("GL_LINEAR_MIPMAP_LINEAR")
	};
	int m_ChoiceMinNChoices = sizeof(m_ChoiceMinChoices) / sizeof(wxString);
	m_ChoiceMin = new wxChoice(this, ID_UNIFORM_TEX2D_CHOICE_MIN, wxDefaultPosition, wxDefaultSize, m_ChoiceMinNChoices, m_ChoiceMinChoices, 0);
	m_ChoiceMin->SetSelection(4);
	m_Min = UniformTexture2DMinEnum::minLINEAR_MIPMAP_LINEAR;
	bSizerMin->Add(m_ChoiceMin, 0, wxALL, 0);

	bSizerParamaters->Add(bSizerMin, 1, wxEXPAND, 0);

	wxBoxSizer* bSizerWrapS;
	bSizerWrapS = new wxBoxSizer(wxHORIZONTAL);

	m_staticTextWrapS = new wxStaticText(this, wxID_ANY, wxT("GL_TEXTURE_WRAP_S"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextWrapS->Wrap(-1);
	bSizerWrapS->Add(m_staticTextWrapS, 1, wxALIGN_CENTER_VERTICAL, 0);

	wxString m_ChoiceWrapSChoices[] =
	{
		wxT("GL_REPEAT"),
		wxT("GL_CLAMP_TO_EDGE"),
		wxT("GL_MIRRORED_REPEAT")
	};
	int m_ChoiceWrapSNChoices = sizeof(m_ChoiceWrapSChoices) / sizeof(wxString);
	m_ChoiceWrapS = new wxChoice(this, ID_UNIFORM_TEX2D_CHOICE_WRAPS, wxDefaultPosition, wxDefaultSize, m_ChoiceWrapSNChoices, m_ChoiceWrapSChoices, 0);
	m_ChoiceWrapS->SetSelection(0);
	m_WrapS = UniformTexture2DWrapSEnum::wrapsREPEAT;
	bSizerWrapS->Add(m_ChoiceWrapS, 0, wxALL, 0);


	bSizerParamaters->Add(bSizerWrapS, 1, wxEXPAND, 0);

	wxBoxSizer* bSizerWrapT;
	bSizerWrapT = new wxBoxSizer(wxHORIZONTAL);

	m_staticTextWrapT = new wxStaticText(this, wxID_ANY, wxT("GL_TEXTURE_WRAP_T"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticTextWrapT->Wrap(-1);
	bSizerWrapT->Add(m_staticTextWrapT, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	wxString m_ChoiceWrapTChoices[] =
	{
		wxT("GL_REPEAT"),
		wxT("GL_CLAMP_TO_EDGE"),
		wxT("GL_MIRRORED_REPEAT")
	};
	int m_ChoiceWrapTNChoices = sizeof(m_ChoiceWrapTChoices) / sizeof(wxString);
	m_ChoiceWrapT = new wxChoice(this, ID_UNIFORM_TEX2D_CHOICE_WRAPT, wxDefaultPosition, wxDefaultSize, m_ChoiceWrapTNChoices, m_ChoiceWrapTChoices, 0);
	m_ChoiceWrapT->SetSelection(0);
	m_WrapT = UniformTexture2DWrapTEnum::wraptREPEAT;
	bSizerWrapT->Add(m_ChoiceWrapT, 0, wxALL, 0);

	bSizerParamaters->Add(bSizerWrapT, 1, wxEXPAND, 0);

	//////////////////////////////////////////////////////////////////////////////////

	bSizer2->Add(bSizerParamaters, 0, wxALIGN_CENTER_VERTICAL, 0);

	//////////////////////////////////////////////////////////////////////////////////
	wxBoxSizer* bSizerParamaters2;
	bSizerParamaters2 = new wxBoxSizer(wxVERTICAL);

	/*wxBoxSizer* bSizerGenerateMipMap;
	bSizerGenerateMipMap = new wxBoxSizer(wxHORIZONTAL);

	m_staticGenerateMipMap = new wxStaticText(this, wxID_ANY, wxT("GL_GENERATE_MIPMAP"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticGenerateMipMap->Wrap(-1);
	bSizerGenerateMipMap->Add(m_staticGenerateMipMap, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	wxString m_ChoiceGenerateMipMapChoices[] =
	{
		wxT("NO"),
		wxT("YES")
	};
	int m_ChoiceGenerateMipMapNChoices = sizeof(m_ChoiceGenerateMipMapChoices) / sizeof(wxString);
	m_ChoiceGenerateMipMap = new wxChoice(this, wxID_ANY/*ID_UNIFORM_TEX2D_CHOICE_GENERATE_MIPMAP*//*, wxDefaultPosition, wxDefaultSize, m_ChoiceGenerateMipMapNChoices, m_ChoiceGenerateMipMapChoices, 0);
	/*m_ChoiceGenerateMipMap->SetSelection(0);
	bSizerGenerateMipMap->Add(m_ChoiceGenerateMipMap, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 0);

	bSizerParamaters2->Add(bSizerGenerateMipMap, 1, wxEXPAND, 0);*/

	wxBoxSizer* bSizerBaselevel;
	bSizerBaselevel = new wxBoxSizer(wxHORIZONTAL);

	m_staticBaselevel = new wxStaticText(this, wxID_ANY, wxT("MIPMAP_BASE_LEVEL"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticBaselevel->Wrap(-1);
	bSizerBaselevel->Add(m_staticBaselevel, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	m_SpinBaselevel = new wxSpinCtrl(this, ID_UNIFORM_TEX2D_CHOICE_MIPMAP_BASE_LEVEL, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);
	m_SpinBaselevel->SetMax(1000);
	m_SpinBaselevel->SetMin(0);
	m_SpinBaselevel->SetValue(m_BaseLevel);
	m_SpinBaselevel->SetSize(wxSize(50, -1));
	bSizerBaselevel->Add(m_SpinBaselevel, 0, wxALL, 0);

	bSizerParamaters2->Add(bSizerBaselevel, 1, wxEXPAND, 0);

	wxBoxSizer* bSizerMaxLevel;
	bSizerMaxLevel = new wxBoxSizer(wxHORIZONTAL);

	m_staticMaxLevel = new wxStaticText(this, wxID_ANY, wxT("MIPMAP_MAX_LEVEL"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticMaxLevel->Wrap(-1);
	bSizerMaxLevel->Add(m_staticMaxLevel, 1, wxALIGN_CENTER_VERTICAL, 0);

	m_SpinMaxLevel = new wxSpinCtrl(this, ID_UNIFORM_TEX2D_CHOICE_MIPMAP_MAX_LEVEL, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS);
	m_SpinMaxLevel->SetMax(1000);
	m_SpinMaxLevel->SetMin(0);
	m_SpinMaxLevel->SetValue(m_MaxLevel);
	m_SpinMaxLevel->SetSize(wxSize(50, -1));
	bSizerMaxLevel->Add(m_SpinMaxLevel, 0, wxALL, 0);

	bSizerParamaters2->Add(bSizerMaxLevel, 1, wxEXPAND, 0);

	/*wxBoxSizer* bSizerInvertY;
	bSizerInvertY = new wxBoxSizer(wxHORIZONTAL);

	m_staticInvertY = new wxStaticText(this, wxID_ANY, wxT("GL_TEXTURE_INVERT_Y"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticInvertY->Wrap(-1);
	bSizerInvertY->Add(m_staticInvertY, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 0);

	wxString m_ChoiceInvertYChoices[] =
	{
		wxT("NO"),
		wxT("YES")
	};
	int m_ChoiceInvertYNChoices = sizeof(m_ChoiceInvertYChoices) / sizeof(wxString);
	m_ChoiceInvertY = new wxChoice(this, ID_UNIFORM_TEX2D_CHOICE_TEXTURE_INVERT_Y, wxDefaultPosition, wxDefaultSize, m_ChoiceInvertYNChoices, m_ChoiceInvertYChoices, 0);
	m_ChoiceInvertY->SetSelection(0);
	bSizerInvertY->Add(m_ChoiceInvertY, 0, wxALL, 0);

	bSizerParamaters2->Add(bSizerInvertY, 1, wxEXPAND, 0);*/

	//////////////////////////////////////////////////////////////////////////////////

	bSizer2->Add(bSizerParamaters2, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxInitAllImageHandlers(); // pour pouvoir charger des fichiers avec wxImage

	this->SetSizer(bSizer2);
	this->Layout();

	GLuint texId;
	glGenTextures(1, &texId);

	m_Tex2D.uSampler2D = texId;
}

UniformTexture2D::~UniformTexture2D()
{
	wxCommandEvent e;
	OnResetTexBtn(e);
}

string UniformTexture2D::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string pict = "PICTURE=\"" + m_FilePathName + "\"";

			if (attrib == "PICTURE") 
				m_FilePathName = value.getS();

			if (attrib == "MAG")
			{
				string param = value.getS();
				if (param == "NEAREST") 
				{
					m_Mag = UniformTexture2DMAGEnum::magNEAREST; 
					m_ChoiceMag->SetSelection(0);
				}
				else if (param == "LINEAR")
				{
					m_Mag = UniformTexture2DMAGEnum::magLINEAR;
					m_ChoiceMag->SetSelection(1);
				}
			}
			if (attrib == "MIN")
			{
				string param = value.getS();
				if (param == "NEAREST") 
				{
					m_Min = UniformTexture2DMinEnum::minNEAREST; 
					m_ChoiceMin->SetSelection(0);
				}
				else if (param == "NEAREST_MIPMAP_NEAREST")
				{
					m_Min = UniformTexture2DMinEnum::minNEAREST_MIPMAP_NEAREST;
					m_ChoiceMin->SetSelection(1);
				}
				else if (param == "LINEAR_MIPMAP_NEAREST")
				{
					m_Min = UniformTexture2DMinEnum::minLINEAR_MIPMAP_NEAREST;
					m_ChoiceMin->SetSelection(2);
				}
				else if (param == "NEAREST_MIPMAP_LINEAR")
				{
					m_Min = UniformTexture2DMinEnum::minNEAREST_MIPMAP_LINEAR;
					m_ChoiceMin->SetSelection(3);
				}
				else if (param == "LINEAR_MIPMAP_LINEAR")
				{
					m_Min = UniformTexture2DMinEnum::minLINEAR_MIPMAP_LINEAR;
					m_ChoiceMin->SetSelection(4);
				}
			}
			if (attrib == "WRAPS")
			{
				string param = value.getS();
				if (param == "REPEAT")
				{
					m_WrapS = UniformTexture2DWrapSEnum::wrapsREPEAT;
					m_ChoiceWrapS->SetSelection(0);
				}
				else if (param == "CLAMP_TO_EDGE")
				{
					m_WrapS = UniformTexture2DWrapSEnum::wrapsCLAMP_TO_EDGE;
					m_ChoiceWrapS->SetSelection(1);
				}
				else if (param == "MIRRORED_REPEAT")
				{
					m_WrapS = UniformTexture2DWrapSEnum::wrapsMIRRORED_REPEAT;
					m_ChoiceWrapS->SetSelection(2);
				}
			}
			if (attrib == "WRAPT")
			{
				string param = value.getS();
				if (param == "REPEAT")
				{
					m_WrapT = UniformTexture2DWrapTEnum::wraptREPEAT;
					m_ChoiceWrapT->SetSelection(0);
				}
				else if (param == "CLAMP_TO_EDGE")
				{
					m_WrapT = UniformTexture2DWrapTEnum::wraptCLAMP_TO_EDGE;
					m_ChoiceWrapT->SetSelection(1);
				}
				else if (param == "MIRRORED_REPEAT")
				{
					m_WrapT = UniformTexture2DWrapTEnum::wraptMIRRORED_REPEAT;
					m_ChoiceWrapT->SetSelection(2);
				}
			}
			/*if (attrib == "DOMM")
			{
				string param = value.getS();
				if (param == "YES")
				{
					m_GenerateMipmap = true;
					m_ChoiceGenerateMipMap->SetSelection(1);
				}
				else if (param == "NO")
				{
					m_GenerateMipmap = false;
					m_ChoiceGenerateMipMap->SetSelection(0);
				}
			}*/
			if (attrib == "MMBASELVL")
			{
				m_BaseLevel = value.getI();
				m_SpinBaselevel->SetValue(m_BaseLevel);
			}
			if (attrib == "MMMAXLVL")
			{
				m_MaxLevel = value.getI();
				m_SpinMaxLevel->SetValue(m_MaxLevel);
			}
			if (attrib == "EMBED")
			{
				m_Embed = value.getS();
			}
			/*if (attrib == "TEXINVERTY")
			{
				string param = value.getS();
				if (param == "YES")
				{
					m_InvertY = true;
					m_ChoiceInvertY->SetSelection(1);
				}
				else if (param == "NO")
				{
					m_InvertY = false;
					m_ChoiceInvertY->SetSelection(0);
				}
			}*/
		}

		// si les fichier son en format relatif on va recreer les chemin absolu
		assert(m_UniformManager != 0);
		if (m_UniformManager != 0)
		{
			DocShaderArt *doc = dynamic_cast<DocShaderArt*>(m_UniformManager->GetParent());
			if (doc != 0)
			{
				string docFilePathName = doc->GetDocument()->GetFilePathName();
				if (docFilePathName != "")
				{
					wxFileName fpn = wxFileName(docFilePathName);
					wxFileName file = wxFileName(m_FilePathName);
					if (file.Exists() == false)
					{
						file.SetPath(SettingsDlg::AssetsDirPath);
					}
					if (file.IsRelative() == true) // si le fichier est un chemin relatif 
					{
						m_FilePathName = file.GetFullPath(); // on recree le chemin absolu
					}

					if (m_Embed.size() > 0)
					{
						if (LoadPictureFromFilePathName(m_Embed, true) == false)
						{
							//m_FilePathName = "";
						}
					}
					else
					{
						if (LoadPictureFromFilePathName(m_FilePathName, false) == false)
						{
							m_FilePathName = "";
						}
					}
				}
			}
		}
	}
	else
	{
		m_BaseLevel = 0;
		m_MaxLevel = 1000;
	}

	return res;
}

void UniformTexture2D::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformTexture2D::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
}

void UniformTexture2D::OnChangeTexBtn(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	wxFileDialog fileDlg(this, _("Open"), SettingsDlg::AssetsDirPath, "", "All Pictures files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		m_FilePathName = fileDlg.GetPath().ToStdString();
		if (LoadPictureFromFilePathName(m_FilePathName, false) == false)
		{
			m_FilePathName = "";
		}
	}
}

void UniformTexture2D::OnResetTexBtn(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint texId = (GLuint)m_Tex2D.uSampler2D;
	glDeleteTextures(1, &texId);
	m_Tex2D.uSampler2D = -1;
	m_bpButton1->SetBitmap(PictureAdd_icon);
	m_FilePathName = "";
	if (m_UniformManager != 0)
	{
		m_UniformManager->UpdateUniformByName(m_mainUniformName, m_Tex2D, true);
		//m_UniformManager->UpdateUniformByName(m_Tex2DSizeName, m_Tex2DSize, false);
	}
		
}

void UniformTexture2D::OnChoiceMag(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	/*
	wxString m_ChoiceMagChoices[] = 
	{ 
		wxT("GL_NEAREST"),
		wxT("GL_LINEAR")
	};
	*/
	int sel = m_ChoiceMag->GetSelection();

	if (sel == 0) UpdateTexParamMag(UniformTexture2DMAGEnum::magNEAREST);
	else if (sel == 1) UpdateTexParamMag(UniformTexture2DMAGEnum::magLINEAR);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnChoiceMin(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	/*
	wxString m_ChoiceMinChoices[] =
	{ 
		wxT("GL_NEAREST"), 
		wxT("GL_NEAREST_MIPMAP_NEAREST"),
		wxT("GL_LINEAR_MIPMAP_NEAREST"),
		wxT("GL_NEAREST_MIPMAP_LINEAR"),
		wxT("GL_LINEAR_MIPMAP_LINEAR")
	};
	*/

	int sel = m_ChoiceMin->GetSelection();

	if (sel == 0) UpdateTexParamMin(UniformTexture2DMinEnum::minNEAREST);
	else if (sel == 1) UpdateTexParamMin(UniformTexture2DMinEnum::minNEAREST_MIPMAP_NEAREST);
	else if (sel == 2) UpdateTexParamMin(UniformTexture2DMinEnum::minLINEAR_MIPMAP_NEAREST);
	else if (sel == 3) UpdateTexParamMin(UniformTexture2DMinEnum::minNEAREST_MIPMAP_LINEAR);
	else if (sel == 4) UpdateTexParamMin(UniformTexture2DMinEnum::minLINEAR_MIPMAP_LINEAR);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnChoiceWrapS(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	/*
	wxString m_ChoiceWrapSChoices[] =
	{
		wxT("GL_REPEAT"),
		wxT("GL_CLAMP_TO_EDGE"),
		wxT("GL_MIRRORED_REPEAT")
	};
	*/

	int sel = m_ChoiceWrapS->GetSelection();

	if (sel == 0) UpdateTexParamWrapS(UniformTexture2DWrapSEnum::wrapsREPEAT);
	else if (sel == 1) UpdateTexParamWrapS(UniformTexture2DWrapSEnum::wrapsCLAMP_TO_EDGE);
	else if (sel == 2) UpdateTexParamWrapS(UniformTexture2DWrapSEnum::wrapsMIRRORED_REPEAT);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnChoiceWrapT(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	/*
	wxString m_ChoiceWrapTChoices[] =
	{
	wxT("GL_REPEAT"),
	wxT("GL_CLAMP_TO_EDGE"),
	wxT("GL_MIRRORED_REPEAT")
	};
	*/

	int sel = m_ChoiceWrapT->GetSelection();

	if (sel == 0) UpdateTexParamWrapT(UniformTexture2DWrapTEnum::wraptREPEAT);
	else if (sel == 1) UpdateTexParamWrapT(UniformTexture2DWrapTEnum::wraptCLAMP_TO_EDGE);
	else if (sel == 2) UpdateTexParamWrapT(UniformTexture2DWrapTEnum::wraptMIRRORED_REPEAT);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::UpdateTexParamMag(UniformTexture2DMAGEnum vParam)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_Mag = vParam;

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	switch (vParam)
	{
	case UniformTexture2DMAGEnum::magLINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // GL_NEAREST
		break;
	case UniformTexture2DMAGEnum::magNEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // GL_NEAREST
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UniformTexture2D::UpdateTexParamMin(UniformTexture2DMinEnum vParam)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_Min = vParam;

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	switch (vParam)
	{
	case UniformTexture2DMinEnum::minNEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case UniformTexture2DMinEnum::minNEAREST_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); 
		break;
	case UniformTexture2DMinEnum::minLINEAR_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		break;
	case UniformTexture2DMinEnum::minLINEAR_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case UniformTexture2DMinEnum::minNEAREST_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UniformTexture2D::UpdateTexParamWrapS(UniformTexture2DWrapSEnum vParam)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_WrapS = vParam;

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	switch (vParam)
	{
	case UniformTexture2DWrapSEnum::wrapsREPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	case UniformTexture2DWrapSEnum::wrapsCLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;
	case UniformTexture2DWrapSEnum::wrapsMIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UniformTexture2D::UpdateTexParamWrapT(UniformTexture2DWrapTEnum vParam)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_WrapT = vParam;

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	switch (vParam)
	{
	case UniformTexture2DWrapTEnum::wraptREPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case UniformTexture2DWrapTEnum::wraptCLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case UniformTexture2DWrapTEnum::wraptMIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void UniformTexture2D::UpdateTexParamGenerateMipMap(bool vMipMap)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	m_GenerateMipmap = vMipMap;

	//if (m_GenerateMipmap == true)
	//	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::UpdateTexParamBaseLevel(int /*vBaseLEvel*/, bool vGenerateMipmap)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	//m_BaseLevel = vBaseLEvel;

	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, &m_BaseLevel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, m_BaseLevel);

	if (vGenerateMipmap == true)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::UpdateTexParamMaxLevel(int /*vMaxLevel*/, bool vGenerateMipmap)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	//m_MaxLevel = vMaxLevel;

	//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &m_MaxLevel);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_MaxLevel);

	if (vGenerateMipmap == true)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::UpdateTexParamInvertY(bool /*vInvertY*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

bool UniformTexture2D::LoadPictureFromFilePathName(string vFilePathName, bool IsEmbedded)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	bool res = false;

	if (IsEmbedded == false)
	{
		if (wxFileExists(vFilePathName) == false)
			return false;
	}
	
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texId = (GLuint)m_Tex2D.uSampler2D;
	glDeleteTextures(1, &texId);
	m_Tex2D.uSampler2D = -1;
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	glGenTextures(1, &texId);
	m_Tex2D.uSampler2D = texId;

	wxImage img;
	if (IsEmbedded == true)
	{
		m_Embed = vFilePathName;

		wxString fragStr64 = vFilePathName;

		wxMemoryBuffer buf = wxBase64Decode(fragStr64);

		wxMemoryInputStream inStream(buf.GetData(), buf.GetDataLen());

		if (wxFileExists(m_FilePathName) == false)
		{
			string msg = "This picture not exist in your asset directory !\n Do you want to extract it in your asset directory ?";
			int answer = wxMessageBox(msg, "INFORMATION", wxYES_NO, this);
			if (answer == wxYES)
			{
				wxFileOutputStream outStream(m_FilePathName);

				outStream.WriteAll(buf.GetData(), buf.GetDataLen());

				outStream.Close();
			}
		}

		img.LoadFile(inStream);
	}
	else
	{
		m_Embed = "";
		img.LoadFile(vFilePathName);
	}
	
	if (img.IsOk())
	{
		//m_Tex2D.uSampler2D
		int imgW, imgH, texW, texH;
		if (m_ShaderRenderer != 0)
		{
			glBindTexture(GL_TEXTURE_2D, m_Tex2D.uSampler2D);

			if (IsEmbedded == true)
			{
				// mettre true pour toujours avoir un pot (power of two)
				res = loadEmbeddedImage(&img, &imgW, &imgH, &texW, &texH, false);
			}
			else
			{
				// mettre true pour toujours avoir un pot (power of two)
				res = loadImage(vFilePathName, &imgW, &imgH, &texW, &texH, false);
			}
			
			if (res == true)
			{
				// Update Uniform
				m_Tex2D.id = m_typeId;
				if (m_UniformManager != 0)
				{
					m_UniformManager->UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

					m_Tex2DSize.x = texW;
					m_Tex2DSize.y = texH;

					// si le uniform n'existe pas on va l'ajouter
					if (m_Tex2DSizeName == "")
					{
						if (m_UniformViewItem != 0)
						{
							// on propose un nom
							m_Tex2DSizeName = m_mainUniformName + "Size";
							// le nom retourné est celui qui a pu etre enregistré dans le program. 
							// il est peut etre modifié par rapport a cequi était prévu
							m_Tex2DSizeName = m_UniformViewItem->AddUniform(m_Tex2DSizeName, UniformsTypeEnum::UVEC2, false);
						}
					}
					
					// on maj le uniform q'il existe
					if (m_Tex2DSizeName != "")
					{
						m_UniformManager->UpdateUniformByName(m_Tex2DSizeName, m_Tex2DSize, false);
					}

					UpdateTexParamMag(m_Mag);
					UpdateTexParamMin(m_Min);
					UpdateTexParamWrapS(m_WrapS);
					UpdateTexParamWrapT(m_WrapT);

					// l'ordre d'application est mportant sinon ca merde
					// en 1er le max et en 2eme le base
					UpdateTexParamMaxLevel(m_MaxLevel, true);
					UpdateTexParamBaseLevel(m_BaseLevel, true);
					
					UpdateTexParamInvertY(m_InvertY);
				}

				m_ShaderView->Refresh(false, 0);

				// on maj le bouton
				wxSize size = m_btnSize;

				img = img.Scale(size.x - 10, size.y - 10, wxImageResizeQuality::wxIMAGE_QUALITY_BICUBIC);

				m_bpButton1->SetBitmap(img);

				m_bpButton1->Refresh();
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	return res;
}


bool UniformTexture2D::loadEmbeddedImage(wxImage* img, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot)
{
	if (img == 0)
		return false;
	if (img->IsOk() == false)
		return false;

	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	// the first time, init image handlers (remove this part if you do it somewhere else in your app)
	static bool is_first_time = true;
	if (is_first_time)
	{
		wxInitAllImageHandlers();

		is_first_time = false;
	}

	(*imageWidth) = img->GetWidth();
	(*imageHeight) = img->GetHeight();

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*
	* Many graphics card require that textures be power of two.
	* Below is a simple implementation, probably not optimal but working.
	* If your texture sizes are not restricted to power of 2s, you can
	* of course adapt the bit below as needed.
	*/

	float power_of_two_that_gives_correct_width = std::log((float)(*imageWidth)) / std::log(2.0);
	float power_of_two_that_gives_correct_height = std::log((float)(*imageHeight)) / std::log(2.0);

	// check if image dimensions are a power of two
	if (((int)power_of_two_that_gives_correct_width == power_of_two_that_gives_correct_width &&
		(int)power_of_two_that_gives_correct_height == power_of_two_that_gives_correct_height && vAutoPot == true) || (vAutoPot == false))
	{
		// note: must make a local copy before passing the data to OpenGL, as GetData() returns RGB 
		// and we want the Alpha channel if it's present. Additionally OpenGL seems to interpret the 
		// data upside-down so we need to compensate for that.
		GLubyte *bitmapData = img->GetData();
		GLubyte *alphaData = img->GetAlpha();

		int bytesPerPixel = img->HasAlpha() ? 4 : 3;

		int imageSize = (*imageWidth) * (*imageHeight) * bytesPerPixel;
		GLubyte *imageData = new GLubyte[imageSize];

		int rev_val = (*imageHeight) - 1;

		for (int y = 0; y<(*imageHeight); y++)
		{
			for (int x = 0; x<(*imageWidth); x++)
			{
				imageData[(x + y*(*imageWidth))*bytesPerPixel + 0] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3];

				imageData[(x + y*(*imageWidth))*bytesPerPixel + 1] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3 + 1];

				imageData[(x + y*(*imageWidth))*bytesPerPixel + 2] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3 + 2];

				if (bytesPerPixel == 4) imageData[(x + y*(*imageWidth))*bytesPerPixel + 3] =
					alphaData[x + (rev_val - y)*(*imageWidth)];
			}//next
		}//next

		// if yes, everything is fine
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			*imageWidth,
			*imageHeight,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, *imageWidth, *imageHeight, img->HasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		(*textureWidth) = (*imageWidth);
		(*textureHeight) = (*imageHeight);

		delete[] imageData;
	}
	else // texture is not a power of two. We need to resize it
	{

		int newWidth = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_width)));
		int newHeight = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_height)));

		//printf("Unsupported image size. Recommand values: %i %i\n",newWidth,newHeight);   

		GLubyte	*bitmapData = img->GetData();
		GLubyte        *alphaData = img->GetAlpha();

		int old_bytesPerPixel = 3;
		int bytesPerPixel = img->HasAlpha() ? 4 : 3;

		int imageSize = newWidth * newHeight * bytesPerPixel;
		GLubyte	*imageData = new GLubyte[imageSize];

		int rev_val = (*imageHeight) - 1;

		for (int y = 0; y<newHeight; y++)
		{
			for (int x = 0; x<newWidth; x++)
			{
				if (x<(*imageWidth) && y<(*imageHeight)){
					imageData[(x + y*newWidth)*bytesPerPixel + 0] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 0];

					imageData[(x + y*newWidth)*bytesPerPixel + 1] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 1];

					imageData[(x + y*newWidth)*bytesPerPixel + 2] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 2];

					if (bytesPerPixel == 4) imageData[(x + y*newWidth)*bytesPerPixel + 3] =
						alphaData[x + (rev_val - y)*(*imageWidth)];
				}
				else
				{

					imageData[(x + y*newWidth)*bytesPerPixel + 0] = 0;
					imageData[(x + y*newWidth)*bytesPerPixel + 1] = 0;
					imageData[(x + y*newWidth)*bytesPerPixel + 2] = 0;
					if (bytesPerPixel == 4) imageData[(x + y*newWidth)*bytesPerPixel + 3] = 0;
				}

			}//next
		}//next

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, m_BaseLevel);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_MaxLevel);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			newWidth,
			newHeight,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, newWidth, newHeight, img->HasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		(*textureWidth) = newWidth;
		(*textureHeight) = newHeight;

		delete[] imageData;
	}

	m_IsEmbedded = true;

	return true;
}

bool UniformTexture2D::loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	// the first time, init image handlers (remove this part if you do it somewhere else in your app)
	static bool is_first_time = true;
	if (is_first_time)
	{
		wxInitAllImageHandlers();

		is_first_time = false;
	}

	// check the file exists
	if (!wxFileExists(path))
	{
		if (path != "")
			wxMessageBox(_("Failed to load resource image"));
		return false;
	}

	wxImage* img = new wxImage(path);

	(*imageWidth) = img->GetWidth();
	(*imageHeight) = img->GetHeight();

	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/*
	* Many graphics card require that textures be power of two.
	* Below is a simple implementation, probably not optimal but working.
	* If your texture sizes are not restricted to power of 2s, you can
	* of course adapt the bit below as needed.
	*/

	float power_of_two_that_gives_correct_width = std::log((float)(*imageWidth)) / std::log(2.0);
	float power_of_two_that_gives_correct_height = std::log((float)(*imageHeight)) / std::log(2.0);

	// check if image dimensions are a power of two
	if (((int)power_of_two_that_gives_correct_width == power_of_two_that_gives_correct_width &&
		(int)power_of_two_that_gives_correct_height == power_of_two_that_gives_correct_height && vAutoPot == true) || (vAutoPot == false))
	{
		// note: must make a local copy before passing the data to OpenGL, as GetData() returns RGB 
		// and we want the Alpha channel if it's present. Additionally OpenGL seems to interpret the 
		// data upside-down so we need to compensate for that.
		GLubyte *bitmapData = img->GetData();
		GLubyte *alphaData = img->GetAlpha();

		int bytesPerPixel = img->HasAlpha() ? 4 : 3;

		int imageSize = (*imageWidth) * (*imageHeight) * bytesPerPixel;
		GLubyte *imageData = new GLubyte[imageSize];

		int rev_val = (*imageHeight) - 1;

		for (int y = 0; y<(*imageHeight); y++)
		{
			for (int x = 0; x<(*imageWidth); x++)
			{
				imageData[(x + y*(*imageWidth))*bytesPerPixel + 0] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3];

				imageData[(x + y*(*imageWidth))*bytesPerPixel + 1] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3 + 1];

				imageData[(x + y*(*imageWidth))*bytesPerPixel + 2] =
					bitmapData[(x + (rev_val - y)*(*imageWidth)) * 3 + 2];

				if (bytesPerPixel == 4) imageData[(x + y*(*imageWidth))*bytesPerPixel + 3] =
					alphaData[x + (rev_val - y)*(*imageWidth)];
			}//next
		}//next

		// if yes, everything is fine
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			*imageWidth,
			*imageHeight,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, *imageWidth, *imageHeight, img->HasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		(*textureWidth) = (*imageWidth);
		(*textureHeight) = (*imageHeight);

		delete[] imageData;
	}
	else // texture is not a power of two. We need to resize it
	{

		int newWidth = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_width)));
		int newHeight = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_height)));

		//printf("Unsupported image size. Recommand values: %i %i\n",newWidth,newHeight);   

		GLubyte	*bitmapData = img->GetData();
		GLubyte        *alphaData = img->GetAlpha();

		int old_bytesPerPixel = 3;
		int bytesPerPixel = img->HasAlpha() ? 4 : 3;

		int imageSize = newWidth * newHeight * bytesPerPixel;
		GLubyte	*imageData = new GLubyte[imageSize];

		int rev_val = (*imageHeight) - 1;

		for (int y = 0; y<newHeight; y++)
		{
			for (int x = 0; x<newWidth; x++)
			{
				if (x<(*imageWidth) && y<(*imageHeight)){
					imageData[(x + y*newWidth)*bytesPerPixel + 0] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 0];

					imageData[(x + y*newWidth)*bytesPerPixel + 1] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 1];

					imageData[(x + y*newWidth)*bytesPerPixel + 2] =
						bitmapData[(x + (rev_val - y)*(*imageWidth))*old_bytesPerPixel + 2];

					if (bytesPerPixel == 4) imageData[(x + y*newWidth)*bytesPerPixel + 3] =
						alphaData[x + (rev_val - y)*(*imageWidth)];
				}
				else
				{

					imageData[(x + y*newWidth)*bytesPerPixel + 0] = 0;
					imageData[(x + y*newWidth)*bytesPerPixel + 1] = 0;
					imageData[(x + y*newWidth)*bytesPerPixel + 2] = 0;
					if (bytesPerPixel == 4) imageData[(x + y*newWidth)*bytesPerPixel + 3] = 0;
				}

			}//next
		}//next

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, m_BaseLevel);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, m_MaxLevel);

		glTexImage2D(GL_TEXTURE_2D,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			newWidth,
			newHeight,
			0,
			img->HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, newWidth, newHeight, img->HasAlpha() ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

		(*textureWidth) = newWidth;
		(*textureHeight) = newHeight;

		delete[] imageData;
	}

	m_IsEmbedded = false;

	return true;
}

string UniformTexture2D::GetParamsToXMLString(DocumentFileFormat vNewFileFormat, DocumentFileFormat vOldFileFormat)
{
	string xml;

	string embed;

	if (vNewFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_ASCII && vOldFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_EMBEDDED)
	{
		if (wxFileExists(m_FilePathName) == false) // si pas existant on extrait la photo
		{
			if (m_Embed.size() > 0)
			{
				wxString fragStr64 = m_Embed;

				wxMemoryBuffer buf = wxBase64Decode(fragStr64);

				wxMemoryInputStream inStream(buf.GetData(), buf.GetDataLen());

				wxFileOutputStream outStream(m_FilePathName);

				outStream.WriteAll(buf.GetData(), buf.GetDataLen());

				outStream.Close();
			}
		}
	}
	if (vNewFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_NOTHING) // juste sauvegarde du format actuel
	{
		DocShaderArt *doc = dynamic_cast<DocShaderArt*>(m_UniformManager->GetParent());
		if (doc != 0)
		{
			vNewFileFormat = doc->GetDocument()->GetFormat();
		}
	}
	if (vNewFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_EMBEDDED)
	{
		if (m_IsEmbedded == true)
		{
			if (m_Embed.size() > 0)
			{
				embed = " EMBED=\"" + m_Embed + "\"";
			}
		}
		else
		{
			if (wxFileExists(m_FilePathName) == true)
			{
				wxFileInputStream inStream(m_FilePathName);

				// read the entire stream contents in memory

				wxWxCharBuffer cbuf;
				static const size_t chunkLen = 1024;

				wxMemoryBuffer buf(chunkLen);
				do
				{
					inStream.Read(buf.GetAppendBuf(chunkLen), chunkLen);
					buf.UngetAppendBuf(inStream.LastRead());

					const wxStreamError err = inStream.GetLastError();

					if (err != wxSTREAM_NO_ERROR && err != wxSTREAM_EOF)
					{
						wxLogError(_("Error reading config options."));
						break;
					}
				} while (!inStream.Eof());

				string base64Str = wxBase64Encode(buf);

				embed = " EMBED=\"" + base64Str + "\"";

				m_IsEmbedded = true;
			}
			else
			{
				string msg = "Problem for embed the picture file!\nAborting !";
				wxMessageBox(msg, "WARNING !!", wxOK, this);
				return "";
			}
		}
	}
	else if (vNewFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_ASCII)
	{
		// tentative pour cree des chemins relatif des chemins aboslue stockés dans m_FilePathName[6]
		assert(m_UniformManager != 0);
		if (m_UniformManager != 0)
		{
			DocShaderArt *doc = dynamic_cast<DocShaderArt*>(m_UniformManager->GetParent());
			if (doc != 0)
			{
				string docFilePathName = doc->GetDocument()->GetFilePathName();
				if (docFilePathName != "")
				{
					wxFileName fpn = wxFileName(docFilePathName);

					wxFileName file = wxFileName(m_FilePathName);
					if (file.Exists() == false)
					{
						file.SetPath(SettingsDlg::AssetsDirPath);
					}
					if (file.IsRelative() == true) // si le fichier est un chemin relatif 
					{
						m_FilePathName = file.GetFullPath(); // on recree le chemin absolu
						file = wxFileName(m_FilePathName);
					}

					string fullpath = file.GetFullPath();
					if (wxFileExists(fullpath) == true)
					{
						if (file.MakeRelativeTo(fpn.GetPath()) == true)
						{
							m_FilePathName = file.GetFullPath();
						}
					}
					else
					{
						if (fullpath != "")
						{
							string msg = "The picture not exist !\nThe save continue, but in the futur, the load of this file will do an error on this picture !";
							wxMessageBox(msg, "ATTENTION !!", wxOK, this);
						}
						// on continue la sauvegarde quand meme
					}
				}
			}
		}

		m_IsEmbedded = false;
	}
	

	// les données interessantes sont :
	// le chemin de la texture

	string pict = "PICTURE=\"" + m_FilePathName + "\"";
	
	pict += " MAG=\"";
	if (m_Mag == UniformTexture2DMAGEnum::magNEAREST) pict += "NEAREST\"";
	else if (m_Mag == UniformTexture2DMAGEnum::magLINEAR) pict += "LINEAR\"";

	pict += " MIN=\"";
	if (m_Min == UniformTexture2DMinEnum::minNEAREST) pict += "NEAREST\"";
	else if (m_Min == UniformTexture2DMinEnum::minNEAREST_MIPMAP_NEAREST) pict += "NEAREST_MIPMAP_NEAREST\"";
	else if (m_Min == UniformTexture2DMinEnum::minLINEAR_MIPMAP_NEAREST) pict += "LINEAR_MIPMAP_NEAREST\"";
	else if (m_Min == UniformTexture2DMinEnum::minNEAREST_MIPMAP_LINEAR) pict += "NEAREST_MIPMAP_LINEAR\"";
	else if (m_Min == UniformTexture2DMinEnum::minLINEAR_MIPMAP_LINEAR) pict += "LINEAR_MIPMAP_LINEAR\"";

	pict += " WRAPS=\"";
	if (m_WrapS == UniformTexture2DWrapSEnum::wrapsREPEAT) pict += "REPEAT\"";
	else if (m_WrapS == UniformTexture2DWrapSEnum::wrapsCLAMP_TO_EDGE) pict += "CLAMP_TO_EDGE\"";
	else if (m_WrapS == UniformTexture2DWrapSEnum::wrapsMIRRORED_REPEAT) pict += "MIRRORED_REPEAT\"";

	pict += " WRAPT=\"";
	if (m_WrapT == UniformTexture2DWrapTEnum::wraptREPEAT) pict += "REPEAT\"";
	else if (m_WrapT == UniformTexture2DWrapTEnum::wraptCLAMP_TO_EDGE) pict += "CLAMP_TO_EDGE\"";
	else if (m_WrapT == UniformTexture2DWrapTEnum::wraptMIRRORED_REPEAT) pict += "MIRRORED_REPEAT\"";

	string v = (m_GenerateMipmap ? "YES" : "NO");
	//pict += " DOMM=\"" + v + "\"";
	pict += " MMBASELVL=\"" + toStr(m_BaseLevel) + "\"";
	pict += " MMMAXLVL=\"" + toStr(m_MaxLevel) + "\"";
	//v = (m_InvertY ? "YES" : "NO");
	//pict += " TEXINVERTY=\"" + v + "\"";
	
	if (vNewFileFormat == DocumentFileFormat::DOC_FILE_FORMAT_EMBEDDED)
		pict += embed;

	xml = pict;

	return xml;
}

void UniformTexture2D::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformTexture2D::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	return code;
}

void UniformTexture2D::OnChoiceGenerateMipMap(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	//m_GenerateMipmap = (bool)m_ChoiceGenerateMipMap->GetSelection();

	// on reload la picture
	if (LoadPictureFromFilePathName(m_FilePathName, false) == false)
	{
		m_FilePathName = "";
	}
	
	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnSpinMipMapBaseLevel(wxSpinEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_BaseLevel = m_SpinBaselevel->GetValue();

	this->UpdateTexParamBaseLevel(m_BaseLevel, true);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnSpinMipMapMaxLevel(wxSpinEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	m_MaxLevel = m_SpinMaxLevel->GetValue();

	this->UpdateTexParamMaxLevel(m_MaxLevel, true);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::OnChoiceTexInvertY(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

//	m_InvertY = (bool)m_ChoiceInvertY->GetSelection();

	this->UpdateTexParamInvertY(m_InvertY);

	// Update Uniform
	UpdateUniformByName(m_mainUniformName, m_Tex2D, true);

	m_ShaderView->Refresh(false, 0);
}

void UniformTexture2D::ChangeUniformName(string vCurrentName, string vNewName)
{
	if (m_Tex2DSizeName == vCurrentName)
		m_Tex2DSizeName = vNewName;
	if (m_mainUniformName == vCurrentName)
		m_mainUniformName = vNewName;
}