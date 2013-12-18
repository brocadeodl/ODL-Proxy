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

/******************************************************************************
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          	      *
 *   All rights reserved.                                                     *
 *   .Author : Mobin Mobin                                              	      *
 *****************************************************************************/

#include "Nsm/Local/ConfigMgmt/ConfigMgmtLocalShowMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    ConfigMgmtLocalShowMessage::ConfigMgmtLocalShowMessage()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), 
						CONFIGMGMTLOCALSHOW)
    {
		m_cmdCode = 0;
		m_isGwRequest = true;
		m_mappedId = 0;
    }

    ConfigMgmtLocalShowMessage::ConfigMgmtLocalShowMessage(
					const UI32 &cmdCode, const UI32 &mappedId,
					bool isGwRequest)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), 
						CONFIGMGMTLOCALSHOW),
        m_cmdCode       (cmdCode),
        m_mappedId         (mappedId),
        m_isGwRequest (isGwRequest)
    {
    }

    ConfigMgmtLocalShowMessage::~ConfigMgmtLocalShowMessage ()
    {
    }

    void  ConfigMgmtLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeBool(&m_isGwRequest, 
															"isGwRequest"));
        addSerializableAttribute (new AttributeString(&m_result,"result"));
    }

    void  ConfigMgmtLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  ConfigMgmtLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
	
	void  ConfigMgmtLocalShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  ConfigMgmtLocalShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }
    
	void ConfigMgmtLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool ConfigMgmtLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }
    
	void  ConfigMgmtLocalShowMessage::setResult(const string &result)
    {
        m_result  =  result;
    }

    string  ConfigMgmtLocalShowMessage::getResult() const
    {
        return (m_result);
    }

    void ConfigMgmtLocalShowMessage::getOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void ConfigMgmtLocalShowMessage::copyAllBuffers (
											const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
    
	void ConfigMgmtLocalShowMessage::copyAllResult (
											 ConfigMgmtLocalShowMessage &pMsg)
    {   
		string result = this->getResult();
		string added_result = result + pMsg.getResult();
        
		this->setResult(added_result);
    }
      
    void ConfigMgmtLocalShowMessage::transferOutputBuffer(
											ConfigMgmtLocalShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		trace(TRACE_LEVEL_DEVEL, string("ConfigMgmtLocalShowMessage::"
								"transferOutputBuffer bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			trace(TRACE_LEVEL_DEVEL, "ConfigMgmtLocalShowMessage::"
								"transferOutputBuffer received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }
}
