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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmUpdateBulkPhyIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"

namespace DcmNs
{

    NsmUpdateBulkPhyIntfMessage::NsmUpdateBulkPhyIntfMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMUPDATEBULKPHYINTF)
    {
		m_cmdCode = 0;
		m_msgId = 0;
    }

    NsmUpdateBulkPhyIntfMessage::~NsmUpdateBulkPhyIntfMessage ()
    {
    }

    void  NsmUpdateBulkPhyIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_msgId,"msgId"));
    }


    void  NsmUpdateBulkPhyIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmUpdateBulkPhyIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmUpdateBulkPhyIntfMessage::setMsgId(const UI32 &msgId)
    {
        m_msgId  =  msgId;
    }

    UI32  NsmUpdateBulkPhyIntfMessage::getMsgId() const
    {
        return (m_msgId);
    }

    void NsmUpdateBulkPhyIntfMessage::loadOutputsFromCStructure (unsigned int size,void *pOutputCStructure)
    {
        addBuffer (NSM_UPDATE_INTf_CONFIG, size, pOutputCStructure, false);
	}

	void NsmUpdateBulkPhyIntfMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
	{
		pBuffer   =   findBuffer(bufferNum, size);
	}
}
