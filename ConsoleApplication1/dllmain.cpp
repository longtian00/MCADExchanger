#include"dllmain.h"


static uiCmdAccessState TestAccessDefault(uiCmdAccessMode access_mode)
{
	return (ACCESS_AVAILABLE);
}

extern "C" int user_initialize(
	int argc,
	char *argv[],
	char *version,
	char *build,
	wchar_t errbuf[80])
{
	uiCmdCmdId cmd_id;

	ProError error;


	////////////////////// Button für Import in Creo


	// Festlegen,welche Funktion durch den Button aufgerufen wird
	error = ProCmdActionAdd(
		"Parameter_import",
		(uiCmdCmdActFn)Parameter_import,
		uiProe2ndImmediate,
		TestAccessDefault,
		PRO_B_TRUE,
		PRO_B_TRUE,
		&cmd_id
	);
	
	///// Beschriftung des Buttons
	error = ProCmdDesignate(
		cmd_id,
		"import1",
		"import2",
		"import3",
		L"menu.txt"
	);
	

	//////////////////////// Button für Export aus Creo
	error = ProCmdActionAdd(
		"Parameter_export",
		(uiCmdCmdActFn)Parameter_export,
		uiProe2ndImmediate,
		TestAccessDefault,
		PRO_B_TRUE,
		PRO_B_TRUE,
		&cmd_id
	);
	
	error = ProCmdDesignate(
		cmd_id,
		"export1",
		"export2",
		"export3",
		L"menu.txt"
	);
	
	return 0;
}

extern "C" void user_terminate()
{
}




string w2string(wchar_t* text)
{
	wstring wstr = text;
	string str;
	str.assign(wstr.begin(), wstr.end());
	return str;
}



/// Funktion zur Umwandlung von double zu wchar_t*
const wchar_t* d2wc(double value) {

	std::string s = std::to_string(value);
	const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
	return a;

}


/// Funktion zur Umwandlung eines strings zu wchar_t*
const wchar_t* s2cwc(string s) {

	const wchar_t* a = xercesc::XMLString::transcode(s.c_str());
	return a;

}



char* s2c(string str) {

	char*cstr = new char[str.length() + 1];

	strcpy(cstr, str.c_str());
	// do stuff

	return cstr;
}

string suc2n(string bib)
{

	{bib.erase(0, bib.find('/') + 1);


	}
	return bib;
}


void creatport(DOMDocument *pDOMDocument, ofstream &log, ProMdl handle,
	DOMElement * pChild, DOMElement *pmInterfaces)
{
	ProError error;
	ProName portname;
	ProFeature feature;
	DOMElement * pport;
	int*	    p_feat_id_array;
	ProFeatStatus*  p_status_array;
	int		    n_features;
	int a = 0;
	ProFeattype p_type;

	error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
	error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

	/* Get a list of features of the specified solid and their statuses */
	error = ProSolidFeatstatusGet(ProMdlToSolid(handle), &p_feat_id_array, &p_status_array,
		&n_features);
	log << n_features << "--------portcreat-----------------------------" << endl;

	for (a; a<n_features; a++)
	{
		/* Get a handle to the feature */
		error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feature);
		error = ProFeatureTypeGet(&feature, &p_type);

		if (p_type == 1118)
		{
			ProModelitemNameGet(&feature, portname);
			pport = pDOMDocument->createElement(L"ExternalInterface");
			log << w2string(portname) << endl;
			pport->setAttribute(L"Name", s2cwc(w2string(portname)));


			if ((strstr(w2string(portname).c_str(), "IFINSTALL") != 0) || (strstr(w2string(portname).c_str(), "IFFINSTALL") != 0))
			{
				Interface[0] = 1;
				pport->setAttribute(L"RefBaseClassPath", L"mInterfaces/ifScrewConnection");
				pChild->appendChild(pport);

			}

			else if ((strstr(w2string(portname).c_str(), "INSTALL") != 0) && strstr(w2string(portname).c_str(), "IFINSTALL") == 0)
			{
				pport->setAttribute(L"RefBaseClassPath", L"mInterfaces/ScrewConnection");
				Interface[1] = 1;
				pChild->appendChild(pport);


			}

			else if ((strstr(w2string(portname).c_str(), "WELD") != 0) && strstr(w2string(portname).c_str(), "IFWELD") == 0)
			{
				pport->setAttribute(L"RefBaseClassPath", L"mInterfaces/WeldConnection");
				Interface[2] = 1;
				pChild->appendChild(pport);


			}

			else if ((strstr(w2string(portname).c_str(), "IFWELD") != 0) || strstr(w2string(portname).c_str(), "IFFWELD") == 0)
			{
				pport->setAttribute(L"RefBaseClassPath", L"mInterfaces/ifWeldConnection");
				Interface[3] = 1;
				pChild->appendChild(pport);



			}




			log << "--------portcreatsuccess-----------------------------" << endl;
		}

	}

}



