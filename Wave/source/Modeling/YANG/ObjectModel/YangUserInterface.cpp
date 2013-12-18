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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka / Mandar Datar                     *
 ***************************************************************************/

#include "Modeling/YANG/ObjectModel/YangUserInterface.h"
#include "Framework/Utils/AssertUtils.h"
#include "SystemManagement/WaveConfigurationSegmentMap.h"

#include <list>
#include <algorithm>

namespace WaveNs
{

YangUserInterface::YangUserInterface ()
    : YangElement (getYangName (), "")
{
    m_hierarchyInformationForYangElementsInTreeForDataPrepared = false;
}

YangUserInterface::~YangUserInterface()
{
}

string YangUserInterface::getYangName ()
{
    return ("user-interface");
}

YangElement *YangUserInterface::createInstance ()
{
    return (new YangUserInterface ());
}

void YangUserInterface::initializeFromYangModuleCollection (const YangModuleCollection &yangModuleCollection)
{
}

void YangUserInterface::computeCliTargetNodeNameMapForData ()
{
    YangElement::computeCliTargetNodeNameMapForData (m_cliTargetNodeNameMapForData, m_cliTargetNodeNameMapForDataWithDropNodeName);
}

vector<YangElement *> YangUserInterface::getYangElementsByTargetNodeNameForDropNode (const string &targetNodeNameForDropNode)
{
    map<string, vector<YangElement *> >::iterator element    = m_cliTargetNodeNameMapForDataWithDropNodeName.find (targetNodeNameForDropNode);
    map<string, vector<YangElement *> >::iterator endElement = m_cliTargetNodeNameMapForDataWithDropNodeName.end  ();

    if (endElement != element)
    {
        return (element->second);
    }
    else
    {
    	return (vector<YangElement *> ());
    }
}

vector<YangElement *> YangUserInterface::getYangElementsByTargetNodeName (const string &targetNodeName)
{
    map<string, vector<YangElement *> >::iterator element    = m_cliTargetNodeNameMapForData.find (targetNodeName);
    map<string, vector<YangElement *> >::iterator endElement = m_cliTargetNodeNameMapForData.end  ();

    if (endElement != element)
    {
        return (element->second);
    }
    else
    {
        return (vector<YangElement *> ());
    }
}

void YangUserInterface::prepareYangElementTreeForData ()
{
    vector<YangElement *>  &pChildYangElements    = getChildElements ();
    UI32                    numberOfChildElements = pChildYangElements.size ();

    vector<YangModule *>    pChildYangModules;

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildYangElement = pChildYangElements[i];
        prismAssert (NULL != pChildYangElement, __FILE__, __LINE__);

        YangModule *pYangModule = dynamic_cast<YangModule *> (pChildYangElement);

        if (NULL != pYangModule)
        {
            pChildYangModules.push_back (pYangModule);
        }
        else
        {
            prismAssert (false, __FILE__, __LINE__);    // Rolling reboot case.
        }
    }

    std::stable_sort (pChildYangModules.begin (), pChildYangModules.end (), YangModule::compareYangModulesByNamespaceUri);

    UI32 numberOfChildYangModules = pChildYangModules.size ();

    for (UI32 i = 0; i < numberOfChildYangModules; i++)
    {
        YangModule *pYangModule = pChildYangModules[i];
        prismAssert (NULL != pYangModule, __FILE__, __LINE__);

        vector<YangElement *>  &pChildElementsOfYangModule          = pYangModule->getChildElements ();
        UI32                    numberOfchildElementsOfYangModule   = pChildElementsOfYangModule.size ();

        for (UI32 j = 0; j < numberOfchildElementsOfYangModule; j++)
        {
            YangElement *pYangElement = pChildElementsOfYangModule[j];
            prismAssert (NULL != pYangElement, __FILE__, __LINE__);

            if (true == pYangElement->isYangElementForData ())
            {
                m_moduleChildElementsForData.push_back (pYangElement);
            }
        }
    }

    std::stable_sort (m_moduleChildElementsForData.begin (), m_moduleChildElementsForData.end (), YangElement::compareYangElementsByDataIndex);

    UI32 numberOfModuleChildElementsForData = m_moduleChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->setupYangElementTreeForData  ();
    }
}

