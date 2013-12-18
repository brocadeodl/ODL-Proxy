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
#include "ClientInterface/Pim/PimIntfCfgClientMsg.h"
#include "ClientInterface/Pim/PimMsgDef.h"
#include "ClientInterface/Pim/PimToolKit.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

PimIntfCfgClientMsg::PimIntfCfgClientMsg()
    : DcmManagementInterfaceMessage(PIM_INTF_CFG_PIM_SP)
{
}

PimIntfCfgClientMsg::~PimIntfCfgClientMsg()
{
}

void
PimIntfCfgClientMsg::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();
    
    addSerializableAttribute(new AttributeUI8(&m_cmdCode,"CmdCode"));
    addSerializableAttribute(new AttributeUI8(&m_intfType,"IntfType"));
    addSerializableAttribute(new AttributeString(&m_intfName,"IntfName"));
    addSerializableAttribute(new AttributeBool(&m_enable,"Enable"));
    addSerializableAttribute(new AttributeUI32(&m_drPriority,"DrPriority"));
	/* commenitg : Type should be bool as we are not accepting any acl name
		uncomment when we support acl name argument */
#if 0
    addSerializableAttribute(new AttributeString(&m_multBoundaryAcl,"MultBoundaryAcl"));
#endif
	addSerializableAttribute(new AttributeBool(&m_multBoundaryAcl,"MultBoundaryAcl"));
    addSerializableAttribute(new AttributeBool(&m_pimBorder,"PimBorder"));
    addSerializableAttribute(new AttributeUI32(&m_ttlThold,"TtlThold"));
}

const void *
PimIntfCfgClientMsg::getCStructureForInputs ()
{
    //int str_len = 0;
    pim_phy_intf_cfg_msg_t *msg = NULL;
    
    trace(TRACE_LEVEL_DEBUG, string("PimIntfCfgClientMsg::getCStructureForInputs:"
        " Entered...."));

    /* Allocate memory for the msg buffer */
    msg = (pim_phy_intf_cfg_msg_t *)calloc(1, sizeof(pim_phy_intf_cfg_msg_t));
    if(!msg) {
        trace(TRACE_LEVEL_FATAL, "Unable to allocate memory for msg"); 
        return NULL;
    }
    
    /* Now copy the data members into the structure variables */
    msg->cmd_code = m_cmdCode;
    msg->intf_type = m_intfType;
    msg->enable = m_enable;
    msg->dr_prio = m_drPriority;
    msg->brdr_rtr = m_pimBorder;
    msg->ttl_thold = m_ttlThold;
	msg->mult_bdry = m_multBoundaryAcl;
    //str_len = strlen(m_intfName.c_str());
    strncpy(msg->intf_name, m_intfName.c_str(), PIM_INTF_NAME_LEN_MAX);
    msg->intf_name[PIM_INTF_NAME_LEN_MAX - 1] = '\0';
//    str_len = strlen(m_multBoundaryAcl.c_str());
	/* commenting out, as we are not supporting prefix list now */
#if 0
    strncpy(msg->mult_bdry, m_multBoundaryAcl.c_str(), MAX_PREFIX_LIST_LEN);
    msg->mult_bdry[MAX_PREFIX_LIST_LEN-1] = '\0';
#endif
    
    trace(TRACE_LEVEL_DEBUG, string("cmdCode: ") + toString(m_cmdCode));
    trace(TRACE_LEVEL_DEBUG, string("enable: ") + toString(m_enable));
    trace(TRACE_LEVEL_DEBUG, string("dr_prio: ") + toString(m_drPriority));
    trace(TRACE_LEVEL_DEBUG, string("brdr_rtr: ") + toString(m_pimBorder));
    trace(TRACE_LEVEL_DEBUG, string("ttl_thold: ") + toString(m_ttlThold));
    trace(TRACE_LEVEL_DEBUG, string("intf_type: ") + toString(m_intfType));
    trace(TRACE_LEVEL_DEBUG, string("intf_name: ") + toString(m_intfName));

    return msg;
}

void
PimIntfCfgClientMsg::loadOutputsFromCStructure (const void *pOutputCStructure)
{
    return;
}

/* Mutator Function Definitions */
void
PimIntfCfgClientMsg::setCmdCode(const UI8 &cmdCode)
{
    m_cmdCode = cmdCode;
}

void
PimIntfCfgClientMsg::setIntfType(const UI8 &intfType)
{
    m_intfType = intfType;
}

void
PimIntfCfgClientMsg::setIntfName(const string &intfName)
{
    m_intfName = intfName;
}

void
PimIntfCfgClientMsg::setEnable(const bool &enable)
{
    m_enable = enable;
}

void
PimIntfCfgClientMsg::setDrPriority(const UI32 &drPriority)
{
    m_drPriority = drPriority;
}

/* Changing the type as we dont accept acl-name now */
#if 0
void
PimIntfCfgClientMsg::setMultBoundaryAcl(const string &multBoundaryAcl)
{
    m_multBoundaryAcl = multBoundaryAcl;
}
#endif
void PimIntfCfgClientMsg::setMultBoundaryAcl(const bool &multBoundaryAcl)
{
	m_multBoundaryAcl = multBoundaryAcl;
}

void
PimIntfCfgClientMsg::setPimBorder(const bool &pimBorder)
{
    m_pimBorder = pimBorder;
}

void
PimIntfCfgClientMsg::setTtlThold(const UI32 &ttlThold)
{
    m_ttlThold = ttlThold;
}

/* Accessor Function Definitions */
UI8
PimIntfCfgClientMsg::getCmdCode() const
{
    return m_cmdCode;
}

UI8
PimIntfCfgClientMsg::getIntfType() const
{
    return m_intfType;
}

string
PimIntfCfgClientMsg::getIntfName() const
{
    return m_intfName;
}

bool
PimIntfCfgClientMsg::getEnable() const
{
    return m_enable;
}

UI32
PimIntfCfgClientMsg::getDrPriority() const
{
    return m_drPriority;
}

/* Changing the type as we dont accept acl-name now */
#if 0
string
PimIntfCfgClientMsg::getMultBoundaryAcl() const
{
    return m_multBoundaryAcl;
}
#endif
bool PimIntfCfgClientMsg::getMultBoundaryAcl() const
{
	return m_multBoundaryAcl;
}

bool
PimIntfCfgClientMsg::getPimBorder() const
{
    return m_pimBorder;
}

UI32
PimIntfCfgClientMsg::getTtlThold() const
{
    return m_ttlThold;
}

} /* namespace */
