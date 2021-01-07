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

#include "XShade.h"

DECLARE_APP(XShade)
IMPLEMENT_APP(XShade)

bool XShade::OnInit()
{
	//bizarre mais pas besoin d'init
	//if (!wxApp::OnInit())
	//	return false;
	
	// permet la sauvegarde des fichier image
	wxInitAllImageHandlers();

	wxArrayString arr;
	for (int i = 0; i < argc; i++)
	{
		string param = argv[i];
		if (param.find(".xsha") != string::npos) // un fichier a été trouvé
		{
			arr.Add(param);
		}
	}

	g_pMainFrame = new MainFrame();
	g_pMainFrame->Show();
	g_pMainFrame->Openfiles(arr, true);

	return true;
}

int XShade::OnExit()
{
	// clean up
	return 0;
}

int XShade::OnRun()
{

	int exitcode = wxApp::OnRun();
	//wxTheClipboard->Flush();
	return exitcode;
}
