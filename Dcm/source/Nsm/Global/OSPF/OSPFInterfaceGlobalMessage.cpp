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

#include "Nsm/Global/OSPF/OSPFInterfaceGlobalMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFInterfaceGlobalMessage::OSPFInterfaceGlobalMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALOSPFINTERFACECONFIG)
    {
        m_FilterAll = false;
        m_FilterOption = OSPF_INTF_ALLOW_INVALID;
        m_MD5KeyId = 256;
        m_MD5Key.clear();
        m_MD5EncryptionType = OSPF_INVALID_ENCR;
        m_EncryptionType = OSPF_INVALID_ENCR;
        m_AuthKey.clear(); 
    }

    OSPFInterfaceGlobalMessage::OSPFInterfaceGlobalMessage (const UI8 &opCode,const UI8 &cmdCode,const string &intfName,const UI32 &intfType,const string &areaId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const OSPFEncrypType &MD5EncryptionType,const string &AuthKey,const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI32 &MD5KeyId,const string &MD5Key,const bool &FilterAll,const UI32 &FilterOption,const bool &mtuIgnore,const bool &Passive,const OSPFIntfNetworkType &NetworkType)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALOSPFINTERFACECONFIG),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
		m_intfName  (intfName),
		m_intfType  (intfType),
        m_areaId    (areaId),
        m_HelloInterval    (HelloInterval),
        m_RouterDeadInterval    (RouterDeadInterval),
        m_RetransmitInterval    (RetransmitInterval),
        m_TransDelay    (TransDelay),
        m_Cost    (Cost),
        m_Priority    (Priority),
        m_AuthType    (AuthType),
        m_EncryptionType    (EncryptionType),
        m_MD5EncryptionType    (MD5EncryptionType),
        m_AuthKey    (AuthKey),
        m_AuthChangeWaitTime    (AuthChangeWaitTime),
        m_MD5KeyActWaitTime    (MD5KeyActWaitTime),
        m_MD5KeyId    (MD5KeyId),
        m_MD5Key    (MD5Key),
        m_FilterAll    (FilterAll),
		m_FilterOption (FilterOption),
        m_mtuIgnore    (mtuIgnore),
        m_Passive    (Passive),
        m_NetworkType    (NetworkType)
    {
    }

    OSPFInterfaceGlobalMessage::~OSPFInterfaceGlobalMessage ()
    {
    }

    void  OSPFInterfaceGlobalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeUI32(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_areaId,"areaId"));
        addSerializableAttribute (new AttributeUI32(&m_HelloInterval,"HelloInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RouterDeadInterval,"RouterDeadInterval"));
        addSerializableAttribute (new AttributeUI32(&m_RetransmitInterval,"RetransmitInterval"));
        addSerializableAttribute (new AttributeUI32(&m_TransDelay,"TransDelay"));
        addSerializableAttribute (new AttributeUI32(&m_Cost,"Cost"));
        addSerializableAttribute (new AttributeUI32(&m_Priority,"Priority"));
        addSerializableAttribute (new AttributeUI32(&m_AuthType,"AuthType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_EncryptionType,"EncryptionType"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_MD5EncryptionType,"MD5EncryptionType"));
        addSerializableAttribute (new AttributeString(&m_AuthKey,"AuthKey"));
        addSerializableAttribute (new AttributeUI32(&m_AuthChangeWaitTime,"AuthChangeWaitTime"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyActWaitTime,"MD5KeyActWaitTime"));
        addSerializableAttribute (new AttributeUI32(&m_MD5KeyId,"MD5KeyId"));
        addSerializableAttribute (new AttributeString(&m_MD5Key,"MD5Key"));
        addSerializableAttribute (new AttributeBool(&m_FilterAll,"FilterAll"));
        addSerializableAttribute (new AttributeUI32(&m_FilterOption,"FilterOption"));
        addSerializableAttribute (new AttributeBool(&m_mtuIgnore,"mtuIgnore"));
        addSerializableAttribute (new AttributeBool(&m_Passive,"Passive"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_NetworkType,"NetworkType"));
    }

    void  OSPFInterfaceGlobalMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFInterfaceGlobalMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFInterfaceGlobalMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFInterfaceGlobalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  OSPFInterfaceGlobalMessage::setintfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  OSPFInterfaceGlobalMessage::getintfName() const
    {
        return (m_intfName);
    }
	
    void  OSPFInterfaceGlobalMessage::setintfType(const UI32 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI32  OSPFInterfaceGlobalMessage::getintfType() const
    {
        return (m_intfType);
    }
	
    void  OSPFInterfaceGlobalMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFInterfaceGlobalMessage::getAreaId() const
    {
        return (m_areaId);
    }

    void  OSPFInterfaceGlobalMessage::setHelloInterval(const UI32 &HelloInterval)
    {
        m_HelloInterval  =  HelloInterval;
    }

    UI32  OSPFInterfaceGlobalMessage::getHelloInterval() const
    {
        return (m_HelloInterval);
    }

    void  OSPFInterfaceGlobalMessage::setRouterDeadInterval(const UI32 &RouterDeadInterval)
    {
        m_RouterDeadInterval  =  RouterDeadInterval;
    }

    UI32  OSPFInterfaceGlobalMessage::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval);
    }

    void  OSPFInterfaceGlobalMessage::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFInterfaceGlobalMessage::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFInterfaceGlobalMessage::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFInterfaceGlobalMessage::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFInterfaceGlobalMessage::setCost(const UI32 &Cost)
    {
        m_Cost  =  Cost;
    }

    UI32  OSPFInterfaceGlobalMessage::getCost() const
    {
        return (m_Cost);
    }

    void  OSPFInterfaceGlobalMessage::setPriority(const UI32 &Priority)
    {
        m_Priority  =  Priority;
    }

    UI32  OSPFInterfaceGlobalMessage::getPriority() const
    {
        return (m_Priority);
    }

    void  OSPFInterfaceGlobalMessage::setAuthType(const UI32 &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    UI32  OSPFInterfaceGlobalMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFInterfaceGlobalMessage::setEncryptionType(const OSPFEncrypType &EncryptionType)
    {
        m_EncryptionType  =  EncryptionType;
    }

    OSPFEncrypType  OSPFInterfaceGlobalMessage::getEncryptionType() const
    {
        return (m_EncryptionType);
    }

    void  OSPFInterfaceGlobalMessage::setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType)
    {
        m_MD5EncryptionType  =  MD5EncryptionType;
    }

    OSPFEncrypType  OSPFInterfaceGlobalMessage::getMD5EncryptionType() const
    {
        return (m_MD5EncryptionType);
    }

    void  OSPFInterfaceGlobalMessage::setAuthKey(const string &AuthKey)
    {
        m_AuthKey  =  AuthKey;
    }

    string  OSPFInterfaceGlobalMessage::getAuthKey() const
    {
        return (m_AuthKey);
    }

    void  OSPFInterfaceGlobalMessage::setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime)
    {
        m_AuthChangeWaitTime  =  AuthChangeWaitTime;
    }

    UI32  OSPFInterfaceGlobalMessage::getAuthChangeWaitTime() const
    {
        return (m_AuthChangeWaitTime);
    }

    void  OSPFInterfaceGlobalMessage::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime)
    {
        m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
    }

    UI32  OSPFInterfaceGlobalMessage::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime);
    }

    void  OSPFInterfaceGlobalMessage::setMD5KeyId(const UI32 &MD5KeyId)
    {
        m_MD5KeyId  =  MD5KeyId;
    }

    UI32  OSPFInterfaceGlobalMessage::getMD5KeyId() const
    {
        return (m_MD5KeyId);
    }

    void  OSPFInterfaceGlobalMessage::setMD5Key(const string &MD5Key)
    {
        m_MD5Key  =  MD5Key;
    }

    string  OSPFInterfaceGlobalMessage::getMD5Key() const
    {
        return (m_MD5Key);
    }

    void  OSPFInterfaceGlobalMessage::setFilterAll(const bool &FilterAll)
    {
        m_FilterAll  =  FilterAll;
    }

    bool  OSPFInterfaceGlobalMessage::getFilterAll() const
    {
        return (m_FilterAll);
    }

    void  OSPFInterfaceGlobalMessage::setFilterOption(const UI32 &FilterOption)
    {
        m_FilterOption  =  FilterOption;
    }

    UI32  OSPFInterfaceGlobalMessage::getFilterOption() const
    {
        return (m_FilterOption);
    }

    void  OSPFInterfaceGlobalMessage::setmtuIgnore(const bool &mtuIgnore)
    {
        m_mtuIgnore  =  mtuIgnore;
    }

    bool  OSPFInterfaceGlobalMessage::getmtuIgnore() const
    {
        return (m_mtuIgnore);
    }

    void  OSPFInterfaceGlobalMessage::setPassive(const bool &Passive)
    {
        m_Passive  =  Passive;
    }

    bool  OSPFInterfaceGlobalMessage::getPassive() const
    {
        return (m_Passive);
    }

    void  OSPFInterfaceGlobalMessage::setNetworkType(const OSPFIntfNetworkType &NetworkType)
    {
        m_NetworkType  =  NetworkType;
    }

    OSPFIntfNetworkType  OSPFInterfaceGlobalMessage::getNetworkType() const
    {
        return (m_NetworkType);
    }

}
