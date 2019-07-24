

#include <stdlib.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <map>
#include "TreeNode.h"
#include <algorithm>
#include <algorithm>
#include <io.h>
#include <cstring>
#include <string.h>
#include <stdio.h>
#include<direct.h> 


#include <cstdio>





/// Header f黵 DOM (Xerces) 
#include <PlatformUtils.hpp>
#include <AbstractDOMParser.hpp>
#include <DOMImplementation.hpp>
#include <DOMImplementationLS.hpp>
#include <DOMImplementationRegistry.hpp>
#include <DOMLSParser.hpp>
#include <DOMException.hpp>
#include <DOMDocument.hpp>
#include <DOMElement.hpp>
#include <DOMCount.hpp>
#include <DOMNodeList.hpp>
#include <DOMError.hpp>
#include <DOMLocator.hpp>
#include <DOMNamedNodeMap.hpp>
#include <DOMAttr.hpp>
#include <DOMErrorHandler.hpp>
#include <XMLString.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>


extern "C" void user_terminate()
{
}

/// Funtion zur Änderung der Parameter in Creo


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

void CreatePort(DOMDocument *pDOMDocument, ofstream &log, ProMdl handle,
	DOMElement * pChild, DOMElement *pmInterfaces)     //Create  Port(ExternalInterface) for the InternalElement
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
			
			log << "--------portCreateClasscess-----------------------------" << endl;
		}

	}

}

void ComponentLibraryUpdate(DOMElement *lib)    // Systemunitclass synchronsiert mit der Klassenbibliothek in Creo 
{
	std::ofstream log("bibsyn.log", std::ofstream::app);
	string foldername = XMLString::transcode(lib->getAttribute(L"Name"));

	for (DOMElement * pInstanceElement = lib->getFirstElementChild(); pInstanceElement != 0; pInstanceElement = pInstanceElement->getNextElementSibling())
	{

		string sucname = XMLString::transcode(pInstanceElement->getAttribute(L"Name"));
		log << "sucname is: " << sucname << endl;
		
		string opath;
		opath = bib + "\\" + foldername+"\\" + sucname;
		string fpath;
		fpath = opath + "\\" + sucname;
		ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
		ProMdlfileType type2 = PRO_MDLFILE_PART;
		ProPath cpath;
		ProError status;
		ProError status1;
		ProMdl chandle;
		
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
			log << "no file in bib:" << sucname << endl;
			ProError einbau;
			ProSolid   phandle;

			ProMdlName new_name;
			char* asmname = s2c(sucname);
			delete[] s2c(sucname);

			ProStringToWstring(new_name, asmname);
						
			einbau = ProSolidMdlnameCreate(new_name, PRO_MDLFILE_ASSEMBLY, &phandle);
			log << "createsuccess:" << sucname << einbau << endl;

			_mkdir(opath.c_str());

			einbau = ProMdlnameBackup(phandle, cpath);                     // speichern
			log << "savesuccess:" << sucname << einbau << endl;

			
		}
		
	}
}


void ComponentLibraryModification(ProMdl p_model, map<ProMdlfileType, string> typeHash)    // Systemunitclass im Creo als Parameter speichern. Eingang: Parametername/Value, Besitzer
{
	ProError error;
	ProMdl pmodel;
	ProModelitem pmodelitem;
	
	std::ofstream log("bibsyn.log", std::ofstream::app);
	error = ProMdlCurrentGet(&pmodel);
	if (error != 0)
	{
		log << "ProMdlCurrentGet returned" << error << std::endl;
	}

	error = ProMdlToModelitem(pmodel, &pmodelitem);


	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	error = ProMdlDependenciesCleanup(p_model);

	error = ProMdlDependenciesDataList(p_model, &depd, &files, &num_pdep);

	for (int i = 0; i < num_pdep; i++)
	{
		ProMdlCurrentGet(&pmodel);
		ProModelitem p_model_item;
		ProParameter param;
		ProParamvalue proval;
		string child_name;
		string Str;
		ProMdlToModelitem(pmodel, &p_model_item);
		error = ProParameterInit(&p_model_item,
			(depd + i)->name,
			&param);
		if (error != 0)
		{
			log << error << endl;
		}

		error = ProParameterValueGet(&param, &proval);

		if (error == 0)
		{
			// Systemunitclasslibrary erzeugen

			wchar_t *value;
			value = proval.value.s_val;

			//string Str = string(cStr);
			Str = w2string(value);
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
		error = ProMdlnameRetrieve((depd + i)->name, *(files + i), &p_model);
		if (!strstr(Str.c_str(), "Module"))
			continue;
		else
		{
			ComponentLibraryModification(p_model, typeHash);
		}

	}
}

void CreateInterfaceLibFromCAD(DOMDocument *pDOMDocument, ofstream &log, DOMElement *pmInterfaces)
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

void CreateConnector(DOMDocument *pDOMDocument, ofstream &log, TreeNode* child,
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
void CreateInternallink(DOMDocument *pDOMDocument, ofstream &log,
	DOMElement * pChild)     // Create the InternalLink for the Internalelement
{
	DOMElement *connector;
	string ifconector;
	ifconector = XMLString::transcode(pChild->getAttribute(L"Name"));
	log << "internallinkfor:" << ifconector << endl;
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
		log << "condition1" << endl;

		for (a; a<n_features; a++)
		{
			/* Get a handle to the feature */
			error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
			error = ProFeatureTypeGet(&feat, &p_type);

		
			 if (p_type == 1000)                                     // 1000 bedeutet ist ein koeper,

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

	

	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	error = ProMdlDependenciesCleanup(handle);

	error = ProMdlDependenciesDataList(handle, &depd, &files, &num_pdep);

	log << "condition2" << endl;

	for (int i = 0; i < num_pdep; i++)
	{

		ProMdl cmodel;
		error = ProMdlnameInit((depd + i)->name, *(files + i), &cmodel);


		ProFeature cfeat;
		int*	    cp_feat_id_array;
		int		    cn_features;
		int b = 0;
		ProFeattype cp_type;
		ProFeatStatus *cp_status_array;

		error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&cp_feat_id_array);
		error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&cp_status_array);

		/* Get a list of features of the specified solid and their statuses */
		error = ProSolidFeatstatusGet(ProMdlToSolid(cmodel), &cp_feat_id_array, &cp_status_array,
			&cn_features);



		for (b; b < cn_features; b++)
		{
			/* Get a handle to the feature */
			error = ProFeatureInit(ProMdlToSolid(cmodel), cp_feat_id_array[b], &cfeat);
			error = ProFeatureTypeGet(&cfeat, &cp_type);

			if (cp_type == 1118)                  //1118 bedeutet interface
			{
				error = ProModelitemNameGet(&cfeat, portname);

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



	}

	}

}

void CreateInternallink1(DOMDocument *pDOMDocument, ofstream &log, TreeNode **Node, map<ProMdlfileType, string> typeHash,
	DOMElement * pChild)     // Probeversion
{
	log << "start" << endl;
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
	log << "start2" << endl;
	log << "name:" << ifconector << endl;
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

		log << "start3" << endl;
		for (a; a<n_features; a++)
		{
			/* Get a handle to the feature */
			error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
			error = ProFeatureTypeGet(&feat, &p_type);


			if (p_type == 1000)                                     // 1000 bedeutet ist ein koeper,

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






			ProMdlnameShortdata *depd;
			ProMdlfileType * files;
			int num_pdep;
			error = ProMdlDependenciesCleanup(handle);

			error = ProMdlDependenciesDataList(handle, &depd, &files, &num_pdep);

			for (int i = 0; i < num_pdep; i++)
			{
				log << "promdlinitial" << endl;
				(*Node)->appendChild(new TreeNode(w2string((depd + i)->name), typeHash[*(files + i)]));
				TreeNode *child = (*Node)->getChild(i);
				ProMdl cmodel;
				error = ProMdlnameInit((depd + i)->name, *(files + i), &cmodel);

				error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
				error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

				/* Get a list of features of the specified solid and their statuses */
				error = ProSolidFeatstatusGet(ProMdlToSolid(cmodel), &p_feat_id_array, &p_status_array,
					&n_features);

				ProFeature cfeat;
				int*	    cp_feat_id_array;
				int		    cn_features;
				int b = 0;
				ProFeattype cp_type;


				for (b; b < cn_features; b++)
				{
					/* Get a handle to the feature */
					error = ProFeatureInit(ProMdlToSolid(cmodel), cp_feat_id_array[b], &cfeat);
					error = ProFeatureTypeGet(&cfeat, &cp_type);

					if (cp_type == 1118)                  //1118 bedeutet interface
					{
						error = ProModelitemNameGet(&cfeat, portname);

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



			}




		}


	}

}


class Createee
{



};

Createee sss;

void CreateClass(ProName pname, string value, ProMdl p_model)    // Systemunitclass im Creo als Parameter speichern. Eingang: Parametername/Value, Besitzer
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




void CreateModulelib(DOMDocument *pDOMDocument, DOMElement * pModules, DOMElement *pmInterfaces)  // create the library of the modul
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


					CreatePort(pDOMDocument, log, chandle, libComponent, pmInterfaces);

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








void CreateComplib(DOMDocument *pDOMDocument, DOMElement * pModules, DOMElement *pmInterfaces, string complibname)// create the library of the component
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

					CreatePort(pDOMDocument, log, chandle, libComponent, pmInterfaces);

					pModules->appendChild(libComponent);


				}
			}

		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}


}

