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

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Local/OSPF/OSPFInterfaceLocalManagedObject.h"
#include "OSPF/Local/OSPFAreaLocalManagedObject.h"
#include "OSPF/Local/OSPFTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/OSPF/OSPFInterfaceConfigMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "vcs.h"


namespace DcmNs
{

    OSPFInterfaceLocalManagedObject::OSPFInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (OSPFInterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_Type = 0;
		m_HelloInterval = OSPF_DEFAULT_HELLO_INTERVAL;
		m_RouterDeadInterval = OSPF_DEFAULT_ROUTER_DEAD_INTERVAL;
		m_RetransmitInterval = OSPF_DEFAULT_RETRANSMIT_INTERVAL;
		m_TransDelay = OSPF_DEFAULT_TRANSMIT_DELAY;
		m_Cost = OSPF_DEFAULT_COST;
		m_Priority = OSPF_DEFAULT_PRIORITY;
		m_AuthType = 0;
		m_EncryptionType = OSPF_NO_ENCR;
		m_AuthKey = string("");
		m_AuthChangeWaitTime = OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME;
		m_MD5KeyActWaitTime = OSPF_DEFAULT_MD5_NEW_KEY_ACTIVATION_WAIT_TIME;
		m_MD5KeyId = 1;
		m_MD5NoEncryptKeyId = 0;
		m_MD5EncryptionType = OSPF_NO_ENCR;
		m_MD5Key = string("");
		m_MD5NoEncryptKey = string("");

		// for ip ospf database-filter *
		m_FilterAll = false;
		m_FilterAllExt = OSPF_INTF_ALLOW_INVALID;
		m_FilterAllSumExt = OSPF_INTF_ALLOW_INVALID;

		//m_out = 0;
		m_MtuIgnore = 0;
		m_NetworkType = OSPF_INTF_NETWORK_NONE;
		m_Active = false;
		m_Passive = false;
    }

