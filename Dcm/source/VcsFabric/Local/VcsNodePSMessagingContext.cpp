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

#include "VcsFabric/Local/VcsNodePSMessagingContext.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "VcsFabric/Local/VcsFabricLocalSerializableObject.h"

namespace DcmNs
{

VcsNodeRemoteMessagingContext::VcsNodeRemoteMessagingContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps)
{
    m_domainId.clear ();
    m_fabDomainList = NULL;
}

VcsNodeRemoteMessagingContext::~VcsNodeRemoteMessagingContext ()
{
}

void VcsNodeRemoteMessagingContext::setPDomainList (u_short *fabDomainList)
{
    m_fabDomainList = fabDomainList;
}

u_short * VcsNodeRemoteMessagingContext::getPDomainList () const
{
    return m_fabDomainList;
}

vector<UI32> VcsNodeRemoteMessagingContext::getDomainIds () const
{
    return (m_domainId);
}

void VcsNodeRemoteMessagingContext::setDomainIds (const vector<UI32> &domainIds)
{
    m_domainId = domainIds;
}

void VcsNodeRemoteMessagingContext::addToDomainIdVector (const UI32 &domainId)
{
    m_domainId.push_back (domainId);
}

void VcsNodeRemoteMessagingContext::addToValClientSynchronousConnection (const UI32 &domainId, ValClientSynchronousConnection *pValClientSynchronousConnection)
{
    map<UI32, ValClientSynchronousConnection *>::iterator  element = m_mapDomainIdToClientSynchronousConnection.find (domainId);
    map<UI32, ValClientSynchronousConnection *>::iterator  end     = m_mapDomainIdToClientSynchronousConnection.end (); 

    if (end == element)
    {   
        m_mapDomainIdToClientSynchronousConnection [domainId] = pValClientSynchronousConnection;
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsNodeRemoteMessagingContext::disconnectFromServer (const UI32 &domainId)
{
    map<UI32, ValClientSynchronousConnection *>::iterator  element = m_mapDomainIdToClientSynchronousConnection.find (domainId);
    map<UI32, ValClientSynchronousConnection *>::iterator  end     = m_mapDomainIdToClientSynchronousConnection.end (); 

    ValClientSynchronousConnection *pValClientSynchronousConnection;
    
    if (end != element)
    {   
        pValClientSynchronousConnection = m_mapDomainIdToClientSynchronousConnection [domainId];

        pValClientSynchronousConnection->close ();

        delete pValClientSynchronousConnection;
    }   
    else
    {   
        prismAssert (false, __FILE__, __LINE__);
    }   
}


VcsNodePSMessagingContext::VcsNodePSMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pSenderContext (NULL)
{
    m_domainId.clear ();
    m_mappedId.clear ();
    m_pMessage = NULL;
    m_fabDomainList = NULL;
    m_switchInfo = NULL;
}

VcsNodePSMessagingContext::~VcsNodePSMessagingContext ()
{
}

void VcsNodePSMessagingContext::setPDomainList (u_short *fabDomainList)
{
    m_fabDomainList = fabDomainList;
}

u_short * VcsNodePSMessagingContext::getPDomainList () const
{
    return m_fabDomainList;
}

void VcsNodePSMessagingContext::setSwitchInfoParametersP (VcsFabricLocalSwitchInfoObject *switchInfo)
{
    m_switchInfo = switchInfo;
}

VcsFabricLocalSwitchInfoObject *VcsNodePSMessagingContext::getSwitchInfoParametersP () const
{
    return m_switchInfo;
}

vector<UI32> VcsNodePSMessagingContext::getMappedId () const
{
    return (m_mappedId);
}

vector<UI32> VcsNodePSMessagingContext::getDomainId () const
{
    return (m_domainId);
}

PrismMessage * VcsNodePSMessagingContext::getPMessage () const
{
	return m_pMessage;
}

UI32 VcsNodePSMessagingContext::getEstablishPrincipalCase () const
{
	return m_establishPrincipalCase;
}

void *VcsNodePSMessagingContext::getPSenderContext ()
{
    return (m_pSenderContext);
}

void VcsNodePSMessagingContext::setPMessage (PrismMessage *pMessage)
{
	m_pMessage = pMessage;
}

void VcsNodePSMessagingContext::setMappedId (const vector<UI32> &mapped_id)
{
    m_mappedId = mapped_id;
}

void VcsNodePSMessagingContext::setDomainId (const vector<UI32> &domain_id)
{
    m_domainId = domain_id;
}

void VcsNodePSMessagingContext::setCompletionStatus (ResourceId &status)
{
	m_pMessage->setCompletionStatus (status);
}

void VcsNodePSMessagingContext::setEstablishPrincipalCase (const UI32 &establishPrincipalCase) 
{
	m_establishPrincipalCase = establishPrincipalCase;
}

void VcsNodePSMessagingContext::addToDomainIdInConnectedStatusVector (UI32 domainId)
{
    m_domainIdsInConnectedStatus.push_back (domainId);
}

vector<UI32> VcsNodePSMessagingContext::getDomainIdInConnectedStatusVector () const
{
    return (m_domainIdsInConnectedStatus);
}

void  VcsNodePSMessagingContext::setPSenderContext (void *pSenderContext)
{
    m_pSenderContext = pSenderContext;
}

VcsNodePSSynchronousMessagingContext::VcsNodePSSynchronousMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
    m_establishPrincipalCase = 0;
    m_pMessage               = NULL;
    m_fabDomainList          = NULL;
    m_switchInfo             = NULL;
}

VcsNodePSSynchronousMessagingContext::~VcsNodePSSynchronousMessagingContext ()
{
}

void VcsNodePSSynchronousMessagingContext::setPDomainList (u_short *fabDomainList)
{
    m_fabDomainList = fabDomainList;
}

u_short * VcsNodePSSynchronousMessagingContext::getPDomainList () const
{
    return m_fabDomainList;
}

void VcsNodePSSynchronousMessagingContext::setSwitchInfoParametersP (VcsFabricLocalSwitchInfoObject *switchInfo)
{
    m_switchInfo = switchInfo;
}

VcsFabricLocalSwitchInfoObject *VcsNodePSSynchronousMessagingContext::getSwitchInfoParametersP () const
{
    return m_switchInfo;
}

vector<UI32> VcsNodePSSynchronousMessagingContext::getMappedId () const
{
    return (m_mappedId);
}

vector<UI32> VcsNodePSSynchronousMessagingContext::getDomainId () const
{
    return (m_domainId);
}

PrismMessage * VcsNodePSSynchronousMessagingContext::getPMessage () const
{
	return m_pMessage;
}

UI32 VcsNodePSSynchronousMessagingContext::getEstablishPrincipalCase () const
{
	return m_establishPrincipalCase;
}

void VcsNodePSSynchronousMessagingContext::setPMessage (PrismMessage *pMessage)
{
	m_pMessage = pMessage;
}

void VcsNodePSSynchronousMessagingContext::setMappedId (const vector<UI32> &mapped_id)
{
    m_mappedId = mapped_id;
}

void VcsNodePSSynchronousMessagingContext::setDomainId (const vector<UI32> &domain_id)
{
    m_domainId = domain_id;
}

void VcsNodePSSynchronousMessagingContext::setCompletionStatus (ResourceId &status)
{
	m_pMessage->setCompletionStatus (status);
}

void VcsNodePSSynchronousMessagingContext::setEstablishPrincipalCase (const UI32 &establishPrincipalCase) 
{
	m_establishPrincipalCase = establishPrincipalCase;
}

}