void YangUserInterface::debugPrintYangElementTreeForData (const string &outputFilePath) const
{
    FILE *pOutputFile = NULL;

    if (true == outputFilePath.empty ())
    {
        pOutputFile = stdout;
    }
    else
    {
        pOutputFile = fopen (outputFilePath.c_str (), "w");

        if (NULL == pOutputFile)
        {
            string errorString = string ("Failed to open outputFile [") + outputFilePath + string ("] ");
            perror (errorString.c_str ());
            return;
        }
    }

    UI32 numberOfModuleChildElementsForData = m_moduleChildElementsForData.size ();

    UI32 depth = 0;

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->debugPrintChildElementInforamtionForData (pOutputFile, depth);
    }

	fclose (pOutputFile);
}

void YangUserInterface::propagateNodeSpecificAndMultiPartitionInformationInDataElementTree ()
{
    UI32 numberOfModuleChildElementsForData = m_moduleChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->propagateNodeSpecificInformation ();
    }

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->propagateMultiPartitionInformation ();
    }
}

void YangUserInterface::prepareHierarchyInformationForYangElementsInTreeForData ()
{
    if (true == m_hierarchyInformationForYangElementsInTreeForDataPrepared)
    {
        return;
    }

    UI32 numberOfModuleChildElementsForData = m_moduleChildElementsForData.size ();

    // Managed Object Hierarchy.

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->prepareHierarchyInformation ();
    }

    // Known names in hierarchy.

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->prepareSetOfKnownDisplayNames ();
    }

    for (UI32 i = 0; i < numberOfModuleChildElementsForData; i++)
    {
        YangElement *pYangElement = m_moduleChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        const set<string> &knownDisplayNamesForChildElement = pYangElement->getKnownDisplayNames ();

        for (set<string>::const_iterator itr = knownDisplayNamesForChildElement.begin (); itr != knownDisplayNamesForChildElement.end (); itr++)
        {
            addYangElementForCliToken (*itr, pYangElement);
        }
    }

    // Mark local config subtree -

    vector<YangElement *> &pChildElements = getChildElements ();

    UI32 numberOfchildren = pChildElements.size ();

    for (UI32 i = 0; i < numberOfchildren; i++)
    {
        YangElement *pChildElement = pChildElements[i];

        prismAssert (NULL != pChildElement, __FILE__, __LINE__);

        pChildElement->computeIsInLocalConfigSubtree ();
    }

    // Get ManagedObjectClassName for node-specific and multi-partition base list.

    WaveConfigurationSegmentMap::cacheManagedObjectClassNameForNodeSpecificList ();
    WaveConfigurationSegmentMap::cacheManagedObjectClassNameForMultiPartitionList ();

    m_hierarchyInformationForYangElementsInTreeForDataPrepared = true;
}

void YangUserInterface::printConfiguration (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    vector<YangElement *> selectedChildElementsForData;

    if (0 != pYangDisplayConfigurationContext->getNumberOfRemainingTokens ())
    {
        string tokenString = pYangDisplayConfigurationContext->advanceToNextToken ();

        getSelectedChildElementsForData (tokenString, selectedChildElementsForData);

        pYangDisplayConfigurationContext->decrementTokenPosition ();
    }
    else
    {
        selectedChildElementsForData = m_moduleChildElementsForData;
    }

    UI32 numberOfSelectedChildElementsForData = selectedChildElementsForData.size ();

    for (UI32 i = 0; i < numberOfSelectedChildElementsForData; i++)
    {
        YangElement *pYangElement = selectedChildElementsForData[i];
        prismAssert (NULL != pYangElement, __FILE__, __LINE__);

        pYangElement->displayConfiguration1 (pYangDisplayConfigurationContext);
    }
}

