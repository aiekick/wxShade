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

#include "ShaderArtDocument.h"

#include "UniformManager.h"

#include "MainFrame.h"

#include <wx/filename.h>

#include <wx/base64.h> 
#include <wx/buffer.h>
#include <wx/mstream.h>

ShaderArtDocument::ShaderArtDocument(string vFileName, UniformManager *vUniformManager) 
: DocumentAbstract(vFileName), m_UniformManager(vUniformManager), m_GetOnlyShaderArtType(false), 
m_ShaderArtType(SHADER_TYPE_UNKNOW), m_BuildNumber(0), m_FileName(vFileName)
{
	// default fragment
	// https://www.shadertoy.com/api/y/shaders/shaderID?key=NtnKwr
}

ShaderArtDocument::~ShaderArtDocument()
{

}
	
bool ShaderArtDocument::NewDocument(ShaderEditorTemplateEnum vShaderEditorTemplate, string vFilePathName)
{
	bool res = false;

	map<string, string> p; // param à zero

	if (vShaderEditorTemplate == ShaderEditorTemplateEnum::TEMPLATE_QUAD)
	{
		if (m_UniformManager->AddUniformFromFile("uTime", UniformsWidgetTypeEnum::UTIME, p) == true)
		{
			if (m_UniformManager->AddUniformFromFile("uScreenSize", UniformsWidgetTypeEnum::USCREEN, p) == true)
			{
				SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_FRAGMENT, "void main(void)\n{\n\tvec2 uv = gl_FragCoord.xy / uScreenSize;\n\tgl_FragColor = vec4(uv, 0.5 + 0.5*sin(uTime), 1.0);\n}\n");
				SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_VERTEX, "attribute vec2 pos;\nvoid main()\n{\n\tgl_Position = vec4(pos.xy, 0.0, 1.0);\n}\n");

				SetRenderMode("TRIANGLES");

				m_NewDocument = true;

				SetModified(true);

				res = true;
			}
		}
	}
	else if (vShaderEditorTemplate == ShaderEditorTemplateEnum::TEMPLATE_POINTS)
	{
		if (m_UniformManager->AddUniformFromFile("uTime", UniformsWidgetTypeEnum::UTIME, p) == true)
		{
			if (m_UniformManager->AddUniformFromFile("uScreenSize", UniformsWidgetTypeEnum::USCREEN, p) == true)
			{
				SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_FRAGMENT, "varying vec4 v_color;\nvoid main()\n{\n\tgl_FragColor = v_color;\n}\n");
				string vertexString = "attribute float vertexId;//max 50000\nvarying vec4 v_color;\nvoid main()\n{\n";
				vertexString += "\tfloat astep = 3.14159 * 2.0 / 70.;\n\tfloat a = astep * float(vertexId) * (uTime+10.) * .3;\n\tvec2 d = a  * vec2(cos(a), sin(a)) / 100.;";
				vertexString += "\n\td.x *= uScreenSize.y/uScreenSize.x;\n\tif (vertexId<150.) gl_Position=vec4(d,0,1);\n\telse gl_Position=vec4(0,0,0,1);\n\tv_color=vec4(1,1,1,1);\n}\n";
				SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_VERTEX, vertexString);

				SetRenderMode("LINES");

				m_NewDocument = true;

				SetModified(true);

				res = true;
			}
		}
	}
	/*else if (vShaderEditorTemplate == ShaderEditorTemplateEnum::TEMPLATE_MUSIC)
	{
		string musicShader = "vec2 mainSound(float time)\n{\n\treturn vec2(sin(6.2831*440.0*time)*exp(-3.0*time));\n}\n\n";
		musicShader += "void main()\n{\n\tfloat t = iBlockOffset + (gl_FragCoord.x + gl_FragCoord.y*512.0) / 44100.0;\n\tvec2 y = mainSound(t);\n\tvec2 v = floor((0.5 + 0.5*y)*65536.0);\n\tvec2 vl = mod(v, 256.0) / 255.0;\n\tvec2 vh = floor(v / 256.0) / 255.0;\n\tgl_FragColor = vec4(vl.x, vh.x, vl.y, vh.y);\n}";
		SetStringToFileMemory(ShaderEditorTemplateEnum::TEMPLATE_MUSIC, musicShader);
		SetModified(true);
		res = true;
	}*/

	return res;
}

string ShaderArtDocument::GetFileMemoryToString(ShaderTypeEnum vShaderTypeEnum)
{
	string res;

	if (vShaderTypeEnum == ShaderTypeEnum::SHADER_TYPE_FRAGMENT)
	{
		return m_FragmentString;
	}
	else if (vShaderTypeEnum == ShaderTypeEnum::SHADER_TYPE_VERTEX)
	{
		return m_VertexString;
	}
	/*else if (vShaderTypeEnum == ShaderTypeEnum::TEMPLATE_MUSIC)
	{
		return m_MusicShaderString;
	}*/

	return res;
}

