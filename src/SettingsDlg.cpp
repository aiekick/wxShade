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

#include "SettingsDlg.h"

#include "UniformView.h"

#include "UniformManager.h"

#include "EditorView.h"

#include "DocShaderArt.h"

// static string
string SettingsDlg::AssetsDirPath = "";
string SettingsDlg::ShadersDirPath = "";

SettingsDlg::SettingsDlg(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxPropertySheetDialog(parent, id, title, pos, size, style)
{
	CreateButtons(wxOK | wxCANCEL);// | wxHELP);

	// load config
	SettingsDlg::LoadConfig();

	//////////////////////////////////////////////////////////
	///// PANE DIRECTORY /////////////////////////////////////
	//////////////////////////////////////////////////////////

	m_PaneDirectory = new wxPanel(GetBookCtrl(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(m_PaneDirectory, wxID_ANY, wxT("Assets :")), wxVERTICAL);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText1 = new wxStaticText(m_PaneDirectory, wxID_ANY, wxT("Directory Path :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer9->Add(m_staticText1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_dirPicker2 = new wxDirPickerCtrl(m_PaneDirectory, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE | wxDIRP_DIR_MUST_EXIST);
	m_dirPicker2->SetPath(SettingsDlg::AssetsDirPath);
	bSizer9->Add(m_dirPicker2, 1, wxALL, 5);

	sbSizer1->Add(bSizer9, 1, wxEXPAND, 5);

	bSizer8->Add(sbSizer1, 0, wxEXPAND, 5);

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(m_PaneDirectory, wxID_ANY, wxT("Shaders :")), wxVERTICAL);

	wxBoxSizer* bSizer91;
	bSizer91 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText11 = new wxStaticText(m_PaneDirectory, wxID_ANY, wxT("Directory Path :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText11->Wrap(-1);
	bSizer91->Add(m_staticText11, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_dirPicker3 = new wxDirPickerCtrl(m_PaneDirectory, wxID_ANY, wxEmptyString, wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE | wxDIRP_DIR_MUST_EXIST);
	m_dirPicker3->SetPath(SettingsDlg::ShadersDirPath);
	bSizer91->Add(m_dirPicker3, 1, wxALL, 5);

	sbSizer2->Add(bSizer91, 1, wxEXPAND, 5);

	bSizer8->Add(sbSizer2, 0, wxEXPAND, 5);

	m_PaneDirectory->SetSizer(bSizer8);
	m_PaneDirectory->Layout();
	bSizer8->Fit(m_PaneDirectory);

	// Add page
	GetBookCtrl()->AddPage(m_PaneDirectory, "Directorys");

	//////////////////////////////////////////////////////////
	///// PANE GENERAL ///////////////////////////////////////
	//////////////////////////////////////////////////////////

	//m_PaneGeneral = new wxPanel(GetBookCtrl(), wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	// Add page
	//GetBookCtrl()->AddPage(m_PaneGeneral, "General");

	/////////////////////////////////////////////////////////
	LayoutDialog();


}

SettingsDlg::~SettingsDlg()
{
	

}

bool SettingsDlg::SaveConfig()
{
	bool res = false;

	SettingsDlg::AssetsDirPath = m_dirPicker2->GetPath();
	wxFileName assetsPath = wxFileName(SettingsDlg::AssetsDirPath);
	assetsPath.MakeRelativeTo();

	SettingsDlg::ShadersDirPath = m_dirPicker3->GetPath();
	wxFileName shadersPath = wxFileName(SettingsDlg::ShadersDirPath);
	shadersPath.MakeRelativeTo();

	string path = "config.xml";

	ofstream docFile(path, ios::out);
	if (docFile.bad() == false)
	{
		docFile << "<CONFIG>\n";

		docFile << "<DIRECTORY type=\"assets\" path=\"" + assetsPath.GetFullPath().ToStdString() + "\"/>\n";
		docFile << "<DIRECTORY type=\"shaders\" path=\"" + shadersPath.GetFullPath().ToStdString() + "\"/>\n";

		docFile << "</CONFIG>\n";

		docFile.close();

		res = true;
	}
	else
	{
		// unable to write file with path
		// message box
	}

	return res;
}

bool SettingsDlg::LoadConfig()
{
	/*
	<CONFIG>
		<DIRECTORY type="assets" path=""/>
		<DIRECTORY type="shaders" path=""/>
	</CONFIG>
	*/

	bool res = false;

	ifstream docFile("config.xml", ios::in);
	if (docFile.is_open())
	{
		stringstream strStream;
		strStream << docFile.rdbuf();//read the file

		DocumentFragmentParseErrorEnum erno = SettingsDlg::parse(strStream.str().c_str());

		docFile.close(); // close file

		// on va afficher l'erreur si erreur il y a
		switch (erno)
		{
		case NOTHING:
			res = true;
			break;
		case BAD_XML_FORMAT:
			break;
		case DUPLICATE_UNIFORM_NAME:
			break;
		case UNIFORM_NAME_MISSED:
			break;
		default:
			res = true;
		}
	}

	return res;
}

DocumentFragmentParseErrorEnum SettingsDlg::parse(const char* datas)
{
	DocumentFragmentParseErrorEnum res;

	try
	{
		// first load the xml file
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError err = doc.Parse(datas);
		if (err == tinyxml2::XMLError::XML_SUCCESS)
		{
			res = SettingsDlg::RecursParsing(doc.FirstChildElement("CONFIG"));
		}
		else
		{
			res = DocumentFragmentParseErrorEnum::BAD_XML_FORMAT;
		}
	}
	catch (std::exception& ex)
	{
		string err = "ShaderArtDocument::parse(const char* datas) => " + toStr(ex.what());
		LogStr(err);
		res = DocumentFragmentParseErrorEnum::BAD_XML_FORMAT;
	}

	return res;
}

DocumentFragmentParseErrorEnum SettingsDlg::RecursParsing(tinyxml2::XMLElement* vElem)
{
	DocumentFragmentParseErrorEnum vErno = DocumentFragmentParseErrorEnum::NOTHING;

	// The value of this child identifies the name of this element
//	UniformsWidgetTypeEnum uniformType = UniformsWidgetTypeEnum::UNONE;
	std::string strName = "";
	std::string strValue = "";
	std::string strType = "";
	std::map<string, string> uniformParams;

	strName = vElem->Value();

	if (strName == "DIRECTORY")
	{
		// now parse through all the attributes
		for (const tinyxml2::XMLAttribute* attr = vElem->FirstAttribute(); attr != 0; attr = attr->Next())
		{
			strName = attr->Name();
			strValue = attr->Value();

			if (strName == "type")
			{
				strType = strValue;
			}
			else if (strName == "path")
			{
				if (strType == "assets")
				{
					SettingsDlg::AssetsDirPath = strValue;
				}
				if (strType == "shaders")
				{
					SettingsDlg::ShadersDirPath = strValue;
				}
			}
		}
	}

	if (vErno != DocumentFragmentParseErrorEnum::NOTHING)
		return vErno;

	// CHILDS 
	// parse through all childs elements
	for (tinyxml2::XMLElement* child = vElem->FirstChildElement(); child != 0; child = child->NextSiblingElement())
	{
		SettingsDlg::RecursParsing(child->ToElement());
	}

	return vErno;
}