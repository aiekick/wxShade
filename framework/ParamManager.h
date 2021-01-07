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

#ifndef _PARAMMANAGER_H_
#define _PARAMMANAGER_H_

#include <Windows.h>

#include "Config.h"

#include "Tools\Tools.hpp"

#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <map>
#include <vector>
#include <sstream>
#include <exception>
using namespace std;

#include "tinyxml2/tinyxml2.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
#define LogStr(n) DebugLogger::Instance().LogString(n)
class DebugLogger
{
private:
	static DebugLogger m_instance;
	static ofstream *debugLogFile;
	static wofstream *wdebugLogFile;
	int64 lastTick;
public:
	static DebugLogger Instance()
	{
		return m_instance;
	}
	static std::vector<std::string> splitString(const std::string& text, char delimiter)
	{
		std::vector<std::string> result;

		std::string::size_type start = 0;
		std::string::size_type end = text.find(delimiter, start);

		while (end != string::npos)
		{
			std::string token = text.substr(start, end - start);

			result.push_back(token);

			start = end + 1;
			end = text.find(delimiter, start);
		}

		result.push_back(text.substr(start));

		return result;
	}
	static std::wstring s2ws(const std::string& s)
	{
		int len;
		int slength = (int)s.length() + 1;
		len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
		wchar_t* buf = new wchar_t[len];
		MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		std::wstring r(buf);
		delete[] buf;
		return r;
	}
	static bool ReplaceString(std::string& str, const std::string& oldStr, const std::string& newStr)
	{
		bool Finded = false;
		size_t pos = 0;
		while ((pos = str.find(oldStr, pos)) != std::string::npos){
			Finded = true;
			str.replace(pos, oldStr.length(), newStr);
			pos += newStr.length();
		}
		return Finded;
	}
	static std::vector<std::string::size_type> strContains(const std::string& text, const std::string& word)
	{
		std::vector<std::string::size_type> result;

		std::string::size_type loc = 0;
		while ((loc = text.find(word, loc)) != std::string::npos)
		{
			result.push_back(loc);
			loc += 1;
		}

		return result;
	}
	

public:
	DebugLogger(void);
	~DebugLogger(void);
	void LogString(string str);
	void LogString(wstring str);
	void Close();
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/*
typedef map<std::string, cVariant> map_string_variant;

class cfg;

class ParamManager
{
private:
	static ParamManager m_instance;
public:
	static map<std::string, cVariant> Vars;
	static map<std::string, map_string_variant> Texs;
	static cfg *config;
	static ParamManager Instance()
	{
		return m_instance;
	}
public:
	ParamManager(void);
	~ParamManager(void);
	
	//void parse(const char* pcFilename);
	void parse(const char* datas);
	void RecursParsing(XMLElement* pElem);
};


class cfg
{
public:
	cfg(bool default_param)
	{
		if (default_param == true)
		{
			//TITLE = string : Balancio
			FPS = 60.0f;
			BOT_MODE = false;
			SCREEN_WIDTH = 768;
			SCREEN_HEIGHT = 1280;
			SCREEN_WIDTH_REF = 480;
			SCREEN_HEIGHT_REF = 800;
			Time_Wait_For_Other_Contact = 0.1f;
			SCORE_RPS = 5;
			Text_Color = cColor(255, 255, 255, 255);

			Atmo_Thickness = 50;
			
			Gravity_Constant = 10.0f;

			Planet_Mass = 10.0f;
			Planet_Radius = 100;
			nPlanet_MAX = 100;

			Asteroid_Mass = 2.0f;
			Asteroid_Radius = 20;
			nAsteroid_MAX = 200;

			Hole_Mass = 100.0f;
			Hole_Radius = 50;
			nHole_MAX = 100;

			Particle_Mass = 1.0f;
			Particle_Radius = 5;
			nParticle_MAX = 10000;

			ParticleCalcMode = false;

			DiscretGeometrySegmentSize_Planet = 3.0f;
			DiscretGeometryPerturbat_Planet = 10.0f;
			Ground_Color_Planet = cColor(0, 255, 255, 255);
			Kernel_Color_Planet = cColor(255, 0, 255, 255);
			Mass_Planet = 100000.0f;
			AttractRadiusCoefOnGround_Planet = 1.5f;

			DiscretGeometrySegmentSize_Asteroid = 3.0f;
			DiscretGeometryPerturbat_Asteroid = 10.0f;
			Ground_Color_Asteroid = cColor(0, 255, 255, 255);
			Kernel_Color_Asteroid = cColor(255, 0, 255, 255);
			Mass_Asteroid = 10.0f;
		}
		else
		{
			SCREEN_WIDTH = ParamManager::Vars["SCREEN_WIDTH"].getF();
			SCREEN_HEIGHT = ParamManager::Vars["SCREEN_HEIGHT"].getF();
			SCREEN_WIDTH_REF = ParamManager::Vars["SCREEN_WIDTH_REF"].getF();
			SCREEN_HEIGHT_REF = ParamManager::Vars["SCREEN_HEIGHT_REF"].getF();
			FPS = ParamManager::Vars["FPS"].getF();
			BOT_MODE = ParamManager::Vars["BOT_MODE"].getB();
			Time_Wait_For_Other_Contact = ParamManager::Vars["Time_Wait_For_Other_Contact"].getF();
			SCORE_RPS = ParamManager::Vars["SCORE_RPS"].getF();
			BG_Color = ParamManager::Vars["BG_Color"].getColor();
			Text_Color = ParamManager::Vars["Text_Color"].getColor();

			Atmo_Thickness = ParamManager::Vars["Atmo_Thickness"].getF();

			Gravity_Constant = ParamManager::Vars["Gravity_Constant"].getF();

			Planet_Mass = ParamManager::Vars["Planet_Mass"].getF();
			Planet_Radius = ParamManager::Vars["Planet_Radius"].getF();
			nPlanet_MAX = ParamManager::Vars["nPlanet_MAX"].getF();

			Asteroid_Mass = ParamManager::Vars["Asteroid_Mass"].getF();
			Asteroid_Radius = ParamManager::Vars["Asteroid_Radius"].getF();
			nAsteroid_MAX = ParamManager::Vars["nAsteroid_MAX"].getF();

			Hole_Mass = ParamManager::Vars["Hole_Mass"].getF();
			Hole_Radius = ParamManager::Vars["Hole_Radius"].getF();
			nHole_MAX = ParamManager::Vars["nHole_MAX"].getF();

			Particle_Mass = ParamManager::Vars["Particle_Mass"].getF();
			Particle_Radius = ParamManager::Vars["Particle_Radius"].getF();
			nParticle_MAX = ParamManager::Vars["nParticle_MAX"].getF();

			ParticleCalcMode = ParamManager::Vars["ParticleCalcMode"].getB();

			DiscretGeometrySegmentSize_Planet = ParamManager::Vars["DiscretGeometrySegmentSize_Planet"].getF();
			DiscretGeometryPerturbat_Planet = ParamManager::Vars["DiscretGeometryPerturbat_Planet"].getF();
			Ground_Color_Planet = ParamManager::Vars["Ground_Color_Planet"].getColor();
			Kernel_Color_Planet = ParamManager::Vars["Kernel_Color_Planet"].getColor();
			Mass_Planet = ParamManager::Vars["Mass_Planet"].getF();
			AttractRadiusCoefOnGround_Planet = ParamManager::Vars["AttractRadiusCoefOnGround_Planet"].getF();

			DiscretGeometrySegmentSize_Asteroid = ParamManager::Vars["DiscretGeometrySegmentSize_Asteroid"].getF();
			DiscretGeometryPerturbat_Asteroid = ParamManager::Vars["DiscretGeometryPerturbat_Asteroid"].getF();
			Ground_Color_Asteroid = ParamManager::Vars["Ground_Color_Asteroid"].getColor();
			Kernel_Color_Asteroid = ParamManager::Vars["Kernel_Color_Asteroid"].getColor();
			Mass_Asteroid = ParamManager::Vars["Mass_Asteroid"].getF();
		}
		GAMERATIO = SCREEN_WIDTH / SCREEN_WIDTH_REF;
	}

public:
	float GAMERATIO;
	float SCREEN_WIDTH;
	float SCREEN_HEIGHT;
	float SCREEN_WIDTH_REF;
	float SCREEN_HEIGHT_REF;
	float FPS;
	bool BOT_MODE;
	float Time_Wait_For_Other_Contact;
	float SCORE_RPS;
	cColor BG_Color;
	cColor Text_Color;

	float Atmo_Thickness;

	float Gravity_Constant;

	float Planet_Mass;
	float Planet_Radius;
	float nPlanet_MAX;

	float Asteroid_Mass;
	float Asteroid_Radius;
	float nAsteroid_MAX;

	float Hole_Mass;
	float Hole_Radius;
	float nHole_MAX;

	float Particle_Mass;
	float Particle_Radius;
	float nParticle_MAX;

	map_string_variant Block_Bar_Tex;
	map_string_variant SUB_Block_Bar_Tex;
	map_string_variant Block_Deck_Tex;
	map_string_variant Block_Barrel_Tex;
	map_string_variant Block_Bullet_Tex;
	map_string_variant Block_Ground_Tex;
	map_string_variant Block_Roof_Tex;
	map_string_variant Block_LeftWall_Tex;
	map_string_variant Block_RightWall_Tex;

	bool BOX2D_DEBUG_DRAW;
	bool ParticleCalcMode;

	float DiscretGeometrySegmentSize_Planet;
	float DiscretGeometryPerturbat_Planet;
	cColor Ground_Color_Planet;
	cColor Kernel_Color_Planet;
	float Mass_Planet;
	float AttractRadiusCoefOnGround_Planet;

	float DiscretGeometrySegmentSize_Asteroid;
	float DiscretGeometryPerturbat_Asteroid;
	cColor Ground_Color_Asteroid;
	cColor Kernel_Color_Asteroid;
	float Mass_Asteroid;
};
*/
#endif /* _PARAMMANAGER_H_ */