void ShaderArtDocument::SetStringToFileMemory(ShaderTypeEnum vShaderTypeEnum, string vTxt)
{
	if (vShaderTypeEnum == ShaderTypeEnum::SHADER_TYPE_FRAGMENT)
	{
		m_FragmentString = vTxt;
	}
	else if (vShaderTypeEnum == ShaderTypeEnum::SHADER_TYPE_VERTEX)
	{
		m_VertexString = vTxt;
	}
	/*else if (vShaderEditorTemplate == ShaderEditorTemplateEnum::TEMPLATE_MUSIC)
	{
		m_MusicShaderString = vTxt;
	}*/
}

bool ShaderArtDocument::LoadDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat, bool vGetOnlyType)
{
	bool res = false;

	ifstream docFile(vFilePathName, ios::in);
	if (docFile.is_open())
	{
		m_FilePathName = vFilePathName;

		m_GetOnlyShaderArtType = vGetOnlyType;

		stringstream strStream;
		strStream << docFile.rdbuf();//read the file

		DocumentFragmentParseErrorEnum erno = parse(strStream.str().c_str());

		docFile.close(); // close file

		// on va afficher l'erreur si erreur il y a
		switch (erno)
		{
		case NOTHING:
			res = true;
			SetModified(false);
			break;
		case BAD_XML_FORMAT:
			// on tente l'import. c'est peut etre un fichier a 'lancienne
			// il faudra montrer une boite de dialogue qui demande si le suer veut tenter l'import
			res = this->ImportDocumentFile(vFilePathName);
			SetModified(true);
			break;
		case DUPLICATE_UNIFORM_NAME:
			break;
		case UNIFORM_NAME_MISSED:
			break;
		default:
			res = true;
		}
	}
	
	if (res == true)
	{
		if (m_VertexString.size() == 0)
			SetStringToFileMemory(ShaderTypeEnum::SHADER_TYPE_VERTEX, "attribute vec2 pos;\nvoid main()\n{\n\tgl_Position = vec4(pos.xy, 0.0, 1.0);\n}\n");

		this->SetFormat(vFileFormat);
	}
	
	return res;
}

DocumentFragmentParseErrorEnum ShaderArtDocument::parse(const char* datas)
{
	DocumentFragmentParseErrorEnum res;
	
	try
	{
		// first load the xml file
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLError err = doc.Parse(datas);
		if (err == tinyxml2::XMLError::XML_SUCCESS)
		{
			res = RecursParsing(doc.FirstChildElement("PROGRAM"));
		}
		else
		{
			res = DocumentFragmentParseErrorEnum::BAD_XML_FORMAT;
		}
	}
	catch (std::exception& ex)
	{
		string err = "ShaderArtDocument::parse(const char* datas) => " + toStr(ex.what());
		LogStr(err);
		res = DocumentFragmentParseErrorEnum::BAD_XML_FORMAT;
	}
	
	return res;
}

