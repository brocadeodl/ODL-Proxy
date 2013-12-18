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

#include "ClientInterface/Dot1X/Dot1XDebugCommand.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"




namespace DcmNs
{

    Dot1XDebugMessage::Dot1XDebugMessage ()
        : DcmManagementInterfaceMessage (DOT1X_DEBUG_ALL)
    {
    }

    Dot1XDebugMessage::Dot1XDebugMessage (const string &Interface, const string &ifName,const UI32 &cmdType, const UI32 &cmdCode, const UI32 &subCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (DOT1X_DEBUG_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdType   (cmdType),
        m_cmdCode   (cmdCode),
        m_subCode   (subCode),
		m_mappedId    (mappedId)                                
    {
    }

    Dot1XDebugMessage::~Dot1XDebugMessage ()
    {
    }

    void  Dot1XDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdType,"cmdType"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_subCode,"subCode"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  Dot1XDebugMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XDebugMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XDebugMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XDebugMessage::getIfName() const
    {
        return (m_ifName);
    }
    

    void  Dot1XDebugMessage::setCmdType(const UI32 &cmdType)
    {
        m_cmdType  =  cmdType;
    }

    UI32  Dot1XDebugMessage::getCmdType() const
    {
        return (m_cmdType);
    }


    void  Dot1XDebugMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XDebugMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  Dot1XDebugMessage::setSubCode(const UI32 &subCode)
    {
        m_subCode  =  subCode;
    }

    UI32  Dot1XDebugMessage::getSubCode() const
    {
        return (m_subCode);
    }

	void  Dot1XDebugMessage::setMappedId(const UI32 &mappedId)
	{
	  m_mappedId  =  mappedId;
	}

	UI32  Dot1XDebugMessage::getMappedId() const
	{
	  return (m_mappedId);
	}

    void Dot1XDebugMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
	}
    
    const void *Dot1XDebugMessage::getCStructureForInputs ()
    {
        Dot1XDebug_InputMsg_t *pInput = new Dot1XDebug_InputMsg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
		pInput->if_name[63] = '\0';
        strncpy (pInput->interface , m_interface.c_str (), 64);
		pInput->interface[63] = '\0';
        pInput->cmdType     = m_cmdType;
        pInput->cmdCode     = m_cmdCode;
        pInput->subCode     = m_subCode;
        return (pInput);
    }

	void Dot1XDebugMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
	    dot1x_mgmt_buffer_num_t  buff_num;
		memset(&buff_num, 0, sizeof(buff_num));
		buff_num.buff.mapped_id = this->getMappedId();
		buff_num.buff.tag = DOT1X_DEBUG_ALL;
		
	//	trace(TRACE_LEVEL_INFO, string("Dot1XDebugMessage::loadOutputsFromCStructure"));
		if(pOutputCStructure)
		{
		addBuffer (buff_num.num, (1024), ( ((unsigned char *)pOutputCStructure )) , false);
		}
		
	}
}
