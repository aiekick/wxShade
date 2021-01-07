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

#include "UniformCubeMap.h"

#include "UniformManager.h"

#include "DocShaderArt.h"

///////////////////////////////////////////////////////////////////////////
#include "../icons/PictureAdd_icon.xpm"
#include "../icons/Delete_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformCubeMap, UniformAbstract)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_DIR_BTN, UniformCubeMap::OnLoadCubeMapBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_1_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_2_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_3_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_4_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_5_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_6_BTN, UniformCubeMap::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_TEXCUBE_WIDGET_TRASH_BTN, UniformCubeMap::OnResetTexBtn)
wxEND_EVENT_TABLE()

UniformCubeMap::UniformCubeMap(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager)
{
	m_btnSize = wxSize(100, 100);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxHORIZONTAL);

	m_bpBtnTrash = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_TRASH_BTN, Delete_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);

	bSizer2->Add(m_bpBtnTrash, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	
	m_Button = new wxButton(this, ID_UNIFORM_TEXCUBE_WIDGET_DIR_BTN, wxT("..."), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_Button->SetMaxSize(wxSize(30, 30));

	bSizer2->Add(m_Button, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	int idx = 0;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	
	idx = 1;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	idx = 2;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	idx = 3;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	idx = 4;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	idx = 5;
	m_bpButton[idx] = new wxBitmapButton(this, ID_UNIFORM_TEXCUBE_WIDGET_1_BTN + idx, PictureAdd_icon, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW);
	m_bpButton[idx]->SetMinSize(m_btnSize);
	m_bpButton[idx]->SetMaxSize(m_btnSize);
	m_bpButton[idx]->SetClientData(&idx);
	m_bpButton[idx]->Enable(false);

	bSizer2->Add(m_bpButton[idx], 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	
	wxInitAllImageHandlers(); // pour pouvoir charger des fichiers avec wxImage

	this->SetSizer(bSizer2);
	this->Layout();
}

UniformCubeMap::~UniformCubeMap()
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	GLuint texId = (GLuint)m_CubeMap.uSamplerCube;
	glDeleteTextures(1, &texId);
}

string UniformCubeMap::Init(string vCodeSelection)
{
	string res;

	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	if (m_Params.size() > 0)
	{
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//string file1 = "FILE1=\"" + m_FilePathName[0] + "\"";
			//string file2 = "FILE2=\"" + m_FilePathName[1] + "\"";
			//string file3 = "FILE3=\"" + m_FilePathName[2] + "\"";
			//string file4 = "FILE4=\"" + m_FilePathName[3] + "\"";
			//string file5 = "FILE5=\"" + m_FilePathName[4] + "\"";
			//string file6 = "FILE6=\"" + m_FilePathName[5] + "\"";

			if (attrib == "FILE1") m_FilePathName[0] = value.getS();
			if (attrib == "FILE2") m_FilePathName[1] = value.getS();
			if (attrib == "FILE3") m_FilePathName[2] = value.getS();
			if (attrib == "FILE4") m_FilePathName[3] = value.getS();
			if (attrib == "FILE5") m_FilePathName[4] = value.getS();
			if (attrib == "FILE6") m_FilePathName[5] = value.getS();
		}

		GLuint CubeMapId = CreateCubeMap();

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
					for (int i = 0; i < 6; i++)
					{
						wxFileName file = wxFileName(m_FilePathName[i]);
						if (file.Exists() == false)
						{
							file.SetPath(SettingsDlg::AssetsDirPath);
						}
						if (file.IsRelative() == true) // si le fichier est un chemin relatif 
						{
							m_FilePathName[i] = file.GetFullPath(); // on recree le chemin absolu
						}
					}

					if (LoadCubeMapFromFilePathName(m_FilePathName, CubeMapId) == false)
					{
						for (int i = 0; i<6; i++)
							m_FilePathName[i] = "";

						glDeleteTextures(1, &CubeMapId); //on detruit le cubemap precedemment créé
					}
				}
			}
		}
	}
	else
	{

	}

	return res;
}

void UniformCubeMap::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformCubeMap::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
}

GLuint UniformCubeMap::CreateCubeMap()
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	GLuint CubeMapId;
	glGenTextures(1, &CubeMapId);

	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return CubeMapId;
}

