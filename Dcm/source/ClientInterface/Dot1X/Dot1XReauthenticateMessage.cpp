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

/************************************************:***************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Sudha Balodia
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Dot1X/Dot1XReauthenticateMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    Dot1XReauthenticateMessage::Dot1XReauthenticateMessage ()
        : DcmManagementInterfaceMessage (DOT1X_REAUTHENTICATE_ALL)
    {
    }

    Dot1XReauthenticateMessage::Dot1XReauthenticateMessage (const string &Interface, const string &ifName, const UI32 &cmdCode, const UI32 &mappedId)
        : DcmManagementInterfaceMessage (DOT1X_REAUTHENTICATE_ALL),
        m_interface (Interface),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
		m_mappedId    (mappedId)                                
    {
    }

    Dot1XReauthenticateMessage::~Dot1XReauthenticateMessage ()
    {
    }

    void  Dot1XReauthenticateMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeString(&m_interface,"Interface"));
		addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  Dot1XReauthenticateMessage::setInterface(const string &Interface)
    {
        m_interface  =  Interface ;
    }

    string  Dot1XReauthenticateMessage::getInterface() const
    {
        return (m_interface);
    }
    
    void  Dot1XReauthenticateMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  Dot1XReauthenticateMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  Dot1XReauthenticateMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  Dot1XReauthenticateMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  Dot1XReauthenticateMessage::setMappedId(const UI32 &mappedId)
	{
	  m_mappedId  =  mappedId;
	}

	UI32  Dot1XReauthenticateMessage::getMappedId() const
	{
	  return (m_mappedId);
	}

      void Dot1XReauthenticateMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
      {
          pBuffer = findBuffer(bufferNum, size);
      }


    void Dot1XReauthenticateMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *Dot1XReauthenticateMessage::getCStructureForInputs ()
    {
        Dot1XReauthenticate_InputMsg_t *pInput = new Dot1XReauthenticate_InputMsg_t;
		trace (TRACE_LEVEL_DEBUG, "Entering Dot1XReauthenticateMessage:: getCStructureForInputs:- " + m_cmdCode);

        strncpy (pInput->if_name, m_ifName.c_str (), (MAX_IF_NAME_LEN+4));
		pInput->if_name[MAX_IF_NAME_LEN+3] = '\0';
        strncpy (pInput->interface , m_interface.c_str (), (MAX_IF_NAME_LEN+4));
		pInput->interface[MAX_IF_NAME_LEN+3] = '\0';
        pInput->cmdCode     = m_cmdCode;

        return (pInput);
    }

	void Dot1XReauthenticateMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
	//	trace (TRACE_LEVEL_DEBUG, "Entering Dot1XReauthenticateMessage:: loadOutputsFromCStructure\n");
	//	  trace (TRACE_LEVEL_DEBUG, "Entering Dot1XReauthenticateMessage:: loadOutputsFromCStructure:- Message NULL");
		  return;
	}
}
