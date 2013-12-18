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
 *   Copyright (C) 2005-2010 Brocade Coreq_pmunications Systems, Inc.      *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/
#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ClientInterface/Lldp/CdpClientConfSetMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    CdpClientConfSetMessage::CdpClientConfSetMessage ()
        : DcmManagementInterfaceMessage (MSG_CDP_GLOBAL_CONF)
    {
    }

    CdpClientConfSetMessage::CdpClientConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const bool &protoEnable)
        : DcmManagementInterfaceMessage (MSG_CDP_GLOBAL_CONF),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_protoEnable    (protoEnable)
    {
    }

    CdpClientConfSetMessage::~CdpClientConfSetMessage ()
    {
    }

    void  CdpClientConfSetMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeBool(&m_protoEnable,"protoEnable"));
    }

    void  CdpClientConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  CdpClientConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  CdpClientConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  CdpClientConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  CdpClientConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  CdpClientConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  CdpClientConfSetMessage::setProtoEnable(const bool &protoEnable)
    {
        m_protoEnable  =  protoEnable;
    }

    bool  CdpClientConfSetMessage::getProtoEnable() const
    {
        return (m_protoEnable);
    }
	
	const void *CdpClientConfSetMessage::getCStructureForInputs ()
    {
        cdp_mgmt_conf_req_t *req_p = (cdp_mgmt_conf_req_t*)calloc(1, sizeof(cdp_mgmt_conf_req_t));
		
		/* Read the header from received message and populate 
		 * sending message
		 */
 		req_p->mgmt_flags	= m_mgmtFlags;
        req_p->cmd_code		= m_cmdcode;
        req_p->msg_type     = m_msgType;

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(m_cmdcode, CDP_GLOBAL_CONF_PROTO_STATUS))
            req_p->entry.global_conf.protoEnable	= m_protoEnable;

        return (req_p);
    }

	void CdpClientConfSetMessage::loadOutputsFromCStructure (
                                                const void *pOutputCStructure)
	{
	}
}
