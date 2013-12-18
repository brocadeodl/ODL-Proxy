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
 *   Author : bvreddy
 **************************************************************************/

#include "ClientInterface/Sflow/SflowDebugMessage.h"
#include "ClientInterface/Sflow/SflowMessageDef.h"

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "WyserEaGateway/GenericClientMessage.h"




namespace DcmNs
{

    SflowDebugMessage::SflowDebugMessage ()
        : DcmManagementInterfaceMessage (SFLOWDEBUGMESSAGE)
    {
    }

    SflowDebugMessage::SflowDebugMessage (const string &Interface, const string &ifName,const UI32 &cmdType, const UI32 &cmdCode, const UI32 &subCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (SFLOWDEBUGMESSAGE),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdType   (cmdType),
        m_cmdCode   (cmdCode),
        m_subCode   (subCode),
		m_mappedId    (mappedId)                                
    {
    }

    SflowDebugMessage::~SflowDebugMessage ()
    {
    }

    void  SflowDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdType,"cmdType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_subCode,"subCode"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  SflowDebugMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  SflowDebugMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  SflowDebugMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  SflowDebugMessage::getIfName() const
    {
        return (m_ifName);
    }
    

    void  SflowDebugMessage::setCmdType(const UI32 &cmdType)
    {
        m_cmdType  =  cmdType;
    }

    UI32  SflowDebugMessage::getCmdType() const
    {
        return (m_cmdType);
    }


    void  SflowDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  SflowDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  SflowDebugMessage::setSubCode(const UI32 &subCode)
    {
        m_subCode  =  subCode;
    }

    UI32  SflowDebugMessage::getSubCode() const
    {
        return (m_subCode);
    }

	void  SflowDebugMessage::setMappedId(const UI32 &mappedId)
	{
	  m_mappedId  =  mappedId;
	}

	UI32  SflowDebugMessage::getMappedId() const
	{
	  return (m_mappedId);
	}

	void  SflowDebugMessage::setBpId(const UI32 &bpId)
	{
	  m_bpId  =  bpId;
	}

    UI32  SflowDebugMessage::getBpId() const
    {
        return (m_subCode);
    }


    void SflowDebugMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
	}
    
    const void *SflowDebugMessage::getCStructureForInputs ()
    {
        sflow_debug_input_msg_t *pInput = new sflow_debug_input_msg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
		pInput->if_name[63] = '\0';
        strncpy (pInput->interface , m_interface.c_str (), 64);
		pInput->interface[63] = '\0';
        pInput->cmdType     = m_cmdType;
        pInput->cmdCode     = m_cmdCode;
        pInput->subCode     = m_subCode;
        pInput->bpId     = m_subCode;
        return (pInput);
    }

	void SflowDebugMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
    	DcmNs::GenericClientWriterLoad
    	((generic_wyser_msg_reply_t*)pOutputCStructure, *this);
    	return;
#if 0
	    SflowBufferNum_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));
		buff_num.buff.mappedId = this->getMappedId();
		buff_num.buff.tag = SFLOWDEBUGMESSAGE;
		
	//	trace(TRACE_LEVEL_INFO, string("SflowDebugMessage::loadOutputsFromCStructure"));
		if(pOutputCStructure)
		{
		addBuffer (buff_num.num, (1024), ( ((unsigned char *)pOutputCStructure )) , false);
		}
#endif
		
	}
}