YangElement *YangUserInterface::getParentYangModuleForGrandChildYangElementByName (const string &uniqueYangElementName, const string &namespaceString) const
{
    YangElement            *pChildModuleElement   = NULL;

    vector<YangElement *>   pChildYangElements;
    getAllChildrenByYangName (YangModule::getYangName (), pChildYangElements);

    UI32                    numberOfChildElements = pChildYangElements.size ();

    for (UI32 i = 0; i < numberOfChildElements; i++)
    {
        YangElement *pChildYangElement = pChildYangElements[i];
        prismAssert (NULL != pChildYangElement, __FILE__, __LINE__);

        YangModule *pYangModule = dynamic_cast<YangModule *> (pChildYangElement);

        if (NULL != pYangModule)
        {
            // trace (TRACE_LEVEL_INFO, "YangUserInterface::getParentYangModuleForGrandChildYangElementByName : searching for [" + uniqueYangElementName + "] in [" + pYangModule->getYangName () + ":" + pYangModule->getName () + "]");

            YangElement *pTargetYangElement = pYangModule->getUniqueYangElementByName (uniqueYangElementName);

            if (NULL != pTargetYangElement)
            {
                pChildModuleElement = pYangModule;
                break;
            }
        }
    }

    // tracePrintf (TRACE_LEVEL_INFO, "YangUserInterface::getParentYangModuleForGrandChildYangElementByName : pYangModule is [%p]", pChildModuleElement);

    return pChildModuleElement;
}

void YangUserInterface::applyAddModeAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("AddMode : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setIsCliAddMode (true);
    }
}

void YangUserInterface::applyCompactElemAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("CompactElem : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setCliCompactSyntax ();
    }
}

void YangUserInterface::applyDropElemAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("DropElem : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setDropNodeNameForCli (true);
    }
}

void YangUserInterface::applyMultiValueAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("MultiValue : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setIsCliMultiValue ();
    }
}

void YangUserInterface::applySuppressModeAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("SuppressMode : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setIsCliSuppressMode (true);
    }
}

void YangUserInterface::applyIncompleteCommandAnnotationToTargetNodes (const string &targetNodeName)
{
    vector<YangElement *> pTargetNodeYangElements = getYangElementsByTargetNodeName (targetNodeName);

    UI32 numberOfTargetNodeYangElements = pTargetNodeYangElements.size ();

    trace (TRACE_LEVEL_INFO, string ("IncompleteCommand : [") + targetNodeName + string ("], numberOfTargetNodeYangElements [") + numberOfTargetNodeYangElements + string ("]"));

    for (UI32 i = 0; i < numberOfTargetNodeYangElements; i++)
    {
        pTargetNodeYangElements[i]->setCliIncompleteCommand (true);
    }
}

void YangUserInterface::applyAnnotationSupplement ()
{
    applyAddModeAnnotation ();
    applyCompactElemAnnotation ();
    applyDropElemAnnotation ();
    applyMultiValueAnnotation ();
    applySuppressModeAnnotation ();
    applyIncompleteCommandAnnotation ();
}

void YangUserInterface::applyAddModeAnnotation ()
{
    applyAddModeAnnotationToTargetNodes ("config hosts");
    applyAddModeAnnotationToTargetNodes ("protocol spanning-tree stp");
    applyAddModeAnnotationToTargetNodes ("protocol spanning-tree rstp");
    applyAddModeAnnotationToTargetNodes ("protocol spanning-tree mstp");
    applyAddModeAnnotationToTargetNodes ("protocol spanning-tree pvst");
    applyAddModeAnnotationToTargetNodes ("protocol spanning-tree rpvst");
}

void YangUserInterface::applyCompactElemAnnotation ()
{
    applyCompactElemAnnotationToTargetNodes ("interface TenGigabitEthernet channel-group");
    applyCompactElemAnnotationToTargetNodes ("interface TenGigabitEthernet channel-group");
    applyCompactElemAnnotationToTargetNodes ("interface FortyGigabitEthernet channel-group");
    applyCompactElemAnnotationToTargetNodes ("interface FortyGigabitEthernet channel-group");
    applyCompactElemAnnotationToTargetNodes ("interface Port-channel private-vlan");
    applyCompactElemAnnotationToTargetNodes ("interface Port-channel private-vlan mapping");
    applyCompactElemAnnotationToTargetNodes ("vlan classifier");
    applyCompactElemAnnotationToTargetNodes ("interface TenGigabitEthernet vlan classifier activate");
    applyCompactElemAnnotationToTargetNodes ("interface Port-channel vlan classifier activate");
}

