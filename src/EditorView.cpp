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

#include "EditorView.h"

#include "DocShaderArt.h"

//-----------------------------------------------------------------------------
// GlslEditor
//-----------------------------------------------------------------------------

#define STC_DEF_MARGIN_WIDTH 16
#define STC_DEF_INDENT_WIDTH 4

const int ANNOTATION_STYLE_ERROR = wxSTC_STYLE_LASTPREDEFINED + 1;
const int ANNOTATION_STYLE_WARNING = wxSTC_STYLE_LASTPREDEFINED + 2;

wxBEGIN_EVENT_TABLE(EditorView, wxPanel)
	EVT_STC_CHARADDED(ID_STYLED_TEXT_CTRL, EditorView::OnCharAdded)
	EVT_STC_CHANGE(ID_STYLED_TEXT_CTRL, EditorView::OnTextChange)
	EVT_CHOICE(ID_SHADER_GLSL_VERSION_COMBOBOX, EditorView::OnGLSLVersionChoice)
	EVT_TOGGLEBUTTON(ID_SHADER_SHOW_HIDE_HEADER_BTN, EditorView::OnShowHideHeader)
	EVT_CHOICE(ID_SHADER_FLOAT_PRECISION_COMBOBOX, EditorView::OnFloatPrecisionChoice)
	//EVT_STC_MARGINCLICK(ID_STYLED_TEXT_CTRL, EditorView::OnMarginClick)
	/*
	EVT_MENU(wxID_CLEAR, Edit::OnEditClear)
	EVT_MENU(wxID_CUT, Edit::OnEditCut)
	EVT_MENU(wxID_COPY, Edit::OnEditCopy)
	EVT_MENU(wxID_PASTE, Edit::OnEditPaste)
	EVT_MENU(myID_INDENTINC, Edit::OnEditIndentInc)
	EVT_MENU(myID_INDENTRED, Edit::OnEditIndentRed)
	EVT_MENU(wxID_SELECTALL, Edit::OnEditSelectAll)
	EVT_MENU(myID_SELECTLINE, Edit::OnEditSelectLine)
	EVT_MENU(wxID_REDO, Edit::OnEditRedo)
	EVT_MENU(wxID_UNDO, Edit::OnEditUndo)
	// find
	EVT_MENU(wxID_FIND, Edit::OnFind)
	EVT_MENU(myID_FINDNEXT, Edit::OnFindNext)
	EVT_MENU(myID_REPLACE, Edit::OnReplace)
	EVT_MENU(myID_REPLACENEXT, Edit::OnReplaceNext)
	EVT_MENU(myID_BRACEMATCH, Edit::OnBraceMatch)
	EVT_MENU(myID_GOTO, Edit::OnGoto)
	// view
	EVT_MENU_RANGE(myID_HILIGHTFIRST, myID_HILIGHTLAST,
	Edit::OnHilightLang)
	EVT_MENU(myID_DISPLAYEOL, Edit::OnDisplayEOL)
	EVT_MENU(myID_INDENTGUIDE, Edit::OnIndentGuide)
	EVT_MENU(myID_LINENUMBER, Edit::OnLineNumber)
	EVT_MENU(myID_LONGLINEON, Edit::OnLongLineOn)
	EVT_MENU(myID_WHITESPACE, Edit::OnWhiteSpace)
	EVT_MENU(myID_FOLDTOGGLE, Edit::OnFoldToggle)
	EVT_MENU(myID_OVERTYPE, Edit::OnSetOverType)
	EVT_MENU(myID_READONLY, Edit::OnSetReadOnly)
	EVT_MENU(myID_WRAPMODEON, Edit::OnWrapmodeOn)
	EVT_MENU(myID_CHARSETANSI, Edit::OnUseCharset)
	EVT_MENU(myID_CHARSETMAC, Edit::OnUseCharset)
	// annotations
	EVT_MENU(myID_ANNOTATION_ADD, Edit::OnAnnotationAdd)
	EVT_MENU(myID_ANNOTATION_REMOVE, Edit::OnAnnotationRemove)
	EVT_MENU(myID_ANNOTATION_CLEAR, Edit::OnAnnotationClear)
	EVT_MENU(myID_ANNOTATION_STYLE_HIDDEN, Edit::OnAnnotationStyle)
	EVT_MENU(myID_ANNOTATION_STYLE_STANDARD, Edit::OnAnnotationStyle)
	EVT_MENU(myID_ANNOTATION_STYLE_BOXED, Edit::OnAnnotationStyle)
	// extra
	EVT_MENU(myID_CHANGELOWER, Edit::OnChangeCase)
	EVT_MENU(myID_CHANGEUPPER, Edit::OnChangeCase)
	EVT_MENU(myID_CONVERTCR, Edit::OnConvertEOL)
	EVT_MENU(myID_CONVERTCRLF, Edit::OnConvertEOL)
	EVT_MENU(myID_CONVERTLF, Edit::OnConvertEOL)
	// stc
	EVT_STC_MARGINCLICK(wxID_ANY, Edit::OnMarginClick)
	EVT_STC_CHARADDED(wxID_ANY, Edit::OnCharAdded)
	EVT_STC_KEY(wxID_ANY, Edit::OnKey)
	*/
	//EVT_KEY_DOWN(EditorView::OnKeyDown)
wxEND_EVENT_TABLE()

