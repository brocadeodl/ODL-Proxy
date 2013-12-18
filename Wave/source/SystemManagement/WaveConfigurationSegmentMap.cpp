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

#include "SystemManagement/WaveConfigurationSegmentMap.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"

#include <algorithm>

namespace WaveNs
{

map<string, ConfigurationSegmentInformation *>          WaveConfigurationSegmentMap::m_configurationSegmentMap;
PrismMutex                                              WaveConfigurationSegmentMap::m_configurationSegmentMapMutex;

ConfigurationSegmentQueryContextModificationFunction    WaveConfigurationSegmentMap::s_setNTupleSortingFunction     = NULL;
ConfigurationSegmentQueryContextModificationFunction    WaveConfigurationSegmentMap::s_customQueryFilterFunction    = NULL;
ConfigurationSegmentQueryContextModificationFunction2   WaveConfigurationSegmentMap::s_addCustomOrderFieldFunction  = NULL;
PrismMutex                                              WaveConfigurationSegmentMap::s_queryContextModificationFunctionsMutex;

string                                                  WaveConfigurationSegmentMap::s_configurationSegmentForNodeSpecificList      = "";
string                                                  WaveConfigurationSegmentMap::s_configurationSegmentForMultiPartitionList    = "";
string                                                  WaveConfigurationSegmentMap::s_managedObjectClassNameForNodeSpecificList    = "";
string                                                  WaveConfigurationSegmentMap::s_managedObjectClassNameForMultiPartitionList  = "";
map<string, ConfigurationSegmentImportantInfo *>        WaveConfigurationSegmentMap::m_configSegmentImportantInfoMap;
PrismMutex                                              WaveConfigurationSegmentMap::m_configSegmentImportantInfoMapMutex;
// ConfigurationSegmentInformation :

ConfigurationSegmentInformation::ConfigurationSegmentInformation (const string &managedObjectClassName, PrismElement *pPrismElement)
    : m_managedObjectClassName          (managedObjectClassName),
      m_pPrismElement                   (pPrismElement),
      m_isSingleton                     (false),
      m_nodeSpecificSupported           (false),
      m_partitionSupported              (false),
      m_isLocalManagedObject            (false)
{
    m_userDefinedKeyCombinationToPAttributeMap = OrmRepository::getUserDefinedKeyCombinationWithTypesForTable (m_managedObjectClassName, m_userDefinedKeyCombination);
}

ConfigurationSegmentInformation::~ConfigurationSegmentInformation ()
{
}

string ConfigurationSegmentInformation::getManagedObjectClassName () const
{
    return m_managedObjectClassName;
}

void ConfigurationSegmentInformation::setParameters (const bool &isSingleton, const bool &nodeSpecificSupported, const bool &partitionSupported)
{
    m_isSingleton           = isSingleton;
    m_nodeSpecificSupported = nodeSpecificSupported;
    m_partitionSupported    = partitionSupported;
}

bool ConfigurationSegmentInformation::getIsSingleton () const
{
    return m_isSingleton;
}

bool ConfigurationSegmentInformation::getIsNodeSpecificSupported () const
{
    return m_nodeSpecificSupported;
}

bool ConfigurationSegmentInformation::getPartitionSupported () const
{
    return m_partitionSupported;
}

void ConfigurationSegmentInformation::setParentInformation (const vector<string> &parentManagedObjectClassNames, const vector<bool> &parentIsSingletonFlags, const vector<bool> &parentIsSkippedForDisplay)
{
    m_parentManagedObjectClassNames     = parentManagedObjectClassNames;
    m_parentIsSingletonFlags            = parentIsSingletonFlags;
    m_parentIsSkippedForDisplay         = parentIsSkippedForDisplay;

    std::reverse (m_parentManagedObjectClassNames.begin (), m_parentManagedObjectClassNames.end ());
    std::reverse (m_parentIsSingletonFlags.begin (),        m_parentIsSingletonFlags.end ());
    std::reverse (m_parentIsSkippedForDisplay.begin (),     m_parentIsSkippedForDisplay.end ());
}

void ConfigurationSegmentInformation::setIsLocalManagedObject (const bool &isLocalManagedObject)
{
    m_isLocalManagedObject = isLocalManagedObject;
}

bool ConfigurationSegmentInformation::getIsLocalManagedObject () const
{
    return m_isLocalManagedObject;
}

void ConfigurationSegmentInformation::debugPrint () const
{
    string isSingletonValue             = m_isSingleton             ? "TRUE" : "FALSE";
    string nodeSpecificSupportedValue   = m_nodeSpecificSupported   ? "TRUE" : "FALSE";
    string partitionSupportedValue      = m_partitionSupported      ? "TRUE" : "FALSE";

    tracePrintf (TRACE_LEVEL_INFO, true, true, "        ManagedObject %s : isSingleton %s : nodeSpecificSupported %s : partitionSupported %s", m_managedObjectClassName.c_str (), isSingletonValue.c_str (), nodeSpecificSupportedValue.c_str (), partitionSupportedValue.c_str ());

    UI32 numberOfManagedObjectClassNamesInOrmHierarchy  = getNumberOfManagedObjectClassNamesInOrmHierarchy ();

    for (UI32 i = 0; i < numberOfManagedObjectClassNamesInOrmHierarchy; i++)
    {
        string parentIsSingletonValue           = m_parentIsSingletonFlags[i]       ? "TRUE" : "FALSE";
        string parentIsSkippedForDisplayValue   = m_parentIsSkippedForDisplay[i]    ? "TRUE" : "FALSE";

        tracePrintf (TRACE_LEVEL_INFO, true, true, "          parent %u. %s : isSingleton %s : isSkippedForDisplay %s", i + 1, m_parentManagedObjectClassNames[i].c_str (), parentIsSingletonValue.c_str (), parentIsSkippedForDisplayValue.c_str ());
    }
}

UI32 ConfigurationSegmentInformation::getNumberOfManagedObjectClassNamesInOrmHierarchy () const
{
    UI32 numberOfManagedObjectClassNamesInOrmHierarchy = m_parentManagedObjectClassNames.size ();

    return numberOfManagedObjectClassNamesInOrmHierarchy;
}

bool ConfigurationSegmentInformation::getParentIsSingletonAtIndex (const UI32 &index) const
{
    UI32 numberOfManagedObjectClassNamesInOrmHieararchy = m_parentManagedObjectClassNames.size ();

    prismAssert (index < numberOfManagedObjectClassNamesInOrmHieararchy, __FILE__, __LINE__);

    return m_parentIsSingletonFlags[index];
}

string ConfigurationSegmentInformation::getParentManagedObjectClassNameAtIndex (const UI32 &index) const
{
    UI32 numberOfManagedObjectClassNamesInOrmHieararchy = m_parentManagedObjectClassNames.size ();

    prismAssert (index < numberOfManagedObjectClassNamesInOrmHieararchy, __FILE__, __LINE__);

    return m_parentManagedObjectClassNames[index];
}

void ConfigurationSegmentInformation::applySetNTupleSortingFunction (WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    if (NULL != m_pPrismElement)
    {
        WaveConfigurationSegmentMap::applySetNTupleSortingFunction (m_pPrismElement, waveManagedObjectQueryContextBase);
    }
}

void ConfigurationSegmentInformation::applyCustomQueryFilterFunction (WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    if (NULL != m_pPrismElement)
    {
        WaveConfigurationSegmentMap::applyCustomQueryFilterFunction (m_pPrismElement, waveManagedObjectQueryContextBase);
    }
}

bool ConfigurationSegmentInformation::applyAddCustomOrderFieldFunction (WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    bool returnValue = false;

    if (NULL != m_pPrismElement)
    {
        returnValue = WaveConfigurationSegmentMap::applyAddCustomOrderFieldFunction (m_pPrismElement, waveManagedObjectQueryContextBase);
    }

    return returnValue;
}

const vector<string> &ConfigurationSegmentInformation::getUserDefinedKeyCombination () const
{
    return m_userDefinedKeyCombination;
}

AttributeType ConfigurationSegmentInformation::getAttributeTypeForAttributeInKeyCombination (const string &keyString) const
{
    map<string, Attribute *>::const_iterator itr = m_userDefinedKeyCombinationToPAttributeMap.find (keyString);
    map<string, Attribute *>::const_iterator end = m_userDefinedKeyCombinationToPAttributeMap.end  ();

    prismAssert (end != itr, __FILE__, __LINE__);

    Attribute *pAttribute = itr->second;
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    AttributeType keyAttributeType = pAttribute->getAttributeType ();

    return keyAttributeType;
}

Attribute *ConfigurationSegmentInformation::getClonedAttributeForKey (const string &keyString) const
{
    map<string, Attribute *>::const_iterator itr = m_userDefinedKeyCombinationToPAttributeMap.find (keyString);
    map<string, Attribute *>::const_iterator end = m_userDefinedKeyCombinationToPAttributeMap.end  ();

    prismAssert (end != itr, __FILE__, __LINE__);

    Attribute *pAttribute = itr->second;
    prismAssert (NULL != pAttribute, __FILE__, __LINE__);

    Attribute *pClonedAttribute = pAttribute->clone ();
    prismAssert (NULL != pClonedAttribute, __FILE__, __LINE__);

    return pClonedAttribute;
}


// WaveConfigurationSegmentMap :

bool WaveConfigurationSegmentMap::isAKnownConfigurationSegmentName (const string &configurationSegmentName)
{
    map<string, ConfigurationSegmentInformation *>::const_iterator  element    = m_configurationSegmentMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement = m_configurationSegmentMap.end  ();
    bool                                                            isKnown    = false;

    if (endElement != element)
    {
        isKnown = true;
    }

    return (isKnown);
}

void WaveConfigurationSegmentMap::addConfigurationSegmentInformation (const string &configurationSegmentName, const string &managedObjectClassName, PrismElement *pPrismElement)
{
    m_configurationSegmentMapMutex.lock ();

    bool isAlreadyKnown = isAKnownConfigurationSegmentName (configurationSegmentName);

    prismAssert (false == isAlreadyKnown, __FILE__, __LINE__);

    trace (TRACE_LEVEL_DEVEL, "Adding configuration Segment Map " + configurationSegmentName + " : " + managedObjectClassName);

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = new ConfigurationSegmentInformation (managedObjectClassName, pPrismElement);

    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    m_configurationSegmentMap[configurationSegmentName] = pConfigurationSegmentInformation;

    m_configurationSegmentMapMutex.unlock ();
}

string WaveConfigurationSegmentMap::getManagedObjectClassNameByConfigurationSegmentName (const string &configurationSegmentName)
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::const_iterator  element                 = m_configurationSegmentMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement              = m_configurationSegmentMap.end  ();
    string                                                          managedObjectClassName;

