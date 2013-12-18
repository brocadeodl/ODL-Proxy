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
#include "ClientInterface/Pim/PimRtrCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimRtrCfgClientMsg::PimRtrCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_CFG_RTR_PIM)
{
}

PimRtrCfgClientMsg::PimRtrCfgClientMsg(const UI32 msgType)
    : DcmManagementInterfaceMessage(msgType)
{
}

PimRtrCfgClientMsg::~PimRtrCfgClientMsg()
{
}

void
PimRtrCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    addSerializableAttribute(new AttributeUI8(&m_msgType,"MsgType"));
	addSerializableAttribute(new AttributeUI8(&m_cmdCode,"cmdCode"));
    addSerializableAttribute(new AttributeString(&m_vrfId,"VrfId"));
    addSerializableAttribute(new AttributeUI32(&m_maxMcache,"MaxMcache"));
    addSerializableAttribute(new AttributeSI16(&m_helloTimer,"HelloTimer"));
    addSerializableAttribute(new AttributeSI16(&m_nbrTo,"NbrTo"));
    addSerializableAttribute(new AttributeSI16(&m_inactTimer,"InactTimer"));
    addSerializableAttribute(new AttributeSI32(&m_msgIntv,"MsgIntv"));
    addSerializableAttribute(new AttributeUI32(&m_sptThold,"SptThold"));
}

const void *
PimRtrCfgClientMsg::getCStructureForInputs ()
{
    trace(TRACE_LEVEL_DEBUG, string("PimRtrCfgClientMsg::getCStructureForInputs:"
        " Entered...."));

	pim_rtr_cfg_msg_t *msg = new pim_rtr_cfg_msg_t;
	
    /* Now copy the data members into the structure variables */
#if 0
    vrf_str_len = strlen(this.getVrfId());
    strncpy(msg->vrf_id, this.getVrfId(), vrf_str_len);
    if(!msg->vrf_id) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory vrf_id");
        return NULL;
    }
#endif /* if 0 */
    msg->msg_type = m_msgType;
    msg->cmd_code = m_cmdCode;
    msg->max_mcache = m_maxMcache;
    msg->hello_timer = m_helloTimer;
    msg->nbr_to = m_nbrTo;
    msg->inact_timer = m_inactTimer;
    msg->msg_intv = m_msgIntv;
	msg->spt_thold = m_sptThold;
    
    trace(TRACE_LEVEL_DEBUG, string("msgType: ") + toString(m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("cmdCode: ") + toString(m_cmdCode));
    trace(TRACE_LEVEL_DEBUG, string("mcache: ") + toString(m_maxMcache));
    trace(TRACE_LEVEL_DEBUG, string("hello_timer: ") + toString(m_helloTimer));
    trace(TRACE_LEVEL_DEBUG, string("nbr_to: ") + toString(m_nbrTo));
    trace(TRACE_LEVEL_DEBUG, string("inact: ") + toString(m_inactTimer));
    trace(TRACE_LEVEL_DEBUG, string("msg_intv: ") + toString(m_msgIntv));
    trace(TRACE_LEVEL_DEBUG, string("spt_thold: ") + toString(m_sptThold));

    return msg;
}

void
PimRtrCfgClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void
PimRtrCfgClientMsg::setVrfId(const string &vrfId)
{
    m_vrfId = vrfId;
}

void 
PimRtrCfgClientMsg::setMsgType(const UI8 &msgType)
{
    m_msgType  =  msgType;
}

void
PimRtrCfgClientMsg::setCmdCode(const UI8 &cmdCode)
{
    m_cmdCode = cmdCode;
}

void
PimRtrCfgClientMsg::setEnable(const bool &enable)
{
    m_enable = enable;
}

void
PimRtrCfgClientMsg::setMaxMcache(const UI32 &maxMcache)
{
    m_maxMcache = maxMcache;
}

void
PimRtrCfgClientMsg::setHelloTimer(const SI16 &helloTimer)
{
    m_helloTimer = helloTimer;
}

void
PimRtrCfgClientMsg::setNbrTo(const SI16 &nbrTo)
{
    m_nbrTo = nbrTo;
}

void
PimRtrCfgClientMsg::setInactTimer(const SI16 &inactTimer)
{
    m_inactTimer = inactTimer;
}

void
PimRtrCfgClientMsg::setMsgIntv(const SI32 &msgIntv)
{
    m_msgIntv = msgIntv;
}

void
PimRtrCfgClientMsg::setSptThold(const UI32 &sptThold)
{
    m_sptThold = sptThold;
}

/* Accessor Function Definitions */
UI8  
PimRtrCfgClientMsg::getMsgType() const
{
    return m_msgType;
}

UI8
PimRtrCfgClientMsg::getCmdCode() const
{
    return m_cmdCode;
}

bool
PimRtrCfgClientMsg::getEnable() const
{
    return m_enable;
}

string
PimRtrCfgClientMsg::getVrfId() const
{
    return m_vrfId;
}

UI32
PimRtrCfgClientMsg::getMaxMcache() const
{
    return m_maxMcache;
}

SI16
PimRtrCfgClientMsg::getHelloTimer() const
{
    return m_helloTimer;
}

SI16
PimRtrCfgClientMsg::getNbrTo() const
{
    return m_nbrTo;
}

SI16
PimRtrCfgClientMsg::getInactTimer() const
{
    return m_inactTimer;
}

SI32
PimRtrCfgClientMsg::getMsgIntv() const
{
    return m_msgIntv;
}

UI32
PimRtrCfgClientMsg::getSptThold() const
{
    return m_sptThold;
}

} /* namespace */
