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

#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "vcs.h"

namespace DcmNs
{

    MstpInstanceConfigManagedObject::MstpInstanceConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (MstpInstanceConfigManagedObject::getClassName (), InstanceBaseManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        InstanceBaseManagedObject (pMstpGlobalObjectManager)
    {
      m_priority=MSTP_DCM_DEFAULT_PRIORITY;
    }

    MstpInstanceConfigManagedObject::MstpInstanceConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &priority)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (MstpInstanceConfigManagedObject::getClassName (), InstanceBaseManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        InstanceBaseManagedObject (pMstpGlobalObjectManager),
        m_priority    (priority)
    {
    }

    MstpInstanceConfigManagedObject::~MstpInstanceConfigManagedObject ()
    {
    }

    string  MstpInstanceConfigManagedObject::getClassName()
    {
        return ("MstpInstanceConfigManagedObject");
    }

    void  MstpInstanceConfigManagedObject::setupAttributesForPersistence()
    {
	UI32 defaultPriority = MSTP_DCM_DEFAULT_PRIORITY;

        InstanceBaseManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_priority, defaultPriority, "priority", xstp_priority));
    }

    void  MstpInstanceConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeUI32(&m_priority, MSTP_DCM_DEFAULT_PRIORITY, "priority", xstp_priority));
    }

    void  MstpInstanceConfigManagedObject::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpInstanceConfigManagedObject::getPriority() const
    {
        return (m_priority);
    }

    Attribute *MstpInstanceConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
    {
	    Attribute *pAttribute = NULL;
#if 0
	    Attribute *pAttribute = new Attribute;
	bool * temp = new bool;
	*temp = true;

	    pAttribute->setAttributeType(AttributeType::AttributeTypeBool);
	    pAttribute->setValue((void *)temp);

#endif
            pAttribute = new AttributeBool (true, "mstp");

	    trace (TRACE_LEVEL_ERROR,
			    string("MstpBridgeConfigManagedObject::getAttributeByUserTagUserDefined: Entry "));
	    //pAttribute = new AttributeBool (true, "id");

	    // This function can be used to return user defined attribute if getAttributeByUserTag returns NULL
	    // in some case
	    return (pAttribute);
    }

    void MstpInstanceConfigManagedObject::setDefaultAttribute(const UI8 instanceId)
    {
	    InstanceBaseManagedObject::setDefaultAttribute(instanceId);
	    setPriority(MSTP_DCM_DEFAULT_PRIORITY);
    }

    void  MstpInstanceConfigManagedObject::setDefaultAttribute()
    {
	    InstanceBaseManagedObject::setDefaultAttribute();
	    setPriority(MSTP_DCM_DEFAULT_PRIORITY);
    }

    ResourceId MstpInstanceConfigManagedObject::setAttributeByMessage(const MstpGlobalUpdateMstpInstanceConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    switch (pMsg->m_cmdCode) {
		    case MSTP_MSG_BR_INST_PRIORITY:
			    setPriority(pMsg->getPriority());
			    break;
		    case MSTP_MSG_BR_INST_NO_PRIORITY:
			    setPriority(MSTP_DCM_DEFAULT_PRIORITY);
			    break;
		    default:
		        status = InstanceBaseManagedObject::setAttributeByMessage(pMsg);
			    break;
	    }
	    return (status);
    }

    ResourceId MstpInstanceConfigManagedObject::getAttributeToMessage(MstpInstanceConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setPriority(getPriority());
	    InstanceBaseManagedObject::getAttributeToMessage(pMsg);
	    //XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }


}
