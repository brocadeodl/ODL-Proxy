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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "OSPF/Local/OSPFVirtualLinkLocalManagedObject.h"
#include "OSPF/Local/OSPFLocalObjectManager.h"
#include "OSPF/Local/OSPFTypes.h"
#include "ClientInterface/OSPF/OSPFAreaMessage.h"
#include "OSPF/Local/OSPFVirtualLinkLocalMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFVirtualLinkLocalManagedObject::OSPFVirtualLinkLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFVirtualLinkLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager)
    {
        m_virtualLinkRouterId = IpV4Address("0.0.0.0");
		m_HelloInterval.setUI32Value(0);
        m_HelloInterval.setIsUserConfigured(false);
        m_RouterDeadInterval = 0;
        m_RetransmitInterval = 0;
        m_TransDelay = 0;
        m_AuthType = OSPF_NO_ENCR;
        m_EncryptionType = OSPF_NO_ENCR;
        m_AuthKey = string("");
        m_MD5KeyActWaitTime = 0;
        m_MD5KeyId = 0;
        m_MD5Key = string("");
    }

    OSPFVirtualLinkLocalManagedObject::OSPFVirtualLinkLocalManagedObject (OSPFLocalObjectManager *pOSPFLocalObjectManager,const IpV4Address &virtualLinkRouterId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const OSPFEncrypType &AuthType,const OSPFEncrypType &EncryptionType,const string &AuthKey, const string &AuthNoEncryptKey, const UI32 &MD5KeyActWaitTime,const UI8 &MD5KeyId, const UI8 &MD5NoEncryptKeyId, const string &MD5Key, const string &MD5NoEncryptKey)
        : PrismElement  (pOSPFLocalObjectManager),
        PrismPersistableObject (OSPFVirtualLinkLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pOSPFLocalObjectManager),
        DcmManagedObject (pOSPFLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pOSPFLocalObjectManager),
        m_virtualLinkRouterId    (virtualLinkRouterId),
        m_HelloInterval    (HelloInterval),
        m_RouterDeadInterval    (RouterDeadInterval),
        m_RetransmitInterval    (RetransmitInterval),
        m_TransDelay    (TransDelay),
        m_AuthType    (AuthType),
        m_EncryptionType    (EncryptionType),
        m_AuthKey    (AuthKey),
        m_AuthNoEncryptKey    (AuthNoEncryptKey),
        m_MD5KeyActWaitTime    (MD5KeyActWaitTime),
        m_MD5KeyId    (MD5KeyId),
        m_MD5NoEncryptKeyId    (MD5NoEncryptKeyId),
        m_MD5Key    (MD5Key),
        m_MD5NoEncryptKey    (MD5NoEncryptKey)
    {
    }

    OSPFVirtualLinkLocalManagedObject::~OSPFVirtualLinkLocalManagedObject ()
    {
    }

    string  OSPFVirtualLinkLocalManagedObject::getClassName()
    {
        return ("OSPFVirtualLinkLocalManagedObject");
    }

    void  OSPFVirtualLinkLocalManagedObject::setupAttributesForPersistence()
    {
        UI32 defaultMD5KeyActWaitTime = OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME;
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4Address(&m_virtualLinkRouterId,"virtualLinkRouterId", ospf_virt_link_neighbor));
        addPersistableAttribute (new AttributeUI32UC(&m_HelloInterval,true,"HelloInterval"));
        addPersistableAttribute (new AttributeUI32UC(&m_RouterDeadInterval,true,"RouterDeadInterval"));
        addPersistableAttribute (new AttributeUI32UC(&m_RetransmitInterval,true,"RetransmitInterval"));
        addPersistableAttribute (new AttributeUI32UC(&m_TransDelay,true,"TransDelay"));
        addPersistableAttribute (new AttributeUI32UC(&m_AuthType,true,"AuthType"));
        addPersistableAttribute (new AttributeUI32UC(&m_EncryptionType,true,"EncryptionType"));
        addPersistableAttribute (new AttributeStringUC(&m_AuthKey,true,"AuthKey"));
        addPersistableAttribute (new AttributeStringUC(&m_AuthNoEncryptKey,true,"AuthNoEncryptKey"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5KeyActWaitTime,true,true,defaultMD5KeyActWaitTime,"MD5KeyActWaitTime"));
        //addPersistableAttribute (new AttributeUI8(&m_MD5KeyId,"MD5KeyId"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5KeyId,true,"MD5KeyId"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5NoEncryptKeyId,true,"MD5NoEncryptKeyId"));
        addPersistableAttribute (new AttributeStringUC(&m_MD5Key,true,"MD5Key"));
        addPersistableAttribute (new AttributeStringUC(&m_MD5NoEncryptKey,true,"MD5NoEncryptKey"));
        
        setUserTagForAttribute ("HelloInterval", ospf_hello_interval);
        setUserTagForAttribute ("RouterDeadInterval", ospf_dead_interval);
        setUserTagForAttribute ("RetransmitInterval", ospf_retransmit_interval);
        setUserTagForAttribute ("TransDelay", ospf_transmit_delay);
        setUserTagForAttribute ("AuthType", ospf_key);
        setUserTagForAttribute ("EncryptionType", ospf_encrypttype);
        setUserTagForAttribute ("AuthKey", ospf_auth_key);
        setUserTagForAttribute ("AuthNoEncryptKey", ospf_no_encrypt_auth_key);
        setUserTagForAttribute ("MD5KeyActWaitTime", ospf_key_activation_wait_time);
        setUserTagForAttribute ("MD5KeyId", ospf_key_id);
        setUserTagForAttribute ("MD5NoEncryptKeyId", ospf_no_encrypt_key_id);
        setUserTagForAttribute ("MD5Key", ospf_md5_authentication_key);
        setUserTagForAttribute ("MD5NoEncryptKey", ospf_no_encrypt_key);
    }

    void  OSPFVirtualLinkLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_virtualLinkRouterId,"virtualLinkRouterId", ospf_virt_link_neighbor));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_HelloInterval,true,"HelloInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_RouterDeadInterval,true,"RouterDeadInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_RetransmitInterval,true,"RetransmitInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_TransDelay,true,"TransDelay"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_AuthType,true,"AuthType"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_EncryptionType,true,"EncryptionType"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_AuthKey,true,"AuthKey"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_AuthNoEncryptKey,true,"AuthNoEncryptKey"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5KeyActWaitTime,true,"MD5KeyActWaitTime"));
        //addPersistableAttributeForCreate  (new AttributeUI8(&m_MD5KeyId,"MD5KeyId"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5KeyId,true,"MD5KeyId"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5NoEncryptKeyId,true,"MD5NoEncryptKeyId"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_MD5Key,true,"MD5Key"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_MD5NoEncryptKey,true,"MD5NoEncryptKey"));
    }

    void OSPFVirtualLinkLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
	keyName.push_back ("virtualLinkRouterId");
	setUserDefinedKeyCombination (keyName);
    }

    void  OSPFVirtualLinkLocalManagedObject::setVirtualLinkRouterId(const IpV4Address &virtualLinkRouterId)
    {
        m_virtualLinkRouterId  =  virtualLinkRouterId;
    }

    IpV4Address  OSPFVirtualLinkLocalManagedObject::getVirtualLinkRouterId() const
    {
        return (m_virtualLinkRouterId);
    }

    void  OSPFVirtualLinkLocalManagedObject::setHelloInterval(const UI32 &HelloInterval, const bool &set)
    {
        m_HelloInterval.setUI32Value(HelloInterval);
        if(HelloInterval != OSPF_DEFAULT_HELLO_INTERVAL)
            m_HelloInterval.setIsUserConfigured(set);
        else
            m_HelloInterval.setIsUserConfigured(false);
    }

    UI32  OSPFVirtualLinkLocalManagedObject::getHelloInterval() const
    {
        return (m_HelloInterval.getUI32Value());
    }

    void  OSPFVirtualLinkLocalManagedObject::setRouterDeadInterval(const UI32 &RouterDeadInterval, const bool &set)
    {
        m_RouterDeadInterval.setUI32Value(RouterDeadInterval);
        if(RouterDeadInterval != OSPF_DEFAULT_ROUTER_DEAD_INTERVAL)
            m_RouterDeadInterval.setIsUserConfigured(set);
        else    
            m_RouterDeadInterval.setIsUserConfigured(false);
    }

    UI32  OSPFVirtualLinkLocalManagedObject::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval.getUI32Value());
    }

    void  OSPFVirtualLinkLocalManagedObject::setRetransmitInterval(const UI32 &RetransmitInterval, const bool &set)
    {
        m_RetransmitInterval.setUI32Value(RetransmitInterval);
        if(RetransmitInterval != OSPF_DEFAULT_RETRANSMIT_INTERVAL)
            m_RetransmitInterval.setIsUserConfigured(set);
        else    
            m_RetransmitInterval.setIsUserConfigured(false);
    }

    UI32  OSPFVirtualLinkLocalManagedObject::getRetransmitInterval() const
    {
        return (m_RetransmitInterval.getUI32Value());
    }

    void  OSPFVirtualLinkLocalManagedObject::setTransDelay(const UI32 &TransDelay, const bool &set)
    {
        m_TransDelay.setUI32Value(TransDelay);
        if(TransDelay != OSPF_DEFAULT_TRANSMIT_DELAY)
            m_TransDelay.setIsUserConfigured(set);
        else    
            m_TransDelay.setIsUserConfigured(false);
    }

    UI32  OSPFVirtualLinkLocalManagedObject::getTransDelay() const
    {
        return (m_TransDelay.getUI32Value());
    }

    void  OSPFVirtualLinkLocalManagedObject::setAuthType(const OSPFEncrypType &AuthType, const bool &set)
    {
        m_AuthType.setUI32Value((UI32)AuthType);
        m_AuthType.setIsUserConfigured(set);
    }

    OSPFEncrypType OSPFVirtualLinkLocalManagedObject::getAuthType() const
    {
        return (OSPFEncrypType)(m_AuthType.getUI32Value());
    }

    bool OSPFVirtualLinkLocalManagedObject::getIsSetAuthType() const
    {
        return (m_AuthType.getIsUserConfigured());
    }

    void  OSPFVirtualLinkLocalManagedObject::setEncryptionType(const OSPFEncrypType &EncryptionType, const bool &set)
    {
        m_EncryptionType.setUI32Value((UI32)EncryptionType);
        m_EncryptionType.setIsUserConfigured(set);
    }

    OSPFEncrypType  OSPFVirtualLinkLocalManagedObject::getEncryptionType() const
    {
        return (OSPFEncrypType)(m_EncryptionType.getUI32Value());
    }

    bool OSPFVirtualLinkLocalManagedObject::getIsSetEncryptionType() const
    {
        return (m_EncryptionType.getIsUserConfigured());
    }

    void  OSPFVirtualLinkLocalManagedObject::setAuthKey(const string &AuthKey, const bool &set)
    {
        m_AuthKey.setStringValue(AuthKey);
        m_AuthKey.setIsUserConfigured(set);
    }

    string  OSPFVirtualLinkLocalManagedObject::getAuthKey() const
    {
        return (m_AuthKey.getStringValue());
    }

    bool  OSPFVirtualLinkLocalManagedObject::getIsSetAuthKey() const
    {
        return (m_AuthKey.getIsUserConfigured());
    }

    void  OSPFVirtualLinkLocalManagedObject::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime, const bool &set)
    {
        m_MD5KeyActWaitTime.setUI32Value(MD5KeyActWaitTime);
        m_MD5KeyActWaitTime.setIsUserConfigured(set);
    }

    UI32  OSPFVirtualLinkLocalManagedObject::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime.getUI32Value());
    }

    void  OSPFVirtualLinkLocalManagedObject::setMD5KeyId(const UI8 &MD5KeyId, const bool &set)
    {
        m_MD5KeyId.setUI32Value((UI32)MD5KeyId);
        m_MD5KeyId.setIsUserConfigured(set);
        //m_MD5KeyId = MD5KeyId;
    }

    UI8  OSPFVirtualLinkLocalManagedObject::getMD5KeyId() const
    {
        return (UI8)(m_MD5KeyId.getUI32Value());
        //return (m_MD5KeyId);
    }

    bool OSPFVirtualLinkLocalManagedObject::getIsSetMD5KeyId() const
    {
        return (m_MD5KeyId.getIsUserConfigured());
    }

    void  OSPFVirtualLinkLocalManagedObject::setMD5Key(const string &MD5Key, const bool &set)
    {
        m_MD5Key.setStringValue(MD5Key);
        m_MD5Key.setIsUserConfigured(set);
    }

    string  OSPFVirtualLinkLocalManagedObject::getMD5Key() const
    {
        return (m_MD5Key.getStringValue());
    }
    
    bool  OSPFVirtualLinkLocalManagedObject::getIsSetMD5Key() const
    {
        return (m_MD5Key.getIsUserConfigured());
    }

    void OSPFVirtualLinkLocalManagedObject::updateLocalMessageFromMO(OSPFVirtualLinkLocalMessage *pMsg)
    {
		trace (TRACE_LEVEL_DEBUG, string("OSPFVirtualLinkLocalManagedObject::updateLocalMessageFromMO: Partition Name=") + pMsg->getPartitionName());
		setUserSpecifiedPartitionName(pMsg->getPartitionName());
        if( (pMsg->getOpCode()) != OSPF_REMOVE)
        {
            switch(pMsg->getCmdCode())
            {
                case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
                case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
                    if( (pMsg->getEncryptionType()) == OSPF_INVALID_ENCR )    
                        pMsg->setEncryptionType(getEncryptionType());

                    if( !((pMsg->getAuthKey()).length()) )    
                        pMsg->setAuthKey(getAuthKey());
                    
                    break;

                case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
                case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
                case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
                    if((pMsg->getMD5KeyId()) == OSPF_INVALID_ENCR)
                        pMsg->setMD5KeyId(getMD5KeyId());
                    if (pMsg->getAuthType() == OSPF_INVALID_ENCR)
                        pMsg->setAuthType(getAuthType());
                    if(!((pMsg->getMD5Key()).length()))    
                        pMsg->setMD5Key(getMD5Key());
                    break;

                default:
                    break;
            }
        }
    }

	void OSPFVirtualLinkLocalManagedObject::updateMOFromLocalMessage(OSPFVirtualLinkLocalMessage *pMsg)
	{
		trace (TRACE_LEVEL_INFO, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: opCode=") + pMsg->getOpCode() + " cmdCode=" + pMsg->getCmdCode());
		
		trace (TRACE_LEVEL_DEBUG, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: Partition Name=") + pMsg->getPartitionName());
		setUserSpecifiedPartitionName(pMsg->getPartitionName());
		m_virtualLinkRouterId = pMsg->getVirtualLinkRouterId();

		switch (pMsg->getOpCode()) {
			case OSPF_CREATE:
			case OSPF_SET_ELEM:
				switch(pMsg->getCmdCode())
				{
					case OSPF_AREA_CONFIG_VIRTUAL_LINK:
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_HELLO_INT:
						setHelloInterval(pMsg->getHelloInterval(),true);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_DEAD_INT:
						setRouterDeadInterval(pMsg->getRouterDeadInterval(),true);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_RETRANS_INT:
						setRetransmitInterval(pMsg->getRetransmitInterval(),true);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_TRANS_DLY:
						setTransDelay(pMsg->getTransDelay(),true);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
                        if( (pMsg->getEncryptionType()) != OSPF_INVALID_ENCR ) 
    						setEncryptionType(pMsg->getEncryptionType(),true);
                        if( (pMsg->getAuthKey()).length() )    
    						setAuthKey(pMsg->getAuthKey(),true);
                        setMD5KeyId(0, false);    
                        setMD5Key("",false);
                        setAuthType(OSPF_NO_ENCR, false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_ACT_WAIT_TIME:
						setMD5KeyActWaitTime(pMsg->getMD5KeyActWaitTime(),true);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
                        if((pMsg->getMD5KeyId()) != OSPF_INVALID_ENCR)
    						setMD5KeyId(pMsg->getMD5KeyId(),true);
                        if((pMsg->getMD5Key()).length())    
    						setMD5Key(pMsg->getMD5Key(),true);
						if (pMsg->getAuthType() != OSPF_INVALID_ENCR)
							setAuthType(pMsg->getAuthType(),true);
                        setEncryptionType(OSPF_NO_ENCR, false);  
                        setAuthKey("", false);
						break;

					default:
						trace (TRACE_LEVEL_ERROR, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: unknown cmdcode"));
						break;
				}
				break;
			case OSPF_REMOVE:
				switch (pMsg->getCmdCode())
				{
					case OSPF_AREA_CONFIG_VIRTUAL_LINK:
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_HELLO_INT:
						setHelloInterval(pMsg->getHelloInterval(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_DEAD_INT:
						setRouterDeadInterval(pMsg->getRouterDeadInterval(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_RETRANS_INT:
						setRetransmitInterval(pMsg->getRetransmitInterval(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_TRANS_DLY:
						setTransDelay(pMsg->getTransDelay(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
						setEncryptionType(pMsg->getEncryptionType(),false);
						setAuthKey(pMsg->getAuthKey(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_ACT_WAIT_TIME:
						setMD5KeyActWaitTime(pMsg->getMD5KeyActWaitTime(),false);
						break;

					case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
					case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
						setAuthType(OSPF_NO_ENCR,false);
						setMD5Key(string(""),false);
						setMD5KeyId(0,false);
						break;

					default:
						trace (TRACE_LEVEL_ERROR, string("OSPFLocalConfWorker::OSPFUpdateVirtualLinkMOStep: unknown cmdcode"));
						break;
				}
			default:
				break;
		}
	}

	void OSPFVirtualLinkLocalManagedObject::updateClientMessageForReplay(OSPFAreaMessage *pMsg)
	{
		pMsg->setOpCode(OSPF_CREATE);
		pMsg->setCmdCode(OSPF_AREA_VIRTUAL_LINK_REPLAY);
		pMsg->setVirtualLinkRouterId(m_virtualLinkRouterId);

		for (UI32 idx = OSPF_AREA_CONFIG_VIRTUAL_LINK; idx <= OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY; idx ++) {
			switch (idx) {
				case OSPF_AREA_CONFIG_VIRTUAL_LINK:
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_HELLO_INT:
					if (m_HelloInterval.getIsUserConfigured()) {
						pMsg->setHelloInterval(m_HelloInterval.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_HELLO_INT);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_DEAD_INT:
					if (m_RouterDeadInterval.getIsUserConfigured()) {
						pMsg->setRouterDeadInterval(m_RouterDeadInterval.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_DEAD_INT);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_RETRANS_INT:
					if (m_RetransmitInterval.getIsUserConfigured()) {
						pMsg->setRetransmitInterval(m_RetransmitInterval.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_RETRANS_INT);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_TRANS_DLY:
					if (m_TransDelay.getIsUserConfigured()) {
						pMsg->setTransDelay(m_TransDelay.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_TRANS_DLY);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE:
					if (m_EncryptionType.getIsUserConfigured()) {
						pMsg->setEncryptionType((OSPFEncrypType)m_EncryptionType.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_ENCRYPTTYPE);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY:
					if (m_AuthKey.getIsUserConfigured()) {
						pMsg->setAuthKey(m_AuthKey.getStringValue());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_AUTH_KEY);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_ACT_WAIT_TIME:
					if (m_MD5KeyActWaitTime.getIsUserConfigured()) {
						pMsg->setMD5KeyActWaitTime(m_MD5KeyActWaitTime.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_ACT_WAIT_TIME);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID:
					if (m_MD5KeyId.getIsUserConfigured()) {
						pMsg->setMD5KeyId(m_MD5KeyId.getUI32Value());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_KEY_ID);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY:
					if (m_MD5KeyId.getIsUserConfigured()) {
						pMsg->setAuthType((OSPFEncrypType)(m_AuthType.getUI32Value()));
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_SWITCH_KEY);
					}
					break;
				case OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY:
					if (m_MD5Key.getIsUserConfigured()) {
						pMsg->setMD5Key(m_MD5Key.getStringValue());
						pMsg->setReplayCmdCodeBitmap(OSPF_AREA_CONFIG_VIRTUAL_LINK_MD5_AUTH_KEY);
					}
					break;
				default:
					break;
			}
		}
	}
}
