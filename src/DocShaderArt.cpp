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

#include "DocShaderArt.h"

#include "MainFrame.h"

#include "UniformTime.h"

wxBEGIN_EVENT_TABLE(DocShaderArt, wxPanel)
	EVT_SIZE(DocShaderArt::OnSize)
	EVT_BUTTON(ID_PERF_BTN, DocShaderArt::DoSwitchPerfMode)
	EVT_AUINOTEBOOK_PAGE_CLOSE(ID_SHADER_NOTEBOOK_EDITOR, DocShaderArt::OnPageClose)
	EVT_AUINOTEBOOK_PAGE_CLOSED(ID_SHADER_NOTEBOOK_EDITOR, DocShaderArt::OnPageClosed)
wxEND_EVENT_TABLE()

DocShaderArt::DocShaderArt(wxWindow* parent, ShaderArtTypeEnum vShaderArtType, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, string vFileName)
: DocVisualControlAbstract(parent, id, pos, size, style), m_IsInit(false), m_EditorFragmentView(0), m_ShaderView(0), m_UniformView(0), 
m_auinotebookEditor(0), m_EditorVertexView(0), m_EditorMusicView(0), m_ShaderViewToolBar(0)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	m_Uniforms_Mgr = new UniformManager(this);
	
	m_Document = new ShaderArtDocument(vFileName, m_Uniforms_Mgr);
	m_Document->SetShaderArtType(vShaderArtType);

	wxBoxSizer* bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_auinotebook3 = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
		wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TOP | wxAUI_NB_WINDOWLIST_BUTTON);
	
	m_auinotebookEditor = new wxAuiNotebook(m_auinotebook3, ID_SHADER_NOTEBOOK_EDITOR, wxDefaultPosition, wxDefaultSize,
		wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_TAB_EXTERNAL_MOVE | wxAUI_NB_TAB_MOVE | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TOP | wxAUI_NB_WINDOWLIST_BUTTON/* | wxAUI_NB_CLOSE_ON_ALL_TABS*/);

	m_auinotebook3->AddPage(m_auinotebookEditor, wxT("Editor"), false, wxNullBitmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (vShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
	{
		m_EditorVertexView = EditorView::Create(this, ShaderTypeEnum::SHADER_TYPE_VERTEX, m_auinotebookEditor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		if (m_EditorVertexView != 0)
			m_auinotebookEditor->AddPage(m_EditorVertexView, wxT("Vertex"), false, wxNullBitmap);
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	m_EditorFragmentView = EditorView::Create(this, ShaderTypeEnum::SHADER_TYPE_FRAGMENT, m_auinotebookEditor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	if (m_EditorFragmentView != 0)
		m_auinotebookEditor->AddPage(m_EditorFragmentView, wxT("Fragment"), false, wxNullBitmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_shaderpane = new wxPanel(m_auinotebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer1 = new wxBoxSizer(wxVERTICAL);
	m_shaderpane->SetSizer(bSizer1);

	m_ShaderView = ShaderView::Create(this, m_shaderpane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, m_Document->GetFileName());
	if (m_ShaderView != 0)
	{
		m_ShaderView->SetRenderMode(m_Document->GetRenderMode());

		m_ShaderViewToolBar = new ShaderViewToolBar(m_ShaderView, m_shaderpane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		if (m_ShaderViewToolBar->Init() == true)
		{
			bSizer1->Add(m_ShaderViewToolBar, 0, wxEXPAND, 0);
		}
		
		bSizer1->Add(m_ShaderView, 1, wxEXPAND, 0);
	}
	
	m_shaderpane->Layout();

	m_auinotebook3->AddPage(m_shaderpane, wxT("Shader"), true, wxNullBitmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	m_uniformpane = new wxPanel(m_auinotebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* bSizer3 = new wxBoxSizer(wxVERTICAL);
	m_uniformpane->SetSizer(bSizer3);

	m_UniformView = UniformView::Create(this, m_uniformpane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, m_Uniforms_Mgr);
	if (m_UniformView != 0)
	{
		m_UniformViewToolBar = new UniformViewToolBar(m_UniformView, m_uniformpane, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
		if (m_UniformViewToolBar->Init() == true)
		{
			bSizer3->Add(m_UniformViewToolBar, 0, wxEXPAND, 0);
		}

		bSizer3->Add(m_UniformView, 1, wxEXPAND, 0);
	}
	
	m_uniformpane->Layout();

	m_auinotebook3->AddPage(m_uniformpane, wxT("Uniforms"), false, wxNullBitmap);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// en majuscule les wxBOTTOM / wxLEFT / wxTOP / wxRIGHT sinon ca merde vu que c'est pas les bon symboles contrairement a ce que dit la doc !!!

	bSizer2->Add(m_auinotebook3, 1, wxEXPAND, 5);

	this->SetSizer(bSizer2);

	this->Layout();

	Init();

	EditorViewSize = cVec2(50, 100); // size in percent in each split
	ShaderViewSize = cVec2(50, 50); // size in percent in each split
	UniformViewSize = cVec2(50, 50); // size in percent in each split

	m_DefaultLayout = ShaderArtLayoutEnum::ER_STL_UBL;
}

DocShaderArt::~DocShaderArt()
{
	m_auinotebook3->DeletePage(m_auinotebook3->GetPageIndex(m_uniformpane));
	if (m_EditorVertexView != 0) 
		m_auinotebook3->DeletePage(m_auinotebook3->GetPageIndex(m_EditorVertexView));
	if (m_EditorMusicView != 0) 
		m_auinotebook3->DeletePage(m_auinotebook3->GetPageIndex(m_EditorMusicView));
	m_auinotebook3->DeletePage(m_auinotebook3->GetPageIndex(m_EditorFragmentView));
	m_auinotebook3->DeletePage(m_auinotebook3->GetPageIndex(m_shaderpane)); // le shader en dernier car il contient GLSahderRenderer qui est pointé par Uniformview
	SAFE_DELETE(m_Uniforms_Mgr);
}

bool DocShaderArt::Init()
{
	if (m_IsInit == true)
		return false;

	if (m_Document == 0)
		return false;

	if (m_EditorFragmentView == 0)
		return false;

	ShaderArtTypeEnum m_ShaderArtType = m_Document->GetShaderArtType();

	if ((m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS && m_EditorVertexView->Init() == true && m_EditorFragmentView->Init() == true) ||
		(m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD && m_EditorFragmentView->Init() == true))
	{
		DoAutoLayout(m_DefaultLayout); // last layout in config file a charger ici pour plus tard

		int w, h;
		m_ShaderView->GetSize(&w, &h);
		if (w>10 && h>10)
			m_IsInit = true;

		m_ShaderView->Refresh(false, 0);
	}

	return false;
}

// declenche la compilation du shader
void DocShaderArt::CompilAndRunShader(bool vPerfAnalysisMode)
{
	EditorView *view = static_cast<EditorView*>(m_auinotebookEditor->GetCurrentPage());
	if (view != 0)
	{
		if (view->GetShaderType() == ShaderEditorTemplateEnum::TEMPLATE_QUAD || view->GetShaderType() == ShaderEditorTemplateEnum::TEMPLATE_POINTS)
		{
			// validation avec glslang
			string output = ValidateWithGlslang();

			string fragment;
			string vertex;

			// pour si on a le shader de type son  d'ouvert

			if (m_EditorFragmentView != 0)
			{
				fragment = m_EditorFragmentView->GetCode();
			}

			ShaderArtTypeEnum m_ShaderArtType = m_Document->GetShaderArtType();

			if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS && m_EditorVertexView != 0)
			{
				vertex = m_EditorVertexView->GetCode();
			}
			else
			{
				vertex = m_Document->GetFileMemoryToString(ShaderTypeEnum::SHADER_TYPE_VERTEX);
			}

			if (fragment.size() > 0 && vertex.size() > 0)
			{
				ComplexStringStruct uniforms = m_Uniforms_Mgr->GetUniformsToString();
				if (m_ShaderView->CompileAndUseVertexAndFragment(uniforms, vertex, fragment, vPerfAnalysisMode) == false)
				{
					// mode debug
				}
				else
				{

				}

				m_EditorFragmentView->UpdateHeaderText();
				m_UniformView->CheckUniformWidgetCurrentlyUsed(); // on verifie les uniform utilisé ou non

				//&g_pOutputWin->AddToOutput("Glslang Check :");
				std::vector<std::string> vec = DebugLogger::splitString(output, '\n');
				for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
				{
					string s = *it;
					g_pOutputWin->AddToOutput(s);
				}
			}
			else
			{
				// error
			}
		}
		else if (view->GetShaderType() == ShaderEditorTemplateEnum::TEMPLATE_MUSIC)
		{

		}
	}
}

string DocShaderArt::ValidateWithGlslang()
{
	string res;
	/*
	m_EditorFragmentView = static_cast<EditorView*>(m_auinotebookEditor->GetCurrentPage());
	if (m_EditorFragmentView != 0)
	{
		int Options = 0;
		ShHandle compiler = ShConstructCompiler(EShLanguage::EShLangFragment, Options);
		if (compiler != 0)
		{
			// Config used for validation
			TBuiltInResource Resources;
			
			char* config = new char[strlen(DefaultConfig) + 1];
			strcpy(config, DefaultConfig);

			const char* delims = " \t\n\r";
			const char* token = strtok(config, delims);
			while (token) 
			{
				const char* valueStr = strtok(0, delims);
				if (valueStr == 0 || !(valueStr[0] == '-' || (valueStr[0] >= '0' && valueStr[0] <= '9'))) 
				{
					printf("Error: '%s' bad .conf file.  Each name must be followed by one number.\n", valueStr ? valueStr : "");
					return res;
				}

				int value = atoi(valueStr);

				if (strcmp(token, "MaxLights") == 0)
					Resources.maxLights = value;
				else if (strcmp(token, "MaxClipPlanes") == 0)
					Resources.maxClipPlanes = value;
				else if (strcmp(token, "MaxTextureUnits") == 0)
					Resources.maxTextureUnits = value;
				else if (strcmp(token, "MaxTextureCoords") == 0)
					Resources.maxTextureCoords = value;
				else if (strcmp(token, "MaxVertexAttribs") == 0)
					Resources.maxVertexAttribs = value;
				else if (strcmp(token, "MaxVertexUniformComponents") == 0)
					Resources.maxVertexUniformComponents = value;
				else if (strcmp(token, "MaxVaryingFloats") == 0)
					Resources.maxVaryingFloats = value;
				else if (strcmp(token, "MaxVertexTextureImageUnits") == 0)
					Resources.maxVertexTextureImageUnits = value;
				else if (strcmp(token, "MaxCombinedTextureImageUnits") == 0)
					Resources.maxCombinedTextureImageUnits = value;
				else if (strcmp(token, "MaxTextureImageUnits") == 0)
					Resources.maxTextureImageUnits = value;
				else if (strcmp(token, "MaxFragmentUniformComponents") == 0)
					Resources.maxFragmentUniformComponents = value;
				else if (strcmp(token, "MaxDrawBuffers") == 0)
					Resources.maxDrawBuffers = value;
				else if (strcmp(token, "MaxVertexUniformVectors") == 0)
					Resources.maxVertexUniformVectors = value;
				else if (strcmp(token, "MaxVaryingVectors") == 0)
					Resources.maxVaryingVectors = value;
				else if (strcmp(token, "MaxFragmentUniformVectors") == 0)
					Resources.maxFragmentUniformVectors = value;
				else if (strcmp(token, "MaxVertexOutputVectors") == 0)
					Resources.maxVertexOutputVectors = value;
				else if (strcmp(token, "MaxFragmentInputVectors") == 0)
					Resources.maxFragmentInputVectors = value;
				else if (strcmp(token, "MinProgramTexelOffset") == 0)
					Resources.minProgramTexelOffset = value;
				else if (strcmp(token, "MaxProgramTexelOffset") == 0)
					Resources.maxProgramTexelOffset = value;
				else if (strcmp(token, "MaxClipDistances") == 0)
					Resources.maxClipDistances = value;
				else if (strcmp(token, "MaxComputeWorkGroupCountX") == 0)
					Resources.maxComputeWorkGroupCountX = value;
				else if (strcmp(token, "MaxComputeWorkGroupCountY") == 0)
					Resources.maxComputeWorkGroupCountY = value;
				else if (strcmp(token, "MaxComputeWorkGroupCountZ") == 0)
					Resources.maxComputeWorkGroupCountZ = value;
				else if (strcmp(token, "MaxComputeWorkGroupSizeX") == 0)
					Resources.maxComputeWorkGroupSizeX = value;
				else if (strcmp(token, "MaxComputeWorkGroupSizeY") == 0)
					Resources.maxComputeWorkGroupSizeY = value;
				else if (strcmp(token, "MaxComputeWorkGroupSizeZ") == 0)
					Resources.maxComputeWorkGroupSizeZ = value;
				else if (strcmp(token, "MaxComputeUniformComponents") == 0)
					Resources.maxComputeUniformComponents = value;
				else if (strcmp(token, "MaxComputeTextureImageUnits") == 0)
					Resources.maxComputeTextureImageUnits = value;
				else if (strcmp(token, "MaxComputeImageUniforms") == 0)
					Resources.maxComputeImageUniforms = value;
				else if (strcmp(token, "MaxComputeAtomicCounters") == 0)
					Resources.maxComputeAtomicCounters = value;
				else if (strcmp(token, "MaxComputeAtomicCounterBuffers") == 0)
					Resources.maxComputeAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxVaryingComponents") == 0)
					Resources.maxVaryingComponents = value;
				else if (strcmp(token, "MaxVertexOutputComponents") == 0)
					Resources.maxVertexOutputComponents = value;
				else if (strcmp(token, "MaxGeometryInputComponents") == 0)
					Resources.maxGeometryInputComponents = value;
				else if (strcmp(token, "MaxGeometryOutputComponents") == 0)
					Resources.maxGeometryOutputComponents = value;
				else if (strcmp(token, "MaxFragmentInputComponents") == 0)
					Resources.maxFragmentInputComponents = value;
				else if (strcmp(token, "MaxImageUnits") == 0)
					Resources.maxImageUnits = value;
				else if (strcmp(token, "MaxCombinedImageUnitsAndFragmentOutputs") == 0)
					Resources.maxCombinedImageUnitsAndFragmentOutputs = value;
				else if (strcmp(token, "MaxCombinedShaderOutputResources") == 0)
					Resources.maxCombinedShaderOutputResources = value;
				else if (strcmp(token, "MaxImageSamples") == 0)
					Resources.maxImageSamples = value;
				else if (strcmp(token, "MaxVertexImageUniforms") == 0)
					Resources.maxVertexImageUniforms = value;
				else if (strcmp(token, "MaxTessControlImageUniforms") == 0)
					Resources.maxTessControlImageUniforms = value;
				else if (strcmp(token, "MaxTessEvaluationImageUniforms") == 0)
					Resources.maxTessEvaluationImageUniforms = value;
				else if (strcmp(token, "MaxGeometryImageUniforms") == 0)
					Resources.maxGeometryImageUniforms = value;
				else if (strcmp(token, "MaxFragmentImageUniforms") == 0)
					Resources.maxFragmentImageUniforms = value;
				else if (strcmp(token, "MaxCombinedImageUniforms") == 0)
					Resources.maxCombinedImageUniforms = value;
				else if (strcmp(token, "MaxGeometryTextureImageUnits") == 0)
					Resources.maxGeometryTextureImageUnits = value;
				else if (strcmp(token, "MaxGeometryOutputVertices") == 0)
					Resources.maxGeometryOutputVertices = value;
				else if (strcmp(token, "MaxGeometryTotalOutputComponents") == 0)
					Resources.maxGeometryTotalOutputComponents = value;
				else if (strcmp(token, "MaxGeometryUniformComponents") == 0)
					Resources.maxGeometryUniformComponents = value;
				else if (strcmp(token, "MaxGeometryVaryingComponents") == 0)
					Resources.maxGeometryVaryingComponents = value;
				else if (strcmp(token, "MaxTessControlInputComponents") == 0)
					Resources.maxTessControlInputComponents = value;
				else if (strcmp(token, "MaxTessControlOutputComponents") == 0)
					Resources.maxTessControlOutputComponents = value;
				else if (strcmp(token, "MaxTessControlTextureImageUnits") == 0)
					Resources.maxTessControlTextureImageUnits = value;
				else if (strcmp(token, "MaxTessControlUniformComponents") == 0)
					Resources.maxTessControlUniformComponents = value;
				else if (strcmp(token, "MaxTessControlTotalOutputComponents") == 0)
					Resources.maxTessControlTotalOutputComponents = value;
				else if (strcmp(token, "MaxTessEvaluationInputComponents") == 0)
					Resources.maxTessEvaluationInputComponents = value;
				else if (strcmp(token, "MaxTessEvaluationOutputComponents") == 0)
					Resources.maxTessEvaluationOutputComponents = value;
				else if (strcmp(token, "MaxTessEvaluationTextureImageUnits") == 0)
					Resources.maxTessEvaluationTextureImageUnits = value;
				else if (strcmp(token, "MaxTessEvaluationUniformComponents") == 0)
					Resources.maxTessEvaluationUniformComponents = value;
				else if (strcmp(token, "MaxTessPatchComponents") == 0)
					Resources.maxTessPatchComponents = value;
				else if (strcmp(token, "MaxPatchVertices") == 0)
					Resources.maxPatchVertices = value;
				else if (strcmp(token, "MaxTessGenLevel") == 0)
					Resources.maxTessGenLevel = value;
				else if (strcmp(token, "MaxViewports") == 0)
					Resources.maxViewports = value;
				else if (strcmp(token, "MaxVertexAtomicCounters") == 0)
					Resources.maxVertexAtomicCounters = value;
				else if (strcmp(token, "MaxTessControlAtomicCounters") == 0)
					Resources.maxTessControlAtomicCounters = value;
				else if (strcmp(token, "MaxTessEvaluationAtomicCounters") == 0)
					Resources.maxTessEvaluationAtomicCounters = value;
				else if (strcmp(token, "MaxGeometryAtomicCounters") == 0)
					Resources.maxGeometryAtomicCounters = value;
				else if (strcmp(token, "MaxFragmentAtomicCounters") == 0)
					Resources.maxFragmentAtomicCounters = value;
				else if (strcmp(token, "MaxCombinedAtomicCounters") == 0)
					Resources.maxCombinedAtomicCounters = value;
				else if (strcmp(token, "MaxAtomicCounterBindings") == 0)
					Resources.maxAtomicCounterBindings = value;
				else if (strcmp(token, "MaxVertexAtomicCounterBuffers") == 0)
					Resources.maxVertexAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxTessControlAtomicCounterBuffers") == 0)
					Resources.maxTessControlAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxTessEvaluationAtomicCounterBuffers") == 0)
					Resources.maxTessEvaluationAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxGeometryAtomicCounterBuffers") == 0)
					Resources.maxGeometryAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxFragmentAtomicCounterBuffers") == 0)
					Resources.maxFragmentAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxCombinedAtomicCounterBuffers") == 0)
					Resources.maxCombinedAtomicCounterBuffers = value;
				else if (strcmp(token, "MaxAtomicCounterBufferSize") == 0)
					Resources.maxAtomicCounterBufferSize = value;
				else if (strcmp(token, "MaxTransformFeedbackBuffers") == 0)
					Resources.maxTransformFeedbackBuffers = value;
				else if (strcmp(token, "MaxTransformFeedbackInterleavedComponents") == 0)
					Resources.maxTransformFeedbackInterleavedComponents = value;
				else if (strcmp(token, "MaxCullDistances") == 0)
					Resources.maxCullDistances = value;
				else if (strcmp(token, "MaxCombinedClipAndCullDistances") == 0)
					Resources.maxCombinedClipAndCullDistances = value;
				else if (strcmp(token, "MaxSamples") == 0)
					Resources.maxSamples = value;
				else if (strcmp(token, "nonInductiveForLoops") == 0)
					Resources.limits.nonInductiveForLoops = (value != 0);
				else if (strcmp(token, "whileLoops") == 0)
					Resources.limits.whileLoops = (value != 0);
				else if (strcmp(token, "doWhileLoops") == 0)
					Resources.limits.doWhileLoops = (value != 0);
				else if (strcmp(token, "generalUniformIndexing") == 0)
					Resources.limits.generalUniformIndexing = (value != 0);
				else if (strcmp(token, "generalAttributeMatrixVectorIndexing") == 0)
					Resources.limits.generalAttributeMatrixVectorIndexing = (value != 0);
				else if (strcmp(token, "generalVaryingIndexing") == 0)
					Resources.limits.generalVaryingIndexing = (value != 0);
				else if (strcmp(token, "generalSamplerIndexing") == 0)
					Resources.limits.generalSamplerIndexing = (value != 0);
				else if (strcmp(token, "generalVariableIndexing") == 0)
					Resources.limits.generalVariableIndexing = (value != 0);
				else if (strcmp(token, "generalConstantMatrixVectorIndexing") == 0)
					Resources.limits.generalConstantMatrixVectorIndexing = (value != 0);
				else
					printf("Warning: unrecognized limit (%s) in configuration file.\n", token);

				token = strtok(0, delims);
			}

			// shader code
			string code = m_EditorFragmentView->GetCode();
			string header = m_EditorFragmentView->GetHeader();
			string shaderString = header + code;
			const char *shaderCode = shaderString.c_str();
			EShMessages messages = EShMsgDefault;
			int ret = ShCompile(compiler, &shaderCode, 1, 0, EShOptNone, &Resources, Options, (Options & EOptionDefaultDesktop) ? 110 : 100, false, messages);
			
			res = ShGetInfoLog(compiler);

			ShDestruct(compiler);
		}
		else
		{
			res = "Validation by Glslang Failed !!";
		}
	}*/

	return res;
}

void DocShaderArt::OnCompilAndRunShaderClick(wxRibbonButtonBarEvent& /*event*/)
{
	CompilAndRunShader(false);

	// on incremente le numero de build que quand le suer click sur run
	m_Document->IncBuildNumber();
}

bool DocShaderArt::NewDocument(string vFilePathName)
{
	bool res = false;

	ShaderArtTypeEnum m_ShaderArtType = m_Document->GetShaderArtType();

	if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD &&
		m_Document->NewDocument(ShaderEditorTemplateEnum::TEMPLATE_QUAD, vFilePathName) == true)
	{
		m_ShaderView->SetRenderMode(m_Document->GetRenderMode());
		m_ShaderViewToolBar->SetRenderMode(m_Document->GetRenderMode());

		// create uniform widget for loaded uniforms
		if (m_Uniforms_Mgr->CompleteUniformsWithCorrespondingWidget(m_UniformView, m_ShaderView) == true)
		{
			// put shader in editor
			m_EditorFragmentView->LoadFileMemoryToEditor();
			
			// on utilise m_FragmentShader et on lance le shader 
			CompilAndRunShader(false);

			m_Document->SetModified(true);

			if (g_pMainFrame != 0)
				g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());

			res = true;
		}
	}
	else if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS &&
			m_Document->NewDocument(ShaderEditorTemplateEnum::TEMPLATE_POINTS, vFilePathName) == true)
	{
		m_ShaderView->SetRenderMode(m_Document->GetRenderMode());
		m_ShaderViewToolBar->SetRenderMode(m_Document->GetRenderMode());

		// create uniform widget for loaded uniforms
		if (m_Uniforms_Mgr->CompleteUniformsWithCorrespondingWidget(m_UniformView, m_ShaderView) == true)
		{
			// put shader in editor
			m_EditorFragmentView->LoadFileMemoryToEditor();
			m_EditorVertexView->LoadFileMemoryToEditor();

			// on utilise m_FragmentShader et on lance le shader 
			CompilAndRunShader(false);

			m_Document->SetModified(true);

			if (g_pMainFrame != 0)
				g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());

			res = true;
		}
	}
	
	return res;
}

bool DocShaderArt::ReplaceUniformNameInStringAndCreateUniformForShaderToyImport(string vText, UniformsWidgetTypeEnum /*vEnum*/)
{
	bool res = false;
	/*
	string UniformName;

	size_t pos = 0;
	while ((pos = vText.find("iMouse", pos)) != std::string::npos)
	{
		size_t pos_coma = txt.find_first_of(',', pos);
		pos += 7; // iMouse + (
		string uniform = txt.substr(pos, pos_coma - pos);
		if (uniform.find("iMouse") != std::string::npos)
		{
			string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::UMOUSE, p);
			if (uniformName != "")
			{
				DebugLogger::ReplaceString(txt, uniform, uniformName);
			}
			else
			{
				res = false;
			}
		}
	}*/

	return res;
}

bool DocShaderArt::ImportDocumentString(string vDocumentString, ImportTypeEnum vImportType)
{
	bool res = false;

	if (vImportType == ImportTypeEnum::INPORT_SHADERTOY)
	{
		map<string, string> p; // param à zero

		// traitement du fichier issu de la page 1
		string txt = vDocumentString;

		// on va remplacer les uniform connu
		size_t pos = 0;

		// iGlobalTime
		pos = 0;
		if (txt.find("iGlobalTime") != std::string::npos)
		{
			string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::UTIME, p);
			if (uniformName != "")
			{
				DebugLogger::ReplaceString(txt, "iGlobalTime", uniformName);
			}
			else
			{
				res = false;
			}
		}

		// iResolution
		pos = 0;
		if (txt.find("iResolution") != std::string::npos)
		{
			string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::USCREEN, p);
			if (uniformName != "")
			{
				DebugLogger::ReplaceString(txt, "iResolution", uniformName);
			}
			else
			{
				res = false;
			}
		}

		// iMouse
		pos = 0;
		if (txt.find("iMouse") != std::string::npos)
		{
			string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::UMOUSE, p);
			if (uniformName != "")
			{
				DebugLogger::ReplaceString(txt, "iMouse", uniformName);
			}
			else
			{
				res = false;
			}
		}

		// la c'est plus compliqué car iChannel peut etre cube ou 2D

		// Texture2D
		pos = 0;
		while ((pos = txt.find("texture2D(", pos)) != std::string::npos)
		{
			size_t pos_coma = txt.find_first_of(',', pos);
			pos += 10; // texture2D(
			string channel = txt.substr(pos, pos_coma - pos);
			if (channel.find("iChannel") != std::string::npos)
			{
				string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::UTEX2D, p);
				if (uniformName != "")
				{
					DebugLogger::ReplaceString(txt, channel, uniformName);
				}
				else
				{
					res = false;
				}
			}
		}

		// TextureCube
		pos = 0;
		while ((pos = txt.find("textureCube(", pos)) != std::string::npos)
		{
			size_t pos_coma = txt.find_first_of(',', pos);
			pos += 12; // textureCube(
			string channel = txt.substr(pos, pos_coma - pos);
			if (channel.find("iChannel") != std::string::npos)
			{
				string uniformName = m_Uniforms_Mgr->AddUniformFromScript(UniformsWidgetTypeEnum::UCUBEMAP, p);
				if (uniformName != "")
				{
					DebugLogger::ReplaceString(txt, channel, uniformName);
				}
				else
				{
					res = false;
				}
			}
		}

		// mainImage( out vec4 fragColor, in vec2 fragCoord )
		pos = 0;
		if ((pos = txt.find("mainImage(")) != std::string::npos)
		{
			size_t endLine = txt.find_first_of(')', pos) + 1;
			string mainImage = txt.substr(pos, endLine - pos);
			DebugLogger::ReplaceString(txt, mainImage, "main()");
			DebugLogger::ReplaceString(txt, "fragColor", "gl_FragColor");
			DebugLogger::ReplaceString(txt, "fragCoord", "gl_FragCoord");
		}

		m_Document->SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_VERTEX, "attribute vec2 pos;\nvoid main()\n{\n\tgl_Position = vec4(pos.xy, 0.0, 1.0);\n}\n");
		
		m_Document->SetRenderMode("TRIANGLES");
		m_ShaderView->SetRenderMode(m_Document->GetRenderMode());
		m_ShaderViewToolBar->SetRenderMode(m_Document->GetRenderMode());

		//  if(d0*sgn<0.) m.z=m.y; else m.x=m.y; en glsl ca merde ca
		// on doit trouver le moyen de rempalcer le pattern a?b:c; par if(a) b; else c;
		// edit 06/06/2015 : en fait ca manque juste de parenthese ca merde pas plus que ca

		// on met le text modifié dans le fichier virtuel du document
		m_Document->SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_FRAGMENT, txt);

		// on cree le widget et on compile
		// create uniform widget for loaded uniforms
		if (m_Uniforms_Mgr->CompleteUniformsWithCorrespondingWidget(m_UniformView, m_ShaderView) == true)
		{
			// put shader in editor
			m_EditorFragmentView->LoadFileMemoryToEditor();

			// on utilise m_FragmentShader et on lance le shader 
			CompilAndRunShader(false);

			m_Document->SetModified(true);

			if (g_pMainFrame != 0)
				g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());

			res = true;
		}
	}
	

	return res;
}


