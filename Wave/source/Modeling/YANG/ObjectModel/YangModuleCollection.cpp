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

#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Modeling/YANG/Parser/YinParser.h"
#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"

#include <fstream>
#include <string>

using namespace std;

namespace WaveNs
{

YangModuleCollection::YangModuleCollection ()
{
}

YangModuleCollection::~YangModuleCollection ()
{
    removeAllModules ();
}

bool YangModuleCollection::isAKnownYangModuleByName (YangModule *pYangModule) const
{
    prismAssert (NULL != pYangModule, __FILE__, __LINE__);

    const string yangModuleName = pYangModule->getName ();

    return (isAKnownYangModuleByName (yangModuleName));
}

bool YangModuleCollection::isAKnownYangModuleByName (const string &yangModuleName) const
{
    map<string, YangModule *>::const_iterator element    = m_yangModulesMap.find (yangModuleName);
    map<string, YangModule *>::const_iterator endElement = m_yangModulesMap.end  ();

    if (element == endElement)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

bool YangModuleCollection::isAKnownYangModuleByPrefix (YangModule *pYangModule) const
{
    prismAssert (NULL != pYangModule, __FILE__, __LINE__);

    const string yangModulePrefix = pYangModule->getPrefix ();

    return (isAKnownYangModuleByPrefix (yangModulePrefix));
}

bool YangModuleCollection::isAKnownYangModuleByPrefix (const string &yangModulePrefix) const
{
    map<string, YangModule *>::const_iterator element    = m_yangModulesMapByPrefix.find (yangModulePrefix);
    map<string, YangModule *>::const_iterator endElement = m_yangModulesMapByPrefix.end  ();

    if (element == endElement)
    {
        return (false);
    }
    else
    {
        return (true);
    }
}

void YangModuleCollection::addYangModule (YangModule *pYangModule)
{
    prismAssert (NULL != pYangModule, __FILE__, __LINE__);

    bool isKnown         = isAKnownYangModuleByName   (pYangModule);
    bool isKnownByPrefix = isAKnownYangModuleByPrefix (pYangModule);

    if ((true == isKnown) || (true == isKnownByPrefix))
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        const string moduleName   = pYangModule->getName   ();
        const string modulePrefix = pYangModule->getPrefix ();

        m_yangModules.push_back (pYangModule);
        m_yangModulesMap[moduleName] = pYangModule;
        m_yangModulesMapByPrefix[modulePrefix] = pYangModule;
    }
}

void YangModuleCollection::addYangModules (vector<YangModule *> yangModules)
{
    UI32 numberOfYangModules = yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        addYangModule (yangModules[i]);
    }
}

void YangModuleCollection::loadModulesFromDirectory (const string &directoryPath)
{
    vector<string>        yinFileNames;
    vector<YangElement *> yangElements;
    vector<YangModule *>  yangModules;
    UI32                  numberOfYangElements = 0;
    UI32                  i                    = 0;

    yangElements = YinParser::parseDiretory (directoryPath, yinFileNames);

    numberOfYangElements = yangElements.size ();

    for (i = 0; i < numberOfYangElements; i++)
    {
        YangElement *pYangElement = yangElements[i];

        if (NULL != pYangElement)
        {
            YangModule *pYangModule = dynamic_cast<YangModule *> (pYangElement);

            if (NULL != pYangModule)
            {
                yangModules.push_back (pYangModule);
            }
        }
    }

    addYangModules (yangModules);
}

void YangModuleCollection::loadModuleFromFile (const string &filePath)
{
    YangElement *pYangElement = NULL;
    YangModule  *pYangModule  = NULL;

    pYangElement = YinParser::parseFile (filePath);

    if (NULL != pYangElement)
    {
        pYangModule = dynamic_cast<YangModule *> (pYangElement);

        if (NULL != pYangModule)
        {
            addYangModule (pYangModule);
        }
    }
}