void bibsyncro(TreeNode* filetree)    // Systemunitclass im Creo als Parameter speichern. Eingang: Parametername/Value, Besitzer
{
	ProError error;
	ProMdl pmodel;
	ProModelitem pmodelitem;
	ProParamvalue proval;
	std::ofstream log("bibsyn.log", std::ofstream::app);
	error = ProMdlCurrentGet(&pmodel);
	if (error != 0)
	{
		log << "ProMdlCurrentGet returned" << error << std::endl;
	}

	error = ProMdlToModelitem(pmodel, &pmodelitem);
	for (int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
		string child_name;
		child_name = child->getTextContent();
		ProParameter param;
		ProName ppname;

		ProStringToWstring(ppname, s2c(child_name));
		delete[] s2c(child_name);
		error = ProParameterInit(&pmodelitem,
			ppname,
			&param);

		error = ProParameterValueGet(&param, &proval);



		if (error == 0)
		{
			// Systemunitclasslibrary erzeugen

			wchar_t *value;
			value = proval.value.s_val;

			//string Str = string(cStr);
			string Str = w2string(value);
			log << "ProParameterValue:" << Str << endl;
			ProName name;

			ProStringToWstring(name, s2c(child_name));


			string opath;
			if (strstr(Str.c_str(), "eComponent") != 0)
			{
				opath = bib + "\\" + "eComponents\\" + suc2n(Str);
			}
			else if (strstr(Str.c_str(), "mComponent") != 0)
			{
				opath = bib + "\\" + "mComponents\\" + suc2n(Str);
			}
			else
			{
				opath = bib + "\\" + "Modules\\" + suc2n(Str);
			}


			string fpath;
			fpath = opath + "\\" + suc2n(Str);
			ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
			ProMdlfileType type2 = PRO_MDLFILE_PART;
			ProPath cpath;
			ProError status;
			ProError status1;
			ProMdl chandle;
			ProMdl p_handle;
			ProStringToWstring(cpath, s2c(fpath));
			delete[] s2c(fpath);

			status = ProMdlFiletypeLoad(cpath,
				type1,
				PRO_B_FALSE,
				&chandle);

			status1 = ProMdlFiletypeLoad(cpath,
				type2,
				PRO_B_FALSE, &chandle);


			if (status != 0 && status1 != 0)// keine Datei in die Bib
			{
				log << "no file in bib:" << suc2n(Str) << endl;
				ProPath   backup_dir;
				string  ppath;
				ppath = ns + "\\" + child_name;
				ProStringToWstring(backup_dir, s2c(ppath));
				status = ProMdlFiletypeLoad(backup_dir,    // die Datei im neue load.
					type1,
					PRO_B_FALSE,
					&p_handle);
				if (status != 0)
				{
					status1 = ProMdlFiletypeLoad(backup_dir,
						type2,
						PRO_B_FALSE,
						&p_handle);
				}

				int*	    p_feat_id_array;
				ProFeatStatus*  p_status_array;
				int		    n_features;
				ProError error;
				error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
				error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);
				error = ProSolidFeatstatusGet(ProMdlToSolid(p_handle), &p_feat_id_array, &p_status_array,
					&n_features);
				if (n_features != 0)   // nicht leer, gleich null falls leer
				{

					_mkdir(opath.c_str());
					ProMdlName new_name;
					char* asmname = s2c(suc2n(Str));
					delete[] s2c(suc2n(Str));
					ProStringToWstring(new_name, asmname);
					status = ProMdlnameRename(p_handle,
						new_name);
					error = ProMdlnameBackup(p_handle, cpath);                     // speichern und rename
					log << "savesuccess:" << asmname << status << endl;
				}


			}


		}
		if (child->hasChildren())
			bibsyncro(child);
		else
			continue;

	}
}


void creatinterfacelib(DOMDocument *pDOMDocument, ofstream &log, DOMElement *pmInterfaces)
{


	if (Interface[0] == 1)
	{
		DOMElement * pmInterface = NULL;
		pmInterface = pDOMDocument->createElement(L"InterfaceClass");
		pmInterface->setAttribute(L"Name", L"ifScrewConnection");
		pmInterfaces->appendChild(pmInterface);
	}
	if (Interface[1] == 1)
	{
		DOMElement * pmInterface = NULL;
		pmInterface = pDOMDocument->createElement(L"InterfaceClass");
		pmInterface->setAttribute(L"Name", L"ScrewConnection");
		pmInterfaces->appendChild(pmInterface);
	}
	if (Interface[2] == 1)
	{
		DOMElement * pmInterface = NULL;
		pmInterface = pDOMDocument->createElement(L"InterfaceClass");
		pmInterface->setAttribute(L"Name", L"WeldConnection");
		pmInterfaces->appendChild(pmInterface);
	}
	if (Interface[3] == 1)
	{
		DOMElement * pmInterface = NULL;
		pmInterface = pDOMDocument->createElement(L"InterfaceClass");
		pmInterface->setAttribute(L"Name", L"ifWeldConnection");
		pmInterfaces->appendChild(pmInterface);
	}



}