DocumentFragmentParseErrorEnum ShaderArtDocument::RecursParsing(tinyxml2::XMLElement* vElem)
{
	DocumentFragmentParseErrorEnum vErno = DocumentFragmentParseErrorEnum::NOTHING;

	// The value of this child identifies the name of this element
	UniformsWidgetTypeEnum uniformType = UniformsWidgetTypeEnum::UNONE;
	std::string strName = "";
	std::string strValue = "";
	std::string uniformName = "";
	std::map<string, string> uniformParams;

	strName = vElem->Value();

	if (m_ShaderArtType != SHADER_TYPE_UNKNOW && m_GetOnlyShaderArtType == true)
		return DocumentFragmentParseErrorEnum::NOTHING;

	if (strName == "BUILD")
	{
		string str = vElem->GetText();
		if (str.size() > 0)
		{
			cVariant buildNumber = str;
			m_BuildNumber = buildNumber.getI();
		}
	}
	if (strName == "TYPE")
	{
		string shaType = vElem->GetText();
		if (shaType == "QUAD") m_ShaderArtType = ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD;
		else if (shaType == "POINTS") m_ShaderArtType = ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS;
		else if (shaType == "CUSTOM") m_ShaderArtType = ShaderArtTypeEnum::SHADER_TYPE_FOR_CUSTOM_SHAPE;
	}
	if (strName == "RENDERMODE")
	{
		m_RenderMode = vElem->GetText();
	}
	if (m_UniformManager != 0)
	{
		if (strName == "UNIFORM")
		{
			uniformName = vElem->GetText();

			// now parse through all the attributes
			for (const tinyxml2::XMLAttribute* attr = vElem->FirstAttribute(); attr != 0; attr = attr->Next())
			{
				strName = attr->Name();
				strValue = attr->Value();

				if (strName == "TYPE")
				{
					uniformType = m_UniformManager->ConvertStringToUniformsWidgetTypeEnum(strValue);
				}
				else
				{
					uniformParams[strName] = strValue;
				}
			}

			if (uniformName != "")
			{
				// on ajoute le uniform au doc
				if (m_UniformManager->AddUniformFromFile(uniformName, uniformType, uniformParams) == true)
				{
					// fichier ok
				}
				else
				{
					// error fichier mal formé
					// uniform en doublon
					vErno = DocumentFragmentParseErrorEnum::DUPLICATE_UNIFORM_NAME;
				}
			}
			else // nom de uniform vide
			{
				// error fichier mal formé
				// nom de uniform manquant
				vErno = DocumentFragmentParseErrorEnum::UNIFORM_NAME_MISSED;
			}
		}
		if (strName == "GLSL") // docFile << "<GLSL VERSION=\"" + m_GlslVersion + "\" PRECISION=\"" + m_FloatPrecision + "\"/GLSL>\n";
		{
			// now parse through all the attributes
			for (const tinyxml2::XMLAttribute* attr = vElem->FirstAttribute(); attr != 0; attr = attr->Next())
			{
				strName = attr->Name();
				strValue = attr->Value();

				if (strName == "VERSION")
				{
					this->SetGlslVersion(strValue);
				}
				else if (strName == "PRECISION")
				{
					this->SetFloatPrecision(strValue);
				}
			}
		}
		else if (strName == "FRAGMENT")
		{
			wxString fragStr64 = vElem->GetText();

			wxMemoryBuffer buf = wxBase64Decode(fragStr64);

			m_FragmentString = string((char*)buf.GetData(), buf.GetDataLen());
		}
		else if (strName == "VERTEX")
		{
			wxString vtxStr64 = vElem->GetText();

			wxMemoryBuffer buf = wxBase64Decode(vtxStr64);

			m_VertexString = string((char*)buf.GetData(), buf.GetDataLen());
		}
		else if (strName == "MUSIC")
		{
			wxString musicStr64 = vElem->GetText();

			wxMemoryBuffer buf = wxBase64Decode(musicStr64);

			m_MusicShaderString = string((char*)buf.GetData(), buf.GetDataLen());
		}
	}
	
	if (vErno != DocumentFragmentParseErrorEnum::NOTHING ) 
		return vErno;
		
	// CHILDS 
	// parse through all childs elements
	for (tinyxml2::XMLElement* child = vElem->FirstChildElement(); child != 0; child = child->NextSiblingElement())
	{
		RecursParsing(child->ToElement());
	}

	return vErno;
}

bool ShaderArtDocument::ImportDocumentFile(string vFilePathName)
{
	bool res = false;

	ifstream docFile(vFilePathName, ios::in);
	if (docFile.is_open())
	{
		m_FilePathName = vFilePathName;

		stringstream strStream;
		strStream << docFile.rdbuf();//read the file

		m_FragmentString = strStream.str();

		if (m_FragmentString != "")
			res = true;
	}

	return res;
}

