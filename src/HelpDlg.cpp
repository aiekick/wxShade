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

#include "HelpDlg.h"

#include "UniformView.h"

#include "UniformManager.h"

#include "EditorView.h"

#include "DocShaderArt.h"

HelpDlg::HelpDlg(wxWindow* parent, DocShaderArt *vDocShaderArt, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxDialog(parent, id, title, pos, size, style), m_DocShaderArt(vDocShaderArt)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxVERTICAL);

	m_listbook2 = new wxListbook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLB_DEFAULT);
	GLShaderRenderer *renderer = vDocShaderArt->GetShaderView()->GetShaderRenderer();
	string content;
	/////////////////////////////////////////////////////////////////////
	///////// GENERAL ///////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	
	content = "<html><body bgcolor=\"#FFFFFF\"><table cellspacing=3 cellpadding=4 width=\"100%\">";
	content += "<tr>";
	content += "<td bgcolor = \"#101010\"><center><font size = +2 color = \"#FFFFFF\"><b><br>Driver Infos :<br>< / b></font></center></td>";
	content += "</tr><tr><td bgcolor = \"#73A183\">";
	content += "<font size = -2><table cellpadding = 0 cellspacing = 0 width = \"100%\">";

	if (renderer != 0)
	{
		content += "<tr><td width = \"65%\"> GL_MAJOR_VERSION : " + renderer->MapParams["GL_MAJOR_VERSION"] + "< / td>< / tr>";
		content += "<tr><td width = \"65%\"> GL_MINOR_VERSION : " + renderer->MapParams["GL_MINOR_VERSION"] + "</td></tr>";
		content += "<tr><td width = \"65%\"> GL_VENDOR : " + renderer->MapParams["GL_VENDOR"] + "</td></tr>";
		content += "<tr><td width = \"65%\"> GL_RENDERER : " + renderer->MapParams["GL_RENDERER"] + "</td></tr>";
		content += "<tr><td width = \"65%\"> GL_GLSL_VERSION_BASE : " + renderer->MapParams["GL_GLSL_VERSION_BASE"] + "</td></tr>";
	}
	else
	{
		content += "<tr><td width = \"65%\">Can't Retrieve Infos</td></tr>";
	}

	content += "</td></tr></table></font></td>";
	content += "</tr></table></body></html>";

	AddPaneHtml("Driver", content);

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////
	///////// Distance Field Shape //////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	content = "<html><body bgcolor=\"#FFFFFF\"><table cellspacing=3 cellpadding=4 width=\"100%\">";
	content += "<tr>";
	content += "<td bgcolor = \"#101010\"><center><font size = +3 color = \"#FFFFFF\"><b><br>Distance Fields Infos :<br>< / b></font></center></td>";
	content += "</tr><tr><td bgcolor = \"#73A183\">";
	content += "<font size = -2><table cellpadding = 0 cellspacing = 0 width = \"100%\">";

	content += "<tr><td bgcolor = \"#73A183\" width = \"65%\"><b>This list come from the <a href=\"http://iquilezles.org/www/articles/distfunctions/distfunctions.htm\">Inigo Quilez Blog Page</a></b><p></td></tr>";

	content += "<tr><td bgcolor = \"#73A183\" width = \"80%\"><font size = +2><b><u>Shapes :</u></b></font><p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Params are : </b></font>vec3 p => Intersect Point / float r => Radius / vec3 c => Size<p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Sphere : </b></font>return length(p)-r</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Box : </b></font>vec3 d = abs(p)-c; return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Infinite Cyclinder Y axis : </b></font>return length(p.xz-c.xy)-c.z;</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Cone Z Axis: </b></font>float q = length(p.xy); return dot(c.xy, vec2(q, p.z));</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Plane Z Axis: </b></font>(n is normalized) return dot(p,n.xyz) + n.w;</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Capped Cylinder Y Axis: </b></font>vec2 d = abs(vec2(length(p.xz), p.y)) - c.xz; return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Torus Y Axis: </b></font>vec2 q = vec2(length(p.xz)-t.x,p.y); return length(q) - t.y; <p></td></tr>";

	content += "<tr><td bgcolor = \"#73A183\" width = \"80%\"><font size = +2><b><u>Booleans Op :</u></b></font><p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Params are : </b></font>Two Distance Fiels Objects Names df1 and df2<p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Union : </b></font>min(df1, df2)</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Substract : </b></font>max(-df1, df2)</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Intersection : </b></font>max(df1, df2)<p></td></tr>";

	content += "<tr><td bgcolor = \"#73A183\" width = \"80%\"><font size = +2><b><u>Smooth Booleans Op :</u></b></font><p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Params are : </b></font>Two Distance Fiels Objects Names df1 and df2 / float k is the smooth radius<p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Smooth Union Exponential : </b></font>float res = exp( -k*df1 ) + exp( -k*df2 ); return -log(res) / k; </td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Smooth Union polynomial : </b></font>float h = clamp( 0.5+0.5*(df2-df1)/k, 0.0, 1.0 ); return mix(df2, df1, h) - k*h*(1.0 - h); </td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Smooth Union power :</b> </font>df1 = pow( df1, k ); df2 = pow( df2, k ); return pow((df1*df2) / (df1 + df2), 1.0 / k); <p></td></tr>";

	content += "<tr><td bgcolor = \"#73A183\" width = \"80%\"><font size = +2><b><u>Domain Op :</u></b></font><p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Params are : </b></font>vec3 q => is the repeat of p (Intersect Point on DF) / vec3 c => Size<p></td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Repeat by Mod : </b></font>vec3 q = mod(q,c) - c/2.;</td></tr>";
	content += "<tr><td width = \"65%\"><font size = +1><b>Repeat by Cos or Sin : </b></font>vec3 q = cos(q)*c or sin(q)*c;</td></tr>";

	content += "</td></tr></table></font></td>";
	content += "</tr></table></body></html>";

	AddPaneHtml("Dist. Fields", content);

	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////

	bSizer1->Add(m_listbook2, 1, wxEXPAND | wxALL, 5);

	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);


}

HelpDlg::~HelpDlg()
{
	

}

void HelpDlg::AddPaneHtml(string vTabName, string vHtmlString)
{
	wxPanel *pane = new wxPanel(m_listbook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxHtmlWindow *html = new wxHtmlWindow(pane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxHW_SCROLLBAR_AUTO);

	html->SetBorders(0);

	html->AppendToPage(vHtmlString);
	html->SetSize(
		html->GetInternalRepresentation()->GetWidth(),
		html->GetInternalRepresentation()->GetHeight());

	sizer->Add(html, 1, wxALL | wxEXPAND, 5);

	pane->SetSizer(sizer);
	pane->Layout();
	sizer->Fit(pane);

	m_listbook2->AddPage(pane, vTabName, false);
}