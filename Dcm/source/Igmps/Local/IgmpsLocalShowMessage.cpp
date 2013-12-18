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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "Igmps/Local/IgmpsLocalShowMessage.h"
#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsTypes.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    IgmpsLocalShowMessage::IgmpsLocalShowMessage()
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALSHOW),
        m_cmdCode       (IGMPS_DEFAULT_CMD_CODE),
        m_isGwRequest   (false),
        m_isMgmtCluster (false)
    {
        
    }

    IgmpsLocalShowMessage::IgmpsLocalShowMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, bool isGwRequest, bool isMgmtCluster)
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (), IGMPSLOCALSHOW),
        m_grpName      (grpName),
        m_ifName       (ifName),
        m_cmdCode       (cmdCode),
        m_isGwRequest (isGwRequest),
        m_isMgmtCluster (isMgmtCluster)
    {
    }

    IgmpsLocalShowMessage::~IgmpsLocalShowMessage ()
    {
    }

    void  IgmpsLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grpName,"grpName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
        addSerializableAttribute(new AttributeBool(&m_isMgmtCluster, "isMgmtCluster"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    }

    void  IgmpsLocalShowMessage::setGrpName(const string &grpName)
    {
        m_grpName  =  grpName;
    }

    string  IgmpsLocalShowMessage::getGrpName() const
    {
        return (m_grpName);
    }
    
    void  IgmpsLocalShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsLocalShowMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  IgmpsLocalShowMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32 IgmpsLocalShowMessage::getIfType() const
    {
        return (m_ifType);
    }


    void  IgmpsLocalShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsLocalShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void IgmpsLocalShowMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool IgmpsLocalShowMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }

    void IgmpsLocalShowMessage::setIsMgmtCluster (bool isMgmtCluster)
    {
		m_isMgmtCluster = isMgmtCluster;
    }

    bool IgmpsLocalShowMessage::getIsMgmtCluster () const
    {
        return (m_isMgmtCluster);
    }

    void IgmpsLocalShowMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void IgmpsLocalShowMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
      
    void IgmpsLocalShowMessage::transferOutputBuffer(IgmpsLocalShowMessage *&pMsg)
    {
		UI32 size = 0;
		UI32 bufferNum = pMsg->getCmdCode();
		void *pBuffer = NULL;

		trace(TRACE_LEVEL_DEVEL, string("IgmpsLocalShowMessage::transferOutputBuffer bufNum is ") + bufferNum);
		pBuffer = pMsg->transferBufferToUser(pMsg->getCmdCode(), size);
		if (pBuffer == NULL) {
			trace(TRACE_LEVEL_DEVEL, "IgmpsLocalShowMessage::transferOutputBuffer received empty buffer");
			return;
		}
		addBuffer(bufferNum, size, pBuffer);
    }
}