void YangModuleCollection::loadModulesFromDirectoriesAndFiles (const vector<string> &directoryPaths, const vector<string> &yinFileNames)
{
    vector<string>        outputYinFileNames;
    vector<YangElement *> yangElements;
    vector<YangModule *>  yangModules;
    UI32                  numberOfYangElements = 0;
    UI32                  i                    = 0;

    yangElements = YinParser::parseDirectoriesAndFiles (directoryPaths, yinFileNames, outputYinFileNames);

    numberOfYangElements = yangElements.size ();

    for (i = 0; i < numberOfYangElements; i++)
    {
        YangElement *pYangElement = yangElements[i];

        if (NULL != pYangElement)
        {
            YangModule *pYangModule = dynamic_cast<YangModule *> (pYangElement);

            if (NULL != pYangModule)
            {
                yangModules.push_back (pYangModule);
            }
        }
    }

    addYangModules (yangModules);
}

void YangModuleCollection::removeAllModules ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        delete m_yangModules[i];
    }

    clearAllModules ();
}

void YangModuleCollection::clearAllModules ()
{
    m_yangModules.clear ();
    m_yangModulesMap.clear ();
    m_yangModulesMapByPrefix.clear ();
}

void YangModuleCollection::getAllKnownModules (vector<string> &allKnownYangModules)
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    allKnownYangModules.clear ();

    for (i = 0; i < numberOfYangModules; i++)
    {
        const string yangModuleName = (m_yangModules[i])->getName ();

        allKnownYangModules.push_back (yangModuleName);
    }
}

YangModule *YangModuleCollection::getYangModuleByName (const string &yangModuleName) const
{
    YangModule *pYangModule = NULL;

    if (true == (isAKnownYangModuleByName (yangModuleName)))
    {
        map<string, YangModule *>::const_iterator element = m_yangModulesMap.find (yangModuleName);

        pYangModule = element->second;

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);
    }

    return (pYangModule);
}

YangModule *YangModuleCollection::getYangModuleByPrefix (const string &yangModulePrefix) const
{
    YangModule *pYangModule = NULL;

    if (true == (isAKnownYangModuleByPrefix (yangModulePrefix)))
    {
        map<string, YangModule *>::const_iterator element = m_yangModulesMapByPrefix.find (yangModulePrefix);

        pYangModule = element->second;

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);
    }

    return (pYangModule);
}

YangModuleCollection *YangModuleCollection::clone () const
{
    UI32                  numberOfYangModules         = m_yangModules.size       ();
    UI32                  i                           = 0;
    YangModuleCollection *pClonedYangModuleCollection = new YangModuleCollection ();

    prismAssert (NULL != pClonedYangModuleCollection, __FILE__, __LINE__);

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        YangModule *pClonedYangModule = dynamic_cast<YangModule *> (pYangModule->clone ());

        prismAssert (NULL != pClonedYangModule, __FILE__, __LINE__);

        pClonedYangModuleCollection->addYangModule (pClonedYangModule);
    }

    return (pClonedYangModuleCollection);
}

void YangModuleCollection::transferAllModulesToUserInterface (YangUserInterface *pYangUSerInterface)
{
    map<string, YangModule *>::const_iterator element    = m_yangModulesMap.begin ();
    map<string, YangModule *>::const_iterator endElement = m_yangModulesMap.end   ();

    while (endElement != element)
    {
        YangModule *pYangModule = element->second;

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangUSerInterface->addChildElement (pYangModule);

        element++;
    }

    clearAllModules ();
}

void YangModuleCollection::computeUsageCountForGroupings ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->computeUsageCountForGroupings (this);
    }
}

void YangModuleCollection::computeUsageCountForGroupingsForProgrammingLanguages ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->computeUsageCountForGroupingsForProgrammingLanguages (this);
    }
}

void YangModuleCollection::inlineGroupingUsage ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->inlineGroupingUsage (this);
    }
}

