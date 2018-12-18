#include "ViwiacParser.h"
#include "EffectStructure\EffectStructure.h"

#include "SystemBuildingStructure\SystemBuildingStructure.h"

#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/AbstractDOMParser.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>

/**
The following functions create dom elements and fill them with content from
the corresponding classes
the first argument is always the parent element(the root element for the child node) of all elements created in the function.
the second argument is always the object which contains the content

*/

class XMLWriter
{
public:

	DOMElement* getRootElement();
	void createDocWithRoot(wchar_t* rootElementName);
	void writeXML(wchar_t* path);

	void appendMapping(DOMElement* root, IDMap& idMap);
	void appendEffectStructure(DOMElement* root, EffectStructure& es);

	void appendFunctionalArchitecture(DOMElement* root, FunctionalArchitecture& fa);
	void appendBuildingStructure(DOMElement* rootElement, SystemBuildingStructure& sbs);

private:
	void appendElementNode(string name, DOMElement* baseElement, DOMElement* &newElement);
	void setTextContent(DOMElement* element, string content);
	void appendSubFunctions(DOMElement* basisElement, Function &oberfunction);
	void appendFunctionalChar(DOMElement* rootElement, FunctionalCharacteristic& funcChar);
	void appendComponent(DOMElement* rootElement, Component& comp);
	void appendEffectNetwork(DOMElement* rootElement, EffectStructure& es);
	void appendConnector(DOMElement* connectorElement, XMLConnector& conn);
	void appendWS(DOMElement* rootElement, XMLws& ws);
	void appendEdc(DOMElement* rootElement, Edc& edc);
	void appendConnType(DOMElement* connectorElement, ConnectorType& connType);
	void appendClassification(DOMElement* rootElement, ConnClassification& connClass);
	void appendCss(DOMElement* cssElement, XMLcss& css);
	void appendEffect(DOMElement* effectElement, EffectType& eff);
	void appendEffectTypeChar(DOMElement* cssEffectCharElement, EffectChType& effChType);
	void appendWSP(DOMElement* wspElement, WsPair& wsPair);


	DOMDocument* newDoc;
	DOMImplementation* impl;
}; #pragma once
