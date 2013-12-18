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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#include "VcsFabric/Global/VcsClusterInfoMessage.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"


namespace DcmNs
{
VcsClusterInfoMessage::VcsClusterInfoMessage ()
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_GET_CLUSTER_INFO),
        m_clusterGenericStatus (WAVE_MESSAGE_SUCCESS),
        m_clusterSpecificStatus (WAVE_MESSAGE_SUCCESS),
        m_switchesInCluster (0),
        m_domainId (0),
        m_startDomainId (0),
        m_endDomainId (cluster_size_max),
        m_isClusterCreated (0),
        m_numDisconnectedSwitches (0)
{
}

VcsClusterInfoMessage::VcsClusterInfoMessage (const UI32 domainId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_GET_CLUSTER_INFO),
        m_clusterGenericStatus (WAVE_MESSAGE_SUCCESS),
        m_clusterSpecificStatus (WAVE_MESSAGE_SUCCESS),
        m_switchesInCluster (0),
        m_domainId (domainId),
        m_startDomainId (0),
        m_endDomainId (domainId),
        m_isClusterCreated (0),
        m_numDisconnectedSwitches (0)
{
}

VcsClusterInfoMessage::VcsClusterInfoMessage (const UI32 startDomainId, const UI32 endDomainId)
: ManagementInterfaceMessage (VcsFabObjectManager::getClassName (), VCS_FABRIC_GET_CLUSTER_INFO),
        m_clusterGenericStatus (WAVE_MESSAGE_SUCCESS),
        m_clusterSpecificStatus (WAVE_MESSAGE_SUCCESS),
        m_switchesInCluster (0),
        m_domainId (0),
        m_startDomainId (startDomainId),
        m_endDomainId (endDomainId),
        m_isClusterCreated (0),
        m_numDisconnectedSwitches (0)
{
}

VcsClusterInfoMessage::~VcsClusterInfoMessage ()
{
}


void VcsClusterInfoMessage::setupAttributesForSerialization ()
{
        ManagementInterfaceMessage::setupAttributesForSerialization ();

         addSerializableAttribute (new AttributeUI32        (&m_clusterGenericStatus,       "clusterGenericStatus"));
         addSerializableAttribute (new AttributeUI32        (&m_clusterSpecificStatus,      "clusterSpecificStatus"));
         addSerializableAttribute (new AttributeUI32        (&m_switchesInCluster,          "switchesInCluster"));
         addSerializableAttribute (new AttributeUI32        (&m_domainId,                   "domainId"));
         addSerializableAttribute (new AttributeUI32        (&m_startDomainId,              "startDomainId"));
         addSerializableAttribute (new AttributeUI32        (&m_endDomainId,                "endDomainId"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_switchSpecificStatus,       "switchSpecificStatus"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_switchGenericStatus,        "switchGenericStatus"));
         addSerializableAttribute (new AttributeStringVector(&m_serialNumbers,              "serialNumbers"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_vcsIds,                     "vcsIds"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_domainIds,                  "domainIds"));
         addSerializableAttribute (new AttributeBoolVector  (&m_inVcss,                     "inVcss"));
         addSerializableAttribute (new AttributeBoolVector  (&m_vcsModes,                   "vcsModes"));
         addSerializableAttribute (new AttributeBoolVector  (&m_isPrincipals,               "isPrincipals"));
         addSerializableAttribute (new AttributeMacAddressVector (&m_fcfMacs,               "fcfMacs"));
         addSerializableAttribute (new AttributeMacAddressVector (&m_switchMacs,            "switchMacs"));
         addSerializableAttribute (new AttributeStringVector(&m_ipAddress,                  "ipAddress"));
         addSerializableAttribute (new AttributeStringVector(&m_publicIpAddress,            "publicIpAddress"));
         addSerializableAttribute (new AttributeString      (&m_clusterFormTime,            "clusterFormTime"));
         addSerializableAttribute (new AttributeUI32Vector  (&m_locationId,                 "locationId"));
         addSerializableAttribute (new AttributeUI32        (&m_isClusterCreated,           "isClusterCreated"));
         addSerializableAttribute (new AttributeUI32        (&m_numDisconnectedSwitches,    "numDisconnectedSwitches"));
}

void VcsClusterInfoMessage::setClusterGenericStatus(UI32 genericStatus)
{
    m_clusterGenericStatus = genericStatus;
}

void VcsClusterInfoMessage::setClusterSpecificStatus(UI32 specificStatus)
{
    m_clusterSpecificStatus = specificStatus;
}

UI32 VcsClusterInfoMessage::getClusterGenericStatus()
{
    return m_clusterGenericStatus;
}

UI32 VcsClusterInfoMessage::getClusterSpecificStatus()
{
    return m_clusterSpecificStatus;
}


void VcsClusterInfoMessage::addSwitchMac (const MacAddress &switchMac)
{
        m_switchMacs.push_back (switchMac);
}

