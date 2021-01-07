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

#ifndef __UNIFORMTEXTURE2D_H__
#define __UNIFORMTEXTURE2D_H__

#include "Globals.h"

#include "UniformAbstract.h"

#include <wx/wx.h>
#include <wx/spinctrl.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"

#include "SettingsDlg.h"

#include <string>
using namespace std;

enum UniformTexture2DMAGEnum
{
	magNEAREST = GL_NEAREST,
	magLINEAR = GL_LINEAR
};

enum UniformTexture2DMinEnum
{
	minNEAREST = GL_NEAREST,
	minNEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
	minLINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
	minNEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
	minLINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
};

enum UniformTexture2DWrapSEnum
{
	wrapsREPEAT = GL_REPEAT,
	wrapsCLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	wrapsMIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

enum UniformTexture2DWrapTEnum
{
	wraptREPEAT = GL_REPEAT,
	wraptCLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
	wraptMIRRORED_REPEAT = GL_MIRRORED_REPEAT
};

class UniformTexture2D : public UniformAbstract
{
public:
	UniformTexture2D(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformTexture2D();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual void UpdateValue(cUniformVariant vValue);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);

	bool LoadPictureFromFilePathName(string vFilePathName, bool IsEmbedded);

	void UpdateTexParamMag(UniformTexture2DMAGEnum vParam);
	void UpdateTexParamMin(UniformTexture2DMinEnum vParam);
	void UpdateTexParamWrapS(UniformTexture2DWrapSEnum vParam);
	void UpdateTexParamWrapT(UniformTexture2DWrapTEnum vParam);
	void UpdateTexParamGenerateMipMap(bool vMipMap);
	void UpdateTexParamBaseLevel(int vBaseLEvel, bool vGenerateMipmap);
	void UpdateTexParamMaxLevel(int vMaxLevel, bool vGenerateMipmap);
	void UpdateTexParamInvertY(bool vInvertY);

	virtual string ReplaceUniformInCode(string vCode);
	virtual void ChangeUniformName(string vCurrentName, string vNewName);

private:
	// vAutoPot a truc permet de retailler pour avoir un pot sinon rien
	bool loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot);
	bool loadEmbeddedImage(wxImage* img, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, bool vAutoPot);
private:
	cUniformVariant m_Tex2D;
	string m_FilePathName;
	wxSize m_btnSize;
	string m_Embed;

	cUniformVariant m_Tex2DSize;
	string m_Tex2DSizeName;

	UniformTexture2DMAGEnum m_Mag;
	UniformTexture2DMinEnum m_Min;
	UniformTexture2DWrapSEnum m_WrapS;
	UniformTexture2DWrapTEnum m_WrapT;
	bool m_GenerateMipmap;
	int m_BaseLevel;
	int m_MaxLevel;
	bool m_InvertY;
	bool m_IsEmbedded;

protected:
	virtual void OnChangeTexBtn(wxCommandEvent& event);
	virtual void OnResetTexBtn(wxCommandEvent& event);
	virtual void OnChoiceMag(wxCommandEvent& event);
	virtual void OnChoiceMin(wxCommandEvent& event);
	virtual void OnChoiceWrapS(wxCommandEvent& event);
	virtual void OnChoiceWrapT(wxCommandEvent& event);
	virtual void OnChoiceGenerateMipMap(wxCommandEvent& event);
	virtual void OnSpinMipMapBaseLevel(wxSpinEvent& event);
	virtual void OnSpinMipMapMaxLevel(wxSpinEvent& event);
	virtual void OnChoiceTexInvertY(wxCommandEvent& event);

protected:
	wxBitmapButton* m_bpButton1;
	wxBitmapButton* m_bpBtnTrash;

	wxStaticText* m_staticTextMag;
	wxChoice* m_ChoiceMag;
	wxStaticText* m_staticTextMin;
	wxChoice* m_ChoiceMin;
	wxStaticText* m_staticTextWrapS;
	wxChoice* m_ChoiceWrapS;
	wxStaticText* m_staticTextWrapT;
	wxChoice* m_ChoiceWrapT;
	wxStaticText* m_staticGenerateMipMap;
	wxChoice* m_ChoiceGenerateMipMap;
	wxStaticText* m_staticBaselevel;
	wxSpinCtrl* m_SpinBaselevel;
	wxStaticText* m_staticMaxLevel;
	wxSpinCtrl* m_SpinMaxLevel;
	wxStaticText* m_staticInvertY;
	wxChoice* m_ChoiceInvertY;

	wxDECLARE_EVENT_TABLE();
}; 


#endif //__UNIFORMTEXTURE2D_H__