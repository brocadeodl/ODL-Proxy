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

#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Nsm/Common/NsmPortStpConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpPortVlanConfigMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "vcs.h"

namespace DcmNs
{

    XstpPortVlanConfigManagedObject::XstpPortVlanConfigManagedObject(WaveObjectManager *pNsmGlobalObjectManager)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (XstpPortVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager)
    {
		m_cost		= UI32UC(MSTP_DCM_DEFAULT_PORT_COST);
		m_priority	= UI32UC(MSTP_DCM_DEFAULT_PORT_PRIORITY);
		m_guard		= BoolUC(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
		m_restrictedTcn		= BoolUC(MSTP_DCM_DEFAULT_PORT_RES_TCN);
		m_restrictedRole	= BoolUC(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
		setId(0);
    }

#if 0
    XstpPortVlanConfigManagedObject::XstpPortVlanConfigManagedObject(WaveObjectManager *pNsmGlobalObjectManager,const ObjectId &vlanObjectId,const string &port,const UI32 &cost,const UI32 &priority,const bool &guard,const bool &restrictedTcn,const bool &restrictedRole)
        : PrismElement  (pNsmGlobalObjectManager),
        PrismPersistableObject (XstpPortVlanConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pNsmGlobalObjectManager),
        DcmManagedObject (pNsmGlobalObjectManager),
        m_vlanObjectId    (vlanObjectId),
        m_port    (port),
        m_cost    (cost),
        m_priority    (priority),
        m_guard    (guard),
        m_restrictedTcn    (restrictedTcn),
        m_restrictedRole    (restrictedRole)
    {
    }
#endif

    XstpPortVlanConfigManagedObject::~XstpPortVlanConfigManagedObject ()
    {
    }

    string  XstpPortVlanConfigManagedObject::getClassName()
    {
        return ("XstpPortVlanConfigManagedObject");
    }

    Attribute *XstpPortVlanConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
        Attribute *pAttribute = NULL;
		pAttribute = new AttributeBool (true, "mstp");
        trace (TRACE_LEVEL_ERROR,
                string("XstpPortVlanConfigManagedObject::getAttributeByUserTagUserDefined: Entry "));
        cout << "POinter is " << pAttribute << endl << "\r";
        return (pAttribute);
    }

    void  XstpPortVlanConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_vlanObjectId,"id", VlanIntfManagedObject::getClassName (), xstp_id));
        addPersistableAttribute (new AttributeString(&m_port,"port"));
        addPersistableAttribute (new AttributeUI32UC(&m_cost, false, false, 0, "cost", xstp_cost));
        addPersistableAttribute (new AttributeUI32UC(&m_priority, false, false, 0, "priority", xstp_priority));
        addPersistableAttribute (new AttributeBoolUC(&m_guard, false, "guardroot", xstp_root));
        addPersistableAttribute (new AttributeBoolUC(&m_restrictedTcn, false, "restrictedTcn", xstp_restricted_tcn));
        addPersistableAttribute (new AttributeBoolUC(&m_restrictedRole, false, "restrictedRole", xstp_restricted_role));
        addPersistableAttribute (new AttributeUI32(&m_id,"vlanId"));
    }

    void  XstpPortVlanConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_vlanObjectId,"id", VlanIntfManagedObject::getClassName (), xstp_id));
        addPersistableAttributeForCreate (new AttributeString(&m_port,"port"));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_cost, false, false, 0, "cost", xstp_cost));
        addPersistableAttributeForCreate (new AttributeUI32UC(&m_priority, false, false, 0, "priority", xstp_priority));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_guard, false, "guardroot", xstp_root));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_restrictedTcn, false, "restrictedTcn", xstp_restricted_tcn));
        addPersistableAttributeForCreate (new AttributeBoolUC(&m_restrictedRole, false, "restrictedRole", xstp_restricted_role));
        addPersistableAttributeForCreate (new AttributeUI32(&m_id,"vlanId"));

        vector<string> keyName;
        keyName.push_back("id");
        setUserDefinedKeyCombination (keyName);
    }

    void  XstpPortVlanConfigManagedObject::setVlanObjectId(const ObjectId &vlanObjectId)
    {
        m_vlanObjectId  =  vlanObjectId;
    }

    ObjectId  XstpPortVlanConfigManagedObject::getVlanObjectId() const
    {
        return (m_vlanObjectId);
    }

    void  XstpPortVlanConfigManagedObject::setPort(const string &port)
    {
        m_port  =  port;
    }

    string  XstpPortVlanConfigManagedObject::getPort() const
    {
        return (m_port);
    }

    void  XstpPortVlanConfigManagedObject::setId(const UI32 &id)
    {
        m_id  =  id;
    }

    UI32  XstpPortVlanConfigManagedObject::getId() const
    {
        return (m_id);
    }

    void  XstpPortVlanConfigManagedObject::setCost(const UI32 &val, bool config)
    {
        m_cost.setIsUserConfigured(config);
        m_cost.setUI32Value(val);
    }

    UI32UC  XstpPortVlanConfigManagedObject::getCost() const
    {
        return (m_cost);
    }

    void  XstpPortVlanConfigManagedObject::setPriority(const UI32 &val, bool config )
    {
        m_priority.setIsUserConfigured(config);
        m_priority.setUI32Value(val);
    }

    UI32UC  XstpPortVlanConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  XstpPortVlanConfigManagedObject::setGuard(const bool &val, bool config)
    {
        m_guard.setIsUserConfigured(config);
        m_guard.setBoolValue(val);
    }

    BoolUC  XstpPortVlanConfigManagedObject::getGuard() const
    {
        return (m_guard);
    }

    void  XstpPortVlanConfigManagedObject::setRestrictedTcn(const bool &val, bool config)
    {
        m_restrictedTcn.setIsUserConfigured(config);
        m_restrictedTcn.setBoolValue(val);
    }

    BoolUC  XstpPortVlanConfigManagedObject::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  XstpPortVlanConfigManagedObject::setRestrictedRole(const bool &val, bool config)
    {
        m_restrictedRole.setIsUserConfigured(config);
        m_restrictedRole.setBoolValue(val);
    }

    BoolUC  XstpPortVlanConfigManagedObject::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

	void XstpPortVlanConfigManagedObject::setDefaultAttribute(int id, ObjectId & vlanObjectId, string &ifName)
	{
		setVlanObjectId(vlanObjectId);
		setPort(ifName);
		m_cost		= UI32UC(MSTP_DCM_DEFAULT_PORT_COST);
		m_priority	= UI32UC(MSTP_DCM_DEFAULT_PORT_PRIORITY);
		m_guard		= BoolUC(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
		m_restrictedTcn		= BoolUC(MSTP_DCM_DEFAULT_PORT_RES_TCN);
		m_restrictedRole	= BoolUC(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
		setId(id);
	}

	ResourceId XstpPortVlanConfigManagedObject::getAttributeToMessage(MstpPortVlanConfigMessage *&pMsg)
	{
		pMsg->setInterfaceInfo(NsmUtils::getIfType(getOwnerManagedObjectId()), m_port);
		pMsg->setVid(m_id);

		// priority 
        if (m_priority.getIsUserConfigured())
            pMsg->setPriority(m_priority.getUI32Value());
        else
            pMsg->setPriority(MSTP_DCM_INVALID_PRIORITY);

		// cost 
        if (m_cost.getIsUserConfigured())
            pMsg->setCost(m_cost.getUI32Value());
        else
            pMsg->setCost(MSTP_DCM_INVALID_COST);

		// root guard 
        if (m_guard.getIsUserConfigured())
            pMsg->setGuard(m_guard.getBoolValue());
        else
            pMsg->setGuard(MSTP_DCM_INVALID_GUARDROOT);
		
		// restricted role
        if (m_restrictedRole.getIsUserConfigured())
            pMsg->setRestrictedRole(m_restrictedRole.getBoolValue());
        else
            pMsg->setRestrictedRole(MSTP_DCM_INVALID_RES_ROLE);

		// restricted tcn
        if (m_restrictedTcn.getIsUserConfigured())
            pMsg->setRestrictedTcn(m_restrictedTcn.getBoolValue());
        else
            pMsg->setRestrictedTcn(MSTP_DCM_INVALID_RES_TCN);
	return (WAVE_MESSAGE_SUCCESS);
	}


	bool XstpPortVlanConfigManagedObject::isDefault()
	{
		return (!m_priority.getIsUserConfigured() &&
				!m_cost.getIsUserConfigured() &&
				!m_guard.getIsUserConfigured() &&
				!m_restrictedRole.getIsUserConfigured() &&
				!m_restrictedTcn.getIsUserConfigured());
	}

	bool XstpPortVlanConfigManagedObject::isConfigDefault(NsmPortStpConfigMessage *pMsg)
	{
		bool isDefault = true;
        switch (pMsg->getCmdCode()) {
			case MSTP_MSG_IF_VLAN_COST:
			case MSTP_MSG_IF_VLAN_PRIORITY:
			case MSTP_MSG_IF_VLAN_RESTRICTED_ROLE:
			case MSTP_MSG_IF_VLAN_RESTRICTED_TCN:
			case MSTP_MSG_IF_VLAN_GUARDROOT:
				isDefault = false;
				break;
			default:
				break;
		}
		return (isDefault);
	}


	ResourceId  XstpPortVlanConfigManagedObject::setAttributeByMessage(NsmPortStpConfigMessage *pMsg)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
			trace (TRACE_LEVEL_DEBUG, string("cmdcode ")+pMsg->getCmdCode());
        switch (pMsg->getCmdCode()) {
			case MSTP_MSG_IF_VLAN_COST:
				setCost(pMsg->getCost(), true);
				break;
			case MSTP_MSG_IF_VLAN_PRIORITY:
				setPriority(pMsg->getPriority(), true);
				break;
			case MSTP_MSG_IF_VLAN_RESTRICTED_ROLE:
				setRestrictedRole(true, true);
				break;
			case MSTP_MSG_IF_VLAN_RESTRICTED_TCN:
				setRestrictedTcn(true, true);
				break;
			case MSTP_MSG_IF_VLAN_GUARDROOT:
				setGuard(true, true);
				break;
			case MSTP_MSG_IF_VLAN_NO_COST:
				setCost(MSTP_DCM_DEFAULT_PORT_COST, false);
				break;
			case MSTP_MSG_IF_VLAN_NO_PRIORITY:
				setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY, false);
				break;
			case MSTP_MSG_IF_VLAN_NO_RESTRICTED_ROLE:
				setRestrictedRole(MSTP_DCM_DEFAULT_PORT_RES_ROLE, false);
				break;
			case MSTP_MSG_IF_VLAN_NO_RESTRICTED_TCN:
				setRestrictedTcn(MSTP_DCM_DEFAULT_PORT_RES_TCN, false);
				break;
			case MSTP_MSG_IF_VLAN_NO_GUARDROOT:
				setGuard(MSTP_DCM_DEFAULT_PORT_GUARDROOT, false);
				break;
			default:
				status = WAVE_MESSAGE_ERROR;
				break;
		}
		return status;
	}
}
