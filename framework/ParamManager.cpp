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

#include "ParamManager.h"

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// singleton
DebugLogger DebugLogger::m_instance = DebugLogger();
ofstream *DebugLogger::debugLogFile = new ofstream("debug.log", ios::out);
//wofstream *DebugLogger::wdebugLogFile = new wofstream("wdebug.log", ios::out);
DebugLogger::DebugLogger(void) 
{
	lastTick = GetTickCount64();
}
DebugLogger::~DebugLogger(void) {}
void DebugLogger::LogString(string str)
{
	if (str.size() > 0)
	{
		int64 ticks = GetTickCount64();
		float time = (ticks - lastTick) / 1000.0f;
		std::cout << "t:" << time << "s " << str << std::endl;
		if (debugLogFile->bad() == false)
			*debugLogFile << "t:" << time << "s " << str << std::endl;
	}
}
void DebugLogger::LogString(wstring str)
{
	if (str.size() > 0)
	{
		//int ticks = 0;// SDL_GetTicks();
		//float time = ticks / 1000.0f;
		//std::cout << "t:" << time << "s " << str << std::endl;
		//if (wdebugLogFile->bad() == false)
		//	*wdebugLogFile << "t:" << time << "s " << str << std::endl;
	}
}
void DebugLogger::Close()
{
	debugLogFile->close();
}

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
/*
// singleton
ParamManager ParamManager::m_instance = ParamManager();
map<std::string, cVariant> ParamManager::Vars = map<std::string, cVariant>();
map<std::string, map_string_variant> ParamManager::Texs = map<std::string, map_string_variant>();
cfg* ParamManager::config = NULL;
ParamManager::ParamManager(void){}
ParamManager::~ParamManager(void){}

/*
void ParamManager::parse(const char* pcFilename)
{
	// first load the xml file
	ticpp::Document doc(pcFilename);
	doc.LoadFile();
	
	RecursParsing(doc.FirstChildElement(false));
}*/
/*
void ParamManager::parse(const char* datas)
{/*
	try
	{
		// first load the xml file
		ticpp::Document doc;
		doc.Parse(datas);
	
		RecursParsing(doc.FirstChildElement(false));
	}
	catch (std::exception& ex)
	{
		string err = "ParamManager::parse(const char* datas) => " + toStr(ex.what());
		LogStr(err);
	}
	*//*
}
/*
void ParamManager::RecursParsing(XMLElement* pElem)
{/*
	std::string str = "";
	
	// The value of this child identifies the name of this element
	std::string strName = "";
	std::string strValue = "";
	std::string strText = "";
	std::string id = "";
	std::string type = "";

	bool isColor = false;
	cColor color; 
	
	bool isTexture = false;
	std::string idTex = "";
	cTexture texture;

	bool isBool = false;
	
	pElem->GetValue(&strName);

	if (strName == "VAR")
	{
		strText = pElem->GetText(false);

		str = "var : ";
		// now parse through all the attributes of this fruit
		ticpp::Iterator< ticpp::Attribute > attribute;
		for (attribute = attribute.begin(pElem); attribute != attribute.end(); attribute++)
		{
			attribute->GetName(&strName);
			attribute->GetValue(&strValue);
			if (isColor == true)
			{
				if (strName == "r" && isColor == true) { color.setColor('r',atoi(strValue.c_str())); str += "(" + strName + "=" + strValue ; }
				else if (strName == "g" && isColor == true) { color.setColor('g', atoi(strValue.c_str())); str += ", " + strName + "=" + strValue; }
				else if (strName == "b" && isColor == true) { color.setColor('b', atoi(strValue.c_str())); str += ", " + strName + "=" + strValue; }
				else if (strName == "a" && isColor == true)
				{
					color.setColor('a', atoi(strValue.c_str()));
					str += ", " + strName + "=" + strValue + ")";
					Vars[id] = color;
				}
			}
			if (isTexture == true)
			{
				if (strName == "idTex") { idTex = strValue; str += "(" + strName + "=" + strValue; }
				else if (strName == "format") { texture.format = strValue; str += ", " + strName + "=" + strValue; }
				else if (strName == "relativPath") { texture.relativPath = strValue.c_str(); str += ", " + strName + "=" + strValue; }
				else if (strName == "zOrder") { texture.zOrder = atof(strValue.c_str()); str += ", " + strName + "=" + strValue; }
				else if (strName == "step") { texture.step = atof(strValue.c_str()); str += ", " + strName + "=" + strValue; }
				else if (strName == "w") { texture.w = atof(strValue.c_str()); str += ", " + strName + "=" + strValue; }
				else if (strName == "h") { texture.h = atof(strValue.c_str()); str += ", " + strName + "=" + strValue; }
				else if (strName == "x") { texture.x = atof(strValue.c_str()); str += ", " + strName + "=" + strValue; }
				else if (strName == "y")
				{
					texture.y = atof(strValue.c_str());
					str += ", " + strName + "=" + strValue + ")";
					Texs[id][idTex] = texture;
				}
			}
			if (isBool == true)
			{
				Vars[id] = strValue;
				//if ( strValue == "true" ) Vars[id] = "1";
				//else if ( strValue == "false" ) Vars[id] = "0";
				str += strValue;
			}
			if (strName == "id")
			{
				id = strValue;
				str += id + "=";
			}
			else
				if (strName == "type") // le type sert juste pour le parsing. et pour le moment c'est seulement pour color qu'on a pr�vu un truc particulier
				{
					type = strValue;
					if (strValue == "color")
					{
						isColor = true;
					}
					if (strValue == "bool")
					{
						isBool = true;
					}
					str += type;
				}
			
		}
		if (strText != "")
		{
			str += ":" + strText;
			Vars[id] = strText;
		}
		
	}
	else
	{
		str = "section : " + strName;
	}
		
	LogStr(str);

	// CHILDS 
	// parse through all fruit elements
	ticpp::Iterator<ticpp::Element> child;
	for(child = child.begin(pElem); child != child.end(); child++)
	{
		RecursParsing(child->ToElement());
	}
	*//*
}
*/
