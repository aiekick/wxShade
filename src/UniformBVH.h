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

#ifndef __UNIFORMBVH_H__
#define __UNIFORMBVH_H__

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
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/slider.h>
#include <wx/dataview.h>
#include <wx/panel.h>

#include <string>
#include <fstream>
#include <vector>
#include <set>
#include <sstream>
#include <algorithm>
#include <functional>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"

#define WIN32_LEAN_AND_MEAN // We don't want the extra stuff like MFC and such
#include <windows.h>

using namespace std;

#define Xposition 0x01
#define Yposition 0x02
#define Zposition 0x04
#define Zrotation 0x10
#define Xrotation 0x20
#define Yrotation 0x40

//////////////////////////////////////////////////////////////////////////////////////////

class MyMusicTreeModelNode;
WX_DEFINE_ARRAY_PTR(MyMusicTreeModelNode*, MyMusicTreeModelNodePtrArray);
class MyMusicTreeModelNode
{
public:
	MyMusicTreeModelNode(MyMusicTreeModelNode* parent,
		const wxString &title, const wxString &artist,
		unsigned int year)
	{
		m_parent = parent;

		m_title = title;
		m_artist = artist;
		m_year = year;
		m_quality = "good";

		m_container = false;
	}

	MyMusicTreeModelNode(MyMusicTreeModelNode* parent,
		const wxString &branch)
	{
		m_parent = parent;

		m_title = branch;
		m_year = -1;

		m_container = true;
	}

	~MyMusicTreeModelNode()
	{
		// free all our children nodes
		size_t count = m_children.GetCount();
		for (size_t i = 0; i < count; i++)
		{
			MyMusicTreeModelNode *child = m_children[i];
			delete child;
		}
	}

	bool IsContainer() const
	{
		return m_container;
	}

	MyMusicTreeModelNode* GetParent()
	{
		return m_parent;
	}
	MyMusicTreeModelNodePtrArray& GetChildren()
	{
		return m_children;
	}
	MyMusicTreeModelNode* GetNthChild(unsigned int n)
	{
		return m_children.Item(n);
	}
	void Insert(MyMusicTreeModelNode* child, unsigned int n)
	{
		m_children.Insert(child, n);
	}
	void Append(MyMusicTreeModelNode* child)
	{
		m_children.Add(child);
	}
	unsigned int GetChildCount() const
	{
		return m_children.GetCount();
	}

public:     // public to avoid getters/setters
	wxString                m_title;
	wxString                m_artist;
	int                     m_year;
	wxString                m_quality;

	// TODO/FIXME:
	// the GTK version of wxDVC (in particular wxDataViewCtrlInternal::ItemAdded)
	// needs to know in advance if a node is or _will be_ a container.
	// Thus implementing:
	//   bool IsContainer() const
	//    { return m_children.GetCount()>0; }
	// doesn't work with wxGTK when MyMusicTreeModel::AddToClassical is called
	// AND the classical node was removed (a new node temporary without children
	// would be added to the control)
	bool m_container;

private:
	MyMusicTreeModelNode          *m_parent;
	MyMusicTreeModelNodePtrArray   m_children;
};


// ----------------------------------------------------------------------------
// MyMusicTreeModel
// ----------------------------------------------------------------------------

/*
Implement this data model
Title               Artist               Year        Judgement
--------------------------------------------------------------------------
1: My Music:
2:  Pop music
3:  You are not alone   Michael Jackson      1995        good
4:  Take a bow          Madonna              1994        good
5:  Classical music
6:  Ninth Symphony      Ludwig v. Beethoven  1824        good
7:  German Requiem      Johannes Brahms      1868        good
*/

class MyMusicTreeModel : public wxDataViewModel
{
public:
	MyMusicTreeModel();
	~MyMusicTreeModel()
	{
		delete m_root;
	}

	// helper method for wxLog

	wxString GetTitle(const wxDataViewItem &item) const;
	wxString GetArtist(const wxDataViewItem &item) const;
	int GetYear(const wxDataViewItem &item) const;

	// helper methods to change the model

	void AddToClassical(const wxString &title, const wxString &artist,
		unsigned int year);
	void Delete(const wxDataViewItem &item);

	wxDataViewItem GetNinthItem() const
	{
		return wxDataViewItem(m_ninth);
	}

	// override sorting to always sort branches ascendingly

	int Compare(const wxDataViewItem &item1, const wxDataViewItem &item2,
		unsigned int column, bool ascending) const;

	// implementation of base class virtuals to define model

	virtual unsigned int GetColumnCount() const
	{
		return 6;
	}

	virtual wxString GetColumnType(unsigned int col) const
	{
		if (col == 2)
			return wxT("long");

		return wxT("string");
	}

	virtual void GetValue(wxVariant &variant,
		const wxDataViewItem &item, unsigned int col) const;
	virtual bool SetValue(const wxVariant &variant,
		const wxDataViewItem &item, unsigned int col);

	virtual bool IsEnabled(const wxDataViewItem &item,
		unsigned int col) const;