bool DocShaderArt::ExportDocumentString(string vDocumentString, ExportTypeEnum /*vImportType*/)
{
	bool res = false;

	return res;
}

bool DocShaderArt::LoadDocument(string vFilePathName, DocumentFileFormat vFileFormat, bool vCompilAfterLoating)
{
	bool res = false;

	if (m_Document->LoadDocumentFile(vFilePathName, vFileFormat, false) == true)
	{
		// maj les combobox de version et precision
		m_EditorFragmentView->UpdateGlslVersionAndFloatPrecisionComboBoxs();

		ShaderArtTypeEnum m_ShaderArtType = m_Document->GetShaderArtType();

		if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
			m_EditorVertexView->UpdateGlslVersionAndFloatPrecisionComboBoxs();

		string renderMode = m_Document->GetRenderMode();
		if (renderMode.size() == 0) renderMode = "TRIANGLES";
		m_ShaderView->SetRenderMode(renderMode);
		m_ShaderViewToolBar->SetRenderMode(renderMode);

		// create uniform widget for loaded uniforms
		if (m_Uniforms_Mgr->CompleteUniformsWithCorrespondingWidget(m_UniformView, m_ShaderView) == true)
		{
			if (OnLoadMusicShader() == true)
			{
				// on utilise m_FragmentShader et on lance le shader 
				if (vCompilAfterLoating == true)
					CompilAndRunShader(false);

				m_Document->SetModified(false);

				if (g_pMainFrame != 0)
					g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());
			}
			
			m_auinotebookEditor->SetSelection(0);

			// put shader in editor
			m_EditorFragmentView->LoadFileMemoryToEditor();

			if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
				m_EditorVertexView->LoadFileMemoryToEditor();

			// on utilise m_FragmentShader et on lance le shader 
			if (vCompilAfterLoating == true)
				CompilAndRunShader(false);

			m_Document->SetModified(false);

			if (g_pMainFrame != 0)
				g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());

			res = true;
		}
	}
	
	return res;
}

