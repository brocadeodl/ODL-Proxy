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

#include "Modeling/YANG/ObjectModel/YangModule.h"
#include "Framework/Utils/AssertUtils.h"
#include "Modeling/YANG/ObjectModel/YangFactory/YangElementFactory.h"
#include "Modeling/YANG/ObjectModel/YangImport.h"
#include "Modeling/YANG/ObjectModel/YangGrouping.h"
#include "Modeling/YANG/ObjectModel/YangPrefix.h"
#include "Modeling/YANG/ObjectModel/YangTypedef.h"
#include "Modeling/YANG/ObjectModel/YangNameSpace.h"
#include "Modeling/YANG/ObjectModel/YangModuleCollection.h"

#include <strings.h>

namespace WaveNs
{

const string YangModule::s_xmlnsTokenPrefix = "xmlns:";

YangModule::YangModule ()
    : YangElement (getYangName (), "")
{
    setPOwnerYangModule (this);
}

YangModule::YangModule (const string &name)
    : YangElement (getYangName (), name)
{
    setPOwnerYangModule (this);
}

YangModule::~YangModule()
{
}

string YangModule::getYangName ()
{
    return ("module");
}

YangElement *YangModule::createInstance ()
{
    return  (new YangModule ());
}

void YangModule::processAttribute (const string &attributeName, const string &attributeValue)
{
    YangElement::processAttribute (attributeName, attributeValue);

    if ("name" == attributeName)
    {
        setPrefix (attributeValue);
    }
    else
    {
        if (s_xmlnsTokenPrefix.size () < attributeName.size ())
        {
            if (0 == attributeName.compare (0, s_xmlnsTokenPrefix.size (), s_xmlnsTokenPrefix))
            {
                string prefixString = attributeName.substr (s_xmlnsTokenPrefix.size (), string::npos);

                prismAssert (false == prefixString.empty (), __FILE__, __LINE__);

                addNamespace (prefixString, attributeValue);
            }
        }
    }
}

void YangModule::processChildElement (YangElement *pYangElement)
{
    YangElement::processChildElement (pYangElement);

    const string yangName = pYangElement->getYangName ();

    if ((YangImport::getYangName()) == yangName)
    {
        YangImport *pYangImport = dynamic_cast<YangImport *> (pYangElement);

        prismAssert (NULL != pYangImport, __FILE__, __LINE__);

        const string importedModuleName   = pYangImport->getModule ();
        const string importedModulePrefix = pYangImport->getPrefix ();

        addImportedModuleDetails (importedModulePrefix, importedModuleName);
    }
    else if ((YangGrouping::getYangName()) == yangName)
    {
        YangGrouping *pYangGrouping = dynamic_cast<YangGrouping *> (pYangElement);

        prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

        addGrouping (pYangGrouping);
    }
    else if ((YangTypedef::getYangName()) == yangName)
    {
        YangTypedef *pYangTypedef = dynamic_cast<YangTypedef *> (pYangElement);

        prismAssert (NULL != pYangTypedef, __FILE__, __LINE__);

        addTypedef (pYangTypedef);
    }
    else if ((YangPrefix::getYangName()) == yangName)
    {
        YangPrefix *pYangPrefix = dynamic_cast<YangPrefix *> (pYangElement);

        prismAssert (NULL != pYangPrefix, __FILE__, __LINE__);

        string prefixValue;

        pYangPrefix->getAttributeValue ("value", prefixValue);

        prismAssert ("" != prefixValue, __FILE__, __LINE__);

        setPrefix (prefixValue);
    }
    else if ((YangNameSpace::getYangName()) == yangName)
    {
        YangNameSpace *pYangNameSpace = dynamic_cast<YangNameSpace *> (pYangElement);

        prismAssert (NULL != pYangNameSpace, __FILE__, __LINE__);

        m_namespaceUri = pYangNameSpace->getUri ();
    }
}

string YangModule::getImportedModuleName (const string &importedModulePrefix) const
{
    map<string, string>::const_iterator element    = m_importedModulesMap.find (importedModulePrefix);
    map<string, string>::const_iterator endElement = m_importedModulesMap.end  ();

    if (endElement != element)
    {
        return (element->second);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    return ("");
}

void YangModule::addImportedModuleDetails (const string &importedModulePrefix, const string &importedModuleName)
{
    bool isKnownPrefix = isAKnownImportedModulePrefix (importedModulePrefix);

    if (false == isKnownPrefix)
    {
        m_importedModulesMap[importedModulePrefix] = importedModuleName;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

bool YangModule::isAKnownImportedModulePrefix (const string &importedModulePrefix) const
{
    map<string, string>::const_iterator element    = m_importedModulesMap.find (importedModulePrefix);
    map<string, string>::const_iterator endElement = m_importedModulesMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool YangModule::isAKnownGrouping (const string &groupingName) const
{
    map<string, YangGrouping *>::const_iterator element    = m_groupingsMap.find (groupingName);
    map<string, YangGrouping *>::const_iterator endElement = m_groupingsMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void YangModule::addGrouping (YangGrouping *pYangGrouping)
{
    prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

    const string groupingName = pYangGrouping->getName ();
    const bool   isKnown      = isAKnownGrouping (groupingName);

    prismAssert (false == isKnown, __FILE__, __LINE__);

    m_groupingsMap[groupingName] = pYangGrouping;
}

YangGrouping *YangModule::getGrouping (const string &groupingName) const
{
    const bool          isKnown       = isAKnownGrouping (groupingName);
          YangGrouping *pYangGrouping = NULL;

    if (true == isKnown)
    {
        map<string, YangGrouping *>::const_iterator element = m_groupingsMap.find (groupingName);

        pYangGrouping = element->second;
    }

    prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

    return (pYangGrouping);
}

void YangModule::incrementUsageCountForGrouping (const string &groupingName)
{
    YangGrouping *pYangGrouping = getGrouping (groupingName);

    prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

    pYangGrouping->incrementUsageCount ();
}

void YangModule::incrementUsageCountForGroupingForProgrammingLanguages (const YangModuleCollection *pYangModuleCollection, const string &groupingName)
{
    YangGrouping *pYangGrouping = getGrouping (groupingName);

    prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

    pYangGrouping->incrementUsageCountForGroupingsInHierarchyForProgrammingLanguages (pYangModuleCollection);
}

void YangModule::getClonedChildrenForGrouping (const string &groupingName, vector<YangElement *> &clonedChildrenForGrouping) const
{
    YangGrouping *pYangGrouping = getGrouping (groupingName);

    prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

    pYangGrouping->getClonedChildren (clonedChildrenForGrouping);
}

void YangModule::removeAllUnusedGroupings ()
{
    vector<YangElement *> allChildElementsByYangName;
    UI32                  numberOfChildElementsByYangName = 0;
    UI32                  i                               = 0;

    getAllChildrenByYangName (YangGrouping::getYangName (), allChildElementsByYangName);

    numberOfChildElementsByYangName = allChildElementsByYangName.size ();

    cout << "  Processing " << getName () << " for unused groupings removal ..." << endl;

    for (i = 0; i < numberOfChildElementsByYangName; i++)
    {
        YangGrouping *pYangGrouping = dynamic_cast<YangGrouping *> (allChildElementsByYangName[i]);

        prismAssert (NULL != pYangGrouping, __FILE__, __LINE__);

        if (0 == (pYangGrouping->getUsageCount ()))
        {
            cout << "    Removing unused grouping " << pYangGrouping->getName () << endl;

            removeChildElement (allChildElementsByYangName[i]);
            delete (allChildElementsByYangName[i]);
        }
    }
}

void YangModule::removeAllGroupings ()
{
    removeAllChildElementsByYangName (YangGrouping::getYangName ());
}

void YangModule::computeCliTargetNodeNameForSelf ()
{
    setCliTargetNodeName ("");
}

string YangModule::getPrefix () const
{
    return (m_prefix);
}

void YangModule::setPrefix (const string &prefix)
{
    m_prefix = prefix;
}

bool YangModule::isAKnownTypedef (const string &typedefName) const
{
    map<string, YangTypedef *>::const_iterator element    = m_typedefsMap.find (typedefName);
    map<string, YangTypedef *>::const_iterator endElement = m_typedefsMap.end  ();

    if (endElement != element)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

void YangModule::addTypedef (YangTypedef *pYangTypedef)
{
    prismAssert (NULL != pYangTypedef, __FILE__, __LINE__);

    const string typedefName = pYangTypedef->getName ();
    const bool   isKnown     = isAKnownTypedef (typedefName);

    prismAssert (false == isKnown, __FILE__, __LINE__);

    m_typedefsMap[typedefName] = pYangTypedef;
}


YangTypedef *YangModule::getTypedef (const string &typedefName) const
{
    const bool    isKnown       = isAKnownTypedef (typedefName);
    YangTypedef  *pYangTypedef  = NULL;

    if (true == isKnown)
    {
        map<string, YangTypedef *>::const_iterator element = m_typedefsMap.find (typedefName);

        pYangTypedef = element->second;
    }

    prismAssert (NULL != pYangTypedef, __FILE__, __LINE__);

    return (pYangTypedef);
}

void YangModule::getClonedChildrenForTypedef (string &typedefName, vector<YangElement *> &clonedChildrenForTypedef) const
{
    YangTypedef *pYangTypedef = getTypedef ( typedefName);

    prismAssert (NULL != pYangTypedef, __FILE__, __LINE__);

    pYangTypedef->getClonedChildren (clonedChildrenForTypedef);
}


void YangModule::removeAllTypedefs ()
{
    removeAllChildElementsByYangName (YangTypedef::getYangName ());
}

void YangModule::addNamespace (const string &namespacePrefix, const string &namespaceString)
{
    // Assertion checks begin   [

    prismAssert (false == namespacePrefix.empty (), __FILE__, __LINE__);
    prismAssert (false == namespaceString.empty (), __FILE__, __LINE__);

    map<string, string>::const_iterator itr = m_namespacePrefixToNamespaceMap.find (namespacePrefix);
    map<string, string>::const_iterator end = m_namespacePrefixToNamespaceMap.end ();
    prismAssert (end == itr, __FILE__, __LINE__);

    // Assertion checks end     ]

    m_namespacePrefixToNamespaceMap[namespacePrefix] = namespaceString;
}

string YangModule::getNamespaceStringForPrefix (const string &namespacePrefix) const
{
    map<string, string>::const_iterator itr = m_namespacePrefixToNamespaceMap.find (namespacePrefix);
    map<string, string>::const_iterator end = m_namespacePrefixToNamespaceMap.end ();

    prismAssert (end != itr, __FILE__, __LINE__);

    return itr->second;
}

string YangModule::getNamespaceUri () const
{
    vector<YangElement *> pNameSpaceChildElements;

    getAllChildrenByYangName (YangNameSpace::getYangName (), pNameSpaceChildElements);

    if (0 < pNameSpaceChildElements.size ())
    {
        YangNameSpace *pYangNameSpace = dynamic_cast<YangNameSpace *> (pNameSpaceChildElements[0]);
        prismAssert (NULL != pYangNameSpace, __FILE__, __LINE__);

        return pYangNameSpace->getUri ();
    }
    else
    {
        return "";
    }
}

// Used by sort algorithm.
// static

bool YangModule::compareYangModulesByNamespaceUri (YangModule *pYangModule1, YangModule *pYangModule2)
{
    prismAssert (NULL != pYangModule1, __FILE__, __LINE__);
    prismAssert (NULL != pYangModule2, __FILE__, __LINE__);

    string namespaceUri1 = pYangModule1->getNamespaceUri ();
    string namespaceUri2 = pYangModule2->getNamespaceUri ();

    return (namespaceUri1 < namespaceUri2);
}

void YangModule::generateHFileForCLanguage (const YangModuleCollection &yangModuleCollection) const
{
    string cLanguageFileName = getHFileNameForCLanguageWithoutDirectory ();

    cout << endl << "    Using File Name : " << cLanguageFileName << ", Namespace : " << m_namespaceUri << " ... ";

    printAllNamesToFileForHFileForCLanguage (cLanguageFileName, yangModuleCollection, m_prefix);
}

string YangModule::getFileName () const
{
    return (m_fileName);
}

void YangModule::setFileName (const string &fileName)
{
    m_fileName = fileName;
}

string YangModule::getHFileNameForCLanguageWithoutDirectory () const
{
    vector<string> fileNameTokens;
    string         fileName;

    StringUtils::tokenize (m_fileName, fileNameTokens, '/');

    UI32 numberOfFileNameTokens = fileNameTokens.size ();

    if (0 != numberOfFileNameTokens)
    {
        fileName = fileNameTokens[numberOfFileNameTokens - 1];

        StringUtils::stripFromLastOccuranceOf (fileName, '.');

        fileName += ".h";
    }

    if ("" == fileName)
    {
        fileName = getName () + ".h";
    }

    return (fileName);
}

void YangModule::getPreifxAndPostfixStringsForHFileForCLanguage (string &prefix, string &postfix, const YangModuleCollection &yangModuleCollection)
{
    string multipleInclusionPreventionMacro = getHFileNameForCLanguageWithoutDirectory ();

    StringUtils::replaceAllInstancesOfInputCharWith (multipleInclusionPreventionMacro, '-', '_');
    StringUtils::replaceAllInstancesOfInputCharWith (multipleInclusionPreventionMacro, '.', '_');
    StringUtils::convertToUpperCase                 (multipleInclusionPreventionMacro);

    prefix  = "#ifndef " + multipleInclusionPreventionMacro + "\n";
    prefix += "#define " + multipleInclusionPreventionMacro + "\n";
    prefix += "\n";

    prefix += "#ifdef __cplusplus\n";
    prefix += "extern \"C\"\n";
    prefix += "{\n";
    prefix += "#endif\n";
    prefix += "\n";

    string namespaceMacro = m_prefix;

    if ("" == namespaceMacro)
    {
        namespaceMacro = getName ();
    }

    StringUtils::replaceAllInstancesOfInputCharWith (namespaceMacro, '-', '_');

    namespaceMacro += "__ns";

    prefix += "#define " + namespaceMacro + " " + yangModuleCollection.getUserTagValueByName (m_namespaceUri) + "\n";
    prefix += "\n";

    postfix += "\n";
    postfix += "#ifdef __cplusplus\n";
    postfix += "}\n";
    postfix += "#endif\n";
    postfix += "\n";

    postfix += "#endif\n";
    postfix += "\n";
}

}
