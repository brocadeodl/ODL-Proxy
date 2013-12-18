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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mobin                                              	   *
 **************************************************************************/

#include "Nsm/Local/LacpLocalShowMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/LacpShowMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    LacpLocalShowMessage::LacpLocalShowMessage()
        : PoPaginationLocalMessage (NsmLocalObjectManager::getPrismServiceId (), LACPLOCALSHOW)
    {
		m_cmdCode = 0;
	    m_isGwRequest = true;
		m_poNum = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    LacpLocalShowMessage::LacpLocalShowMessage(const string &ifName, const UI32 &poNum, const UI32 &cmdCode, bool isGwRequest)
        : PoPaginationLocalMessage (NsmLocalObjectManager::getPrismServiceId (), LACPLOCALSHOW, isGwRequest),
        m_ifName       (ifName),
        m_poNum         (poNum),
        m_cmdCode       (cmdCode)
    {
		m_ifType = IF_TYPE_INVALID;
    }

    LacpLocalShowMessage::~LacpLocalShowMessage ()
    {
    }

    void  LacpLocalShowMessage::setupAttributesForSerialization()
    {
    	PoPaginationLocalMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_poNum,"poNum"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        //addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  LacpLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  LacpLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
	void LacpLocalShowMessage::setPoNum (const UI32 &poNum)
    {
        m_poNum = poNum;
    }

    UI32 LacpLocalShowMessage::getPoNum () const
    {
        return (m_poNum);
    }


    void  LacpLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LacpLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
    
	/*void LacpLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool LacpLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }*/

    void  LacpLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  LacpLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }

    void LacpLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void LacpLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
      
    void LacpLocalShowMessage::transferOutputBuffer(LacpLocalShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		trace(TRACE_LEVEL_DEVEL, string("LacpLocalShowMessage::transferOutputBuffer bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			trace(TRACE_LEVEL_DEVEL, "LacpLocalShowMessage::transferOutputBuffer received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }
}
