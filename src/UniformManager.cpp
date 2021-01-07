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

#include "UniformManager.h"

#include "DocVisualControlAbstract.h"

#include "ShaderView.h"
#include "UniformView.h"

#include "UniformTime.h"
#include "UniformScreen.h"
#include "UniformMouse.h"
#include "UniformTexture2D.h"
#include "UniformCubeMap.h"
#include "UniformColor.h"
#include "UniformSlider.h"
#include "UniformGamePad360.h"
#include "UniformBVH.h"
#include "UniformExport.h"
#include "UniformFont.h"

UniformManager::UniformManager(DocVisualControlAbstract* vParent) : m_wxParent(vParent), m_modified(false)
{

}

UniformManager::~UniformManager()
{

}
	
// converti UniformsWidgetTypeEnum vers string
string UniformManager::ConvertUniformsWidgetTypeEnumToString(UniformsWidgetTypeEnum vKey)
{
	string vName = "";

	switch (vKey)
	{
	case UniformsWidgetTypeEnum::USCREEN:
		vName = "USCREEN";
		break;
	case UniformsWidgetTypeEnum::UTIME:
		vName = "UTIME";
		break;
	case UniformsWidgetTypeEnum::UMOUSE:
		vName = "UMOUSE";
		break;
	case UniformsWidgetTypeEnum::UTEX2D:
		vName = "UTEX2D";
		break;
	case UniformsWidgetTypeEnum::UCUBEMAP:
		vName = "UCUBEMAP";
		break;
	case UniformsWidgetTypeEnum::UVIDEO:
		vName = "UVIDEO";
		break;
	case UniformsWidgetTypeEnum::UMIC:
		vName = "UMIC";
		break;
	case UniformsWidgetTypeEnum::UWEBCAM:
		vName = "UWEBCAM";
		break;
	case UniformsWidgetTypeEnum::USLIDER:
		vName = "USLIDER";
		break;
	case UniformsWidgetTypeEnum::UCURVE:
		vName = "UCURVE";
		break;
	case UniformsWidgetTypeEnum::UMUSIC:
		vName = "UMUSIC";
		break;
	case UniformsWidgetTypeEnum::UCOLOR:
		vName = "UCOLOR";
		break;
	case UniformsWidgetTypeEnum::UGAMEPAD360:
		vName = "UGAMEPAD360";
		break;
	case UniformsWidgetTypeEnum::UBVH:
		vName = "UBVH";
		break;
	case UniformsWidgetTypeEnum::UEXPORT:
		vName = "UEXPORT";
		break;
	case UniformsWidgetTypeEnum::UFONT:
		vName = "UFONT";
		break;
	}

	return vName;
}
	
// converti string vers UniformsWidgetTypeEnum
UniformsWidgetTypeEnum UniformManager::ConvertStringToUniformsWidgetTypeEnum(string vKey)
{
	UniformsWidgetTypeEnum type = UniformsWidgetTypeEnum::UNONE;
	
	if (vKey == "USCREEN") type = UniformsWidgetTypeEnum::USCREEN;
	else if (vKey == "UTIME") type = UniformsWidgetTypeEnum::UTIME;
	else if (vKey == "UMOUSE") type = UniformsWidgetTypeEnum::UMOUSE;
	else if (vKey == "UTEX2D") type = UniformsWidgetTypeEnum::UTEX2D;
	else if (vKey == "UCUBEMAP") type = UniformsWidgetTypeEnum::UCUBEMAP;
	else if (vKey == "UVIDEO") type = UniformsWidgetTypeEnum::UVIDEO;
	else if (vKey == "UMIC") type = UniformsWidgetTypeEnum::UMIC;
	else if (vKey == "UWEBCAM") type = UniformsWidgetTypeEnum::UWEBCAM;
	else if (vKey == "USLIDER") type = UniformsWidgetTypeEnum::USLIDER;
	else if (vKey == "UCURVE") type = UniformsWidgetTypeEnum::UCURVE;
	else if (vKey == "UMUSIC") type = UniformsWidgetTypeEnum::UMUSIC;
	else if (vKey == "UCOLOR") type = UniformsWidgetTypeEnum::UCOLOR;
	else if (vKey == "UGAMEPAD360") type = UniformsWidgetTypeEnum::UGAMEPAD360;
	else if (vKey == "UBVH") type = UniformsWidgetTypeEnum::UBVH;
	else if (vKey == "UEXPORT") type = UniformsWidgetTypeEnum::UEXPORT;
	else if (vKey == "UFONT") type = UniformsWidgetTypeEnum::UFONT;

	return type;
}

