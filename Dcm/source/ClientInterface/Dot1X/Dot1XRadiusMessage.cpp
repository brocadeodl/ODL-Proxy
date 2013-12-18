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
 *   Author : Srikanth
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Dot1X/Dot1XRadiusMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs
{

    Dot1XRadiusMessage::Dot1XRadiusMessage ()
        : DcmManagementInterfaceMessage (DOT1X_RADIUS_MSG)
    {
    }

    Dot1XRadiusMessage::Dot1XRadiusMessage (const string &host,const UI16 &port,const UI32 &protocol,const string &key,const UI32 &retransmit, const UI32 &timeout, const UI32 &cmdCode)
        : DcmManagementInterfaceMessage (DOT1X_RADIUS_MSG),
		 m_host (host),
	    m_port (port),
	    m_protocol (protocol),
	    m_key (key),
	    m_retransmit (retransmit),
	    m_timeout (timeout),
        m_cmdCode (cmdCode)
    {
    }

    Dot1XRadiusMessage::~Dot1XRadiusMessage ()
    {
    }

    void  Dot1XRadiusMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeString(&m_host,"host"));
    	addSerializableAttribute (new AttributeUI16(&m_port,"port"));
	    addSerializableAttribute (new AttributeUI32(&m_protocol,"protocol"));
	    addSerializableAttribute (new AttributeString(&m_key,"key"));
	    addSerializableAttribute (new AttributeUI32(&m_retransmit,"retransmit"));
	    addSerializableAttribute (new AttributeUI32(&m_timeout,"timeout"));
    }


void  Dot1XRadiusMessage::setCmdCode(const UI32 &cmdCode)
{   
    m_cmdCode  =  cmdCode;
}

UI32  Dot1XRadiusMessage::getCmdCode() const
{
   return (m_cmdCode);
}



void Dot1XRadiusMessage::setHost(const string &host)
{
    m_host  =  host;
}

string Dot1XRadiusMessage::getHost() const
{
    return (m_host);
}

void Dot1XRadiusMessage::setPort(const UI16 &port)
{
    m_port  =  port;
}

UI16 Dot1XRadiusMessage::getPort() const
{
    return (m_port);
}

void Dot1XRadiusMessage::setProtocol(const UI32 &protocol)
{
    m_protocol  =  protocol;
}

UI32 Dot1XRadiusMessage::getProtocol() const
{
    return (m_protocol);
}

void Dot1XRadiusMessage::setKey(const string &key)
{
    m_key  =  key;
}

string Dot1XRadiusMessage::getKey() const
{
    return (m_key);
}

void  Dot1XRadiusMessage::setRetransmit(const UI32 &retransmit)
{
    m_retransmit  =  retransmit;
}

UI32  Dot1XRadiusMessage::getRetransmit() const
{
    return (m_retransmit);
}

void  Dot1XRadiusMessage::setTimeout(const UI32 &timeout)
{
    m_timeout  =  timeout;
}

UI32  Dot1XRadiusMessage::getTimeout() const
{
    return (m_timeout);
}


      void Dot1XRadiusMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
      {
          pBuffer = findBuffer(bufferNum, size);
      }


    void Dot1XRadiusMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
													UI32 &size, void *&pBuffer)
    {
            pBuffer   =   findBuffer(bufferNum, size);
    }
    
    const void *Dot1XRadiusMessage::getCStructureForInputs ()
    {
        Dot1XRadius_InputMsg_t *pInput = new Dot1XRadius_InputMsg_t;
		trace (TRACE_LEVEL_DEBUG, "Entering Dot1XRadiusMessage:: getCStructureForInputs:- " + m_cmdCode);

        strncpy (pInput->hostname, m_host.c_str (), (MAX_IF_NAME_LEN+4));
		pInput->hostname[MAX_IF_NAME_LEN+3] = '\0';
        strncpy (pInput->secret, m_key.c_str (), (MAX_IF_NAME_LEN+4));
		pInput->secret[MAX_IF_NAME_LEN+3] = '\0';
		pInput->timeout = m_timeout;
		pInput->retries = m_retransmit;
		pInput->port = m_port;
		pInput->proto = m_protocol;
        pInput->cmdCode = m_cmdCode;
		
        return (pInput);
    }

	void Dot1XRadiusMessage::loadOutputsFromCStructure (const void 
														*pOutputCStructure)
	{
	//	trace (TRACE_LEVEL_DEBUG, "Entering Dot1XRadiusMessage:: loadOutputsFromCStructure\n");
	//	  trace (TRACE_LEVEL_DEBUG, "Entering Dot1XRadiusMessage:: loadOutputsFromCStructure:- Message NULL");
		  return;
	}
}



