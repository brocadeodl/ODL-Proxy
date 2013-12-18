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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Leifang Hu                                    *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeHaMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"
//#include "VcsFabric/Local/VcsNodeRemoteMessagingObjectManager.h"
#include "VcsFabric/Local/VcsNodeHaObjectManager.h"
#include "VcsFabric/Local/VcsNodeHaClientMsgCStruct.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VcsNodeNotifyStartSyncDumpMessage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VcsNodeNotifyStartSyncDumpMessage::VcsNodeNotifyStartSyncDumpMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort)
      :ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_NOTIFY_START_SYNC_DUMP),
       m_peerHaIpAddress(peerHaIpAddress),
       m_peerHaIpPort(peerHaIpPort)
{
}

VcsNodeNotifyStartSyncDumpMessage::~VcsNodeNotifyStartSyncDumpMessage()
{
}

void VcsNodeNotifyStartSyncDumpMessage::setPeerHaIpAddress(const string &peerHaIpAddress)
{
    m_peerHaIpAddress = peerHaIpAddress;
}

void VcsNodeNotifyStartSyncDumpMessage::setPeerHaIpPort(const SI32 &peerHaIpPort)
{
    m_peerHaIpPort = peerHaIpPort;
}

string VcsNodeNotifyStartSyncDumpMessage::getPeerHaIpAddress() const
{
    return m_peerHaIpAddress;
}

SI32 VcsNodeNotifyStartSyncDumpMessage::getPeerHaIpPort() const
{
    return m_peerHaIpPort;
}

void VcsNodeNotifyStartSyncDumpMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString  (&m_peerHaIpAddress,    "peerHaIpAddress"));
     addSerializableAttribute (new AttributeSI32    (&m_peerHaIpPort,       "peerHaIpPort"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VcsNodeNotifyEndSyncDumpMessage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VcsNodeNotifyEndSyncDumpMessage::VcsNodeNotifyEndSyncDumpMessage (const string &peerHaIpAddress, const SI32 &peerHaIpPort)
      :ManagementInterfaceMessage (VcsFabricLocalObjectManager::getClassName (), VCS_NODE_NOTIFY_END_SYNC_DUMP),
       m_peerHaIpAddress(peerHaIpAddress),
       m_peerHaIpPort(peerHaIpPort)
{
}

VcsNodeNotifyEndSyncDumpMessage::~VcsNodeNotifyEndSyncDumpMessage()
{
}

void VcsNodeNotifyEndSyncDumpMessage::setPeerHaIpAddress(const string &peerHaIpAddress)
{
    m_peerHaIpAddress = peerHaIpAddress;
}

void VcsNodeNotifyEndSyncDumpMessage::setPeerHaIpPort(const SI32 &peerHaIpPort)
{
    m_peerHaIpPort = peerHaIpPort;
}

string VcsNodeNotifyEndSyncDumpMessage::getPeerHaIpAddress() const
{
    return m_peerHaIpAddress;
}

SI32 VcsNodeNotifyEndSyncDumpMessage::getPeerHaIpPort() const
{
    return m_peerHaIpPort;
}

void VcsNodeNotifyEndSyncDumpMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeString  (&m_peerHaIpAddress,"peerHaIpAddress"));
     addSerializableAttribute (new AttributeSI32    (&m_peerHaIpPort,   "peerHaIpPort"));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VcsNodeHaPeerEnqMessage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


VcsNodeHaPeerEnqMessage::VcsNodeHaPeerEnqMessage ()
: ManagementInterfaceMessage (VcsNodeHaObjectManager::getClassName (), VCS_NODE_HA_PEER_ENQ_MESSAGE),
    m_receiverCpState (0),
    m_receiverIpAddress (""),
    m_receiverPort (0),
    m_senderCpState (0),
    m_senderIpAddress (""),
    m_senderPort (0)
{
}

VcsNodeHaPeerEnqMessage::VcsNodeHaPeerEnqMessage (const string &objectManagerName, const UI32 &operationCode)
: ManagementInterfaceMessage (objectManagerName, operationCode),
    m_receiverCpState (0),
    m_receiverIpAddress (""),
    m_receiverPort (0),
    m_senderCpState (0),
    m_senderIpAddress (""),
    m_senderPort (0)
{
}

VcsNodeHaPeerEnqMessage::~VcsNodeHaPeerEnqMessage ()
{
}

void VcsNodeHaPeerEnqMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32    (&m_receiverCpState,    "receiverCpState"));
     addSerializableAttribute (new AttributeString  (&m_senderIpAddress,    "senderIpAddress"));
     addSerializableAttribute (new AttributeUI32    (&m_receiverPort,       "receiverPort"));
     addSerializableAttribute (new AttributeUI32    (&m_senderCpState,      "senderCpState"));
     addSerializableAttribute (new AttributeString  (&m_receiverIpAddress,  "receiverIpAddress"));
     addSerializableAttribute (new AttributeUI32    (&m_senderPort,         "senderPort"));
}

