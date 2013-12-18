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
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#include "VcsFabric/Local/VcsFabricLocalMessages.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalTypes.h"

namespace DcmNs
{
VcsFabricGetSwitchIdentifierInfoMessage::VcsFabricGetSwitchIdentifierInfoMessage ()
: PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_GET_SWITCH_IDENTIFIER_INFO)
{
}

VcsFabricGetSwitchIdentifierInfoMessage::~VcsFabricGetSwitchIdentifierInfoMessage ()
{
}

void VcsFabricGetSwitchIdentifierInfoMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_switchname,      "switchname"));
    addSerializableAttribute (new AttributeString (&m_eth0IpAddress,   "eth0IpAddress"));
    addSerializableAttribute (new AttributeString (&m_eth0IPv6Address, "eth0IPv6Address"));
}

string VcsFabricGetSwitchIdentifierInfoMessage::getSwitchname () const
{
	return m_switchname;
}

void VcsFabricGetSwitchIdentifierInfoMessage::setSwitchname (string switchname)
{
    m_switchname = switchname;
}

string VcsFabricGetSwitchIdentifierInfoMessage::getEth0IpAddress () const
{
	return m_eth0IpAddress;
}

void VcsFabricGetSwitchIdentifierInfoMessage::setEth0IpAddress (string eth0IpAddress)
{
    m_eth0IpAddress = eth0IpAddress;
}

string VcsFabricGetSwitchIdentifierInfoMessage::getEth0IPv6Address () const
{
      return m_eth0IPv6Address;
}

void VcsFabricGetSwitchIdentifierInfoMessage::setEth0IPv6Address (string eth0IPv6Address)
{
    m_eth0IPv6Address = eth0IPv6Address;
}

VcsNodeResetDomainBitMapMessage::VcsNodeResetDomainBitMapMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_RESET_DOMAIN_BITMAP_MESSAGE),
      m_isWarmRecovery (false)
{
}

VcsNodeResetDomainBitMapMessage::~VcsNodeResetDomainBitMapMessage ()
{
}

void VcsNodeResetDomainBitMapMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization (); 

    addSerializableAttribute (new AttributeBool (&m_isWarmRecovery, "isWarmRecovery"));
}

void VcsNodeResetDomainBitMapMessage::setIsWarmRecoveryInvolved (const bool &isWarmRecovery)
{
    m_isWarmRecovery = isWarmRecovery;
}

bool VcsNodeResetDomainBitMapMessage::getIsWarmRecoveryInvolved ()
{
    return (m_isWarmRecovery);
}

VcsNodePrepareDomainListAfterHaRecoveryMessage::VcsNodePrepareDomainListAfterHaRecoveryMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_PREPARE_DOMAIN_LIST_MESSAGE),
      m_isWarmRecovery (false)

{
}

VcsNodePrepareDomainListAfterHaRecoveryMessage::~VcsNodePrepareDomainListAfterHaRecoveryMessage ()
{
}

void VcsNodePrepareDomainListAfterHaRecoveryMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeBool (&m_isWarmRecovery, "isWarmRecovery"));
}

void VcsNodePrepareDomainListAfterHaRecoveryMessage::setIsWarmRecoveryInvolved (const bool &isWarmRecovery)
{
    m_isWarmRecovery = isWarmRecovery;
}

bool VcsNodePrepareDomainListAfterHaRecoveryMessage::getIsWarmRecoveryInvolved ()
{
    return (m_isWarmRecovery);
}

VcsNodeEnableSlotScnHandlingMessage::VcsNodeEnableSlotScnHandlingMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_ENABLE_SLOT_SCN_MESSAGE)
{
}

VcsNodeEnableSlotScnHandlingMessage::~VcsNodeEnableSlotScnHandlingMessage ()
{
}

void VcsNodeEnableSlotScnHandlingMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
}

VcsNodeConfigSnapshotMessage::VcsNodeConfigSnapshotMessage ( const UI32 &opcode)
    : PrismMessage  ( VcsFabricLocalObjectManager::getPrismServiceId (), opcode ),
      m_snapshotId  (""),
      m_rbridgeId   (0),
      m_isNodePrimary(false)
{
}

VcsNodeConfigSnapshotMessage::~VcsNodeConfigSnapshotMessage ()
{
}

void VcsNodeConfigSnapshotMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString      (&m_snapshotId,     "snapshotId"));
    addSerializableAttribute (new AttributeUI32        (&m_rbridgeId,      "rbridgeId"));
    addSerializableAttribute (new AttributeBool        (&m_isNodePrimary,  "isNodePrimary"));
}

void VcsNodeConfigSnapshotMessage::setSnapshotId  (const string &snapshotId )
{
    m_snapshotId = snapshotId ;
}

string VcsNodeConfigSnapshotMessage::getSnapshotId  () const
{
    return ( m_snapshotId );
}

void VcsNodeConfigSnapshotMessage::setRbridgeId (const UI32 &rbridgeId )
{
    m_rbridgeId = rbridgeId;
}

UI32 VcsNodeConfigSnapshotMessage::getRbridgeId () const
{
    return ( m_rbridgeId );
}

void VcsNodeConfigSnapshotMessage::setIsNodePrimary ( const bool &isNodePrimary )
{   
    m_isNodePrimary = isNodePrimary;
}