void YangModuleCollection::inlineGroupingUsageForRpcs ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->inlineGroupingUsageForRpcs (this);
    }
}

void YangModuleCollection::inlineAugmentUsage ()
{
    UI32 numberOfYangModules                  = m_yangModules.size ();
    UI32 i                                    = 0;
    UI32 numberOfAugmentsResolved             = 1;
    UI32 numberOfAugmentsNotResolved          = 1;
    UI32 numberOfAugmentsResolvedPerModule    = 1;
    UI32 numberOfAugmentsNotResolvedPerModule = 1;
    UI32 passNumber                           = 0;


    while (0 != numberOfAugmentsNotResolved && passNumber < 100)
    {
        passNumber++;

        cout << "PASS : " << passNumber << endl;

        numberOfAugmentsResolved    = 0;
        numberOfAugmentsNotResolved = 0;

        for (i = 0; i < numberOfYangModules; i++)
        {
            YangModule *pYangModule = m_yangModules[i];

            prismAssert (NULL != pYangModule, __FILE__, __LINE__);

            numberOfAugmentsResolvedPerModule    = 0;
            numberOfAugmentsNotResolvedPerModule = 0;

            pYangModule->inlineAugmentUsage (this, numberOfAugmentsResolvedPerModule, numberOfAugmentsNotResolvedPerModule);

            numberOfAugmentsResolved    += numberOfAugmentsResolvedPerModule;
            numberOfAugmentsNotResolved += numberOfAugmentsNotResolvedPerModule;
        }
    }

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->rearrangeAugmenterChildElements ();
    }
}

void YangModuleCollection::checkIntegrity () const
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->checkIntegrity ();
    }
}

void YangModuleCollection::removeAllUnusedGroupings ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->removeAllUnusedGroupings ();
    }
}

void YangModuleCollection::removeAllGroupings ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->removeAllGroupings ();
    }
}

void YangModuleCollection::printYinForQualifiedYangPath (const string &qualifiedYangPath, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    string moduleName;
    string targetNodePath;

    getTargetNodeDetails (qualifiedYangPath, moduleName, targetNodePath);

    cout << "Printing YIN for " << moduleName << ":" << targetNodePath << endl;

    YangModule *pYangModule = getYangModuleByName (moduleName);

    if (NULL != pYangModule)
    {
        pYangModule->printYinForTargetNode (targetNodePath, pFile);
    }
}

void YangModuleCollection::printYinForQualifiedYangPathToFile (const string &qualifiedYangPath, const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYinForQualifiedYangPath (qualifiedYangPath, pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYinForQualifiedYangPath (qualifiedYangPath, pFile);
    }
}

void YangModuleCollection::printYinForPrefixQualifiedYangPath (const string &prefixQualifiedYangPath, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    string modulePrefix;
    string targetNodePath;

    getTargetNodeDetails (prefixQualifiedYangPath, modulePrefix, targetNodePath);

    cout << "Printing YIN for " << modulePrefix << ":" << targetNodePath << endl;

    YangModule *pYangModule = getYangModuleByPrefix (modulePrefix);

    if (NULL != pYangModule)
    {
        pYangModule->printYinForTargetNode (targetNodePath, pFile);
    }
}

void YangModuleCollection::printYinForPrefixQualifiedYangPathToFile (const string &prefixQualifiedYangPath, const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYinForPrefixQualifiedYangPath (prefixQualifiedYangPath, pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYinForPrefixQualifiedYangPath (prefixQualifiedYangPath, pFile);
    }
}

void YangModuleCollection::printYinForAllModules (FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    cout << "Printing YIN for all modules ..." << endl;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        string moduleName = pYangModule->getName ();

        cout << "    Processing " << moduleName << " ... ";

        pYangModule->printYin (0, true, pFile);

        cout << "Done." << endl;
    }
}

void YangModuleCollection::printYinForAllModulesToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYinForAllModules (pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYinForAllModules (pFile);
    }
}

