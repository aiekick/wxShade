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

#include "UniformFont.h"

#include "UniformManager.h"

#include "DocShaderArt.h"

///////////////////////////////////////////////////////////////////////////
#include "../icons/PictureAdd_icon.xpm"
#include "../icons/Delete_icon.xpm"
///////////////////////////////////////////////////////////////////////////

#include <wx/wfstream.h>
#include <wx/base64.h> 
#include <wx/buffer.h>
#include <wx/mstream.h>

wxBEGIN_EVENT_TABLE(UniformFont, UniformAbstract)
EVT_BUTTON(ID_UNIFORM_FONT_SELECT_SYSTEM_FONT, UniformFont::OnChangeFontBtn)
EVT_COLOURPICKER_CHANGED(ID_UNIFORM_FONT_CHANGE_COLOR, UniformFont::OnChangeFontColor)
EVT_SPINCTRL(ID_UNIFORM_FONT_CHANGE_PARAMS, UniformFont::OnChangeParams)
EVT_TEXT(ID_UNIFORM_FONT_CHANGE_TEXT, UniformFont::OnChangeText)
wxEND_EVENT_TABLE()

UniformFont::UniformFont(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), m_CountRows(10), m_CountCols(10), m_ColSpace(2), m_RowSpace(2), m_ColWidth(10), m_RowWidth(10), m_FontPixelSize(100)
{
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxVERTICAL);

	m_button1 = new wxButton(this, ID_UNIFORM_FONT_SELECT_SYSTEM_FONT, wxT("Font"), wxDefaultPosition, wxDefaultSize, 0);
	m_button1->SetMinSize(wxSize(100, 32));

	bSizer11->Add(m_button1, 0, wxALL, 5);

	m_colourPicker2 = new wxColourPickerCtrl(this, ID_UNIFORM_FONT_CHANGE_COLOR, *wxBLACK, wxDefaultPosition, wxDefaultSize, wxCLRP_DEFAULT_STYLE);
	m_colourPicker2->SetMinSize(wxSize(100, 32));

	bSizer11->Add(m_colourPicker2, 0, wxALL, 5);

	bSizer1->Add(bSizer11, 0, wxALIGN_CENTER_VERTICAL, 0);

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer(wxVERTICAL);

	/*wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("Space (Col,Row) :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	m_staticText1->SetMinSize(wxSize(100, -1));

	bSizer17->Add(m_staticText1, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 0);

	m_spinCtrl2 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 2);
	m_spinCtrl2->SetMinSize(wxSize(60, -1));

	bSizer17->Add(m_spinCtrl2, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 5);

	m_spinCtrl21 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 10000, 2);
	m_spinCtrl21->SetMinSize(wxSize(60, -1));

	bSizer17->Add(m_spinCtrl21, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);

	bSizer16->Add(bSizer17, 1, wxEXPAND | wxALIGN_RIGHT, 0);*/

	/*wxBoxSizer* bSizer18;
	bSizer18 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText111 = new wxStaticText(this, wxID_ANY, wxT("Count (Col) :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText111->Wrap(-1);
	m_staticText111->SetMinSize(wxSize(100, -1));

	bSizer18->Add(m_staticText111, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);

	m_spinCtrl211 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10);
	m_spinCtrl211->SetMinSize(wxSize(60, -1));

	bSizer18->Add(m_spinCtrl211, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL | wxALL, 0);

	//m_spinCtrl212 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10);
	//m_spinCtrl212->SetMinSize(wxSize(60, -1));

	//bSizer18->Add(m_spinCtrl212, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);

	bSizer16->Add(bSizer18, 1, wxEXPAND | wxALIGN_RIGHT, 0);*/

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText1111 = new wxStaticText(this, wxID_ANY, wxT("Precision :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1111->Wrap(-1);
	m_staticText1111->SetMinSize(wxSize(100, -1));

	bSizer19->Add(m_staticText1111, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);

	m_spinCtrl2111 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10);
	m_spinCtrl2111->SetMinSize(wxSize(60, -1));

	bSizer19->Add(m_spinCtrl2111, 0, wxALL | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);

	//m_spinCtrl2121 = new wxSpinCtrl(this, ID_UNIFORM_FONT_CHANGE_PARAMS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10000, 10);
	//m_spinCtrl2121->SetMinSize(wxSize(60, -1));

	//bSizer19->Add(m_spinCtrl2121, 0, wxALIGN_CENTER_HORIZONTAL | wxALL | wxALIGN_CENTER_VERTICAL, 0);

	bSizer16->Add(bSizer19, 1, wxEXPAND, 0);

	bSizer1->Add(bSizer16, 0, wxALIGN_CENTER_VERTICAL, 0);

	m_textCtrl1 = new wxTextCtrl(this, ID_UNIFORM_FONT_CHANGE_TEXT, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", wxDefaultPosition, wxDefaultSize, wxTE_CENTRE | wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB | wxTE_WORDWRAP);
	m_textCtrl1->SetMinSize(wxSize(200, 70));

	bSizer1->Add(m_textCtrl1, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	GLuint texId;
	glGenTextures(1, &texId);

	m_Font.uSampler2D = texId;

	// the first time, init image handlers (remove this part if you do it somewhere else in your app)
	static bool is_first_time = true;
	if (is_first_time)
	{
		wxInitAllImageHandlers();

		is_first_time = false;
	}
}

UniformFont::~UniformFont()
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	glBindTexture(GL_TEXTURE_2D, 0);
	GLuint texId = (GLuint)m_Font.uSampler2D;
	glDeleteTextures(1, &texId);
	m_Font.uSampler2D = -1;
	m_FilePathName = "";
	if (m_UniformManager != 0)
	{
		m_UniformManager->UpdateUniformByName(m_mainUniformName, m_Font, true);
		//m_UniformManager->UpdateUniformByName(m_Tex2DSizeName, m_Tex2DSize, false);
	}
}

string UniformFont::Init(string vCodeSelection)
{
	string res;

	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	if (m_Params.size() > 0)
	{
		// init from param
		string spaceStr;
		string fontName;
		string colorStr;
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			//xml = "FONT=\"" + font.GetNativeFontInfoDesc().ToStdString() + "\"";
			//xml += " FONTSIZE=\"" + toStr(font.GetPixelSize().y) + "\"";
			//xml += " PRECISION=\"" + toStr(m_Precision) + "\"";
			//xml += " COLOR=\"" + toStr(fontColour.Red) + "," + toStr(fontColour.Green) + "," + toStr(fontColour.Blue) + "\"";
			//xml += "FILE=\"" + m_FilePathName + "\"";
			//xml += " TXT=\"" + txt + "\"";
			//xml += " SPACE=\"" + toStr(m_ColSpace) + "," + toStr(m_RowSpace) + "\"";
			//xml += " COLUMNS=\"" + toStr(m_CountCols) + "\"";

			if (attrib == "FONT")
				m_FontName = value.getS();

			if (attrib == "PRECISION")
				m_Precision = value.getI();

			if (attrib == "FONTSIZE")
				m_FontPixelSize = value.getI();

			if (attrib == "COLOR")
				colorStr = value.getS();

			if (attrib == "TXT")
				m_Text = value.getS();

			/*if (attrib == "SPACE")
				spaceStr = value.getS();
			
			if (attrib == "COLUMNS")
				m_CountCols = value.getI();*/
		}

		vector<string> vec;
		//int idx = 0;

		// FONTNAME FONTSIZE
		wxFont font(wxFontInfo(m_FontPixelSize).FaceName(m_FontName));
		m_FontData.SetInitialFont(font);

		// COLOR
		unsigned int v = 0.;
		std::stringstream IsItaUnsignedInt = std::stringstream(colorStr);
		IsItaUnsignedInt >> v;
		if (IsItaUnsignedInt)
		{
			m_FontColour.SetRGB(v);
		}
		m_FontData.SetColour(m_FontColour);
		m_colourPicker2->SetColour(m_FontColour);

		// TEXT
		if (m_Text.size() > 0)
		{
			wxMemoryBuffer buf = wxBase64Decode(m_Text);
			m_Text = string((char*)buf.GetData(), buf.GetDataLen());
			m_textCtrl1->SetFont(font);
			m_textCtrl1->SetLabel(m_Text);
		}

		// SPACE
		/*vec = DebugLogger::splitString(spaceStr, ';');
		idx = 0;
		for (vector<string>::iterator it = vec.begin(); it != vec.end(); ++it)
		{
			string s = *it;
			if (s != "")
			{
				float v = 0.;
				if (StringToFloat(s, &v) == true)
				{
					if (idx == 0) m_ColSpace = v;
					else if (idx == 1) m_RowSpace = v;
				}
			}
			idx++;
		}

		m_spinCtrl2->SetValue(m_ColSpace);
		m_spinCtrl21->SetValue(m_RowSpace);

		m_spinCtrl211->SetValue(m_CountCols);*/
		
		m_spinCtrl2111->SetValue(m_Precision);

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
					wxFileName file = wxFileName(m_FilePathName);
					if (file.Exists() == false)
					{
						file.SetPath(SettingsDlg::AssetsDirPath);
					}
					if (file.IsRelative() == true) // si le fichier est un chemin relatif 
					{
						m_FilePathName = file.GetFullPath(); // on recree le chemin absolu
					}
				}
			}
		}
	}
	else
	{
		m_FontName = "Courrier";
		m_FontPixelSize = 12;
		m_Precision = 100;
		m_CountCols = 10;
		m_ColSpace = 2;
		m_RowSpace = 2;
		m_Text = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789?.*+-/_'()&:,°=!\"\\"; 
		m_FontColour.SetRGB(0);

		wxFont font(wxFontInfo(m_FontPixelSize).FaceName(m_FontName));
		m_FontData.SetInitialFont(font);
		m_FontData.SetColour(m_FontColour);
		m_textCtrl1->SetFont(font);
		m_textCtrl1->SetLabel(m_Text);
		m_colourPicker2->SetColour(m_FontColour);
		//m_spinCtrl2->SetValue(m_ColSpace);
		//m_spinCtrl21->SetValue(m_RowSpace);
		//m_spinCtrl211->SetValue(m_CountCols);
		m_spinCtrl2111->SetValue(m_Precision);
	}

	UpdateFontTex();

	return res;
}