bool DocShaderArt::SaveDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat)
{
	if (m_EditorFragmentView != 0)
		m_EditorFragmentView->SaveEditorToFileMemory();

	if (m_EditorVertexView != 0)
		m_EditorVertexView->SaveEditorToFileMemory();

	if (m_EditorMusicView != 0)
		m_EditorMusicView->SaveEditorToFileMemory();
	
	return m_Document->SaveDocumentFile(vFilePathName, vFileFormat);
}

void DocShaderArt::RefreshSizeAndDisplay()
{
	// size voir http://redmine.cuicuiprod.net/issues/214 pour le pourquoi de ce code
	int w, h;
	m_ShaderView->GetSize(&w, &h);
	m_ShaderView->SetScreenSize(w, h);

	// display
	m_ShaderView->Refresh(true, 0);
}

void DocShaderArt::RefreshShaderView()
{
	// display
	m_ShaderView->Refresh(false, 0);
}

void DocShaderArt::RecordGif(float /*vTime0*/, float /*vTime1*/, float /*vFps*/, string /*vFilepathName*/)
{

}

void DocShaderArt::DoAutoLayout(ShaderArtLayoutEnum vLay)
{
	m_CurrentLayout = vLay;

	// 0:EditorView 1:ShaderView 2:UniformView
	
	if (vLay == ShaderArtLayoutEnum::EL_ST_UT)
	{
		m_auinotebook3->UnSplit(1, 0);
		m_auinotebook3->UnSplit(2, 0);
	}
	if ( vLay == ShaderArtLayoutEnum::EL_STR_UBR)
	{
		m_auinotebook3->Split(2, wxBOTTOM);
		m_auinotebook3->Split(0, wxLEFT);
	}
	else if ( vLay == ShaderArtLayoutEnum::ER_STL_UBL)
	{
		m_auinotebook3->Split(2, wxBOTTOM);
		m_auinotebook3->Split(0, wxRIGHT);
	}
	else if (vLay == ShaderArtLayoutEnum::ETL_STR_UB)
	{
		m_auinotebook3->Split(0, wxLEFT);
		m_auinotebook3->Split(1, wxRIGHT);
		m_auinotebook3->Split(2, wxBOTTOM);
	}
	else if (vLay == ShaderArtLayoutEnum::ETR_STL_UB)
	{
		m_auinotebook3->Split(0, wxRIGHT);
		m_auinotebook3->Split(1, wxLEFT);
		m_auinotebook3->Split(2, wxBOTTOM);
	}
	else if (vLay == ShaderArtLayoutEnum::ETL_SR_UBL)
	{
		m_auinotebook3->Split(2, wxBOTTOM);
		m_auinotebook3->Split(1, wxRIGHT);
	}
	else if (vLay == ShaderArtLayoutEnum::ETR_SL_UBR)
	{
		m_auinotebook3->Split(2, wxBOTTOM);
		m_auinotebook3->Split(1, wxLEFT);
	}

	// pour rafraichir l'uniform size meme si la page est pas active quand le resize ce produit
	if (m_ShaderView != 0)
	{
		int w, h;
		m_ShaderView->GetSize(&w, &h);
		m_ShaderView->SetScreenSize(w, h);
	}

	if (g_pMainFrame != 0)
		g_pMainFrame->ToggleLayoutButton(vLay);
}

