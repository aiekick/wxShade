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

#ifndef __XSHADEGLOBALS_H__
#define __XSHADEGLOBALS_H__

// moyenne du calcul du fps sur COUNT_ITEM_FOR_FPS_MEAN elments
#define COUNT_ITEM_FOR_FPS_MEAN 5

class OutputWin;
class MainFrame;

extern OutputWin* g_pOutputWin;
extern MainFrame* g_pMainFrame;

enum DocumentFragmentParseErrorEnum
{
	NOTHING=0,
	BAD_XML_FORMAT=1,
	DUPLICATE_UNIFORM_NAME=2,
	UNIFORM_NAME_MISSED=3
	
};

enum DocumentFileFormat
{
	DOC_FILE_FORMAT_NOTHING = 0,
	DOC_FILE_FORMAT_ASCII = 1,
	DOC_FILE_FORMAT_EMBEDDED = 2
};

// Glslang Command-line options
enum TOptions {
	EOptionNone = 0x000,
	EOptionIntermediate = 0x001,
	EOptionSuppressInfolog = 0x002,
	EOptionMemoryLeakMode = 0x004,
	EOptionRelaxedErrors = 0x008,
	EOptionGiveWarnings = 0x010,
	EOptionLinkProgram = 0x020,
	EOptionMultiThreaded = 0x040,
	EOptionDumpConfig = 0x080,
	EOptionDumpReflection = 0x100,
	EOptionSuppressWarnings = 0x200,
	EOptionDumpVersions = 0x400,
	EOptionBil = 0x800,
	EOptionDefaultDesktop = 0x1000,
};

// Mainfame
#define ID_MEMORY_CHECK_TIMER					400
#define ID_NEW_DOCUMENT							500
#define ID_OPEN_DOCUMENT						501
#define ID_OPEN_DOCUMENT_WITHOUT_COMPILATION	502
#define ID_SAVE_DOCUMENT						503
#define ID_SAVEAS_DOCUMENT						504
#define ID_DELETE_DOCUMENT						505
#define ID_SHOW_HIDE_OUTPUT_WIN					506
#define ID_COMPIL_RUN_SHADER					507
#define ID_SETTINGS								508
#define ID_HELP									509
#define ID_ABOUT								510
#define ID_SHADER_NOTEBOOK						511
#define ID_SHADER_DO_AUTO_LAYOUT0				512
#define ID_SHADER_DO_AUTO_LAYOUT1				513
#define ID_SHADER_DO_AUTO_LAYOUT2				514
#define ID_SHADER_DO_AUTO_LAYOUT3				515
#define ID_SHADER_DO_AUTO_LAYOUT4				516
#define ID_SHADER_DO_AUTO_LAYOUT5				517
#define ID_SHADER_DO_AUTO_LAYOUT6				518
#define ID_AUI_MANAGER							519
#define ID_SHADER_DO_AUTO_INDENT				520
#define ID_SHADER_ANALYSE_CODE					521
#define ID_SHADER_DO_PERF_ANALYSE				522
#define ID_SHADER_MUSIC_ADD						523

// Import From ShaderToy
#define ID_IMPORT_FROM_SHADERTOY_DOCUMENT		524
#define ID_IMPORT_FROM_SHADERTOY_EDITOR			525
#define ID_EXPORT_SHADER_TO_GIF					526
#define ID_EXPORT_TO_SHADERTOY					527

// new 
#define ID_NEW_SHADER_FOR_QUAD					528
#define ID_NEW_SHADER_FOR_POINTS				529
#define ID_NEW_SHADER_FOR_CUSTOM_SHAPE			530

// export
#define ID_EXPORT_SHADER_TO_PICTURE				531
#define ID_EXPORT_SHADER_TO_WEBGL				532

// UniformViewItem
#define ID_REMOVE_UNIFORM_ABSTRACT_BTN			600
#define ID_TYPE_UNIFORM_ABSTRACT_BTN			601
#define ID_MERGE_IN_CODE_UNIFORM_ABSTRACT_BTN	602
#define ID_SEARCH_IN_CODE_UNIFORM_ABSTRACT_BTN	603

// ShaderView
#define ID_SHADERVIEW_TIMER						700 // wxTimer
#define ID_SHADERVIEW_FPSTIMER					701 // wxFpsTimer

// FPS
#define ID_FPS_TIMER							702 // wxTimer
#define ID_PERF_BTN								703 // get perf
#define ID_PAUSEPLAY_BTN						704 // play pause the view

// ShaderView ToolBar
#define ID_TOGGLE_CROP_MODE_BTN					705
#define ID_TOGGLE_DEBUG_MODE_BTN				706
#define ID_CONFIG_SHADER_VIEW_BTN				707
#define ID_SHADERVIEW_RENDER_MODE				708

// UniformView ToolBar
#define ID_TOGGLE_FILTER_BTN					709

