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
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include "PostBoot.h"

using namespace std;
using namespace xercesc;

#define INDENT1 "    "
#define INDENT2 "        "
#define GLOBAL_TABLE_BEGIN        "DCM_POSTBOOT_GLOBAL_BEGIN"
#define LINECARD_TABLE_BEGIN      "DCM_POSTBOOT_SLOT_BEGIN"
#define GLOBAL_TABLE_END          "DCM_POSTBOOT_GLOBAL_END"
#define LINECARD_TABLE_END        "DCM_POSTBOOT_SLOT_END"
#define POSTBOOT_GLOBAL_CONFIG    "POSTBOOT_GLOBAL_CONFIG"
#define POSTBOOT_LINECARD_CONFIG  "POSTBOOT_LINECARD_CONFIG"

string projectName;

int parseXml (char* xml_file, configReplayTable *pTable); 

configReplayTable::configReplayTable () 
{

}

configReplayTable::~configReplayTable () 
{

}

void serviceHeaderFileInclude (string filePath) {
	
	ofstream cpp;

	cpp.open (filePath.c_str(), std::ios::app);
    
    FILE* pipe = popen("cd .. && find . -name *ObjectManager.h", "r");
    if (!pipe) 
    {
        return;
    }

    char buffer[1024];
    
    std::string result = "";
   
    while(!feof(pipe)) {
        if(fgets(buffer, 1024, pipe) != NULL)
        {
                result = buffer;
                int k = result.size()-1;

                if ( k > 0 )
                {
                    if (result[k] != 'h')
                    {
                        result[k] = '\0';
                        result.resize (k);
                    }
                }   
                cpp << "#include \"" << result << "\"" << endl;
        }
    }

    pclose(pipe);
    cpp.close();
}


void configReplayTable::printHeader (string filePath) 
{
	ofstream cpp;

	cpp.open (filePath.c_str(), std::ios::app);
   
    cpp << "#include <string>";
    cpp << endl;
    cpp << "#include <vector>";
    cpp << endl;


    cpp.close();
}

string configReplayTable::printServicePushBack (string serviceName) 
{
    string retval = "    service"; 

    retval.append (".push_back (");
    retval.append (serviceName);
    retval.append ("::getPrismServiceId ());");
   
    return (retval);
}

void configReplayTable::printDotH (string filePath) 
{
	ofstream cpp;

	cpp.open (filePath.c_str(), std::ios::trunc);

    cpp << "#ifndef POSTBOOT_H" << endl;
    cpp << "#define POSTBOOT_H" << endl;
    cpp << endl;
    cpp.close ();
    
    printHeader(filePath);
   
	cpp.open (filePath.c_str(), std::ios::app);
    cpp << endl;
    cpp << "#include \"Framework/ObjectModel/WaveLocalObjectManager.h\"" << endl;
    cpp << "#include \"Framework/Postboot/PrismPostbootTypes.h\"" << endl;
    cpp << endl;

    cpp << "using namespace WaveNs;";
    cpp << endl;

    cpp << "namespace " << projectName << "Ns" << endl;
    cpp << "{" << endl << endl;
    cpp << "class "<< projectName << "PostBootTable" << endl;
    cpp << "{" << endl;
    cpp << INDENT1 << "private:" << endl;
    cpp << INDENT1 << "protected:" << endl;
    cpp << INDENT1 << "public:" << endl;
    cpp << INDENT2 << "static map <UI32, vector <postbootPass> > get" << projectName << "PostBootTables();" << endl;
    cpp << endl << endl;
    cpp << INDENT1 << "private:" << endl;
    cpp << INDENT1 << "protected:" << endl;
    cpp << INDENT1 << "public:" << endl;

    cpp << "};" << endl << endl << "}" ;
    cpp << endl;
    cpp << "#endif   // POSTBOOT_H " << endl;


    cpp.close ();

}


