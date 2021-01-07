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

#include "UniformAbstract.h"
#include "DocShaderArt.h"

#include "MainFrame.h"

UniformAbstract::UniformAbstract(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, GLShaderRenderer *vShaderRenderer, ShaderView *vShaderView, UniformManager *vUniformManager)
: wxPanel(parent, id, pos, size, style), m_ShaderRenderer(vShaderRenderer), m_ShaderView(vShaderView), m_UniformManager(vUniformManager), m_value(), m_UniformViewItem(0), IsMergeInCodeAvailable(false), InitIsFailed(false)
{
	
}


UniformAbstract::~UniformAbstract()
{

}

void UniformAbstract::SetShaderRenderer(GLShaderRenderer *vShaderRenderer)
{
	m_ShaderRenderer = vShaderRenderer;
	//if (m_ShaderRenderer != 0) m_UniformManager = m_ShaderRenderer->GetUniformManager();
}

void UniformAbstract::SetShaderView(ShaderView* vShaderView)
{
	m_ShaderView = vShaderView;
}

void UniformAbstract::SetDocShaderArt(DocShaderArt* vDocShaderArt)
{
	m_DocShaderArt = vDocShaderArt;
}

void UniformAbstract::SetType(UniformsWidgetTypeEnum vType)
{
	m_type = vType;
}

UniformsWidgetTypeEnum UniformAbstract::GetType()
{
	return m_type;
}

void UniformAbstract::UpdateValue(cUniformVariant vValue)
{
	m_value = vValue;
}

cUniformVariant UniformAbstract::GetValue()
{
	return m_value;
}

void UniformAbstract::SetUniformViewItem(UniformViewItem *vUniformViewItem)
{
	m_UniformViewItem = vUniformViewItem;
}

void UniformAbstract::ChangeUniformName(string vCurrentName, string vNewName)
{
	if (m_mainUniformName == vCurrentName)
	{
		m_mainUniformName = vNewName;
	}
	else
	{
		string msg = "The Actual uniform name is :" + m_mainUniformName + " and not corresponding to " + vCurrentName + " !\n Cant replace !";
		g_pMainFrame->SetStatusBarText(msg, 0);
		wxMessageBox(msg, "FAIL !!");
		return;
	}
}

void UniformAbstract::UpdateUniformByName(string vUniformName, cUniformVariant vValue, bool vModified)
{
	if (m_UniformManager != 0)
	{
		string err = m_UniformManager->UpdateUniformByName(vUniformName, vValue, vModified);
		if (err.size() > 0)
		{
			string msg = err;
			g_pMainFrame->SetStatusBarText(msg, 0);
			wxMessageBox(msg, "FAIL !!");
			return;
		}
	}
	else
	{
		string msg = "Udpate of Uniform < " + vUniformName + " > Failed because can't accesing the Uniform Manager";
		g_pMainFrame->SetStatusBarText(msg, 0);
		wxMessageBox(msg, "FAIL !!");
		return;
	}
}