    if (endElement != element)
    {
        ConfigurationSegmentInformation *pConfigurationSegmentInformation = element->second;
        prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

        managedObjectClassName = pConfigurationSegmentInformation->getManagedObjectClassName ();
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_configurationSegmentMapMutex.unlock ();

    return (managedObjectClassName);
}

void WaveConfigurationSegmentMap::print ()
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::const_iterator  element                   = m_configurationSegmentMap.begin ();
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement                = m_configurationSegmentMap.end   ();
    string                                                          configurationSegmentName;
    string                                                          managedObjectClassName;
    UI32                                                            i                         = 1;

    trace (TRACE_LEVEL_INFO, string ("Wave Configuration Segment Map contains ") + (m_configurationSegmentMap.size ()) + " entries.  They are listed below :");

    while (endElement != element)
    {
        configurationSegmentName = element->first;

        ConfigurationSegmentInformation *pConfigurationSegmentInformation = element->second;
        prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);
        managedObjectClassName   = pConfigurationSegmentInformation->getManagedObjectClassName ();

        trace (TRACE_LEVEL_INFO, string ("    ") +  i + string (". ") + configurationSegmentName + " : " + managedObjectClassName);
        pConfigurationSegmentInformation->debugPrint ();

        element++;
        i++;
    }

    m_configurationSegmentMapMutex.unlock ();
}

