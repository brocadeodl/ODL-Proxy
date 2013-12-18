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
 *   Author : sudheend                                                     *
 **************************************************************************/

#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdatePhyInstanceStpConfigMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoInstanceStpConfigMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoInstanceStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePhyInstanceStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "vcs.h"

namespace DcmNs
{


    XstpPortInstanceConfigManagedObject::XstpPortInstanceConfigManagedObject (WaveObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (XstpPortInstanceConfigManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		setCost(MSTP_DCM_INVALID_COST);
		setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
		setGuard(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
		setRestrictedTcn(MSTP_DCM_DEFAULT_PORT_RES_TCN);
		setRestrictedRole(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
                setId(0);
    }

    XstpPortInstanceConfigManagedObject::~XstpPortInstanceConfigManagedObject ()
    {
    }

    string  XstpPortInstanceConfigManagedObject::getClassName()
    {
        return ("XstpPortInstanceConfigManagedObject");
    }

    void  XstpPortInstanceConfigManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI8(&m_id,"id", xstp_id));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_instanceId,"instanceId", InstanceBaseManagedObject::getClassName (), true));
        addPersistableAttribute (new AttributeString(&m_port,"port"));
        addPersistableAttribute (new AttributeUI32(&m_cost, MSTP_DCM_INVALID_COST, "cost", xstp_cost));
        addPersistableAttribute (new AttributeUI32(&m_priority,"priority", xstp_priority));
        addPersistableAttribute (new AttributeBool(&m_guard,"guardroot", xstp_root));
        addPersistableAttribute (new AttributeBool(&m_restrictedTcn,"restrictedTcn", xstp_restricted_tcn));
        addPersistableAttribute (new AttributeBool(&m_restrictedRole,"restrictedRole", xstp_restricted_role));
    }

    void  XstpPortInstanceConfigManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate (new AttributeUI8(&m_id, "id", xstp_id));
        addPersistableAttributeForCreate (new AttributeObjectIdAssociation(&m_instanceId, "instanceId", InstanceBaseManagedObject::getClassName (), true));
        addPersistableAttributeForCreate (new AttributeString(&m_port,"port"));
        addPersistableAttributeForCreate (new AttributeUI32(&m_cost, MSTP_DCM_INVALID_COST, "cost", xstp_cost));
        addPersistableAttributeForCreate (new AttributeUI32(&m_priority,"priority", xstp_priority));
        addPersistableAttributeForCreate (new AttributeBool(&m_guard,"guardroot", xstp_root));
        addPersistableAttributeForCreate (new AttributeBool(&m_restrictedTcn,"restrictedTcn", xstp_restricted_tcn));
        addPersistableAttributeForCreate (new AttributeBool(&m_restrictedRole,"restrictedRole", xstp_restricted_role));

        vector<string> keyName;
        keyName.push_back("id");
        setUserDefinedKeyCombination (keyName);
    }

    void  XstpPortInstanceConfigManagedObject::setInstanceId(const ObjectId &instanceId)
    {
        m_instanceId  =  instanceId;
    }

    ObjectId  XstpPortInstanceConfigManagedObject::getInstanceId() const
    {
        return (m_instanceId);
    }

    void  XstpPortInstanceConfigManagedObject::setId(const UI8 &id)
    {
        m_id  =  id;
    }

    UI8  XstpPortInstanceConfigManagedObject::getId() const
    {
        return (m_id);
    }
    void  XstpPortInstanceConfigManagedObject::setPort(const string &port)
    {
        m_port  =  port;
    }

    string  XstpPortInstanceConfigManagedObject::getPort() const
    {
        return (m_port);
    }

    void  XstpPortInstanceConfigManagedObject::setCost(const UI32 &cost)
    {
        m_cost  =  cost;
    }

    UI32  XstpPortInstanceConfigManagedObject::getCost() const
    {
        return (m_cost);
    }

    void  XstpPortInstanceConfigManagedObject::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  XstpPortInstanceConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  XstpPortInstanceConfigManagedObject::setGuard(const bool &guard)
    {
        m_guard  =  guard;
    }

    bool  XstpPortInstanceConfigManagedObject::getGuard() const
    {
        return (m_guard);
    }

    void  XstpPortInstanceConfigManagedObject::setRestrictedTcn(const bool &restrictedTcn)
    {
        m_restrictedTcn  =  restrictedTcn;
    }

    bool  XstpPortInstanceConfigManagedObject::getRestrictedTcn() const
    {
        return (m_restrictedTcn);
    }

    void  XstpPortInstanceConfigManagedObject::setRestrictedRole(const bool &restrictedRole)
    {
        m_restrictedRole  =  restrictedRole;
    }

    bool  XstpPortInstanceConfigManagedObject::getRestrictedRole() const
    {
        return (m_restrictedRole);
    }

	void XstpPortInstanceConfigManagedObject::setDefaultAttribute(UI8 &id, ObjectId & instanceId, string &ifName)
	{
		setId(id);
		setInstanceId(instanceId);
		setPort(ifName);
		setCost(MSTP_DCM_INVALID_COST);
		setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
		setGuard(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
		setRestrictedTcn(MSTP_DCM_DEFAULT_PORT_RES_TCN);
		setRestrictedRole(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
	}



	ResourceId  XstpPortInstanceConfigManagedObject::setAttributeByMessage(NsmUpdatePhyInstanceStpConfigMessage *pMsg)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
	    MSTP_PLUG_DBG("Entering...");
		switch (pMsg->getCmdCode()) {
			case MSTP_MSG_IF_INST_COST:
				 setCost(pMsg->getCost());
				break;
			case MSTP_MSG_IF_INST_PRIORITY:
				 setPriority(pMsg->getPriority());
				break;
			case MSTP_MSG_IF_INST_RESTRICTED_ROLE:
				 setRestrictedRole(true);
				break;
			case MSTP_MSG_IF_INST_RESTRICTED_TCN:
				 setRestrictedTcn(true);
				break;

			case MSTP_MSG_IF_INST_GUARDROOT:
				 setGuard(true);
				break;

			case MSTP_MSG_IF_INST_NO_COST:
				 setCost(MSTP_DCM_INVALID_COST);
				break;
			case MSTP_MSG_IF_INST_NO_PRIORITY:
				 setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
				break;
			case MSTP_MSG_IF_INST_NO_RESTRICTED_ROLE:
				 setRestrictedRole(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
				break;
			case MSTP_MSG_IF_INST_NO_RESTRICTED_TCN:
				 setRestrictedTcn(MSTP_DCM_DEFAULT_PORT_RES_TCN);
				break;

			case MSTP_MSG_IF_INST_NO_GUARDROOT:
				 setGuard(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
				break;

			default:
			    MSTP_PLUG_DBG_ERR_S(string("cmd not supprted : ") + pMsg->getCmdCode());
				 status = WAVE_MESSAGE_ERROR;
				break;
		}
		return status;
	}
	
        //template <typename T>
	//ResourceId  XstpPortInstanceConfigManagedObject::setAttributeByMessage(T *pMsg)
	ResourceId  XstpPortInstanceConfigManagedObject::setAttributeByMessage(NsmGlobalUpdatePoInstanceStpConfigMessage *pMsg)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
	    MSTP_PLUG_DBG("Entering...");
		switch (pMsg->getCmdCode()) {
			case MSTP_MSG_IF_INST_COST:
				 setCost(pMsg->getCost());
				break;
			case MSTP_MSG_IF_INST_PRIORITY:
				 setPriority(pMsg->getPriority());
				break;
			case MSTP_MSG_IF_INST_RESTRICTED_ROLE:
				 setRestrictedRole(true);
				break;
			case MSTP_MSG_IF_INST_RESTRICTED_TCN:
				 setRestrictedTcn(true);
				break;

			case MSTP_MSG_IF_INST_GUARDROOT:
				 setGuard(true);
				break;

			case MSTP_MSG_IF_INST_NO_COST:
				 setCost(MSTP_DCM_INVALID_COST);
				break;
			case MSTP_MSG_IF_INST_NO_PRIORITY:
				 setPriority(MSTP_DCM_DEFAULT_PORT_PRIORITY);
				break;
			case MSTP_MSG_IF_INST_NO_RESTRICTED_ROLE:
				 setRestrictedRole(MSTP_DCM_DEFAULT_PORT_RES_ROLE);
				break;
			case MSTP_MSG_IF_INST_NO_RESTRICTED_TCN:
				 setRestrictedTcn(MSTP_DCM_DEFAULT_PORT_RES_TCN);
				break;

			case MSTP_MSG_IF_INST_NO_GUARDROOT:
				 setGuard(MSTP_DCM_DEFAULT_PORT_GUARDROOT);
				break;

			default:
			     MSTP_PLUG_DBG_ERR_S(string("cmd not supprted : ") + pMsg->getCmdCode());
				 status = WAVE_MESSAGE_ERROR;
				break;
		}
		return status;
	}
 
	ResourceId XstpPortInstanceConfigManagedObject::getAttributeToMessage(MstpPortConfigMessage *&pMsg)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		pMsg->setInstanceId (getId());
		pMsg->setInterfaceInfo(NsmUtils::getIfType(getOwnerManagedObjectId()), getPort());

		pMsg->setPriority (getPriority());
		pMsg->setCost (getCost());
		pMsg->setRestrictedRole (getRestrictedRole());
		pMsg->setRestrictedTcn (getRestrictedTcn());
		pMsg->setGuard (getGuard());

		return (status);
	}

	bool XstpPortInstanceConfigManagedObject::isDefault ()
	{
		return ( ( m_cost == MSTP_DCM_INVALID_COST) &&
				( m_priority == MSTP_DCM_DEFAULT_PORT_PRIORITY ) &&
				( m_guard == MSTP_DCM_DEFAULT_PORT_GUARDROOT) &&
				(m_restrictedTcn == MSTP_DCM_DEFAULT_PORT_RES_TCN) &&
				(m_restrictedRole == MSTP_DCM_DEFAULT_PORT_RES_ROLE)) ;
	}

}