void VcsClusterInfoMessage::addFcfMac (const MacAddress &fcfMac)
{
        m_fcfMacs.push_back (fcfMac);
}

void VcsClusterInfoMessage::addIsPrincipal (const bool &isPrincipal)
{
        m_isPrincipals.push_back (isPrincipal);
}

void VcsClusterInfoMessage::addVcsMode (const bool &vcsMode)
{
        m_vcsModes.push_back (vcsMode);
}

void VcsClusterInfoMessage::addInVcs (const bool &inVcs)
{
        m_inVcss.push_back (inVcs);
}

void VcsClusterInfoMessage::addDomainId (const UI32 &domainId)
{
        m_domainIds.push_back (domainId);       
}
                

void VcsClusterInfoMessage::addSerialNumber (const string &serialNumber)
{
        m_serialNumbers.push_back (serialNumber);
}
void VcsClusterInfoMessage::addVcsId (const UI32 &vcsId)
{
        m_vcsIds.push_back (vcsId);
}

void VcsClusterInfoMessage::addIpAddress (const string &ipAddress)
{
        m_ipAddress.push_back (ipAddress);
}

void VcsClusterInfoMessage::addPublicIpAddress (const vector<string> &publicIpAddress)
{
    UI32 i;
    for (i = 0; i < publicIpAddress.size (); i++)
    {
            m_publicIpAddress.push_back (publicIpAddress[i]);
    }
}

void VcsClusterInfoMessage::addClusterFormTime (const string &dateTime)
{
    m_clusterFormTime = dateTime;
}

void VcsClusterInfoMessage::addGenericStatus (const UI32 &genericStatus)
{
        m_switchGenericStatus.push_back (genericStatus);
}

void VcsClusterInfoMessage::addSpecificStatus (const UI32 &specificStatus)
{
        m_switchSpecificStatus.push_back (specificStatus);
}

void VcsClusterInfoMessage::addLocationId (const UI32 &locationId)
{
        m_locationId.push_back (locationId);
}

void VcsClusterInfoMessage::setSwitchesInCluster (const UI32 &switchesInCluster)
{
        m_switchesInCluster = switchesInCluster;
}

UI32 VcsClusterInfoMessage::getSwitchesInCluster ()
{
        return m_switchesInCluster;
}
vector<string> VcsClusterInfoMessage::getSerialNumber ()
{
        return m_serialNumbers;
}

vector<UI32> VcsClusterInfoMessage::getVcsId ()
{
        return m_vcsIds;
}
vector<UI32> VcsClusterInfoMessage::getDomainId ()
{
        return m_domainIds;
}
vector<bool> VcsClusterInfoMessage::getInVcs ()
{
        return m_inVcss;
}
vector<bool> VcsClusterInfoMessage::getVcsMode ()
{
        return m_vcsModes;
}
vector<bool> VcsClusterInfoMessage::getIsPrincipal ()
{
        return m_isPrincipals;
}
vector<MacAddress> VcsClusterInfoMessage::getFcfMac ()
{
        return m_fcfMacs;
}
vector<MacAddress> VcsClusterInfoMessage::getSwitchMac ()
{
        return m_switchMacs;
}

UI32 VcsClusterInfoMessage::getStartDomainId ()
{
        return m_startDomainId;
}
UI32 VcsClusterInfoMessage::getEndDomainId ()
{
        return m_endDomainId;
}

void VcsClusterInfoMessage::setStartDomainId (const UI32 &startDomainId)
{
        m_startDomainId = startDomainId;
}

void VcsClusterInfoMessage::setEndDomainId (const UI32 &endDomainId)
{
        m_endDomainId = endDomainId;
}

vector<string> VcsClusterInfoMessage::getIpAddress ()
{
        return m_ipAddress;
}

vector<string> VcsClusterInfoMessage::getPublicIpAddress ()
{
        return m_publicIpAddress;
}

string VcsClusterInfoMessage::getClusterFormTime ()
{
    return m_clusterFormTime;
}

vector<UI32> VcsClusterInfoMessage::getSpecificStatus ()
{
        return m_switchSpecificStatus;
}

vector<UI32> VcsClusterInfoMessage::getGenericStatus ()
{
        return m_switchGenericStatus;
}

vector<UI32> VcsClusterInfoMessage::getLocationId ()
{
        return m_locationId;
}
UI32  VcsClusterInfoMessage::isClusterCreated ()
{
    return m_isClusterCreated;

}
void  VcsClusterInfoMessage::setClusterCreated (UI32 clusterCreated)
{
    m_isClusterCreated = clusterCreated;
}

void VcsClusterInfoMessage::setNumberOfDisconnectedSwitches(UI32 numberOfDisconnectedSwitches)
{
    m_numDisconnectedSwitches = numberOfDisconnectedSwitches;
}

UI32 VcsClusterInfoMessage::getNumberOfDisconnctedSwitches()
{
    return m_numDisconnectedSwitches;
}
}