string UniformManager::GetBasenameForUniformsWidgetTypeEnum(UniformsWidgetTypeEnum vKey)
{
	string vName = "";
	
	switch(vKey)
	{
		case UniformsWidgetTypeEnum::USCREEN:
			vName = "uScreenSize";
			break;
		case UniformsWidgetTypeEnum::UTIME:
			vName = "uTime";
			break;
		case UniformsWidgetTypeEnum::UMOUSE:
			vName = "uMouse";
			break;
		case UniformsWidgetTypeEnum::UTEX2D:
			vName = "uTex2D";
			break;
		case UniformsWidgetTypeEnum::UCUBEMAP:
			vName = "uCubeMap";
			break;
		case UniformsWidgetTypeEnum::UVIDEO:
			vName = "uVideo";
			break;
		case UniformsWidgetTypeEnum::UMIC:
			vName = "uMic";
			break;
		case UniformsWidgetTypeEnum::UWEBCAM:
			vName = "uWebCam";
			break;
		case UniformsWidgetTypeEnum::USLIDER:
			vName = "uSlider";
			break;
		case UniformsWidgetTypeEnum::UCURVE:
			vName = "uCurve";
			break;
		case UniformsWidgetTypeEnum::UMUSIC:
			vName = "uMusic";
			break;
		case UniformsWidgetTypeEnum::UCOLOR:
			vName = "uColor";
			break;
		case UniformsWidgetTypeEnum::UGAMEPAD360:
			vName = "uGamePad360";
			break;
		case UniformsWidgetTypeEnum::UBVH:
			vName = "uBvh";
			break;
		case UniformsWidgetTypeEnum::UEXPORT:
			vName = "uExport";
			break;
		case UniformsWidgetTypeEnum::UFONT:
			vName = "uFont";
			break;
	}
	
	return vName;
}

UniformsTypeEnum UniformManager::GetGlslTypeForUniformsWidgetTypeEnum(UniformsWidgetTypeEnum vKey, int vCount)
{
	UniformsTypeEnum glslType = UniformsTypeEnum::UTYPENONE;
	
	switch(vKey)
	{
		case UniformsWidgetTypeEnum::USCREEN:
			glslType = UniformsTypeEnum::UVEC2;
			break;
		case UniformsWidgetTypeEnum::UTIME:
			glslType = UniformsTypeEnum::UFLOAT;
			break;
		case UniformsWidgetTypeEnum::UMOUSE:
			glslType = UniformsTypeEnum::UVEC4;
			break;
		case UniformsWidgetTypeEnum::UTEX2D:
			glslType = UniformsTypeEnum::USAMPLER2D;
			break;
		case UniformsWidgetTypeEnum::UCUBEMAP:
			glslType = UniformsTypeEnum::USAMPLERCUBE;
			break;
		case UniformsWidgetTypeEnum::UVIDEO:
			break;
		case UniformsWidgetTypeEnum::UMIC:
			break;
		case UniformsWidgetTypeEnum::UWEBCAM:
			break;
		case UniformsWidgetTypeEnum::USLIDER:
			if (vCount == 1) // mutation en float
				glslType = UniformsTypeEnum::UFLOAT;
			else if (vCount == 2) // mutation en vec2
				glslType = UniformsTypeEnum::UVEC2;
			else if (vCount == 3) // mutation en vec3
				glslType = UniformsTypeEnum::UVEC3;
			else if (vCount == 4) // mutation en vec4
				glslType = UniformsTypeEnum::UVEC4;
			break;
		case UniformsWidgetTypeEnum::UCURVE:
			break;
		case UniformsWidgetTypeEnum::UMUSIC:
			break;
		case UniformsWidgetTypeEnum::UCOLOR:
			glslType = UniformsTypeEnum::UVEC3;
			break;
		case UniformsWidgetTypeEnum::UGAMEPAD360:
			glslType = UniformsTypeEnum::UVEC4;
			break;
		case UniformsWidgetTypeEnum::UBVH:
			glslType = UniformsTypeEnum::UVEC4;
			break;
		case UniformsWidgetTypeEnum::UEXPORT:
			glslType = UniformsTypeEnum::UFLOAT;
			break;
		case UniformsWidgetTypeEnum::UFONT:
			glslType = UniformsTypeEnum::USAMPLER2D;
			break;
	}
	
	return glslType;
}