void WaveConfigurationSegmentMap::getAllConfigurationSegmentNamesAndManagedObjectClassNames (vector<string> &configurationSegmentNames, vector<string> &managedObjectClassNames)
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::const_iterator  element                   = m_configurationSegmentMap.begin ();
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement                = m_configurationSegmentMap.end   ();
    string                                                          configurationSegmentName;
    string                                                          managedObjectClassName;

    while (endElement != element)
    {
        configurationSegmentName = element->first;

        ConfigurationSegmentInformation *pConfigurationSegmentInformation = element->second;
        prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);
        managedObjectClassName   = pConfigurationSegmentInformation->getManagedObjectClassName ();

        configurationSegmentNames.push_back (configurationSegmentName);
        managedObjectClassNames.push_back (managedObjectClassName);

        element++;
    }

    m_configurationSegmentMapMutex.unlock ();
}

void WaveConfigurationSegmentMap::registerSetNTupleSortingFunction (ConfigurationSegmentQueryContextModificationFunction setNTupleSortingFunction)
{
    s_queryContextModificationFunctionsMutex.lock ();
    s_setNTupleSortingFunction  = setNTupleSortingFunction;
    s_queryContextModificationFunctionsMutex.unlock ();
}

void WaveConfigurationSegmentMap::registerCustomQueryFilterFunction (ConfigurationSegmentQueryContextModificationFunction customQueryFilterFunction)
{
    s_queryContextModificationFunctionsMutex.lock ();
    s_customQueryFilterFunction = customQueryFilterFunction;
    s_queryContextModificationFunctionsMutex.unlock ();
}

