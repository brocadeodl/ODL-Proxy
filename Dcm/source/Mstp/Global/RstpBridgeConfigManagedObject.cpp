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

#include "Mstp/Global/RstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "vcs.h"


namespace DcmNs
{

    RstpBridgeConfigManagedObject::RstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (RstpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager)
    {
      m_rstp        =   true;
      m_txHoldCount =   MSTP_DCM_DEFAULT_TX_HOLDCOUNT;
      m_helloTime   =   MSTP_DCM_DEFAULT_HELLO_TIME;
    }

    RstpBridgeConfigManagedObject::RstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime,const UI32 &txHoldCount)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (RstpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager),
        m_helloTime    (helloTime),
        m_txHoldCount    (txHoldCount)
    {
      m_rstp        =   true;
    }

    RstpBridgeConfigManagedObject::~RstpBridgeConfigManagedObject ()
    {
    }

    string  RstpBridgeConfigManagedObject::getClassName()
    {
        return ("RstpBridgeConfigManagedObject");
    }

    void  RstpBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        XstpBridgeConfigManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttribute (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
        addPersistableAttribute (new AttributeBool(&m_rstp,"rstp", xstp_rstp));
    }

    void  RstpBridgeConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
        addPersistableAttributeForCreate  (new AttributeBool(&m_rstp,"rstp", xstp_rstp));
    }

    void  RstpBridgeConfigManagedObject::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  RstpBridgeConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  RstpBridgeConfigManagedObject::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  RstpBridgeConfigManagedObject::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  RstpBridgeConfigManagedObject::setDefaultAttribute()
    {
		m_rstp = true;
	    XstpBridgeConfigManagedObject::setDefaultAttribute();
	    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
	    setTxHoldCount(MSTP_DCM_DEFAULT_TX_HOLDCOUNT);

    }


    ResourceId RstpBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    switch (pMsg->m_cmdCode) {
		    case MSTP_MSG_BR_HELLO_TIME:
			    setHelloTime(pMsg->getHelloTime());
			    break;
		    case MSTP_MSG_BR_NO_HELLO_TIME:
			    setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
			    break;
		    case MSTP_MSG_BR_TRANS_HOLDCOUNT:
			    setTxHoldCount(pMsg->getTxHoldCount());
			    break;
		    case MSTP_MSG_BR_NO_TRANS_HOLDCOUNT:
			    setTxHoldCount(MSTP_DCM_DEFAULT_TX_HOLDCOUNT);
			    break;
		    default:
			    status = XstpBridgeConfigManagedObject::setAttributeByMessage(pMsg);
			    break;
	    }
	    return (status);
    }

    ResourceId RstpBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setHelloTime(getHelloTime());
	    pMsg->setTxHoldCount(getTxHoldCount());
	    XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }

}