void DocShaderArt::DoAutoIndent()
{
	EditorView *view = static_cast<EditorView*>(m_auinotebookEditor->GetCurrentPage());
	if (view != 0)
	{
		view->DoAutoIndent();
	}
}

void DocShaderArt::DoAnalyseShaderCode()
{
	EditorView *view = static_cast<EditorView*>(m_auinotebookEditor->GetCurrentPage());
	if (view != 0)
	{
		view->DoAnalyseShaderCode();
	}
}

ShaderArtLayoutEnum DocShaderArt::GetCurrentLayout()
{
	return m_CurrentLayout;
}

void DocShaderArt::OnSize(wxSizeEvent& /*event*/)
{
	this->Layout();

	Init();

	if (m_IsInit == true)
	{
		// pour rafraichir l'uniform size meme si la page est pas active quand le resize ce produit
		if (m_ShaderView != 0)
		{
			int w, h;
			m_ShaderView->GetSize(&w, &h);
			m_ShaderView->SetScreenSize(w, h);
		}
	}
}

bool DocShaderArt::AddUniformWidget(UniformsWidgetTypeEnum vType)
{
	bool res = false;

	UniformAbstract *abs = m_Uniforms_Mgr->CreateUniformWidget(vType);
	if (abs != 0)
	{
		if (m_Uniforms_Mgr->AddUniformFromWidget(vType, abs) == true)
		{
			abs->SetShaderView(m_ShaderView);
			abs->SetShaderRenderer(m_ShaderView->GetShaderRenderer());
			if (m_UniformView->AddPane(abs) == true)
			{
				m_Document->SetModified(true);
				res = true;
			}
		}
	}

	if (res == false)
	{

		SAFE_DELETE(abs);
	}

	return res;
}

