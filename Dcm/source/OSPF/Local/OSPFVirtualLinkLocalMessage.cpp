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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "OSPF/Local/OSPFVirtualLinkLocalMessage.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "ClientInterface/OSPF/OSPFMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{
    OSPFVirtualLinkLocalMessage::OSPFVirtualLinkLocalMessage ()
        //: ManagementInterfaceMessage (OSPFLocalObjectManager::getClassName(), OSPFVIRTUALLINKLOCAL)
        : PrismMessage (OSPFLocalObjectManager::getPrismServiceId (), OSPFVIRTUALLINKLOCAL)
    {
		m_virtualLinkRouterId.fromString("0.0.0.0");
		m_areaId = "";
		m_opCode = 0;
		m_cmdCode  = 0;
		m_HelloInterval = 0;
		m_RouterDeadInterval = 0;
		m_RetransmitInterval = 0;
		m_TransDelay = 0;
		m_AuthType = OSPF_INVALID_ENCR;
		m_EncryptionType = OSPF_INVALID_ENCR;
		m_AuthKey = "";
		m_MD5KeyActWaitTime = 0;
		m_MD5KeyId = OSPF_INVALID_ENCR;
		m_MD5Key = "";
    }


    OSPFVirtualLinkLocalMessage::~OSPFVirtualLinkLocalMessage ()
    {
    }

    void  OSPFVirtualLinkLocalMessage::setupAttributesForSerialization()
    {
        //ManagementInterfaceMessage::setupAttributesForSerialization ();
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4Address(&m_virtualLinkRouterId, "virtualLinkRouterId"));
        addSerializableAttribute (new AttributeString(&m_areaId, "areaId"));
        addSerializableAttribute (new AttributeUI32(&m_opCode, "opCode"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_HelloInterval, "HelloInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RouterDeadInterval, "RouterDeadInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RetransmitInterval, "RetransmitInterval"));
        addSerializableAttribute (new AttributeUI32(&m_TransDelay, "TransDelay"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_AuthType), "AuthType"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_EncryptionType), "EncryptionType"));
        addSerializableAttribute (new AttributeString(&m_AuthKey, "AuthKey"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyActWaitTime, "MD5KeyActWaitTime"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyId, "MD5KeyId"));
        addSerializableAttribute (new AttributeString(&m_MD5Key, "MD5Key"));
    }

    void OSPFVirtualLinkLocalMessage::setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId)
    {
        m_virtualLinkRouterId = virtualLinkRouterId;
    }    
    
    IpV4Address OSPFVirtualLinkLocalMessage::getVirtualLinkRouterId()  const
    {
        return (m_virtualLinkRouterId);
    }   

    void OSPFVirtualLinkLocalMessage::setAreaId(const string &id)
    {
        m_areaId = id;
    }
    
    string OSPFVirtualLinkLocalMessage::getAreaId()  const
    {
        return (m_areaId);
    }   

    void  OSPFVirtualLinkLocalMessage::setOpCode(const unsigned int &opCode)
    {
        m_opCode  =  opCode;
    }

    unsigned int OSPFVirtualLinkLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }
        
    void  OSPFVirtualLinkLocalMessage::setCmdCode(const unsigned int &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    unsigned int OSPFVirtualLinkLocalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
        
    void  OSPFVirtualLinkLocalMessage::setHelloInterval(const UI32 &HelloInterval)
    {
        m_HelloInterval  =  HelloInterval;
    }

    UI32  OSPFVirtualLinkLocalMessage::getHelloInterval() const
    {
        return (m_HelloInterval);
    }

    void  OSPFVirtualLinkLocalMessage::setRouterDeadInterval(const UI32 &RouterDeadInterval)
    {
        m_RouterDeadInterval  =  RouterDeadInterval;
    }

    UI32  OSPFVirtualLinkLocalMessage::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval);
    }

    void  OSPFVirtualLinkLocalMessage::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFVirtualLinkLocalMessage::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFVirtualLinkLocalMessage::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFVirtualLinkLocalMessage::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFVirtualLinkLocalMessage::setAuthType(const OSPFEncrypType &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    OSPFEncrypType OSPFVirtualLinkLocalMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFVirtualLinkLocalMessage::setEncryptionType(const OSPFEncrypType &EncryptionType)
    {
        m_EncryptionType  =  EncryptionType;
    }

    OSPFEncrypType  OSPFVirtualLinkLocalMessage::getEncryptionType() const
    {
        return (m_EncryptionType);
    }

    void  OSPFVirtualLinkLocalMessage::setAuthKey(const string &AuthKey)
    {
        m_AuthKey  =  AuthKey;
    }

    string  OSPFVirtualLinkLocalMessage::getAuthKey() const
    {
        return (m_AuthKey);
    }

    void  OSPFVirtualLinkLocalMessage::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime)
    {
        m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
    }

    UI32  OSPFVirtualLinkLocalMessage::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime);
    }

    void  OSPFVirtualLinkLocalMessage::setMD5KeyId(const UI8 &MD5KeyId)
    {
        m_MD5KeyId  =  MD5KeyId;
    }

    UI32  OSPFVirtualLinkLocalMessage::getMD5KeyId() const
    {
        return (m_MD5KeyId);
    }

    void  OSPFVirtualLinkLocalMessage::setMD5Key(const string &MD5Key)
    {
        m_MD5Key  =  MD5Key;
    }

    string  OSPFVirtualLinkLocalMessage::getMD5Key() const
    {
        return (m_MD5Key);
    }

	void 
    OSPFVirtualLinkLocalMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		return;
	}
}