void configReplayTable::printCpp (string filePath) 
{
	ofstream cpp;

	cpp.open (filePath.c_str(), std::ios::trunc);

    cpp << "#include \"" << projectName << "PostBootProgrammingTable.h\"";
    cpp << endl;
    
    cpp.close();
    serviceHeaderFileInclude(filePath);
    
	cpp.open (filePath.c_str(), std::ios::app);
   
    cpp << endl;
    cpp << "using namespace std;" << endl << endl;
    cpp << "namespace " << projectName << "Ns" << endl;
    cpp << "{" << endl;
    cpp << "map <UI32, vector <postbootPass> > " << projectName << "PostBootTable::get" << projectName << "PostBootTables ()" << endl;
    cpp << "{" << endl << endl;
    cpp << INDENT1 << "map <UI32, vector <postbootPass> > postBootTable;" << endl;
    cpp << INDENT1 << "postbootPass pass;" << endl;
    cpp << INDENT1 << "vector <PrismServiceId> &service = pass.serviceId;" << endl;
    cpp << INDENT1 << "vector <int> &recoveryVec = pass.recoveryType;" << endl << endl;
   
    map<string, map<int, vector <pair<string, int> > > >::iterator it;

    for ( it = configReplayMap.begin(); it != configReplayMap.end(); it++ )
    {
        string tableName = (*it).first;

        int tableSize = ((*it).second).size();

        // looping from index 1 as pass# starts from 1
        for (int j=1; j<=tableSize;j++)
        { 
            // first entry is pass name in the vector
            string passName = (((*it).second)[j])[0].first;
            cpp << INDENT1 << "pass.passName = \"" << passName << "\";" << endl;
            cpp << INDENT1 << "service.clear();" << endl;
            cpp << INDENT1 << "recoveryVec.clear();" << endl;

            int passSize = (((*it).second)[j]).size();

            // looping from index 1 as service names start from 1 in the string vector
            for (int k=1;k < passSize;k++) 
            {
                cpp << printServicePushBack ((((*it).second)[j])[k].first);
                cpp << endl;
                cpp << INDENT1 << "recoveryVec.push_back ( " << (((*it).second)[j])[k].second << " );" << endl;
            }

            cpp << INDENT1 << "postBootTable[" << tableName << "].push_back (pass);" << endl << endl;
        }

        cpp << endl;
    }

    cpp << INDENT1 << "return (postBootTable);" << endl;

    cpp << "}" << endl << endl;
    cpp << "}" << endl;

    cpp.close();
}

// tableName is updated in THIS function
void configReplayTable::addPass (string *tableName, int passNumber, vector <pair <string, int> > pass_name_service) 
{
    static int passNumberBase = 0;

    if ((*tableName).size() == 0)
    {
         configReplayMap.clear();
         passNumberBase = 0;
    }

    if (pass_name_service[0].first.compare(GLOBAL_TABLE_BEGIN) == 0)
    {
         *tableName = POSTBOOT_GLOBAL_CONFIG;

    }
    else if (pass_name_service[0].first.compare(LINECARD_TABLE_BEGIN) == 0)
    {
        *tableName = POSTBOOT_LINECARD_CONFIG;
    }

    (configReplayMap[*tableName])[(passNumber - passNumberBase)] = pass_name_service;

    if ((pass_name_service[0].first.compare(GLOBAL_TABLE_END) == 0) ||
       (pass_name_service[0].first.compare(LINECARD_TABLE_END) == 0))
    {
        passNumberBase = passNumber;
    }
}

int main(int argc, char *argv[]) 
{
    int	retval;

    if (argc != 4) {
        cout << "Error: Arguments to the PostBootParser are not correct" << endl;
        return (0);
    }

    configReplayTable *pTable = new configReplayTable(); 

    retval = parseXml (argv[1], pTable); 
    if (retval < 0) {
        cout << "Error: Code could not be generated" << endl;	
        return (0);
    }

    string cppFilePath = argv[2];
    cppFilePath.append ("/");
    cppFilePath.append (argv[3]);
    cppFilePath.append ("PostBootProgrammingTable.cpp");

    string dotHFilePath = argv[2];
    dotHFilePath.append ("/");
    dotHFilePath.append (argv[3]);
    dotHFilePath.append ("PostBootProgrammingTable.h");
    

    projectName = argv[3];

    pTable->printCpp (cppFilePath);
    pTable->printDotH (dotHFilePath);

    delete pTable;

    return 0;
}

