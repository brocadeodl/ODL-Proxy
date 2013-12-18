/* Copyright (c) 2005-2013 Brocade Communications Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <ctype.h>

using namespace std;
using namespace XERCES_CPP_NAMESPACE;

void parseComponents       (const string &waveComponentsFileName, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues);
void parseComponentModules (const string &waveComponentName, const string &waveComponentSourceFileName, const unsigned long int &waveComponentResourceStart, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues);
void parseModuleResources  (const string &waveModuleName, const string &waveModuleSourceFileName, const unsigned long int &waveModuleResourceStart, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues);
void parseResourceEnum     (unsigned long int &waveResourceStart, DOMElement *pRootNode, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues);
void processResourceIds    (map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues, const string &outputDirectoryName, const string &waveComponentName);

int main (int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage:" << endl << endl;
        cout << argv[0] << " <Prim Components File Name> <Output Directory> <Component Name>" << endl << endl;
        return (1);
    }

    map<unsigned long int, string> waveResourceIdNames;
    map<unsigned long int, string> waveResourceIdValues;
    map<unsigned long int, string> waveResourceEnumNames;
    map<unsigned long int, vector<string> > waveResourceEnumValues;

    XMLPlatformUtils::Initialize ();

    parseComponents (argv[1], waveResourceIdNames, waveResourceIdValues, waveResourceEnumNames, waveResourceEnumValues);

    processResourceIds (waveResourceIdNames, waveResourceIdValues, waveResourceEnumNames, waveResourceEnumValues, argv[2], argv[3]);

    XMLPlatformUtils::Terminate ();

    return (0);
}

void parseComponents (const string &waveComponentsFileName, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues)
{
    XercesDOMParser    waveComponentsParser;
    XMLCh             *pPrismComponentsInputFileName = XMLString::transcode (waveComponentsFileName.c_str ());
    XMLCh             *pPrismComponentName           = XMLString::transcode ("Name");
    XMLCh             *pPrismComponentSource         = XMLString::transcode ("Source");
    char              *pPrismComponentNameAscii      = NULL;
    char              *pPrismComponentSourceAscii    = NULL;
    unsigned long int  waveComponentResourceStart   = 0x00000000;

    waveComponentsParser.setValidationScheme (XercesDOMParser::Val_Auto);
    waveComponentsParser.setDoNamespaces (false);
    waveComponentsParser.setDoSchema (false);
    waveComponentsParser.setCreateEntityReferenceNodes (false);

    LocalFileInputSource componentsInputSource (pPrismComponentsInputFileName);

    try
    {
        cout << "Parsing the Components ... ";

        waveComponentsParser.parse (componentsInputSource);

        cout << "Done." << endl;

        DOMDocument *pDocument  = waveComponentsParser.getDocument ();

        if (NULL == pDocument)
        {
            cout << "\033[31mNo Components found.\033[0m" << endl;
        }
        else
        {
            DOMElement  *pRootNode  = (pDocument->getDocumentElement ());
            DOMNode     *pChildNode = NULL;

            if (NULL == pRootNode)
            {
                cout << "\033[31mNo Components found.\033[0m" << endl;
            }
            else
            {
                for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
                {
                    if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
                    {
                        DOMElement *pDomElement = reinterpret_cast<DOMElement *> (pChildNode);

                        pPrismComponentNameAscii   = XMLString::transcode (pDomElement->getAttribute (pPrismComponentName));
                        pPrismComponentSourceAscii = XMLString::transcode (pDomElement->getAttribute (pPrismComponentSource));

                        cout << "    Component " << pPrismComponentNameAscii << " (" << pPrismComponentSourceAscii << ")" << endl;

                        parseComponentModules (pPrismComponentNameAscii, pPrismComponentSourceAscii, waveComponentResourceStart, waveResourceIdNames, waveResourceIdValues, waveResourceEnumNames, waveResourceEnumValues);

                        XMLString::release (&pPrismComponentNameAscii);
                        XMLString::release (&pPrismComponentSourceAscii);

                        waveComponentResourceStart += 0x10000000;
                    }
                }
            }
        }
    }
    catch (const XMLException &waveComponentParserException)
    {
        cout << "\033[31mParsing Componets Failed. \033[0m" << endl;
    }

    XMLString::release (&pPrismComponentsInputFileName);
    XMLString::release (&pPrismComponentName);
    XMLString::release (&pPrismComponentSource);
}

void parseComponentModules (const string &waveComponentName, const string &waveComponentSourceFileName, const unsigned long int &waveComponentResourceStart, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues)
{
    XercesDOMParser    waveModulesParser;
    XMLCh             *pPrismModulesInputFileName = XMLString::transcode (waveComponentSourceFileName.c_str ());
    XMLCh             *pPrismModuleName           = XMLString::transcode ("Name");;
    XMLCh             *pPrismModuleSource         = XMLString::transcode ("Source");
    char              *pPrismModuleNameAscii      = NULL;
    char              *pPrismModuleSourceAscii    = NULL;
    unsigned long int  waveModuleResourceStart   = waveComponentResourceStart;

    waveModulesParser.setValidationScheme (XercesDOMParser::Val_Auto);
    waveModulesParser.setDoNamespaces (false);
    waveModulesParser.setDoSchema (false);
    waveModulesParser.setCreateEntityReferenceNodes (false);

    LocalFileInputSource modulesInputSource (pPrismModulesInputFileName);

    try
    {
        cout << "        Parsing the Modules ... ";

        waveModulesParser.parse (modulesInputSource);

        cout << "Done." << endl;

        DOMDocument *pDocument  = waveModulesParser.getDocument ();

        if (NULL == pDocument)
        {
            cout << "\033[31m            No Modules found.\033[0m" << endl;
        }
        else
        {
            DOMElement  *pRootNode  = (pDocument->getDocumentElement ());
            DOMNode     *pChildNode = NULL;

            if (NULL == pRootNode)
            {
                cout << "\033[31m            No Modules found.\033[0m" << endl;
            }
            else
            {
                for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
                {
                    if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
                    {
                        DOMElement *pDomElement = reinterpret_cast<DOMElement *> (pChildNode);

                        pPrismModuleNameAscii   = XMLString::transcode (pDomElement->getAttribute (pPrismModuleName));
                        pPrismModuleSourceAscii = XMLString::transcode (pDomElement->getAttribute (pPrismModuleSource));

                        cout << "            Module " << pPrismModuleNameAscii << " (" << pPrismModuleSourceAscii << ")" << endl;

                        parseModuleResources (pPrismModuleNameAscii, pPrismModuleSourceAscii, waveModuleResourceStart, waveResourceIdNames, waveResourceIdValues, waveResourceEnumNames, waveResourceEnumValues);

                        XMLString::release (&pPrismModuleNameAscii);
                        XMLString::release (&pPrismModuleSourceAscii);

                        waveModuleResourceStart += 0x00010000;
                    }
                }
            }
        }
    }
    catch (const XMLException &waveComponentModulesParserException)
    {
        cout << "\033[31mParsing Modules Failed. \033[0m" << endl;
    }

    XMLString::release (&pPrismModulesInputFileName);
    XMLString::release (&pPrismModuleName);
    XMLString::release (&pPrismModuleSource);
}

void parseModuleResources  (const string &waveModuleName, const string &waveModuleSourceFileName, const unsigned long int &waveModuleResourceStart, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues)
{
    XercesDOMParser    waveResourcesParser;
    XMLCh             *pPrismResourcesInputFileName = XMLString::transcode (waveModuleSourceFileName.c_str ());
    char              *pModuleTagNameAscii          = NULL;
    char              *pNodeTagNameAscii            = NULL;
    XMLCh             *pPrismResourceName           = XMLString::transcode ("Name");;
    char              *pPrismResourceNameAscii      = NULL;
    char              *pPrismResourceValueAscii     = NULL;
    unsigned long int  waveResourceStart           = waveModuleResourceStart + 1;

    waveResourcesParser.setValidationScheme (XercesDOMParser::Val_Auto);
    waveResourcesParser.setDoNamespaces (false);
    waveResourcesParser.setDoSchema (false);
    waveResourcesParser.setCreateEntityReferenceNodes (false);

    LocalFileInputSource resourcesInputSource (pPrismResourcesInputFileName);

    try
    {
        cout << "                Parsing the Resources ... ";

        waveResourcesParser.parse (resourcesInputSource);

        cout << "Done." << endl;

        DOMDocument *pDocument  = waveResourcesParser.getDocument ();

        if (NULL == pDocument)
        {
            cout << "\033[31m                    No Resources found.\033[0m" << endl;
        }
        else
        {
            DOMElement  *pRootNode  = (pDocument->getDocumentElement ());
            DOMNode     *pChildNode = NULL;

            const XMLCh *pModuleTagName = pRootNode->getNodeName ();

            if (NULL != pModuleTagName)
            {
                pModuleTagNameAscii = XMLString::transcode (pModuleTagName);

                if (0 != (strcmp (pModuleTagNameAscii, "ModuleResources")))
                {
                    cout << "\033[31m                Skipping " << pModuleTagNameAscii << "\033[0m" << endl;
                    return;
                }

                XMLString::release (&pModuleTagNameAscii);
            }
            else
            {
                return;
            }

            for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
            {
                if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
                {
                    DOMElement *pDomElement = reinterpret_cast<DOMElement *> (pChildNode);

                    const XMLCh *pNodeTagName = pDomElement->getNodeName ();

                    if (NULL != pNodeTagName)
                    {
                        pNodeTagNameAscii = XMLString::transcode (pNodeTagName);
                    }
                    else
                    {
                        continue;
                    }

                    if (NULL == pNodeTagNameAscii)
                    {
                        continue;
                    }

                    if (0 == (strcmp (pNodeTagNameAscii, "Resource")))
                    {
                        XMLString::release (&pNodeTagNameAscii);

                        pPrismResourceNameAscii  = XMLString::transcode (pDomElement->getAttribute (pPrismResourceName));

                        if (NULL != pPrismResourceNameAscii)
                        {
                            waveResourceIdNames[waveResourceStart] = pPrismResourceNameAscii;

                            DOMNode *pTempNode = pChildNode->getFirstChild ();

                            if (NULL != pTempNode)
                            {
                                pPrismResourceValueAscii = XMLString::transcode (pTempNode->getNodeValue ());
                            }
                            else
                            {
                                pPrismResourceValueAscii = NULL;
                            }

                            if (NULL != pPrismResourceValueAscii)
                            {
                                waveResourceIdValues[waveResourceStart] = pPrismResourceValueAscii;
                            }
                            else
                            {
                                waveResourceIdValues[waveResourceStart] = "";
                            }
                        }

                        XMLString::release (&pPrismResourceNameAscii);
                        XMLString::release (&pPrismResourceValueAscii);

                        waveResourceStart += 0x00000001;
                    }
                    else if (0 == (strcmp (pNodeTagNameAscii, "ResourceEnum")))
                    {
                        XMLString::release (&pNodeTagNameAscii);

                        parseResourceEnum (waveResourceStart, pDomElement, waveResourceIdNames, waveResourceIdValues, waveResourceEnumNames, waveResourceEnumValues);
                    }
                    else
                    {
                        cout << "\033[31m                    Skipping " << pNodeTagNameAscii << "\033[0m" << endl;
                        continue;
                    }
                }
            }
        }
    }
    catch (const XMLException &waveModuleResourcesParserException)
    {
        cout << "\033[31mParsing Resources Failed. \033[0m" << endl;
    }

    XMLString::release (&pPrismResourcesInputFileName);
    XMLString::release (&pPrismResourceName);
}

void parseResourceEnum (unsigned long int &waveResourceStart, DOMElement *pRootNode, map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues)
{
    char              *pNodeTagNameAscii        = NULL;
    XMLCh             *pPrismResourceName       = XMLString::transcode ("Name");;
    char              *pPrismResourceNameAscii  = NULL;
    char              *pPrismResourceValueAscii = NULL;
    DOMNode           *pChildNode               = NULL;
    unsigned long int  waveResourceEnumValue   = waveResourceStart;

    pPrismResourceNameAscii = XMLString::transcode (pRootNode->getAttribute (pPrismResourceName));

    if (NULL != pPrismResourceNameAscii)
    {
        waveResourceEnumNames[waveResourceStart] = pPrismResourceNameAscii;
        waveResourceStart += 0x00000001;
    }
    else
    {
        cout << "\033[31m                    Skipping NULL ResourceEnum Entry." << "\033[0m" << endl;
        return;
    }

    for (pChildNode = pRootNode->getFirstChild (); NULL != pChildNode; pChildNode = pChildNode->getNextSibling ())
    {
        if (DOMNode::ELEMENT_NODE == (pChildNode->getNodeType ()))
        {
            DOMElement *pDomElement = reinterpret_cast<DOMElement *> (pChildNode);

            const XMLCh *pNodeTagName = pDomElement->getNodeName ();

            if (NULL != pNodeTagName)
            {
                pNodeTagNameAscii = XMLString::transcode (pNodeTagName);
            }
            else
            {
                continue;
            }

            if (NULL == pNodeTagNameAscii)
            {
                continue;
            }

            if (0 == (strcmp (pNodeTagNameAscii, "Resource")))
            {
                XMLString::release (&pNodeTagNameAscii);

                pPrismResourceNameAscii = XMLString::transcode (pDomElement->getAttribute (pPrismResourceName));

                if (NULL != pPrismResourceNameAscii)
                {
                    waveResourceIdNames[waveResourceStart] = pPrismResourceNameAscii;
                    waveResourceEnumValues[waveResourceEnumValue].push_back (pPrismResourceNameAscii);

                    DOMNode *pTempNode = pChildNode->getFirstChild ();

                    if (NULL != pTempNode)
                    {
                        pPrismResourceValueAscii = XMLString::transcode (pTempNode->getNodeValue ());
                    }
                    else
                    {
                        pPrismResourceValueAscii = NULL;
                    }

                    if (NULL != pPrismResourceValueAscii)
                    {
                        waveResourceIdValues[waveResourceStart] = pPrismResourceValueAscii;
                    }
                    else
                    {
                        waveResourceIdValues[waveResourceStart] = "";
                    }

                    waveResourceStart += 0x00000001;
                }

                XMLString::release (&pPrismResourceNameAscii);
                XMLString::release (&pPrismResourceValueAscii);
            }
            else
            {
                cout << "\033[31m                    Skipping " << pNodeTagNameAscii << "\033[0m" << endl;
                continue;
            }
        }
    }

    XMLString::release (&pPrismResourceName);
    XMLString::release (&pPrismResourceNameAscii);
}

void processResourceIds (map<unsigned long int, string> &waveResourceIdNames, map<unsigned long int, string> &waveResourceIdValues, map<unsigned long int, string> &waveResourceEnumNames, map<unsigned long int, vector<string> > &waveResourceEnumValues, const string &outputDirectoryName, const string &waveComponentName)
{
    map<unsigned long int, string>::iterator element                                            = waveResourceIdNames.begin ();
    map<unsigned long int, string>::iterator end                                                = waveResourceIdNames.end ();
    map<unsigned long int, string>::iterator elementEnums                                       = waveResourceEnumNames.begin ();
    map<unsigned long int, string>::iterator endEnums                                           = waveResourceEnumNames.end ();
    unsigned long int                        maximumResourceIdNameSize                          = 16;
    const int                                buff_size = 256;
    char                                     formatString[buff_size];
    char                                     formatStringWithoutTrailingComma[buff_size];
    char                                     enumsFormatString[buff_size];
    char                                     enumsFormatStringWithoutTrailingComma[buff_size];
    char                                     enumsTypedefFormatString[buff_size];
    char                                     enumsTypedefFormatStringWithoutTrailingComma[buff_size];
    const int                                buff_size1 = 2048;
    char                                     formattedOutput[buff_size1];
	unsigned long int                        numberOfPrismResources                             = 0;
	unsigned long int                        numberOfPrismResourceEnums                         = 0;
    string                                   waveComponentNameInUpperCase                      = waveComponentName;
    int                                      waveComponentNameSize                             = waveComponentName.size ();
    int                                      i                                                  = 0;
	
	

    // First convert the Prism Component Name into upper case and store it.

    for (i = 0; i < waveComponentNameSize; i++)
    {
        waveComponentNameInUpperCase[i] = toupper (waveComponentName[i]);
    }

    while (end != element)
    {
        if (((element->second).length ()) > maximumResourceIdNameSize)
        {
            maximumResourceIdNameSize = (element->second).length ();
        }

        element++;
		numberOfPrismResources++;
    }

    while (endEnums != elementEnums)
    {
        if (((elementEnums->second).length ()) > maximumResourceIdNameSize)
        {
            maximumResourceIdNameSize = (elementEnums->second).length ();
        }

        elementEnums++;
		numberOfPrismResourceEnums++;
    }

    cout << endl;
	cout << "Total number of " << waveComponentName << " Resources      = " << numberOfPrismResources << endl;
	cout << "Total number of " << waveComponentName << " Resource Enums = " << numberOfPrismResourceEnums << endl;

    ofstream waveResourceIdEnumsFile           ((outputDirectoryName + "/" + waveComponentName + "ResourceIdEnums.h").c_str ());
    ofstream prismResourceIdsHeaderFile         ((outputDirectoryName + "/" + waveComponentName + "ResourceIds.h").c_str ());
    ofstream prismResourceIdsImplementationFile ((outputDirectoryName + "/" + waveComponentName + "ResourceIds.cpp").c_str ());
    ofstream prismResourceIdsCHeaderFile        ((outputDirectoryName + "/" + waveComponentName + "CStatus.h").c_str ());
	//ofstream for propertyfile for BNA consumption.
	ofstream prismResourcePropertiesFile		((outputDirectoryName + "/" + waveComponentName + "_en.properties").c_str());

    element      = waveResourceIdNames.begin ();
    elementEnums = waveResourceEnumNames.begin ();

    waveResourceIdEnumsFile << "#ifndef " << waveComponentNameInUpperCase << "RESOURCEIDENUMS_H\n";
    waveResourceIdEnumsFile << "#define " << waveComponentNameInUpperCase << "RESOURCEIDENUMS_H\n\n";
    waveResourceIdEnumsFile << "#include \"" << waveComponentName << "ResourceIds.h\"\n\n";
    waveResourceIdEnumsFile << "namespace " << waveComponentName << "Ns\n{\n\n";

    prismResourceIdsHeaderFile << "#ifndef " << waveComponentNameInUpperCase << "RESOURCEIDS_H\n";
    prismResourceIdsHeaderFile << "#define " << waveComponentNameInUpperCase << "RESOURCEIDS_H\n\n";
    prismResourceIdsHeaderFile << "#include <vector>\n";
    prismResourceIdsHeaderFile << "#include <string>\n\n";
    prismResourceIdsHeaderFile << "using namespace std;\n\n";
    prismResourceIdsHeaderFile << "namespace " << waveComponentName << "Ns\n{\n\n";
    prismResourceIdsHeaderFile << "typedef enum\n{\n";

    prismResourceIdsCHeaderFile << "#ifndef " << waveComponentNameInUpperCase << "CSTATUS_H\n";
    prismResourceIdsCHeaderFile << "#define " << waveComponentNameInUpperCase << "CSTATUS_H\n\n";
    prismResourceIdsCHeaderFile << "typedef enum\n{\n";

    prismResourceIdsImplementationFile << "#include \"Resources/" << waveComponentName << "ResourceIdscpp.prefix\"\n\n";
    prismResourceIdsImplementationFile << "namespace " << waveComponentName << "Ns\n{\n\n";
    prismResourceIdsImplementationFile << "void initialize" << waveComponentName << "ResourceIds ()\n{\n";

    snprintf (formatString, buff_size, "    WRC_%%-%lus = 0x%%08X,\n", maximumResourceIdNameSize + 5); // + 5 is to account for ENUM_ in the enums which does not exist for ids.
    snprintf (formatStringWithoutTrailingComma, buff_size, "    WRC_%%-%lus = 0x%%08X\n", maximumResourceIdNameSize + 5); // + 5 is to account for ENUM_ in the enums which does not exist for ids.
    snprintf (enumsFormatString, buff_size, "    WRC_ENUM_%%-%lus = 0x%%08X,\n", maximumResourceIdNameSize);
    snprintf (enumsFormatStringWithoutTrailingComma, buff_size, "    WRC_ENUM_%%-%lus = 0x%%08X\n", maximumResourceIdNameSize);
    snprintf (enumsTypedefFormatString, buff_size, "    %%-%lus = WRC_%%s,\n", maximumResourceIdNameSize);
    snprintf (enumsTypedefFormatStringWithoutTrailingComma, buff_size, "    %%-%lus = WRC_%%s\n", maximumResourceIdNameSize);

    unsigned long int tempNumberOfPrismResources = 1;

    while (end != element)
    {
        snprintf (formattedOutput, buff_size1, formatString, (element->second).c_str (), element->first);

        prismResourceIdsHeaderFile << formattedOutput;

        if (numberOfPrismResources != tempNumberOfPrismResources)
        {
            snprintf (formattedOutput, buff_size1, formatString, (element->second).c_str (), element->first);
        }
        else
        {
            snprintf (formattedOutput, buff_size1, formatStringWithoutTrailingComma, (element->second).c_str (), element->first);
        }

        prismResourceIdsCHeaderFile << formattedOutput;

        // Add the source code line to the initialize resource ids function to build the static map of resource id to resource name 
        snprintf (formattedOutput, buff_size1, "    addSourceCodeEnum (0x%08lX, \"%s\");\n", element->first, waveResourceIdNames[element->first].c_str ());
        prismResourceIdsImplementationFile << formattedOutput;

        // Add the source code line to the initialize resource ids function to build the static map of resource id to resource localized string value
        snprintf (formattedOutput, buff_size1, "    addResourceId (0x%08lX, \"%s\");\n", element->first, waveResourceIdValues[element->first].c_str ());
        prismResourceIdsImplementationFile << formattedOutput;

	    // FormatProperty for BNA resourceIdName = localizedMessageString
	    snprintf (formattedOutput, buff_size1,"%s = %s\n", waveResourceIdNames[element->first].c_str (), waveResourceIdValues[element->first].c_str());
	    prismResourcePropertiesFile << formattedOutput;

        element++;
		tempNumberOfPrismResources++;
    }
     

    prismResourceIdsHeaderFile << endl;

    prismResourceIdsImplementationFile << endl;

	prismResourcePropertiesFile << endl;

    unsigned long int tempNumberOfPrismResourceEnums = 1;

    while (endEnums != elementEnums)
    {
        waveResourceIdEnumsFile << "typedef enum" << endl;
        waveResourceIdEnumsFile << "{" << endl;


        if (numberOfPrismResourceEnums != tempNumberOfPrismResourceEnums)
        {
            snprintf (formattedOutput, buff_size1, enumsFormatString, (elementEnums->second).c_str (), elementEnums->first);
        }
        else
        {
            snprintf (formattedOutput, buff_size1, enumsFormatStringWithoutTrailingComma, (elementEnums->second).c_str (), elementEnums->first);
        }

        prismResourceIdsHeaderFile << formattedOutput;

        unsigned long int numberOfResourceIdsForResourceEnum = waveResourceEnumValues[elementEnums->first].size ();
        unsigned long int j                                  = 0;

        for (j = 0; j < numberOfResourceIdsForResourceEnum; j++)
        {
            if (j < (numberOfResourceIdsForResourceEnum - 1))
            {
                snprintf (formattedOutput, buff_size1, enumsTypedefFormatString, waveResourceEnumValues[elementEnums->first][j].c_str (), waveResourceEnumValues[elementEnums->first][j].c_str ());
            }
            else
            {
                snprintf (formattedOutput, buff_size1, enumsTypedefFormatStringWithoutTrailingComma, waveResourceEnumValues[elementEnums->first][j].c_str (), waveResourceEnumValues[elementEnums->first][j].c_str ());
            }

            waveResourceIdEnumsFile << formattedOutput;

            //snprintf (formattedOutput, buff_size1, "    s_waveResourceEnumsMap[0x%08lX].push_back (WRC_%s);\n", elementEnums->first, waveResourceEnumValues[elementEnums->first][j].c_str ());
            snprintf (formattedOutput, buff_size1, "    addResourceIdToResourceEnum (0x%08lX, WRC_%s);\n", elementEnums->first, waveResourceEnumValues[elementEnums->first][j].c_str ());
            prismResourceIdsImplementationFile << formattedOutput;
        }

        if (0 != numberOfResourceIdsForResourceEnum)
        {
            prismResourceIdsImplementationFile << endl;
        }

        waveResourceIdEnumsFile << "} " << (elementEnums->second).c_str () << ";" << endl;
        waveResourceIdEnumsFile << endl;

        elementEnums++;
        tempNumberOfPrismResourceEnums++;
    }

    waveResourceIdEnumsFile << "}\n\n";
    waveResourceIdEnumsFile << "#endif // " << waveComponentNameInUpperCase << "RESOURCEIDENUMS_H\n\n";

    prismResourceIdsHeaderFile << "} All" << waveComponentName << "ResourceIds;\n\n";
    prismResourceIdsHeaderFile << "#include \"Resources/" << waveComponentName << "ResourceIdsh.postfix\"\n\n";
    prismResourceIdsHeaderFile << "}\n\n";
    prismResourceIdsHeaderFile << "#endif // " << waveComponentNameInUpperCase << "RESOURCEIDS_H\n\n";

    prismResourceIdsCHeaderFile << "} " << waveComponentName << "CStatus;\n\n";
    prismResourceIdsCHeaderFile << "#endif // " << waveComponentNameInUpperCase << "CSTATUS_H\n\n";

    prismResourceIdsImplementationFile << "}\n\n";
    prismResourceIdsImplementationFile << "}\n\n";
    prismResourceIdsImplementationFile << "#include \"Resources/" << waveComponentName << "ResourceIdscpp.postfix\"\n\n";

    waveResourceIdEnumsFile.close ();
    prismResourceIdsHeaderFile.close ();
    prismResourceIdsCHeaderFile.close ();
    prismResourceIdsImplementationFile.close ();
	prismResourcePropertiesFile.close();
}