void DocShaderArt::DoSwitchPerfMode(wxCommandEvent& /*event*/)
{
	if (m_Uniforms_Mgr != 0 && m_ShaderView != 0)
	{
		m_ShaderView->m_ShowPerf = !m_ShaderView->m_ShowPerf;
		if (m_ShaderView->m_ShowPerf == true) m_ShaderView->m_timer.Stop();
		else m_ShaderView->m_timer.Start(-1);

		map<string, UniformStruct> dico = m_Uniforms_Mgr->GetMap();
		for (std::map<string, UniformStruct>::iterator it = dico.begin(); it != dico.end(); ++it)
		{
			string key = it->first;
			UniformStruct uni = it->second;

			if (uni.wType == UniformsWidgetTypeEnum::UTIME)
			{
				if (uni.widget != 0)
				{
					UniformTime *unitime = dynamic_cast<UniformTime*>(uni.widget);
					if (unitime != 0)
					{
						if (m_ShaderView->m_ShowPerf == true)
							unitime->PauseTimer();
					}
				}
			}
		}

		m_ShaderView->Refresh(false, 0);
	}
}

void DocShaderArt::OnPageClose(wxAuiNotebookEvent& event)
{
	int pageIdx = event.GetSelection();
	if (pageIdx != wxNOT_FOUND)
	{
		EditorView *editor = 0;
		editor = dynamic_cast<EditorView*>(m_auinotebookEditor->GetPage(pageIdx));
		if (editor != 0)
		{
			if (editor->GetShaderType() == ShaderEditorTemplateEnum::TEMPLATE_QUAD)
			{
				event.Veto(); // on block l'event car on veut pas le fermer lui
			}
			else
			{
				// on detruit le shader associé
				if (m_EditorMusicView == editor)
				{
					m_EditorMusicView = 0; // on met la reference a 0. ca ne detruit pas l'objet

					m_Document->SetModified(true);

					if (g_pMainFrame != 0)
						g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());
				}
			}
		}
	}
	else
	{
		event.Veto(); // on block l'event
	}
}

