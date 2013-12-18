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
 *   Author : Sree Shankar                                                 *
 ***************************************************************************/

#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Pim/PimBsrCandClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimBsrCandClientMsg::PimBsrCandClientMsg()
    : DcmManagementInterfaceMessage(PIM_BSR_CANDIDATE)
{
}

PimBsrCandClientMsg::PimBsrCandClientMsg(const UI8 &msgType,
    const UI8 &intfType, const string &intfName,const UI32  &hashMask,
    const UI32  &priority)
    : DcmManagementInterfaceMessage(PIM_BSR_CANDIDATE),
      m_msgType (msgType),
      m_intfType (intfType),
      m_intfName (intfName),
	  m_hashMask (hashMask),
      m_priority (priority)
{
}

PimBsrCandClientMsg::~PimBsrCandClientMsg()
{
}

void
PimBsrCandClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
	addSerializableAttribute(new AttributeUI8(&m_msgType,"MsgType"));
    addSerializableAttribute(new AttributeUI8(&m_intfType,"IntfType"));
    addSerializableAttribute(new AttributeString(&m_intfName,"IntfName"));
	addSerializableAttribute(new AttributeUI32(&m_hashMask,"hash_mask"));
	addSerializableAttribute(new AttributeUI32(&m_priority,"priority"));
}

const void *
PimBsrCandClientMsg::getCStructureForInputs ()
{
    string str_val;
    trace(TRACE_LEVEL_INFO, string("PimBsrCandClientMsg::getCStructureForInputs:"
        " Entered...."));

	pim_bsr_candidate_msg_t *msg = new pim_bsr_candidate_msg_t;

    /* Now copy the data members into the structure variables */
    msg->msg_type = m_msgType;
    msg->intf_type = m_intfType;
    strncpy(msg->intf_name, m_intfName.c_str(), PIM_INTF_NAME_LEN_MAX);
    msg->intf_name[PIM_INTF_NAME_LEN_MAX-1] = '\0';
	msg->hash_mask = m_hashMask;
	msg->priority = m_priority;
   
    trace(TRACE_LEVEL_DEBUG, string("msg_type: ") + toString(m_msgType));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(m_intfName));
    trace(TRACE_LEVEL_DEBUG, string("hash_mask: ") + toString(m_hashMask));
    trace(TRACE_LEVEL_DEBUG, string("priority: ") + toString(m_priority));
    
    return msg;
}

void
PimBsrCandClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void
PimBsrCandClientMsg::setMsgType(const UI8 &msgType)
{
    m_msgType  =  msgType;
}

void
PimBsrCandClientMsg::setIntfType(const UI8 &intfType)
{
    m_intfType  =  intfType;
}

void
PimBsrCandClientMsg::setIntfName(const string &intfName)
{
    m_intfName = intfName;
}

void
PimBsrCandClientMsg::setHashMask(const UI32 &hashMask)
{
	m_hashMask = hashMask;
}

void
PimBsrCandClientMsg::setPriority(const UI32 &priority)
{
	m_priority = priority;
}

/* Accessor Function Definitions */
UI8
PimBsrCandClientMsg::getMsgType() const
{
    return m_msgType;
}

UI8
PimBsrCandClientMsg::getIntfType() const
{
    return m_intfType;
}

string
PimBsrCandClientMsg::getIntfName() const
{
    return m_intfName;
}

UI32
PimBsrCandClientMsg::getPriority() const
{
	return m_priority;
}

UI32
PimBsrCandClientMsg::getHashMask() const
{
	return m_hashMask;
}

} /* namespace */
