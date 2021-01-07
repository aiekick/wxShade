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

#include "UniformBVH.h"

#include "UniformManager.h"

#include "DocShaderArt.h"

#include <kazmath/kazmath/kazmath.h>

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/ControlStartBlue_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////

wxBEGIN_EVENT_TABLE(UniformBVH, UniformAbstract)
EVT_TIMER(ID_UNIFORM_TIMER, UniformBVH::OnTimer)
EVT_BUTTON(ID_UNIFORM_BVH_WIDGET_DIR_BTN, UniformBVH::OnChangeTexBtn)
EVT_BUTTON(ID_UNIFORM_BVH_WIDGET_TRASH_BTN, UniformBVH::OnResetTexBtn)
wxEND_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////////////

MyMusicTreeModel::MyMusicTreeModel()
{
	m_root = new MyMusicTreeModelNode(NULL, "My Music");

	// setup pop music
	m_pop = new MyMusicTreeModelNode(m_root, "Pop music");
	m_pop->Append(
		new MyMusicTreeModelNode(m_pop, "You are not alone", "Michael Jackson", 1995));
	m_pop->Append(
		new MyMusicTreeModelNode(m_pop, "Take a bow", "Madonna", 1994));
	m_root->Append(m_pop);

	// setup classical music
	m_classical = new MyMusicTreeModelNode(m_root, "Classical music");
	m_ninth = new MyMusicTreeModelNode(m_classical, "Ninth symphony",
		"Ludwig van Beethoven", 1824);
	m_classical->Append(m_ninth);
	m_classical->Append(new MyMusicTreeModelNode(m_classical, "German Requiem",
		"Johannes Brahms", 1868));
	m_root->Append(m_classical);

	m_classicalMusicIsKnownToControl = false;
}

wxString MyMusicTreeModel::GetTitle(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return wxEmptyString;

	return node->m_title;
}

wxString MyMusicTreeModel::GetArtist(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return wxEmptyString;

	return node->m_artist;
}

int MyMusicTreeModel::GetYear(const wxDataViewItem &item) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return 2000;

	return node->m_year;
}

void MyMusicTreeModel::AddToClassical(const wxString &title, const wxString &artist,
	unsigned int year)
{
	if (!m_classical)
	{
		wxASSERT(m_root);

		// it was removed: restore it
		m_classical = new MyMusicTreeModelNode(m_root, "Classical music");
		m_root->Append(m_classical);

		// notify control
		wxDataViewItem child((void*)m_classical);
		wxDataViewItem parent((void*)m_root);
		ItemAdded(parent, child);
	}

	// add to the classical music node a new node:
	MyMusicTreeModelNode *child_node =
		new MyMusicTreeModelNode(m_classical, title, artist, year);
	m_classical->Append(child_node);

	// FIXME: what's m_classicalMusicIsKnownToControl for?
	if (m_classicalMusicIsKnownToControl)
	{
		// notify control
		wxDataViewItem child((void*)child_node);
		wxDataViewItem parent((void*)m_classical);
		ItemAdded(parent, child);
	}
}

void MyMusicTreeModel::Delete(const wxDataViewItem &item)
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	if (!node)      // happens if item.IsOk()==false
		return;

	wxDataViewItem parent(node->GetParent());
	if (!parent.IsOk())
	{
		wxASSERT(node == m_root);

		// don't make the control completely empty:
		wxLogError("Cannot remove the root item!");
		return;
	}

	// is the node one of those we keep stored in special pointers?
	if (node == m_pop)
		m_pop = NULL;
	else if (node == m_classical)
		m_classical = NULL;
	else if (node == m_ninth)
		m_ninth = NULL;

	// first remove the node from the parent's array of children;
	// NOTE: MyMusicTreeModelNodePtrArray is only an array of _pointers_
	//       thus removing the node from it doesn't result in freeing it
	node->GetParent()->GetChildren().Remove(node);

	// free the node
	delete node;

	// notify control
	ItemDeleted(parent, item);
}

