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

/***************************************************************************
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include <iomanip>
#include <strings.h>
#include <string.h>

#include "Framework/Core/Wave.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Shell/WaveCliShell.h"
#include "Framework/../../application/WaveCliReplyThread.h"
#include "Modeling/YANG/Parser/YinParser.h"
#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "Modeling/YANG/ObjectModel/YangType.h"

using namespace std;
using namespace WaveNs;

extern "C" int dcmClientMain (const char * const pProgramName, const int clientPort);

void displayUsage (const char *pProgramName)
{
    string emptyProgramName;
    UI32   programNameSize   = strlen (pProgramName);
    UI32   i                 = 0;

    for (i = 0; i < programNameSize; i++)
    {
        emptyProgramName += " ";
    }

    cout << "USAGE:" << endl;
    cout << pProgramName     << " [[-yf | -yinfile] <YIN File Path>]" << endl;
    cout << emptyProgramName << " [[-yp | -yinpath] <Directory Path containing YIN Files>]" << endl;
    cout << emptyProgramName << " [-module <YANG Module Name>]" << endl;
    cout << emptyProgramName << " [[-qyp | -qualifiedyangpath] <Qualified Yang Path, a.k.a. Qualified Path for Target Yang Node>]" << endl;
    cout << emptyProgramName << " [[-pqyp | -prefixqualifiedyangpath] <Qualified Yang Path, a.k.a. Prefix Qualified Path for Target Yang Node>]" << endl;
    cout << emptyProgramName << " [-checkintegrity]" << endl;
    cout << emptyProgramName << " [-printyin]" << endl;
    cout << emptyProgramName << " [-printclonedyin]" << endl;
    cout << emptyProgramName << " [-printfullyin]" << endl;
    cout << emptyProgramName << " [-printfullui]" << endl;
    cout << emptyProgramName << " [-printallnames]" << endl;
    cout << emptyProgramName << " [-printallclitargetnodenamesfordata]" << endl;
    cout << emptyProgramName << " [-printsorteddataelementtree]" << endl;
    cout << emptyProgramName << " [-printtargetnodeinformation <target name>]" << endl;
    cout << emptyProgramName << " [[-of | -outputfile] <Output File Path>]" << endl;
    cout << endl;
    cout << endl;
    cout << "Command Line Options:" << endl;
    cout << endl;
}

void initializeResourceIdStringVector ( vector <string> &attributeTypeResourceIdStringVector);
void attributeTest (int argc, char* argv[]);
void testAllAttributesForInputString (int argc, char* argv[]);

int main (int argc, char *argv[])
{
    attributeTest (argc, argv);
    testAllAttributesForInputString (argc, argv);

    string                 command;
    SI32                   i                                 = 0;
    vector<string>         yinFileNames;
    vector<string>         outputYinFileNames;
    vector<YangElement *>  yangElements;
    bool                   printYin                          = false;
    bool                   printClonedYin                    = false;
    string                 yinDirectoryPath;
    vector<string>         yangDirectoryPaths;
    bool                   printFullYin                      = false;
    bool                   printFullUi                       = false;
    string                 qualifiedYangPath;
    string                 prefixQualifiedYangPath;
    string                 outputfilePath;
    bool                   checkIntegrity                    = false;
    string                 moduleName;
    bool                   printAllNames                     = false;
    bool                   printAllCliTargetNodeNamesForData = false;
	bool                   findMaxDepth                      = false;
    string                 maxDepthFile                      = "";
    bool                   printSortedDataElementTree        = false;
    bool                   printTargetnodeInformation        = false;
    vector<string>         targetNames;
    bool                   generateHFileForCLanguage         = false;
    string                 wyserTagsFile;

    for (i = 1; i < argc; i++)
    {
        if ((0 == strcasecmp ("-yf", argv[i])) || (0 == strcasecmp ("-yinfile", argv[i])))
        {
            if (argc > (i + 1))
            {
                yinFileNames.push_back (string (argv[i + 1]));
            }

            i++;
        }
        else if ((0 == strcasecmp ("-yp", argv[i])) || (0 == strcasecmp ("-yinpath", argv[i])))
        {
            if (argc > (i + 1))
            {
                yinDirectoryPath = argv[i + 1];
                yangDirectoryPaths.push_back (yinDirectoryPath);
            }

            i++;
        }
        else if ((0 == strcasecmp ("-qyp", argv[i])) || (0 == strcasecmp ("-qualifiedyangpath", argv[i])))
        {
            if (argc > (i + 1))
            {
                qualifiedYangPath = argv[i + 1];
            }

            i++;
        }
        else if ((0 == strcasecmp ("-pqyp", argv[i])) || (0 == strcasecmp ("-prefixqualifiedyangpath", argv[i])))
        {
            if (argc > (i + 1))
            {
                prefixQualifiedYangPath = argv[i + 1];
            }

            i++;
        }
        else if ((0 == strcasecmp ("-of", argv[i])) || (0 == strcasecmp ("-outputfile", argv[i])))
        {
            if (argc > (i + 1))
            {
                outputfilePath = argv[i + 1];
            }

            i++;
        }
        else if (0 == strcasecmp ("-printyin", argv[i]))
        {
            printYin = true;
        }
        else if (0 == strcasecmp ("-printclonedyin", argv[i]))
        {
            printClonedYin = true;
        }
        else if (0 == strcasecmp ("-printfullyin", argv[i]))
        {
            printFullYin = true;
        }
        else if (0 == strcasecmp ("-printfullui", argv[i]))
        {
            printFullUi = true;
        }
        else if (0 == strcasecmp ("-checkintegrity", argv[i]))
        {
            checkIntegrity = true;
        }
        else if (0 == strcasecmp ("-printallnames", argv[i]))
        {
            printAllNames = true;
        }
        else if (0 == strcasecmp ("-printallclitargetnodenamesfordata", argv[i]))
        {
            printAllCliTargetNodeNamesForData = true;
        }
        else if (0 == strcasecmp ("-module", argv[i]))
        {
            if (argc > (i + 1))
            {
                moduleName = argv[i + 1];
            }

            i++;
        }
		else if (0 == strcasecmp ("-maxdepth", argv[i]))
        {
            findMaxDepth = true;
        }
        else if (0 == strcasecmp ("-maxdepthfile", argv[i]))
        {
            findMaxDepth = true;
            if (argc > (i + 1))
            {
                maxDepthFile = argv[i + 1];
                cout << maxDepthFile << endl;
            }

            i++;
        }
        else if (0 == strcasecmp ("-printsorteddataelementtree", argv[i]))
        {
            printSortedDataElementTree = true;
        }
        else if (0 == strcasecmp ("-printtargetnodeinformation", argv[i]))
        {
            if (argc > (i + 1))
            {
                targetNames.push_back (argv[i + 1]);

                printTargetnodeInformation = true;
            }

            i++;
        }
        else if (0 == strcasecmp ("-generate-h-c", argv[i]))
        {
            generateHFileForCLanguage = true;
        }
        else if (0 == strcasecmp ("-wysertags", argv[i]))
        {
            if (argc > (i + 1))
            {
                wyserTagsFile = argv[i + 1];
            }

            i++;
        }
        else
        {
            displayUsage (argv[0]);
            return (1);
        }
    }

    YangModuleCollection yangModuleCollection;

    yangModuleCollection.loadModulesFromDirectoriesAndFiles (yangDirectoryPaths, yinFileNames);

    cout << "Updating Original Module NameSpace Uri in All Modules ..." << endl;

    yangModuleCollection.setOriginalModuleNameSpaceUriInAllModules ();

    cout << "Done.\r\n";

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Yang Module Collection ... ";

        yangModuleCollection.checkIntegrity ();

        cout << "Done.\r\n";
    }

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Cloned Yang Module Collection After Inlining Typedef ... ";
        yangModuleCollection.checkIntegrity ();
        cout << "Done.\r\n";
    }

    if (true == generateHFileForCLanguage)
    {
        cout << "Inlining Grouping Usage for RPC s ..." << endl;

        yangModuleCollection.inlineGroupingUsageForRpcs ();

        cout << "Computing Usage Counts For Groupings ..." << endl;

        yangModuleCollection.computeUsageCountForGroupings                        ();
        yangModuleCollection.computeUsageCountForGroupingsForProgrammingLanguages ();
        yangModuleCollection.removeAllUnusedGroupings                             ();

        if ("" != wyserTagsFile)
        {
            cout << "Loading WYSER Tags File ..." << endl;

            yangModuleCollection.loadUserTagsFromFile (wyserTagsFile);
        }

        yangModuleCollection.generateHFilesForCLanguageForAllModules ();

        return (0);
    }

    YangType::initializeBuiltinTypes();
    yangModuleCollection.inlineTypedef ();

    yangModuleCollection.inlineGroupingUsage ();

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Cloned Yang Module Collection After Inlining Groupings ... ";

        yangModuleCollection.checkIntegrity ();

        cout << "Done.\r\n";
    }

    yangModuleCollection.inlineAugmentUsage ();

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Cloned Yang Module Collection After Inlining Augments ... ";

        yangModuleCollection.checkIntegrity ();

        cout << "Done.\r\n";
    }

    yangModuleCollection.removeAllGroupings ();

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Cloned Yang Module Collection After Removing Groupings ... ";

        yangModuleCollection.checkIntegrity ();

        cout << "Done.\r\n";
    }

    yangModuleCollection.processTypeInformations ();

    if (true == checkIntegrity)
    {
        cout << "Checking Integrity For Cloned Yang Module Collection After process type informations ... ";

        yangModuleCollection.checkIntegrity ();

        cout << "Done.\r\n";
    }

    YangUserInterface yangUserInterface;

    if ("" != qualifiedYangPath)
    {
        yangModuleCollection.printYinForQualifiedYangPathToFile (qualifiedYangPath, outputfilePath);
    }

    if ("" != prefixQualifiedYangPath)
    {
        yangModuleCollection.printYinForPrefixQualifiedYangPathToFile (prefixQualifiedYangPath, outputfilePath);
    }

    if (true == printClonedYin)
    {
        // TODO : Sagar : print cloned yin.
    }

    if (true == printFullYin)
    {
        yangModuleCollection.printYinForAllModulesToFile (outputfilePath);
    }

    if (true == printYin)
    {
        yangModuleCollection.printYinForModuleToFile (moduleName, outputfilePath);
    }

    yangModuleCollection.transferAllModulesToUserInterface (&yangUserInterface);

    cout << "Collecting Child Leaf with custom display ..." << endl;

    yangUserInterface.collectInformationAboutChildLeafWithCustomDisplay ();

    if (true == printFullUi)
    {
        yangUserInterface.printYinToFile (outputfilePath);
    }

    if (true == printAllNames)
    {
        yangUserInterface.printAllNamesToFile (outputfilePath);
    }

	if (true == printAllCliTargetNodeNamesForData || true == findMaxDepth)
    {
        yangUserInterface.computeCliTargetNodeNames ();
        yangUserInterface.computeWillBeShownInConfigurationTree ();

        if (true == printAllCliTargetNodeNamesForData)
        {
            yangUserInterface.printAllCliTargetNodeNamesForDataToFile (outputfilePath);
        }

        if (true == findMaxDepth)
        {
            if (false == maxDepthFile.empty ())
            {
                FILE *pFile = fopen (maxDepthFile.c_str(), "w");
                UI32 maxDepth = yangUserInterface.getMaxDepth ();
                fprintf (pFile,"%d\n", maxDepth);
            }
            else
            {
                cout << "Error: maxDepth is set to true but no maxDepthFile given to write the maxDepth to" << endl;
            }
        }
    }

    if (true == printSortedDataElementTree)
    {
        if (false == printAllCliTargetNodeNamesForData)
        {
            yangUserInterface.computeCliTargetNodeNames ();
            yangUserInterface.computeWillBeShownInConfigurationTree ();
        }

        yangUserInterface.prepareYangElementTreeForData ();
        yangUserInterface.debugPrintYangElementTreeForData (outputfilePath);
    }

    if (true == printTargetnodeInformation)
    {
        if (false == printAllCliTargetNodeNamesForData)
        {
            yangUserInterface.computeCliTargetNodeNames ();
            yangUserInterface.computeWillBeShownInConfigurationTree ();
        }

        yangUserInterface.prepareYangElementTreeForData ();
        yangUserInterface.computeCliTargetNodeNameMapForData ();

        for (vector<string>::iterator itr1 = targetNames.begin (); itr1 != targetNames.end (); itr1++)
        {
            cout << "Target name [" << *itr1 << "]" << endl;

            vector<YangElement *>   targetYangElements  = yangUserInterface.getYangElementsByTargetNodeName (*itr1);

            if (0 == targetYangElements.size ())
            {
                cout << "    " << "No elements found." << endl;
                continue;
            }

            UI32                    counter             = 0;

            for (vector<YangElement *>::iterator itr2 = targetYangElements.begin (); itr2 != targetYangElements.end (); itr2++)
            {
                counter++;
                cout << "    " << counter << ". " << (*itr2)->getYangName () << ":" << (*itr2)->getName () << endl;
            }
        }
    }

    if (false == printAllCliTargetNodeNamesForData)
    {
        yangUserInterface.computeCliTargetNodeNames ();
        yangUserInterface.computeWillBeShownInConfigurationTree ();
        yangUserInterface.prepareTypeInformation ();
    }

    return (0);
}

void attributeTest (int argc, char* argv[])
{
    if (2 > argc)
    {
        return;
    }

    if ("-attributetest" != string (argv[1]))
    {
        return;
    }

    if (4 > argc)
    {
        cout << "Insufficient number of argument." << endl;

        exit (1);
    }

    string attributeTypeResourceIdString    = argv[2];
    string attributeValuePlainString        = argv[3];

    cout << "Input values are :" << endl;

    cout << "    AttributeType              [" << attributeTypeResourceIdString << "]" << endl;
    cout << "    AttributeValuePlainString  [" << attributeValuePlainString << "]" << endl;

    FrameworkToolKit::setDefaultTraceLevel (TRACE_LEVEL_FATAL);
    WaveNs::initializeWaveResourceIds ();

    ResourceId attributeTypeResourceIdValue = FrameworkToolKit::localizeToSourceCodeResourceId (attributeTypeResourceIdString);

    if (0 == attributeTypeResourceIdValue)
    {
        cerr << "Invalid AttributeType. Exiting." << endl;

        exit (1);
    }

    Attribute *pAttribute = Attribute::getAttributeFromAttributeType (attributeTypeResourceIdValue);

    if (NULL == pAttribute)
    {
        cerr << "Cound not create attribute. Exiting." << endl;

        exit (1);
    }

    cout << "Created an attribute of type [" << FrameworkToolKit::localizeToSourceCodeEnum (attributeTypeResourceIdValue) << "]" << endl;

    string stringValue;

    pAttribute->toString (stringValue);

    cout << "       toString : [" << stringValue << "]" << endl;

    pAttribute->getPlainString (stringValue);

    cout << " getPlainString : [" << stringValue << "]" << endl;

    cout << "Loading AttributeValuePlainString into this attribute. ";

    ResourceId loadFromPlainStringStatus = pAttribute->loadFromPlainString (attributeValuePlainString);

    cout << "Status [" << FrameworkToolKit::localizeToSourceCodeEnum (loadFromPlainStringStatus) << "]" << endl;

    if (WAVE_MESSAGE_SUCCESS != loadFromPlainStringStatus)
    {
        cout << "Exiting." << endl;

        delete pAttribute;

        exit (1);
    }

    pAttribute->toString (stringValue);

    cout << "       toString : [" << stringValue << "]" << endl;

    pAttribute->getPlainString (stringValue);

    cout << " getPlainString : [" << stringValue << "]" << endl;

    if (stringValue == attributeValuePlainString)
    {
        cout << "    Plain string value matches with input value. Success." << endl;
    }
    else
    {
        cerr << "    Mismatch between 'input' and 'output' Plain String values." << endl;
        cerr << "    Input  [" << attributeValuePlainString << "]" << endl;
        cerr << "    Output [" << stringValue << "]" << endl;

        delete pAttribute;

        exit (1);
    }

    delete pAttribute;

    exit (0);
}


void testAllAttributesForInputString (int argc, char* argv[])
{
    if (2 > argc)
    {
        return;
    }

    if ("-testallattributes" != string (argv[1]))
    {
        return;
    }

    if (3 > argc)
    {
        cout << "Insufficient number of argument." << endl;

        exit (1);
    }

    vector<string> attributeTypeResourceIdStringVector;

    initializeResourceIdStringVector (attributeTypeResourceIdStringVector);

    for (int j = 2 ; j < argc ; j++)
    {
        string attributeValuePlainString        = argv[j];

        cout << "\nInput value AttributeValuePlainString is  [" << attributeValuePlainString << "]" << endl;
        cout << "\n----------------------------------------------------------------------------- :" << endl;
        cout << "\n----------------------------------------------------------------------------- :" << endl;

        for (UI32 i = 0 ; i < attributeTypeResourceIdStringVector.size(); ++i )
        {

            //FrameworkToolKit::setDefaultTraceLevel (TRACE_LEVEL_FATAL);
            FrameworkToolKit::setDefaultTraceLevel (TRACE_LEVEL_ERROR);
            WaveNs::initializeWaveResourceIds ();

            ResourceId attributeTypeResourceIdValue = FrameworkToolKit::localizeToSourceCodeResourceId (attributeTypeResourceIdStringVector[i]);

            if (0 == attributeTypeResourceIdValue)
            {
                cout << "\nAttributeType : [" << attributeTypeResourceIdStringVector[i] << "] ";
                cout << "Invalid AttributeType. Exiting." << endl;
                continue ;
            }

            Attribute *pAttribute = Attribute::getAttributeFromAttributeType (attributeTypeResourceIdValue);

            if (NULL == pAttribute)
            {
                cout << "\nAttributeType : [" << attributeTypeResourceIdStringVector[i] << "] ";
                cout << "Cound not create attribute. Exiting." << endl;
                continue;
            }

            cout << "--------------------------------------------------------------------------------------------------------------------------------------- :" << endl;
            ResourceId loadFromPlainStringStatus = pAttribute->loadFromPlainString (attributeValuePlainString);

            cout <<std::setw(19) <<"AttributeType : [";
            cout <<std::setw(45) <<std::left <<attributeTypeResourceIdStringVector[i];
            cout << "] ";
            cout <<std::setw (10) <<"Status: [";
            cout <<std::setw(20) <<std::left << FrameworkToolKit::localizeToSourceCodeEnum (loadFromPlainStringStatus) << "]\n" ;

            if (WAVE_MESSAGE_SUCCESS != loadFromPlainStringStatus)
            {
                delete pAttribute;
                continue;
            }

            string stringValue;
            pAttribute->toString (stringValue);
            pAttribute->getPlainString (stringValue);

            //     cout << "toString : [" << stringValue << "]";
            //     cout << " getPlainString : [" << stringValue << "]" << endl;

            if (stringValue == attributeValuePlainString)
            {
                cout << "    Plain string value matches with input value. Success." << endl;
            }
            else
            {
                cout << "    Mismatch between 'input' and 'output' Plain String values.";
                cout << "    Input  [" << attributeValuePlainString << "]";
                cout << "    Output [" << stringValue << "]" <<endl;
                delete pAttribute;
                continue;
            }

            delete pAttribute;
        }
    }
    exit (0);
}

void initializeResourceIdStringVector ( vector <string> &attributeTypeResourceIdStringVector)
{
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI32");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI64");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI64");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI32");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI16");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI16");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI8");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI8");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_BOOL");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_CHAR");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_STRING");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_STRING_VECTOR");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_MACADDRESS");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_ENUM");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DATE");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DATETIME");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DECIMAL64");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_NETWORK_MASK");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SNMPOBJECTID");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI32_RANGE");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPVXADDRESS");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_MAC_RANGE");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI32_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_STRING_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_BOOL_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_ENUM_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_MACADDRESS2");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_STRING_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_HOST_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI64_UC");
    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_OBJECTID");

    // attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_BITMAP");      // ------tostring failing on invalid inputs commenting it out

    //    vectors not in use so not implemented
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_OBJECTID_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI64_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI32_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI64_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI32_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_BOOL_VECTOR");

    // check these closely
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_RESOURCEID");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_RESOURCEENUM");


    //  Vector type will be dealt with later
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_WORLD_WIDE_NAME_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_MACADDRESS_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_MACADDRESS2_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV4ADDRESS_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DATE_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DATETIME_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_DECIMAL64_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPV6ADDRESS_NETWORK_MASK");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_IPVXADDRESS_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI32_VECTOR_UC");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_RESOURCEID_VECTOR");
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_RESOURCEENUM_VECTOR");


    // unsupported resource id
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI8_VECTOR");                   // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI8_VECTOR");                   // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SI16_VECTOR");                  // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UI16_VECTOR");                  // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_TYPE");            // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_WAVE_MESSAGE_PRIORITY");        // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UNION");                        // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_PRISM_SERVICE_ID");             // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT_TYPE");     // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_LOCATIONID");                   // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SQL_BETWEEN");                  // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SQL_IN");                       // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_SERIALIZABLE_OBJECT");          // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_COMPOSITION");                  // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_COMPOSITION_VECTOR");           // unsupported
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_UUID");                         // Not implemented as is  not a user input thing so leaving it
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_STRING_VECTOR_VECTOR");         // Not implemented as is  not a user input thing so leaving it
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_TIME");                         // Not implemented as is  not a user input thing so leaving it
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_TIME_VECTOR");                  // Not implemented as is  not a user input thing so leaving it
    //    attributeTypeResourceIdStringVector.push_back ("WAVE_ATTRIBUTE_TYPE_LARGEOBJECT");                  // Not implemented as it is not used any where
}