void BuildElement(TreeNode* filetree, int i,DOMDocument *pDOMDocument, TreeNode *child,DOMElement * pChild, DOMElement * peComponents, ProModelitem p_model_item, DOMElement *pmInterfaces, DOMElement **pInstanceElement,DOMElement * pModules, DOMElement * pmComponents)
{// add Internalelemt, internallink , classinformation
	ProError error;
	DOMElement * pport = NULL;
	DOMElement * libport = NULL;
	DOMElement * connector = NULL;
	DOMElement * libComponent = NULL;
	ProParamvalue  proval;
	string child_name;
	ProMdlName  name;
	DOMElement * pAttr2 = NULL;
	DOMText* pTextNode = NULL;
	ofstream log("writeaml.log", ofstream::app);
	DOMElement * pValue2 = NULL;
	child_name = child->getTextContent();
	
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
		ComponentLibraryModification(child_name,Str);

		}
		*/

		pChild->setAttribute(L"RefBaseSystemUnitPath", s2cwc(Str));
		CreatePort(pDOMDocument, log, chandle, pChild, pmInterfaces);
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
				CreatePort(pDOMDocument, log, chandle, libComponent, pmInterfaces);
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
				CreatePort(pDOMDocument, log, chandle, libComponent, pmInterfaces);
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
				CreatePort(pDOMDocument, log, chandle, pModule, pmInterfaces);

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
			//CreateConnector(pDOMDocument, log1, filetree,	*pInstanceElement);

			CreateInternallink(pDOMDocument, log1, *pInstanceElement);

		}
	}
	
	
}



void CreateSystemUnitLibFromCAD(DOMDocument *pDOMDocument, DOMElement * pModules, DOMElement * pmComponents, DOMElement * peComponents, DOMElement *pmInterfaces)
{
	// combine the function of Creation of modul und component
	CreateModulelib(pDOMDocument, pModules, pmInterfaces);
	CreateComplib(pDOMDocument, pmComponents, pmInterfaces, "mComponents");
	CreateComplib(pDOMDocument, peComponents, pmInterfaces, "eComponents");

}



// Creat Instancehierarchy
void CreateInstanceHierarchyFromCAD(TreeNode* filetree, DOMDocument *pDOMDocument, DOMElement **pInstanceElement, ofstream &log,
	DOMElement * pModules, DOMElement * pmComponents, DOMElement * peComponents, ProModelitem p_model_item, DOMElement *pmInterfaces)
{
	
	DOMElement * pport = NULL;
	DOMElement * libport = NULL;
	DOMElement * connector = NULL;
	DOMElement * pChild = NULL;
	DOMElement * libComponent = NULL;
	int a = 0;
	
	//vector<TreeNode *> children = filetree->getChild;
	for (int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
				
		pChild = pDOMDocument->createElement(L"InternalElement");
		BuildElement(filetree, i, pDOMDocument, child, pChild, peComponents, p_model_item, pmInterfaces, pInstanceElement, pModules, pmComponents);


		if (child->hasChildren())
			CreateInstanceHierarchyFromCAD(child, pDOMDocument, &pChild, log, pModules, pmComponents, peComponents, p_model_item, pmInterfaces);
		else
			continue;
		
	}
}




// die Sturctureinformation in tree speichern

void AddTrees(TreeNode **Node, ProMdl p_model, map<ProMdlfileType, string> typeHash)
{
	std::ofstream log("addTrees.log", std::ofstream::app);
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
			AddTrees(&child, p_model, typeHash);
		}
	}
}

// create the rootelemtn of the mcad
ProSolid CreateRootelement(string mode)
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


void CreateAttribute(ProName pname, double value, ProParamvalueType typ)

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


// speichern the structureinformation of the aml file

void DOMtree(TreeNode **Node, DOMElement * DOMDOM)
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

				DOMtree(&child, pChild2);  //Substruktur

			}
		}
	}
	return;
}

struct result
{
	int a;
	ProMdlfileType b;
	string c;
};


// serach the library file
result * SearchElement(string mode)
{
	result *test = new result;
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


	//string curr="E:\\cad_Li\\PPU\\Bibliothek\\";     


	ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
	ProMdlfileType type2 = PRO_MDLFILE_PART;
	ProPath cpath;
	ProError status;
	ProError status1;
	ProMdl chandle;
	ProStringToWstring(cpath, s2c(ocurr));
	delete[] s2c(ocurr);

	status = ProMdlFiletypeLoad(cpath,
		type1,
		PRO_B_FALSE,
		&chandle);

	status1 = ProMdlFiletypeLoad(cpath,
		type2,
		PRO_B_FALSE,
		&chandle);

	if (status == 0)
	{
		test->a = status;
		test->b = type1;
		test->c = ocurr;

	}
	else if (status1 == 0)
	{
		test->a = status1;
		test->b = type2;
		test->c = ocurr;

	}
	else
	{
		test->a = -1;
		test->b = type1;
	}

	return test;
}


ProMdl f2m(ProMdl parent,int i)
{
	ProError error;
	std::ofstream log("f2m.log", std::ofstream::app);
	
	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	ProMdl p_model;
	error = ProMdlDependenciesCleanup(parent);

	error = ProMdlDependenciesDataList(parent, &depd, &files, &num_pdep);

	
	log  << w2string((depd + i)->name) << i << endl;

		error = ProMdlnameRetrieve((depd + i)->name, *(files + i), &p_model);
		
	return p_model;

}


int m2fid(ProMdl pmodel,ProMdl child)
{
	ProError status;
	
	ProMdlnameShortdata *depd;
	ProMdlfileType * files;
	int num_pdep;
	
	status = ProMdlDependenciesCleanup(pmodel);
	status = ProMdlDependenciesDataList(pmodel, &depd, &files, &num_pdep);
	ProName childname;
	ProMdlNameGet(child, childname);
	string name = w2string(childname);

	int j = 0;

	for (j; j < num_pdep; j++)
	{

		string cname = w2string((depd + j)->name);

		if (_stricmp(cname.c_str(), name.c_str())==0)
		{
			break;
		}
						
	}


	return j;
}



string link2dom(string ID, DOMElement* child)
{
	std::ofstream log("link2dom.log", std::ofstream::app);
	log << "ID: " << ID << endl;
	string attr_pChild1;
	for (DOMElement* pChild1 = child->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
	{
						
		string id_pChild1;
		string tagpchild1 = XMLString::transcode(pChild1->getTagName());
		if (tagpchild1.compare("InternalElement") == 0)
		{

			id_pChild1 = XMLString::transcode(pChild1->getAttribute(L"ID"));
			log << "IDchild: " << id_pChild1 << endl;
			if (id_pChild1.compare(ID) == 0)
			{

				attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));
				break;
			}
					
			
		}
	}

	log << "parentname: " << attr_pChild1 << endl;
	return attr_pChild1;

}

int CheckExist(ProFeature feat, string DomRefA,string DomRefB)
{
	int exist=0;
	ProError error;
	std::ofstream log("importlink.log", std::ofstream::app);
	
	log << "check exist" << endl;
	int*	    p_feat_id_array;
	
	int a = 0;
	
	ProFeatStatus *p_status_array = NULL;
	
		error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);
		error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_array);

		/* Get a list of features of the specified solid and their statuses */

		
		
				ProError err;
				ProElement elemtree;
				err = ProFeatureElemtreeExtract(&feat, NULL, PRO_FEAT_EXTRACT_NO_OPTS,
					&elemtree);
			
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

					(_stricmp(refB.c_str(), DomRefA.c_str()) == 0&&_stricmp(refA.c_str(), DomRefB.c_str()) == 0);
			if((_stricmp(refB.c_str(), DomRefA.c_str()) == 0 && _stricmp(refA.c_str(), DomRefB.c_str()) == 0) || (_stricmp(refB.c_str(), DomRefB.c_str()) == 0 && _stricmp(refA.c_str(), DomRefA.c_str()) == 0))
				
				
			{
				exist = 1;
			
			}

				}

				if(exist==1)
				{
					log << "Replacement no need" << endl;
				}
				return exist;

			}


