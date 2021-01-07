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

#ifndef __UNIFORMMANAGER_H__
#define __UNIFORMMANAGER_H__


#include "Globals.h"

class DocVisualControlAbstract;

#include "..\framework\Config.h"
#include "..\framework\GLShaderRenderer.h"
#include "..\framework\Tools\Tools.hpp"

#include <string>
using namespace std;

class UniformView;
class ShaderView;

#include "UniformAbstract.h"

struct UniformStruct
{
	string name;
	UniformsWidgetTypeEnum wType;
	UniformsTypeEnum uType;
	int loc;
	UniformAbstract *widget;
	cUniformVariant value;
	map<string, string> params;
	int id;
	bool CanBeSaved;
};

struct ComplexStringStruct
{
	string lines;
	int count;
};

class UniformManager
{
public:
	UniformManager(DocVisualControlAbstract* vParent);
	~UniformManager();

	DocVisualControlAbstract* GetParent() { return m_wxParent; }

public:
	// converti UniformsWidgetTypeEnum vers string
	string ConvertUniformsWidgetTypeEnumToString(UniformsWidgetTypeEnum vKey);
	
	// convertit le type d'unifrom enum en string
	string ConvertUniformsTypeEnumToString(UniformsTypeEnum vKey);

	// converti string vers UniformsWidgetTypeEnum
	UniformsWidgetTypeEnum ConvertStringToUniformsWidgetTypeEnum(string vKey);

	// retourne le nom par default associé au type UniformsWidgetTypeEnum
	string GetBasenameForUniformsWidgetTypeEnum(UniformsWidgetTypeEnum vKey);
	
	// retourne le type de donnée GLSL associé au type UniformsWidgetTypeEnum
	UniformsTypeEnum GetGlslTypeForUniformsWidgetTypeEnum(UniformsWidgetTypeEnum vKey, int vCount);

	// upload selon le type glsl associé au type UniformsWidgetTypeEnum
	void UploadUniformForGlslType(UniformsTypeEnum glslType, int loc, cUniformVariant v);
	
	// upload uniforms in shader
	void UploadUniforms(); 

	// retourne une string qui contient les uniform a inserer au script
	ComplexStringStruct GetUniformsToString();
	
	// complete la struct avec les widgets et les ajoute dans l'uniform view
	bool CompleteUniformsWithCorrespondingWidget(UniformView *vUniformView, ShaderView *vShaderView);

	// complete la struct avec le loc
	bool CompleteMapWithLocFromShader(GLShader *vShader); 
	
	// mise a jour des uniform / met a jour la map et le widget d'un uniform precis
	string UpdateUniformByName(string vName, cUniformVariant vValue, bool vModified);
	
	// mise a jour des uniform / met a jour la map et le widget de tout les uniforms associé a un type
	void UpdateUniformByType(UniformsWidgetTypeEnum vType, cUniformVariant vValue);
	
	// retourne un nom possible de uniform en fonction d'un type
	string GetAValidUniformNameForType(UniformsWidgetTypeEnum vType, int *vIndex);
	string GetAValidUniformName(string vName, int *vIndex);

	// retourne le nombre de uniform dans la map pour le type donné
	int CountItemOfType(UniformsWidgetTypeEnum vType);

	// retourne unid disponible pour un type correspondant a une texture
	int GetFreeIdForTypeCorrespondingToATexture();
	
	// remove uniform by name from map
	bool RemoveUniformByName(string vName);
	
	// retourne la strucutre pour l'uniform grace a son nom
	UniformStruct GetUniformStructByName(string vName);

	// WIDGET
	// cree le widget correspondant à la demande avec comme parent m_wxParent
	UniformAbstract* CreateUniformWidget(UniformsWidgetTypeEnum vType);
	
	// FROM SCRIPT
	string AddUniformFromScript(UniformsWidgetTypeEnum vType, map<string, string> vParams);

	// FROM FILE
	bool AddUniformFromFile(string vName, UniformsWidgetTypeEnum vType, map<string, string> vParams);

	// FROM WIDGET
	bool AddUniformFromWidget(UniformsWidgetTypeEnum vType, UniformAbstract *vWidget);
	string AddUniformTypeFromWidget(string vName, UniformsTypeEnum vType, UniformAbstract *vWidget, bool vCanBeSaved);

	// get map
	map<string, UniformStruct> GetMap() { return m_UniMap; }

	// get xml param on string format from widget
	string GetXMLParamsByName(string vName, DocumentFileFormat vNewFileFormat, DocumentFileFormat vOldFileFormat);

	// indique si les uniform on été modifié
	bool IsModified() { return m_modified; }

	// change le nom de l'unform par le nouveau
	// renvoie le l'erreur si erreur il y a
	// sinon renvoi un sring vide si tous c'est bien passé
	string ChangeUniformName(string vCurrentName, string vNewName);

private:
	// associe le nom de l'uniform et UniFloat qui contient la valeur que le client met a jour et son loc dans le shader => associé lors de la creation du shader
	map<string, UniformStruct> m_UniMap; 
	bool m_modified;

protected:
	DocVisualControlAbstract* m_wxParent;
};

#endif //__UNIFORMMANAGER_H__