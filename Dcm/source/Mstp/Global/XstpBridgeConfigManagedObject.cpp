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

#include "Framework/Attributes/AttributeEnum.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "vcs.h"

namespace DcmNs
{

    XstpBridgeConfigManagedObject::XstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (XstpBridgeConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager)
    {
      setDefaultAttribute();
    }

    XstpBridgeConfigManagedObject::XstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &priority,const string &description,const UI32 &errDisableInterval,const bool &errDisableEnable,const UI32 &forwardDelay,const UI32 &maxAge,const MstpPoPathcost &poPathcost,const bool &stpDisable)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (XstpBridgeConfigManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        m_priority    (priority),
        m_description    (description),
        m_errDisableInterval    (errDisableInterval),
        m_errDisableEnable    (errDisableEnable),
        m_forwardDelay    (forwardDelay),
        m_maxAge    (maxAge),
        m_poPathcost    (poPathcost),
        m_stpDisable    (stpDisable)
    {
        m_brMacByte = 0;
    }

    XstpBridgeConfigManagedObject::~XstpBridgeConfigManagedObject ()
    {
    }

    string  XstpBridgeConfigManagedObject::getClassName()
    {
        return ("XstpBridgeConfigManagedObject");
    }

    void  XstpBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_priority,"priority", xstp_bridge_priority));
        addPersistableAttribute (new AttributeString(&m_description,"description", xstp_description ));
        addPersistableAttribute (new AttributeUI32(&m_errDisableInterval,"errDisableInterval", xstp_interval));
        addPersistableAttribute (new AttributeBool(&m_errDisableEnable,"errDisableEnable", xstp_enable));
        addPersistableAttribute (new AttributeUI32(&m_forwardDelay,"forwardDelay", xstp_forward_delay));
        addPersistableAttribute (new AttributeUI32(&m_maxAge,"maxAge",xstp_max_age ));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_poPathcost),"poPathcost", xstp_path_cost));
        addPersistableAttribute (new AttributeBool(&m_stpDisable,"stpDisable", xstp_shutdown));
        addPersistableAttribute (new AttributeUI8(&m_brMacByte, "brMacByte"));
    }

    void  XstpBridgeConfigManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_priority,"priority", xstp_bridge_priority));
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description", xstp_description));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_errDisableInterval,"errDisableInterval", xstp_interval  ));
        addPersistableAttributeForCreate  (new AttributeBool(&m_errDisableEnable,"errDisableEnable", xstp_enable ));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_forwardDelay,"forwardDelay", xstp_forward_delay));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_maxAge,"maxAge", xstp_max_age));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_poPathcost),"poPathcost", xstp_path_cost));
        addPersistableAttributeForCreate  (new AttributeBool(&m_stpDisable,"stpDisable", xstp_shutdown));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_brMacByte, "brMacByte"));
    }

    void  XstpBridgeConfigManagedObject::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  XstpBridgeConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  XstpBridgeConfigManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  XstpBridgeConfigManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  XstpBridgeConfigManagedObject::setErrDisableInterval(const UI32 &errDisableInterval)
    {
        m_errDisableInterval  =  errDisableInterval;
    }

    UI32  XstpBridgeConfigManagedObject::getErrDisableInterval() const
    {
        return (m_errDisableInterval);
    }

    void  XstpBridgeConfigManagedObject::setErrDisableEnable(const bool &errDisableEnable)
    {
        m_errDisableEnable  =  errDisableEnable;
    }

    bool  XstpBridgeConfigManagedObject::getErrDisableEnable() const
    {
        return (m_errDisableEnable);
    }

    void  XstpBridgeConfigManagedObject::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  XstpBridgeConfigManagedObject::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  XstpBridgeConfigManagedObject::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  XstpBridgeConfigManagedObject::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  XstpBridgeConfigManagedObject::setPoPathcost(const MstpPoPathcost &poPathcost)
    {
        m_poPathcost  =  poPathcost;
    }

    MstpPoPathcost  XstpBridgeConfigManagedObject::getPoPathcost() const
    {
        return (m_poPathcost);
    }

    void  XstpBridgeConfigManagedObject::setStpDisable(const bool &stpDisable)
    {
        m_stpDisable  =  stpDisable;
    }

    bool  XstpBridgeConfigManagedObject::getStpDisable() const
    {
        return (m_stpDisable);
    }

    void XstpBridgeConfigManagedObject::setBridgeMacByte (UI8 value)
    {
        m_brMacByte = value;
    }

    UI8 XstpBridgeConfigManagedObject::getBridgeMacByte () const
    {
        return m_brMacByte;
    }

    void XstpBridgeConfigManagedObject::setDefaultAttribute()
	{
		setPriority(MSTP_DCM_DEFAULT_PRIORITY);
		setStpDisable(MSTP_DCM_DEFUALT_PROTO_STATUS);
		setDescription(MSTP_DCM_DEFAULT_DESCRIPTION);
		setErrDisableEnable(MSTP_DCM_DEFAULT_ERR_TIMEOUT);
		setErrDisableInterval(MSTP_DCM_DEFAULT_ERR_INTERVAL);
		setForwardDelay(MSTP_DCM_DEFAULT_FWD_DELAY);
		setMaxAge(MSTP_DCM_DEFAULT_MAX_AGE);
		setPoPathcost(MSTP_DCM_DEFAULT_PATHCOST);
        setBridgeMacByte(0);
	}

    ResourceId XstpBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
    {
		
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		switch (pMsg->m_cmdCode) {
			case MSTP_MSG_BR_PRIORITY:
				setPriority(pMsg->getPriority());
				break;
			case MSTP_MSG_BR_NO_PRIORITY:
				setPriority(MSTP_DCM_DEFAULT_PRIORITY);
				break;
			case MSTP_MSG_BR_PROTO_ENABLE:
				setStpDisable(MSTP_DCM_DEFUALT_PROTO_STATUS);
				break;
			case MSTP_MSG_BR_NO_PROTO_ENABLE:
				setStpDisable(true);
				break;
			case MSTP_MSG_BR_DESCRITION:
				setDescription(pMsg->getDesc());
				break;
			case MSTP_MSG_BR_NO_DESCRITION:
				setDescription(MSTP_DCM_DEFAULT_DESCRIPTION);
				break;
			case MSTP_MSG_BR_ERR_TIMEOUT:
				setErrDisableEnable(true);
				break;
			case MSTP_MSG_BR_NO_ERR_TIMEOUT:
				setErrDisableEnable(MSTP_DCM_DEFAULT_ERR_TIMEOUT);
				break;
			case MSTP_MSG_BR_ERR_INTERVAL:
				setErrDisableInterval(pMsg->getErrDisableInterval());
				break;
			case MSTP_MSG_BR_NO_ERR_INTERVAL:
				setErrDisableInterval(MSTP_DCM_DEFAULT_ERR_INTERVAL);
				break;
			case MSTP_MSG_BR_FWD_DELAY:
				setForwardDelay(pMsg->getForwardDelay());
				break;
			case MSTP_MSG_BR_NO_FWD_DELAY:
				setForwardDelay(MSTP_DCM_DEFAULT_FWD_DELAY);
				break;
			case MSTP_MSG_BR_MAX_AGE:
				setMaxAge(pMsg->getMaxAge());
				break;
			case MSTP_MSG_BR_NO_MAX_AGE:
				setMaxAge(MSTP_DCM_DEFAULT_MAX_AGE);
				break;
			case MSTP_MSG_BR_PATHCOST_STDANDARD:
				setPoPathcost(MSTPPOPATHCOSTSTANDARD);
				break;
			case MSTP_MSG_BR_NO_PATHCOST_STDANDARD:
				setPoPathcost(MSTPPOPATHCOSTCUSTOM);
				break;
			default:
				status = WAVE_MESSAGE_ERROR;
				break;
		}
        return (status);
    }

    ResourceId XstpBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    pMsg->setDesc (getDescription());
	    pMsg->setPriority (getPriority());
	    pMsg->setErrDisableInterval (getErrDisableInterval());
	    pMsg->setErrDisableEnable (getErrDisableEnable());
	    pMsg->setForwardDelay (getForwardDelay());
	    pMsg->setMaxAge (getMaxAge());
	    pMsg->setPoPathcost (m_poPathcost == MSTPPOPATHCOSTSTANDARD);
	    pMsg->setStpDisable (getStpDisable());

	    return (status);
    }


}