void UniformFont::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformFont::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
}

bool UniformFont::UpdateFontTex()
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	/*
		int m_CountRows;
		int m_CountCols;
		float m_ColSpace;
		float m_RowSpace;
		float m_ColWidth;
		float m_RowWidth;
	*/

	wxMemoryDC memDC;

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////

	string txt = m_textCtrl1->GetValue();

	wxFont font = m_FontData.GetInitialFont();
	font.SetPointSize(m_Precision);

	int pixelSize = 100;

	// pour la prochaine release en 3.1.0 on pourra utilise GetSelectedColour(). pour l'instant en 3.0.2 pas le choix
	wxColour fontColour = m_FontData.GetColour();

	//////////////////////////////////////////////////////////////
	// on va calculer la taille finale de l'image avec la font actuelle
	//////////////////////////////////////////////////////////////

	wxBitmap *fontTex = new wxBitmap(pixelSize * 3, pixelSize * 3, 24); // 32 bpp

	// Tell memDC to write on “fontTex”.
	memDC.SelectObject(*fontTex);

	memDC.SetFont(font);

	wxSize finalSize = memDC.GetTextExtent(txt); // size du char avec la font actuelle

	memDC.SelectObject(wxNullBitmap);

	delete fontTex;
	fontTex = 0;

	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////
	int offsetX = 50;
	int offsetY = 50;

	fontTex = new wxBitmap(finalSize.x + offsetX, finalSize.y + offsetY, 24); // 32 bpp

	// Tell memDC to write on “fontTex”.
	memDC.SelectObject(*fontTex);
	
	memDC.SetFont(font);

	//int pointSize = font.GetPointSize();

	// Clear the Device Context to all white
	memDC.SetBrush(*wxWHITE_BRUSH);
	memDC.Clear();

	//memDC.SetPen(wxPen(fontColour, pointSize, wxPenStyle::wxPENSTYLE_SOLID)); // pen used for lines
	
	//memDC.SetBrush(wxBrush(fontColour, wxBrushStyle::wxBRUSHSTYLE_SOLID)); // brush used for fill

	memDC.SetTextBackground(wxColour(0, 0, 0, 0)); // text used for coloring text
	memDC.SetTextForeground(fontColour);

	int posx = offsetX/2;
	int posy = offsetY/2;

	memDC.DrawText(txt, posx, posy);
	
	// Tell memDC to write on a fake bitmap;
	// this frees up "paper" so that it can write itself to a file.
	memDC.SelectObject(wxNullBitmap);
	
	wxImage img = fontTex->ConvertToImage();
	
	delete fontTex;

	bool res = false;

	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint texId = (GLuint)m_Font.uSampler2D;
	glDeleteTextures(1, &texId);
	m_Font.uSampler2D = -1;
	UpdateUniformByName(m_mainUniformName, m_Font, true);

	glGenTextures(1, &texId);
	m_Font.uSampler2D = texId;

	if (img.IsOk())
	{
		//m_Tex2D.uSampler2D
		int imgW, imgH, texW, texH;
		if (m_ShaderRenderer != 0)
		{
			glBindTexture(GL_TEXTURE_2D, m_Font.uSampler2D);

			// mettre true pour toujours avoir un pot (power of two)
			res = loadFontImage(&img, &imgW, &imgH, &texW, &texH, false);

			if (res == true)
			{
				// Update Uniform
				m_Font.id = m_typeId;
				if (m_UniformManager != 0)
				{
					m_UniformManager->UpdateUniformByName(m_mainUniformName, m_Font, true);

					m_FontSize.x = texW;
					m_FontSize.y = texH;

					// si le uniform n'existe pas on va l'ajouter
					if (m_FontSizeName == "")
					{
						if (m_UniformViewItem != 0)
						{
							// on propose un nom
							m_FontSizeName = m_mainUniformName + "Size";
							// le nom retourné est celui qui a pu etre enregistré dans le program. 
							// il est peut etre modifié par rapport a cequi était prévu
							m_FontSizeName = m_UniformViewItem->AddUniform(m_FontSizeName, UniformsTypeEnum::UVEC2, false);
						}
					}

					// on maj le uniform q'il existe
					if (m_FontSizeName != "")
					{
						m_UniformManager->UpdateUniformByName(m_FontSizeName, m_FontSize, false);
					}

					glGenerateMipmap(GL_TEXTURE_2D);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

					// l'ordre d'application est mportant sinon ca merde
					// en 1er le max et en 2eme le base
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 1000);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
				}

				m_ShaderView->Refresh(false, 0);
			}

			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	return res;
}


