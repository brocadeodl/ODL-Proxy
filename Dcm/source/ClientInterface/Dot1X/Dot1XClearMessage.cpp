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
 *   Author : Adarsh
 **************************************************************************/

#include "ClientInterface/Dot1X/Dot1XClearMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"

namespace DcmNs
{

    Dot1XClearMessage::Dot1XClearMessage ()
        : DcmManagementInterfaceMessage (DOT1X_CLEAR_ALL)
    {
    }

    Dot1XClearMessage::Dot1XClearMessage (const string &Interface, const string &ifName, const UI32 &cmdCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (DOT1X_CLEAR_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
		m_mappedId  (mappedId)
    {
    }

    Dot1XClearMessage::~Dot1XClearMessage ()
    {
    }

    void  Dot1XClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
    	addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
	}

    void  Dot1XClearMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XClearMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XClearMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XClearMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  Dot1XClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  Dot1XClearMessage::setMappedId(const UI32 &mappedId)
    {
      m_mappedId  =  mappedId;
    }

    UI32  Dot1XClearMessage::getMappedId() const
    {
      return (m_mappedId);
    }

	void Dot1XClearMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *Dot1XClearMessage::getCStructureForInputs ()
    {
        Dot1XShow_InputMsg_t *pInput = new Dot1XShow_InputMsg_t;

        strncpy (pInput->if_name, m_ifName.c_str (), 64);
		pInput->if_name[63] = '\0';
        strncpy (pInput->interface , m_interface.c_str (), 64);
		pInput->interface[63] = '\0';
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void Dot1XClearMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
        Dot1X_show_msg_t *msg = ((Dot1X_show_msg_t *)pOutputCStructure);
		if (msg == 0) {
		  return;
		}
		
		return;
	}
}