const char* glslKeyWords =
"return if "
"float int bool vec2 vec3 vec4 ivec2 ivec3 ivec4 bvec2 bvec3 bvec4 "
"mat2 mat3 mat4 sampler1D sampler2D sampler3D samplerCube sampler1DShadow sampler2DShadow "
"void const attribute uniform varying for else while do-while do while break continue "
"in out inout radians degrees sin cos tan asin acos atan pow exp log exp2 log2 sqrt "
"inversesqrt abs sign floor ceil fract mod min max clamp mix step smoothstep length distance "
"dot cross normalize ftransform faceforward reflect refract matrixCompMult "
"lessThan lessThanEqual greaterThan greaterThanEqual "
"equal notEqual any all not texture1D texture1DProj texture1DLod "
"texture1DProjLod texture2D texture2DProj texture2DLod texture2DProjLod "
"texture3D texture3DProj texture3DLod texture3DProjLod "
"textureCube textureCubeLod shadow1D shadow2D "
"shadow1DProj shadow2DProj shadow1DLod shadow2DLod "
"shadow1DProjLod shadow2DProjLod dFdx dFdy fwidth noise1 noise2 noise3 noise4 "
"gl_FragColor gl_FragCoord ";

const char* glslFunctions =
"radians degrees sin cos tan asin acos atan pow exp log exp2 log2 sqrt "
"inversesqrt abs sign floor ceil fract mod min max clamp mix step smoothstep length distance "
"dot cross normalize ftransform faceforward reflect refract matrixCompMult "
"lessThan lessThanEqual greaterThan greaterThanEqual "
"equal notEqual any all not texture1D texture1DProj texture1DLod "
"texture1DProjLod texture2D texture2DProj texture2DLod texture2DProjLod "
"texture3D texture3DProj texture3DLod texture3DProjLod "
"textureCube textureCubeLod shadow1D shadow2D "
"shadow1DProj shadow2DProj shadow1DLod shadow2DLod "
"shadow1DProjLod shadow2DProjLod dFdx dFdy fwidth noise1 noise2 noise3 noise4 "
"gl_FragColor gl_FragCoord ";

const char* glslTypeAndInstructions =
"float int bool vec2 vec3 vec4 ivec2 ivec3 ivec4 bvec2 bvec3 bvec4 "
"mat2 mat3 mat4 sampler1D sampler2D sampler3D samplerCube sampler1DShadow sampler2DShadow "
"const uniform varying in out "
"return if else void const attribute for while do-while do while break continue ";

enum
{
	MARGIN_LINE_NUMBERS,
	MARGIN_DEBUG,
	MARGIN_FOLD
};

