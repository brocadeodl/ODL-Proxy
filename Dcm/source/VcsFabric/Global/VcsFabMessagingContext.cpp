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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                             *
 ***************************************************************************/

#include "VcsFabric/Global/VcsFabMessagingContext.h"
#include "VcsFabric/Global/VcsFabAddRejoinNodeWorker.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"

namespace DcmNs
{

VcsFabMessagingContext::VcsFabMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps), m_pMessage(NULL), m_rejoinNodeStatus(WAVE_MESSAGE_ERROR), m_addNodeStatus(WAVE_MESSAGE_ERROR), m_isClusterReboot(false),
        m_replaceRejoinNodeStatus (WAVE_MESSAGE_SUCCESS)
{
}

VcsFabMessagingContext::~VcsFabMessagingContext ()
{
}

vector<UI32> VcsFabMessagingContext::getMappedId () const
{
    return (m_mappedId);
}

vector<UI32> VcsFabMessagingContext::getDomainId () const
{
    return (m_domainId);
}

PrismMessage * VcsFabMessagingContext::getPMessage () const
{
        return m_pMessage;
}
void VcsFabMessagingContext::setPMessage (PrismMessage *pMessage)
{
        m_pMessage = pMessage;
}

void VcsFabMessagingContext::setMappedId (const vector<UI32> &mapped_id)
{
    m_mappedId = mapped_id;
}

void VcsFabMessagingContext::setDomainId (const vector<UI32> &domain_id)
{
    m_domainId = domain_id;
}

void VcsFabMessagingContext::setCompletionStatus (ResourceId &status)
{
        m_pMessage->setCompletionStatus (status);
}
ResourceId VcsFabMessagingContext::getCompletionStatus () const
{
        return m_pMessage->getCompletionStatus ();
}

void  VcsFabMessagingContext::addSwitchInfoToAddList (const VcsFabricLocalSwitchInfoObject & switchInfo)
{
    m_switchesToBeAdded.push_back(switchInfo);
}

void  VcsFabMessagingContext::addSwitchInfoToRejoinList(const VcsFabricLocalSwitchInfoObject & switchInfo)
{
    m_switchesToBeRejoined.push_back(switchInfo);
}

void VcsFabMessagingContext::addToUncontrolledFailoverList (LocationId failedLocationId)
{
    m_uncontrolledFailoverSwitches.push_back(failedLocationId);
}

void VcsFabMessagingContext::addToControlledFailoverList (LocationId failedLocationId)
{
    m_controlledFailoverSwitches.push_back(failedLocationId);
}

void  VcsFabMessagingContext::addSwitchInfoToReplaceRejoinList(const VcsFabricLocalSwitchInfoObject & switchInfo)
{
    m_switchesToBeReplaceRejoined.push_back(switchInfo);
}

vector<VcsFabricLocalSwitchInfoObject> VcsFabMessagingContext::getSwitchesToBeAdded ()
{
    return m_switchesToBeAdded;
}

vector<VcsFabricLocalSwitchInfoObject> VcsFabMessagingContext::getSwitchesToBeRejoined ()
{
    return m_switchesToBeRejoined;
}

vector<LocationId>   VcsFabMessagingContext::getSwitchesForUncontrolledFailover () 
{
   return m_uncontrolledFailoverSwitches;
}

vector<LocationId>   VcsFabMessagingContext::getSwitchesForControlledFailover ()
{
   return m_controlledFailoverSwitches;
}

vector<VcsFabricLocalSwitchInfoObject> VcsFabMessagingContext::getSwitchesToBeReplaceRejoined ()
{
    return m_switchesToBeReplaceRejoined;
}

void    VcsFabMessagingContext::setRejoinNodeStatus (ResourceId rejoinStatus)
{
   m_rejoinNodeStatus = rejoinStatus;
}

ResourceId  VcsFabMessagingContext::getRejoinNodeStatus () const
{
   return m_rejoinNodeStatus;
}

void  VcsFabMessagingContext::setAddNodeStatus (ResourceId addStatus)
{
   m_addNodeStatus = addStatus;
}

ResourceId VcsFabMessagingContext::getAddNodeStatus () const
{

    return m_addNodeStatus;

}


void  VcsFabMessagingContext::setClusterReboot(bool isClusterReboot)
{
    m_isClusterReboot = isClusterReboot;

}
bool  VcsFabMessagingContext::getClusterReboot () const
{
    return m_isClusterReboot;
}

void    VcsFabMessagingContext::setReplaceRejoinNodeStatus (ResourceId replaceRejoinStatus)
{
   m_replaceRejoinNodeStatus = replaceRejoinStatus;
}

ResourceId  VcsFabMessagingContext::getReplaceRejoinNodeStatus () const
{
   return m_replaceRejoinNodeStatus;
}

void VcsFabMessagingContext::setFilenamesToSync ( const vector<string> &filenamesToSync )
{   
    m_filenamesToSync = filenamesToSync;
}

void VcsFabMessagingContext::getFilenamesToSync ( vector<string> &filenamesToSync )
{
    filenamesToSync = m_filenamesToSync;
}

}
