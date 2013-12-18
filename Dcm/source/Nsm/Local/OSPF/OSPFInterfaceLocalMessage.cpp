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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    OSPFInterfaceLocalMessage::OSPFInterfaceLocalMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALOSPFINTERFACECONFIG)
    {
        m_FilterAll = false;
        m_FilterOption = OSPF_INTF_ALLOW_INVALID;
        m_MD5KeyId = 256;
        m_MD5Key.clear();
        m_MD5EncryptionType = OSPF_INVALID_ENCR;
        m_EncryptionType = OSPF_INVALID_ENCR;
        m_AuthKey.clear(); 
    }

    OSPFInterfaceLocalMessage::OSPFInterfaceLocalMessage (const UI8 &opCode,const UI8 &cmdCode,const string &intfName,const UI32 &intfType,const string &areaId,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const OSPFEncrypType &MD5EncryptionType,const string &AuthKey,const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI32 &MD5KeyId,const string &MD5Key,const bool &FilterAll,const UI32 &FilterOption,const bool &mtuIgnore,const bool &Active,const bool &Passive,const OSPFIntfNetworkType &NetworkType)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALOSPFINTERFACECONFIG),
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
        m_FilterOption    (FilterOption),
        m_mtuIgnore    (mtuIgnore),
        m_Active    (Active),
        m_Passive    (Passive),
        m_NetworkType    (NetworkType)
    {
    }

    OSPFInterfaceLocalMessage::~OSPFInterfaceLocalMessage ()
    {
    }

    void  OSPFInterfaceLocalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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
        addSerializableAttribute (new AttributeBool(&m_Active,"Active"));
        addSerializableAttribute (new AttributeBool(&m_Passive,"Passive"));
        addSerializableAttribute (new AttributeEnum((UI32 *)&m_NetworkType,"NetworkType"));
    }

    void  OSPFInterfaceLocalMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  OSPFInterfaceLocalMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  OSPFInterfaceLocalMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  OSPFInterfaceLocalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  OSPFInterfaceLocalMessage::setintfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  OSPFInterfaceLocalMessage::getintfName() const
    {
        return (m_intfName);
    }
	
    void  OSPFInterfaceLocalMessage::setintfType(const UI32 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI32  OSPFInterfaceLocalMessage::getintfType() const
    {
        return (m_intfType);
    }
	
    void  OSPFInterfaceLocalMessage::setAreaId(const string &areaId)
    {
        m_areaId  =  areaId;
    }

    string  OSPFInterfaceLocalMessage::getAreaId() const
    {
        return (m_areaId);
    }

    void  OSPFInterfaceLocalMessage::setHelloInterval(const UI32 &HelloInterval)
    {
        m_HelloInterval  =  HelloInterval;
    }

    UI32  OSPFInterfaceLocalMessage::getHelloInterval() const
    {
        return (m_HelloInterval);
    }

    void  OSPFInterfaceLocalMessage::setRouterDeadInterval(const UI32 &RouterDeadInterval)
    {
        m_RouterDeadInterval  =  RouterDeadInterval;
    }

    UI32  OSPFInterfaceLocalMessage::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval);
    }

    void  OSPFInterfaceLocalMessage::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFInterfaceLocalMessage::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFInterfaceLocalMessage::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFInterfaceLocalMessage::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFInterfaceLocalMessage::setCost(const UI32 &Cost)
    {
        m_Cost  =  Cost;
    }

    UI32  OSPFInterfaceLocalMessage::getCost() const
    {
        return (m_Cost);
    }

    void  OSPFInterfaceLocalMessage::setPriority(const UI32 &Priority)
    {
        m_Priority  =  Priority;
    }

    UI32  OSPFInterfaceLocalMessage::getPriority() const
    {
        return (m_Priority);
    }

    void  OSPFInterfaceLocalMessage::setAuthType(const UI32 &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    UI32  OSPFInterfaceLocalMessage::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFInterfaceLocalMessage::setEncryptionType(const OSPFEncrypType &EncryptionType)
    {
        m_EncryptionType  =  EncryptionType;
    }

    OSPFEncrypType  OSPFInterfaceLocalMessage::getEncryptionType() const
    {
        return (m_EncryptionType);
    }

    void  OSPFInterfaceLocalMessage::setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType)
    {
        m_MD5EncryptionType  =  MD5EncryptionType;
    }

    OSPFEncrypType  OSPFInterfaceLocalMessage::getMD5EncryptionType() const
    {
        return (m_MD5EncryptionType);
    }

    void  OSPFInterfaceLocalMessage::setAuthKey(const string &AuthKey)
    {
        m_AuthKey  =  AuthKey;
    }

    string  OSPFInterfaceLocalMessage::getAuthKey() const
    {
        return (m_AuthKey);
    }

    void  OSPFInterfaceLocalMessage::setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime)
    {
        m_AuthChangeWaitTime  =  AuthChangeWaitTime;
    }

    UI32  OSPFInterfaceLocalMessage::getAuthChangeWaitTime() const
    {
        return (m_AuthChangeWaitTime);
    }

    void  OSPFInterfaceLocalMessage::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime)
    {
        m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
    }

    UI32  OSPFInterfaceLocalMessage::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime);
    }

    void  OSPFInterfaceLocalMessage::setMD5KeyId(const UI32 &MD5KeyId)
    {
        m_MD5KeyId  =  MD5KeyId;
    }

    UI32  OSPFInterfaceLocalMessage::getMD5KeyId() const
    {
        return (m_MD5KeyId);
    }

    void  OSPFInterfaceLocalMessage::setMD5Key(const string &MD5Key)
    {
        m_MD5Key  =  MD5Key;
    }

    string  OSPFInterfaceLocalMessage::getMD5Key() const
    {
        return (m_MD5Key);
    }

    void  OSPFInterfaceLocalMessage::setFilterAll(const bool &FilterAll)
    {
        m_FilterAll  =  FilterAll;
    }

    bool  OSPFInterfaceLocalMessage::getFilterAll() const
    {
        return (m_FilterAll);
    }

    void  OSPFInterfaceLocalMessage::setFilterOption(const UI32 &FilterOption)
    {
        m_FilterOption  =  FilterOption;
    }

    UI32  OSPFInterfaceLocalMessage::getFilterOption() const
    {
        return (m_FilterOption);
    }

    void  OSPFInterfaceLocalMessage::setmtuIgnore(const bool &mtuIgnore)
    {
        m_mtuIgnore  =  mtuIgnore;
    }

    bool  OSPFInterfaceLocalMessage::getmtuIgnore() const
    {
        return (m_mtuIgnore);
    }

    void  OSPFInterfaceLocalMessage::setActive(const bool &Active)
    {
        m_Active  =  Active;
    }

    bool  OSPFInterfaceLocalMessage::getActive() const
    {
        return (m_Active);
    }

    void  OSPFInterfaceLocalMessage::setPassive(const bool &Passive)
    {
        m_Passive  =  Passive;
    }

    bool  OSPFInterfaceLocalMessage::getPassive() const
    {
        return (m_Passive);
    }

    void  OSPFInterfaceLocalMessage::setNetworkType(const OSPFIntfNetworkType &NetworkType)
    {
        m_NetworkType  =  NetworkType;
    }

    OSPFIntfNetworkType OSPFInterfaceLocalMessage::getNetworkType() const
    {
        return (m_NetworkType);
    }
}
