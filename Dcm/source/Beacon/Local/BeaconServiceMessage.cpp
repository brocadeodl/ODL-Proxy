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
 **************************************************************************/
#include "Beacon/Local/BeaconLocalObjectManager.h"
#include "Beacon/Local/BeaconServiceMessage.h"
#include "Beacon/Local/BeaconActTypes.h"


namespace DcmNs
{

    BeaconServiceMessage:: BeaconServiceMessage()
        : PrismMessage (BeaconLocalObjectManager::getPrismServiceId (), BEACONSERVICE)
    {
    }

	BeaconServiceMessage:: BeaconServiceMessage (const UI32 cmdId, const string
&ifName, const SI32 mstatus, const UI32 ifType)
		: PrismMessage (BeaconLocalObjectManager::getPrismServiceId (), BEACONSERVICE),
		  m_cmdId	(cmdId),
		  m_if		(ifName),
		  m_status (mstatus),
		  m_ifType	(ifType)
    {
    }

    BeaconServiceMessage::~BeaconServiceMessage ()
    {
    }

    void  BeaconServiceMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdId,"cmdId"));
		addSerializableAttribute (new AttributeString(&m_if,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_status,"msgStatus"));
    	addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
	}

	void  BeaconServiceMessage::setCmdId(const UI32 cmdId)
    {
        m_cmdId =  cmdId;
    }

    void  BeaconServiceMessage::setIfType(const UI32 ifType)
    {
        m_ifType =  ifType;
    }
	
	void  BeaconServiceMessage::setIf(const string &ifName)
    {
        m_if  =  ifName;
    }

    UI32  BeaconServiceMessage::getCmdId() const
    {
        return (m_cmdId);
    }

    UI32  BeaconServiceMessage::getIfType() const
    {
        return (m_ifType);
    }

    string  BeaconServiceMessage::getIf() const
    {
        return (m_if);
    }

    void  BeaconServiceMessage::setStatus(const SI32 msgStatus)
    {
        m_status  =  msgStatus;
    }

    SI32  BeaconServiceMessage::getStatus() const
    {
		return (m_status);
    }

}