EditorView* EditorView::Create(DocShaderArt *vDocShaderArt, ShaderTypeEnum vShaderType, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
{
	EditorView *res = 0;

	if (parent != 0)
	{
		res = new EditorView(vDocShaderArt, vShaderType, parent, id, pos, size, style);
	}
	
	return res;
}

EditorView::EditorView(DocShaderArt *vDocShaderArt, ShaderTypeEnum vShaderType, wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style)
: wxPanel(parent, id, pos, size, style), m_DocShaderArt(vDocShaderArt), m_ShaderType(vShaderType), m_scintillaCode(0), m_scintillaHeader(0), m_IsInit(false)
{
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);
	
	/////////////////////////////////////////////////////////////////////////////////////////
	wxColour color = this->GetBackgroundColour();

	m_panel1 = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	m_panel1->SetBackgroundColour(color);
	wxBoxSizer* bSizerToolBar = new wxBoxSizer(wxHORIZONTAL);

	m_staticText1 = new wxStaticText(m_panel1, wxID_ANY, wxT("GLSL Vers :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizerToolBar->Add(m_staticText1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	m_choice1 = new wxChoice(m_panel1, ID_SHADER_GLSL_VERSION_COMBOBOX, wxDefaultPosition, wxDefaultSize);
	m_choice1->SetSelection(0);
	bSizerToolBar->Add(m_choice1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	m_choice2 = new wxChoice(m_panel1, ID_SHADER_FLOAT_PRECISION_COMBOBOX, wxDefaultPosition, wxDefaultSize);
	m_choice2->AppendString("highp");
	m_choice2->AppendString("mediump");
	m_choice2->AppendString("lowp");
	m_choice2->SetSelection(0);
	bSizerToolBar->Add(m_choice2, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	m_staticline1 = new wxStaticLine(m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL);
	bSizerToolBar->Add(m_staticline1, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	m_ShowHideHeaderBtn = new wxToggleButton(m_panel1, ID_SHADER_SHOW_HIDE_HEADER_BTN, wxT("Header"), wxDefaultPosition, wxDefaultSize);
	bSizerToolBar->Add(m_ShowHideHeaderBtn, 0, wxALIGN_CENTER_VERTICAL | wxALL, 1);

	m_panel1->SetSizer(bSizerToolBar);
	m_panel1->Layout();
	bSizerToolBar->Fit(m_panel1);
	bSizer2->Add(m_panel1, 0, wxEXPAND, 0);

	/////////////////////////////////////////////////////////////////////////////////////////
	m_scintillaHeader = CreateScintillaControl();
	m_scintillaHeader->Hide();
	bSizer2->Add(m_scintillaHeader, 0, wxEXPAND | wxALL, 4);

	m_scintillaCode = CreateScintillaControl();
	bSizer2->Add(m_scintillaCode, 1, wxEXPAND | wxALL, 4);
	
	this->SetSizer(bSizer2);
	this->Layout();
}

EditorView::~EditorView()
{

}

bool EditorView::Init()
{
	bool res = false;

	if (m_IsInit == false)
	{
		ShaderView *view = m_DocShaderArt->GetShaderView();
		if (view != 0)
		{
			// on initialise la liste des version de glsl
			// et cette liste est stocke dans la calsse qui gere opengl
			GLShaderRenderer *renderer = view->GetShaderRenderer();
			if (renderer != 0)
			{
				m_choice1->Clear();
				std::map<string, string> glslVersions = renderer->GetGlslVersionMap();
				for (std::map<string, string>::iterator it = glslVersions.begin(); it != glslVersions.end(); ++it)
				{
					string key = it->first;
					string value = it->second;
					m_choice1->AppendString(key);
				}

				m_choice1->SetSelection(0);

				m_IsInit = true;

				res = true;
			}
		}

		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			string version = m_choice1->GetStringSelection();
			doc->SetGlslVersion(version);
			string floatprecision = m_choice2->GetStringSelection();
			doc->SetFloatPrecision(floatprecision);
		}
	}
	else
		res = true;

	return res;
}

wxStyledTextCtrl* EditorView::CreateScintillaControl()
{
	wxStyledTextCtrl *scintilla = 0;

	scintilla = new wxStyledTextCtrl(this, ID_STYLED_TEXT_CTRL, wxDefaultPosition, wxDefaultSize, 0, wxEmptyString);

	scintilla->StyleClearAll();

	/*
	wxFont(int size,
	int family,
	int style,
	int weight,
	bool underlined = false,
	const wxString& face = wxEmptyString,
	wxFontEncoding encoding = wxFONTENCODING_DEFAULT)
	{
	(void)Create(size, (wxFontFamily)family, (wxFontStyle)style, (wxFontWeight)weight, underlined, face, encoding);
	}
	*/

	//wxFont font(12, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_NORMAL, false, "Arial", wxFontEncoding::wxFONTENCODING_UNICODE);/
	//wxFont font(wxSystemSettings::GetFont(wxSystemFont::wxSYS_SYSTEM_FONT));

	wxFont font(10, wxFontFamily::wxFONTFAMILY_MODERN, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD, false, wxT("Comic Sans MS"), wxFontEncoding::wxFONTENCODING_ISO8859_1);

	scintilla->SetFont(font);
	scintilla->StyleSetFont(wxSTC_STYLE_DEFAULT, font);

	scintilla->SetLexer(wxSTC_LEX_CPP);

	scintilla->StyleSetForeground(wxSTC_C_WORD, wxColour(0, 0, 150));
	//scintilla->StyleSetBold(wxSTC_C_WORD, true);
	scintilla->StyleSetFont(wxSTC_C_WORD, font);
	scintilla->SetKeyWords(0, glslFunctions);

	scintilla->StyleSetForeground(wxSTC_C_WORD2, wxColour(150, 0, 150));
	//scintilla->StyleSetBold(wxSTC_C_WORD2, true);
	scintilla->StyleSetFont(wxSTC_C_WORD2, font);
	scintilla->SetKeyWords(1, glslTypeAndInstructions);

	scintilla->StyleSetForeground(wxSTC_C_DEFAULT, wxColour(255, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_DEFAULT, true);
	scintilla->StyleSetFont(wxSTC_C_DEFAULT, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENT, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENT, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENT, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENTLINE, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENTLINE, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENTLINE, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENTDOC, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENTDOC, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENTDOC, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENTLINEDOC, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENTLINEDOC, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENTLINEDOC, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORD, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORD, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORD, font);

	scintilla->StyleSetForeground(wxSTC_C_COMMENTDOCKEYWORDERROR, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_COMMENTDOCKEYWORDERROR, true);
	scintilla->StyleSetFont(wxSTC_C_COMMENTDOCKEYWORDERROR, font);

	scintilla->StyleSetForeground(wxSTC_C_PREPROCESSORCOMMENT, wxColour(192, 124, 67));
	//scintilla->StyleSetBold(wxSTC_C_PREPROCESSORCOMMENT, true);
	scintilla->StyleSetFont(wxSTC_C_PREPROCESSORCOMMENT, font);

	scintilla->StyleSetForeground(wxSTC_C_NUMBER, wxColour(0, 100, 100));
	//scintilla->StyleSetBold(wxSTC_C_NUMBER, true);
	scintilla->StyleSetFont(wxSTC_C_NUMBER, font);

	//scintilla->StyleSetForeground(wxSTC_C_STRING, wxColour(0, 100, 150));
	//scintilla->StyleSetBold(wxSTC_C_STRING, true);
	scintilla->StyleSetFont(wxSTC_C_STRING, font);

	//scintilla->StyleSetForeground(wxSTC_C_CHARACTER, wxColour(0, 150, 150));
	//scintilla->StyleSetBold(wxSTC_C_CHARACTER, true);
	scintilla->StyleSetFont(wxSTC_C_CHARACTER, font);

	//scintilla->StyleSetForeground(wxSTC_C_UUID, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_UUID, true);
	scintilla->StyleSetFont(wxSTC_C_UUID, font);

	scintilla->StyleSetForeground(wxSTC_C_PREPROCESSOR, wxColour(150, 150, 50));
	//scintilla->StyleSetBold(wxSTC_C_PREPROCESSOR, true);
	scintilla->StyleSetFont(wxSTC_C_PREPROCESSOR, font);

	//scintilla->StyleSetForeground(wxSTC_C_OPERATOR, wxColour(0, 150, 150));
	//scintilla->StyleSetBold(wxSTC_C_OPERATOR, true);
	scintilla->StyleSetFont(wxSTC_C_OPERATOR, font);

	//scintilla->StyleSetForeground(wxSTC_C_IDENTIFIER, wxColour(0, 100, 150));
	//scintilla->StyleSetBold(wxSTC_C_IDENTIFIER, true);
	scintilla->StyleSetFont(wxSTC_C_IDENTIFIER, font);

	//scintilla->StyleSetForeground(wxSTC_C_STRINGEOL, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_STRINGEOL, true);
	scintilla->StyleSetFont(wxSTC_C_STRINGEOL, font);

	//scintilla->StyleSetForeground(wxSTC_C_VERBATIM, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_VERBATIM, true);
	scintilla->StyleSetFont(wxSTC_C_VERBATIM, font);

	//scintilla->StyleSetForeground(wxSTC_C_REGEX, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_REGEX, true);
	scintilla->StyleSetFont(wxSTC_C_REGEX, font);

	//scintilla->StyleSetForeground(wxSTC_C_GLOBALCLASS, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_GLOBALCLASS, true);
	scintilla->StyleSetFont(wxSTC_C_GLOBALCLASS, font);

	//scintilla->StyleSetForeground(wxSTC_C_STRINGRAW, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_STRINGRAW, true);
	scintilla->StyleSetFont(wxSTC_C_STRINGRAW, font);

	//scintilla->StyleSetForeground(wxSTC_C_TRIPLEVERBATIM, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_TRIPLEVERBATIM, true);
	scintilla->StyleSetFont(wxSTC_C_TRIPLEVERBATIM, font);

	//scintilla->StyleSetForeground(wxSTC_C_HASHQUOTEDSTRING, wxColour(0, 0, 0));
	//scintilla->StyleSetBold(wxSTC_C_HASHQUOTEDSTRING, true);
	scintilla->StyleSetFont(wxSTC_C_HASHQUOTEDSTRING, font);

	scintilla->SetUseTabs(true);
	scintilla->SetTabWidth(STC_DEF_INDENT_WIDTH);
	scintilla->SetIndent(STC_DEF_INDENT_WIDTH);
	scintilla->SetTabIndents(true);
	scintilla->SetBackSpaceUnIndents(true);
	scintilla->SetViewEOL(false);

	//scintilla->SetThemeEnabled(true);

	// pour eviter l'utilisation de \r\n comme sous windows. mieux vaut utiliser le style linux de juste \n
	// wxSTC_EOL_CRLF 0 / wxSTC_EOL_CR 1 / wxSTC_EOL_LF 2
	scintilla->SetEOLMode(wxSTC_EOL_LF);

	scintilla->SetViewWhiteSpace(false);
	scintilla->SetIndentationGuides(true);

	scintilla->SetMarginType(MARGIN_LINE_NUMBERS, wxSTC_MARGIN_NUMBER);
	scintilla->SetMarginWidth(MARGIN_LINE_NUMBERS, scintilla->TextWidth(wxSTC_STYLE_LINENUMBER, wxT("_9999")));

	scintilla->SetMarginType(MARGIN_DEBUG, wxSTC_MARGIN_SYMBOL);
	scintilla->SetMarginMask(MARGIN_DEBUG, wxSTC_MARK_CIRCLE);
	scintilla->SetMarginWidth(MARGIN_DEBUG, 16);
	scintilla->SetMarginSensitive(MARGIN_DEBUG, true);

	scintilla->SetMarginType(MARGIN_FOLD, wxSTC_MARGIN_SYMBOL);
	scintilla->SetMarginMask(MARGIN_FOLD, wxSTC_MASK_FOLDERS);
	scintilla->SetMarginWidth(MARGIN_FOLD, 16);
	scintilla->SetMarginSensitive(MARGIN_FOLD, true);

	scintilla->SetProperty(wxT("fold"), wxT("1"));
	scintilla->SetFoldFlags(wxSTC_FOLDFLAG_LINEBEFORE_CONTRACTED | wxSTC_FOLDFLAG_LINEAFTER_CONTRACTED);

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDER, wxSTC_MARK_BOXPLUS);
	scintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("BLACK")));
	scintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDER, wxColour(wxT("WHITE")));

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPEN, wxSTC_MARK_BOXMINUS);
	scintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("BLACK")));
	scintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPEN, wxColour(wxT("WHITE")));

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERSUB, wxSTC_MARK_EMPTY);

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEREND, wxSTC_MARK_BOXPLUS);
	scintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("BLACK")));
	scintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEREND, wxColour(wxT("WHITE")));

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDEROPENMID, wxSTC_MARK_BOXMINUS);
	scintilla->MarkerSetBackground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("BLACK")));
	scintilla->MarkerSetForeground(wxSTC_MARKNUM_FOLDEROPENMID, wxColour(wxT("WHITE")));

	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERMIDTAIL, wxSTC_MARK_EMPTY);
	scintilla->MarkerDefine(wxSTC_MARKNUM_FOLDERTAIL, wxSTC_MARK_EMPTY);

	scintilla->SetSelBackground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	scintilla->SetSelForeground(true, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));

	scintilla->SetProperty(wxT("fold.comment"), wxT("1"));

	scintilla->SetWrapMode(wxSTC_WRAP_WORD); // other choice is wxSCI_WRAP_NONE

	wxFont fontAnnotate(14, wxFontFamily::wxFONTFAMILY_SWISS, wxFontStyle::wxFONTSTYLE_NORMAL, wxFontWeight::wxFONTWEIGHT_BOLD, false, wxT("Comic Sans MS"), wxFontEncoding::wxFONTENCODING_ISO8859_1);

	// annotations style ERROR
	scintilla->StyleSetBackground(ANNOTATION_STYLE_ERROR, wxColour(244, 220, 220));
	scintilla->StyleSetForeground(ANNOTATION_STYLE_ERROR, wxColour(244, 50, 50));
	scintilla->StyleSetFont(ANNOTATION_STYLE_ERROR, fontAnnotate);
	scintilla->StyleSetSizeFractional(ANNOTATION_STYLE_ERROR, (scintilla->StyleGetSizeFractional(wxSTC_STYLE_DEFAULT) * 4) / 5);

	// annotations style WARNING
	scintilla->StyleSetBackground(ANNOTATION_STYLE_WARNING, wxColour(255, 230, 200));
	scintilla->StyleSetForeground(ANNOTATION_STYLE_WARNING, wxColour(255, 130, 0));
	scintilla->StyleSetFont(ANNOTATION_STYLE_WARNING, fontAnnotate);
	scintilla->StyleSetSizeFractional(ANNOTATION_STYLE_WARNING, (scintilla->StyleGetSizeFractional(wxSTC_STYLE_DEFAULT) * 4) / 5);


	scintilla->Connect(wxEVT_STC_MARGINCLICK, wxStyledTextEventHandler(EditorView::OnMarginClick), NULL, this);

	return scintilla;
}

