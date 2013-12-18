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
#include "ClientInterface/Pim/PimIpMultCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimIpMultCfgClientMsg::PimIpMultCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_CFG_IP_MULT)
{
}

PimIpMultCfgClientMsg::PimIpMultCfgClientMsg(const bool &enable)
    : DcmManagementInterfaceMessage(PIM_CFG_IP_MULT),
      m_enable (enable)
{
}

PimIpMultCfgClientMsg::~PimIpMultCfgClientMsg()
{
}

void
PimIpMultCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
    addSerializableAttribute(new AttributeBool(&m_enable,"Enable"));
}

const void *
PimIpMultCfgClientMsg::getCStructureForInputs ()
{
    pim_ip_mult_cfg_msg_t *msg = NULL;
    
    trace(TRACE_LEVEL_DEBUG, string("PimIpMultCfgClientMsg::getCStructureForInputs:"
        " Entered...."));

    /* Allocate memory for the msg buffer */
    msg = (pim_ip_mult_cfg_msg_t *)calloc(1, sizeof(pim_ip_mult_cfg_msg_t));
    if(!msg) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory for msg"); 
        return NULL;
    }
    
    /* Now copy the data members into the structure variables */
    msg->enable = m_enable;
    
    trace(TRACE_LEVEL_DEBUG, string("Enable: ") + toString(m_enable));

    return msg;
}

void
PimIpMultCfgClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void
PimIpMultCfgClientMsg::setEnable(const bool &enable)
{
    m_enable = enable;
}

/* Accessor Function Definitions */
bool
PimIpMultCfgClientMsg::getEnable() const
{
    return m_enable;
}

} /* namespace */