void creatconnector(DOMDocument *pDOMDocument, ofstream &log, TreeNode* child,
	DOMElement * pChild)     // change
{
	DOMElement *connector;
	string ifconector;
	ifconector = child->getTextContent();
	log << "treenode:" << ifconector << endl;
	ProError error;
	ProName name;
	ProStringToWstring(name, s2c(ifconector));
	delete[] s2c(ifconector);
	ProMdl handle;
	ProName portname;
	ProFeature feat;
	int*	    p_feat_id_array;
	int		    n_features;
	int a = 0;
	ProFeattype p_type;
	ProFeatStatus *p_status_array = NULL;

	error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &handle);
	if (error != 0)
	{

		error = ProMdlnameInit(name, PRO_MDLFILE_PART, &handle);
	}


	error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
	error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

	/* Get a list of features of the specified solid and their statuses */
	error = ProSolidFeatstatusGet(ProMdlToSolid(handle), &p_feat_id_array, &p_status_array,
		&n_features);


	for (a; a<n_features; a++)
	{
		/* Get a handle to the feature */
		error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
		error = ProFeatureTypeGet(&feat, &p_type);

		if (p_type == 1118)
		{
			error = ProModelitemNameGet(&feat, portname);

			log << "cooportname:" << w2string(portname) << error << endl;

			int f = 0;
			if (strstr(s2c(w2string(portname)), "IFINSTALL") || strstr(s2c(w2string(portname)), "IFWELD"))
			{
				string s1;
				s1 = (w2string(portname)).erase(0, 2);

				string connect;
				connect = "Connector [" + w2string(portname) + "-" + s1 + "]";

				s1 = (w2string(portname)).erase(0, 2);

				connector = pDOMDocument->createElement(L"InternalLink");
				connector->setAttribute(L"Name", s2cwc(connect));
				connector->setAttribute(L"RefPartnerSideA", s2cwc(w2string(portname)));
				connector->setAttribute(L"RefPartnerSideB", s2cwc(s1));
				pChild->appendChild(connector);
			}

			else if (strstr(s2c(w2string(portname)), "IFFINSTALL") || strstr(s2c(w2string(portname)), "IFFWELD"))
			{
				string s1;
				s1 = (w2string(portname)).erase(1, 1);

				string partA;
				partA = w2string(portname);
				string connect;
				connect = "Connector [" + partA + "-" + s1 + "]";

				connector = pDOMDocument->createElement(L"InternalLink");
				connector->setAttribute(L"Name", s2cwc(connect));
				connector->setAttribute(L"RefPartnerSideA", s2cwc(partA));
				connector->setAttribute(L"RefPartnerSideB", s2cwc(s1));

				pChild->appendChild(connector);

			}


			delete[] s2c(w2string(portname));
		}


	}




	// need a new boss of the gym

	for (int m = 0; m < child->childrenNumber(); m++)

	{
		string cchild_name;
		TreeNode *cchild = child->getChild(m);

		cchild_name = cchild->getTextContent();
		ProName cppname;
		ProStringToWstring(cppname, s2c(cchild_name));
		delete[] s2c(cchild_name);


		ProName name;
		ProMdl handle;
		ProName portname;
		ProFeature feat;
		int*	    p_feat_id_array;
		ProFeatStatus*  p_status_array;
		int		    n_features;
		int a = 0;
		ProFeattype p_type;
		ProStringToWstring(name, s2c(cchild_name));

		error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &handle);
		if (error != 0)
		{

			error = ProMdlnameInit(name, PRO_MDLFILE_PART, &handle);
		}


		error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
		error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

		/* Get a list of features of the specified solid and their statuses */
		error = ProSolidFeatstatusGet(ProMdlToSolid(handle), &p_feat_id_array, &p_status_array,
			&n_features);


		for (a; a<n_features; a++)
		{
			/* Get a handle to the feature */
			error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
			error = ProFeatureTypeGet(&feat, &p_type);

			if (p_type == 1118)
			{
				error = ProModelitemNameGet(&feat, portname);

				log << "portname:" << w2string(portname) << error << endl;

				int f = 0;
				f = m + 1;

				for (f; f < child->childrenNumber(); f++)

				{

					string ptchild_name;
					TreeNode *cchild = child->getChild(f);
					ptchild_name = cchild->getTextContent();

					string connect;
					connect = "Connector [" + w2string(portname) + "-INSTALL" + cchild_name + "]";

					log << "brothername:" << ptchild_name << endl;

					if (strstr(s2c(w2string(portname)), ptchild_name.c_str()))
					{
						connector = pDOMDocument->createElement(L"InternalLink");
						connector->setAttribute(L"Name", s2cwc(connect));
						connector->setAttribute(L"RefPartnerSideA", s2cwc(w2string(portname)));
						connector->setAttribute(L"RefPartnerSideB", s2cwc(" INSTALL" + cchild_name));
						pChild->appendChild(connector);
					}

					delete[] s2c(w2string(portname));
				}
			}

		}


	}

}
void creatinternallink(DOMDocument *pDOMDocument, ofstream &log,
	DOMElement * pChild)     // change
{
	DOMElement *connector;
	string ifconector;
	ifconector = XMLString::transcode(pChild->getAttribute(L"Name"));
	ProError error;
	ProName name;
	ProStringToWstring(name, s2c(ifconector));
	delete[] s2c(ifconector);
	ProMdl handle;
	ProName portname;
	ProFeature feat;
	int*	    p_feat_id_array;
	int		    n_features;
	int a = 0;
	ProFeattype p_type;
	ProFeatStatus *p_status_array = NULL;

	error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &handle);
	if (error != 0)
	{

		error = ProMdlnameInit(name, PRO_MDLFILE_PART, &handle);
	}
	if (error == 0)

	{
		error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
		error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

		/* Get a list of features of the specified solid and their statuses */
		error = ProSolidFeatstatusGet(ProMdlToSolid(handle), &p_feat_id_array, &p_status_array,
			&n_features);


		for (a; a<n_features; a++)
		{
			/* Get a handle to the feature */
			error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
			error = ProFeatureTypeGet(&feat, &p_type);

			if (p_type == 1118)                  //1118 bedeutet interface
			{
				error = ProModelitemNameGet(&feat, portname);

				log << "cooportname:" << w2string(portname) << error << endl;

				int f = 0;
				if (strstr(s2c(w2string(portname)), "IFINSTALL") || strstr(s2c(w2string(portname)), "IFWELD"))
				{
					string s1;
					s1 = (w2string(portname)).erase(0, 2);

					string connect;
					connect = "Connector [" + w2string(portname) + "-" + s1 + "]";
					s1 = (w2string(portname)).erase(0, 2);
					connector = pDOMDocument->createElement(L"InternalLink");
					connector->setAttribute(L"Name", s2cwc(connect));
					connector->setAttribute(L"RefPartnerSideA", s2cwc(w2string(portname)));
					connector->setAttribute(L"RefPartnerSideB", s2cwc(s1));

					pChild->appendChild(connector);
				}

				else if (strstr(s2c(w2string(portname)), "IFFINSTALL") || strstr(s2c(w2string(portname)), "IFFWELD"))
				{
					string s1;
					s1 = (w2string(portname)).erase(1, 1);
					string partA;
					partA = w2string(portname);
					string connect;
					connect = "Connector [" + partA + "-" + s1 + "]";
					connector = pDOMDocument->createElement(L"InternalLink");
					connector->setAttribute(L"Name", s2cwc(connect));
					connector->setAttribute(L"RefPartnerSideA", s2cwc(partA));
					connector->setAttribute(L"RefPartnerSideB", s2cwc(s1));



					pChild->appendChild(connector);

				}

				delete[] s2c(w2string(portname));
			}

			else if (p_type == 1000)                                     // 1000 bedeutet ist ein koeper,

			{
				ProError err;
				ProElement elemtree;
				err = ProFeatureElemtreeExtract(&feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS,
					&elemtree);
				int exist = 0;
				int          p_array_count;
				ProElementArrayCount(elemtree, NULL, &p_array_count);
				ProElement *chai;
				err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai);
				err = ProElementArrayGet(elemtree, NULL, &chai);
				ProValue asmvalue;
				ProValueData asmvalue_data;
				ProValue comvalue;
				ProValueData comvalue_data;
				int          p_array_count1;
				ProElementArrayCount(*(chai + 4), NULL, &p_array_count1);       // *(chai+4) POR E COMP _PLACE_INTERFACE
				ProElement *chai1;
				err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai1);


				err = ProElementArrayGet(*(chai + 4), NULL, &chai1);

				ProElement *chai2;
				err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai2);        //*(chai1+2) PROE COMP PLACE INTERFACE ASSEMS
				err = ProElementArrayGet(*(chai1 + 2), NULL, &chai2);
				log << err << endl;

				ProElement *chai3;
				err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai3);       //*(chai2) PROE COMP PLACE INTERFACE ASSEM
				err = ProElementArrayGet(*(chai2), NULL, &chai3);
				if (err == 0)                                                       //   bedeutet interface als Platzierungsweise
				{
					exist = 1;
					err = ProElementValueGet(*(chai3), &asmvalue);                 //*(chai3) PROE COMP PLACE INTERFACE ASSEM REF

					err = ProValueDataGet(asmvalue, &asmvalue_data);


					err = ProElementValueGet(*(chai1 + 1), &comvalue);                 //*(chai3) PROE COMP PLACE INTERFACE ASSEM REF
					err = ProValueDataGet(comvalue, &comvalue_data);

					ProSelection asmsele;
					asmsele = asmvalue_data.v.r;
					ProModelitem asmitem;
					ProSelectionModelitemGet(asmsele, &asmitem);

					ProName asmname;
					ProModelitemNameGet(&asmitem, asmname);
					string refA;
					refA = w2string(asmname);

					ProSelection comsele;
					comsele = comvalue_data.v.r;
					ProModelitem comitem;
					ProSelectionModelitemGet(comsele, &comitem);
					ProName compname;
					ProModelitemNameGet(&comitem, compname);
					string refB;
					refB = w2string(compname);
					log << refB << refA << endl;
					string connect;
					connect = "Connector [" + refA + "-" + refB + "]";
					connector = pDOMDocument->createElement(L"InternalLink");
					connector->setAttribute(L"Name", s2cwc(connect));
					connector->setAttribute(L"RefPartnerSideA", s2cwc(refA));
					connector->setAttribute(L"RefPartnerSideB", s2cwc(refB));
					pChild->appendChild(connector);

				}

			}

		}

	}
}