// upload selon le type glsl associé au type UniformsWidgetTypeEnum
void UniformManager::UploadUniformForGlslType(UniformsTypeEnum glslType, int loc, cUniformVariant v)
{
	if (glslType == UniformsTypeEnum::UFLOAT && loc > -1)
		glUniform1f(loc, v.x);
	if (glslType == UniformsTypeEnum::UVEC2 && loc > -1)
		glUniform2f(loc, v.x, v.y);
	if (glslType == UniformsTypeEnum::UVEC3 && loc > -1)
		glUniform3f(loc, v.x, v.y, v.z);
	if (glslType == UniformsTypeEnum::UVEC4 && loc > -1)
		glUniform4f(loc, v.x, v.y, v.z, v.w);
	if (glslType == UniformsTypeEnum::USAMPLER2D && loc > -1)
	{
		glActiveTexture(GL_TEXTURE0 + v.id);
		glBindTexture(GL_TEXTURE_2D, v.uSampler2D);
		glUniform1i(loc, v.id);
	}
	if (glslType == UniformsTypeEnum::USAMPLERCUBE && loc > -1)
	{
		glActiveTexture(GL_TEXTURE0 + v.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, v.uSamplerCube);
		glUniform1i(loc, v.id);
	}
}

// upload uniforms in shader
void UniformManager::UploadUniforms()
{
	// on doit avaoir fait le contexte gl en current avant
	// on doit avaoir fait le contexte gl en current avant
	// on doit avaoir fait le contexte gl en current avant
	// on doit avaoir fait le contexte gl en current avant
	// on doit avaoir fait le contexte gl en current avant
	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;
		cUniformVariant v = uni.value;
		
		if (uni.uType != UniformsTypeEnum::UTYPENONE)
		{
			UploadUniformForGlslType(uni.uType, uni.loc, v);
		}
		else
		{
			UniformsTypeEnum glslType = GetGlslTypeForUniformsWidgetTypeEnum(uni.wType, v.count);
			UploadUniformForGlslType(glslType, uni.loc, v);
		}
	}
}

// retourne la strucutre pour l'uniform grace a son nom
UniformStruct UniformManager::GetUniformStructByName(string vName)
{
	UniformStruct uni;
	if (m_UniMap.find(vName) != m_UniMap.end()) // trouvé
	{
		uni = m_UniMap[vName];
	}
	return uni;
}

string UniformManager::ConvertUniformsTypeEnumToString(UniformsTypeEnum vKey)
{
	if (vKey == UniformsTypeEnum::UFLOAT)
		return "float";
	if (vKey == UniformsTypeEnum::UVEC2)
		return "vec2";
	if (vKey == UniformsTypeEnum::UVEC3)
		return "vec3";
	if (vKey == UniformsTypeEnum::UVEC4)
		return "vec4";
	if (vKey == UniformsTypeEnum::USAMPLER2D)
		return "sampler2D";
	if (vKey == UniformsTypeEnum::USAMPLERCUBE)
		return "samplerCube";
	return "";
}

// retourne une string qui contient les uniform a inserer au script
ComplexStringStruct UniformManager::GetUniformsToString()
{
	ComplexStringStruct res;
	res.count = 0;
	
	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;
		cUniformVariant v = uni.value;

		UniformsTypeEnum type = UniformsTypeEnum::UTYPENONE;
		if (uni.uType != UniformsTypeEnum::UTYPENONE)
		{
			type = uni.uType;
		}
		else
		{
			type = GetGlslTypeForUniformsWidgetTypeEnum(uni.wType, v.count);
		}
		res.lines += "uniform " + ConvertUniformsTypeEnumToString(type) + " " + key + ";\n";
		res.count++;
	}
	
	return res;
}
	