int MyMusicTreeModel::Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
	unsigned int column, bool ascending) const
{
	wxASSERT(item1.IsOk() && item2.IsOk());
	// should never happen

	if (IsContainer(item1) && IsContainer(item2))
	{
		wxVariant value1, value2;
		GetValue(value1, item1, 0);
		GetValue(value2, item2, 0);

		wxString str1 = value1.GetString();
		wxString str2 = value2.GetString();
		int res = str1.Cmp(str2);
		if (res) return res;

		// items must be different
		wxUIntPtr litem1 = (wxUIntPtr)item1.GetID();
		wxUIntPtr litem2 = (wxUIntPtr)item2.GetID();

		return litem1 - litem2;
	}

	return wxDataViewModel::Compare(item1, item2, column, ascending);
}

void MyMusicTreeModel::GetValue(wxVariant &variant,
	const wxDataViewItem &item, unsigned int col) const
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	switch (col)
	{
	case 0:
		variant = node->m_title;
		break;
	case 1:
		variant = node->m_artist;
		break;
	case 2:
		variant = (long)node->m_year;
		break;
	case 3:
		variant = node->m_quality;
		break;
	case 4:
		variant = 80L;  // all music is very 80% popular
		break;
	case 5:
		if (GetYear(item) < 1900)
			variant = "old";
		else
			variant = "new";
		break;

	default:
		wxLogError("MyMusicTreeModel::GetValue: wrong column %d", col);
	}
}

bool MyMusicTreeModel::SetValue(const wxVariant &variant,
	const wxDataViewItem &item, unsigned int col)
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	switch (col)
	{
	case 0:
		node->m_title = variant.GetString();
		return true;
	case 1:
		node->m_artist = variant.GetString();
		return true;
	case 2:
		node->m_year = variant.GetLong();
		return true;
	case 3:
		node->m_quality = variant.GetString();
		return true;

	default:
		wxLogError("MyMusicTreeModel::SetValue: wrong column");
	}
	return false;
}

bool MyMusicTreeModel::IsEnabled(const wxDataViewItem &item,
	unsigned int col) const
{
	wxASSERT(item.IsOk());

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();

	// disable Beethoven's ratings, his pieces can only be good
	return !(col == 3 && node->m_artist.EndsWith("Beethoven"));
}

wxDataViewItem MyMusicTreeModel::GetParent(const wxDataViewItem &item) const
{
	// the invisible root node has no parent
	if (!item.IsOk())
		return wxDataViewItem(0);

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();

	// "MyMusic" also has no parent
	if (node == m_root)
		return wxDataViewItem(0);

	return wxDataViewItem((void*)node->GetParent());
}

bool MyMusicTreeModel::IsContainer(const wxDataViewItem &item) const
{
	// the invisble root node can have children
	// (in our model always "MyMusic")
	if (!item.IsOk())
		return true;

	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)item.GetID();
	return node->IsContainer();
}

unsigned int MyMusicTreeModel::GetChildren(const wxDataViewItem &parent,
	wxDataViewItemArray &array) const
{
	MyMusicTreeModelNode *node = (MyMusicTreeModelNode*)parent.GetID();
	if (!node)
	{
		array.Add(wxDataViewItem((void*)m_root));
		return 1;
	}

	if (node == m_classical)
	{
		MyMusicTreeModel *model = (MyMusicTreeModel*)(const MyMusicTreeModel*) this;
		model->m_classicalMusicIsKnownToControl = true;
	}

	if (node->GetChildCount() == 0)
	{
		return 0;
	}

	unsigned int count = node->GetChildren().GetCount();
	for (unsigned int pos = 0; pos < count; pos++)
	{
		MyMusicTreeModelNode *child = node->GetChildren().Item(pos);
		array.Add(wxDataViewItem((void*)child));
	}

	return count;
}