void creatsuc(ProName pname, string value, ProMdl p_model)    // Systemunitclass im Creo als Parameter speichern. Eingang: Parametername/Value, Besitzer
{
	ProError error;
	std::ofstream log("systemunitclass.log", std::ofstream::app);

	ProModelitem p_model_item;
	ProParameter param;
	ProParamvalue provalue;
	ProName suc;

	error = ProMdlToModelitem(p_model, &p_model_item);
	if (error != 0)
	{
		log << "ProMdlToModelitem returned" << error << std::endl;
	}

	/// ProParameter initiieren
	error = ProParameterInit(&p_model_item, pname, &param);
	//error=ProParameterValueGet	(&param, &provalue);

	ProStringToWstring(suc, s2c(value));

	delete[] s2c(value);
	error = ProParamvalueSet(&provalue, &suc, PRO_PARAM_STRING); // Definition eine Value(Typ und Wert)
	error = ProParameterWithUnitsCreate(&p_model_item,
		pname,
		&provalue,
		NULL,
		&param);

	log << "Erstellen von Parameter klappt" << value << error << endl;

}




void creatmodulelib(DOMDocument *pDOMDocument, DOMElement * pModules, DOMElement *pmInterfaces)
{
	string path = bib + "\\" + "Modules";
	DOMElement * pAttr1 = NULL;
	DOMElement * pValue1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue2 = NULL;
	DOMElement * pport = NULL;
	DOMElement * libport = NULL;
	DOMElement * connector = NULL;
	DOMText* pTextNode = NULL;
	DOMText* pTextNode2 = NULL;
	ProError error;
	DOMElement * libComponent = NULL;
	string child_name;
	std::ofstream log("export.log", std::ofstream::app);
	int a = 0;
	long  hFile = 0;
	struct _finddata_t fileinfo;
	string p = path + "\\*.*";
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if ((strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0))
				{
					libComponent = pDOMDocument->createElement(L"SystemUnitClass");
					libComponent->setAttribute(L"Name", s2cwc(fileinfo.name));


					ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
					ProMdlfileType type2 = PRO_MDLFILE_PART;
					ProPath cpath;
					ProError status;
					ProError status1;
					ProMdl chandle;
					string fpath;
					fpath = path + "\\" + fileinfo.name + "\\" + fileinfo.name;
					log << fpath << endl;
					ProStringToWstring(cpath, s2c(fpath));
					delete[] s2c(fpath);

					status = ProMdlFiletypeLoad(cpath,
						type1,
						PRO_B_FALSE,
						&chandle);
					if (status != 0)
					{
						status1 = ProMdlFiletypeLoad(cpath,
							type2,
							PRO_B_FALSE,
							&chandle);
					}
					ProMdlnameShortdata *depd;
					ProMdlfileType * files;
					int num_pdep;
					error = ProMdlDependenciesCleanup(chandle);
					error = ProMdlDependenciesDataList(chandle, &depd, &files, &num_pdep);
					log << error << "Promdlget" << endl;


					pAttr1 = pDOMDocument->createElement(L"Attribute");
					pAttr1->setAttribute(L"AttributeDataType", L"xs:string");
					pAttr1->setAttribute(L"Name", L"Filepath");
					pValue1 = pDOMDocument->createElement(L"Value");

					pTextNode = pDOMDocument->createTextNode(s2cwc(fpath)); //Text Node erstellen
					pValue1->appendChild(pTextNode); //Node verbinden
					pAttr1->appendChild(pValue1);
					libComponent->appendChild(pAttr1);

					pAttr2 = pDOMDocument->createElement(L"Attribute");
					pAttr2->setAttribute(L"AttributeDataType", L"xs:string");
					pAttr2->setAttribute(L"Name", L"Typ");
					pValue2 = pDOMDocument->createElement(L"Value");
					if (status == 0)
					{
						pTextNode2 = pDOMDocument->createTextNode(L"asm");
					} //Text Node erstellen
					else
					{
						pTextNode2 = pDOMDocument->createTextNode(L"prt");
					}
					pValue2->appendChild(pTextNode2); //Node verbinden
					pAttr2->appendChild(pValue2);
					libComponent->appendChild(pAttr2);


					creatport(pDOMDocument, log, chandle, libComponent, pmInterfaces);

					for (int i = 0; i < num_pdep; i++)
					{
						DOMElement * ppChild = NULL;
						string cchild_name;
						ppChild = pDOMDocument->createElement(L"InternalElement");
						ppChild->setAttribute(L"Name", (depd + i)->name);
						libComponent->appendChild(ppChild);

						log << error << w2string((depd + i)->name) << i << endl;
					}

					pModules->appendChild(libComponent);

				}
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}


}