void YangUserInterface::applyDropElemAnnotation ()
{
    applyDropElemAnnotationToTargetNodes ("ip route routing-table");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet channel-group port-int");
    applyDropElemAnnotationToTargetNodes ("interface FortyGigabitEthernet channel-group port-int");
    applyDropElemAnnotationToTargetNodes ("port-profile vlan-profile switchport mode vlanMode");
    applyDropElemAnnotationToTargetNodes ("interface Vlan private-vlan pvlan_type_leaf");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport mode vlanMode");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport mode private-vlan private-vlanMode");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport mode private-vlan trunk private-vlan-trunkMode");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan mapping primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan mapping secondaryVlanIds");
    applyDropElemAnnotationToTargetNodes ("interface Vlan private-vlan association secondaryVlanIds");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan host-association primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan association trunk primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan host-association secondary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan association trunk secondaryVlanIds");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport mode vlanMode");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport mode private-vlan private-vlanMode");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport mode private-vlan trunk private-vlan-trunkMode");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan mapping primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan mapping secondaryVlanIds");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan host-association primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan association trunk primary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan host-association secondary-vlan");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel switchport private-vlan association trunk secondaryVlanIds");
    applyDropElemAnnotationToTargetNodes ("vlan classifier rule mac address");
    applyDropElemAnnotationToTargetNodes ("vlan classifier rule proto proto-val");
    applyDropElemAnnotationToTargetNodes ("vlan classifier group oper");
    applyDropElemAnnotationToTargetNodes ("ip access-list standard seq deny ip-address");
    applyDropElemAnnotationToTargetNodes ("ip access-list standard seq permit ip-address");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq deny ip");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq permit ip");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq deny dst-ip");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq deny ip-proto");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq permit dst-ip");
    applyDropElemAnnotationToTargetNodes ("ip access-list extended seq permit ip-proto");
    applyDropElemAnnotationToTargetNodes ("interface TenGigabitEthernet ip access-group l3-access-list");
    applyDropElemAnnotationToTargetNodes ("interface Port-channel ip access-group l3-access-list");
    applyDropElemAnnotationToTargetNodes ("interface Vlan ip access-group l3-access-list");
    applyDropElemAnnotationToTargetNodes ("alias-config alias expansion");
    applyDropElemAnnotationToTargetNodes ("alias-config user alias expansion");
    applyDropElemAnnotationToTargetNodes ("clock-set-datetime");
    applyDropElemAnnotationToTargetNodes ("clock-set-timezone");
    applyDropElemAnnotationToTargetNodes ("snmp-server agtconfig");
    applyDropElemAnnotationToTargetNodes ("snmp-server community access");
}

void YangUserInterface::applyMultiValueAnnotation ()
{
    applyMultiValueAnnotationToTargetNodes ("alias-config alias expansion");
    applyMultiValueAnnotationToTargetNodes ("alias-config user alias expansion");
}