void CheckInterface(DOMElement * DOMDOM)     // compare the interface in Creo and ExternalInterface in AML
{

	std::ofstream log("CheckInterface.log", std::ofstream::app);
	int i = 0;
	vector<string> vectora;
	vector<string> vectorb;
	ProError error;
	log << "----------Test Anfang------------------------" << endl;
	for (DOMElement* pChild1 = DOMDOM->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
	{
		string attr_pChild1 = XMLString::transcode(DOMDOM->getAttribute(L"Name"));
		const wchar_t *ss;
		ss = DOMDOM->getAttribute(L"Name");

		string tagpchild1 = XMLString::transcode(pChild1->getTagName());
		if (tagpchild1.compare("InternalElement") == 0)
		{
			attr_pChild1 = XMLString::transcode(pChild1->getAttribute(L"Name"));

			ProMdl chandle;
			ProName name;
			ProStringToWstring(name, s2c(attr_pChild1));
			delete[] s2c(attr_pChild1);

			error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &chandle);
			if (error != 0)
			{
				error = ProMdlnameInit(name, PRO_MDLFILE_PART, &chandle);
			}

			
			ProFeature cfeat;
			int*	    cp_feat_id_array;
			int		    cn_features;
			int b = 0;
			ProFeattype cp_type;
			ProFeatStatus *cp_status_array;

			error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&cp_feat_id_array);
			error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&cp_status_array);

			/* Get a list of features of the specified solid and their statuses */
			error = ProSolidFeatstatusGet(ProMdlToSolid(chandle), &cp_feat_id_array, &cp_status_array,
				&cn_features);



			for (b; b < cn_features; b++)
			{
				/* Get a handle to the feature */
				error = ProFeatureInit(ProMdlToSolid(chandle), cp_feat_id_array[b], &cfeat);
				error = ProFeatureTypeGet(&cfeat, &cp_type);
				ProName portname;

				if (cp_type == 1118)                  //1118 bedeutet interface
				{
					error = ProModelitemNameGet(&cfeat, portname);
					string ppname = w2string(portname);
					vectorb.push_back(ppname);
				}




			}





			for (DOMElement* face1 = pChild1->getFirstElementChild(); face1 != 0; face1 = face1->getNextElementSibling())
			{
				string interfacename;
				string tagpchild = XMLString::transcode(face1->getTagName());

				if (tagpchild.compare("ExternalInterface") == 0)
				{
					interfacename = XMLString::transcode(face1->getAttribute(L"Name"));
					int exist = 0;
					for (i = 0; i < vectorb.size(); i++)
					{
						if (_stricmp(interfacename.c_str(), vectorb[i].c_str())==0)
						{

							exist = 1;
						}
					
					}

					if (exist == 0)
					{
						log << "add interface " + interfacename + " in " + attr_pChild1 << endl;
					}


					vectora.push_back(interfacename);

				}




			}


			int m;
			for (m = 0; m < vectorb.size(); m++)
			{

				int exist = 0;

				for (i = 0; i < vectora.size(); i++)
				{
					if (_stricmp(vectorb[m].c_str(), vectora[i].c_str())==0)
					{

						exist = 1;
					}
				

				}



				if (exist == 0)
				{
					log << "delete interface " + vectorb[m] + " in " + attr_pChild1 << endl;
				}


			}

			
			DOMElement* ppChild = pChild1->getFirstElementChild();

			if (ppChild == 0)
			{

				continue;
			}

			else
				
			{

				CheckInterface(pChild1);

			}




		}

	}
}

// input two component interface of creo, output new placement

void  Replacement(ProFeature platz,ProFeature interface1, ProFeature interface2)
{
	
	ProMdl comp;
	std::ofstream log("Replacement.log", std::ofstream::app);
	ProError err;
	ProElement elemtree;
	ProError status;
	

	ProName name;
	ProModelitemNameGet(&interface1, name);
	log << "inf1:" << w2string(name) << endl;

	ProName name2;
	ProModelitemNameGet(&interface2, name2);
	log << "inf2:" << w2string(name2) << endl;
		
	err = ProFeatureElemtreeExtract(&platz, NULL, PRO_FEAT_EXTRACT_NO_OPTS,
		&elemtree);

	ProElement *chai;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai);
	err = ProElementArrayGet(elemtree, NULL, &chai);
	ProValue asmvalue;
	ProValueData asmvalue_data;
	ProValue comvalue;
	ProValueData comvalue_data;
	
	ProAsmcomppath comp_path1;
	ProIdTable c_id_table;
	int id=0;
	ProMdlIdGet(interface2.owner, &id);

	log << "id is " << id << endl;
	ProName dddd;
	ProMdlNameGet(interface2.owner, dddd);
	log << "modelname is" << w2string(dddd) << endl;
	int position = 0;
	position = m2fid(platz.owner,interface2.owner);
	id = mdl2feat(ProMdlToSolid(platz.owner), position);
	c_id_table[0] = id;
	log << "id is " << id << endl;
		
	status = ProAsmcomppathInit(ProMdlToSolid(platz.owner), c_id_table, 1, &comp_path1);

	if (status != 0)
	{
		log << "ProAsmcomppathInit" << endl;
	}
	ProSelection csele2;
	status = ProSelectionAlloc(&comp_path1, &interface2, &csele2);


	ProReference REPDEP_ref2;

	err = ProSelectionToReference(csele2, &REPDEP_ref2);
	ProReference refe;
	ProReferenceAlloc(&refe);

	err = ProReferenceSet(refe, NULL, &interface2);
	if (err != 0)
	{
		log << "Proreferencesetfail:" << err << endl;
	}
	ProElement *chai1;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai1);

	err = ProElementArrayGet(*(chai + 4), NULL, &chai1);



	int y = 0;
	status = ProElementIntegerGet(*(chai1), NULL, &y);

	log << status << "ProElementIntegerGet" << y << endl;
	status = ProElementIntegerSet(*(chai1), 2);
	log << "ProElementIntegerSet" << status << endl;

	ProSelection csele;
	status = ProSelectionAlloc(NULL, &interface1, &csele);
	
	ProReference REPDEP_ref1;

	err = ProSelectionToReference(csele, &REPDEP_ref1);
	log << err << endl;
	err = ProElementReferenceSet(*(chai1 + 1), REPDEP_ref1);
	log << err << endl;

	ProReferenceFree(REPDEP_ref1);


	ProElement *chai2;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai2);        //*(chai1+2) PROE COMP PLACE INTERFACE ASSEMS
	err = ProElementArrayGet(*(chai1 + 2), NULL, &chai2);
	log << err << endl;


	ProElement *chai3;

	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai3);       //*(chai2) PROE COMP PLACE INTERFACE ASSEM


	err = ProElementArrayGet(*(chai2), NULL, &chai3);

	ProReference data;
	err = ProElementReferenceGet(*(chai3), NULL, &data);

	log << "ProElementReferenceGet" << err << endl;

	if (err != 0)
	{
		ProElement ASSEM;

		status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM, &ASSEM);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM
		status = ProElemtreeElementAdd(*(chai1 + 2), NULL, ASSEM);
		log << "ProElemtreeElementAdd:" << status << endl;
		
		ProElement asmref;

		status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM_REF, &asmref);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM


		status = ProElemtreeElementAdd(ASSEM, NULL, asmref);
		log << "ProElemtreeElementAdd:" << status << endl;


		err = ProElementReferenceSet(asmref, REPDEP_ref2);
		log << err << "ProElementReferenceSet" << endl;




	}


	else
	{
		ProSelection selection;

		ProReferenceToSelection(data, &selection);
		ProReferenceFree(data);
		ProModelitem item;
		ProSelectionModelitemGet(selection, &item);
		ProAsmcomppath   ppath;
		ProSelectionAsmcomppathGet(selection, &ppath);


		ProName itemname;
		ProModelitemNameGet(&item, itemname);

		log << "name:" << w2string(itemname) << endl;



		err = ProElementReferenceSet(*(chai3), REPDEP_ref2);
		log << err << "ProElementReferenceSet" << endl;

		err = ProElementArraySet(*(chai2), NULL, chai3);

		log << err << "ProElementArraySet" << endl;
		err = ProElementArraySet(*(chai1 + 2), NULL, chai2);
		log << err << "ProElementArraySet" << endl;
	}




	ProReferenceFree(REPDEP_ref2);

	ProFeatureCreateOptions *opts = 0;
	status = ProArrayAlloc(1, sizeof(ProFeatureCreateOptions),
		1, (ProArray*)&opts);

	opts[0] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;

	//opts[0] = PRO_FEAT_CR_NO_OPTS;
	ProErrorlist   errors;

	ProAsmcomppath   comp_path;

	ProSelection model_sel;
	status = ProSelectionAlloc(NULL, &platz, &model_sel);
	status = ProSelectionAsmcomppathGet(model_sel, &comp_path);

	err = ProFeatureWithoptionsRedefine(&comp_path, &platz, elemtree, opts, PRO_REGEN_NO_FLAGS, &errors);


	

	
}


struct FindLinkresult
{
	int a;
	DOMElement *b;

};



