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
 *   Author : Chirag Taunk                                                 *
 ***************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimCandRpPrefixCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimCandRpPrefixCfgClientMsg::PimCandRpPrefixCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_CFG_CAND_RP_PREFIX)
{
}

PimCandRpPrefixCfgClientMsg::PimCandRpPrefixCfgClientMsg(const UI8 &msgType,
    const UI8 &prefixAction, const IpV4Address &prefixIP, const UI16 
	&prefixLen)
    : DcmManagementInterfaceMessage(PIM_CFG_CAND_RP_PREFIX),
      m_msgType (msgType),
      m_prefixIP (prefixIP),
      m_prefixLen (prefixLen)
{
}

PimCandRpPrefixCfgClientMsg::~PimCandRpPrefixCfgClientMsg()
{
}

void
PimCandRpPrefixCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
    addSerializableAttribute(new AttributeUI8(&m_msgType,"MsgType"));
	addSerializableAttribute(new AttributeUI8(&m_prefixAction,
		"prefixAction"));
    addSerializableAttribute(new AttributeIpV4Address(&m_prefixIP,
		"PrefixIP"));
    addSerializableAttribute(new AttributeUI16(&m_prefixLen,"PrefixLen"));
}

const void *
PimCandRpPrefixCfgClientMsg::getCStructureForInputs ()
{
    string str_val;
    pim_cand_rp_prefix_msg_t *msg = NULL;
    
    trace(TRACE_LEVEL_DEBUG, 
		string("PimCandRpPrefixCfgClientMsg::getCStructureForInputs:")+
        string(" Entered...."));

    /* Allocate memory for the msg buffer */
    msg = (pim_cand_rp_prefix_msg_t *)calloc(1, 
		sizeof(pim_cand_rp_prefix_msg_t));
    if(!msg) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory for msg"); 
        return NULL;
    }
    
    /* Now copy the data members into the structure variables */
	trace(TRACE_LEVEL_DEBUG,
        string("PimCandRpPrefixCfgClientMsg::getCStructureForInputs:")+
        string(" prefixAction = ") + toString(m_prefixAction));
	trace(TRACE_LEVEL_DEBUG,
        string("PimCandRpPrefixCfgClientMsg::getCStructureForInputs:")+
        string(" prefixIP = ") + m_prefixIP.toString());
	trace(TRACE_LEVEL_DEBUG,
        string("PimCandRpPrefixCfgClientMsg::getCStructureForInputs:")+
        string(" prefixLen = ") + toString(m_prefixLen));
    msg->msg_type = m_msgType;
	msg->prefixAction = m_prefixAction;
    str_val = m_prefixIP.toString();
    strncpy(msg->prefixIP, str_val.c_str(), IPV4_ADDR_LEN);
    msg->prefixIP[IPV4_ADDR_LEN-1] = '\0';
    msg->prefixLen = m_prefixLen;
    
    return msg;
}

void
PimCandRpPrefixCfgClientMsg::loadOutputsFromCStructure (const void 
	*pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void 
PimCandRpPrefixCfgClientMsg::setMsgType(const UI8 &msgType)
{
    m_msgType  =  msgType;
}

void
PimCandRpPrefixCfgClientMsg::setAction(const UI8 &prefixAction)
{
    m_prefixAction  =  prefixAction;
}
	
void
PimCandRpPrefixCfgClientMsg::setPrefixIP(const IpV4Address &prefixIP)
{
    m_prefixIP = prefixIP;
}

void
PimCandRpPrefixCfgClientMsg::setPrefixLen(const UI16 &prefixLen)
{
    m_prefixLen = prefixLen;
}

/* Accessor Function Definitions */
UI8  
PimCandRpPrefixCfgClientMsg::getMsgType() const
{
    return (m_msgType);
}

UI8
PimCandRpPrefixCfgClientMsg::getAction() const
{
    return (m_prefixAction);
}

IpV4Address
PimCandRpPrefixCfgClientMsg::getPrefixIP() const
{
    return m_prefixIP;
}

UI16
PimCandRpPrefixCfgClientMsg::getPrefixLen() const
{
    return m_prefixLen;
}

} /* namespace */