void DocShaderArt::OnPageClosed(wxAuiNotebookEvent& event)
{
	int pageIdx = event.GetSelection();
	if (pageIdx != wxNOT_FOUND)
	{
		
	}
}

void DocShaderArt::OnAddMusicShader()
{
	/*if (m_Uniforms_Mgr != 0 && m_ShaderView != 0)
	{
		if (m_EditorMusicView == 0)
		{
			m_EditorMusicView = EditorView::Create(this, ShaderTypeEnum::TEMPLATE_MUSIC, m_auinotebookEditor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
			if (m_EditorMusicView != 0)
			{
				m_auinotebookEditor->AddPage(m_EditorMusicView, wxT("Music Editor"), true, wxNullBitmap);

				if (m_EditorMusicView->Init() == true)
				{
					if (m_Document->NewDocument(ShaderEditorTemplateEnum::TEMPLATE_MUSIC, "") == true)
					{
						// put shader in editor
						m_EditorMusicView->LoadFileMemoryToEditor();

						// on utilise m_FragmentShader et on lance le shader 
						CompilAndRunShader(false);

						m_Document->SetModified(true);

						if (g_pMainFrame != 0)
							g_pMainFrame->SetSavedStatusOFCurrentPage(!m_Document->IsModified());
					}
				}
			}
		}
	}*/
}