void creatcomplib(DOMDocument *pDOMDocument, DOMElement * pModules, DOMElement *pmInterfaces, string complibname)
{

	string path = bib + "\\" + complibname;
	DOMElement * pAttr1 = NULL;
	DOMElement * pValue1 = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue2 = NULL;
	DOMElement * pport = NULL;
	DOMElement * libport = NULL;
	DOMElement * connector = NULL;
	DOMText* pTextNode = NULL;
	DOMText* pTextNode2 = NULL;
	DOMElement * libComponent = NULL;
	string child_name;
	std::ofstream log("export.log", std::ofstream::app);
	int a = 0;
	long  hFile = 0;
	struct _finddata_t fileinfo;
	string p = path + "\\*.*";
	if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if ((strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0))
				{
					libComponent = pDOMDocument->createElement(L"SystemUnitClass");
					libComponent->setAttribute(L"Name", s2cwc(fileinfo.name));


					ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
					ProMdlfileType type2 = PRO_MDLFILE_PART;
					ProPath cpath;
					ProError status;
					ProError status1;
					ProMdl chandle;
					string fpath;
					fpath = path + "\\" + fileinfo.name + "\\" + fileinfo.name;
					log << fpath << endl;
					ProStringToWstring(cpath, s2c(fpath));
					delete[] s2c(fpath);

					status = ProMdlFiletypeLoad(cpath,
						type1,
						PRO_B_FALSE,
						&chandle);
					if (status != 0)
					{
						status1 = ProMdlFiletypeLoad(cpath,
							type2,
							PRO_B_FALSE,
							&chandle);
					}


					pAttr1 = pDOMDocument->createElement(L"Attribute");
					pAttr1->setAttribute(L"AttributeDataType", L"xs:string");
					pAttr1->setAttribute(L"Name", L"Filepath");
					pValue1 = pDOMDocument->createElement(L"Value");

					pTextNode = pDOMDocument->createTextNode(s2cwc(fpath)); //Text Node erstellen
					pValue1->appendChild(pTextNode); //Node verbinden
					pAttr1->appendChild(pValue1);
					libComponent->appendChild(pAttr1);

					pAttr2 = pDOMDocument->createElement(L"Attribute");
					pAttr2->setAttribute(L"AttributeDataType", L"xs:string");
					pAttr2->setAttribute(L"Name", L"Typ");
					pValue2 = pDOMDocument->createElement(L"Value");
					if (status == 0)
					{
						pTextNode2 = pDOMDocument->createTextNode(L"asm");
					} //Text Node erstellen
					else
					{
						pTextNode2 = pDOMDocument->createTextNode(L"prt");
					}
					pValue2->appendChild(pTextNode2); //Node verbinden
					pAttr2->appendChild(pValue2);
					libComponent->appendChild(pAttr2);

					creatport(pDOMDocument, log, chandle, libComponent, pmInterfaces);

					pModules->appendChild(libComponent);


				}
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}


}


