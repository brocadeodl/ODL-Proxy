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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : mmohan                                                       *
 **************************************************************************/

#include "discover/Framework/discoverObjectManager.h"
#include "discover/Framework/discoverMessages.h"
#include "discover/Framework/discoverTypes.h"

namespace DcmNs
{

	// ************* DiscoverStatusMessage ************

    DiscoverStatusMessage::DiscoverStatusMessage ()
        : PrismMessage (DiscoverObjectManager::getPrismServiceId (), DISCOVERSTATUS)
    {
        m_cmdcode = 0;
        m_msgType = 0;
    }

    DiscoverStatusMessage::DiscoverStatusMessage (const UI32 &cmdcode,const UI32 &msgType, const string &vcenter)
        : PrismMessage (DiscoverObjectManager::getPrismServiceId (), DISCOVERSTATUS),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_vcenter    (vcenter)
    {
    }

    DiscoverStatusMessage::~DiscoverStatusMessage ()
    {
    }

    void  DiscoverStatusMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeString(&m_vcenter,"vcenter"));
    }
    
	void  DiscoverStatusMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }
    
	UI32  DiscoverStatusMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  DiscoverStatusMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  DiscoverStatusMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  DiscoverStatusMessage::setVcenter(const string &vcenter)
    {
        m_vcenter = vcenter;
    }

    string  DiscoverStatusMessage::getVcenter() const
    {
        return (m_vcenter);
    }
	
    
	void DiscoverStatusMessage::getOutputBufferRef (UI32 bufferNum, 
                                              UI32 &size, 
                                              void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

}