void YangModuleCollection::printYinForModule (const string &moduleName, FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    YangModule *pYangModule = getYangModuleByName (moduleName);

    if (NULL != pYangModule)
    {

        cout << "Printing YIN for module " << moduleName << " ... ";

        pYangModule->printYin (0, true, pFile);

        cout << "Done." << endl;
    }
}

void YangModuleCollection::printYinForModuleToFile (const string &moduleName, const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printYinForModule (moduleName, pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printYinForModule (moduleName, pFile);
    }
}

void YangModuleCollection::getTargetNodeDetails (const string &qualifiedYangPath, string &moduleName, string &targetNodePath) const
{
    vector<string> qualifiedYangPathLevel1Tokens;
    UI32           numberOfLevel1TokensInQualifiedYangPath = 0;
    UI32           i                                       = 0;

    moduleName     = "";
    targetNodePath = "";

    StringUtils::tokenize (qualifiedYangPath, qualifiedYangPathLevel1Tokens, '/');

    numberOfLevel1TokensInQualifiedYangPath = qualifiedYangPathLevel1Tokens.size ();

    for (i = 0; i < numberOfLevel1TokensInQualifiedYangPath; i++)
    {
        vector<string> qualifiedYangPathLevel2Tokens;
        UI32           numberOfLevel2TokensInQualifiedYangPath = 0;

        StringUtils::tokenize (qualifiedYangPathLevel1Tokens[i], qualifiedYangPathLevel2Tokens, ':');

        numberOfLevel2TokensInQualifiedYangPath = qualifiedYangPathLevel2Tokens.size ();

        prismAssert (1 <= numberOfLevel2TokensInQualifiedYangPath, __FILE__, __LINE__);
        prismAssert (2 >= numberOfLevel2TokensInQualifiedYangPath, __FILE__, __LINE__);

        if (2 == numberOfLevel2TokensInQualifiedYangPath)
        {
            targetNodePath += "/" + qualifiedYangPathLevel2Tokens[1];

            if (0 == i)
            {
                moduleName = qualifiedYangPathLevel2Tokens[0];
            }
        }
        else
        {
            targetNodePath += "/" + qualifiedYangPathLevel2Tokens[0];
        }
    }
}

void YangModuleCollection::getAllNames (set<string> &allNames) const
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->getAllNames (allNames);
    }
}

void YangModuleCollection::printAllNames (FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    set<string> allNames;

    getAllNames (allNames);

    set<string>::const_iterator element    = allNames.begin ();
    set<string>::const_iterator endElement = allNames.end   ();

    while (endElement != element)
    {
        fprintf (pFile, "%s\n", (*element).c_str ());

        element++;
    }
}

void YangModuleCollection::printAllNamesToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printAllNames (pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printAllNames (pFile);
    }
}

void YangModuleCollection::getAllCliTargetNodeNamesForData (vector<string> &allCliTargetNodeNamesForData) const
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->getAllCliTargetNodeNamesForData (allCliTargetNodeNamesForData);
    }
}

void YangModuleCollection::printAllCliTargetNodeNamesForData (FILE *pFile) const
{
    if (NULL == pFile)
    {
        pFile = stdout;
    }

    vector<string> allCliTargetNodeNamesForData;

    getAllCliTargetNodeNamesForData (allCliTargetNodeNamesForData);

    vector<string>::const_iterator element    = allCliTargetNodeNamesForData.begin ();
    vector<string>::const_iterator endElement = allCliTargetNodeNamesForData.end   ();

    while (endElement != element)
    {
        fprintf (pFile, "%s\n", (*element).c_str ());

        element++;
    }
}