bool ShaderArtDocument::SaveDocumentFile(string vFilePathName, DocumentFileFormat vFileFormat)
{
	bool res = false;

	string path = vFilePathName;
	
	string outputFile;

	try
	{
		if (vFilePathName == "")
		{
			path = m_FilePathName;
		}
		else // changement de nom
		{
			m_FilePathName = vFilePathName;
			wxFileName wxfn(path);
			m_FileName = wxfn.GetName();
		}

		// formate la string
		outputFile += "<PROGRAM>\n";

		// Build Number
		outputFile += "<BUILD>" + toStr(m_BuildNumber) + "</BUILD>\n";

		// SHADER ART TYPE
		if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_QUAD)
		{
			outputFile += "<TYPE>QUAD</TYPE>\n";
		}
		else if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
		{
			outputFile += "<TYPE>POINTS</TYPE>\n";
		}
		else if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_CUSTOM_SHAPE)
		{
			outputFile += "<TYPE>CUSTOM</TYPE>\n";
		}

		// rendermode
		if (m_RenderMode.size() > 0)
			outputFile += "<RENDERMODE>" + m_RenderMode + "</RENDERMODE>\n";

		// glsl version && float precision (is Gl ES Used )
		outputFile += "<GLSL VERSION=\"" + m_GlslVersion + "\" PRECISION=\"" + m_FloatPrecision + "\"/>\n";

		// uniforms
		// <UNIFORM TYPE="USCREEN">uScreenSize</UNIFORM>
		map<string, UniformStruct> uniMap = m_UniformManager->GetMap();
		for (std::map<string, UniformStruct>::iterator it = uniMap.begin(); it != uniMap.end(); it++)
		{
			string key = it->first;
			UniformStruct uni = it->second;
			if (uni.CanBeSaved == true)
			{
				string xmlParams = m_UniformManager->GetXMLParamsByName(key, vFileFormat,  m_Format);
				if (xmlParams.size() > 0)
					xmlParams = " " + xmlParams;
				outputFile += "<UNIFORM TYPE=\"" + m_UniformManager->ConvertUniformsWidgetTypeEnumToString(uni.wType) + "\"" + xmlParams + ">" + key + "</UNIFORM>\n";
			}
		}

		// fragment
		wxString fragStr64 = wxBase64Encode(m_FragmentString.c_str(), m_FragmentString.length());
		outputFile += "<FRAGMENT>" + fragStr64.ToStdString() + "</FRAGMENT>\n";

		// vertex
		if (m_ShaderArtType == ShaderArtTypeEnum::SHADER_TYPE_FOR_POINTS)
		{
			wxString vtxStr64 = wxBase64Encode(m_VertexString.c_str(), m_VertexString.length());
			outputFile += "<VERTEX>" + vtxStr64.ToStdString() + "</VERTEX>\n";
		}
		
		if (m_MusicShaderString.size() > 0)
		{
			wxString musicStr64 = wxBase64Encode(m_MusicShaderString.c_str(), m_MusicShaderString.length());
			outputFile += "<MUSIC>" + musicStr64.ToStdString() + "</MUSIC>\n";
		}

		outputFile += "</PROGRAM>\n";
	}
	catch (std::exception e)
	{
		LogStr(e.what());
		if (g_pMainFrame != 0)
			g_pMainFrame->SetStatusBarText(e.what(), 0);
	}

	try
	{
		ofstream docFile(path, ios::out);
		if (docFile.bad() == false)
		{
			docFile << outputFile;

			docFile.close();

			m_NewDocument = false;

			SetModified(false);

			res = true;
		}
		else
		{
			// unable to write file with path
			// message box
		}
	}
	catch (std::exception e)
	{
		LogStr(e.what());
		g_pMainFrame->SetStatusBarText(e.what(), 0);
	}
	
	return res;
}

bool ShaderArtDocument::IsModified()
{
	if (m_NewDocument == true)
		return true;

	return m_Modified;
}

void ShaderArtDocument::SetModified(bool vIsModified)
{
	m_Modified = vIsModified;

	if (g_pMainFrame != 0)
		g_pMainFrame->SetSavedStatusOFCurrentPage(!IsModified());
}

/** Compress a STL string using zlib with given compression level and return
* the binary data. */
std::string ShaderArtDocument::compress_string(const std::string& str, int compressionlevel)
{
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (deflateInit(&zs, compressionlevel) != Z_OK)
		throw(std::runtime_error("deflateInit failed while compressing."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();           // set the z_stream's input

	int ret;
	char outbuffer[32768];
	std::string outstring;

	// l'analyse montre 33020 octet de pile alors qu'une statcksize ne doit pas exceder 16384.
	//  a ameliorer donc
	// retrieve the compressed bytes blockwise
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = deflate(&zs, Z_FINISH);

		if (outstring.size() < zs.total_out) {
			// append the block to the output string
			outstring.append(outbuffer,
				zs.total_out - outstring.size());
		}
	} while (ret == Z_OK);

	deflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
		throw(std::runtime_error(oss.str()));
	}

	return outstring;
}

/** Decompress an STL string using zlib and return the original data. */
std::string ShaderArtDocument::decompress_string(const std::string& str)
{
	z_stream zs;                        // z_stream is zlib's control structure
	memset(&zs, 0, sizeof(zs));

	if (inflateInit(&zs) != Z_OK)
		throw(std::runtime_error("inflateInit failed while decompressing."));

	zs.next_in = (Bytef*)str.data();
	zs.avail_in = str.size();

	int ret;
	char outbuffer[32768];
	std::string outstring;

	// l'analyse montre 33020 octet de pile alors qu'une statcksize ne doit pas exceder 16384.
	//  a ameliorer donc
	// get the decompressed bytes blockwise using repeated calls to inflate
	do {
		zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
		zs.avail_out = sizeof(outbuffer);

		ret = inflate(&zs, 0);

		if (outstring.size() < zs.total_out) {
			outstring.append(outbuffer,
				zs.total_out - outstring.size());
		}

	} while (ret == Z_OK);

	inflateEnd(&zs);

	if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
		std::ostringstream oss;
		oss << "Exception during zlib decompression: (" << ret << ") "
			<< zs.msg;
		throw(std::runtime_error(oss.str()));
	}

	return outstring;
}