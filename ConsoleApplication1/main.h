#ifndef MAIN_H
#define MAIN_H


/// C++ Bibliotheken
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







/// Header für DOM (Xerces) 
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






using namespace std;	

struct NAMES {   
	string Name;     
	wchar_t* x; 
	wchar_t* y; 
	wchar_t* z; 
	wchar_t* rx; 
	wchar_t* ry; 
	wchar_t* rz; 
} names;

struct Values {   
	XMLCh* Name;  
	double x;
	double y;
	double z;
	double rx;
	double ry;
	double rz;
};

// Pfad von neue CAD Datei
string ns="..\\neue";
string ab;

// Pfad von Bibliothek
string bib="E:\\cad_Li\\PPU\\Bibliothek";

// Pafd von AML Datei
string amlordner="..\\..\\files";

// inputaml
const char* xmlFile = "..\\..\\files\\IM.aml";


struct FRAME {   
		string Name; 
		string ref;
	} frame;  
	FRAME child_frame;
	std::vector<FRAME> v;
	std::vector<FRAME> vid;



struct Ref {   
		string A; 
		string B;
	} refPartner;  
Ref partner;
	std::vector<Ref> coop;


string asm_name = "ASM";
string prt_name = "PRT";



double Interface[10]={0, 0, 0, 0, 0 ,0 ,0 ,0, 0, 0 };


#endif