// ABOUT
#define ID_ABOUT_HTML_WIN						710 // html win from about dlg

// Layout
enum ShaderArtLayoutEnum
{
	NONE=0,
	EL_ST_UT = 1,// Editor Left / Shader Top  / Uniform Top (Top => en ONGLET )
	EL_STR_UBR = 2, // Editor Left / Shader Top Right / Uniform Bottom Right
	ER_STL_UBL = 3, // Editor Right / Shader Top Left / Uniform Bottom Left
	ETL_STR_UB = 4, // Editor Top Left / Shader Top Right / Uniform Bottom
	ETR_STL_UB = 5, // Editor Top Right / Shader Top Left / Uniform Bottom
	ETL_SR_UBL = 6, // Editor Top Left / Shader Right / Uniform Bottom Left
	ETR_SL_UBR = 7 // Editor Top Right / Shader Left / Uniform Bottom Right
};

// Uniforms
enum UniformsWidgetTypeEnum
{
	UNONE=0,
	USCREEN,
	UTIME,
	UMOUSE,
	UTEX2D,
	UCUBEMAP,
	UVIDEO,
	UMIC,
	UWEBCAM,
	USLIDER,
	UCURVE,
	UMUSIC,
	UCOLOR,
	UGAMEPAD360,
	UBVH,
	UEXPORT,
	UFONT,
	SizeOfUniformsWidgetTypeEnum
};

// Uniforms
enum UniformsTypeEnum
{
	UTYPENONE = 0,
	UFLOAT,
	UVEC2,
	UVEC3,
	UVEC4,
	USAMPLER2D,
	USAMPLERCUBE,
	SizeOfUniformsTypeEnum
};

enum ShaderMsg
{
	SHADER_MSG_OK=0,
	SHADER_MSG_ERROR=1,
	SHADER_MSG_WARNING=2
};
//static const char* UniformsTypeEnumNames[UniformsWidgetTypeEnum::SizeOfUniformsTypeEnum] = 
//{ "USCREEN", "UTIME", "UMOUSE", "UTEX2D", "UTEXCUBE", "UVIDEO", "UMIC", "UWEBCAM", "USLIDER", "UCURVE", "UMUSIC", "UCOLOR" };

// auinotebook editor
#define ID_SHADER_NOTEBOOK_EDITOR						1007

// Uniforms
#define ID_SHADER_UNIFORM_SCREEN						1008
#define ID_SHADER_UNIFORM_MOUSE							1009
#define ID_SHADER_UNIFORM_TEXTURE2D						1010
#define ID_SHADER_UNIFORM_TEXTURECUBE					1011
#define ID_SHADER_UNIFORM_VIDEO							1012
#define ID_SHADER_UNIFORM_MIC							1013
#define ID_SHADER_UNIFORM_SLIDER						1014
#define ID_SHADER_UNIFORM_CURVE							1015
#define ID_SHADER_UNIFORM_WEBCAM						1016
#define ID_SHADER_UNIFORM_MUSIC							1017
#define ID_SHADER_UNIFORM_TIME							1018
#define ID_SHADER_UNIFORM_COLOR							1019
#define ID_SHADER_UNIFORM_GAMEPAD360					1020
#define ID_SHADER_UNIFORM_BVH							1021
#define ID_SHADER_UNIFORM_EXPORT						1022
#define ID_SHADER_UNIFORM_FONT							1023

//glsl version
#define ID_SHADER_GLSL_VERSION_COMBOBOX					1040

// glsl float precision
#define ID_SHADER_FLOAT_PRECISION_COMBOBOX				1045

//show/heide header
#define ID_SHADER_SHOW_HIDE_HEADER_BTN					1049

// Uniform Widgets
#define ID_UNIFORM_WIDGET_EDIT_CHECKBOX					1050 // checkbox pour editer le nom de l'uniform qu'on retrouvera dans le shader
#define ID_UNIFORM_WIDGET_UNIFORM_NAME_EDIT_BOX			1051 // checkbox pour editer le nom de l'uniform qu'on retrouvera dans le shader


// Time Widget : UniformTime
#define ID_UNIFORM_TIME_WIDGET_TIME_VALUE				1100 // valeur du temps actuel
#define ID_UNIFORM_TIME_WIDGET_PLAY_PAUSE_TIME_BTN		1101 // bouton pour mettre en pause ou pour jouer le temps
#define ID_UNIFORM_TIME_WIDGET_RESET_TIME_BTN			1102 // bouton pour remettre le temps à 0
#define ID_UNIFORM_TIME_WIDGET_INV_CHECKBOX				1103 // check box d'inversion du temps (compte / decompte)
#define ID_UNIFORM_TIME_WIDGET_MULT_COEF_VALUE			1104 // coef multiplicateur
#define ID_UNIFORM_TIMER								1105 // wxTimer

