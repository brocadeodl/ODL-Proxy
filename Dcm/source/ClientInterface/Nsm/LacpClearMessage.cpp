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
 *   Author : Bhavin Patel                                                  *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/LacpClearMessage.h"
#include "ClientInterface/Nsm/lacp_dcm_def.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    LacpClearMessage::LacpClearMessage ()
        : DcmManagementInterfaceMessage (LACP_CLEARCOUNTER)
    {
		//DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LLDPCLEARMESSAGE,sizeof(LacpClearMessage),DCMMEMSTATINCREMENT);
    }

    LacpClearMessage::LacpClearMessage (const UI32 &PortchannelNumber,const UI32 &cmdCode,const UI32 &mappedId)
        : DcmManagementInterfaceMessage (LACP_CLEARCOUNTER),
        m_PortchannelNumber (PortchannelNumber),
        m_cmdCode (cmdCode),
        m_mappedId   (mappedId)
    {
		//DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LLDPCLEARMESSAGE,sizeof(LacpClearMessage),DCMMEMSTATINCREMENT);
    }

    LacpClearMessage::~LacpClearMessage ()
    {
		 //DcmMemUtil::memStatUpdate (DCM_MEM_TYPE_LLDP_LLDPCLEARMESSAGE,sizeof(LacpClearMessage),DCMMEMSTATDECREMENT);
    }

    void  LacpClearMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_PortchannelNumber,"PortchannelNumber"));
        addSerializableAttribute (new AttributeUI32(&m_mappedId,"mappedId"));
    }

    void  LacpClearMessage::setPortchannelNumber(const UI32 &PortchannelNumber)
    {
        m_PortchannelNumber  =  PortchannelNumber;
    }

    UI32  LacpClearMessage::getPortchannelNumber() const
    {
        return (m_PortchannelNumber);
    }

    void  LacpClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  LacpClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }
	
	void  LacpClearMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  LacpClearMessage::getMappedId() const
    {
        return (m_mappedId);
    }

	
    const void *LacpClearMessage::getCStructureForInputs ()
    {

        Lacp_Clearcounter_t *p_Input = new  Lacp_Clearcounter_t;
		
		trace (TRACE_LEVEL_DEBUG, "Entering LacpClearMessage::getCStructureForInputs\n");

	    p_Input->po_id = m_PortchannelNumber;
        p_Input->cmdCode = m_cmdCode;

        return (p_Input);
	}	
    
}