void WaveConfigurationSegmentMap::registerAddCustomOrderFieldFunction (ConfigurationSegmentQueryContextModificationFunction2 addCustomOrderFieldFunction)
{
    s_queryContextModificationFunctionsMutex.lock ();
    s_addCustomOrderFieldFunction   = addCustomOrderFieldFunction;
    s_queryContextModificationFunctionsMutex.unlock ();
}

void WaveConfigurationSegmentMap::applySetNTupleSortingFunction (PrismElement *pPrismElement, WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    s_queryContextModificationFunctionsMutex.lock ();

    if (NULL != s_setNTupleSortingFunction)
    {
        (*s_setNTupleSortingFunction) (pPrismElement, waveManagedObjectQueryContextBase);
    }

    s_queryContextModificationFunctionsMutex.unlock ();
}

void WaveConfigurationSegmentMap::applyCustomQueryFilterFunction (PrismElement *pPrismElement, WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    s_queryContextModificationFunctionsMutex.lock ();

    if (NULL != s_customQueryFilterFunction)
    {
        (*s_customQueryFilterFunction) (pPrismElement, waveManagedObjectQueryContextBase);
    }

    s_queryContextModificationFunctionsMutex.unlock ();
}

bool WaveConfigurationSegmentMap::applyAddCustomOrderFieldFunction (PrismElement *pPrismElement, WaveManagedObjectQueryContextBase &waveManagedObjectQueryContextBase)
{
    bool returnValue = false;

    s_queryContextModificationFunctionsMutex.lock ();

    if (NULL != s_addCustomOrderFieldFunction)
    {
        returnValue = (*s_addCustomOrderFieldFunction) (pPrismElement, waveManagedObjectQueryContextBase);
    }

    s_queryContextModificationFunctionsMutex.unlock ();

    return returnValue;
}

void WaveConfigurationSegmentMap::updateConfigurationSegmentHierarchyInformation (const string &configurationSegmentName, const vector<string> &parentClassNames, const vector<bool> &parentIsSingletonFlags, const vector<bool> &parentIsSkippedForDisplay)
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::iterator  element       = m_configurationSegmentMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentInformation *>::iterator  endElement    = m_configurationSegmentMap.end  ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = element->second;
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    pConfigurationSegmentInformation->setParentInformation (parentClassNames, parentIsSingletonFlags, parentIsSkippedForDisplay);

    m_configurationSegmentMapMutex.unlock ();
}

void WaveConfigurationSegmentMap::updateConfigurationSegmentParameterInformation (const string &configurationSegmentName, const bool &isSingleton, const bool &nodeSpecificSupported, const bool &partitionSupported)
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::iterator  element       = m_configurationSegmentMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentInformation *>::iterator  endElement    = m_configurationSegmentMap.end  ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    ConfigurationSegmentInformation *pConfigurationSegmentInformation = element->second;
    prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

    pConfigurationSegmentInformation->setParameters (isSingleton, nodeSpecificSupported, partitionSupported);

    m_configurationSegmentMapMutex.unlock ();
}