    OSPFInterfaceLocalManagedObject::OSPFInterfaceLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const string &id,const UI32 &Type,const ObjectId &Area,const UI32 &HelloInterval,const UI32 &RouterDeadInterval,const UI32 &RetransmitInterval,const UI32 &TransDelay,const UI32 &Cost,const UI32 &Priority,const UI32 &AuthType,const OSPFEncrypType &EncryptionType,const string &AuthKey,const string &AuthNoEncryptKey, const UI32 &AuthChangeWaitTime,const UI32 &MD5KeyActWaitTime,const UI8 &MD5KeyId, const UI8 &MD5NoEncryptKeyId, const OSPFEncrypType &MD5EncryptionType,const string &MD5Key, const string &MD5NoEncryptKey, const bool &FilterAll, const OSPFIntfDatabaseFilterAllow &FilterAllExt,  const OSPFIntfDatabaseFilterAllow &FilterAllSumExt, const bool &MtuIgnore,const OSPFIntfNetworkType &NetworkType,const bool &Active,const bool &Passive)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (OSPFInterfaceLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_id    (id),
        m_Type    (Type),
        m_Area    (Area),
        m_HelloInterval    (HelloInterval),
        m_RouterDeadInterval    (RouterDeadInterval),
        m_RetransmitInterval    (RetransmitInterval),
        m_TransDelay    (TransDelay),
        m_Cost    (Cost),
        m_Priority    (Priority),
        m_AuthType    (AuthType),
        m_EncryptionType    (EncryptionType),
        m_AuthKey    (AuthKey),
        m_AuthNoEncryptKey    (AuthNoEncryptKey),
        m_AuthChangeWaitTime    (AuthChangeWaitTime),
        m_MD5KeyActWaitTime    (MD5KeyActWaitTime),
        m_MD5KeyId    (MD5KeyId),
        m_MD5NoEncryptKeyId (MD5NoEncryptKeyId),
        m_MD5EncryptionType    (MD5EncryptionType),
        m_MD5Key    (MD5Key),
        m_MD5NoEncryptKey (MD5NoEncryptKey),

		// for ip ospf database-filter *
		m_FilterAll       (FilterAll),
	    m_FilterAllExt    (FilterAllExt),
	    m_FilterAllSumExt (FilterAllSumExt),

        m_MtuIgnore    (MtuIgnore),
        m_NetworkType    (NetworkType),
        m_Active    (Active),
        m_Passive    (Passive)
    {
    }

    OSPFInterfaceLocalManagedObject::~OSPFInterfaceLocalManagedObject ()
    {
    }

    string  OSPFInterfaceLocalManagedObject::getClassName()
    {
        return ("OSPFInterfaceLocalManagedObject");
    }

    void  OSPFInterfaceLocalManagedObject::setupAttributesForPersistence()
    {
		UI32 defaultRetransmitInterval = OSPF_DEFAULT_RETRANSMIT_INTERVAL;
		UI32 defaultTransDelay = OSPF_DEFAULT_TRANSMIT_DELAY;
		//UI32 defaultCost = OSPF_DEFAULT_COST;
		UI32 defaultPriority = OSPF_DEFAULT_PRIORITY;
		//UI32 defaultEncr = OSPF_NO_ENCR;
		UI32 defaultAuthChangeWaitTime = OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME;
		//UI32 defaultMD5KeyActWaitTime = 0;
		//UI8 defaultMD5KeyId = 0;
		//UI32 defaultFilterType = OSPF_INTF_FILTER_NONE;
		//UI32 defautAllowOption = OSPF_INTF_ALLOW_INVALID;
		UI32 defaultNetworkType = OSPF_INTF_NETWORK_NONE;
		UI32 encrTypeDefault = OSPF_INVALID_ENCR;

        DcmLocalManagedObject::setupAttributesForPersistence ();
        // addPersistableAttribute (new AttributeString(&m_id,"id",logical_switch_interface));
        addPersistableAttribute (new AttributeString(&m_id,"id"));

        addPersistableAttribute (new AttributeUI32(&m_Type,0,"Type"));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_Area,"Area", OSPFAreaLocalManagedObject::getClassName (),true));
        addPersistableAttribute (new AttributeUI32UC(&m_HelloInterval,true,"HelloInterval"));
        addPersistableAttribute (new AttributeUI32UC(&m_RouterDeadInterval,true,"RouterDeadInterval"));
        addPersistableAttribute (new AttributeUI32(&m_RetransmitInterval,defaultRetransmitInterval,"RetransmitInterval",ospf_retransmit_interval));
        addPersistableAttribute (new AttributeUI32(&m_TransDelay,defaultTransDelay,"TransDelay",ospf_transmit_delay));
        addPersistableAttribute (new AttributeUI32UC(&m_Cost,true,"Cost"));
        addPersistableAttribute (new AttributeUI32(&m_Priority,defaultPriority,"Priority",ospf_priority));
        addPersistableAttribute (new AttributeUI32(&m_AuthType,"AuthType"));
        addPersistableAttribute (new AttributeUI32UC(&m_EncryptionType,true,true,encrTypeDefault,"EncryptionType"));
        addPersistableAttribute (new AttributeStringUC(&m_AuthKey,true,"AuthKey"));
        addPersistableAttribute (new AttributeStringUC(&m_AuthNoEncryptKey,true,"AuthNoEncryptKey"));
        addPersistableAttribute (new AttributeUI32(&m_AuthChangeWaitTime,defaultAuthChangeWaitTime,"AuthChangeWaitTime",ospf_auth_change_wait_time));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5KeyActWaitTime,true,true,defaultAuthChangeWaitTime,"MD5KeyActWaitTime"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5KeyId,true,"MD5KeyId"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5NoEncryptKeyId,true,"MD5NoEncryptKeyId"));
        addPersistableAttribute (new AttributeUI32UC(&m_MD5EncryptionType,true,"MD5EncryptionType"));
        addPersistableAttribute (new AttributeStringUC(&m_MD5Key,true,"MD5Key"));
        addPersistableAttribute (new AttributeStringUC(&m_MD5NoEncryptKey,true,"MD5NoEncryptKey"));

		// for ip ospf database-filter
        addPersistableAttribute (new AttributeBool(&m_FilterAll,"FilterAll",ospf_all_out));
        addPersistableAttribute (new AttributeEnum(&m_FilterAllExt,0,"FilterAllExt", ospf_all_external));
        addPersistableAttribute (new AttributeEnum(&m_FilterAllSumExt,0,"FilterAllSumExt", ospf_all_summary_external));

        addPersistableAttribute (new AttributeBool(&m_MtuIgnore,"MtuIgnore",ospf_mtu_ignore));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_NetworkType,defaultNetworkType,"NetworkType",ospf_network));
        addPersistableAttribute (new AttributeBool(&m_Active,"Active",ospf_active));
        addPersistableAttribute (new AttributeBool(&m_Passive,"Passive",ospf_passive));
		setUserTagForAttribute("Area", ospf_area);

		setUserTagForAttribute("Cost",ospf_cost);
        setUserTagForAttribute("HelloInterval",ospf_hello_interval);
        setUserTagForAttribute("RouterDeadInterval",ospf_dead_interval);
        setUserTagForAttribute("EncryptionType",ospf_encrypttype);
        setUserTagForAttribute("AuthKey",ospf_auth_key);
        setUserTagForAttribute("AuthNoEncryptKey",ospf_no_encrypt_auth_key);
        setUserTagForAttribute("MD5KeyActWaitTime",ospf_key_activation_wait_time);
        setUserTagForAttribute("MD5KeyId",ospf_key_id);
        setUserTagForAttribute("MD5NoEncryptKeyId",ospf_no_encrypt_key_id);
        setUserTagForAttribute("MD5EncryptionType",ospf_key);
        setUserTagForAttribute("MD5Key",ospf_md5_authentication_key);
        setUserTagForAttribute("MD5NoEncryptKey",ospf_no_encrypt_key);
    }

    void  OSPFInterfaceLocalManagedObject::setupAttributesForCreate()
    {
		UI32 defaultRetransmitInterval = 0;
		UI32 defaultTransDelay = 0;
		//UI32 defaultCost = 0;
		UI32 defaultPriority = 0;
		//UI32 defaultEncr = OSPF_NO_ENCR;
		UI32 defaultAuthChangeWaitTime = 0;
		//UI32 defaultMD5KeyActWaitTime = 0;
		//UI8 defaultMD5KeyId = 0;
		//UI32 defaultFilterType = OSPF_INTF_FILTER_NONE;
		//UI32 defautAllowOption = OSPF_INTF_ALLOW_INVALID;
		UI32 defaultNetworkType = OSPF_INTF_NETWORK_NONE;
		UI32 encrTypeDefault = OSPF_INVALID_ENCR;
        DcmLocalManagedObject::setupAttributesForCreate ();
        // addPersistableAttributeForCreate  (new AttributeString(&m_id,"id",logical_switch_interface));
        addPersistableAttributeForCreate  (new AttributeString(&m_id,"id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Type,0,"Type"));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_Area,"Area", OSPFAreaLocalManagedObject::getClassName (),true));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_HelloInterval,true,"HelloInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_RouterDeadInterval,true,"RouterDeadInterval"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_RetransmitInterval,defaultRetransmitInterval,"RetransmitInterval",ospf_retransmit_interval));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_TransDelay,defaultTransDelay,"TransDelay",ospf_transmit_delay));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_Cost,true,"Cost"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_Priority,defaultPriority,"Priority",ospf_priority));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_AuthType,"AuthType"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_EncryptionType,true,true,encrTypeDefault,"EncryptionType"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_AuthKey,true,"AuthKey"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_AuthNoEncryptKey,true,"AuthNoEncryptKey"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_AuthChangeWaitTime,defaultAuthChangeWaitTime,"AuthChangeWaitTime",ospf_auth_change_wait_time));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5KeyActWaitTime,true,"MD5KeyActWaitTime"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5KeyId,true,"MD5KeyId"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5NoEncryptKeyId,true,"MD5NoEncryptKeyId"));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_MD5EncryptionType,true,"MD5EncryptionType"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_MD5Key,true,"MD5Key"));
        addPersistableAttributeForCreate  (new AttributeStringUC(&m_MD5NoEncryptKey,true,"MD5NoEncryptKey"));

		// for ip ospf database-filter
        addPersistableAttributeForCreate  (new AttributeBool(&m_FilterAll,"FilterAll",ospf_all_out));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_FilterAllExt,"FilterAllExt",ospf_all_external));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_FilterAllSumExt,"FilterAllSumExt",ospf_all_summary_external));

        addPersistableAttributeForCreate  (new AttributeBool(&m_MtuIgnore,"MtuIgnore",ospf_mtu_ignore));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_NetworkType,defaultNetworkType,"NetworkType",ospf_network));
        addPersistableAttributeForCreate  (new AttributeBool(&m_Active,"Active",ospf_active));
        addPersistableAttributeForCreate  (new AttributeBool(&m_Passive,"Passive",ospf_passive));
    }

    /*void  OSPFInterfaceLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("id");
        setUserDefinedKeyCombination (keyName);
    }*/

    void  OSPFInterfaceLocalManagedObject::setId(const string &id)
    {
        m_id  =  id;
    }

    string  OSPFInterfaceLocalManagedObject::getId() const
    {
        return (m_id);
    }

    void  OSPFInterfaceLocalManagedObject::setType(const UI32 &Type)
    {
        m_Type  =  Type;
    }

    UI32  OSPFInterfaceLocalManagedObject::getType() const
    {
        return (m_Type);
    }

    void  OSPFInterfaceLocalManagedObject::setArea(const ObjectId &Area)
    {
        m_Area  =  Area;
    }

    ObjectId  OSPFInterfaceLocalManagedObject::getArea() const
    {
        return (m_Area);
    }

    void  OSPFInterfaceLocalManagedObject::setHelloInterval(const UI32 &HelloInterval, const bool &set)
    {
        m_HelloInterval.setUI32Value (HelloInterval);
        m_HelloInterval.setIsUserConfigured(set);
    }

    UI32  OSPFInterfaceLocalManagedObject::getHelloInterval() const
    {
        return (m_HelloInterval.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setRouterDeadInterval(const UI32 &RouterDeadInterval, const bool &set)
    {
        m_RouterDeadInterval.setUI32Value (RouterDeadInterval);
        m_RouterDeadInterval.setIsUserConfigured(set);
    }

    UI32  OSPFInterfaceLocalManagedObject::getRouterDeadInterval() const
    {
        return (m_RouterDeadInterval.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setRetransmitInterval(const UI32 &RetransmitInterval)
    {
        m_RetransmitInterval  =  RetransmitInterval;
    }

    UI32  OSPFInterfaceLocalManagedObject::getRetransmitInterval() const
    {
        return (m_RetransmitInterval);
    }

    void  OSPFInterfaceLocalManagedObject::setTransDelay(const UI32 &TransDelay)
    {
        m_TransDelay  =  TransDelay;
    }

    UI32  OSPFInterfaceLocalManagedObject::getTransDelay() const
    {
        return (m_TransDelay);
    }

    void  OSPFInterfaceLocalManagedObject::setCost(const UI32 &Cost, const bool &set)
    {
        m_Cost.setUI32Value (Cost);
        m_Cost.setIsUserConfigured(set);
    }

    UI32  OSPFInterfaceLocalManagedObject::getCost() const
    {
        return (m_Cost.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setPriority(const UI32 &Priority)
    {
        m_Priority  =  Priority;
    }

    UI32  OSPFInterfaceLocalManagedObject::getPriority() const
    {
        return (m_Priority);
    }

    void  OSPFInterfaceLocalManagedObject::setAuthType(const UI32 &AuthType)
    {
        m_AuthType  =  AuthType;
    }

    UI32  OSPFInterfaceLocalManagedObject::getAuthType() const
    {
        return (m_AuthType);
    }

    void  OSPFInterfaceLocalManagedObject::setEncryptionType(const OSPFEncrypType &EncryptionType, const bool &set)
    {
        //m_EncryptionType  =  EncryptionType;
        m_EncryptionType.setUI32Value((UI32)EncryptionType);
        m_EncryptionType.setIsUserConfigured(set);        
    }

    OSPFEncrypType  OSPFInterfaceLocalManagedObject::getEncryptionType() const
    {
        return (OSPFEncrypType)(m_EncryptionType.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setAuthKey(const string &AuthKey, const bool &set)
    {
        //m_AuthKey  =  AuthKey;
        m_AuthKey.setStringValue(AuthKey);
        m_AuthKey.setIsUserConfigured(set);
    }

    string  OSPFInterfaceLocalManagedObject::getAuthKey() const
    {
        return (m_AuthKey.getStringValue());
    }

    void  OSPFInterfaceLocalManagedObject::setAuthChangeWaitTime(const UI32 &AuthChangeWaitTime)
    {
        m_AuthChangeWaitTime  =  AuthChangeWaitTime;

        if(OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME != AuthChangeWaitTime)
            if( (m_MD5KeyActWaitTime.getIsUserConfigured()) )
                setMD5KeyActWaitTime(OSPF_DEFAULT_MD5_NEW_KEY_ACTIVATION_WAIT_TIME, false);
    }

    UI32  OSPFInterfaceLocalManagedObject::getAuthChangeWaitTime() const
    {
        return (m_AuthChangeWaitTime);
    }

    void  OSPFInterfaceLocalManagedObject::setMD5KeyActWaitTime(const UI32 &MD5KeyActWaitTime, const bool &set)
    {
        //m_MD5KeyActWaitTime  =  MD5KeyActWaitTime;
        m_MD5KeyActWaitTime.setUI32Value(MD5KeyActWaitTime);
        m_MD5KeyActWaitTime.setIsUserConfigured(set);

        if(set)
            if(m_AuthChangeWaitTime != OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME)
                m_AuthChangeWaitTime = OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME;
    }

    UI32  OSPFInterfaceLocalManagedObject::getMD5KeyActWaitTime() const
    {
        return (m_MD5KeyActWaitTime.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setMD5KeyId(const UI8 &MD5KeyId, const bool &set)
    {
        m_MD5KeyId.setUI32Value((UI32)MD5KeyId);
        m_MD5KeyId.setIsUserConfigured(set);
    }

    UI8  OSPFInterfaceLocalManagedObject::getMD5KeyId() const
    {
        return (UI8)(m_MD5KeyId.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setMD5NoEncryptKeyId(const UI8 &MD5NoEncryptKeyId, const bool &set)
    {
        m_MD5NoEncryptKeyId.setUI32Value((UI32)MD5NoEncryptKeyId);
        m_MD5NoEncryptKeyId.setIsUserConfigured(set);
    }

    UI8  OSPFInterfaceLocalManagedObject::getMD5NoEncryptKeyId() const
    {
        return (UI8)(m_MD5NoEncryptKeyId.getUI32Value());
    }
    void  OSPFInterfaceLocalManagedObject::setMD5EncryptionType(const OSPFEncrypType &MD5EncryptionType, const bool &set)
    {
        //m_MD5EncryptionType  =  MD5EncryptionType;
        m_MD5EncryptionType.setUI32Value((UI32)MD5EncryptionType);
        m_MD5EncryptionType.setIsUserConfigured(set);
    }

    OSPFEncrypType  OSPFInterfaceLocalManagedObject::getMD5EncryptionType() const
    {
        return (OSPFEncrypType)(m_MD5EncryptionType.getUI32Value());
    }

    void  OSPFInterfaceLocalManagedObject::setMD5Key(const string &MD5Key, const bool &set)
    {
        //m_MD5Key  =  MD5Key;
        m_MD5Key.setStringValue(MD5Key);
        m_MD5Key.setIsUserConfigured(set);
    }

    string  OSPFInterfaceLocalManagedObject::getMD5Key() const
    {
        return (m_MD5Key.getStringValue());
    }

    void  OSPFInterfaceLocalManagedObject::setMD5NoEncryptKey(const string &MD5NoEncryptKey, const bool &set)
    {
        m_MD5NoEncryptKey.setStringValue(MD5NoEncryptKey);
        m_MD5NoEncryptKey.setIsUserConfigured(set);
    }

    string  OSPFInterfaceLocalManagedObject::getMD5NoEncryptKey() const
    {
        return (m_MD5NoEncryptKey.getStringValue());
    }

	void  OSPFInterfaceLocalManagedObject::setFilterAll(const bool &FilterAll)
    {
        m_FilterAll = FilterAll;
    }

    bool  OSPFInterfaceLocalManagedObject::getFilterAll() const
    {
        return (m_FilterAll);
    }
	
	void  OSPFInterfaceLocalManagedObject::setFilterAllExt(const UI32 &FilterAllExt)
    {
        m_FilterAllExt = FilterAllExt;
    }

    UI32  OSPFInterfaceLocalManagedObject::getFilterAllExt() const
    {
        return (m_FilterAllExt);
    }
	
	void  OSPFInterfaceLocalManagedObject::setFilterAllSumExt(const UI32 &FilterAllSumExt)
    {
        m_FilterAllSumExt = FilterAllSumExt;
    }

    UI32  OSPFInterfaceLocalManagedObject::getFilterAllSumExt() const
    {
        return (m_FilterAllSumExt);
    }

	void OSPFInterfaceLocalManagedObject::clearDatabaseFilterOptions(void)
    {
		m_FilterAll = false;
		m_FilterAllExt = OSPF_INTF_ALLOW_INVALID;
		m_FilterAllSumExt = OSPF_INTF_ALLOW_INVALID;
    }
    void  OSPFInterfaceLocalManagedObject::setMtuIgnore(const bool &MtuIgnore)
    {
        m_MtuIgnore  =  MtuIgnore;
    }

    bool  OSPFInterfaceLocalManagedObject::getMtuIgnore() const
    {
        return (m_MtuIgnore);
    }

    void  OSPFInterfaceLocalManagedObject::setNetworkType(const OSPFIntfNetworkType &NetworkType)
    {
        m_NetworkType  =  NetworkType;
    }

    OSPFIntfNetworkType  OSPFInterfaceLocalManagedObject::getNetworkType() const
    {
        return (m_NetworkType);
    }
	
    void  OSPFInterfaceLocalManagedObject::setActive(const bool &Active)
    {
        m_Active  =  Active;
    }

    bool  OSPFInterfaceLocalManagedObject::getActive() const
    {
        return (m_Active);
    }

    void  OSPFInterfaceLocalManagedObject::setPassive(const bool &Passive)
    {
        m_Passive  =  Passive;
    }

    bool  OSPFInterfaceLocalManagedObject::getPassive() const
    {
        return (m_Passive);
    }

	void OSPFInterfaceLocalManagedObject::updateClientMessageForReplay(OSPFInterfaceConfigMessage *pMsg)
	{
		trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalManagedObject::updateClientMessageForReplay ..");
		LocationId locId = FrameworkToolKit::getThisLocationId();
        pMsg->setOpCode(OSPF_CREATE);
        pMsg->setCmdCode(OSPF_INTF_CONFIG_REPLAY);
		pMsg->setintfName(getId());
		pMsg->setintfType(getType());
        for (UI32 idx = 0; idx < OSPF_INTF_DEL; idx ++) {
			switch (idx) {
				case OSPF_INTF_AREA:
					{
					if(getArea() == ObjectId::NullObjectId)
						break;
					OSPFAreaLocalManagedObject *pOSPFAreaLocalManagedObject = NULL;
					WaveManagedObjectSynchronousQueryContext syncQueryCtxt(OSPFAreaLocalManagedObject::getClassName());
					syncQueryCtxt.addAndAttribute(new AttributeUI32(getArea().getClassId(), "objectidclassid") );
					syncQueryCtxt.addAndAttribute(new AttributeUI64(getArea().getInstanceId(), "objectidinstanceid") );
					vector<WaveManagedObject *> *pResults =querySynchronouslyLocalManagedObjectsForLocationId (locId, &syncQueryCtxt);
					if (pResults && pResults->size()  == 1) {
						pOSPFAreaLocalManagedObject =  dynamic_cast<OSPFAreaLocalManagedObject*>((*pResults)[0]);
					}
					/*
					if (pResults) {
						pResults->clear();
						delete pResults;
					}
					*/

					pMsg->setAreaId(pOSPFAreaLocalManagedObject->getAreaId());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_AREA);

					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

					break;
					}
				case OSPF_INTF_HELLO:
					if(!m_HelloInterval.getIsUserConfigured())
						break;
					pMsg->setHelloInterval(getHelloInterval());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_HELLO);
					break;
				case OSPF_INTF_DEAD:
					if(!m_RouterDeadInterval.getIsUserConfigured())
						break;
					pMsg->setRouterDeadInterval(getRouterDeadInterval());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_DEAD);
					break;
				case OSPF_INTF_RETRANSMIT:
					if(getRetransmitInterval() == OSPF_DEFAULT_RETRANSMIT_INTERVAL)
						break;
					pMsg->setRetransmitInterval(getRetransmitInterval());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_RETRANSMIT);
					break;
				case OSPF_INTF_TRANSDELAY:
					if(getTransDelay() == OSPF_DEFAULT_TRANSMIT_DELAY)
						break;
					pMsg->setTransDelay(getTransDelay());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_TRANSDELAY);
					break;
				case OSPF_INTF_COST:
					if(false == m_Cost.getIsUserConfigured()) {
						trace (TRACE_LEVEL_INFO, "OSPFInterfaceLocalManagedObject::updateClientMessageForReplay: cost is not user configured");
						break;
					}
					pMsg->setCost(getCost());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_COST);
					break;
				case OSPF_INTF_PRIORITY:
					if(getPriority() == OSPF_DEFAULT_PRIORITY)
						break;
					pMsg->setPriority(getPriority());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_PRIORITY);
					break;
				case OSPF_INTF_AUTH_CHANGE_WAIT_TIME:
					if(getAuthChangeWaitTime() == OSPF_DEFAULT_AUTH_CHANGE_WAIT_TIME)
						break;
					pMsg->setAuthChangeWaitTime(getAuthChangeWaitTime());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_AUTH_CHANGE_WAIT_TIME);
					break;
				case OSPF_INTF_AUTH_KEY:
					if(getAuthKey() == string(""))
						break;
					pMsg->setEncryptionType(getEncryptionType());
					pMsg->setAuthKey(getAuthKey());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_AUTH_KEY);
					break;
				case OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME:
					if(getMD5KeyActWaitTime() == OSPF_DEFAULT_MD5_NEW_KEY_ACTIVATION_WAIT_TIME)
						break;
					pMsg->setMD5KeyActWaitTime(getMD5KeyActWaitTime());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_MD5_KEY_ACTV_WAIT_TIME);
					break;
				case OSPF_INTF_MD5_AUTH_KEY:
					if(getMD5Key() == string(""))
						break;
					pMsg->setMD5EncryptionType(getMD5EncryptionType());
					pMsg->setMD5KeyId(getMD5KeyId());
					pMsg->setMD5Key(getMD5Key());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_MD5_AUTH_KEY);
					break;

				// For ip ospf database-filter *
				// Based on MO fields, specify CLI command in OSPFInterfaceConfigMessage *pMsg
				case OSPF_INTF_FILTER_ALL:
					if (!getFilterAll())
						break;
					pMsg->setFilterAll(getFilterAll());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_FILTER_ALL);
					break;
				case OSPF_INTF_FILTER_EXT:
					if (getFilterAllExt() == OSPF_INTF_ALLOW_INVALID)
						break;
					pMsg->setFilterOption(getFilterAllExt());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_FILTER_EXT);
					break;
					
				case OSPF_INTF_FILTER_SUMM_EXT:
					if (getFilterAllSumExt() == OSPF_INTF_ALLOW_INVALID)
						break;
					pMsg->setFilterOption(getFilterAllSumExt());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_FILTER_SUMM_EXT);
					break;

				case OSPF_INTF_MTU_IGNORE:
					if(!getMtuIgnore())
						break;
					pMsg->setmtuIgnore(getMtuIgnore());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_MTU_IGNORE);
					break;
				case OSPF_INTF_NETWORK:
					if(getNetworkType() == OSPF_INTF_NETWORK_NONE)
						break;
					pMsg->setNetworkType(getNetworkType());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_NETWORK);
					break;
				case OSPF_INTF_ACTIVE:
					if(!getActive())
						break;
					pMsg->setActive(getActive());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_ACTIVE);
					break;
				case OSPF_INTF_PASSIVE:
					if(!getPassive())
						break;
					pMsg->setPassive(getPassive());
					pMsg->setReplayCmdCodeBitmap(OSPF_INTF_PASSIVE);
					break;
			}
			if(getArea() == ObjectId::NullObjectId)
				break;	
		}
	}
}
