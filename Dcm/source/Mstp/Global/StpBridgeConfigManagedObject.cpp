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

#include "Mstp/Global/StpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "vcs.h"

namespace DcmNs
{

    StpBridgeConfigManagedObject::StpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (StpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager)
    {
       m_stp = true;
       m_helloTime=MSTP_DCM_DEFAULT_HELLO_TIME;
    }

    StpBridgeConfigManagedObject::StpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (StpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager),
        m_helloTime    (helloTime)
    {
      m_stp = true;
    }

    StpBridgeConfigManagedObject::~StpBridgeConfigManagedObject ()
    {
    }

    string  StpBridgeConfigManagedObject::getClassName()
    {
        return ("StpBridgeConfigManagedObject");
    }

    void  StpBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        XstpBridgeConfigManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttribute (new AttributeBool(&m_stp,"stp", xstp_stp));
    }

    void  StpBridgeConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttributeForCreate  (new AttributeBool(&m_stp,"stp", xstp_stp));
    }

    void  StpBridgeConfigManagedObject::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  StpBridgeConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  StpBridgeConfigManagedObject::setDefaultAttribute()
    {
		m_stp = true;
	    XstpBridgeConfigManagedObject::setDefaultAttribute();
	    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
    }

    ResourceId StpBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    switch (pMsg->m_cmdCode) {
		    case MSTP_MSG_BR_HELLO_TIME:
			    setHelloTime(pMsg->getHelloTime());
			    break;
		    case MSTP_MSG_BR_NO_HELLO_TIME:
			    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
			    break;
		    default:
			    status = XstpBridgeConfigManagedObject::setAttributeByMessage(pMsg);
			    break;
	    }
	    return (status);
    }

    ResourceId StpBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setHelloTime(getHelloTime());
	    XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }


}
