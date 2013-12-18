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

#include "Framework/Attributes/Attributes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeSI64.h"
#include "Framework/Attributes/AttributeBitMap.h"
#include "Framework/Attributes/AttributeDate.h"
#include "Framework/Attributes/AttributeTime.h"
#include "Framework/Attributes/AttributeDateTime.h"
//#include "Framework/Attributes/AttributeDecimal64.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
//#include "Framework/Attributes/AttributeIpV6AddressNetworkMask.h"
#include "Framework/Attributes/AttributeSnmpObjectId.h"
//#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/AttributeTypes.h"

#include "Mstp/Global/MstpBridgeConfigManagedObject.h"
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

    MstpBridgeConfigManagedObject::MstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (MstpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager)
    {
      m_id              = 0;
      m_ciscoInterop    = MSTP_DCM_DEFAULT_CISCO_INTEROP;
      m_maxHop          = MSTP_DCM_DEFAULT_MAX_HOP;
      m_mstp            = true;
      m_revision        = MSTP_DCM_DEFAULT_REVISION;
      m_txHoldCount     = MSTP_DCM_DEFAULT_TX_HOLDCOUNT;
    }

    MstpBridgeConfigManagedObject::MstpBridgeConfigManagedObject (MstpGlobalObjectManager *pMstpGlobalObjectManager,const string &region,const UI16 &revision,const MstpCiscoInterOp &ciscoInterop,const UI32 &txHoldCount,const UI32 &maxHop)
        : PrismElement  (pMstpGlobalObjectManager),
        PrismPersistableObject (MstpBridgeConfigManagedObject::getClassName (), XstpBridgeConfigManagedObject::getClassName ()),
        WaveManagedObject (pMstpGlobalObjectManager),
        DcmManagedObject (pMstpGlobalObjectManager),
        XstpBridgeConfigManagedObject (pMstpGlobalObjectManager),
        m_region    (region),
        m_revision    (revision),
        m_ciscoInterop    (ciscoInterop),
        m_txHoldCount    (txHoldCount),
        m_maxHop    (maxHop)
    {
        m_mstp  = true;
        m_id    = 0;
    }

    MstpBridgeConfigManagedObject::~MstpBridgeConfigManagedObject ()
    {
    }

    string  MstpBridgeConfigManagedObject::getClassName()
    {
        return ("MstpBridgeConfigManagedObject");
    }

    void  MstpBridgeConfigManagedObject::setupAttributesForPersistence()
    {
        XstpBridgeConfigManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_region,"region", xstp_region));
        addPersistableAttribute (new AttributeUI16(&m_revision,"revision", xstp_revision));
        addPersistableAttribute (new AttributeEnum((UI32 *)(&m_ciscoInterop),"ciscoInterop", xstp_cisco_interoperability));
        addPersistableAttribute (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
        addPersistableAttribute (new AttributeUI32(&m_maxHop,"maxHop", xstp_max_hops));
        addPersistableAttribute (new AttributeBool(&m_mstp,"mstp", xstp_mstp));

        //addPersistableAttribute (new AttributeUI32(&m_id,"id", xstp_id));
    }

    Attribute *MstpBridgeConfigManagedObject::getAttributeByUserTagUserDefined (const UI32 &attributeUserTagUserDefined)
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
	    cout << "POinter is " << pAttribute << endl << "\r";


	    //pAttribute = new AttributeBool (true, "id");

	    // This function can be used to return user defined attribute if getAttributeByUserTag returns NULL
	    // in some case
	    return (pAttribute);
    }

    void  MstpBridgeConfigManagedObject::setupAttributesForCreate()
    {
        addPersistableAttributeForCreate  (new AttributeString(&m_region,"region", xstp_region));
        addPersistableAttributeForCreate  (new AttributeUI16(&m_revision,"revision", xstp_revision));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)(&m_ciscoInterop),"ciscoInterop", xstp_cisco_interoperability));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_txHoldCount,"txHoldCount", xstp_transmit_holdcount));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_maxHop,"maxHop", xstp_max_hops));
        addPersistableAttributeForCreate  (new AttributeBool(&m_mstp,"mstp", xstp_mstp));
        //addPersistableAttributeForCreate  (new AttributeUI32(&m_id,"id", xstp_id));
    }

    void  MstpBridgeConfigManagedObject::setRegion(const string &region)
    {
        m_region  =  region;
    }

    string  MstpBridgeConfigManagedObject::getRegion() const
    {
        return (m_region);
    }

    void  MstpBridgeConfigManagedObject::setRevision(const UI16 &revision)
    {
        m_revision  =  revision;
    }

    UI16  MstpBridgeConfigManagedObject::getRevision() const
    {
        return (m_revision);
    }

    void  MstpBridgeConfigManagedObject::setCiscoInterop(const MstpCiscoInterOp &ciscoInterop)
    {
        m_ciscoInterop  =  ciscoInterop;
    }

    MstpCiscoInterOp  MstpBridgeConfigManagedObject::getCiscoInterop() const
    {
        return (m_ciscoInterop);
    }

    void  MstpBridgeConfigManagedObject::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  MstpBridgeConfigManagedObject::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  MstpBridgeConfigManagedObject::setMaxHop(const UI32 &maxHop)
    {
        m_maxHop  =  maxHop;
    }

    UI32  MstpBridgeConfigManagedObject::getMaxHop() const
    {
        return (m_maxHop);
    }

    void  MstpBridgeConfigManagedObject::setDefaultAttribute()
    {
		m_mstp = true;
	    XstpBridgeConfigManagedObject::setDefaultAttribute();
	    setTxHoldCount(MSTP_DCM_DEFAULT_TX_HOLDCOUNT);
	    setRegion(MSTP_DCM_DEFAULT_REGION);
	    setRevision(MSTP_DCM_DEFAULT_REVISION);
	    setCiscoInterop(MSTP_DCM_DEFAULT_CISCO_INTEROP);
	    setMaxHop(MSTP_DCM_DEFAULT_MAX_HOP);
    }

    ResourceId MstpBridgeConfigManagedObject::setAttributeByMessage(MstpGlobalUpdateBridgeConfigMessage *pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    switch (pMsg->m_cmdCode) {
		    case MSTP_MSG_BR_TRANS_HOLDCOUNT:
			    setTxHoldCount(pMsg->getTxHoldCount());
			    break;
		    case MSTP_MSG_BR_NO_TRANS_HOLDCOUNT:
			    setTxHoldCount(MSTP_DCM_DEFAULT_TX_HOLDCOUNT);
			    break;
		    case MSTP_MSG_BR_REGION:
				setRegion(pMsg->getRegion());
			    break;
		    case MSTP_MSG_BR_NO_REGION:
				setRegion(MSTP_DCM_DEFAULT_REGION);
			    break;
		    case MSTP_MSG_BR_REVISION:
			    setRevision(pMsg->getRevision());
			    break;
		    case MSTP_MSG_BR_NO_REVISION:
			    setRevision(MSTP_DCM_DEFAULT_REVISION);
			    break;
		    case MSTP_MSG_BR_CISCO_INTEROP:
				setCiscoInterop(MSTPCISCOINTEROPENABLE);
			    break;
		    case MSTP_MSG_BR_NO_CISCO_INTEROP:
				setCiscoInterop(MSTP_DCM_DEFAULT_CISCO_INTEROP);
			    break;
		    case MSTP_MSG_BR_MAX_HOP:
			    setMaxHop(pMsg->getMaxHop());
			    break;
		    case MSTP_MSG_BR_NO_MAX_HOP:
			    setMaxHop(MSTP_DCM_DEFAULT_MAX_HOP);
			    break;
		    default:
			    status = XstpBridgeConfigManagedObject::setAttributeByMessage(pMsg);
			    break;
	    }
	    return (status);
    }

    ResourceId MstpBridgeConfigManagedObject::getAttributeToMessage(MstpBridgeConfigMessage *&pMsg)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;

	    pMsg->setTxHoldCount(getTxHoldCount());
            pMsg->setRegion(getRegion());
            pMsg->setRevision(getRevision());
            pMsg->setCiscoInterop(MSTPCISCOINTEROPENABLE==m_ciscoInterop);
            pMsg->setMaxHop(getMaxHop());

	    XstpBridgeConfigManagedObject::getAttributeToMessage(pMsg);

	    return (status);
    }

}
