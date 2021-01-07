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

#ifndef __XSHADEASSETS_H__
#define __XSHADEASSETS_H__

#include "Globals.h"
#include <map>
#include <wx/bitmap.h>

///////////////////////////////////////////////////////////////////////////
// XnView pour convertir en XPM
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/Image_small_icon.xpm"
#include "../icons/20x20/Page_small_icon.xpm"
///////////////////////////////////////////////////////////////////////////
#include "../icons/Error_icon.xpm"
#include "../icons/GearIn_icon.xpm"
#include "../icons/Page_icon.xpm"
#include "../icons/PageAdd_icon.xpm"
#include "../icons/PageDelete_icon.xpm"
#include "../icons/PageSave_icon.xpm"
#include "../icons/Layout_icon.xpm"
#include "../icons/Help_icon.xpm"
#include "../icons/Information_icon.xpm"
#include "../icons/SettingTools_icon.xpm"
#include "../icons/PictureSave_icon.xpm"
///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/Layers_small_icon.xpm"
#include "../icons/20x20/Font_small_icon.xpm" // font
#include "../icons/20x20/ScreenError_small_icon.xpm" // screen size
#include "../icons/20x20/MouseSelectScroll_small_icon.xpm" // mouse
#include "../icons/20x20/Picture_small_icon.xpm" // texture2d
#include "../icons/20x20/floodIt_small_icon.xpm" // texturecube
#include "../icons/20x20/Film_small_icon.xpm" // film
#include "../icons/20x20/Microphone_small_icon.xpm" // mic
#include "../icons/20x20/Hslider_small_icon.xpm" // slider
#include "../icons/20x20/DrawVertex_small_icon.xpm" // curve
#include "../icons/20x20/Webcam_small_icon.xpm" // webcam
#include "../icons/20x20/Music_small_icon.xpm" // music
#include "../icons/20x20/Time_small_icon.xpm" // Time
#include "../icons/20x20/ColorWheel_small_icon.xpm"// color
#include "../icons/20x20/Controller_small_icon.xpm"// controller
#include "../icons/20x20/Walk_small_icon.xpm"// bvh
#include "../icons/20x20/WrappingBetween_small_icon.xpm" // export (video, gif, swf, etc..)
#include "../icons/20x20/Cog_small_icon.xpm" // config btn dans la sahderview
///////////////////////////////////////////////////////////////////////////
#include "../icons/20x20/ControlPlayBlue_small_icon.xpm"
#include "../icons/20x20/ControlPauseBlue_small_icon.xpm"
#include "../icons/20x20/Bug_small_icon.xpm" // debug mode
#include "../icons/20x20/TransformCrop_small_icon.xpm" // crop mode
///////////////////////////////////////////////////////////////////////////
#include "../icons/psd/Layout0_icon.xpm"
#include "../icons/psd/Layout1_icon.xpm"
#include "../icons/psd/Layout2_icon.xpm"
#include "../icons/psd/Layout3_icon.xpm"
#include "../icons/psd/Layout4_icon.xpm"
#include "../icons/psd/Layout5_icon.xpm"
#include "../icons/psd/Layout6_icon.xpm"
///////////////////////////////////////////////////////////////////////////
#include "../icons/psd/AutoIndent_icon.xpm"
#include "../icons/ApplicationXpTerminal_icon.xpm"
#include "../icons/psd/PerfCompiling_icon.xpm"
#include "../icons/Fire_icon.xpm"
///////////////////////////////////////////////////////////////////////////

class AssetsManager
{
public:
	static std::map<UniformsWidgetTypeEnum, wxBitmap> gUniformIcon;
};

#endif //__XSHADEASSETS_H__