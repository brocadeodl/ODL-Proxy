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

#include "Mstp/Global/PvstBridgeConfigManagedObject.h"
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

    PvstBridgeConfigManagedObject::PvstBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (PvstBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager)
    {
      m_helloTime=MSTP_DCM_DEFAULT_HELLO_TIME ;
      m_macReduction=MSTP_DCM_MAC_REDUCTION ; 
      m_stp=true;
    }

    PvstBridgeConfigManagedObject::PvstBridgeConfigManagedObject(MstpGlobalObjectManager *pMstpGlobalObjectManager,const UI32 &helloTime,const MstpMacReduction  &macReduction,const vector<ObjectId> &activeVlan,const vector<ObjectId> &inactiveVlan)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (PvstBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager),
        m_helloTime    (helloTime),
        m_macReduction    (macReduction)
    {
       m_stp = true;
    }

    PvstBridgeConfigManagedObject::~PvstBridgeConfigManagedObject ()
    {
    }

    string  PvstBridgeConfigManagedObject::getClassName()
    {
        return ("PvstBridgeConfigManagedObject");
    }

    void  PvstBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        XstpBridgeConfigManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttribute (new AttributeBool(&m_stp,"pvst", xstp_pvst));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_macReduction),"macReduction", hidden_cli_mac_address_reduction));
    }

    void  PvstBridgeConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate (new AttributeUI32(&m_helloTime,"helloTime", xstp_hello_time));
        addPersistableAttributeForCreate (new AttributeBool(&m_stp,"pvst", xstp_pvst));
        addPersistableAttributeForCreate (new AttributeEnum((UI32 *)(&m_macReduction),"macReduction", hidden_cli_mac_address_reduction));
    }

    void  PvstBridgeConfigManagedObject::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  PvstBridgeConfigManagedObject::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  PvstBridgeConfigManagedObject::setMacReduction(const MstpMacReduction &macReduction)
    {
        m_macReduction  =  macReduction;
    }

    MstpMacReduction  PvstBridgeConfigManagedObject::getMacReduction() const
    {
        return (m_macReduction);
    }

	void  PvstBridgeConfigManagedObject::setDefaultAttribute()
	{
		m_stp = true;
		XstpBridgeConfigManagedObject::setDefaultAttribute();
		setHelloTime(MSTP_DCM_DEFAULT_HELLO_TIME);
		setMacReduction(MSTP_DCM_MAC_REDUCTION);
	}


	ResourceId PvstBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
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
			default:
				status = XstpBridgeConfigManagedObject::setAttributeByMessage(pMsg);
				break;
		}
		return (status);
	}

	
    ResourceId PvstBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setHelloTime(m_helloTime);
		pMsg->setMacReduction(m_macReduction==MSTPMACREDUCTIONENABLE);

	    XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }

}
