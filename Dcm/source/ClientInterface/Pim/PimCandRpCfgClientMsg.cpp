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
#include "ClientInterface/Pim/PimCandRpCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimCandRpCfgClientMsg::PimCandRpCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_CFG_CAND_RP)
{
}

PimCandRpCfgClientMsg::PimCandRpCfgClientMsg(const UI8 &msgType, 
    const UI8 &intfType, const string &intfName, const string &prefixList)
    : DcmManagementInterfaceMessage(PIM_CFG_CAND_RP),
      m_msgType (msgType),
      m_intfType (intfType),
      m_intfName (intfName),
      m_prefixList (prefixList)
{
}

PimCandRpCfgClientMsg::~PimCandRpCfgClientMsg()
{
}

void
PimCandRpCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
    addSerializableAttribute(new AttributeUI8(&m_msgType,"MsgType"));
    addSerializableAttribute(new AttributeUI8(&m_intfType,"IntfType"));
    addSerializableAttribute(new AttributeString(&m_intfName,"IntfName"));
    addSerializableAttribute(new AttributeString(&m_prefixList,"PrefixList"));
}

const void *
PimCandRpCfgClientMsg::getCStructureForInputs ()
{
    string str_val;
    pim_cand_rp_cfg_msg_t *msg = NULL;
    
    trace(TRACE_LEVEL_DEBUG, string("PimCandRpCfgClientMsg::getCStructureForInputs:"
        " Entered...."));

    /* Allocate memory for the msg buffer */
    msg = (pim_cand_rp_cfg_msg_t *)calloc(1, sizeof(pim_cand_rp_cfg_msg_t));
    if(!msg) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory for msg"); 
        return NULL;
    }
    
    /* Now copy the data members into the structure variables */
    msg->msg_type = m_msgType;
    msg->intf_type = m_intfType;
    strncpy(msg->intf_name, m_intfName.c_str(), PIM_INTF_NAME_LEN_MAX);
    msg->intf_name[PIM_INTF_NAME_LEN_MAX-1] = '\0';
    strncpy((char *)(msg->prefix_list), (const char *)(m_prefixList.c_str()), 
        MAX_PREFIX_LIST_LEN);
    msg->prefix_list[MAX_PREFIX_LIST_LEN-1] = '\0';
    
    trace(TRACE_LEVEL_DEBUG, string("msg_type: ") + toString(m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(m_intfName));
    trace(TRACE_LEVEL_DEBUG, string("prefix_list: ") + m_prefixList);
     
    return msg;
}

void
PimCandRpCfgClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void 
PimCandRpCfgClientMsg::setMsgType(const UI8 &msgType)
{
    m_msgType  =  msgType;
}

void 
PimCandRpCfgClientMsg::setIntfType(const UI8 &intfType)
{
    m_intfType  =  intfType;
}

void
PimCandRpCfgClientMsg::setIntfName(const string &intfName)
{
    m_intfName = intfName;
}

void
PimCandRpCfgClientMsg::setPrefixList(const string &prefixList)
{
    m_prefixList = prefixList;
}

/* Accessor Function Definitions */
UI8  
PimCandRpCfgClientMsg::getMsgType() const
{
    return m_msgType;
}

UI8  
PimCandRpCfgClientMsg::getIntfType() const
{
    return m_intfType;
}

string
PimCandRpCfgClientMsg::getIntfName() const
{
    return m_intfName;
}

string
PimCandRpCfgClientMsg::getPrefixList() const
{
    return m_prefixList;
}

} /* namespace */