void UniformCubeMap::OnLoadCubeMapBtn(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	wxFileDialog fileDlg(this, _("Open"), SettingsDlg::AssetsDirPath, "", "All Pictures files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxFileName fpn = fileDlg.GetPath();
		if (wxFileExists(fpn.GetFullPath()) == false)
			return;

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		GLuint texId = (GLuint)m_CubeMap.uSamplerCube;
		glDeleteTextures(1, &texId);
		m_CubeMap.uSamplerCube = -1;

		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_CubeMap, true);

		string file = fpn.GetName();
		string name = file.substr(0, file.length() - 1);

		GLuint CubeMapId = CreateCubeMap();

		for (int i = 0; i < 6; i++)
		{
			// la verif de l'existence du fichier est faite dans LoadCubeMapFromFilePathName
			m_FilePathName[i] = fpn.GetPath() + "\\" + name + toStr(i) + "." + fpn.GetExt(); 
		}

		if ( LoadCubeMapFromFilePathName(m_FilePathName, CubeMapId) == false)
		{
			for (int i = 0; i<6; i++)
				m_FilePathName[i] = "";

			glDeleteTextures(1, &CubeMapId); //on detruit le cubemap precedemment créé
		}
	}
}

void UniformCubeMap::OnChangeTexBtn(wxCommandEvent& event) // on change de tex apres appui sur le bouton d'une image
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	wxFileDialog fileDlg(this, _("Open"), SettingsDlg::AssetsDirPath, "", "All Pictures files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		wxBitmapButton *btn = dynamic_cast<wxBitmapButton*>(event.GetEventObject());
		if (btn != 0)
		{
			int idx = 0;
			for (int i = 0; i < 6; i++)
			{
				if (m_bpButton[i] == btn)
				{
					idx = i;
					break;
				}
			}

			m_FilePathName[idx] = fileDlg.GetPath().ToStdString();
			wxFileName fpn = m_FilePathName[idx];
			if (wxFileExists(fpn.GetFullPath()) == false)
				return;

			bool res = CreateTexture(m_FilePathName[idx], m_bpButton[idx], m_CubeMap.uSamplerCube, idx);

			if (res == true)
			{
				// Update Uniform
				if (m_UniformManager != 0)
					m_UniformManager->UpdateUniformByName(m_mainUniformName, m_CubeMap, true);

				m_ShaderView->Refresh(false, 0);
			}
			else
			{
				m_FilePathName[idx] = "";
			}
		}
	}
}

void UniformCubeMap::OnResetTexBtn(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	GLuint texId = (GLuint)m_CubeMap.uSamplerCube;
	glDeleteTextures(1, &texId);
	m_CubeMap.uSamplerCube = -1;
	for (int i = 0; i < 6; i++)
	{
		m_bpButton[i]->SetBitmap(PictureAdd_icon);
		m_FilePathName[0] = "";
	}

	UpdateUniformByName(m_mainUniformName, m_CubeMap, true);
}

bool UniformCubeMap::LoadCubeMapFromFilePathName(string /*vFilePathName*/[6], GLuint vCubeMapId)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	bool res = true;

	for (int i = 0; i < 6; i++)
	{
		res = CreateTexture(m_FilePathName[i], m_bpButton[i], vCubeMapId, i);
		if (res == false) // si une texture a echoué, aps besoin de continuer avec les autres, un cubemap avec une tex en moins de nous serre a rien
			break;
	}

	if (res == true)
	{
		// Update Uniform
		m_CubeMap.uSamplerCube = vCubeMapId;
		m_CubeMap.id = m_typeId;
		if (m_UniformManager != 0)
			m_UniformManager->UpdateUniformByName(m_mainUniformName, m_CubeMap, true);

		m_ShaderView->Refresh(false, 0);
	}

	return res;
}


bool UniformCubeMap::CreateTexture(wxString vFilePathName, wxBitmapButton* vBtn, GLuint vCubeMapId, int id)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	bool res = true;
	wxImage img;
	img.LoadFile(vFilePathName);
	if (img.IsOk())
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, vCubeMapId);

		int imgW, imgH, texW, texH;
		if (loadImage(vFilePathName, &imgW, &imgH, &texW, &texH, (GLuint)id, false) == false)
		{
			vBtn->Enable(false);
			LogStr("CubeMap Creation Failed !");
			res = false;
		}
		else
		{
			m_CubeMapSize.x = texW;
			m_CubeMapSize.y = texH;

			// si le uniform n'existe pas on va l'ajouter
			if (m_CubeMapSizeName == "")
			{
				if (m_UniformViewItem != 0)
				{
					// on propose un nom
					m_CubeMapSizeName = m_mainUniformName + "Size";
					// le nom retourné est celui qui a pu etre enregistré dans le program. 
					// il est peut etre modifié par rapport a cequi était prévu
					m_CubeMapSizeName = m_UniformViewItem->AddUniform(m_CubeMapSizeName, UniformsTypeEnum::UVEC2, false);
				}
			}

			// on maj le uniform q'il existe
			if (m_CubeMapSizeName != "")
			{
				m_UniformManager->UpdateUniformByName(m_CubeMapSizeName, m_CubeMapSize, false);
			}

			if (vBtn != 0)
			{
				wxSize size = m_btnSize;
				img = img.Scale(size.x - 10, size.y - 10, wxImageResizeQuality::wxIMAGE_QUALITY_BICUBIC);
				vBtn->Enable(true);
				vBtn->SetBitmap(img);
				vBtn->Refresh();
			}
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	return res;
}