// Creat Internalelement und die Bibliothek 
void creatIH(TreeNode* filetree, DOMDocument *pDOMDocument, DOMElement **pInstanceElement, ofstream &log,
	DOMElement * pModules, DOMElement * pmComponents, DOMElement * peComponents, ProModelitem p_model_item, DOMElement *pmInterfaces)
{
	DOMElement * pChild = NULL;
	DOMElement * pAttr2 = NULL;
	DOMElement * pValue2 = NULL;
	DOMElement * pport = NULL;
	DOMElement * libport = NULL;
	DOMElement * connector = NULL;
	DOMText* pTextNode = NULL;
	ProError error;
	DOMElement * libComponent = NULL;
	ProParamvalue  proval;
	string child_name;
	ProMdlName  name;
	int a = 0;


	//vector<TreeNode *> children = filetree->getChild;
	for (int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
		child_name = child->getTextContent();
		pChild = pDOMDocument->createElement(L"InternalElement");
		log << "creating name attribute: " << child_name << endl;
		pChild->setAttribute(L"Name", s2cwc(child_name));
		pAttr2 = pDOMDocument->createElement(L"Attribute");
		pAttr2->setAttribute(L"AttributeDataType", L"xs:string");
		pAttr2->setAttribute(L"Name", L"Typ");
		pValue2 = pDOMDocument->createElement(L"Value");
		pTextNode = pDOMDocument->createTextNode(s2cwc(child->getType())); //Text Node erstellen
		pValue2->appendChild(pTextNode); //Node verbinden
		pAttr2->appendChild(pValue2);
		pChild->appendChild(pAttr2);
		ProParameter param;
		ProName ppname;
		ProStringToWstring(ppname, s2c(child_name));
		delete[] s2c(child_name);

		error = ProParameterInit(&p_model_item,
			ppname,
			&param);
		log << "ProParameterInit" << error << endl;
		error = ProParameterValueGet(&param, &proval);
		log << "ProParameterValueGet" << error << endl;

		if (error == 0)
		{
			// Systemunitclasslibrary erzeugen

			wchar_t *value;
			value = proval.value.s_val;

			//string Str = string(cStr);
			string Str = w2string(value);
			log << "ProParameterValue:" << Str << endl;

			ProStringToWstring(name, s2c(child_name));


			//addfilelink(pDOMDocument,pChild);
			(*pInstanceElement)->appendChild(pChild);
			ProMdl chandle;
			error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &chandle);
			if (error != 0)
			{
				error = ProMdlnameInit(name, PRO_MDLFILE_PART, &chandle);
			}


			/*			if (error!=0)     // Im Prinzip die Ausfurhung der Synchronisierung hier ist schneller.
			{
			bibsyncro(child_name,Str);

			}
			*/

			pChild->setAttribute(L"RefBaseSystemUnitPath", s2cwc(Str));
			creatport(pDOMDocument, log, chandle, pChild, pmInterfaces);
			log << "supplement suc" << endl;

			if (strstr(Str.c_str(), "eComponent") != 0)
			{
				int exist = 0;
				for (DOMElement* pChild1 = peComponents->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
				{
					string attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));
					if (suc2n(Str) == attr_pChild1)
					{
						exist = 1;
						break;
					}
				}
				if (exist == 0)
				{
					libComponent = pDOMDocument->createElement(L"SystemUnitClass");
					libComponent->setAttribute(L"Name", s2cwc(suc2n(Str)));
					peComponents->appendChild(libComponent);
					creatport(pDOMDocument, log, chandle, libComponent, pmInterfaces);
				}

			}
			else if (strstr(Str.c_str(), "mComponent") != 0)
			{
				int exist = 0;
				for (DOMElement* pChild1 = pmComponents->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
				{
					string attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));
					if (suc2n(Str) == attr_pChild1)
					{
						exist = 1;
						break;
					}
				}
				if (exist == 0)
				{
					libComponent = pDOMDocument->createElement(L"SystemUnitClass");
					libComponent->setAttribute(L"Name", s2cwc(suc2n(Str)));
					pmComponents->appendChild(libComponent);
					creatport(pDOMDocument, log, chandle, libComponent, pmInterfaces);
				}
			}


			else if (strstr(Str.c_str(), "Module"))
			{
				DOMElement *pModule = NULL;
				int exist = 0;
				for (DOMElement* pChild1 = pModules->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
				{
					string attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));
					log << attr_pChild1 << "modulecompname" << endl;
					if (suc2n(Str) == attr_pChild1)
					{
						exist = 1;
						break;
					}
				}
				if (exist == 0)
				{
					pModule = pDOMDocument->createElement(L"SystemUnitClass");
					pModule->setAttribute(L"Name", s2cwc(suc2n(Str)));
					pModules->appendChild(pModule);
					creatport(pDOMDocument, log, chandle, pModule, pmInterfaces);

					for (int m = 0; m < child->childrenNumber(); m++)

					{
						DOMElement * ppChild = NULL;
						string cchild_name;
						TreeNode *cchild = child->getChild(m);
						ppChild = pDOMDocument->createElement(L"InternalElement");
						cchild_name = cchild->getTextContent();
						ppChild->setAttribute(L"Name", s2cwc(cchild_name));
						ProParameter cparam;
						ProName cppname;
						ProParamvalue  cproval;

						ProStringToWstring(cppname, s2c(cchild_name));
						delete[] s2c(child_name);
						error = ProParameterInit(&p_model_item,
							cppname,
							&cparam);

						if (error == 0)
						{
							error = ProParameterValueGet(&cparam, &cproval);
							wchar_t *cvalue;
							cvalue = cproval.value.s_val;
							string cStr = w2string(cvalue);

							ppChild->setAttribute(L"RefBaseSystemUnitPath", s2cwc(cStr));
							pModule->appendChild(ppChild);


						}

					}
				}

			}
			map<ProMdlfileType, string> typeHash;
			typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY] = asm_name;
			typeHash[ProMdlfileType::PRO_MDLFILE_PART] = prt_name;

			if (i == filetree->childrenNumber() - 1)
			{
				std::ofstream log1("connector.log", std::ofstream::app);
				//creatconnector(pDOMDocument, log1, filetree,	*pInstanceElement);
				creatinternallink(pDOMDocument, log1, *pInstanceElement);

			}
		}



		if (child->hasChildren())
			creatIH(child, pDOMDocument, &pChild, log, pModules, pmComponents, peComponents, p_model_item, pmInterfaces);
		else
			continue;






	}
}


void addTrees(TreeNode **Node, ProMdl p_model, map<ProMdlfileType, string> typeHash)
{
	std::ofstream log("addtrees.log", std::ofstream::app);
	ProError error;
	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	error = ProMdlDependenciesCleanup(p_model);

	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);

	ProMdl pmodel;

	for (int i = 0; i < num_pdep; i++)
	{
		(*Node)->appendChild(new TreeNode(w2string((depd + i)->name), typeHash[*(files + i)]));
		TreeNode *child = (*Node)->getChild(i);

		log << error << w2string((depd + i)->name) << i << endl;
		ProMdlCurrentGet(&pmodel);
		ProModelitem p_model_item;
		ProParameter param;
		ProParamvalue proval;
		ProMdlToModelitem(pmodel, &p_model_item);
		error = ProParameterInit(&p_model_item,
			(depd + i)->name,
			&param);
		if (error != 0)
		{
			log << error << endl;
		}

		error = ProParameterValueGet(&param, &proval);
		//retrieve current modle from name and type	  	  
		// Systemunitclasslibrary erzeugen

		wchar_t *value;
		value = proval.value.s_val;

		//string Str = string(cStr);
		string Str = w2string(value);
		log << Str << "lib" << endl;
		error = ProMdlnameRetrieve((depd + i)->name, *(files + i), &p_model);
		if (!strstr(Str.c_str(), "Module"))
			// Falls du die ganz Model im Tree addieren möchte		, diese if löschen und die folgende Kommentar benutzen
			/*	int*	    p_feat_id_array;
			ProFeatStatus*  p_status_array;
			int		    n_features;

			if (error!=0)
			log<<error<<"ProMdlnameRetrieve"<<endl;

			error = ProArrayAlloc( 0, sizeof(int), 1, (ProArray *)&p_feat_id_array );
			error = ProArrayAlloc( 0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array );


			error = ProSolidFeatstatusGet( ProMdlToSolid(p_model), &p_feat_id_array, &p_status_array,
			&n_features );

			ProArrayFree( (ProArray *)&p_feat_id_array);
			ProArrayFree( (ProArray *)&p_status_array);

			if(	n_features==0 ||  typeHash[*(files+i)] == prt_name )
			*/

			continue;
		else
		{
			addTrees(&child, p_model, typeHash);
		}
	}
}