// Screen Widget : UniformScreen
#define ID_UNIFORM_SCREEN_WIDGET_WIDTH_VALUE			1106
#define ID_UNIFORM_SCREEN_WIDGET_HEIGHT_VALUE			1107
#define ID_UNIFORM_SCREEN_REFRESH_BTN					1108

// Texture2D Widget : UniformTex2D
#define ID_UNIFORM_TEX2D_WIDGET_DIR_BTN					1109
#define ID_UNIFORM_TEX2D_WIDGET_TRASH_BTN				1110
#define ID_UNIFORM_TEX2D_CHOICE_MAG						1111
#define ID_UNIFORM_TEX2D_CHOICE_MIN						1112
#define ID_UNIFORM_TEX2D_CHOICE_WRAPS					1113
#define ID_UNIFORM_TEX2D_CHOICE_WRAPT					1114
#define ID_UNIFORM_TEX2D_CHOICE_GENERATE_MIPMAP			1115
#define ID_UNIFORM_TEX2D_CHOICE_MIPMAP_BASE_LEVEL		1116
#define ID_UNIFORM_TEX2D_CHOICE_MIPMAP_MAX_LEVEL		1117
#define ID_UNIFORM_TEX2D_CHOICE_TEXTURE_INVERT_Y		1118

// TexturCubeD Widget : UniformTexCube
#define ID_UNIFORM_TEXCUBE_WIDGET_DIR_BTN				1119
#define ID_UNIFORM_TEXCUBE_WIDGET_TRASH_BTN				1120
#define ID_UNIFORM_TEXCUBE_WIDGET_1_BTN					1121
#define ID_UNIFORM_TEXCUBE_WIDGET_2_BTN					1122
#define ID_UNIFORM_TEXCUBE_WIDGET_3_BTN					1123
#define ID_UNIFORM_TEXCUBE_WIDGET_4_BTN					1124
#define ID_UNIFORM_TEXCUBE_WIDGET_5_BTN					1125
#define ID_UNIFORM_TEXCUBE_WIDGET_6_BTN					1126

// GamePad360 Widget : UniformGamePad360
#define ID_UNIFORM_GAMEPAD360_ACCUM_CHECKBOX			1127
#define ID_UNIFORM_GAMEPAD360_RESET_ALL					1128
#define ID_UNIFORM_GAMEPAD360_RESET_X					1129
#define ID_UNIFORM_GAMEPAD360_RESET_Y					1130
#define ID_UNIFORM_GAMEPAD360_RESET_Z					1131
#define ID_UNIFORM_GAMEPAD360_RESET_W					1132
#define ID_UNIFORM_GAMEPAD360_NAV_ORIGIN_X				1133
#define ID_UNIFORM_GAMEPAD360_NAV_ORIGIN_Y				1134
#define ID_UNIFORM_GAMEPAD360_NAV_ORIGIN_Z				1135
#define ID_UNIFORM_GAMEPAD360_NAV_DIRECTION_X			1136
#define ID_UNIFORM_GAMEPAD360_NAV_DIRECTION_Y			1137
#define ID_UNIFORM_GAMEPAD360_NAV_DIRECTION_Z			1138
#define ID_UNIFORM_GAMEPAD360_ACCUM_X					1139
#define ID_UNIFORM_GAMEPAD360_ACCUM_Y					1140
#define ID_UNIFORM_GAMEPAD360_ACCUM_Z					1141
#define ID_UNIFORM_GAMEPAD360_ACCUM_W					1142

// Bvh Widget : UniformBvh
#define ID_UNIFORM_BVH_WIDGET_DIR_BTN					1143
#define ID_UNIFORM_BVH_WIDGET_TRASH_BTN					1144

// Export Widget : UniformExport
#define ID_UNIFORM_EXPORT_WIDGET_TIME_VALUE				1145 // valeur du temps actuel
#define ID_UNIFORM_EXPORT_WIDGET_PLAY_PAUSE_TIME_BTN	1146 // bouton pour mettre en pause ou pour jouer le temps
#define ID_UNIFORM_EXPORT_WIDGET_RESET_TIME_BTN			1147 // bouton pour remettre le temps à 0
#define ID_UNIFORM_EXPORT_WIDGET_INV_CHECKBOX			1148 // check box d'inversion du temps (compte / decompte)
#define ID_UNIFORM_EXPORT_WIDGET_MULT_COEF_VALUE		1149 // coef multiplicateur
#define ID_UNIFORM_EXPORT_TIMER							1150 // wxTimer

// Export Widget : UniformFont
#define ID_UNIFORM_FONT_SELECT_SYSTEM_FONT				1151
#define ID_UNIFORM_FONT_SELECT_FILE_FONT				1152
#define ID_UNIFORM_FONT_CHANGE_PARAMS					1153
#define ID_UNIFORM_FONT_CHANGE_TEXT						1154
#define ID_UNIFORM_FONT_CHANGE_COLOR					1155

#endif //__XSHADEGLOBALS_H__