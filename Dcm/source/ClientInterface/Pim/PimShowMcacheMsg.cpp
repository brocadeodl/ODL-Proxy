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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Rahul Jain                                                 *
 **************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimShowMcacheMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"


namespace DcmNs
{

    PimShowMcacheMsg::PimShowMcacheMsg ()
        : DcmManagementInterfaceMessage(PIM_SHOW_MCACHE)
    {
	}

    PimShowMcacheMsg::PimShowMcacheMsg (const UI8 &cmd, const UI8 &firstPg, 
        const IpV4Address &ipAddr1, const IpV4Address &ipAddr2,
		/* RJAIN */
        const UI32 &nxtSrc, const UI32 &nxtGrp)
        : DcmManagementInterfaceMessage (PIM_SHOW_MCACHE),
          m_cmd	(cmd), 
          m_firstPg	(firstPg), 
          m_ipAddr1 (ipAddr1), 
          m_ipAddr2 (ipAddr2),
          /* RJAIN */
          m_nxtSrc (nxtSrc),
          m_nxtGrp (nxtGrp)
    {
    }

    PimShowMcacheMsg::~PimShowMcacheMsg ()
    {
    }

    void  PimShowMcacheMsg::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_cmd, "cmd"));
        addSerializableAttribute (new AttributeUI8(&m_firstPg, "firstPg"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr1, "ipAddr1"));
        addSerializableAttribute (new AttributeIpV4Address(&m_ipAddr2, "ipAddr2"));
		/* RJAIN */
        addSerializableAttribute (new AttributeUI32(&m_nxtSrc, "nxtSrc"));
        addSerializableAttribute (new AttributeUI32(&m_nxtGrp, "nxtGrp"));
    }

    void  PimShowMcacheMsg::setCmdCode(const UI8 &cmd)
    {
        m_cmd  =  cmd;
    }

    UI8  PimShowMcacheMsg::getCmdCode() const
    {
        return (m_cmd);
    }

    void  PimShowMcacheMsg::setFirstPg(const UI8 &firstPg)
    {
        m_firstPg  =  firstPg;
    }

    UI8  PimShowMcacheMsg::getFirstPg() const
    {
        return (m_firstPg);
    }

    void  PimShowMcacheMsg::setIpAddr1(const IpV4Address &ipAddr)
    {
        m_ipAddr1 = ipAddr;
    }

    IpV4Address  PimShowMcacheMsg::getIpAddr1() const
    {
        return (m_ipAddr1);
    }

    void  PimShowMcacheMsg::setIpAddr2(const IpV4Address &ipAddr)
    {
        m_ipAddr2 = ipAddr;
    }

    IpV4Address  PimShowMcacheMsg::getIpAddr2() const
    {
        return (m_ipAddr2);
    }

	/* RJAIN */
    void  PimShowMcacheMsg::setNxtSrc(const UI32 &nxtSrc)
    {
        m_nxtSrc = nxtSrc;
    }

    UI32  PimShowMcacheMsg::getNxtSrc() const
    {
        return (m_nxtSrc);
    }

    void  PimShowMcacheMsg::setNxtGrp(const UI32 &nxtGrp)
    {
        m_nxtGrp = nxtGrp;
    }

    UI32  PimShowMcacheMsg::getNxtGrp() const
    {
        return (m_nxtGrp);
    }

    void PimShowMcacheMsg::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        trace(TRACE_LEVEL_DEBUG, 
            string("PimShowNeighborMessag:getOutputBuffer: Entered...."));
        pBuffer = findBuffer(bufferNum, size);
    }

    const void *PimShowMcacheMsg::getCStructureForInputs ()
    {
        string str_val;
        pim_show_in_mcache_msg  *msg = new pim_show_in_mcache_msg;

        /* Populate the message to be sent to the daemon */
        msg->cmd_code = m_cmd; //PIM_SHOW_MCACHE_CMD;
        msg->first_pg = m_firstPg;
        str_val = m_ipAddr1.toString();
        strncpy(msg->ip_addr1, str_val.c_str(), IPV4_ADDR_LEN);
        msg->ip_addr1[IPV4_ADDR_LEN-1] = '\0';
        str_val = m_ipAddr2.toString();
        strncpy(msg->ip_addr2, str_val.c_str(), IPV4_ADDR_LEN);
        msg->ip_addr2[IPV4_ADDR_LEN-1] = '\0';
		/* RJAIN */
        msg->nxt_src = m_nxtSrc;
        msg->nxt_grp = m_nxtGrp;

        return msg;
    }

	void 
    PimShowMcacheMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
		if(m_cmd == PIM_SHOW_MCACHE_CMD) {
            pim_show_out_mcache_msg *msg = ((pim_show_out_mcache_msg *)pOutputCStructure);			
            if (msg == 0) 
                return;

            addBuffer(PIM_SHOW_MCACHE, msg->size, pOutputCStructure, false);
        }
        return;
    }
}