bool VcsNodeConfigSnapshotMessage::getIsNodePrimary () const
{
    return (m_isNodePrimary);
}

VcsFabricCollectSwitchIdentifierInfoMessage::VcsFabricCollectSwitchIdentifierInfoMessage ()
: PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_COLLECT_SWITCH_IDENTIFIER_INFO)
{
}

VcsFabricCollectSwitchIdentifierInfoMessage::~VcsFabricCollectSwitchIdentifierInfoMessage ()
{
}

void VcsFabricCollectSwitchIdentifierInfoMessage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeUI32Vector   (&m_rbridgeIdVector,        "rbridgeIdVector"));
    addSerializableAttribute (new AttributeStringVector (&m_switchnameVector,       "switchnameVector"));
    addSerializableAttribute (new AttributeStringVector (&m_eth0IpAddressVector,    "eth0IpAddressVector"));
    addSerializableAttribute (new AttributeStringVector (&m_eth0IPv6AddressVector,  "eth0IPv6AddressVector"));
}

vector<UI32> VcsFabricCollectSwitchIdentifierInfoMessage::getRbridgeIdVector () const
{
    return m_rbridgeIdVector;
}

void VcsFabricCollectSwitchIdentifierInfoMessage::setRbridgeIdVector (vector<UI32> rbridgeIdVector)
{
    m_rbridgeIdVector = rbridgeIdVector;
}

vector<string> VcsFabricCollectSwitchIdentifierInfoMessage::getSwitchnameVector () const
{
    return m_switchnameVector;
}

void VcsFabricCollectSwitchIdentifierInfoMessage::setSwitchnameVector (vector<string> switchnameVector)
{
    m_switchnameVector = switchnameVector;
}

vector<string> VcsFabricCollectSwitchIdentifierInfoMessage::getEth0IpAddressVector () const
{
    return m_eth0IpAddressVector;
}

void VcsFabricCollectSwitchIdentifierInfoMessage::setEth0IpAddressVector (vector<string> eth0IpAddressVector)
{
    m_eth0IpAddressVector = eth0IpAddressVector;
}

vector<string> VcsFabricCollectSwitchIdentifierInfoMessage::getEth0IPv6AddressVector () const
{
    return m_eth0IPv6AddressVector;
}

void VcsFabricCollectSwitchIdentifierInfoMessage::setEth0IPv6AddressVector (vector<string> eth0IPv6AddressVector)
{
    m_eth0IPv6AddressVector = eth0IPv6AddressVector;
}

VirtualIpLocalShowVipMessage::VirtualIpLocalShowVipMessage ()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VIRTUAL_IP_LOCAL_SHOW_VIP)
{
}

VirtualIpLocalShowVipMessage::VirtualIpLocalShowVipMessage (const string &VirtualIpAddress, const UI32 &pNodeId)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VIRTUAL_IP_LOCAL_SHOW_VIP),
    m_VirtualIpAddress  (VirtualIpAddress),
    m_pNodeId       (pNodeId)
{
}

VirtualIpLocalShowVipMessage::~VirtualIpLocalShowVipMessage ()
{
}

void  VirtualIpLocalShowVipMessage::setupAttributesForSerialization()
{
    PrismMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeString(&m_VirtualIpAddress,"VirtualIpAddress"));
    addSerializableAttribute (new AttributeUI32(&m_pNodeId,"pNodId"));
}

void  VirtualIpLocalShowVipMessage::setVirtualIpAddress(const string &VirtualIpAddress)
{
    m_VirtualIpAddress  =  VirtualIpAddress;
}

string  VirtualIpLocalShowVipMessage::getVirtualIpAddress() const
{
    return (m_VirtualIpAddress);
}

void  VirtualIpLocalShowVipMessage::setpNodeId(const UI32 &pNodeId)
{
    m_pNodeId  =  pNodeId;
}

UI32  VirtualIpLocalShowVipMessage::getpNodeId() const
{
    return (m_pNodeId);
}

VcsNodeHaSyncConfigToKernelMessaage::VcsNodeHaSyncConfigToKernelMessaage (const string &globalConfigs)
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_HA_SYNC_CONFIG_TO_KERNEL),
      m_globalConfigs (globalConfigs)
{
}

VcsNodeHaSyncConfigToKernelMessaage::VcsNodeHaSyncConfigToKernelMessaage()
    : PrismMessage (VcsFabricLocalObjectManager::getPrismServiceId (), VCS_NODE_HA_SYNC_CONFIG_TO_KERNEL)
{       
}     
      
VcsNodeHaSyncConfigToKernelMessaage::~VcsNodeHaSyncConfigToKernelMessaage ()
{
}

void VcsNodeHaSyncConfigToKernelMessaage::setupAttributesForSerialization ()
{
    PrismMessage::setupAttributesForSerialization ();

    addSerializableAttribute (new AttributeString (&m_globalConfigs, "globalConfigs"));
}


string VcsNodeHaSyncConfigToKernelMessaage::getGlobalConfigs () const
{
    return (m_globalConfigs);
}   
    
void VcsNodeHaSyncConfigToKernelMessaage::setGlobalConfigs (const string &globalConfigs)
{
    m_globalConfigs = globalConfigs;
}

}
