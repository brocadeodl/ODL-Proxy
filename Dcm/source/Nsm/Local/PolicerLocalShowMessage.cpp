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
 *   Author : Sudha.
 **************************************************************************/
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PolicerLocalShowMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Policer/PolicerCommon.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    PolicerLocalShowMessage::PolicerLocalShowMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), POLICER_LOCAL_SHOW_ALL)
    {
    }

    PolicerLocalShowMessage::PolicerLocalShowMessage(const string &Interface,
		const string &ifName, const UI32 &direction, const UI32 &cmdCode,
		bool isGwRequest, const string &policyMapName, const UI32 &locationId)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), POLICER_LOCAL_SHOW_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
		m_direction (direction),
        m_cmdCode   (cmdCode),
        m_isGwRequest (isGwRequest),
        m_policyMapName  (policyMapName),
        m_locationId (locationId)
    {
    }

    PolicerLocalShowMessage::~PolicerLocalShowMessage ()
    {
    }

    void  PolicerLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_direction,"direction"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute (new AttributeString(&m_policyMapName,"policyMapName"));
		addSerializableAttribute (new AttributeUI32(&m_locationId,"locationId"));

    }

    void  PolicerLocalShowMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  PolicerLocalShowMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  PolicerLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PolicerLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
	void  PolicerLocalShowMessage::setDirection(const UI32 &direction)
    {
        m_direction  =  direction;
    }

    UI32  PolicerLocalShowMessage::getDirection() const
    {
        return (m_direction);
    }

    void  PolicerLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PolicerLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void PolicerLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool PolicerLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

	string PolicerLocalShowMessage::getPolicyMapName() const {
		return m_policyMapName;
	}

	void PolicerLocalShowMessage::setPolicyMapName(const string &policyMapName) {
		m_policyMapName = policyMapName;
	}

	UI32 PolicerLocalShowMessage::getLocationId() const {
		return m_locationId;
	}

	void PolicerLocalShowMessage::setLocationId(const UI32 &locationId) {
		m_locationId = locationId;
	}

    void PolicerLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void PolicerLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void PolicerLocalShowMessage::transferOutputBuffer(PolicerLocalShowMessage *&pMsg)
    {
        UI32 size = 0;
        UI32 bufferNum = pMsg->getCmdCode();
        void *pBuffer = NULL;

        trace(TRACE_LEVEL_DEVEL, string("PolicerLocalShowMessage::transferOutputBuffer bufNum is ") + bufferNum);
        pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
        if (pBuffer == NULL) {
            trace(TRACE_LEVEL_DEVEL, "PolicerLocalShowMessage::transferOutputBuffer received empty buffer");
            return;
        }
        addBuffer(bufferNum, size, pBuffer);
    }

}