/** Event callback when a margin is clicked, used here for code folding */
void EditorView::OnMarginClick(wxStyledTextEvent &event)
{
	if (event.GetMargin() == MARGIN_FOLD)
	{
		int lineClick = m_scintillaCode->LineFromPosition(event.GetPosition());
		int levelClick = m_scintillaCode->GetFoldLevel(lineClick);

		if ((levelClick & wxSTC_FOLDLEVELHEADERFLAG) > 0)
		{
			m_scintillaCode->ToggleFold(lineClick);
		}
	}
	else if (event.GetMargin() == MARGIN_DEBUG)
	{
		int lineClick = m_scintillaCode->LineFromPosition(event.GetPosition());

		m_scintillaCode->MarkerAdd(lineClick, wxSTC_MARK_CIRCLE);
	}
}

void EditorView::LoadFileMemoryToEditor()
{
	if (m_DocShaderArt != 0)
	{
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			//		int eolMode = m_scintillaCode->GetEOLMode();
			m_scintillaCode->SetText(doc->GetFileMemoryToString(m_ShaderType).c_str());
		}
	}
}

void EditorView::SaveEditorToFileMemory()
{
	if (m_DocShaderArt != 0)
	{
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
	//		int eolMode = m_scintillaCode->GetEOLMode();
			doc->SetStringToFileMemory(m_ShaderType, m_scintillaCode->GetText().ToStdString());
		}
	}
}