void YangModuleCollection::printAllCliTargetNodeNamesForDataToFile (const string &filePath) const
{
    FILE *pFile = NULL;

    if ("" != filePath)
    {
        pFile = fopen (filePath.c_str (), "w");

        if (NULL != pFile)
        {
            printAllCliTargetNodeNamesForData (pFile);

            fclose (pFile);
        }
    }
    else
    {
        pFile = stdout;

        printAllCliTargetNodeNamesForData (pFile);
    }
}

void YangModuleCollection::loadUserTagsFromFile (const string &userTagsFilePath)
{
    ifstream       userTagsFileStream   (userTagsFilePath.c_str ());
    char           *pLine             = NULL;
    string          line;
    vector<string> tokens;
    UI32           numberOfTokens     = 0;

    pLine = (char *) malloc (sizeof (char) * 1024);

    prismAssert (NULL != pLine, __FILE__, __LINE__);

    if (false == (userTagsFileStream.is_open ()))
    {
        trace (TRACE_LEVEL_ERROR, "YangModuleCollection::loadUserTagsFromFile : " + userTagsFilePath + " could not be opened.");

		free (pLine);
        return;
    }


    while (false == (userTagsFileStream.eof ()))
    {
        getline (userTagsFileStream, line);

        tokens.clear ();
        StringUtils::tokenize (line, tokens, ' ');

        numberOfTokens = tokens.size ();

        if (2 == numberOfTokens)
        {
            string userTagName  = tokens[0];
            UI32   userTagValue = strtoul (tokens[1].c_str (), NULL, 10);

            addUserTag (userTagName, userTagValue);
        }
    }

	free (pLine);
    userTagsFileStream.close ();

    trace (TRACE_LEVEL_INFO, string ("YangModuleCollection::loadUserTagsFromFile : Number Of loaded User Tags : ") + m_userTagsMap.size ());
}

void YangModuleCollection::addUserTag (const string &userTagName, const UI32 &userTagValue)
{
    m_userTagsMap[userTagName] = userTagValue;
}

UI32 YangModuleCollection::getUserTagValueByName (const string &userTagName) const
{
    map<string, UI32>::const_iterator element      = m_userTagsMap.find (userTagName);
    map<string, UI32>::const_iterator endElement   = m_userTagsMap.end  ();
    UI32                              userTagValue = 0;

    if (endElement != element)
    {
        userTagValue = element->second;
    }

    return (userTagValue);
}

void YangModuleCollection::updateModulesWithUserTags ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->updateUserTags (this);
    }
}

void YangModuleCollection::computeConfigurationSegmentNames ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->computeConfigurationSegmentNames ();
    }
}

YangElement *YangModuleCollection::getYangElementByQualifiedYangPath (const string &qualifiedYangPath) const
{
    string moduleName;
    string targetNodePath;

    getTargetNodeDetails (qualifiedYangPath, moduleName, targetNodePath);

    YangModule *pYangModule   = getYangModuleByName (moduleName);
    YangElement *pYangElement = NULL;

    if (NULL != pYangModule)
    {
        pYangElement = pYangModule->getYangElementByTargetNodePath (targetNodePath);
    }

    return (pYangElement);
}

YangElement *YangModuleCollection::getYangElementByPrefixQualifiedYangPath (const string &prefixQualifiedYangPath) const
{
    string modulePrefix;
    string targetNodePath;

    getTargetNodeDetails (prefixQualifiedYangPath, modulePrefix, targetNodePath);

    YangModule *pYangModule   = getYangModuleByPrefix (modulePrefix);
    YangElement *pYangElement = NULL;

    if (NULL != pYangModule)
    {
        pYangElement = pYangModule->getYangElementByTargetNodePath (targetNodePath);
    }

    return (pYangElement);
}

void YangModuleCollection::computeCliTargetNodeNames ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->computeCliTargetNodeNames ();
    }
}

// TODO Get this information registered from libDcm.

