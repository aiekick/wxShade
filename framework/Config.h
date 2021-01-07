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

#ifndef _config_H_
#define _config_H_

#include <glad/glad.h>

#include "ParamManager.h"

#include "../src/Globals.h"

#define CHECK_GL_ERROR_DEBUG()do{GLenum __error=glGetError();if(__error){LogStr("OpenGL error :"+toStr((int)__error)+", file:"+toStr(__FILE__)+", func:"+toStr(__FUNCTION__)+", line:"+toStr(__LINE__));}}while(false);

#define SAFE_DELETE(ptr) if (ptr != 0) { delete(ptr); ptr = 0; }
#define IFEXIST(ptr) if (ptr !=0) ptr
#define kBackground 0
#define kMiddleground 1
#define kForeground 2

/*
static float maxValue(float A, float B)
{
	if (A > B) return A;
	else return B;
}
*/
/*
static string convertInt(int number)
{
	stringstream ss;//create a stringstream
	ss << number;//add number to the stream
	return ss.str();//return a string with the contents of the stream
}
*/
// COLLIDE FILTERING
// http://www.aurelienribon.com/blog/2011/07/box2d-tutorial-collision-filtering/
#define Box2DCollideFilteringCategoryNONE 0x0001
#define Box2DCollideFilteringCategorySTATIC 0x0002
#define Box2DCollideFilteringCategoryDYNAMIC 0x0004
#define Box2DCollideFilteringCategoryBULLET 0x0008
#define Box2DCollideFilteringCategoryBAR 0x0010
#define Box2DCollideFilteringCategoryWALL 0x0020
#define Box2DCollideFilteringCategoryGROUND 0x0040

// SCREEN RATIO AND DISPLAY
#define PTM_RATIO 150.0f // Box2d est optimis� pour des objet inferieur � 10 donc 80 pour 800/80 = 10. 800 est la plus grand longueur du jeu
#define WORLD_TO_LOCAL(n) ((n) / PTM_RATIO)
#define WTOL(n) ((n) / PTM_RATIO)
#define LOCAL_TO_WORLD(n) ((n) * PTM_RATIO)
#define LTOW(n) ((n) * PTM_RATIO)

// DEG / RAD
#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

#endif // _ParamManager::config_H__