ConfigurationSegmentInformation *WaveConfigurationSegmentMap::getPConfigurationSegmentInformation (const string &configurationSegmentName)
{
    ConfigurationSegmentInformation *pConfigurationSegmentInformation = NULL;

    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::const_iterator  element     = m_configurationSegmentMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement  = m_configurationSegmentMap.end  ();

    // TODO :   Should not be called during initialization. Verify this.
    //          Will work only in combined data provider (Wsmd). Otherwise, it will crash.
    // prismAssert (endElement != element, __FILE__, __LINE__);

    if (endElement != element)
    {
        pConfigurationSegmentInformation = element->second;
        prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);
    }

    m_configurationSegmentMapMutex.unlock ();

    return pConfigurationSegmentInformation;
}

void WaveConfigurationSegmentMap::setConfigurationSegmentNameForNodeSpecificList (const string &configurationSegmentNameForNodeSpecificList)
{
    s_configurationSegmentForNodeSpecificList = configurationSegmentNameForNodeSpecificList;
}

void WaveConfigurationSegmentMap::setConfigurationSegmentNameForMultiPartitionList (const string &configurationSegmentNameForMultiPartitionList)
{
    s_configurationSegmentForMultiPartitionList = configurationSegmentNameForMultiPartitionList;
}

void WaveConfigurationSegmentMap::cacheManagedObjectClassNameForNodeSpecificList ()
{
    do
    {
        if (true == s_configurationSegmentForNodeSpecificList.empty ())
        {
            break;
        }

        ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation (s_configurationSegmentForNodeSpecificList);

        if (NULL == pConfigurationSegmentInformation)
        {
            break;
        }

        s_managedObjectClassNameForNodeSpecificList = pConfigurationSegmentInformation->getManagedObjectClassName ();

    } while (0);
}

void WaveConfigurationSegmentMap::cacheManagedObjectClassNameForMultiPartitionList ()
{
    do
    {
        if (true == s_configurationSegmentForMultiPartitionList.empty ())
        {
            break;
        }

        ConfigurationSegmentInformation *pConfigurationSegmentInformation = getPConfigurationSegmentInformation (s_configurationSegmentForMultiPartitionList);

        if (NULL == pConfigurationSegmentInformation)
        {
            break;
        }

        s_managedObjectClassNameForMultiPartitionList = pConfigurationSegmentInformation->getManagedObjectClassName ();

    } while (0);
}

string WaveConfigurationSegmentMap::getManagedObjectClassNameForNodeSpecificList ()
{
    return s_managedObjectClassNameForNodeSpecificList;
}

string WaveConfigurationSegmentMap::getManagedObjectClassNameForMultiPartitionList ()
{
    return s_managedObjectClassNameForMultiPartitionList;
}

void WaveConfigurationSegmentMap::addToConfigSegmentImportantInfoMap  (const string &configurationSegmentName, const string &managedObjectName, const string &serverName, WaveClientSynchronousConnection *pWaveClientSynchronousConnection)
{
    // Find out if the Configuration Segment Name is already present
    m_configSegmentImportantInfoMapMutex.lock ();

    map<string, ConfigurationSegmentImportantInfo *>::const_iterator  element     = m_configSegmentImportantInfoMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentImportantInfo *>::const_iterator  endElement  = m_configSegmentImportantInfoMap.end  ();

    if (endElement != element)
    {
        // We should not add an already existing Config Segment Name
        prismAssert (false, __FILE__, __LINE__);
    }

    m_configSegmentImportantInfoMapMutex.unlock ();

    ConfigurationSegmentImportantInfo *pConfigurationSegmentImportantInfo = new ConfigurationSegmentImportantInfo (configurationSegmentName, managedObjectName, serverName, pWaveClientSynchronousConnection);

    m_configSegmentImportantInfoMapMutex.lock ();
    m_configSegmentImportantInfoMap[configurationSegmentName] = pConfigurationSegmentImportantInfo;
    m_configSegmentImportantInfoMapMutex.unlock ();
}

