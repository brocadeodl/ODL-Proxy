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
#include "Nsm/Local/PolicerLocalClearMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Policer/PolicerCommon.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    PolicerLocalClearMessage::PolicerLocalClearMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), POLICER_LOCAL_CLEAR_ALL)
    {
    }

    PolicerLocalClearMessage::PolicerLocalClearMessage (const string &Interface, const string &ifName, const string &mapName, const UI32 &direction, const UI32 &cmdCode, bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), POLICER_LOCAL_CLEAR_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
		m_mapName   (mapName),
		m_direction (direction),
        m_cmdCode   (cmdCode),
        m_isGwRequest (isGwRequest)
    {
    }

    PolicerLocalClearMessage::~PolicerLocalClearMessage ()
    {
    }

    void  PolicerLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_direction,"direction"));
		addSerializableAttribute (new AttributeString(&m_mapName,"mapName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }

    void  PolicerLocalClearMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  PolicerLocalClearMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  PolicerLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  PolicerLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }

	void  PolicerLocalClearMessage::setMapName(const string &mapName)
    {
        m_mapName  =  mapName;
    }

    string  PolicerLocalClearMessage::getMapName() const
    {
        return (m_mapName);
    }

    
	void  PolicerLocalClearMessage::setDirection(const UI32 &direction)
    {
        m_direction  =  direction;
    }

    UI32  PolicerLocalClearMessage::getDirection() const
    {
        return (m_direction);
    }

    void  PolicerLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  PolicerLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    void PolicerLocalClearMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool PolicerLocalClearMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void PolicerLocalClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void PolicerLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }

    void PolicerLocalClearMessage::transferOutputBuffer(PolicerLocalClearMessage *&pMsg)
    {
        UI32 size = 0;
        UI32 bufferNum = pMsg->getCmdCode();
        void *pBuffer = NULL;

        trace(TRACE_LEVEL_DEVEL, string("PolicerLocalClearMessage::transferOutputBuffer bufNum is ") + bufferNum);
        pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
        if (pBuffer == NULL) {
            trace(TRACE_LEVEL_DEVEL, "PolicerLocalClearMessage::transferOutputBuffer received empty buffer");
            return;
        }
        addBuffer(bufferNum, size, pBuffer);
    }

}