//////////////////////////////////////////////////////////////////////////////
UniformBVH::UniformBVH(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: UniformAbstract(parent, id, pos, size, style, vShaderRenderer, vShaderView, vUniformManager), m_timer(this, ID_UNIFORM_TIMER), rootJoint(0), m_CurrentFrame(0)
{
	wxBoxSizer* bSizer39;
	bSizer39 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer40;
	bSizer40 = new wxBoxSizer(wxHORIZONTAL);

	m_button1 = new wxButton(this, ID_UNIFORM_BVH_WIDGET_DIR_BTN, wxT("..."), wxDefaultPosition, wxDefaultSize, 0);
	m_button1->SetMaxSize(wxSize(24, 24));

	bSizer40->Add(m_button1, 0, wxALL, 5);

	m_button11 = new wxButton(this, ID_UNIFORM_BVH_WIDGET_TRASH_BTN, wxT("X"), wxDefaultPosition, wxDefaultSize, 0);
	m_button11->SetMaxSize(wxSize(24, 24));

	bSizer40->Add(m_button11, 0, wxALL, 5);

	m_button111 = new wxButton(this, wxID_ANY, wxT("Play"), wxDefaultPosition, wxDefaultSize, 0);
	m_button111->SetMaxSize(wxSize(24, 24));

	bSizer40->Add(m_button111, 0, wxALL, 5);

	m_button112 = new wxButton(this, wxID_ANY, wxT("Boucle"), wxDefaultPosition, wxDefaultSize, 0);
	m_button112->SetMaxSize(wxSize(24, 24));

	bSizer40->Add(m_button112, 0, wxALL, 5);

	m_button1121 = new wxButton(this, wxID_ANY, wxT("Reset"), wxDefaultPosition, wxDefaultSize, 0);
	m_button1121->SetMaxSize(wxSize(24, 24));

	bSizer40->Add(m_button1121, 0, wxALL, 5);

	m_staticText30 = new wxStaticText(this, wxID_ANY, wxT("MyLabel"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE);
	m_staticText30->Wrap(-1);
	bSizer40->Add(m_staticText30, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	bSizer39->Add(bSizer40, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer401;
	bSizer401 = new wxBoxSizer(wxHORIZONTAL);

	m_textCtrl40 = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	m_textCtrl40->SetMaxSize(wxSize(50, -1));

	bSizer401->Add(m_textCtrl40, 0, wxALL, 5);

	m_staticline7 = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizer401->Add(m_staticline7, 0, wxALL | wxEXPAND, 5);

	m_slider6 = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_slider6->Connect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Connect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);

	bSizer401->Add(m_slider6, 1, wxALL, 5);

	bSizer39->Add(bSizer401, 0, wxEXPAND, 5);

	wxBoxSizer* bSizer41;
	bSizer41 = new wxBoxSizer(wxVERTICAL);

	m_dataViewCtrl1 = new wxDataViewCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
	bSizer41->Add(m_dataViewCtrl1, 1, wxALL | wxEXPAND, 5);

	bSizer39->Add(bSizer41, 1, wxEXPAND, 5);

	this->SetSizer(bSizer39);
	this->Layout();

	//InitDataViewCtrl();

	motionData.data = 0;
}

UniformBVH::~UniformBVH()
{
	m_slider6->Disconnect(wxEVT_SCROLL_TOP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_BOTTOM, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_LINEUP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_PAGEUP, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);
	m_slider6->Disconnect(wxEVT_SCROLL_CHANGED, wxScrollEventHandler(UniformBVH::OnChangeSlideValue), NULL, this);

	clear(true);
}

// bool vDestroyClass est pour savoir si c'est la phase ou on detruit la classe ou non
void UniformBVH::clear(bool vDestroyClass)
{
	m_staticText30->SetLabel("");
	deleteJoint(rootJoint, vDestroyClass);
	rootJoint = 0;
	if (motionData.data != NULL)
		delete[] motionData.data;
	motionData.data = 0;
}

string UniformBVH::Init(string vCodeSelection)
{
	string res;

	if (m_Params.size() > 0)
	{
		string BVHFILE;
		
		// init from param
		for (std::map<string, string>::iterator it = m_Params.begin(); it != m_Params.end(); ++it)
		{
			string attrib = it->first;
			cVariant value = it->second;

			if (attrib == "BVHFILE") BVHFILE = value.getS();
			if (attrib == "FRAME") m_CurrentFrame = value.getF();
		}

		if (this->LoadBvh(BVHFILE) == true)
		{
			m_slider6->SetValue(m_CurrentFrame);
			wxScrollEvent e; e.SetEventObject(m_slider6);
			OnChangeSlideValue(e);
		}
	}
	else
	{

	}

	return res;
}

void UniformBVH::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	UniformAbstract::SetShaderRenderer(vShaderRenderer);
}

void UniformBVH::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
	SetDatas(vValue);
}

void UniformBVH::SetDatas(cUniformVariant /*vValue*/)
{

}

string UniformBVH::GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/)
{
	string xml;

	// les données interessantes sont :
	// le fichier du bvh

	string bvhfile = "BVHFILE=\"" + m_FilePathName + "\"";
	string frame = "FRAME=\"" + toStr(m_CurrentFrame) + "\"";

	xml = bvhfile + " " + frame;

	return xml;
}