ConfigurationSegmentImportantInfo *WaveConfigurationSegmentMap::getConfigurationSegmentImportantInfoFromMap (const string &configurationSegmentName)
{
    ConfigurationSegmentImportantInfo *pConfigurationSegmentImportantInfo = NULL;

    // Find out if the Configuration Segment Name is already present
    m_configSegmentImportantInfoMapMutex.lock ();

    map<string, ConfigurationSegmentImportantInfo *>::const_iterator  element     = m_configSegmentImportantInfoMap.find (configurationSegmentName);
    map<string, ConfigurationSegmentImportantInfo *>::const_iterator  endElement  = m_configSegmentImportantInfoMap.end  ();

    if (endElement != element)
    {
        pConfigurationSegmentImportantInfo = m_configSegmentImportantInfoMap[configurationSegmentName]; 
    }

    m_configSegmentImportantInfoMapMutex.unlock ();

    return (pConfigurationSegmentImportantInfo);
}

ConfigurationSegmentImportantInfo::ConfigurationSegmentImportantInfo (const string &configurationSegmentName, const string &managedObjectName, const string &serverName, WaveClientSynchronousConnection *pWaveClientSynchronousConnection)
    :   m_configurationSegmentName (configurationSegmentName),
        m_managedObjectName (managedObjectName),
        m_serverName (serverName),
        m_pWaveClientSynchronousConnection (pWaveClientSynchronousConnection)
{
    prismAssert ("" != m_configurationSegmentName, __FILE__, __LINE__);
    prismAssert ("" != m_managedObjectName, __FILE__, __LINE__);
    prismAssert ("" != m_serverName, __FILE__, __LINE__);
    prismAssert (NULL != m_pWaveClientSynchronousConnection, __FILE__, __LINE__);
}

ConfigurationSegmentImportantInfo::~ConfigurationSegmentImportantInfo ()
{

}

string ConfigurationSegmentImportantInfo::getConfigurationSegmentName () const
{
    return (m_configurationSegmentName);
}

void ConfigurationSegmentImportantInfo::setConfigurationSegmentName (const string &configurationSegmentName)
{
    m_configurationSegmentName = configurationSegmentName;
}

string ConfigurationSegmentImportantInfo::getManagedObjectName () const
{
    return (m_managedObjectName);
}
void ConfigurationSegmentImportantInfo::setManagedObjectName (const string &managedObjectName)
{
    m_managedObjectName = managedObjectName;
}
            
string ConfigurationSegmentImportantInfo::getServerName () const
{
    return (m_serverName);
}

void ConfigurationSegmentImportantInfo::setServerName (const string &serverName)
{
    m_serverName = serverName;
}

WaveClientSynchronousConnection* ConfigurationSegmentImportantInfo::getWaveClientSynchronousConnection () const
{
    return (m_pWaveClientSynchronousConnection);
}

void ConfigurationSegmentImportantInfo::setWaveClientSynchronousConnection (WaveClientSynchronousConnection *pWaveClientSynchronousConnection)
{
    m_pWaveClientSynchronousConnection = pWaveClientSynchronousConnection;
}

void WaveConfigurationSegmentMap::cacheIsLocalManagedObjectInformation ()
{
    m_configurationSegmentMapMutex.lock ();

    map<string, ConfigurationSegmentInformation *>::const_iterator  element     = m_configurationSegmentMap.begin ();
    map<string, ConfigurationSegmentInformation *>::const_iterator  endElement  = m_configurationSegmentMap.end   ();

    for (; endElement != element; element++)
    {
        ConfigurationSegmentInformation    *pConfigurationSegmentInformation    = element->second;
        prismAssert (NULL != pConfigurationSegmentInformation, __FILE__, __LINE__);

        string  managedObjectClassName = pConfigurationSegmentInformation->getManagedObjectClassName ();
        bool    isLocalManagedObject   = OrmRepository::isALocalManagedObject (managedObjectClassName);

        pConfigurationSegmentInformation->setIsLocalManagedObject (isLocalManagedObject);
    }

    m_configurationSegmentMapMutex.unlock ();
}

}