FindLinkresult * FindLink(ProMdl Child , DOMElement* child, ProMdl parent,int a)
{
	FindLinkresult *test = new FindLinkresult;
	std::ofstream log("FindLink.log", std::ofstream::app);
	ProName name;
	ProMdlNameGet(Child,name);
	string childname=w2string(name);
	DOMElement * link;
	int exist = 0;
	ProError error;
	int findb = 0;
	int b = 0;
	for (DOMElement * pInstanceElement = child->getFirstElementChild(); pInstanceElement != 0; pInstanceElement = pInstanceElement->getNextElementSibling())
	{

		string tagname = XMLString::transcode(pInstanceElement->getTagName());
		log << "tagname is: " << tagname << endl;
		
		/// Entscheiden ob es Instancehierarchy ist
		if (tagname.compare("InternalLink") == 0)

		{
			string Name = XMLString::transcode(pInstanceElement->getAttribute(L"Name")); //InstanceHierarchy lesen
			log << "InstanceName is: " << Name << endl;       //// InternalElementname 
			
			string RefPartnerSideAName = XMLString::transcode(pInstanceElement->getAttribute(L"RefPartnerSideA"));
			log << RefPartnerSideAName << endl;
			string RefPartnerSideA = RefPartnerSideAName.erase(0, RefPartnerSideAName.find(':') + 1);
			
			log << "RefPartnerSideA:" << RefPartnerSideA << endl;
			RefPartnerSideAName = XMLString::transcode(pInstanceElement->getAttribute(L"RefPartnerSideA"));
			string RefAID = RefPartnerSideAName.erase(RefPartnerSideAName.find(':') );
			log << "RefAID:" << RefAID << endl;
			

			string RefAName = link2dom(RefAID, child);

			
			string RefPartnerSideBName = XMLString::transcode(pInstanceElement->getAttribute(L"RefPartnerSideB"));
			string RefPartnerSideB = RefPartnerSideBName.erase(0, RefPartnerSideBName.find(':') + 1);
			RefPartnerSideBName = XMLString::transcode(pInstanceElement->getAttribute(L"RefPartnerSideB"));
			string RefBID = RefPartnerSideBName.erase(RefPartnerSideBName.find(':') );
			log << "RefBID:" << RefBID << endl;
			string RefBName = link2dom(RefBID, child);
			log << "childname: "<< childname << endl;
			log << "refa: " << RefAName << endl;
			log << "refb: " << RefBName << endl;
			string compname;

			if(_stricmp(childname.c_str(), RefAName.c_str())==0|| _stricmp(childname.c_str(), RefBName.c_str())==0)
			{
				
				if(_stricmp(childname.c_str(), RefAName.c_str()) == 0)
				{
					compname = RefBName;
				}
				else
				{
					compname = RefAName;
				}


				ProMdlnameShortdata *depd;
				ProMdlfileType *files;
				int num_pdep;
				error = ProMdlDependenciesDataList(parent, &depd, &files, &num_pdep);
				
				for (int i = 0; i < num_pdep; i++)
				{
					ProMdl cmodel;
					if (_stricmp(compname.c_str(), w2string((depd + i)->name).c_str()) == 0)
					{
						b = i;
						findb = 1;
						break;
					}

					
				}
				
				
				if(a>b&&findb==1)
				
				{ 				
				
				link = pInstanceElement;
				exist = 1;
				log << "getlink" << endl;
				break;
				}
			}


		}
	}
	test->a = exist;
	test->b = link;
	return test;

}


struct interfacejg
{
	int a;
	ProFeature b;
	
};






interfacejg* FindInterface(ProMdl handle, string Iname)
{
		
	interfacejg *test = new interfacejg;
	
	ProError error;		
	ProName portname;
	ProFeature feat;
	int*	    p_feat_id_arrayc=NULL;
	std::ofstream log("importlink.log", std::ofstream::app);
	int		    n_features;
	int a = 0;
	int exist = 0;
	ProFeattype p_type;
	ProFeatStatus *p_status_arrayc = NULL;
	error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_arrayc);
	error = ProArrayAlloc(0, sizeof(ProFeatStatus), 1, (ProArray *)&p_status_arrayc);
	log <<"gesuchtinterface"<< Iname << endl;

	/* Get a list of features of the specified solid and their statuses */
	error = ProSolidFeatstatusGet(ProMdlToSolid(handle), &p_feat_id_arrayc, &p_status_arrayc,
		&n_features);
	
	if(n_features!=0)
	{
	
	for (a; a < n_features; a++)
	{
		/* Get a handle to the feature */
		error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_arrayc[a], &feat);
		error = ProFeatureTypeGet(&feat, &p_type);

		if (p_type == 1118)
		{
			error = ProModelitemNameGet(&feat, portname);
			log << "portname:" << w2string(portname) << endl;
			if (_stricmp(Iname.c_str(), w2string(portname).c_str())==0)
			{
				exist = 1;
			}
			break;

		}
		
	}

	error = ProArrayFree((ProArray*)&p_status_arrayc);
	error = ProArrayFree((ProArray*)&p_feat_id_arrayc);
	
	test->a = exist;
	test->b = feat;
	
	
	}


	log << "exist:" <<exist<< endl;
		return test;
	}



void ImportInternalLinks(DOMElement* child)
{
	std::ofstream log("importlink.log", std::ofstream::app);
	
	string mname;
	mname = XMLString::transcode(child->getAttribute(L"Name"));
	log << "Replacement for:" << mname << endl;
	ProError error;
	ProName name;
	ProStringToWstring(name, s2c(mname));
	delete[] s2c(mname);
	ProMdl handle;
	
	ProFeature feat;
	int*	    p_feat_id_array;
	int		    n_features;
	int a = 0;
	ProFeattype p_type;
	int b=0;
	ProFeatStatus *p_status_array = NULL;
	error = ProMdlnameInit(name, PRO_MDLFILE_ASSEMBLY, &handle);             // Initial the CAD-Model
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


		for (a; a < n_features; a++)
		{
			error = ProFeatureInit(ProMdlToSolid(handle), p_feat_id_array[a], &feat);
			error = ProFeatureTypeGet(&feat, &p_type);
			
			
			if (p_type == 1000)                                // first component don´t need check
			{
				if(b>0)
				{ProMdl mchild;

				mchild = f2m(handle, b);
				
								
				DOMElement *link1 = NULL;

				FindLinkresult *tt = NULL;
				tt = FindLink(mchild, child,handle,b);
				link1 = tt->b;


				if (tt->a == 1)
				{
					string linkname= XMLString::transcode(link1->getAttribute(L"Name"));
					log << "FindLink success:" << linkname<<endl;
					string RefPartnerSideAName = XMLString::transcode(link1->getAttribute(L"RefPartnerSideA"));

					string RefPartnerSideA = RefPartnerSideAName.erase(0, RefPartnerSideAName.find(':') + 1);
					RefPartnerSideAName = XMLString::transcode(link1->getAttribute(L"RefPartnerSideA"));
					string RefAID = RefPartnerSideAName.erase(RefPartnerSideAName.find(':'));
					string RefPartnerA;
					RefPartnerA = link2dom(RefAID, child);

					string RefPartnerSideBName = XMLString::transcode(link1->getAttribute(L"RefPartnerSideB"));
					string RefPartnerSideB = RefPartnerSideBName.erase(0, RefPartnerSideBName.find(':') + 1);
					RefPartnerSideBName = XMLString::transcode(link1->getAttribute(L"RefPartnerSideB"));
					string RefBID = RefPartnerSideBName.erase(RefPartnerSideBName.find(':'));
					string RefPartnerB;
					RefPartnerB = link2dom(RefBID, child);
					log << "RefPartnerA :" << RefPartnerA << endl;
					log << "RefPartnerB :" << RefPartnerB << endl;
					
					ProFeature interfaceA;
					interfacejg *jgA;
					ProMdl mhandleA;
					ProName mnameA;
					ProStringToWstring(mnameA, s2c(RefPartnerA));

					delete[] s2c(RefPartnerA);
					error = ProMdlnameInit(mnameA, PRO_MDLFILE_ASSEMBLY, &mhandleA);             // Initial the CAD-Model
					if (error != 0)
					{

						error = ProMdlnameInit(mnameA, PRO_MDLFILE_PART, &mhandleA);
					}
					
					jgA = FindInterface(mhandleA, RefPartnerSideA);
					
					ProMdl mhandleB;
					ProName mnameB;
					ProStringToWstring(mnameB, s2c(RefPartnerB));

					delete[] s2c(RefPartnerB);
					ProError errorB;
					errorB = ProMdlnameInit(mnameB, PRO_MDLFILE_ASSEMBLY, &mhandleB);             // Initial the CAD-Model
					if (errorB != 0)
					{

						errorB = ProMdlnameInit(mnameB, PRO_MDLFILE_PART, &mhandleB);
					}




					if (error == 0 && errorB == 0)
					{
						ProFeature interfaceB;
						interfacejg *jgB;
						jgB = FindInterface(mhandleB, RefPartnerSideB);

						log << jgB->a <<"barrasingm e:" << jgA->a << endl;
						if (jgB->a == 1 && jgA->a == 1
							)
						{
							log << "FindInterface success" << endl;
							interfaceA = jgA->b;
							interfaceB = jgB->b;





							int exist = CheckExist(feat, RefPartnerSideA, RefPartnerSideB);
							if (exist == 0)
							{

								Replacement(feat, interfaceA, interfaceB);

							}
						}
					}
				}
			}
				b++;

			}




		}
	}

	log <<" Replacementchild" << endl;

	for (DOMElement* pChild1 = child->getFirstElementChild(); pChild1 != 0; pChild1 = pChild1->getNextElementSibling())
	{
				
		string tagpchild1 = XMLString::transcode(pChild1->getTagName());
		if (tagpchild1.compare("InternalElement") == 0)
		{
			string Ref_pChild1 = XMLString::transcode(pChild1->getAttribute(L"RefBaseSystemUnitPath"));

			if (strstr(Ref_pChild1.c_str(), "Module")!=0)
			{
				ImportInternalLinks(pChild1);
				
			}
		}
				
	}


}