// marche que quand on ecrit ou quand on fait enter !!!
// marche pas quand on efface...
void EditorView::OnCharAdded(wxStyledTextEvent& event)
{
	char chr = (char)event.GetKey();
	int currentLine = m_scintillaCode->GetCurrentLine();
	// Change this if support for mac files with \r is needed
	if (chr == '\n') // enter 
	{
		int lineInd = 0;
		if (currentLine > 0) 
		{
			lineInd = m_scintillaCode->GetLineIndentation(currentLine-1);
		}
		if (lineInd == 0) return;
		m_scintillaCode->SetLineIndentation(currentLine, lineInd);
		m_scintillaCode->GotoPos(m_scintillaCode->PositionFromLine(currentLine) + lineInd / STC_DEF_INDENT_WIDTH);
	}
}

void EditorView::OnTextChange(wxStyledTextEvent &/*event*/)
{
	if (m_DocShaderArt != 0)
	{
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			// ca fait chier !!! c'est pas tres performant de faire ca
			string scintillaText = m_scintillaCode->GetText().ToStdString();
			string memoryText = doc->GetFileMemoryToString(m_ShaderType);

			// avant jke comparait au texte qui était sauvé en interne mais ca psait des porbleme
			// mioeux vaus activer le falg de modif des qu'il y a un truc qui bouge.
			// visual studi fait ca aussi meme si la mofig est un aller retour ( donc changement nul)
			doc->SetModified(true); // scintillaText != memoryText);
		}
	}
}