void YangModuleCollection::markNodeSpecificAndPartitionBaseYangElements ()
{
    do
    {
        YangElement *pYangModuleForNodeSpecific = getYangModuleByName ("brocade-rbridge");

        if (NULL == pYangModuleForNodeSpecific)
        {
            break;
        }
        // prismAssert (NULL != pYangModuleForNodeSpecific, __FILE__, __LINE__); // rolling reboot.

        YangElement *pYangElementForNodeSpecificList = pYangModuleForNodeSpecific->getYangElementForTargetNode ("/rbridge-id");

        if (NULL == pYangElementForNodeSpecificList)
        {
            break;
        }
        // prismAssert (NULL != pYangElementForNodeSpecificList, __FILE__, __LINE__);

        YangDataElement *pYangDataElementForNodeSpecificList = dynamic_cast<YangDataElement *> (pYangElementForNodeSpecificList);

        if (NULL == pYangDataElementForNodeSpecificList)
        {
            break;
        }
        // prismAssert (NULL != pYangDataElementForNodeSpecificList, __FILE__, __LINE__);

        pYangDataElementForNodeSpecificList->setIsNodeSpecificBaseList ();

        WaveConfigurationSegmentMap::setConfigurationSegmentNameForNodeSpecificList (pYangDataElementForNodeSpecificList->getConfigurationSegmentName ());

        YangElement *pYangElementForMultiPartitionList = pYangModuleForNodeSpecific->getYangElementForTargetNode ("/rbridge-id/vrf");

        if (NULL == pYangElementForMultiPartitionList)
        {
            break;
        }
        // prismAssert (NULL != pYangElementForMultiPartitionList, __FILE__, __LINE__);

        YangDataElement *pYangDataElementForMultiPartitionList = dynamic_cast<YangDataElement *> (pYangElementForMultiPartitionList);

        if (NULL == pYangDataElementForMultiPartitionList)
        {
            break;
        }
        // prismAssert (NULL != pYangDataElementForMultiPartitionList, __FILE__, __LINE__);

        pYangDataElementForMultiPartitionList->setIsMultiPartitionBaseList ();

        WaveConfigurationSegmentMap::setConfigurationSegmentNameForMultiPartitionList (pYangDataElementForMultiPartitionList->getConfigurationSegmentName ());
    }
    while (0);
}

void YangModuleCollection::processTypeInformations ()
{
    UI32 numberOfYangModules = m_yangModules.size ();

    for (UI32 i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];
        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->processTypeInformations ();
    }
}

void YangModuleCollection::inlineTypedef ()
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->inlineTypedefInsideModule ();
    }

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->inlineTypedefFromImportedModule (this);
    }
}

UI32 YangModuleCollection::getNumberOfAllChildYangElementsInAllModules () const
{
    UI32 numberOfAllChildElementsInTree = 0;
    UI32 numberOfYangModules            = m_yangModules.size ();

    for (UI32 i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];
        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        numberOfAllChildElementsInTree += pYangModule->getNumberOfAllChildYangElementsInTree ();
    }

    return numberOfAllChildElementsInTree + numberOfYangModules;
}

void YangModuleCollection::setOriginalModuleNameSpaceUriInAllModules ()
{
    UI32 numberOfYangModules            = m_yangModules.size ();

    for (UI32 i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];
        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        pYangModule->setOriginalModuleNameSpaceUriForTree ();
    }

    // std::stable_sort (m_yangModules.begin (), m_yangModules.end (), YangModule::compareYangModulesByNamespaceUri);
}

void YangModuleCollection::generateHFilesForCLanguageForAllModules () const
{
    UI32 numberOfYangModules = m_yangModules.size ();
    UI32 i                   = 0;

    cout << "Generating H (C) for all modules ..." << endl;

    for (i = 0; i < numberOfYangModules; i++)
    {
        YangModule *pYangModule = m_yangModules[i];

        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        string moduleName = pYangModule->getName ();

        cout << "    Generating H (C) file for " << moduleName << " ... ";

        pYangModule->generateHFileForCLanguage (*this);

        cout << "Done." << endl;
    }
}

}