ProSolid creatrootelement(string mode)
{
	ProSolid phandle;
	ProError erstell;
	ProMdlName  nname;
	string ocurr;
	if (strstr(mode.c_str(), "eComponent") != 0)
	{
		ocurr = bib + "\\" + "eComponents\\" + suc2n(mode) + "\\" + suc2n(mode);
	}
	else if (strstr(mode.c_str(), "mComponent") != 0)
	{
		ocurr = bib + "\\" + "mComponents\\" + suc2n(mode) + "\\" + suc2n(mode);
	}
	else
	{
		ocurr = bib + "\\" + "Modules\\" + suc2n(mode) + "\\" + suc2n(mode);
	}
	ProPath       cpath;
	ProStringToWstring(cpath, s2c(ocurr));
	delete[] s2c(ocurr);
	ProMdl handle;
	erstell = ProMdlFiletypeLoad(cpath,
		PRO_MDLFILE_ASSEMBLY,
		PRO_B_FALSE,
		&handle);
	phandle = ProMdlToSolid(handle);

	if (erstell != 0)
	{
		erstell = ProSolidCreate(nname, PRO_ASSEMBLY, &phandle);

	}
	return phandle;

}






int mdl2feat(ProSolid model, int h)           // nache der Modelnummer die entsprechende Feature ID finden.

{
	std::ofstream log("consistent.log", std::ofstream::app);
	ProError	    status;
	int*	    p_feat_id_array;
	ProFeatStatus*  p_status_array;
	int		    n_features = 0;
	int id = 0;
	int	a = 0;
	int k = 0;
	ProFeature	    feature;
	status = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
	status = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

	/* Get a list of features of the specified solid and their statuses */
	status = ProSolidFeatstatusGet(model, &p_feat_id_array, &p_status_array,
		&n_features);

	for (a; a<n_features; a++)
	{
		/* Get a handle to the feature */
		status = ProFeatureInit(model, p_feat_id_array[a], &feature);
		ProAsmcompconstraint *pp_constraints = NULL;

		status = ProAsmcompConstraintsGet(&feature, &pp_constraints);

		if (status != -18)
		{
			k = k + 1;
			log << k << endl;
		}

		if (k == h + 1)
		{
			id = p_feat_id_array[a];

		}
	}
	ProArrayFree((ProArray *)&p_feat_id_array);
	ProArrayFree((ProArray *)&p_status_array);

	return id;
}


void creatatr(ProName pname, double value, ProParamvalueType typ)

{
	ProError error;
	std::ofstream log("logMain.log", std::ofstream::app);
	ProMdl p_model;
	ProModelitem p_model_item;
	ProParameter param;
	ProParamvalue provalue;

	///ProMdl bekommen 
	error = ProMdlCurrentGet(&p_model);
	if (error != 0)
	{
		log << "ProMdlCurrentGet returned" << error << std::endl;
	}

	///ProMdl wird zu ProModelitem
	error = ProMdlToModelitem(p_model, &p_model_item);
	if (error != 0)
	{
		log << "ProMdlToModelitem returned" << error << std::endl;
	}

	/// ProParameter initiieren
	error = ProParameterInit(&p_model_item, pname, &param);
	error = ProParameterValueGet(&param, &provalue);

	error = ProParamvalueSet(&provalue, &value, typ);


	error = ProParameterWithUnitsCreate(&p_model_item,
		pname,
		&provalue,
		NULL,
		&param);

	return;
}


void domtree(TreeNode **Node, DOMElement * DOMDOM)
{
	// Ausgang der Struktur der AML-Datei.

	std::ofstream log("addxmltrees.log", std::ofstream::app);

	int i = 0;
	for (DOMElement* pChild1 = DOMDOM->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
	{
		string attr_pChild1 = XMLString::transcode(DOMDOM->getAttribute(L"Name"));
		const wchar_t *ss;
		ss = DOMDOM->getAttribute(L"Name");
		string Ref_pChild1 = XMLString::transcode(DOMDOM->getAttribute(L"RefBaseSystemUnitPath"));
		string id_pChild1;
		string idname;
		log << "name: " << attr_pChild1 << endl;


		// Anfang des Nodetree
		string tagpchild1 = XMLString::transcode(pChild1->getTagName());
		if (tagpchild1.compare("InternalElement") == 0)
		{
			string attr = XMLString::transcode(DOMDOM->getAttribute(L"Name"));
			attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));
			Ref_pChild1 = XMLString::transcode(pChild1->getAttribute(L"RefBaseSystemUnitPath"));
			id_pChild1 = XMLString::transcode(pChild1->getAttribute(L"ID"));
			idname = "id" + attr_pChild1;
			log << "name: " << attr_pChild1 << endl;
			child_frame.Name = attr_pChild1;
			child_frame.ref = Ref_pChild1;
			v.push_back(child_frame);

			child_frame.Name = idname;
			child_frame.ref = id_pChild1;

			vid.push_back(child_frame);

			(*Node)->appendChild(new  TreeNode(attr_pChild1, Ref_pChild1));

			TreeNode *child = (*Node)->getChild(i);

			DOMElement* ppChild = pChild1->getFirstElementChild();

			log << "ref: " << Ref_pChild1 << endl;
			i++;
			if (ppChild == 0)
			{
				log << "nochild" << endl;
				continue;
			}

			else
				//TreeNode * child = (*Node)->getChild(i);
			{
				log << "child" << endl;
				DOMElement* pChild2 = pChild1;

				domtree(&child, pChild2);  //Substruktur

			}
		}
	}
	return;
}