// check ob the bauteil existiert in the aml file, if no, it will be deleted

void Consistent(TreeNode* mdltree)
{
	std::ofstream log("consistent.log", std::ofstream::app);
	ProError status;
	string cadchild_name;
	string child_name;
	ProMdl pmodel;
	ProMdlnameShortdata *depd = NULL;
	ProMdlfileType *files = NULL;
	ProName creocadname;
	string cad_name;
	int o = -1;
	ProFeatureDeleteOptions *delete_opts = 0;
	int *feat_ids = NULL;
	cad_name = mdltree->getTextContent();
	ProStringToWstring(creocadname, s2c(cad_name));

	int num_pdep;
	status = ProMdlnameRetrieve(creocadname, PRO_MDLFILE_ASSEMBLY, &pmodel);

	if (status != 0)
		log << "ProMdlnameRetrieve" << status << endl;
	status = ProMdlDependenciesCleanup(pmodel);
	status = ProMdlDependenciesDataList(pmodel, &depd, &files, &num_pdep);
	if (status != 0)
		log << "ProMdlDependenciesDataList" << status << endl;

	int j = 0;

	for (j; j < num_pdep; j++)
	{

		cadchild_name = w2string((depd + j)->name);

		log << cadchild_name << j << endl;
		int i = 0;
		for (i; i < mdltree->childrenNumber(); i++)
		{

			TreeNode *child = mdltree->getChild(i);
			child_name = child->getTextContent();
			log << "--------------" << child_name << endl;
			if (!_stricmp(cadchild_name.c_str(), child_name.c_str()))
			{
				break;
			}

		}

		if (i == mdltree->childrenNumber())
		{
			o++;
			int id = 0;

			id = mdl2feat(ProMdlToSolid(pmodel), j - o);

			status = ProArrayAlloc(1, sizeof(int),
				1, (ProArray*)&feat_ids);
			feat_ids[0] = id;

			ProFeatureDeleteOptions opt[] = { PRO_FEAT_DELETE_CLIP };

			status = ProArrayAlloc(1, sizeof(ProFeatureDeleteOptions),
				1, (ProArray*)&delete_opts);

			delete_opts[0] = PRO_FEAT_DELETE_CLIP;


			status = ProFeatureWithoptionsDelete(ProMdlToSolid(pmodel),
				feat_ids,
				delete_opts,
				PRO_REGEN_NO_FLAGS);
			log << "ProFeatureWithoptionsDelete" << status << "id:" << id << endl;
		}

	}


	int k = 0;
	for (k; k < mdltree->childrenNumber(); k++)
	{

		TreeNode *child1 = mdltree->getChild(k);

		log << "child" << endl;

		if (!child1->hasChildren())
			continue;
		else
		{
			Consistent(child1);
		}

	}

	return;
}


//helpfuncktion of create cad

void CreateStructure(TreeNode* filetree, ProSolid   hauptasm)
{

	std::ofstream log("logMain.txt", std::ofstream::app);
	string child_name;
	ProError status;
	ProMdl temple;

	for (int i = 0; i < filetree->childrenNumber(); i++)
	{
		TreeNode *child = filetree->getChild(i);
		child_name = child->getTextContent();
		log << "childname: " << child_name << endl;

		log << "childref " << child->getType() << endl;

		result *test;
		test = SearchElement(child->getType());


		// Ordner für erstellte Model
		ProPath   backup_dir;


		ProStringToWstring(backup_dir, s2c(ns));
		delete[] s2c(ns);
		log << "ooooooooooooo " << test->a << endl;
		log << "ooooooooooooo " << test->c << endl;
		ProMatrix matrix = { { 1.0, 0.0, 0.0, 0.0 },     //  Initialposition
		{ 0.0, 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 0.0, 1.0 } };

		ProMdlName new_name;
		char* asmname = s2c(child_name);
		delete[] s2c(child_name);

		ProStringToWstring(new_name, asmname);
		ProMdl asmasm;
		ProMdlfileType type1 = PRO_MDLFILE_ASSEMBLY;
		ProMdlfileType type2 = PRO_MDLFILE_PART;
		status = ProMdlnameInit(new_name, type1, &asmasm);
		if (status != 0)
		{
			status = ProMdlnameInit(new_name, type2, &asmasm);
		}

		if ((test->a) == 0 && status != 0)       // Es gibt Datei in die Bib

		{
			ProPath       cpath;

			ProStringToWstring(cpath, s2c(test->c));

			delete[] s2c(test->c);
			ProMdl       p_handle;
			ProError err;

			err = ProMdlFiletypeLoad(cpath,
				test->b,
				PRO_B_FALSE,
				&p_handle);

			log << "loadsuccess:" << err << endl;

			ProAsmcomp  p_comp_handle;

			ProError einbau1;

			einbau1 = ProAsmcompAssemble(ProMdlToAssembly(ProSolidToMdl(hauptasm)),
				ProMdlToSolid(p_handle),
				matrix,
				&p_comp_handle);


			ProError ee;
			ee = ProMdlnameRename(p_handle,
				new_name);                 // load die Bibliothekdatei und umnennen

			log << "condition1:" << einbau1 << endl;
		}

		else if ((test->a) == -1 && status != 0)           // Es gibt keine Element in die Bibliothek, neue leer Datei wird erstellt.
		{
			ProError einbau;
			ProSolid phandle;
			einbau = ProSolidMdlnameCreate(new_name, PRO_MDLFILE_ASSEMBLY, &phandle);

			ProAsmcomp  p_comp_handle;
			if (einbau == -5)
			{
				ProMdl transition;

				einbau = ProMdlnameInit(new_name, PRO_MDLFILE_ASSEMBLY, &transition);

				einbau = ProAsmcompAssemble(ProMdlToAssembly(ProSolidToMdl(hauptasm)),
					ProMdlToSolid(transition), matrix,
					&p_comp_handle);

			}
			else {
				einbau = ProMdlSave(ProSolidToMdl(phandle));


				einbau = ProAsmcompAssemble(ProMdlToAssembly(ProSolidToMdl(hauptasm)),
					ProMdlToSolid(phandle),
					matrix,
					&p_comp_handle);
			}

			//einbau=	ProAsmcompMdlnameCreateCopy(ProMdlToAssembly(ProSolidToMdl(hauptasm)),new_name,PRO_MDL_ASSEMBLY, NULL,PRO_B_FALSE ,&p_comp_handle );

			//einbau= ProAsmcompAssemble ( ProMdlToAssembly(ProSolidToMdl(hauptasm)),  phandle,        matrix,          &p_comp_handle );

			log << "condition2:" << einbau << endl;
		}

		if (child->hasChildren())
		{
			status = ProMdlnameInit(new_name, PRO_MDLFILE_ASSEMBLY, &temple);
			CreateStructure(child, ProMdlToSolid(temple));
		}
		else
			continue;

	}
	return;
}

//create the mcad-model

ProMdl CreateCAD(DOMElement* root)
{
	std::ofstream log("logMain.txt", std::ofstream::app);
	string instanceName;
	ProMdl hauptassembly;
	DOMElement* rootelement;
		for (DOMElement * pInstanceElement = root->getFirstElementChild(); pInstanceElement != 0; pInstanceElement = pInstanceElement->getNextElementSibling())
	{

		string tagname = XMLString::transcode(pInstanceElement->getTagName());
		log << "tagname is: " << tagname << endl;
		/// Entscheiden ob es Instancehierarchy ist
		if (tagname.compare("InstanceHierarchy") == 0)

		{
			instanceName = XMLString::transcode(pInstanceElement->getAttribute(L"Name")); //InstanceHierarchy lesen
			log << "InstanceName is: " << instanceName << endl;       //// InternalElementname 


			for (DOMElement* pChild = pInstanceElement->getFirstElementChild(); pChild != 0; pChild = pChild->getNextElementSibling())
			{

				// Anfang des Nodetree
				string name_pchild = XMLString::transcode(pChild->getTagName());
				if (name_pchild.compare("InternalElement") == 0)

				{
					rootelement = pChild;
					string attr_part = XMLString::transcode(pChild->getAttribute(L"Name"));
					string refre = XMLString::transcode(pChild->getAttribute(L"RefBaseSystemUnitPath"));
					TreeNode *mdltree = new TreeNode(attr_part, refre);
					// Instancehierchy als tree speichern
					child_frame.Name = attr_part;
					child_frame.ref = refre;
					v.push_back(child_frame);
					DOMtree(&mdltree, pChild);

					ProSolid phandle = CreateRootelement(attr_part);
					ProMdlName  nname;
					ProStringToWstring(nname, s2c(attr_part));
					// base on the tree the cad file erzeugen
					CreateStructure(mdltree, phandle);
					// delete the different part of cad
					log << "Consistent anfang" << endl;
					Consistent(mdltree);

					ProMdlnameInit(nname, PRO_MDLFILE_ASSEMBLY, &hauptassembly);


				}
				else
					continue;
			}


		}


	}
	
	CheckInterface(rootelement);
	ImportInternalLinks(rootelement);

	return hauptassembly;

}