bool UniformCubeMap::loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, GLuint vID, bool vAutoPot)
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

	wxImage img = wxImage(path);

	wxImage flipyImg = img.Mirror(false);

	(*imageWidth) = flipyImg.GetWidth();
	(*imageHeight) = flipyImg.GetHeight();

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
		GLubyte *bitmapData = flipyImg.GetData();
		GLubyte *alphaData = flipyImg.GetAlpha();

		int bytesPerPixel = flipyImg.HasAlpha() ? 4 : 3;

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
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + vID,
			0,
			bytesPerPixel,
			*imageWidth,
			*imageHeight,
			0,
			flipyImg.HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		(*textureWidth) = (*imageWidth);
		(*textureHeight) = (*imageHeight);

		delete[] imageData;
	}
	else // texture is not a power of two. We need to resize it
	{

		int newWidth = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_width)));
		int newHeight = (int)std::pow(2.0, (int)(std::ceil(power_of_two_that_gives_correct_height)));

		//printf("Unsupported image size. Recommand values: %i %i\n",newWidth,newHeight);   

		GLubyte	*bitmapData = flipyImg.GetData();
		GLubyte *alphaData = flipyImg.GetAlpha();

		int old_bytesPerPixel = 3;
		int bytesPerPixel = flipyImg.HasAlpha() ? 4 : 3;

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


		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + vID,
			0,
			flipyImg.HasAlpha() ? 4 : 3,
			newWidth,
			newHeight,
			0,
			flipyImg.HasAlpha() ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData);

		(*textureWidth) = newWidth;
		(*textureHeight) = newHeight;

		delete[] imageData;
	}

	return true;
}

string UniformCubeMap::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

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
				for (int i = 0; i < 6; i++)
				{
					wxFileName file = wxFileName(m_FilePathName[i]);
					if (file.Exists() == false)
					{
						file.SetPath(SettingsDlg::AssetsDirPath);
					}
					if (file.IsRelative() == true) // si le fichier est un chemin relatif 
					{
						m_FilePathName[i] = file.GetFullPath(); // on recree le chemin absolu
						file = wxFileName(m_FilePathName[i]);
					}

					string fullpath = file.GetFullPath();
					if (wxFileExists(fullpath) == true)
					{
						if (file.MakeRelativeTo(fpn.GetPath()) == true)
						{
							m_FilePathName[i] = file.GetFullPath();
						}
					}
					else
					{
						if (fullpath != "")
						{
							string msg = "The picture " + toStr(i) + " not exist !\nThe save continue, but in the futur, the load of this file will do an error on this picture !";
							wxMessageBox(msg, "ATTENTION !!", wxOK, this);
						}
						// on continue la sauvegarde quand meme
					}
				}
			}
		}
	}

	// les données interessantes sont :
	// le chemin des 6 fichiers

	string file1 = "FILE1=\"" + m_FilePathName[0] + "\"";
	string file2 = "FILE2=\"" + m_FilePathName[1] + "\"";
	string file3 = "FILE3=\"" + m_FilePathName[2] + "\"";
	string file4 = "FILE4=\"" + m_FilePathName[3] + "\"";
	string file5 = "FILE5=\"" + m_FilePathName[4] + "\"";
	string file6 = "FILE6=\"" + m_FilePathName[5] + "\"";

	xml = file1 + " " + file2 + " " + file3 + " " + file4 + " " + file5 + " " + file6;

	return xml;
}

void UniformCubeMap::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformCubeMap::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	return code;
}

void UniformCubeMap::ChangeUniformName(string vCurrentName, string vNewName)
{
	if (m_mainUniformName == vCurrentName)
		m_mainUniformName = vNewName;
	if (m_CubeMapSizeName == vCurrentName)
		m_CubeMapSizeName = vNewName;
}