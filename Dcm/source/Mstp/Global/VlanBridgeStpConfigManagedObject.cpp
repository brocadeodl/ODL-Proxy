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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Mstp/Global/VlanBridgeStpConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpVlanBridgeConfigMessage.h"
#include "vcs.h"


namespace DcmNs
{

    VlanBridgeStpConfigManagedObject::VlanBridgeStpConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (VlanBridgeStpConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager)
    {
		m_helloTime = UI32UC(MSTP_DCM_DEFAULT_HELLO_TIME);
		m_maxAge = UI32UC(MSTP_DCM_DEFAULT_MAX_AGE);
		m_priority = UI32UC(MSTP_DCM_DEFAULT_PRIORITY);
		m_agingTime = UI32UC(MSTP_DCM_DEFAULT_AGING_TIME);
		m_fwdDelay = UI32UC(MSTP_DCM_DEFAULT_FWD_DELAY);
		m_id = 0;
    }

#if 0
    VlanBridgeStpConfigManagedObject::VlanBridgeStpConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const ObjectId &vlanObjectId,const UI32UC &priority,const UI32UC &fwdDelay,const UI32UC &maxAge, const UI32UC &agingTime, const UI32UC &helloTime)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (VlanBridgeStpConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        m_vlanObjectId    (vlanObjectId),
        m_priority    (priority),
        m_fwdDelay    (fwdDelay),
        m_maxAge    (maxAge),
        m_agingTime    (agingTime),
        m_helloTime    (helloTime)
    {
    }
#endif

    VlanBridgeStpConfigManagedObject::~VlanBridgeStpConfigManagedObject ()
    {
    }

    string  VlanBridgeStpConfigManagedObject::getClassName()
    {
        return ("VlanBridgeStpConfigManagedObject");
    }

    void  VlanBridgeStpConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_vlanObjectId,"id", VlanIntfManagedObject::getClassName (), true, xstp_id));
        addPersistableAttribute (new AttributeUI32UC(&m_priority, false, false, 0, "priority", xstp_priority));
        addPersistableAttribute (new AttributeUI32UC(&m_fwdDelay, false, false, 0, "fwdDelay", xstp_forward_delay));
        addPersistableAttribute (new AttributeUI32UC(&m_maxAge, false, false, 0, "maxAge", xstp_max_age));
        addPersistableAttribute (new AttributeUI32UC(&m_helloTime, false, false, 0, "helloTime", xstp_hello_time));
        addPersistableAttribute (new AttributeUI32UC(&m_agingTime, false, false, 0, "agingTime", hidden_cli_aging_time));
        addPersistableAttribute (new AttributeUI32(&m_id,"vlan"));
    }

    void  VlanBridgeStpConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_vlanObjectId,"id", VlanIntfManagedObject::getClassName(), true, xstp_id));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_priority, false, false, 0, "priority", xstp_priority));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_fwdDelay, false, false, 0, "fwdDelay", xstp_forward_delay));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_maxAge, false, false, 0, "maxAge", xstp_max_age));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_helloTime, false, false, 0, "helloTime", xstp_hello_time));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_agingTime, false, false, 0, "agingTime", hidden_cli_aging_time));
        addPersistableAttributeForCreate (new AttributeUI32(&m_id,"vlan"));

        vector<string> keyName;
        keyName.push_back("id");
        setUserDefinedKeyCombination (keyName);
    }

    void  VlanBridgeStpConfigManagedObject::setVlanObjectId(const ObjectId &vlanObjectId)
    {
        m_vlanObjectId  =  vlanObjectId;
    }

    ObjectId  VlanBridgeStpConfigManagedObject::getVlanObjectId() const
    {
        return (m_vlanObjectId);
    }

    void  VlanBridgeStpConfigManagedObject::setPriority(const UI32 &val, bool userConfig)
    {
		m_priority.setIsUserConfigured(userConfig);
		m_priority.setUI32Value(val);
    }

    UI32UC  VlanBridgeStpConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  VlanBridgeStpConfigManagedObject::setForwardDelay(const UI32 &val, bool userConfig)
    {
		m_fwdDelay.setIsUserConfigured(userConfig);
		m_fwdDelay.setUI32Value(val);
    }

    UI32UC  VlanBridgeStpConfigManagedObject::getForwardDelay() const
    {
        return (m_fwdDelay);
    }

    UI32 VlanBridgeStpConfigManagedObject::getId() const
    {
        return (m_id);
    }

    void  VlanBridgeStpConfigManagedObject::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    void  VlanBridgeStpConfigManagedObject::setMaxAge(const UI32 &val, bool userConfig)
    {
		m_maxAge.setIsUserConfigured(userConfig);
		m_maxAge.setUI32Value(val);
    }

    UI32UC  VlanBridgeStpConfigManagedObject::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  VlanBridgeStpConfigManagedObject::setAgingTime(const UI32 &val, bool userConfig)
    {
		m_agingTime.setIsUserConfigured(userConfig);
		m_agingTime.setUI32Value(val);
    }

    UI32UC  VlanBridgeStpConfigManagedObject::getAgingTime() const
    {
        return (m_agingTime);
    }

    void  VlanBridgeStpConfigManagedObject::setHelloTime(const UI32 &val, bool userConfig)
    {
		m_helloTime.setIsUserConfigured(userConfig);
		m_helloTime.setUI32Value(val);
    }

    UI32UC  VlanBridgeStpConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    bool  VlanBridgeStpConfigManagedObject::isDefault()
    {
		return (!m_helloTime.getIsUserConfigured() &&
				!m_maxAge.getIsUserConfigured()	&&
				!m_priority.getIsUserConfigured() &&
				!m_agingTime.getIsUserConfigured() &&
				!m_fwdDelay.getIsUserConfigured());
    }

    void  VlanBridgeStpConfigManagedObject::setDefaultAttribute(int id, ObjectId vlanObjectId)
    {
		m_helloTime = UI32UC(MSTP_DCM_DEFAULT_HELLO_TIME);
		m_maxAge = UI32UC(MSTP_DCM_DEFAULT_MAX_AGE);
		m_priority = UI32UC(MSTP_DCM_DEFAULT_PRIORITY);
		m_agingTime = UI32UC(MSTP_DCM_DEFAULT_AGING_TIME);
		m_fwdDelay = UI32UC(MSTP_DCM_DEFAULT_FWD_DELAY);
		m_vlanObjectId = vlanObjectId;
		m_id = id;
    }

	ResourceId VlanBridgeStpConfigManagedObject::getAttributeToMessage(MstpVlanBridgeConfigMessage *&pMsg)
	{
		// hello time
		if (m_helloTime.getIsUserConfigured()) 
			pMsg->setHelloTime(m_helloTime.getUI32Value());
		else
			pMsg->setHelloTime(MSTP_DCM_INVALID_HELLO_TIME);

		// max age
		if (m_maxAge.getIsUserConfigured()) 
			pMsg->setMaxAge(m_maxAge.getUI32Value());
		else
			pMsg->setMaxAge(MSTP_DCM_INVALID_MAX_AGE);

		// priority
		if (m_priority.getIsUserConfigured()) 
			pMsg->setPriority(m_priority.getUI32Value());
		else
			pMsg->setPriority(MSTP_DCM_INVALID_PRIORITY);

		// forward delay
		if (m_fwdDelay.getIsUserConfigured()) 
			pMsg->setForwardDelay(m_fwdDelay.getUI32Value());
		else
			pMsg->setForwardDelay(MSTP_DCM_INVALID_FWD_DELAY);

		// aging time
		if (m_agingTime.getIsUserConfigured()) 
			pMsg->setAgingTime(m_agingTime.getUI32Value());
		else
			pMsg->setAgingTime(MSTP_DCM_INVALID_AGING_TIME);

		pMsg->setId(m_id);
		//pMsg->setMode(m_mode);
		pMsg->setCmdCode(MSTP_MSG_POST_BOOT_BR_VLAN_CONFIG);
		return (WAVE_MESSAGE_SUCCESS);
	}


	bool VlanBridgeStpConfigManagedObject::isConfigDefault(MstpGlobalUpdateVlanBridgeConfigMessage *pMsg)
	{
		bool isDefault = true;
        switch (pMsg->m_cmdCode) {
            case MSTP_MSG_BR_VLAN_HELLO_TIME:
            case MSTP_MSG_BR_VLAN_MAX_AGE:
            case MSTP_MSG_BR_VLAN_PRIORITY:
            case MSTP_MSG_BR_VLAN_FWD_DELAY:
            case MSTP_MSG_BR_VLAN_AGING_TIME:
				isDefault = false;
                break;
            default:
                break;
		}
		return (isDefault);
	}

    ResourceId VlanBridgeStpConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateVlanBridgeConfigMessage *pMsg)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        switch (pMsg->m_cmdCode) {
            case MSTP_MSG_BR_VLAN_HELLO_TIME:
                setHelloTime(pMsg->getHelloTime(), true);
                break;
            case MSTP_MSG_BR_VLAN_NO_HELLO_TIME:
                setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
                break;
            case MSTP_MSG_BR_VLAN_MAX_AGE:
                setMaxAge(pMsg->getMaxAge(), true);
                break;
            case MSTP_MSG_BR_VLAN_NO_MAX_AGE:
                setMaxAge(MSTP_DCM_DEFAULT_MAX_AGE);
                break;
            case MSTP_MSG_BR_VLAN_PRIORITY:
                setPriority(pMsg->getPriority(), true);
                break;
            case MSTP_MSG_BR_VLAN_NO_PRIORITY:
                setPriority(MSTP_DCM_DEFAULT_PRIORITY);
                break;
            case MSTP_MSG_BR_VLAN_FWD_DELAY:
                setForwardDelay(pMsg->getForwardDelay(), true);
                break;
            case MSTP_MSG_BR_VLAN_NO_FWD_DELAY:
                setForwardDelay(MSTP_DCM_DEFAULT_FWD_DELAY);
                break;
            case MSTP_MSG_BR_VLAN_AGING_TIME:
                setAgingTime(pMsg->getAgingTime(), true);
                break;
            case MSTP_MSG_BR_VLAN_NO_AGING_TIME:
                setAgingTime(MSTP_DCM_DEFAULT_AGING_TIME);
                break;
            default:
				status = WAVE_MESSAGE_ERROR;
                break;
        }
        return (status);
    }

}