// complete la struct avec le loc
bool UniformManager::CompleteMapWithLocFromShader(GLShader *vShader)
{
	bool res = false;

	if (vShader != 0)
	{
		for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
		{
			string key = it->first;
			UniformStruct uni = it->second;
			
			uni.loc = vShader->getUniformLocationForName(key.c_str());
			m_UniMap[key] = uni;
		}
		res = true;
	}
	else
	{
		//LogToOutput("Shader is NULL !!", false);
		res = false;
	}
	return res;
}
	
// mise a jour des uniform / met a jour la map et le widget d'un uniform precis
// retour l'erreur si erreur sion renvoit un string vide
string UniformManager::UpdateUniformByName(string vName, cUniformVariant vValue, bool vModified)
{
	string err;
	if ( m_UniMap.find(vName) != m_UniMap.end()) // trouvé
	{
		UniformStruct uni = m_UniMap[vName];
		
		uni.value = vValue;
		if ( uni.widget != 0)
			uni.widget->UpdateValue(vValue);
			
		m_UniMap[vName] = uni;

		if (vModified == true)
		{
			m_modified = true;
			m_wxParent->GetDocument()->SetModified(true);
		}
			
	}
	else
	{
		err = "The name < " + vName + " > not exist in the uniform bd !\n Failed to updating !";
	}
	return err;
}
	
// mise a jour des uniform / met a jour la map et le widget de tout les uniforms associé a un type
void UniformManager::UpdateUniformByType(UniformsWidgetTypeEnum vType, cUniformVariant vValue)
{
	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;
		
		if (uni.wType == vType)
		{
			uni.value = vValue;
			if ( uni.widget != 0)
				uni.widget->UpdateValue(vValue);
				
			m_UniMap[key] = uni;
		}
	}
}
	
// retourne un nom possible de uniform en fonction d'un type
string UniformManager::GetAValidUniformNameForType(UniformsWidgetTypeEnum vType, int *vIndex)
{
	string baseName = GetBasenameForUniformsWidgetTypeEnum(vType); // retourne le nom par defaut associé au type vType
	
	string newName = baseName;

	int idx = 0;
	while (m_UniMap.find(newName) != m_UniMap.end()) // tant que le nom est trouvé on continu
	{
		newName = baseName + toStr(++idx);// on increment l'index et on l'affecte au nom
	}
	
	*vIndex = idx;
	return newName;
}

// retourne un nom possible de uniform en fonction d'un type
string UniformManager::GetAValidUniformName(string vName, int *vIndex)
{
	string baseName = vName; // retourne le nom par defaut associé au type vType

	string newName = baseName;

	int idx = 0;
	while (m_UniMap.find(newName) != m_UniMap.end()) // tant que le nom est trouvé on continu
	{
		newName = baseName + toStr(++idx);// on increment l'index et on l'affecte au nom
	}

	*vIndex = idx;
	return newName;
}

// retourne le nombre de uniform dans la map pour le type donné
int UniformManager::CountItemOfType(UniformsWidgetTypeEnum vType)
{
	int count = 0;
	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;

		if (uni.wType == vType)
			count++;
	}

	return count;
}

// retourne unid disponible pour un type correspondant a une texture
int UniformManager::GetFreeIdForTypeCorrespondingToATexture()
{
	int idx = 0;
	
	int *tab = new int[14]; // 13 est la limite haute du nombre de texture disponible
	for (int i = 0; i < 14; i++){tab[i] = -1;}

	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;

		if (uni.wType == UTEX2D || uni.wType == UCUBEMAP || uni.wType == UVIDEO || uni.wType == UWEBCAM || uni.wType == UFONT)
		{
			tab[uni.id] = uni.id;
		}
	}

	for (int i = 0; i < 13; i++)
	{
		idx = i;
		if (tab[i] == -1)
		{
			break;
		}
	}

	return idx;
}
// remove uniform by name from map
bool UniformManager::RemoveUniformByName(string vName)
{
	bool res = false;

	if (m_UniMap.find(vName) != m_UniMap.end()) // item trouvé
	{
		UniformStruct uni = m_UniMap[vName];
		//SAFE_DELETE(uni.widget);
		size_t countErased = m_UniMap.erase(vName);
		if (countErased > 0)
		{
			res = true;
			if (countErased > 1)
			{
				//on a effacé l'item mais plus d'un a chequer donc
				LogStr("UniformManager::RemoveUniformByName(" + vName + ") => " + toStr(countErased) + " Items effacés au lieu de 1 seul...");
			}
		}
	}

	return res;
}
	