void creatAml(TreeNode* filetree, ProModelitem p_model_item)
{
	/// Xerces initiieren 
	//XMLPlatformUtils::Initialize();
	ofstream log("writeaml.log", ofstream::app);
	log << "initiating the platform" << std::endl;

	xercesc::XMLPlatformUtils::Initialize();
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(gLS);
	DOMDocument *pDOMDocument = NULL;
	// leeres DOMDocument wird erstellt

	///DOMDocument erstellen und Rootelement festlegen
	pDOMDocument = impl->createDocument(0, L"CAEXFile", 0);	// Root erstellen
	DOMElement * pRootElement = NULL;
	pRootElement = pDOMDocument->getDocumentElement();// File name als Child von Rootelement festlegen

	string rootname;
	rootname = filetree->getTextContent();
	string filename;

	filename = rootname + "output.aml";
	string outputpfad;
	outputpfad = amlordner + "\\" + filename;

	pRootElement->setAttribute(L"FileName", s2cwc(filename));
	pRootElement->setAttribute(L"SchemaVersion", L"2.15");
	pRootElement->setAttribute(L"xsi:noNamespaceSchemaLocation", L"CAEX_ClassModel_V2.15.xsd");
	pRootElement->setAttribute(L"xmlns:xsi", L"http://www.w3.org/2001/XMLSchema-instance");

	DOMElement * padd = NULL;
	padd = pDOMDocument->createElement(L"AdditionalInformation"); //AdditionalInformation  erstellen
	padd->setAttribute(L"AutomationMLVersion", L"2.0");
	pRootElement->appendChild(padd);

	DOMElement * ExternalReference1 = NULL;
	ExternalReference1 = pDOMDocument->createElement(L"ExternalReference"); //ExternalReference erstellen
	ExternalReference1->setAttribute(L"Path", L"Libs/RoleClass Libraries/AutomationMLBaseRoleClassLib.aml");
	ExternalReference1->setAttribute(L"Alias", L"BaseRoleClassLib");
	pRootElement->appendChild(ExternalReference1);

	DOMElement * ExternalReference2 = NULL;
	ExternalReference2 = pDOMDocument->createElement(L"ExternalReference"); //ExternalReference erstellen
	ExternalReference2->setAttribute(L"Path", L"Libs/InterfaceClass Libraries/AutomationMLInterfaceClassLib.aml");
	ExternalReference2->setAttribute(L"Alias", L"BaseInterfaceClassLib");
	pRootElement->appendChild(ExternalReference2);

	DOMElement * pInstanceElement = NULL;
	XMLCh *InstanceName = XMLString::transcode(rootname.c_str());
	pInstanceElement = pDOMDocument->createElement(L"InstanceHierarchy"); //InstanceHierarchy erstellen
	pInstanceElement->setAttribute(L"Name", L"Structure");
	pRootElement->appendChild(pInstanceElement); // die InstanceHierarchy und File verbinden

	DOMElement * pmInterfaces = NULL;
	pmInterfaces = pDOMDocument->createElement(L"InterfaceClassLib");
	pmInterfaces->setAttribute(L"Name", L"mInterfaces");
	pRootElement->appendChild(pmInterfaces);

	DOMElement * pModules = NULL;
	pModules = pDOMDocument->createElement(L"SystemUnitClassLib");
	pModules->setAttribute(L"Name", L"Modules");
	pRootElement->appendChild(pModules);

	DOMElement * peComponents = NULL;
	peComponents = pDOMDocument->createElement(L"SystemUnitClassLib");
	peComponents->setAttribute(L"Name", L"eComponents");
	pRootElement->appendChild(peComponents);

	DOMElement * pmComponents = NULL;
	pmComponents = pDOMDocument->createElement(L"SystemUnitClassLib");
	pmComponents->setAttribute(L"Name", L"mComponents");
	pRootElement->appendChild(pmComponents);


	log << "-------------add Component in Library---------------------- " << endl;
	creatmodulelib(pDOMDocument, pModules, pmInterfaces);

	creatcomplib(pDOMDocument, pmComponents, pmInterfaces, "mComponents");
	creatcomplib(pDOMDocument, peComponents, pmInterfaces, "eComponents");
	log << "-------------add interfacelib---------------------- " << endl;
	creatinterfacelib(pDOMDocument, log, pmInterfaces);
	log << "-------------add internal element in Instancehiercharchy---------------------- " << endl;
	creatIH(filetree, pDOMDocument, &pInstanceElement, log,
		pModules, pmComponents, peComponents, p_model_item, pmInterfaces);

	/// xml datei schreiben
	DOMLSSerializer *writer = NULL;
	writer = ((DOMImplementationLS*)impl)->createLSSerializer();
	DOMLSOutput *theOutputDesc = NULL;
	theOutputDesc = ((DOMImplementationLS*)impl)->createLSOutput();
	XMLFormatTarget *formTarget = 0;                                          /// need change

	formTarget = new LocalFileFormatTarget((XMLCh*)s2cwc(outputpfad));

	log << "#### this is second test ###" << endl;

	theOutputDesc->setByteStream(formTarget);
	writer->getDomConfig()->setParameter(L"format-pretty-print", true);
	writer->write(pDOMDocument, theOutputDesc);
	/*delete formTarget;*/
}


uiCmdCmdActFn Parameter_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)

{
	xercesc::XMLPlatformUtils::Initialize();

	return 0;
}



/// Funktion für den Export der Dateien



uiCmdCmdActFn Parameter_export(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{
	ProError error;
	std::ofstream log("export.log", std::ofstream::app);
	ProMdl pmodel;
	ProModelitem pmodelitem;
	log << "------------exportanfang" << std::endl;

	//	ProParamvalueType protype;
	ProMdlType promdltype = ProMdlType::PRO_MDL_ASSEMBLY; //= PRO_ASSEMBLY;


														  // define a hash to represent promdlfiletype as normal string
	map<ProMdlfileType, string> typeHash;
	typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY] = asm_name;
	typeHash[ProMdlfileType::PRO_MDLFILE_PART] = prt_name;


	///Export the model  
	error = ProMdlCurrentGet(&pmodel);
	if (error != 0)
	{
		log << "ProMdlCurrentGet returned" << error << std::endl;
	}

	error = ProMdlToModelitem(pmodel, &pmodelitem);
	ProMdlName test_name;
	error = ProMdlMdlnameGet(pmodel, test_name);

	TreeNode *mdltree = new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]);
	//error = ProSessionMdlList(promdltype, &p_model_array, &p_model_num);
	mdltree->appendChild(new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]));

	TreeNode *childtree = mdltree->getChild(0);
	log << "---------------anfangaddtree-----------" << std::endl;
	//generate model tree
	addTrees(&childtree, pmodel, typeHash);
	//mdltree->appendChild(new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]));

	log << "-------endaddtree----startbibsyn-----------" << std::endl;

	bibsyncro(mdltree);

	log << "-------creataml-----------" << std::endl;
	// generate and write AML file to local disk
	creatAml(mdltree, pmodelitem);


	return 0;

}










