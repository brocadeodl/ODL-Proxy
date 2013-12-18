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
 *   Author : Rahul Jain                                                   *
 ***************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimStaticRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimStaticRpCfgClientMsg::PimStaticRpCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_CFG_STATIC_RP)
{
}

PimStaticRpCfgClientMsg::PimStaticRpCfgClientMsg(const UI8 &msgType,
    const IpV4Address &rpAddr, const string &prefixList)
    : DcmManagementInterfaceMessage(PIM_CFG_STATIC_RP),
      m_msgType (msgType),
      m_rpAddr (rpAddr),
      m_prefixList (prefixList)
{
}

PimStaticRpCfgClientMsg::~PimStaticRpCfgClientMsg()
{
}

void
PimStaticRpCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
    addSerializableAttribute(new AttributeUI8(&m_msgType,"MsgType"));
    addSerializableAttribute(new AttributeIpV4Address(&m_rpAddr,"RpAddr"));
    addSerializableAttribute(new AttributeString(&m_prefixList,"PrefixList"));
}

const void *
PimStaticRpCfgClientMsg::getCStructureForInputs ()
{
    string str_val;
    pim_static_rp_msg_t *msg = NULL;
    
    trace(TRACE_LEVEL_DEBUG, string("PimStaticRpCfgClientMsg::getCStructureForInputs:"
        " Entered...."));

    /* Allocate memory for the msg buffer */
    msg = (pim_static_rp_msg_t *)calloc(1, sizeof(pim_static_rp_msg_t));
    if(!msg) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory for msg"); 
        return NULL;
    }
    
    /* Now copy the data members into the structure variables */
    msg->msg_type = m_msgType;
    str_val = m_rpAddr.toString();
    strncpy(msg->rp_addr, str_val.c_str(), IPV4_ADDR_LEN);
    msg->rp_addr[IPV4_ADDR_LEN-1] = '\0';
    strncpy((char *)(msg->prefix_list), (const char *)(m_prefixList.c_str()), 
        MAX_PREFIX_LIST_LEN);
    msg->prefix_list[MAX_PREFIX_LIST_LEN-1] = '\0';
    
    return msg;
}

void
PimStaticRpCfgClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void 
PimStaticRpCfgClientMsg::setMsgType(const UI8 &msgType)
{
    m_msgType  =  msgType;
}
    
void
PimStaticRpCfgClientMsg::setRpAddr(const IpV4Address &rpAddr)
{
    m_rpAddr = rpAddr;
}

void
PimStaticRpCfgClientMsg::setPrefixList(const string &prefixList)
{
    m_prefixList = prefixList;
}

/* Accessor Function Definitions */
UI8  
PimStaticRpCfgClientMsg::getMsgType() const
{
    return (m_msgType);
}

IpV4Address
PimStaticRpCfgClientMsg::getRpAddr() const
{
    return m_rpAddr;
}

string
PimStaticRpCfgClientMsg::getPrefixList() const
{
    return m_prefixList;
}

} /* namespace */