bool DocShaderArt::OnLoadMusicShader()
{
	bool res = false;

	/*if (m_Uniforms_Mgr != 0 && m_ShaderView != 0)
	{
		string music = m_Document->GetFileMemoryToString(ShaderTypeEnum::::TEMPLATE_MUSIC);
		if (music.size() > 0)
		{
			if (m_EditorMusicView == 0)
			{
				m_EditorMusicView = EditorView::Create(this, ShaderEditorTemplateEnum::TEMPLATE_MUSIC, m_auinotebookEditor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
				
				if (m_EditorMusicView != 0)
				{
					m_auinotebookEditor->AddPage(m_EditorMusicView, wxT("Music Editor"), true, wxNullBitmap);

					if (m_EditorMusicView->Init() == true)
					{
						// put shader in editor
						m_EditorMusicView->LoadFileMemoryToEditor();
						res = true;
					}
				}
			}
		}
	}*/

	return res;
}

void DocShaderArt::DoShaderPerfAnalyse()
{
	// fonctionnement
	// on replace gl_FragCoord par un uniform vec2 special qu'on va piloter
	// le but est ici d'obtenir une carte de cellule en focntion du temps d'execution
	// il faut aussi donner une taille maxi de cellule

	if (m_Uniforms_Mgr != 0 && m_ShaderView != 0)
	{
		// on pause . hors de question de faire ca en animation
		//wxCommandEvent event;
		//event.SetEventObject(m_PausePlayBtn);
		//DoPausePlayView(event); // on simule le clic sur le bouton pause

		// on ajout le uniform PerfCoord
		m_Uniforms_Mgr->AddUniformFromFile("PerfCoord", UniformsWidgetTypeEnum::USLIDER, map<string, string>());
		cUniformVariant PerfCoord;
		PerfCoord.count = 4; // pour qu'il soit prit en vec4
		m_Uniforms_Mgr->UpdateUniformByName("PerfCoord", PerfCoord, false);

		// on ajout le uniform CellSize
		m_Uniforms_Mgr->AddUniformFromFile("CellSize", UniformsWidgetTypeEnum::USLIDER, map<string, string>());
		cUniformVariant CellSize;
		CellSize.count = 2; // pour qu'il soit prit en vec4
		m_Uniforms_Mgr->UpdateUniformByName("CellSize", CellSize, false);

		//CompilAndRunShader(true);
		string fragment = m_EditorFragmentView->GetCode();
		string vertex = m_EditorVertexView->GetCode();
		ComplexStringStruct uniforms = m_Uniforms_Mgr->GetUniformsToString();
		if (m_ShaderView->CompileAndUseVertexAndFragment(uniforms, vertex, fragment, true) == false)
		{
			// mode debug
			return;
		}

		HDC hdc = m_ShaderView->GetHDC();

		int w, h;
		m_ShaderView->GetSize(&w, &h);

		LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
		LARGE_INTEGER Frequency;

		GLShaderRenderer *m_GLShaderRenderer = m_ShaderView->GetShaderRenderer();
		if (m_GLShaderRenderer != 0)
		{
			m_GLShaderRenderer->SetCurrent(hdc);

			int countX = 40;
			int countY = 30;

			int maxSizeX = floor(w / countX);
			int maxSizeY = floor(h / countY);

			CellSize.x = maxSizeX;
			CellSize.y = maxSizeY;
			m_Uniforms_Mgr->UpdateUniformByName("CellSize", CellSize, false);

			// virtual 2d array :
			//sx = count cell for x axis of the virtual 2d array (matrice2)
			//ID(cx,cy) = cy*sx+cx
			//cx(ID) = ID modulo sx
			//cy(ID) = (ID-cx)/sx
			int count = countX*countY;
			LONGLONG *arr = new LONGLONG[count];
			for (int i = 0; i < count; i++) arr[i] = 0;// init de l'array

			LONGLONG var = m_ShaderView->GetExecTime();
			LONGLONG min = var;
			LONGLONG max = var;
			LONGLONG MEAN = 0;

			// on test le shader pour chaque pixel de la shadeview
			// et on recupere le temps d'execution du shader pour chaque pixel
			for (int i = 0; i < countX; i ++)
			{
				for (int j = 0; j < countY; j ++)
				{
					PerfCoord.x = i*maxSizeX;
					PerfCoord.y = j*maxSizeY;
					m_Uniforms_Mgr->UpdateUniformByName("PerfCoord", PerfCoord, false);

					QueryPerformanceFrequency(&Frequency);
					QueryPerformanceCounter(&StartingTime);

					m_GLShaderRenderer->DrawSceneWithoutSwap(hdc);

					QueryPerformanceCounter(&EndingTime);
					ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;

					ElapsedMicroseconds.QuadPart *= 1000000;
					ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

					if (ElapsedMicroseconds.QuadPart > 0)
					{
						var = ElapsedMicroseconds.QuadPart;

						if (var < min)
							min = var;
						if (var > max)
							max = var;

						MEAN += var;
						int ID = floor(floor(j * countX) + i);
						arr[ID] = var;
					}
				}
			}
			MEAN /= count;
			MEAN *= 1.5; // valeur alea pour faire 75% des valeurs
			/*
			// on parcours le tab et on va moyenner 
			for (int i = 0; i < w; i++)
			{
				for (int j = 0; j < h; j++)
				{
					cUniformVariant coord;
					coord.x = i;
					coord.y = j;
					m_Uniforms_Mgr->UpdateUniformByName("PerfCoord", coord, true);

					m_ShaderView->OnPaint(wxPaintEvent());
					arr[j*w + i] = m_ShaderView->GetExecTime();
				}
			}
			*/
			// on compose un shader dedie a afficher la perf
			// https://www.shadertoy.com/view/XlXSzB
			string shader;
			shader += "vec2 gs = vec2(" + toStr(countX) + ".," + toStr(countY) + ".);\n";
			shader += "vec2 minmax = vec2(" + toStr(min) + ".," + toStr(MEAN) + ".);\n";
			shader += "vec2 s, g, h, m; float z, t;\n \
						vec3 getcell(vec2 s, vec2 h, vec2 sz)\n \
						{\n \
							float cx = floor(h.x / (s.x / sz.x));\n \
							float cy = floor(h.y / (s.y / sz.y));\n \
							float id = cy*sz.x + cx;\n \
							vec2 size = s / sz;\n \
							float ratio = size.x / size.y;\n \
							vec2 uv = (2.*(h)-size) / size.y - vec2(cx*ratio, cy)*2.;\n \
							uv *= 1.5;\n \
							return vec3(id, uv);\n \
						}\n";
			shader += "void main()\n \
			{\n \
				s = uScreenSize\n; \
				h = gl_FragCoord.xy;\n \
				g = (2.*h - s) / s.y;\n \
				float id = getcell(s, h, gs).x;\n \
				vec3 c = vec3(0.);\n \
				float v = 0.0;\n";

			// on boucle pour ecrire un truc du genre if (id == 0.) c = vec3(0.); \ puis else if (id == 1.) c = vec3(0.1);
			for (int i = 0; i < countX; i ++)
			{
				for (int j = 0; j < countY; j ++)
				{
					int ID = floor(floor(j * countX) + i);
					var = arr[ID];
					shader += "if (id == " + toStr(ID) + ".) v = " + toStr(var) + ".;\n";
				}
			}

			// on reprend
			shader += "	v /= minmax.y-minmax.x;\n \
						c = vec3(v, v / 2., v / 3.);\n \
						gl_FragColor = vec4(c, 1); }\n";

			// avant de l'exuter on le sauve dans un fichier pour debug
			ofstream docFile("PerfShader.frag", ios::out);
			if (docFile.bad() == false)
			{
				docFile << shader;
				docFile.close();
			}

			// on enleve le uniform
			m_Uniforms_Mgr->RemoveUniformByName("PerfCoord");
			m_Uniforms_Mgr->RemoveUniformByName("CellSize");

			// on l'upload
			uniforms = m_Uniforms_Mgr->GetUniformsToString();
			
			// a ce moemnt on doit plus afficher un sahder de perf don 4eme param a false
			if (m_ShaderView->CompileAndUseVertexAndFragment(uniforms, shader, false) == false)
			{
				// mode debug
				return;
			}
		}
		
	}

	m_ShaderView->Refresh(false, 0);
}