void UniformBVH::SetParamsFromXML(map<string, string> vParams)
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

void UniformBVH::OnTimer(wxTimerEvent& /*event*/)
{

}

void UniformBVH::OnValueChange(wxCommandEvent& event)
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
					/*
					// action
					if (txtCtrl == m_textCtrl322) // org x
						m_NavOrigin.x = t;
					else if (txtCtrl == m_textCtrl324) // org y
						m_NavOrigin.y = t;
					else if (txtCtrl == m_textCtrl325) // org z
						m_NavOrigin.z = t;
					else if (txtCtrl == m_textCtrl3221) // dir x
						m_NavDirection.x = t;
					else if (txtCtrl == m_textCtrl3241) // dir y
						m_NavDirection.y = t;
					else if (txtCtrl == m_textCtrl3251) // dir z
						m_NavDirection.z = t;
					// Update Uniform
					if (m_UniformManager != 0)
					{
						m_UniformManager->UpdateUniformByName(m_NavOriginUnifromName, m_NavOrigin, true);
						m_UniformManager->UpdateUniformByName(m_NavDirectionUnifromName, m_NavDirection, true);
					}
					*/
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

void UniformBVH::OnChangeTexBtn(wxCommandEvent& /*event*/)
{
	HDC hdc = m_ShaderView->GetHDC();
	m_ShaderRenderer->SetCurrent(hdc);

	wxFileDialog fileDlg(this, _("Open"), "", "", "All BVH files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (fileDlg.ShowModal() == wxID_OK)
	{
		m_FilePathName = fileDlg.GetPath().ToStdString();
		if (LoadBvh(m_FilePathName) == true)
		{

		}
	}}

void UniformBVH::OnResetTexBtn(wxCommandEvent& /*event*/)
{
	this->clear(false);

	/*
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
	*/

	this->Layout();
	if (this->m_parent != 0)
		this->m_parent->Layout();
	this->GetParent()->GetParent()->Layout();
	this->GetParent()->GetParent()->GetParent()->Layout();
}

bool UniformBVH::LoadBvh(string vFilePathName)
{
	bool state = false;

	this->clear(false);

	std::fstream file;
	file.open(vFilePathName.c_str(), std::ios_base::in);

	if (file.is_open())
	{
		std::string line;

		while (file.good())
		{
			file >> line;
			//klog.l("load") << line;
			if (trim(line) == "HIERARCHY")
			{
				loadHierarchy(file);

				string countFrames = "Frames : " + toStr(motionData.num_frames);
				string title = vFilePathName + "\n" + countFrames;

				m_staticText30->SetLabel(title);

				m_slider6->SetMax(motionData.num_frames);
				m_slider6->SetMin(0);
				m_slider6->SetValue(0);

				m_FilePathName = vFilePathName;

				state = true;
			}
			break;
		}

		file.close();
	}

	this->Layout();
	if (this->m_parent != 0)
		this->m_parent->Layout();
	this->GetParent()->GetParent()->Layout();
	this->GetParent()->GetParent()->GetParent()->Layout();

	return state;
}

// vOnlyRemoveUniformFromManager a trus faire que l'uniform est retiré du manager mais que le widget de l'item est liassé au bon soin de wxWidget*
// car deleteJoint est appellé dans le destructuer et aussi avec l'appui sur reset
void UniformBVH::deleteJoint(JOINT* joint, bool vOnlyRemoveUniformFromManager)
{
	if (joint == NULL)
		return;

	for (JOINT* child : joint->children)
		deleteJoint(child, vOnlyRemoveUniformFromManager);

	if (joint->channels_order != NULL)
		delete joint->channels_order;

	//    if( joint->name != NULL && joint->name != "EndSite" ){
	//        klog.l() << joint->name;
	//        delete joint->name;
	//    }

	if (vOnlyRemoveUniformFromManager == false)
	{
		// uniform
		m_UniformViewItem->RemoveUniform(joint->uniformName);
	}
	else
	{
		// uniform on enleve juste l'uniform de la bd et on laisse wxwidget s'occuper de detruire les widgets
		m_UniformManager->RemoveUniformByName(joint->uniformName);
	}

	delete joint;

	joint = 0;
}

void UniformBVH::printJoint(const JOINT* const joint) const
{
	//klog.l("joint") << "print joint" << joint->name << joint->channel_start;

	for (std::vector<JOINT*>::const_iterator ct = joint->children.begin();
		ct != joint->children.end(); ++ct)
	{
		JOINT* _tmp = *ct;

		if (_tmp->children.size() > 0)
			printJoint(_tmp);
	}
}

// calculate local transformation matrix for specific frame - revisit this
void UniformBVH::moveJoint(JOINT* vJoint, MOTION* vMotionData, int vFrame_starts_index)
{
	if (vJoint == 0) return;

	int start_index = vFrame_starts_index + vJoint->channel_start;

	// postavljamo prvo lokalnu mat na offset poziciju
	vJoint->matrix = glm::translate(glm::mat4(1.0),
		glm::vec3(vJoint->offset.x,
			vJoint->offset.y,
			vJoint->offset.z));

	float x = 0, y = 0, z = 0;
	for (unsigned int i = 0; i < vJoint->num_channels; i++)
	{
		const short& channel = vJoint->channels_order[i];

		float value = vMotionData->data[start_index + i];
		if (channel & Xposition)
		{
			vJoint->matrix = glm::translate(vJoint->matrix, glm::vec3(value, 0, 0));
		}
		if (channel & Yposition)
		{
			vJoint->matrix = glm::translate(vJoint->matrix, glm::vec3(0, value, 0));
		}
		if (channel & Zposition)
		{
			vJoint->matrix = glm::translate(vJoint->matrix, glm::vec3(0, 0, value));
		}

		// doit etre y x et z
		if (channel & Xrotation)
		{
			//joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(1, 0, 0));
			x = value;
		}
		if (channel & Yrotation)
		{
			//joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 1, 0));
			y = value;
		}
		if (channel & Zrotation)
		{
			//joint->matrix = glm::rotate(joint->matrix, value, glm::vec3(0, 0, 1));
			z = value;
		}
	}

	if (vJoint->num_channels >= 3)
	{
		vJoint->matrix = glm::rotate(vJoint->matrix, y, glm::vec3(0, 1, 0));
		vJoint->matrix = glm::rotate(vJoint->matrix, x, glm::vec3(1, 0, 0));
		vJoint->matrix = glm::rotate(vJoint->matrix, z, glm::vec3(0, 0, 1));
	}
	else
	{
		x = 0;
	}
	// mul current matrix with one of a parent
	if (vJoint->parent != NULL)
		vJoint->matrix = vJoint->parent->matrix * vJoint->matrix;

	for (auto& child : vJoint->children)
		moveJoint(child, vMotionData, vFrame_starts_index);
}

void UniformBVH::moveTo(unsigned frame)
{
	// calculate start index for specific frame for motion data
	unsigned start_index = frame * motionData.num_motion_channels;

	rootJoint = (JOINT*)this->getRootJoint();
	moveJoint(rootJoint, &motionData, start_index);
}

void UniformBVH::testOutput() const
{
	if (rootJoint == 0)
		return;

	//klog.l() << "output";
//	printJoint(rootJoint);

	LogStr("num frames: " + toStr(motionData.num_frames));
	LogStr("num motion channels: " + toStr(motionData.num_motion_channels));

	/*int num_frames = motionData.num_frames;
	int num_channels = motionData.num_motion_channels;

	for(int frame = 0; frame < num_frames; frame++)
	{
		klog.l() << "frame: " << frame;

	        for(int channel = 0; channel < num_channels; channel++)
	        {
	            int index = frame * num_channels + channel;

	            klog << motionData.data[index];
	        }

	        static int tmp = 0;
	        std::for_each(allJoints.begin(), allJoints.end(), 
				[](const JOINT* joint)
				{
					tmp += joint->num_channels;
				}
			);

	        int tmp = frame * num_channels;
	        int tmp2 = 0;

	        for(std::set<JOINT*>::const_iterator it = allJoints.begin();
	            it != allJoints.end(); ++it)
	        {
	            JOINT* joint = *it;

	            klog.l() << joint->name;
	            if( joint->name == "EndSite" )
	                continue;

	            int motion_index = tmp + tmp2;

	            int start = motion_index;
	            int end = start + joint->num_channels;

	            klog.l() << joint->name << motion_index;
	            for(int x = start; x < end; x++)
	            {
	                //klog << motionData.data[x];
	            }

	            tmp2 += joint->num_channels;

	            float* niz = this->getMotion(frame, joint);
	            for( int i = 0; i < joint->num_channels; i++)
	            {
	                klog << niz[i];
	            }
	        }

	        klog.l("p") << frame << motionData.data[frame];
	    }*/

}

/** put translated joint vertices into array */
void UniformBVH::bvh_to_vertices(JOINT* joint)
{
	// vertex from current joint is in 4-th ROW (column-major ordering)
	glm::vec4 translatedVertex = joint->matrix[3];

	joint->uniform.x = translatedVertex.x;
	joint->uniform.y = translatedVertex.y;
	joint->uniform.z = translatedVertex.z;
	//joint->uniform.w = translatedVertex.w;

	if (m_UniformManager != 0)
	{
		m_UniformManager->UpdateUniformByName(joint->uniformName, joint->uniform, true);
	}

	// foreach child same thing
	for (auto& child : joint->children)
	{
		bvh_to_vertices(child);
	}
}

void UniformBVH::uploadForFrame(int frame)
{
	m_CurrentFrame = frame;

	this->moveTo(frame);

	rootJoint = (JOINT*)this->getRootJoint();
	this->bvh_to_vertices(rootJoint);

	// here goes OpenGL stuff with gen/bind buffer and sending data
	// basically you want to use GL_DYNAMIC_DRAW so you can update same VBO
}

void UniformBVH::loadHierarchy(std::istream& stream)
{
	std::string tmp;

	while (stream.good())
	{
		stream >> tmp;
		//klog.l("hier") << tmp;

		if (trim(tmp) == "ROOT")
			rootJoint = loadJoint(stream);
		else if (trim(tmp) == "MOTION")
			loadMotion(stream);
	}
}

void UniformBVH::loadMotion(std::istream& stream)
{
	//    klog.l() << "num joints:" << stats.num_total_joints;
	//    klog << stats.num_motion_channels;

	std::string tmp;

	while (stream.good())
	{
		stream >> tmp;

		if (trim(tmp) == "Frames:")
		{
			stream >> motionData.num_frames;
		}
		else if (trim(tmp) == "Frame")
		{
			float frame_time;
			stream >> tmp >> frame_time;

			int num_frames = motionData.num_frames;
			int num_channels = motionData.num_motion_channels;

			//klog.l() << "num frames:" << num_frames;
			//klog.l() << "num channels:" << num_channels;

			motionData.data = new float[num_frames * num_channels];

			for (int frame = 0; frame < num_frames; frame++)
			{
				for (int channel = 0; channel < num_channels; channel++)
				{
					float x;
					std::stringstream ss;
					stream >> tmp;
					ss << tmp;
					ss >> x;

					int index = frame * num_channels + channel;
					motionData.data[index] = x;
				}
			}
		}
	}
}

JOINT* UniformBVH::loadJoint(std::istream& stream, JOINT* parent)
{
	JOINT* joint = new JOINT();
	joint->parent = parent;

	//    stats.num_total_joints++;
	//    allJoints.insert(joint);

	// load joint name
	std::string name;
	stream >> name;
	joint->name = name.c_str();

	// uniform
	joint->uniformName = m_UniformViewItem->AddUniform(joint->name, UniformsTypeEnum::UVEC3, false);

	std::string tmp;
	joint->matrix = glm::mat4(1.0);

	static int _channel_start = 0;
	unsigned channel_order_index = 0;
	while (stream.good())
	{
		stream >> tmp;
		tmp = trim(tmp);

		// setting channels
		char c = tmp.at(0);
		if (c == 'X' || c == 'Y' || c == 'Z')
		{
			if (tmp == "Xposition")
			{
				joint->channels_order[channel_order_index++] = Xposition;
			}
			if (tmp == "Yposition")
			{
				joint->channels_order[channel_order_index++] = Yposition;
			}
			if (tmp == "Zposition")
			{
				joint->channels_order[channel_order_index++] = Zposition;
			}

			if (tmp == "Xrotation")
			{
				joint->channels_order[channel_order_index++] = Xrotation;
			}
			if (tmp == "Yrotation")
			{
				joint->channels_order[channel_order_index++] = Yrotation;
			}
			if (tmp == "Zrotation")
			{
				joint->channels_order[channel_order_index++] = Zrotation;
			}
		}

		if (tmp == "OFFSET")
		{
			stream >> joint->offset.x
				>> joint->offset.y
				>> joint->offset.z;
		}
		else if (tmp == "CHANNELS")
		{
			// loading num of channels
			stream >> joint->num_channels;

			// adding to statistics
			motionData.num_motion_channels += joint->num_channels;

			// increasing static counter of channel index startin motion section
			joint->channel_start = _channel_start;
			_channel_start += joint->num_channels;

			// creating array for channel order specification
			joint->channels_order = new short[joint->num_channels];

		}
		else if (tmp == "JOINT")
		{
			JOINT* tmp_joint = loadJoint(stream, joint);

			tmp_joint->parent = joint;
			joint->children.push_back(tmp_joint);

			//            allJoints.insert(tmp_joint);

		}
		else if (tmp == "End")
		{
			// End Site {
			stream >> tmp >> tmp;

			JOINT* tmp_joint = new JOINT();

			
			tmp_joint->parent = joint;
			tmp_joint->num_channels = 0;
			tmp_joint->name = "EndSite";
			joint->children.push_back(tmp_joint);

			// uniform
			name = joint->name; name += tmp_joint->name;
			tmp_joint->uniformName = m_UniformViewItem->AddUniform(name, UniformsTypeEnum::UVEC3, false);

			//            allJoints.insert(tmp_joint);

			stream >> tmp;
			if (tmp == "OFFSET")
				stream >> tmp_joint->offset.x
				>> tmp_joint->offset.y
				>> tmp_joint->offset.z;

			// ucitavanje }
			stream >> tmp;
		}
		else if (tmp == "}")
			return joint;

	}
	return joint;
}


// on maj la valeur en focntion du slider
void UniformBVH::OnChangeSlideValue(wxScrollEvent& event)
{
	wxSlider *slider = static_cast<wxSlider*>(event.GetEventObject());
	if (slider != 0)
	{
		int frame = slider->GetValue();
		this->uploadForFrame(frame);
		m_textCtrl40->SetValue(toStr(frame));
		UpdateDataViewCtrlForFrame(frame);
	}
}

void UniformBVH::InitDataViewCtrl()
{
	//m_music_model = new MyMusicTreeModel();
	//m_dataViewCtrl1->AssociateModel(m_music_model.get());

	wxDataViewTextRenderer *tr = 0;
	wxDataViewColumn *column = 0;

	tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);

	column = new wxDataViewColumn("Skeleton", tr, 0, 200, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE);
	m_dataViewCtrl1->AppendColumn(column);

	tr = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_EDITABLE);

	column = new wxDataViewColumn("TX", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

	column = new wxDataViewColumn("TY", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

	column = new wxDataViewColumn("TZ", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

	column = new wxDataViewColumn("RX", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

	column = new wxDataViewColumn("RY", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

	column = new wxDataViewColumn("RZ", tr, 1, 50, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_REORDERABLE | wxDATAVIEW_COL_RESIZABLE);
	column->SetMinWidth(150); // this column can't be resized to be smaller
	m_dataViewCtrl1->AppendColumn(column);

}

void UniformBVH::UpdateDataViewCtrlForFrame(int /*vFrame*/)
{

}

string UniformBVH::ReplaceUniformInCode(string vCode)
{
	string code = vCode;

	string msg = "The Replace Uniform In Code in the widget are not implemented for the BVH Widget";
	wxMessageBox(msg, "FAIL !!");

	return code;
}

// ca sera utile
void UniformBVH::ChangeUniformName(string vCurrentName, string vNewName)
{
	string msg = "The rename of uniforme name in the widget are not implemented for the BVH Widget";
	wxMessageBox(msg, "FAIL !!");
}