int parseXml (char* xml_file, configReplayTable *pTable) 
{
    XMLPlatformUtils::Initialize ();

    XercesDOMParser* plugin_parser = new XercesDOMParser();
    plugin_parser->setValidationScheme(XercesDOMParser::Val_Auto);
    plugin_parser->setDoNamespaces(false);    // optional

    plugin_parser->parse(xml_file);

    DOMDocument *pdoc = plugin_parser->getDocument();

    if (NULL == pdoc) 
    {
        cout << "No Components found" << endl;
    }
    else
    { 
        DOMElement  *pRootNode  = (pdoc->getDocumentElement ());
        if (NULL == pRootNode)
        {
            cout << "\033[31mNo Components found.\033[0m" << endl;
        }
        else
        {
            XMLCh *pass_attribute = XMLString::transcode ("Pass");

            DOMNodeList *pass_childlist = pRootNode->getElementsByTagName(pass_attribute);

            DOMNode *pass_child_node = NULL;
            string tableName = "";

            // for all "Pass" entries
            for (int i=1; i <= (int)pass_childlist->getLength(); i++) 
            {
                pass_child_node = pass_childlist->item(i-1);

                if (pass_child_node->getNodeType() == DOMNode::ELEMENT_NODE) 
                {
                    DOMElement *pPassElement = reinterpret_cast<DOMElement *> (pass_child_node);

                    vector <pair <string, int> >    pass_name_service;
                    pair <string, int>              service_recovery_types;
                    pass_name_service.clear();
	
                    XMLCh *pass_name_attribute = XMLString::transcode ("name");
                    char *pass_name_value = XMLString::transcode (pPassElement->getAttribute(pass_name_attribute));

                    string passname = pass_name_value; 

                    // first entry of pass_name_service contains the pass name and other entries contain the service names under that pass
                    service_recovery_types.first = passname;
                    service_recovery_types.second = WAVE_RECOVERY_TYPE_INVALID; // recovery type is not valid entry for pass name

                    pass_name_service.push_back (service_recovery_types);
                    //cout <<"passname " << service_recovery_types.first << " recovery type " << service_recovery_types.second << endl;

	                XMLCh *service_attribute = XMLString::transcode ("Service");

                    DOMNode *service_child_node = NULL;
                    DOMNodeList *service_childlist = pPassElement->getElementsByTagName(service_attribute);

                    // for all "Service" entries under a "Pass"
                    for (int j=1; j <= (int)service_childlist->getLength(); j++) 
                    {
                        service_child_node = service_childlist->item(j-1);
                        
                        if (service_child_node->getNodeType() == DOMNode::ELEMENT_NODE)
                        {
                            DOMElement *pServiceElement = reinterpret_cast<DOMElement *> (service_child_node);

                            XMLCh *service_name_attribute = XMLString::transcode ("name");
                            char *service_name_value = XMLString::transcode (pServiceElement->getAttribute(service_name_attribute));

                            string serviceName = service_name_value;
                            //cout <<"servicename " << serviceName << endl;

                            XMLCh *recovery_attribute = XMLString::transcode ("Recovery");

                            DOMNode *recovery_child_node = NULL;
                            DOMNodeList *recovery_childlist = pServiceElement->getElementsByTagName(recovery_attribute);

                            // for all "Recovery" entries under a "Service"
                            int recovery_types = 0;
                            for (int k=1; k <= (int)recovery_childlist->getLength(); k++) 
                            {
                                recovery_child_node = recovery_childlist->item(k-1);
                                
                                if (recovery_child_node->getNodeType() == DOMNode::ELEMENT_NODE)
                                {
                                    DOMElement *pRecoveryElement = reinterpret_cast<DOMElement *> (recovery_child_node);

                                    XMLCh *recovery_name_attribute = XMLString::transcode ("name");
                                    char *recovery_name_value = XMLString::transcode (pRecoveryElement->getAttribute(recovery_name_attribute));

                                    string recoveryName = recovery_name_value;

                                    //pass_name_recovery.push_back (recoveryName);
                                    if ( 0 == recoveryName.compare ("Warm") )
                                    {
                                        recovery_types |= WAVE_RECOVERY_TYPE_WARM;
                                    }
                                    else if ( 0 == recoveryName.compare ("Cold") )
                                    {
                                        recovery_types |= WAVE_RECOVERY_TYPE_COLD;
                                    }
                                    else
                                    {
                                        // invalid entry in xml. ignore
                                    }

                                    //cout <<"recoveryname " << recoveryName << " recovery_types " << recovery_types << endl;
                                }
                            }

                            service_recovery_types.first = serviceName;
                            service_recovery_types.second = recovery_types;
                            pass_name_service.push_back (service_recovery_types);
                            //cout <<"serviceName " << service_recovery_types.first << " recovery type " << service_recovery_types.second << endl;
                        }
                    }
                    pTable->addPass (&tableName, i, pass_name_service);
                    XMLString::release (&pass_name_value);
                }
            }
        }
    }

    return 0;
}