	virtual wxDataViewItem GetParent(const wxDataViewItem &item) const;
	virtual bool IsContainer(const wxDataViewItem &item) const;
	virtual unsigned int GetChildren(const wxDataViewItem &parent,
		wxDataViewItemArray &array) const;

private:
	MyMusicTreeModelNode*   m_root;

	// pointers to some "special" nodes of the tree:
	MyMusicTreeModelNode*   m_pop;
	MyMusicTreeModelNode*   m_classical;
	MyMusicTreeModelNode*   m_ninth;

	// ??
	bool                    m_classicalMusicIsKnownToControl;
};

//////////////////////////////////////////////////////////////////////////////////////////

// trim from start
static inline std::string &ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) 
{
	return ltrim(rtrim(s));
}

struct OFFSET
{
	float x, y, z;
};


struct JOINT
{
	const char* name;// joint name
	JOINT* parent;// joint parent
	OFFSET offset;// offset data
	unsigned int num_channels;// num of channels joint has
	short* channels_order;// ordered list of channels
	std::vector<JOINT*> children;// joint's children
	glm::mat4 matrix;// local transofrmation matrix (premultiplied with parents'
	unsigned int channel_start;// index of joint's channel data in motion array
	cUniformVariant uniform;
	string uniformName;

	JOINT()
	{
		name = NULL;
		parent = NULL;
		num_channels = 0;
		channels_order = NULL;
		channel_start = 0;
	}
};

struct HIERARCHY
{
	JOINT* rootJoint;
	int num_channels;

	HIERARCHY()
	{
		rootJoint = 0;
		num_channels = 0;
	}
};

struct MOTION
{
	unsigned int num_frames;           
	unsigned int num_motion_channels;  
	float* data;                        
	unsigned* joint_channel_offsets;   

	MOTION()
	{
		num_frames = 0;
		num_motion_channels = 0;
		data = 0;
		joint_channel_offsets = 0;
	}
};

struct bvh_statistics
{
	unsigned int num_hierarchies;
	unsigned int num_total_joints;
	unsigned int num_motion_channels;
	unsigned int num_frames;

	bvh_statistics()
	{
		num_hierarchies = 0;
		num_total_joints = 0;
		num_motion_channels = 0;
		num_frames = 0;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class UniformBVH : public UniformAbstract
{
public:
	UniformBVH(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(816, 490), long style = wxTAB_TRAVERSAL, GLShaderRenderer *vShaderRenderer=0, ShaderView *vShaderView=0, UniformManager *vUniformManager=0);
	~UniformBVH();

	void UpdateValue(cUniformVariant vValue);
	void SetDatas(cUniformVariant vValue);
	bool LoadBvh(string vFilePathName);
	void clear(bool vDestroyClass);

private:
	JOINT* loadJoint(std::istream& stream, JOINT* parent = NULL);
	void loadHierarchy(std::istream& stream);
	void loadMotion(std::istream& stream);
	void testOutput() const;
	void printJoint(const JOINT* const joint) const;
	void moveJoint(JOINT* joint, MOTION* motionData, int frame_starts_index);
	void moveTo(unsigned frame);
	void deleteJoint(JOINT* joint, bool vOnlyRemoveUniformFromManager);
	const JOINT* getRootJoint() const { return rootJoint; }
	unsigned getNumFrames() const { return motionData.num_frames; }
	void bvh_to_vertices(JOINT* joint);
	void uploadForFrame(int frame);
	
	void InitDataViewCtrl();
	void UpdateDataViewCtrlForFrame(int vFrame);

	virtual string ReplaceUniformInCode(string vCode);
	virtual void ChangeUniformName(string vCurrentName, string vNewName);

private:
	JOINT* rootJoint;
	MOTION motionData;
	int m_CurrentFrame;

protected:
	virtual string Init(string vCodeSelection);
	virtual void SetShaderRenderer(GLShaderRenderer *vShaderRenderer);
	virtual string GetParamsToXMLString(DocumentFileFormat /*vNewFileFormat*/, DocumentFileFormat /*vOldFileFormat*/);
	virtual void SetParamsFromXML(map<string, string> vParams);
	virtual void OnTimer(wxTimerEvent& event);
	virtual void OnValueChange(wxCommandEvent& event);
	virtual void OnChangeSlideValue(wxScrollEvent& /*event*/);
	virtual void OnChangeTexBtn(wxCommandEvent& /*event*/);
	virtual void OnResetTexBtn(wxCommandEvent& /*event*/);

private:
	wxTimer	m_timer; // timer
	bool m_Modified;
	string m_FilePathName;
	wxObjectDataPtr<MyMusicTreeModel> m_music_model;

private:
	wxButton* m_button1;
	wxButton* m_button11;
	wxButton* m_button111;
	wxButton* m_button112;
	wxButton* m_button1121; 
	wxStaticText* m_staticText30;
	wxTextCtrl* m_textCtrl40;
	wxStaticLine* m_staticline7;
	wxSlider* m_slider6;
	wxDataViewCtrl* m_dataViewCtrl1;

	wxDECLARE_EVENT_TABLE();
};

#endif //__UNIFORMGAMEPAD360_H__