bool UniformFont::loadFontImage(wxImage* img, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot)
{
	if (img == 0)
		return false;
	if (img->IsOk() == false)
		return false;

	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

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

	//m_IsEmbedded = true;

	return true;
}

void UniformFont::OnChangeFontBtn(wxCommandEvent& /*event*/)
{
	wxFontDialog *m_FontDialog = new wxFontDialog(this, m_FontData);
	if (m_FontDialog->ShowModal() == wxID_OK)
	{
		m_FontData.SetInitialFont(m_FontDialog->GetFontData().GetChosenFont());
		m_FontData.SetColour(m_FontDialog->GetFontData().GetColour());

		m_FontName = m_FontData.GetInitialFont().GetFaceName();

		m_textCtrl1->SetFont(m_FontData.GetInitialFont());
		
		m_colourPicker2->SetColour(m_FontData.GetColour());

		UpdateFontTex();
	}
	m_FontDialog->Destroy();
	m_FontDialog = 0;
}

void UniformFont::OnChangeFontColor(wxColourPickerEvent& /*event*/)
{
	m_FontData.SetColour(m_colourPicker2->GetColour());

	UpdateFontTex();
}

void UniformFont::OnChangeParams(wxSpinEvent& event)
{
	wxSpinCtrl *ctrl = dynamic_cast<wxSpinCtrl*>(event.GetEventObject());
	if (ctrl != 0)
	{
		//if (ctrl == m_spinCtrl2) m_ColSpace = ctrl->GetValue();
		//if (ctrl == m_spinCtrl21) m_RowSpace = ctrl->GetValue();

		//if (ctrl == m_spinCtrl211) m_CountCols = ctrl->GetValue();

		if (ctrl == m_spinCtrl2111) m_Precision = ctrl->GetValue();
	}
	UpdateFontTex();
}