// WIDGET
// cree le widget correspondant à la demande avec comme parent m_wxParent
UniformAbstract* UniformManager::CreateUniformWidget(UniformsWidgetTypeEnum vType)
{
	UniformAbstract* ptr = 0;
	
	switch (vType)
	{
		case UniformsWidgetTypeEnum::USCREEN:
			ptr = new UniformScreen(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL,0,0,this);
			break;
		case UniformsWidgetTypeEnum::UTIME:
			ptr = new UniformTime(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UMOUSE:
			ptr = new UniformMouse(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UTEX2D:
			ptr = new UniformTexture2D(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UCUBEMAP:
			ptr = new UniformCubeMap(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UVIDEO:
			break;
		case UniformsWidgetTypeEnum::UMIC:
			break;
		case UniformsWidgetTypeEnum::UWEBCAM:
			break;
		case UniformsWidgetTypeEnum::USLIDER:
			ptr = new UniformSlider(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UCURVE:
			break;
		case UniformsWidgetTypeEnum::UMUSIC:
			break;
		case UniformsWidgetTypeEnum::UCOLOR:
			ptr = new UniformColor(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UGAMEPAD360:
			ptr = new UniformGamePad360(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UBVH:
			ptr = new UniformBVH(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UEXPORT:
			ptr = new UniformExport(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
		case UniformsWidgetTypeEnum::UFONT:
			ptr = new UniformFont(m_wxParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSTATIC_BORDER | wxTAB_TRAVERSAL, 0, 0, this);
			break;
	}

	cAssert(ptr == 0, "UniformManager::CreateUniformWidget error => le type " + ConvertUniformsWidgetTypeEnumToString(vType) + " n'a pas de class de widget !");
	if (ptr != 0)
	{
		ptr->SetType(vType);

	}

	return ptr;
}

// FROM SCRIPT
string UniformManager::AddUniformFromScript(UniformsWidgetTypeEnum vType, map<string, string> vParams)
{
	string vName;

	int idx = 0;
	vName = GetAValidUniformNameForType(vType, &idx);

	if (m_UniMap.find(vName) == m_UniMap.end()) // non trouvés
	{
		UniformStruct uni;
		uni.name = vName;
		uni.wType = vType;
		uni.uType = UniformsTypeEnum::UTYPENONE;
		uni.CanBeSaved = true;
		uni.params = vParams;
		uni.loc = -1;
		uni.widget = 0;
		uni.value = cUniformVariant();
		uni.id = GetFreeIdForTypeCorrespondingToATexture();

		m_UniMap[vName] = uni;
	}
	else
		vName = "";

	return vName;
}

// FROM FILE
bool UniformManager::AddUniformFromFile(string vName, UniformsWidgetTypeEnum vType, map<string, string> vParams)
{
	bool res = true;
	
	if ( m_UniMap.find(vName) == m_UniMap.end()) // non trouvés
	{
		UniformStruct uni;
		uni.name = vName;
		uni.wType = vType;
		uni.uType = UniformsTypeEnum::UTYPENONE;
		uni.CanBeSaved = true;
		uni.params = vParams;
		uni.loc = -1;
		uni.widget = 0;
		uni.value = cUniformVariant();
		uni.id = GetFreeIdForTypeCorrespondingToATexture();
		uni.value.count = 1;
		m_UniMap[vName] = uni;
	}
	else
		res = false;

	return res;
}

// FROM WIDGET
bool UniformManager::AddUniformFromWidget(UniformsWidgetTypeEnum vType, UniformAbstract *vWidget)
{
	bool res = false;
	
	if ( vWidget != 0)
	{
		int idx = 0;
		string vName = GetAValidUniformNameForType(vType, &idx);

		vWidget->AddUniformName(vName);
		
		UniformStruct uni;

		if ( m_UniMap.find(vName) == m_UniMap.end()) // non trouvés
		{
			uni.name = vName;
			uni.wType = vType;
			uni.uType = UniformsTypeEnum::UTYPENONE;
			uni.CanBeSaved = true;
			uni.loc = -1;
			uni.widget = vWidget;
			uni.value = cUniformVariant();
			uni.id = GetFreeIdForTypeCorrespondingToATexture();

			vWidget->m_typeId = uni.id;

			m_UniMap[vName] = uni;
		
			res = true;
		}/*
		else
		{
			uni = m_UniMap[vName];
			uni.widget = vWidget;

			res = true;
		}*/
	}
	
	return res;
}

// FROM WIDGET
string UniformManager::AddUniformTypeFromWidget(string vName, UniformsTypeEnum vType, UniformAbstract *vWidget, bool vCanBeSaved)
{
	string uniformName;

	if (vWidget != 0)
	{
		int idx = 0;
		uniformName = GetAValidUniformName(vName, &idx);

		//vWidget->SetUniformName(vName);

		UniformStruct uni;

		if (m_UniMap.find(uniformName) == m_UniMap.end()) // non trouvés
		{
			uni.name = uniformName;
			uni.wType = vWidget->GetType();
			uni.uType = vType;
			uni.CanBeSaved = vCanBeSaved;
			uni.loc = -1;
			uni.widget = vWidget;
			uni.value = cUniformVariant();
			uni.id = GetFreeIdForTypeCorrespondingToATexture();

			vWidget->m_typeId = uni.id;

			m_UniMap[uniformName] = uni;

			uniformName = uniformName;
		}
	}

	return uniformName;
}

// complete la struct avec les widgets et les ajoute dans l'uniform view
bool UniformManager::CompleteUniformsWithCorrespondingWidget(UniformView *vUniformView, ShaderView *vShaderView)
{
	bool res = false;

	if (vUniformView == 0 || vShaderView == 0) return false;

	if (m_UniMap.size() == 0) return true; // si la map est vide, c'est qu'il n'y a pas de uniform relié au shader alors ce n'est pas une erreur

	for (std::map<string, UniformStruct>::iterator it = m_UniMap.begin(); it != m_UniMap.end(); ++it)
	{
		string key = it->first;
		UniformStruct uni = it->second;

		if (uni.CanBeSaved == true)
		{
			UniformAbstract *widget = CreateUniformWidget(uni.wType);
			if (widget != 0)
			{
				widget->AddUniformName(key);
				uni.widget = widget;
				widget->m_typeId = uni.id;
				widget->SetParamsFromXML(uni.params);
				m_UniMap[key] = uni;

				widget->SetShaderView(vShaderView);
				widget->SetShaderRenderer(vShaderView->GetShaderRenderer());
				vUniformView->AddPane(widget);

				res = true;
			}
			else
			{
				res = false;
			}
		}
	}

	return res;
}

string UniformManager::GetXMLParamsByName(string vName, DocumentFileFormat vNewFileFormat, DocumentFileFormat vOldFileFormat)
{
	string res;

	if (m_UniMap.find(vName) != m_UniMap.end()) // item trouvé
	{
		UniformStruct uni = m_UniMap[vName];
		
		// si le unifrom peut etre sauvé
		if (uni.CanBeSaved == true)
		{
			if (uni.widget != 0)
			{
				res = uni.widget->GetParamsToXMLString(vNewFileFormat, vOldFileFormat);
			}
		}
	}

	return res;
}

string UniformManager::ChangeUniformName(string vCurrentName, string vNewName)
{
	string err;
	if (m_UniMap.find(vNewName) == m_UniMap.end()) // item trouvé
	{
		if (m_UniMap.find(vCurrentName) != m_UniMap.end()) // item trouvé
		{
			UniformStruct uni = m_UniMap[vCurrentName];
			uni.name = vNewName;
			m_UniMap.erase(vCurrentName);
			m_UniMap[vNewName] = uni;
		}
		else // probleme vCurrentName n'est pas utilisé actuellement... qu'est ce qu'il ce passe...
		{
			err = " The actual name < " + vCurrentName + " > is not actually used.. Weird ! \n can't continue the renaming !";
		}
	}
	else // vNewName est deja le nom d'un uniform et ne peut etre reutilié
	{
		err = " < " + vNewName + " > is already used by another uniform !";
	}

	return err;
}