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
 *   Author : Glenn Wenig                                                     *
 **************************************************************************/

#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Lldp/Local/LldpLocalLldpDebugMessage.h"
#include "Utils/DcmMemUtil.h"
namespace DcmNs
{

    LldpLocalLldpDebugMessage::LldpLocalLldpDebugMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),
                                      LLDPLOCALLLDPDEBUG)
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALLLDPDEBUGMESSAGE,sizeof(LldpLocalLldpDebugMessage),DCMMEMSTATINCREMENT);
	}

    LldpLocalLldpDebugMessage::LldpLocalLldpDebugMessage (const string &ifName, 
                const UI32 &cmdCode, const UI32 &dbgFlags, const UI32 &lastIfIndex, const UI32 &numRecords, bool isGwRequest)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (), 
                        LLDPLOCALLLDPDEBUG),
    m_ifName    (ifName),
    m_cmdCode    (cmdCode), 
    m_dbgFlags    (dbgFlags), 
    m_lastIfIndex (lastIfIndex),
    m_numRecords  (numRecords),
    m_isGwRequest (isGwRequest)
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALLLDPDEBUGMESSAGE,sizeof(LldpLocalLldpDebugMessage),DCMMEMSTATINCREMENT);
    }

    LldpLocalLldpDebugMessage::~LldpLocalLldpDebugMessage ()
    {
		DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LOCALLLDPDEBUGMESSAGE,sizeof(LldpLocalLldpDebugMessage),DCMMEMSTATDECREMENT);
    }

    void  LldpLocalLldpDebugMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute(new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute(new AttributeUI32(&m_dbgFlags,"dbgFlags"));
        addSerializableAttribute(new AttributeUI32(&m_lastIfIndex,"lastIfIndex"));
        addSerializableAttribute(new AttributeUI32(&m_numRecords,"numRecords"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, 
                                "isGwRequest"));
    }

    void  LldpLocalLldpDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LldpLocalLldpDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  LldpLocalLldpDebugMessage::setDbgFlags(const UI32 &dbgFlags)
    {
        m_dbgFlags  =  dbgFlags;
    }

    UI32  LldpLocalLldpDebugMessage::getDbgFlags() const
    {
        return (m_dbgFlags);
    }

    string 
    LldpLocalLldpDebugMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void 
    LldpLocalLldpDebugMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    void
    LldpLocalLldpDebugMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool
    LldpLocalLldpDebugMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void
    LldpLocalLldpDebugMessage::getOutputBufferRef (UI32 bufferNum, 
        UI32 &size, 
        void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void
    LldpLocalLldpDebugMessage::copyAllBuffers (const PrismMessage &prismMessage) 
    {
        this->copyBuffersFrom(prismMessage);

    }

    UI32 LldpLocalLldpDebugMessage::getLastIfIndex() const
    {
        return m_lastIfIndex;
    }

    UI32 LldpLocalLldpDebugMessage::getNumRecords() const
    {
        return m_numRecords;
    }

    void LldpLocalLldpDebugMessage::setLastIfIndex(const UI32 lastIfIndex)
    {
        m_lastIfIndex = lastIfIndex;
    }

    void LldpLocalLldpDebugMessage::setNumRecords(const UI32 numRecords)
    {
        m_numRecords = numRecords;
    }
}