void YangUserInterface::applySuppressModeAnnotation ()
{
    applySuppressModeAnnotationToTargetNodes ("vlan classifier rule");
    applySuppressModeAnnotationToTargetNodes ("vlan classifier group");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet vlan classifier activate group");
    applySuppressModeAnnotationToTargetNodes ("port-profile vlan-profile vlan classifier activate group");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel vlan classifier activate group");
    applySuppressModeAnnotationToTargetNodes ("ip access-list standard seq");
    applySuppressModeAnnotationToTargetNodes ("ip access-list extended seq");
    applySuppressModeAnnotationToTargetNodes ("mac-address-table static");
    applySuppressModeAnnotationToTargetNodes ("interface Vlan ip igmp snooping mrouter interface");
    applySuppressModeAnnotationToTargetNodes ("interface Vlan ip igmp static-group");
    applySuppressModeAnnotationToTargetNodes ("alias-config alias");
    applySuppressModeAnnotationToTargetNodes ("alias-config user alias");
    applySuppressModeAnnotationToTargetNodes ("interface Vlan private-vlan association secondaryVlanIds");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet private-vlan association trunk secondaryVlanIds");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet private-vlan association trunk");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel private-vlan association trunk secondaryVlanIds");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel private-vlan association trunk");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan mapping primary-vlan secondaryVlanIds");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet switchport private-vlan mapping");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel switchport private-vlan mapping");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel switchport private-vlan mapping primary-vlan secondaryVlanIds");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet channel-group");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet switchport trunk allowed vlanoper vlan");
    applySuppressModeAnnotationToTargetNodes ("port-profile vlan-profile switchport trunk allowed vlanoper vlan");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel switchport trunk allowed vlanoper vlan");
    applySuppressModeAnnotationToTargetNodes ("clock-set-datetime");
    applySuppressModeAnnotationToTargetNodes ("clock-set-timezone");
    applySuppressModeAnnotationToTargetNodes ("snmp-server");
    applySuppressModeAnnotationToTargetNodes ("protocol spanning-tree mstp instance");
    applySuppressModeAnnotationToTargetNodes ("protocol spanning-tree pvst vlan");
    applySuppressModeAnnotationToTargetNodes ("protocol spanning-tree rpvst vlan");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet spanning-tree vlan");
    applySuppressModeAnnotationToTargetNodes ("interface GigabitEthernet spanning-tree vlan");
    applySuppressModeAnnotationToTargetNodes ("interface TenGigabitEthernet spanning-tree instance");
    applySuppressModeAnnotationToTargetNodes ("interface GigabitEthernet spanning-tree instance");
    applySuppressModeAnnotationToTargetNodes ("interface FortyGigabitEthernet spanning-tree instance");
    applySuppressModeAnnotationToTargetNodes ("interface FortyGigabitEthernet spanning-tree vlan");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel spanning-tree instance");
    applySuppressModeAnnotationToTargetNodes ("interface Port-channel spanning-tree vlan");
}

void YangUserInterface::applyIncompleteCommandAnnotation ()
{
    applyIncompleteCommandAnnotationToTargetNodes ("vlan classifier rule");
    applyIncompleteCommandAnnotationToTargetNodes ("interface");
    applyIncompleteCommandAnnotationToTargetNodes ("vlan classifier rule mac");
    applyIncompleteCommandAnnotationToTargetNodes ("vlan classifier rule proto");
    applyIncompleteCommandAnnotationToTargetNodes ("vlan classifier rule proto proto-val");
    applyIncompleteCommandAnnotationToTargetNodes ("vcenter");
    applyIncompleteCommandAnnotationToTargetNodes ("mac-address-table static forward interface_type interface vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("mac access-list extended seq");
    applyIncompleteCommandAnnotationToTargetNodes ("mac access-list extended seq action");
    applyIncompleteCommandAnnotationToTargetNodes ("mac access-list extended seq source");
    applyIncompleteCommandAnnotationToTargetNodes ("mac access-list extended seq srchost");
    applyIncompleteCommandAnnotationToTargetNodes ("protocol spanning-tree mstp instance");
    applyIncompleteCommandAnnotationToTargetNodes ("protocol spanning-tree pvst vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("protocol spanning-tree rpvst vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("interface TenGigabitEthernet spanning-tree instance");
    applyIncompleteCommandAnnotationToTargetNodes ("interface GigabitEthernet spanning-tree instance");
    applyIncompleteCommandAnnotationToTargetNodes ("interface TenGigabitEthernet spanning-tree vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("interface GigabitEthernet spanning-tree vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("interface TenGigabitEthernet spanning-tree");
    applyIncompleteCommandAnnotationToTargetNodes ("interface GigabitEthernet spanning-tree");
    applyIncompleteCommandAnnotationToTargetNodes ("interface FortyGigabitEthernet spanning-tree instance");
    applyIncompleteCommandAnnotationToTargetNodes ("interface FortyGigabitEthernet spanning-tree vlan");
    applyIncompleteCommandAnnotationToTargetNodes ("interface FortyGigabitEthernet spanning-tree");
    applyIncompleteCommandAnnotationToTargetNodes ("interface Port-channel spanning-tree");
    applyIncompleteCommandAnnotationToTargetNodes ("interface Port-channel spanning-tree instance");
    applyIncompleteCommandAnnotationToTargetNodes ("interface Port-channel spanning-tree vlan");
}

void YangUserInterface::lock ()
{
    m_mutexToBlockVisibilityInConcurrentReadThreads.lock ();
}

void YangUserInterface::unlock ()
{
    m_mutexToBlockVisibilityInConcurrentReadThreads.unlock ();
}

}