void EditorView::ShowErrorInCode(string vError, int vOffset, ShaderMsg vMsg)
{
	ShaderMsg typeMsg = vMsg;

	m_scintillaCode->AnnotationClearAll();
	m_scintillaCode->AnnotationSetVisible(wxSTC_ANNOTATION_HIDDEN);
	this->Refresh(true, 0);
	if (vError != "")
	{
		//if (typeMsg == ShaderMsg::SHADER_MSG_ERROR)
		//{
			this->SetBackgroundColour(wxColour(255, 100, 100));
		//}
		//if (typeMsg == ShaderMsg::SHADER_MSG_WARNING)
		//{
		//	this->SetBackgroundColour(wxColour(255, 182, 100));
		//}
		int lineIdx = 0;
		// on pete la ligne via les \n pour recuperer plusieurs lignes
		std::vector<std::string> vec = DebugLogger::splitString(vError, '\n');
		for (std::vector<std::string>::const_iterator i = vec.begin(); i != vec.end(); ++i)
		{
			string word = *i;
			if (word != L"")
			{
				string test = word.substr(0, 20);
				if (test.find("warning") != string::npos)
				{
					typeMsg = ShaderMsg::SHADER_MSG_WARNING;
				}
				else if (test.find("error") != string::npos)
				{
					typeMsg = ShaderMsg::SHADER_MSG_ERROR;
				}

				// on isole le numero de ligne de la string
				std::size_t p0 = word.find_first_of("0(") + 2;
				std::size_t p1 = word.find_first_of(")");
				string line = word.substr(p0, p1 - p0);
				// on tente de le convertir en nombre
				int lineNumber;
				stringstream ss = stringstream(line);
				ss >> lineNumber;
				if (ss)
				{
					lineNumber -= (vOffset + 2);
					if (lineNumber < 0) lineNumber = 0;
					// attention peut etre une exeption non traité si l'erreur est mal formatée
					string filteredErrorString = word.substr(p1 + 4);
					if (filteredErrorString != "")
					{
						// conversion reussi alors on ajoute l'annotation
						m_scintillaCode->AnnotationSetText(lineNumber, filteredErrorString);

						if (typeMsg == ShaderMsg::SHADER_MSG_ERROR)
						{
							m_scintillaCode->AnnotationSetStyle(lineNumber, ANNOTATION_STYLE_ERROR);
						}
						if (typeMsg == ShaderMsg::SHADER_MSG_WARNING)
						{
							m_scintillaCode->AnnotationSetStyle(lineNumber, ANNOTATION_STYLE_WARNING);
						}
					}
				}
				else
				{
					m_scintillaCode->AnnotationSetText(lineIdx, word);

					if (typeMsg == ShaderMsg::SHADER_MSG_ERROR)
					{
						m_scintillaCode->AnnotationSetStyle(lineIdx, ANNOTATION_STYLE_ERROR);
					}
					if (typeMsg == ShaderMsg::SHADER_MSG_WARNING)
					{
						m_scintillaCode->AnnotationSetStyle(lineIdx, ANNOTATION_STYLE_WARNING);
					}
					lineIdx++;
				}
			}
		}
		
		m_scintillaCode->AnnotationSetVisible(wxSTC_ANNOTATION_STANDARD);
	}
	else
	{
		this->SetBackgroundColour(wxColour(50, 200, 50));
	}
	this->Refresh(true, 0);
}

void EditorView::SetCode(string vCode)
{ 
	int scrolledLine = m_scintillaCode->GetFirstVisibleLine();
	int cursorAnchor = m_scintillaCode->GetAnchor();
	int cursorPos = m_scintillaCode->GetCurrentPos();
	
	m_scintillaCode->SetText(vCode);

	m_scintillaCode->SetFirstVisibleLine(scrolledLine);
	m_scintillaCode->SetAnchor(cursorAnchor);
	m_scintillaCode->SetCurrentPos(cursorPos);
}

void EditorView::DoAutoIndent()
{
	int scrolledLine = m_scintillaCode->GetFirstVisibleLine();
	int cursorAnchor = m_scintillaCode->GetAnchor();
	int cursorPos = m_scintillaCode->GetCurrentPos();

	int tabwidth = m_scintillaCode->GetTabWidth();

//	int lastIndent = 0;

	string file = m_scintillaCode->GetText().ToStdString();
	string tab = ""; for (int i = 0; i < tabwidth; i++)tab += " ";

	//DebugLogger::ReplaceString(file, "\t", tab);
	/*DebugLogger::ReplaceString(file, "){\n", ")\n{\n");
	DebugLogger::ReplaceString(file, "{ \n", "{\n");
	DebugLogger::ReplaceString(file, "{  \n", "{\n");
	DebugLogger::ReplaceString(file, "{   \n", "{\n");
	DebugLogger::ReplaceString(file, "\n\n{\n", "{\n");*/
	
	m_scintillaCode->SetText(file);

	int accoladeLevel = 0;
	int returnLevel = 0;
	int commentLevel = 0;
//	bool CommentState = false;

	size_t pos = 0;
	string currentline;
	string nextline;
	string lastline;
	string lastline_firstTwoChars;
	string lastline_lastTwoChars;
	string firstTwoChars;
	string lastTwoChars;
	string lastThreeChars;
	string firstChar;
	string subline;

	// il faut parcourir toutes les lignes du fichier
	int nLine = m_scintillaCode->GetLineCount();
	for (int i = 0; i < nLine; i++)
	{
	//	if (i == 68)
	//		i = 68;
		try
		{
			currentline = m_scintillaCode->GetLine(i);

			pos = currentline.find_first_of("abcdefghijklmnopqrstuvwxyz0123456789.*_=-+?:;({})/#");
			if (pos != std::string::npos)
			{
				subline = currentline.substr(pos); // pos to end right

				if (subline.size() >= 1)
					firstChar = subline.substr(0, 1);

				if (subline.size() >= 2) 
					firstTwoChars = subline.substr(0, 2);

				if (firstTwoChars == "/*")
					commentLevel++;
				else
				{
					pos = subline.find("/*");
					if (pos != std::string::npos)
					{
						commentLevel++;
					}
				}

				if (firstTwoChars != "//" && firstChar != "#" && commentLevel == 0)
				{
					DebugLogger::ReplaceString(subline, " ", "");
					DebugLogger::ReplaceString(subline, "\t", "");

					if (currentline.find_first_of("}") != std::string::npos)
					{
						if (accoladeLevel > 0)
							accoladeLevel--;
						m_scintillaCode->SetLineIndentation(i, tabwidth * (accoladeLevel + returnLevel));

					}
					else if (currentline.find_first_of("{") != std::string::npos)
					{
						m_scintillaCode->SetLineIndentation(i, tabwidth * (accoladeLevel + returnLevel));
						accoladeLevel++;
					}
					else
					{
						if (subline == ");\n" || subline == ";\n")
						{
							if (returnLevel > 0)
								returnLevel = 0;
						}

						m_scintillaCode->SetLineIndentation(i, tabwidth * (accoladeLevel + returnLevel));

						// si le // ets contenu avant la fin de phrase alors on dit que la phrase est ce qui est avant le //
						pos = subline.rfind("//");
						if (pos != std::string::npos)
						{
							if (subline.size() >= pos)
								subline = subline.substr(0, pos) + "\n";
						}

						pos = subline.rfind("/*");
						if (pos != std::string::npos)
						{
							commentLevel++;
							if (subline.size() >= pos)
								subline = subline.substr(0, pos) + "\n";
						}

						int sublineSize = subline.size();

						if (sublineSize >= 2)
							lastTwoChars = subline.substr(subline.length() - 2);

						if (sublineSize >= 3) 
							lastThreeChars = subline.substr(subline.length() - 3);

						if (lastTwoChars != ";\n" && lastTwoChars != ")\n" && lastTwoChars != "}\n" && lastThreeChars != "*/\n") // se => else
						{
							if (i + 1 < nLine)
							{
								nextline = m_scintillaCode->GetLine(i + 1);
								DebugLogger::ReplaceString(nextline, " ", "");
								DebugLogger::ReplaceString(nextline, "\t", "");
								if (nextline.substr(0, 1) != "{" && nextline.substr(0, 2) != "if")
									returnLevel = 1;
							}
						}
						if (lastTwoChars == ";\n" || lastTwoChars == ")\n" || lastTwoChars == "}\n")
						{
							if (subline.find_first_of(",+-*/") != 0)
							{
								if (returnLevel > 0)
									returnLevel = 0;
							}
						}
						if (firstTwoChars == "if" && lastTwoChars == ")\n")
						{
							if (i + 1 < nLine)
							{
								nextline = m_scintillaCode->GetLine(i+1);
								DebugLogger::ReplaceString(nextline, " ", "");
								DebugLogger::ReplaceString(nextline, "\t", "");
								if (nextline.substr(0, 1) != "{")
									returnLevel = 1;
							}
						}
					}
				}
				
				// preporc on les met sur le 0
				if (firstChar == "#" )
					m_scintillaCode->SetLineIndentation(i, 0);

				if (firstTwoChars == "*/")
					commentLevel--;
				else
				{
					pos = subline.find("*/");
					if (pos != std::string::npos)
					{
						commentLevel--;
					}
				}

				lastline_firstTwoChars = firstTwoChars;
				lastline_lastTwoChars = lastTwoChars;
			}

			lastline = currentline;
			
		}
		catch (std::exception &err)
		{
			LogStr("Exception in EditorView::DoAutoIndent() :" + toStr(err.what()));
		}
	}

	m_scintillaCode->SetFirstVisibleLine(scrolledLine);
	m_scintillaCode->SetAnchor(cursorAnchor);
	m_scintillaCode->SetCurrentPos(cursorPos);
}

