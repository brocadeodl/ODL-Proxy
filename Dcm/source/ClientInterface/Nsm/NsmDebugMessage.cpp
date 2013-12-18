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
 *   Author : bvreddy	                                                   *
 **************************************************************************/
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/NsmDebugMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    NsmDebugMessage::NsmDebugMessage ()
        : DcmManagementInterfaceMessage (NSM_DEBUG_HANDLER)
    {
    }

    NsmDebugMessage::NsmDebugMessage (const UI32 &DebugType, const UI32 &cmdCode,const UI32 &mappedId)
        : DcmManagementInterfaceMessage (NSM_DEBUG_HANDLER),
        m_DebugType   (DebugType),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
    }


    NsmDebugMessage::~NsmDebugMessage ()
    {
    }

    void  NsmDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_DebugType,"DebugType"));
		addSerializableAttribute(new AttributeUI32(&m_mappedId,"mappedId"));
    }


    void  NsmDebugMessage::setDebugType(const UI32 &DebugType)
    {
        m_DebugType  =  DebugType;
    }

    UI32  NsmDebugMessage::getDebugType() const
    {
        return (m_DebugType);
    }

    void  NsmDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmDebugMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  NsmDebugMessage::getMappedId() const
    {
        return (m_mappedId);
    }


    void NsmDebugMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *NsmDebugMessage::getCStructureForInputs ()
    {
        NsmDebug_InputMsg_t *pInput = new NsmDebug_InputMsg_t;
		pInput->cmdCode     = m_cmdCode;
		pInput->DebugType     = m_DebugType;
        return (pInput);
    }

	void NsmDebugMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		NSM_PLUG_DBG("Entering...");

		if(pOutputCStructure)
		{
		nsm_mgmt_buffer_num_t  buff_num = {{0}};
		
		switch(m_cmdCode)
		{
		    case NSMDEBUGSHOW_TRACE:
			{	
				buff_num.buff.mapped_id = this->getMappedId();
			    buff_num.buff.tag = NSM_DEBUG_HANDLER;
               	addBuffer (buff_num.num, (1024), ( ((unsigned char *)pOutputCStructure )) , false);
		//		NSM_PLUG_DBG_S(string("client ifName =") + ifName);
			}	
			break;
		}	
		//NSM_PLUG_DBG("#####Done");
		}        
	}
}
