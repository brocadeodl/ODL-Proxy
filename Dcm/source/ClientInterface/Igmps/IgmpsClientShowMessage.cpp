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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#include "ClientInterface/Igmps/IgmpsClientShowMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    IgmpsClientShowMessage::IgmpsClientShowMessage()
        : DcmManagementInterfaceMessage (IGMPSCLIENTSHOW),
        m_cmdCode   (IGMPS_DEFAULT_CMD_CODE),
        m_mappedId  (0)
    {
    }

    IgmpsClientShowMessage::IgmpsClientShowMessage(const string &grpName, const string &ifName, const UI32 &cmdCode, const UI32 &mappedId,
	                                               const bool &bIsMgmtClusterEnabled)
        : DcmManagementInterfaceMessage (IGMPSCLIENTSHOW),
        m_grpName   (grpName),
        m_ifName    (ifName),
        m_cmdCode    (cmdCode),
        m_mappedId   (mappedId),
		m_bIsMgmtClusterEnabled(bIsMgmtClusterEnabled)
    {
    }

    IgmpsClientShowMessage::~IgmpsClientShowMessage ()
    {
    }

    void  IgmpsClientShowMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_grpName,"grpName"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32 (&m_mappedId,"mappedId"));
        addSerializableAttribute (new AttributeBool (&m_bIsMgmtClusterEnabled,"mgmtClusterEnabled"));
    }

    void  IgmpsClientShowMessage::setGrpName(const string &grpName)
    {
        m_grpName  =  grpName;
    }

    string  IgmpsClientShowMessage::getGrpName() const
    {
        return (m_grpName);
    }
    
    void  IgmpsClientShowMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IgmpsClientShowMessage::getIfName() const
    {
        return (m_ifName);
    }
    
    void  IgmpsClientShowMessage::setMgmtClusterEnabled (const bool &bIsMgmtClusterEnabled)
    {
		m_bIsMgmtClusterEnabled  =  bIsMgmtClusterEnabled;
    }

	bool  IgmpsClientShowMessage::getMgmtClusterEnabled() const
    {
		return (m_bIsMgmtClusterEnabled);
	}

    void  IgmpsClientShowMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  IgmpsClientShowMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  IgmpsClientShowMessage::setMappedId(const UI32 &mappedId)
    {
        m_mappedId  =  mappedId;
    }

    UI32  IgmpsClientShowMessage::getMappedId() const
    {
        return (m_mappedId);
    }

    void IgmpsClientShowMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
    
    const void *IgmpsClientShowMessage::getCStructureForInputs ()
    {
        igmps_show_input_msg_t *pInput = (igmps_show_input_msg_t *) malloc (sizeof (igmps_show_input_msg_t));

        trace (TRACE_LEVEL_DEBUG, "Entering IgmpsClientShowMessage::getCStructureForInputs\n");
        strncpy (pInput->grp_name, m_grpName.c_str (), IGMPS_MAX_STR);
        strncpy (pInput->if_name, m_ifName.c_str (), IGMPS_MAX_STR);
        pInput->cmdCode = m_cmdCode;
		pInput->bIsMgmtClusterEnabled = m_bIsMgmtClusterEnabled;
        return (pInput);
    }

	void IgmpsClientShowMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		igmps_show_hdr_t *hdr;

        trace (TRACE_LEVEL_DEBUG, "Entering IgmpsClientShowMessage::loadOutputsFromCStructure\n");

        if (!pOutputCStructure) return;

        igmp_mgmt_buffer_num_t  buff_num;

        memset(&buff_num, 0, sizeof(buff_num));
        buff_num.buff.mapped_id = this->getMappedId();
        buff_num.buff.tag = IGMPSCLIENTSHOW;

		hdr = (igmps_show_hdr_t*) pOutputCStructure;

       	addBuffer (this->getMappedId(), hdr->buflen, pOutputCStructure, false);
	}
}