void EditorView::SetVisibleWord(int vCharStart, int vCountChar)
{
	m_scintillaCode->SetCurrentPos(vCharStart);
	m_scintillaCode->SetAnchor(vCharStart + vCountChar);
	m_scintillaCode->SetFirstVisibleLine(m_scintillaCode->GetCurrentLine());
}

string EditorView::GetSelection()
{
	string res;

	int cursorAnchor = m_scintillaCode->GetAnchor(); // start
	int cursorPos = m_scintillaCode->GetCurrentPos(); // end

	res = m_scintillaCode->GetTextRange(cursorAnchor, cursorPos);

	return res;
}

void EditorView::ReplaceSelectionBy(string vReplaceString)
{
	m_scintillaCode->ReplaceSelection(vReplaceString);
}

void EditorView::DoAnalyseShaderCode()
{
	string txt = m_scintillaCode->GetText();
	
	//CodeAnalysis codeAnalyser;

	//codeAnalyser.AnalyseCode(txt);
}

void EditorView::OnGLSLVersionChoice(wxCommandEvent& /*event*/)
{
	if (m_DocShaderArt != 0)
	{
		string version = m_choice1->GetStringSelection();
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			doc->SetGlslVersion(version);
			m_DocShaderArt->CompilAndRunShader(false);
			doc->SetModified(true);
		}
	}
}

void EditorView::OnFloatPrecisionChoice(wxCommandEvent& /*event*/)
{
	if (m_DocShaderArt != 0)
	{
		string floatprecision = m_choice2->GetStringSelection();
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			doc->SetFloatPrecision(floatprecision);
			m_DocShaderArt->CompilAndRunShader(false);
			doc->SetModified(true);
		}
	}
}

void EditorView::OnShowHideHeader(wxCommandEvent& /*event*/)
{
	UpdateHeaderText();

	if (m_ShowHideHeaderBtn->GetValue() == true)
	{
		m_scintillaHeader->Show(true);
		m_scintillaHeader->Update();
	}
	else
	{
		m_scintillaHeader->Hide();
	}

	this->Layout();
}

void EditorView::UpdateGlslVersionAndFloatPrecisionComboBoxs()
{
	if (m_DocShaderArt != 0)
	{
		ShaderArtDocument *doc = m_DocShaderArt->GetDocument();
		if (doc != 0)
		{
			string glsl = doc->GetGlslVersion();
			m_choice1->SetStringSelection(glsl);
			string prec = doc->GetFloatPrecision();
			m_choice2->SetStringSelection(prec);
		}
	}
}