void UniformFont::OnChangeText(wxCommandEvent& /*event*/)
{
	UpdateFontTex();
}

string UniformFont::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
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
					if (file.MakeRelativeTo(file.GetPath()) == true)
					{
						m_FilePathName = file.GetFullPath();
					}
				}
				else
				{
					if (fullpath != "")
					{
						string msg = "The font not exist !\nThe save continue, but in the futur, the load of this file will do an error on this picture !";
						//wxMessageBox(msg, "ATTENTION !!", wxOK, this);
					}
					// on continue la sauvegarde quand meme
				}
			}
		}
	}

	string txt = m_textCtrl1->GetValue();

	// les données interessantes sont :
	// le chemin du fichier

	wxFont font = m_FontData.GetInitialFont();
	int pointSize = font.GetPointSize();

	// pour la prochaine release en 3.1.0 on pourra utilise GetSelectedColour(). pour l'instant en 3.0.2 pas le choix
	wxColour fontColour = m_FontData.GetColour();

	xml = "FONT=\"" + m_FontData.GetInitialFont().GetFaceName().ToStdString() + "\"";

	xml += " PRECISION=\"" + toStr(m_Precision) + "\"";

	xml += " FONTSIZE=\"" + toStr(pointSize) + "\"";

	xml += " COLOR=\"" + toStr(fontColour.GetRGB()) + "\"";

	string code64Str = wxBase64Encode(txt.c_str(), txt.length());
	xml += " TXT=\"" + code64Str + "\"";

	//xml += " SPACE=\"" + toStr(m_ColSpace) + ";" + toStr(m_RowSpace) + "\"";

	//xml += " COLUMNS=\"" + toStr(m_CountCols) + "\"";

	return xml;
}

void UniformFont::SetParamsFromXML(map<string, string> vParams)
{
	m_Params = vParams;
}

string UniformFont::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	return code;
}

void UniformFont::ChangeUniformName(string vCurrentName, string vNewName)
{
	if (m_mainUniformName == vCurrentName)
		m_mainUniformName = vNewName;
	if (m_FontSizeName == vCurrentName)
		m_FontSizeName = vNewName;
}