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

#include "GLShaderManager.h"

GLShaderManager* g_pShaderManager = 0;

GLShaderManager::GLShaderManager()
{

}

GLShaderManager::~GLShaderManager()
{
	for (std::map<std::string, GLShader*>::iterator iterator = shaderCache.begin(); iterator != shaderCache.end(); iterator++)
	{
		if (iterator->second != 0)
		{
			delete iterator->second;
			iterator->second = 0;
		}
	}
	shaderCache.clear();
}

bool GLShaderManager::AddShader(GLShader *_shader, string _key)
{
	if (_shader == 0) return false;
	shaderCache[_key] = _shader;
	return true;
}

GLShader* GLShaderManager::getShader(string key)
{
	GLShader *shader = shaderCache[key];
	return shader;
}