void EditorView::UpdateHeaderText()
{
	if (m_DocShaderArt != 0)
	{
		ShaderView *view = m_DocShaderArt->GetShaderView();
		if (view != 0)
		{
			GLShaderRenderer *renderer = view->GetShaderRenderer();
			if (renderer != 0)
			{
				string header = renderer->GetLastHeader();
				m_scintillaHeader->SetReadOnly(false); // si je fait pas ca curieusement le remplacement de text ne marche pas 
				m_scintillaHeader->SetText(header.c_str());
				m_scintillaHeader->SetReadOnly(true);
				m_scintillaHeader->Update();
			}
		}
	}
}

void EditorView::ShowHideToolBar(bool vMode)
{
	m_panel1->Show(vMode);
}

// vMayWizzled a true eprmet de trouver les therme wizzled tu style uSlider.xy alors qu'a la base on chercher uSlider.x ou uSlider.y
vector<std::string::size_type> EditorView::GetOccurencesOfWord(string vWord, bool vMayWizzled)
{
	vector<std::string::size_type> res;
	string code = this->GetCode();
	size_t pos = 0;

	// on doit separer le nom de son wizzle
	vector<string> vec = DebugLogger::splitString(vWord, '.');
	
	if (vMayWizzled == true && vec.size() == 2)
	{
		// vMayWizzled == true
		// deux cas
		// on cherche uSlider.x et on a uSlider.xy
		// on cherche uSlider.y et on uSlider.xy
		string name = vec[0];
		string wizzle = vec[1];
		if (wizzle.size() == 1)
		{
			while ((pos = code.find(name, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
			{
				string num = code.substr(pos + name.length(), 1); // 2
				size_t pos2 = num.find_first_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
				if (pos2 != 0) // c'est bien celui qu'on cherche
				{
					// on va tenter de determiner les wizzle qu'il y a derriere
					size_t endWordPos = pos + name.length();
					if (code[endWordPos] == '.') // wizzled
					{
						num = code.substr(endWordPos+1, 4); // .xyzw
						size_t pos3 = num.find_first_not_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
						float len = pos3;
						if (len > 0) // xy
						{
							string wizzler = num.substr(0, len);
							// on va iteer pour trouver notre wizzle
							for (int i = 0; i < len; i++)
							{
								if (wizzler[i] == wizzle[0])
									res.push_back(pos);
							}
						}
					}
				}
				pos += vWord.length();
			}
		}
	}
	else
	{
		while ((pos = code.find(vWord, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
		{
			string num = code.substr(pos + vWord.length(), 1); // 2
			size_t pos2 = num.find_first_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
			if (pos2 == std::string::npos) // c'est bien celui qu'on cherche
			{
				res.push_back(pos);
			}
			pos += vWord.length();
		}
	}
	
	return res;
}

void EditorView::ReplaceOccurenceOfWordBy(string vCurrentWord, string vNewWord, bool vMayWizzled)
{
	string code = this->GetCode();
	size_t pos = 0;

	// on doit separer le nom de son wizzle
	vector<string> vec = DebugLogger::splitString(vCurrentWord, '.');

	if (vMayWizzled == true && vec.size() == 2)
	{
		// vMayWizzled == true
		// deux cas
		// on cherche uSlider.x et on a uSlider.xy
		// on cherche uSlider.y et on uSlider.xy
		string name = vec[0];
		string wizzle = vec[1];
		if (wizzle.size() == 1)
		{
			while ((pos = code.find(name, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
			{
				string newStr;
				string num = code.substr(pos + name.length(), 1); // 2
				size_t pos2 = num.find_first_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
				if (pos2 != 0) // c'est bien celui qu'on cherche
				{
					size_t endWordPos = pos + name.length();
					if (code[endWordPos] == '.') // wizzled
					{
						num = code.substr(endWordPos + 1, 4); // .xyzw
						size_t pos3 = num.find_first_not_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
						float len = pos3;
						if (len > 0) // xy
						{
							string wizzler = num.substr(0, len);
							if (wizzler.find_first_of(wizzle) != string::npos)
							{
								if (len == 2) newStr = "vec2(";
								if (len == 3) newStr = "vec3(";
								if (len == 4) newStr = "vec4(";

								// on va iteer pour trouver notre wizzle
								for (int i = 0; i < len; i++)
								{
									if (i > 0)
										newStr += ",";

									if (wizzler[i] == wizzle[0]) // wizzle trouvé
									{
										newStr += toStr(vNewWord);
									}
									else // wizzle non trouvé
									{
										newStr += name + "." + wizzler[i];
									}

									if (i == len - 1 && len > 1)
										newStr += ")";
								}

								string currentNameWizzled = name + "." + wizzler;
								code.replace(pos, currentNameWizzled.length(), newStr);
							}
						}
					}
				}
				if (newStr.size() > 0)
					pos += newStr.length();
				else
					pos += vNewWord.length();
			}
		}
	}
	else
	{
		while ((pos = code.find(vCurrentWord, pos)) != std::string::npos) // name trouvé on va check si existence d'un chiffre derriere qui ferait dire que c'est un autre uniform que celui qu'on cherche
		{
			string num = code.substr(pos + vCurrentWord.length(), 1); // 2
			size_t pos2 = num.find_first_of("abcdefghijklmnopqrstuvxyzwABSDEFGHIJKLMNOPQRSTUVWXYZ1234567890.");
			if (pos2 == std::string::npos) // c'est bien celui qu'on cherche
			{
				code.replace(pos, vCurrentWord.length(), vNewWord);
			}
			pos += vNewWord.length();
		}
	}

	this->SetCode(code);
}