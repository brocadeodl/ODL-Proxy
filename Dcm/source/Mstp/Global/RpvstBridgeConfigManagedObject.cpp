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

#include "Mstp/Global/RpvstBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

#include <algorithm>

using namespace std;

namespace DcmNs
{

    RpvstBridgeConfigManagedObject::RpvstBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (RpvstBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager)
        , m_helloTime ( MSTP_DCM_DEFAULT_HELLO_TIME )
        , m_txHoldCount ( MSTP_DCM_DEFAULT_TX_HOLDCOUNT )
        , m_stp ( true )
        , m_macReduction ( MSTP_DCM_MAC_REDUCTION )
    {
    }

    RpvstBridgeConfigManagedObject::RpvstBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime,const UI32 &txHoldCount,const MstpMacReduction &macReduction,const vector<ObjectId> &activeVlan,const vector<ObjectId> &inactiveVlan)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (RpvstBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager),
        m_helloTime    (helloTime),
        m_txHoldCount    (txHoldCount),
        m_macReduction    (macReduction)
    {
      m_stp=true;
    }

    RpvstBridgeConfigManagedObject::~RpvstBridgeConfigManagedObject ()
    {
    }

    string  RpvstBridgeConfigManagedObject::getClassName()
    {
        return ("RpvstBridgeConfigManagedObject");
    }

    void  RpvstBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        XstpBridgeConfigManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttribute (new AttributeBool(&m_stp,"rpvst", xstp_rpvst));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_macReduction),"macReduction", hidden_cli_mac_address_reduction));
        addPersistableAttribute (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
    }

    void  RpvstBridgeConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttributeForCreate (new AttributeBool(&m_stp,"rpvst", xstp_rpvst));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_macReduction),"macReduction", hidden_cli_mac_address_reduction));
        addPersistableAttributeForCreate (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
    }

    void  RpvstBridgeConfigManagedObject::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  RpvstBridgeConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  RpvstBridgeConfigManagedObject::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  RpvstBridgeConfigManagedObject::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  RpvstBridgeConfigManagedObject::setMacReduction(const MstpMacReduction &macReduction)
    {
        m_macReduction  =  macReduction;
    }

    MstpMacReduction  RpvstBridgeConfigManagedObject::getMacReduction() const
    {
        return (m_macReduction);
    }


	void  RpvstBridgeConfigManagedObject::setDefaultAttribute()
	{
		m_stp = true;
		XstpBridgeConfigManagedObject::setDefaultAttribute();
		setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
		setMacReduction(MSTP_DCM_MAC_REDUCTION);
		setTxHoldCount(MSTP_DCM_DEFAULT_TX_HOLDCOUNT);
	}

	ResourceId RpvstBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		switch (pMsg->m_cmdCode) {
			case MSTP_MSG_BR_HELLO_TIME:
				setHelloTime(pMsg->getHelloTime());
				break;
			case MSTP_MSG_BR_NO_HELLO_TIME:
				setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
				break;
			case MSTP_MSG_BR_MAC_REDUCTION:
				setMacReduction(MSTP_DCM_MAC_REDUCTION);
				break;
			case MSTP_MSG_BR_NO_MAC_REDUCTION:
				setMacReduction(MSTPMACREDUCTIONSTANDARD);
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

	
    ResourceId RpvstBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setHelloTime(m_helloTime);
	    pMsg->setTxHoldCount(m_txHoldCount);
		pMsg->setMacReduction(m_macReduction==MSTPMACREDUCTIONENABLE);

	    XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }

}
