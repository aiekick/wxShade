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

#ifndef __UNIFORMTEXTURECUBE_H__
#define __UNIFORMTEXTURECUBE_H__

#include "Globals.h"

#include "UniformAbstract.h"

#include <wx/wx.h>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"

#include "SettingsDlg.h"

#include <string>
using namespace std;

class UniformCubeMap : public UniformAbstract
{
public:
	UniformCubeMap(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformCubeMap();

	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual void UpdateValue(cUniformVariant vValue);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);

	GLuint CreateCubeMap();
	bool LoadCubeMapFromFilePathName(string vFilePathName[6], GLuint vCubeMapId);

	virtual string ReplaceUniformInCode(string vCode);
	virtual void ChangeUniformName(string vCurrentName, string vNewName);

private:
	bool CreateTexture(wxString filepathname, wxBitmapButton* btn, GLuint CubeMapId, int id);
	bool loadImage(wxString path, int* imageWidth, int* imageHeight, int* textureWidth, int* textureHeight, GLuint vID, bool vAutoPot);

private:
	cUniformVariant m_CubeMap;
	string m_FilePathName[6];
	wxSize m_btnSize;
	cUniformVariant m_CubeMapSize;
	string m_CubeMapSizeName;

protected:
	virtual void OnLoadCubeMapBtn(wxCommandEvent& event);
	virtual void OnChangeTexBtn(wxCommandEvent& event);
	virtual void OnResetTexBtn(wxCommandEvent& event);

protected:
	wxButton *m_Button;
	wxBitmapButton* m_bpButton[6];
	wxBitmapButton* m_bpBtnTrash;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMTEXTURECUBE_H__
