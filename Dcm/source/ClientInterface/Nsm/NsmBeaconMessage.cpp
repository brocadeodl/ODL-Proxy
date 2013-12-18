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
 **************************************************************************/
#include "ClientInterface/Nsm/NsmBeaconMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#include "Beacon/Local/BeaconLocalObjectManager.h"
#include "Beacon/Local/BeaconServiceMessage.h"
#include "BeaconActTypes.h"


namespace DcmNs
{

   NsmBeaconMessage :: NsmBeaconMessage()
        : DcmManagementInterfaceMessage (NSMBEACONMSG)
    {
		 m_cmdId = 0;
    }

	
	
	NsmBeaconMessage :: NsmBeaconMessage(const UI32 cliCode,
	  const UI32 cmdId, const string &ifName, const string &msgStatus)
		: DcmManagementInterfaceMessage (NSMBEACONMSG),
		  m_cliCode (cliCode),	
		  m_cmdId	(cmdId),
		  m_if		(ifName),
		  m_msgStatus    (msgStatus)
    {
    }

    NsmBeaconMessage::~NsmBeaconMessage()
    {
    }

	

    void  NsmBeaconMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_cmdId,"cmdId"));
		addSerializableAttribute (new AttributeString(&m_if,"ifName"));
        addSerializableAttribute (new AttributeString(&m_msgStatus,"msgStatus"));
    }

	const void *NsmBeaconMessage::getCStructureForInputs ()
	{
		NsmBeaconMsg_t *pInput = new NsmBeaconMsg_t;
		strncpy (pInput->ifName, m_if.c_str (), 64);
		pInput->cmdId = m_cmdId;
		pInput->cliCode = m_cliCode;
		return (pInput);
	}

	void NsmBeaconMessage::getDuplicateInterfaceOutputBuffer (UI32 bufferNum,
                          UI32 &size, void *&pBuffer)
	{
        pBuffer   =   findBuffer(bufferNum, size);
	}

	void NsmBeaconMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	   addBuffer (NSMBEACONMSG, sizeof(NsmBeaconMsgOut_t),
	       pOutputCStructure, false);
	}


    void  NsmBeaconMessage::setIf(const string &ifName)
    {
        m_if  =  ifName;
    }

	string  NsmBeaconMessage::getIf() const
    {
        return (m_if);
    }


	void  NsmBeaconMessage::setCmdId(const UI32 cmdId)
    {
        m_cmdId =  cmdId;
    }

    UI32  NsmBeaconMessage::getCmdId() const
    {
        return (m_cmdId);
    }

    void  NsmBeaconMessage::setCliCode(const UI32 cliCode)
    {
        m_cliCode =  cliCode;
    }

    UI32  NsmBeaconMessage::getCliCode() const
    {
        return (m_cliCode);
    }

    void  NsmBeaconMessage::setMsgStatus(const string &msgStatus)
    {
        m_msgStatus  =  msgStatus;
    }

    string  NsmBeaconMessage::getMsgStatus() const
    {
        return (m_msgStatus);
    }
}