UI32 VcsNodeHaPeerEnqMessage::getReceiverCpState () const
{
    return m_receiverCpState;
}

void VcsNodeHaPeerEnqMessage::setReceiverCpState (const UI32 &cpState)
{
    m_receiverCpState = cpState;
}

UI32 VcsNodeHaPeerEnqMessage::getSenderCpState () const
{
    return m_senderCpState;
}

void VcsNodeHaPeerEnqMessage::setSenderCpState (const UI32 &cpState)
{
    m_senderCpState = cpState;
}

string  VcsNodeHaPeerEnqMessage::getReceiverIpAddress () const
{
    return m_receiverIpAddress;
}
void    VcsNodeHaPeerEnqMessage::setReceiverIpAddress (const string &receiverIpAddress)
{
    m_receiverIpAddress = receiverIpAddress;
}

string  VcsNodeHaPeerEnqMessage::getSenderIpAddress () const
{
    return m_senderIpAddress;
}

void    VcsNodeHaPeerEnqMessage::setSenderIpAddress (const string &senderIpAddress)
{
    m_senderIpAddress = senderIpAddress;   
}

UI32    VcsNodeHaPeerEnqMessage::getReceiverPort () const
{
    return m_receiverPort;
}

void    VcsNodeHaPeerEnqMessage::setReceiverPort (const UI32 &receiverPort)
{
    m_receiverPort = receiverPort;
}


UI32    VcsNodeHaPeerEnqMessage::getSenderPort () const
{
    return m_senderPort;
}

void    VcsNodeHaPeerEnqMessage::setSenderPort (const UI32 &senderPort)
{
    m_senderPort = senderPort;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VcsNodeHaHeartbeatMessage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VcsNodeHaHeartbeatMessage::VcsNodeHaHeartbeatMessage ()
    : VcsNodeHaPeerEnqMessage (VcsNodeHaObjectManager::getClassName (), VCS_NODE_HA_HEARTBEAT_MESSAGE),
    m_messageCount (0)
{
}

VcsNodeHaHeartbeatMessage::VcsNodeHaHeartbeatMessage (const UI32 &messageCount)
    : VcsNodeHaPeerEnqMessage (VcsNodeHaObjectManager::getClassName (), VCS_NODE_HA_HEARTBEAT_MESSAGE),
    m_messageCount (messageCount)
{
}

VcsNodeHaHeartbeatMessage::~VcsNodeHaHeartbeatMessage ()
{
}

void VcsNodeHaHeartbeatMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

     addSerializableAttribute (new AttributeUI32 (&m_messageCount,      "messageCount"));
     addSerializableAttribute (new AttributeUI32 (&m_senderCpState,     "senderCpState"));
     addSerializableAttribute (new AttributeUI32 (&m_receiverCpState,   "receiverCpState"));
}

void VcsNodeHaHeartbeatMessage::setMessageCount (const UI32 &messageCount)
{
    m_messageCount = messageCount;
}

UI32 VcsNodeHaHeartbeatMessage::getMessageCount () const
{
    return m_messageCount;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// VcsNodeCpStateChangeClientMessage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VcsNodeCpStateChangeClientMessage::VcsNodeCpStateChangeClientMessage (UI32 msgOpCode)
    : DcmManagementInterfaceMessage (msgOpCode),
    m_cpState (0),
    m_clientName ("")
{
}

VcsNodeCpStateChangeClientMessage::VcsNodeCpStateChangeClientMessage (UI32 msgOpCode, string &clientName)
    : DcmManagementInterfaceMessage (msgOpCode),
    m_cpState (0),
    m_clientName (clientName)
{
}

VcsNodeCpStateChangeClientMessage::~VcsNodeCpStateChangeClientMessage ()
{
}

void  VcsNodeCpStateChangeClientMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_cpState,"cpState"));
    addSerializableAttribute (new AttributeString(&m_clientName,"client_name"));
    addSerializableAttribute (new AttributeUI32(&m_tryCount,"tryCount"));
}

void  VcsNodeCpStateChangeClientMessage::setCpState(const UI32 &cpState)
{
    m_cpState  =  cpState;
}

UI32  VcsNodeCpStateChangeClientMessage::getCpState() const
{
    return (m_cpState);
}

string  VcsNodeCpStateChangeClientMessage::getClientName () const
{
    return (m_clientName);
}

void  VcsNodeCpStateChangeClientMessage::setClientName (const string &clientName)
{
    m_clientName = clientName;
}

UI32  VcsNodeCpStateChangeClientMessage::getTryCount () const
{
    return (m_tryCount);
}

void  VcsNodeCpStateChangeClientMessage::setTryCount (const UI32 &tryCount)
{
    m_tryCount = tryCount;
}

const void *VcsNodeCpStateChangeClientMessage::getCStructureForInputs ()
{
    CpState_InputMsg_t *pInput = new CpState_InputMsg_t;
	pInput->cpState     = m_cpState;
    return (pInput);
}


void VcsNodeCpStateChangeClientMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
// nothing to return           
}



}