/// Funktion für den Import von Parametern 
/// wird durch Klicken des Buttons "Parameter_import" ausgelöst

uiCmdCmdActFn Parameter_import(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)

{
	xercesc::XMLPlatformUtils::Initialize();
	std::ofstream log("import.log", std::ofstream::app);
	long Handle;
	struct _finddata_t FileInfo;
	Handle = _findfirst("*.asm.1", &FileInfo);
	if (Handle == -1L)
	{
		log << "cant find" << endl;
	}

	else {
		string pppp = FileInfo.name;
		int s = remove(pppp.c_str());
		log << FileInfo.name << endl;
		while (_findnext(Handle, &FileInfo) == 0)
		{
			string pppp = FileInfo.name;
			remove(pppp.c_str());
		}
		log << "delete:" << FileInfo.name << endl;
		_findclose(Handle);
	}
	log << "------------------------like that----------------------------------- " << endl;
	XMLCh gLS[] = { chLatin_L, chLatin_S, chNull };
	DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(gLS);
	DOMLSParser* parser = impl->createLSParser(DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	DOMDocument* doc = NULL;
	doc = parser->parseURI(xmlFile);
	DOMElement* root = doc->getDocumentElement();//rootElement
												 //DOMElement* rootModel;
	string name = XMLString::transcode(root->getNodeName());
	ProPath   backup_dir;
	string instanceName;
	ProMdl hauptassembly;
	ProStringToWstring(backup_dir, s2c(ns));
	delete[] s2c(ns);
		
	DOMElement* rootelement;
	log << "------------------------Parameterimport Testanfang----------------------------------- " << endl;

	for (DOMElement * syslib = root->getFirstElementChild(); syslib != 0; syslib = syslib->getNextElementSibling())
	{
		string tagname0 = XMLString::transcode(syslib->getTagName());     
		if (tagname0.compare("SystemUnitClassLib") == 0)      /// Entscheiden ob es SystemUnitClassLib ist
		{
			ComponentLibraryUpdate(syslib);
		}
	}

	log << "------------------------Synchronisation von Bibliothek fertig----------------------------------- " << endl;

	hauptassembly=CreateCAD(root);
	
	// Systemunitclasslib als Parameter speichern


	double n = v.size();
	log << "vsize: " << n << endl;

	for (int i = 0; i<n; i++)
	{
		ProName ppname;
		ProStringToWstring(ppname, s2c(v[i].Name));
		delete[] s2c(v[i].Name);
		CreateClass(ppname, v[i].ref, hauptassembly);
		log << "creat systemunitclass in den Parameter " << v[i].ref << v[i].Name << endl;

		/*ProName ppnameid;
		ProStringToWstring(ppnameid,s2c(vid[i].Name));
		delete [] s2c(vid[i].Name);
		CreateClass(ppnameid, vid[i].ref,hauptassembly);*/
		
	}

	log << "------------------------createclassfinisch----------------------------------- " << endl;

	ProMdlnameBackup(hauptassembly, backup_dir);

	// delete the unnötige leer CAD-File


	ProError status;


	ProLine         buff;
	ProStringToWstring(buff, (char *)"*.prt,*.asm");


	ProMdlFileName pname;
	ProStringToWstring(pname, s2c(instanceName));
	//ProPath         sel_path;
	//status = ProFileMdlnameOpen(NULL, buff, NULL, NULL, backup_dir, pname,	sel_path);

	//status = ProSolidDisplay(ProMdlToSolid(hauptassembly));


	log << "import end " << endl;

	return 0;
}




// generate the aml file, inclusiv the instancehierarchy and systemunitclasslib and interfaceclasslib

void CreateAML(TreeNode* filetree, ProModelitem p_model_item)
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

	log << "-------------add interfacelib---------------------- " << endl;
	CreateInterfaceLibFromCAD(pDOMDocument, log, pmInterfaces);
	
	log << "-------------add Component in Library---------------------- " << endl;
	CreateSystemUnitLibFromCAD(pDOMDocument, pModules, pmComponents, peComponents, pmInterfaces);

	
	log << "-------------add internal element in Instancehiercharchy---------------------- " << endl;

	CreateInstanceHierarchyFromCAD(filetree, pDOMDocument, &pInstanceElement, log,
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




/// Funktion für den Export der Dateien



void SaveAsminfo(DOMDocument *pDOMDocument,
	DOMElement *bauteil)
{
	ProError error;
	DOMElement *pAttr01 = NULL;
	DOMElement *pAttr02 = NULL;
	DOMElement *pAttr03 = NULL;
	DOMElement *pAttr04 = NULL;
	DOMElement *pAttr1 = NULL;
	DOMElement *pAttr2 = NULL;
	DOMElement *pAttr3 = NULL;
	DOMElement *pAttr4 = NULL;
	DOMElement *pAttr5 = NULL;
	DOMElement *pAttr6 = NULL;
	DOMElement *pAttr7 = NULL;
	DOMElement *pAttr8 = NULL;
	DOMElement *pAttr9 = NULL;
	DOMElement *pAttr10 = NULL;
	DOMText* pTextNode1 = NULL;
	DOMText* pTextNode2 = NULL;
	DOMText* pTextNode3 = NULL;
	DOMText* pTextNode4 = NULL;
	DOMText* pTextNode5 = NULL;
	DOMText* pTextNode6 = NULL;
	DOMText* pTextNode7 = NULL;
	DOMText* pTextNode8 = NULL;
	DOMText* pTextNode9 = NULL;
	DOMText* pTextNode10 = NULL;
	DOMElement* pValue1 = NULL;
	DOMElement* pValue2 = NULL;
	DOMElement* pValue3 = NULL;
	DOMElement* pValue4 = NULL;
	DOMElement* pValue5 = NULL;
	DOMElement* pValue6 = NULL;
	DOMElement* pValue7 = NULL;
	DOMElement* pValue8 = NULL;
	DOMElement* pValue9 = NULL;
	DOMElement* pValue10 = NULL;

	std::ofstream log("logMain.txt", std::ofstream::app);


	ProMdl model;



	error = ProMdlCurrentGet(&model);


	int       *p_feat_id_array;
	ProFeatStatus *p_status_array = NULL;
	int            p_features;

	error = ProArrayAlloc(0, sizeof(int), 1, (ProArray *)&p_feat_id_array);

	error = ProSolidFeatstatusGet(ProMdlToSolid(model), &p_feat_id_array, &p_status_array, &p_features);

	/* Get a handle to the feature */

	ProFeature fff;
	error = ProFeatureInit(ProMdlToSolid(model), 40, &fff);

	ProAsmcompconstraint *pp_constraints = NULL;
	ProAsmcompconstraint constraint;
	error = ProAsmcompConstraintsGet(&fff, &pp_constraints);
	int   p_size, i;
	error = ProArraySizeGet(pp_constraints, &p_size);

	for (i = 0; i<p_size; i++)
	{
		constraint = *(pp_constraints + i);

		ProSelection comp_ref;
		ProDatumside comp_orient;
		ProSelection asm_ref;
		ProDatumside asm_orient;
		error = ProAsmcompconstraintCompreferenceGet(constraint,
			&comp_ref,
			&comp_orient);

		error = ProAsmcompconstraintAsmreferenceGet(constraint,
			&asm_ref,
			&asm_orient);

		ProAsmcompConstrType type;

		error = ProAsmcompconstraintTypeGet(constraint, &type);

		int value;
		ProAsmcompconstraintAttributesGet(constraint, &value);

		ProModelitem comp_mdl_item;

		ProSelectionModelitemGet(comp_ref, &comp_mdl_item);

		ProModelitem asm_mdl_item;

		ProSelectionModelitemGet(asm_ref, &asm_mdl_item);
		ProAsmcomppath p_cmp_path;
		ProSelectionAsmcomppathGet(asm_ref, &p_cmp_path);


		pAttr01 = pDOMDocument->createElement(L"Attribute");
		pAttr01->setAttribute(L"Name", L"asmdatum" + i);


		pAttr02 = pDOMDocument->createElement(L"Attribute");
		pAttr02->setAttribute(L"Name", L"compdatum" + i);


		pAttr03 = pDOMDocument->createElement(L"Attribute");
		pAttr03->setAttribute(L"Name", L"constraint" + i);


		pAttr1 = pDOMDocument->createElement(L"Attribute");
		pAttr1->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr1->setAttribute(L"Name", L"id");



		pValue1 = pDOMDocument->createElement(L"Value");
		pTextNode1 = pDOMDocument->createTextNode(d2wc(asm_mdl_item.id)); //Text Node erstellen

		pAttr2 = pDOMDocument->createElement(L"Attribute");
		pAttr2->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr2->setAttribute(L"Name", L"type");

		pValue2 = pDOMDocument->createElement(L"Value");
		pTextNode2 = pDOMDocument->createTextNode(d2wc(asm_mdl_item.type));

		pAttr3 = pDOMDocument->createElement(L"Attribute");
		pAttr3->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr3->setAttribute(L"Name", L"orient");

		pValue3 = pDOMDocument->createElement(L"Value");
		pTextNode3 = pDOMDocument->createTextNode(d2wc(asm_orient));

		pAttr4 = pDOMDocument->createElement(L"Attribute");
		pAttr4->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr4->setAttribute(L"Name", L"id");

		pValue4 = pDOMDocument->createElement(L"Value");
		pTextNode4 = pDOMDocument->createTextNode(d2wc(comp_mdl_item.id));

		pAttr5 = pDOMDocument->createElement(L"Attribute");
		pAttr5->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr5->setAttribute(L"Name", L"type");

		pValue5 = pDOMDocument->createElement(L"Value");
		pTextNode5 = pDOMDocument->createTextNode(d2wc(comp_mdl_item.type));

		pAttr6 = pDOMDocument->createElement(L"Attribute");
		pAttr6->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr6->setAttribute(L"Name", L"orient");

		pValue6 = pDOMDocument->createElement(L"Value");
		pTextNode6 = pDOMDocument->createTextNode(d2wc(comp_orient));

		pAttr7 = pDOMDocument->createElement(L"Attribute");
		pAttr7->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr7->setAttribute(L"Name", L"type");

		pValue7 = pDOMDocument->createElement(L"Value");
		pTextNode7 = pDOMDocument->createTextNode(d2wc(type));


		pAttr8 = pDOMDocument->createElement(L"Attribute");
		pAttr8->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr8->setAttribute(L"Name", L"value");

		pValue8 = pDOMDocument->createElement(L"Value");
		pTextNode8 = pDOMDocument->createTextNode(d2wc(value));

		log << "deep" << endl;

		pAttr01->appendChild(pAttr1);
		pAttr01->appendChild(pAttr2);
		pAttr01->appendChild(pAttr3);
		pAttr02->appendChild(pAttr4);
		pAttr02->appendChild(pAttr5);
		pAttr02->appendChild(pAttr6);
		pAttr03->appendChild(pAttr7);
		pAttr03->appendChild(pAttr8);


		pAttr1->appendChild(pValue1);
		pAttr2->appendChild(pValue2);
		pAttr3->appendChild(pValue3);
		pAttr4->appendChild(pValue4);
		pAttr5->appendChild(pValue5);
		pAttr6->appendChild(pValue6);
		pAttr7->appendChild(pValue7);
		pAttr8->appendChild(pValue8);


		pValue1->appendChild(pTextNode1);
		pValue2->appendChild(pTextNode2);
		pValue3->appendChild(pTextNode3);
		pValue4->appendChild(pTextNode4);
		pValue5->appendChild(pTextNode5);
		pValue6->appendChild(pTextNode6);
		pValue7->appendChild(pTextNode7);
		pValue8->appendChild(pTextNode8);


		bauteil->appendChild(pAttr01);
		bauteil->appendChild(pAttr02);
		bauteil->appendChild(pAttr03);

		pAttr04 = pDOMDocument->createElement(L"Attribute");
		pAttr04->setAttribute(L"Name", L"compath" + i);



		int ff;
		for (ff = 0; ff<p_cmp_path.table_num + 1; ff++)
		{
			pAttr9 = pDOMDocument->createElement(L"Attribute");
			pAttr9->setAttribute(L"AttributeDataType", L"xs:double");
			pAttr9->setAttribute(L"Name", L"identifier table" + ff);
			pValue9 = pDOMDocument->createElement(L"Value");
			pTextNode9 = pDOMDocument->createTextNode(d2wc(p_cmp_path.comp_id_table[ff]));
			pAttr9->appendChild(pValue9);
			pValue9->appendChild(pTextNode9);
		}


		pAttr10 = pDOMDocument->createElement(L"Attribute");
		pAttr10->setAttribute(L"AttributeDataType", L"xs:double");
		pAttr10->setAttribute(L"Name", L"table size");


		pValue10 = pDOMDocument->createElement(L"Value");
		pTextNode10 = pDOMDocument->createTextNode(d2wc(p_cmp_path.table_num));
		pAttr04->appendChild(pAttr9);
		pAttr04->appendChild(pAttr10);
		pAttr10->appendChild(pValue10);
		pValue10->appendChild(pTextNode10);
		bauteil->appendChild(pAttr04);

	}

}






uiCmdCmdActFn Parameter_expor(uiCmdCmdId  command, uiCmdValue *p_value, void *p_push_command_data)
{
	
	ProMdl comp;
	std::ofstream log("importlink.log", std::ofstream::app);
	ProName prtname;
	ProError err;
	ProError status;
	ProError error;
		
	ProMatrix matrix = { { 1.0, 0.0, 0.0, 0.0 },     //  Initialposition
	{ 0.0, 1.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 0.0, 1.0 } };

		
	ProMdl model;
		
	ProSolid solid;
	ProMdlCurrentGet(&model);
	
	ProFeature interface1;
	
	
	string aname = "PRT0002";

	ProStringToWstring(prtname, s2c(aname));
	delete[] s2c(aname);
	PRO_MDLFILE_ASSEMBLY;
	PRO_MDLFILE_PART;
	err=ProMdlnameRetrieve(prtname, PRO_MDLFILE_PART, &comp);
	

	log << "----------------------start--------------------------------------" <<err<< endl;

	ProName cprtname;
	ProMdl asmo;
	string cname = "PRT0001";
	ProStringToWstring(cprtname, s2c(cname));
	delete[] s2c(cname);
	PRO_MDLFILE_ASSEMBLY;
	PRO_MDLFILE_PART;
	ProMdlnameRetrieve(cprtname, PRO_MDLFILE_PART, &asmo);
	err=ProFeatureInit(ProMdlToSolid(asmo), 92, &interface1);
	
	ProFeature hauptfeat1;
	err = ProFeatureInit(ProMdlToSolid(model), 97, &hauptfeat1);

	ProFeature prt1inf2;
	err = ProFeatureInit(ProMdlToSolid(asmo), 103, &prt1inf2);
	
	ProFeature platz;
	ProFeatureInit(ProMdlToSolid(model), 91, &platz);
				
	ProSelection csele;
	status = ProSelectionAlloc(NULL, &prt1inf2, &csele);
	err = ProSelectionSet(csele, NULL, &prt1inf2);
	log << "proselectionset " << err << endl;
	
	
	ProElement elemtree;
	err = ProFeatureElemtreeExtract(&platz, NULL, PRO_FEAT_EXTRACT_NO_OPTS,
		&elemtree);
	log << "are " << endl;
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
	log << "chain " << endl;
	
	ProFeature interface2;
	err = ProFeatureInit(ProMdlToSolid(comp), 168, &interface2);
	log << err << "interface2find" << endl;
	ProAsmcomppath comp_path1;
	ProIdTable c_id_table;
	int id;
	ProMdlIdGet(interface2.owner, &id);
	ProName dddd;
	ProMdlNameGet(interface2.owner, dddd);
	log << "modelname is" << w2string(dddd) << endl;


	id = mdl2feat(ProMdlToSolid(model),id);
	c_id_table[0] = id;
	log << "id is " << id << endl;




	status = ProAsmcomppathInit(ProMdlToSolid(model), c_id_table, 1, &comp_path1);
	
	if(status!=0)
	{
		log << "ProAsmcomppathInit" << endl;
	}
	ProSelection csele2;
	status = ProSelectionAlloc(&comp_path1, &interface2, &csele2);
	


	if (status != 0)
	{
		log << "ProSelectionAlloc" <<status<< endl;
	}

	ProReference REPDEP_ref2;

	err = ProSelectionToReference(csele2, &REPDEP_ref2);
	ProReference refe;
	ProReferenceAlloc(&refe);

	err=ProReferenceSet(refe, NULL, &interface2);
	if(err!=0)
	{
		log << "Proreferencesetfail:" << err << endl;
	}
	ProElement *chai1;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai1);

	err = ProElementArrayGet(*(chai + 4), NULL, &chai1);

	

	int y = 0;
	status = ProElementIntegerGet(*(chai1), NULL, &y);

	log << status << "ProElementIntegerGet" << y << endl;
	status = ProElementIntegerSet(*(chai1), 2);
	log << "ProElementIntegerSet" << status << endl;



	ProReference REPDEP_ref1;
	
	log << "tight " << endl;
	err = ProSelectionToReference(csele, &REPDEP_ref1);
	log << err << endl;
	err = ProElementReferenceSet(*(chai1 + 1), REPDEP_ref1);
	log << err << endl;

	ProReferenceFree(REPDEP_ref1);
			

	ProElement *chai2;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai2);        //*(chai1+2) PROE COMP PLACE INTERFACE ASSEMS
	err = ProElementArrayGet(*(chai1 + 2), NULL, &chai2);
	log << err << endl;
	
	
	ProElement *chai3;

	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai3);       //*(chai2) PROE COMP PLACE INTERFACE ASSEM
	

	err = ProElementArrayGet(*(chai2), NULL, &chai3);

	ProReference data;
	err = ProElementReferenceGet(*(chai3), NULL, &data);

	log << "ProElementReferenceGet" << err << endl;
	
	if (err != 0)
	{
		ProElement ASSEM;

		status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM, &ASSEM);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM
		status = ProElemtreeElementAdd(*(chai1 + 2), NULL, ASSEM);
		log << "ProElemtreeElementAdd:" << status << endl;


		ProElementArrayCount(*(chai1+2), NULL, &p_array_count1);

		log << "p_array_count1" << p_array_count1 << endl;
					

		ProElement asmref;

		status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM_REF, &asmref);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM

		
		status = ProElemtreeElementAdd(ASSEM, NULL, asmref);
		log << "ProElemtreeElementAdd:" << status << endl;


		err = ProElementReferenceSet(asmref, REPDEP_ref2);
		log << err << "ProElementReferenceSet" << endl;

		
	
		
	}

	
	else
	{
		ProSelection selection;

		ProReferenceToSelection(data, &selection);
		ProReferenceFree(data);
		ProModelitem item;
		ProSelectionModelitemGet(selection, &item);
		ProAsmcomppath   ppath;
		ProSelectionAsmcomppathGet(selection,&ppath);
		log<<"table_num"<<ppath.table_num<<endl;

		log << "table_num" << ppath.comp_id_table[0]   << endl;

		ProName itemname;
		ProModelitemNameGet(&item, itemname);

		log << "name:" << w2string(itemname) << endl;


	
	err = ProElementReferenceSet(*(chai3), REPDEP_ref2);
	log << err << "ProElementReferenceSet" << endl;
	
	err = ProElementArraySet(*(chai2), NULL, chai3);

	log << err << "ProElementArraySet" << endl;
	err = ProElementArraySet(*(chai1+2), NULL, chai2);
	log << err << "ProElementArraySet" << endl;
	}




	ProReferenceFree(REPDEP_ref2);
	
	ProFeatureCreateOptions *opts = 0;
	status = ProArrayAlloc(1, sizeof(ProFeatureCreateOptions),
		1, (ProArray*)&opts);

	opts[0] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;
	
	//opts[0] = PRO_FEAT_CR_NO_OPTS;
	ProErrorlist   errors;
		
	ProAsmcomppath   comp_path;

	ProSelection model_sel;
	status = ProSelectionAlloc(NULL, &platz, &model_sel);
	status = ProSelectionAsmcomppathGet(model_sel, &comp_path);

	err = ProFeatureWithoptionsRedefine(&comp_path, &platz, elemtree, opts, PRO_REGEN_NO_FLAGS, &errors);
	log << "ProFeatureWithoptionsRedefine" << err << endl;



	ProElementArrayCount(*(chai1 + 2), NULL, &p_array_count1);

	log << "p_array_count1" << p_array_count1 << endl;









	/*




	ProReference data;
	log << err << endl;


	ProElement ASSEM;

	status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM, &ASSEM);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM


	status = ProElemtreeElementAdd(*(chai1+2), NULL, ASSEM);
	log << "ProElemtreeElementAdd:" << status << endl;


	ProElementArrayCount(*(chai1 + 2), NULL, &p_array_count1);       

	log << "p_array_count1" << p_array_count1 << endl;

		
	
	ProElement asmref;

	status = ProElementAlloc(PRO_E_COMP_PLACE_INTERFACE_ASSEM_REF, &asmref);    //PRO_E_COMP_PLACE_INTERFACE_ASSEM


	err = ProElementReferenceSet(asmref, REPDEP_ref2);
	log << err << "ProElementReferenceSet" << endl;

	ProReferenceFree(REPDEP_ref2);
	status = ProElemtreeElementAdd(ASSEM, NULL, asmref);
	log << "ProElemtreeElementAdd:" << status << endl;

		

	//err = ProArrayFree((ProArray*)&chai2);
		
	
	ProElement *chai3;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai3);       //*(chai2) PROE COMP PLACE INTERFACE ASSEM
	err = ProElementArrayGet(ASSEM, NULL, &chai3);


	log << err << "ProElementArrayGet" << endl;

	ProElementArrayCount(ASSEM, NULL, &p_array_count1);

	log << "p_array_count1" << p_array_count1 << endl;

	err = ProElementReferenceGet(*(chai3), NULL, &data);

	log << "ProElementReferenceGet" << err << endl;
	if(err==0)
	{
	ProSelection selection;

	ProReferenceToSelection(data, &selection);
	ProReferenceFree(data);
	ProModelitem item;
	ProSelectionModelitemGet(selection, &item);

	ProName itemname;
	ProModelitemNameGet(&item, itemname);

	log << "name:" << w2string(itemname) << endl;


	}

	
	
	//status = ProArrayObjectAdd((ProArray*)&chai2, -1, 1, &chai3);
	log << status << "ProArrayObjectAdd" << endl;


	ProSelection model_sel;
	status = ProSelectionAlloc(NULL, &platz, &model_sel);
	err = ProSelectionSet(model_sel, NULL, &platz);
	ProFeatureCreateOptions *opts = 0;

	status = ProArrayAlloc(1, sizeof(ProFeatureCreateOptions),
		1, (ProArray*)&opts);
	log << "door" << endl;
	opts[0] = PRO_FEAT_CR_FIX_MODEL_ON_FAIL;
	//opts[0] = PRO_FEAT_CR_NO_OPTS;
	ProErrorlist   errors;
	ProAsmcomppath   comp_path;
	status = ProSelectionAsmcomppathGet(model_sel, &comp_path);

	err = ProFeatureWithoptionsRedefine(NULL, &platz, elemtree, opts, PRO_REGEN_NO_FLAGS, &errors);
	log << "ProFeatureWithoptionsRedefine" << err << endl;







	
	




	//status = ProSelectionFree(&comsele);


	



	













	*/
	/*

	

	ProReference REPDEP_ref1;
	log << "the "<< endl;


	
	//status = ProSelectionFree(&comsele);

	

	

	err = ProElementValueGet(*(chai3), &comvalue);                 //*(chai3) PROE COMP PLACE INTERFACE ASSEM REF
	err = ProValueDataGet(comvalue, &comvalue_data);

	ProSelection comsele;
	comsele = comvalue_data.v.r;
	ProModelitem comitem;
	ProSelectionModelitemGet(comsele, &comitem);
	ProName compname;
	ProModelitemNameGet(&comitem, compname);
	string refB;
	refB = w2string(compname);
	log << refB  << endl;




	
	


	
	log << err << "redefiniation" << endl;

	

	
	

	log << comvalue_data.v.w << endl;


	ProSelection csele;
	status = ProSelectionAlloc(NULL, &interface1, &csele);
	err=ProSelectionSet(csele,NULL,&interface1);
	log << err << endl;

	
	
	ProElement *chai2;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai2);        //*(chai1+2) PROE COMP PLACE INTERFACE ASSEMS
	err = ProElementArrayGet(*(chai1 + 2), NULL, &chai2);
	log << err << endl;

	ProElement *chai3;
	err = ProArrayAlloc(0, sizeof(ProElement), 1, (ProArray*)&chai3);       //*(chai2) PROE COMP PLACE INTERFACE ASSEM



																			

		
		*/
	

	return 0;
}







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

	log << "-------bibsyn start-----------" << std::endl;

	ComponentLibraryModification(pmodel, typeHash);
	
	error = ProMdlToModelitem(pmodel, &pmodelitem);
	ProMdlName test_name;
	error = ProMdlMdlnameGet(pmodel, test_name);

	TreeNode *mdltree = new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]);
	//error = ProSessionMdlList(promdltype, &p_model_array, &p_model_num);
	mdltree->appendChild(new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]));

	TreeNode *childtree = mdltree->getChild(0);
	log << "---------------anfangaddtree-----------" << std::endl;
	//generate model tree
	AddTrees(&childtree, pmodel, typeHash);
	//mdltree->appendChild(new TreeNode(w2string(test_name), typeHash[ProMdlfileType::PRO_MDLFILE_ASSEMBLY]));

	log << "-------endaddtree------------" << std::endl;
		
	log << "-------creataml-----------" << std::endl;
	// generate and write AML file to local disk
	CreateAML(mdltree, pmodelitem);


	return 0;

}





