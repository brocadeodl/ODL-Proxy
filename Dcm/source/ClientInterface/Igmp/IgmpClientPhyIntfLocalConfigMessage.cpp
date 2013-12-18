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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Abhay Gidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"

#include "ClientInterface/Igmp/IgmpClientPhyIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpMessageDef.h"

namespace DcmNs
{

IgmpClientPhyIntfLocalConfigMessage::IgmpClientPhyIntfLocalConfigMessage ()
    : DcmManagementInterfaceMessage (IGMPCLIENTPHYINTFLOCALCONFIG),
    m_lmqi              (0),
    m_qmrt              (0),
    m_immediateLeave    (false),
    m_queryInterval     (0),
    m_ifName            (""),
    m_ifIndex           (0),
    m_ifType            (0),
    m_opCode            (IGMP_DEFAULT_CMD_CODE),
    m_negation          (false)
{ 
//    WaveNs::trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

IgmpClientPhyIntfLocalConfigMessage::IgmpClientPhyIntfLocalConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval,const string &ifName,const UI64 &ifIndex, const UI32 &ifType, const UI32 &opCode,const bool &negation)
    : DcmManagementInterfaceMessage (IGMPCLIENTPHYINTFLOCALCONFIG),
    m_lmqi    (lmqi),
    m_qmrt    (qmrt),
    m_immediateLeave    (immediateLeave),
    m_queryInterval    (queryInterval),
    m_ifName    (ifName),
    m_ifIndex   (ifIndex),
    m_ifType    (ifType),
    m_opCode    (opCode),
    m_negation  (negation)
{
}

IgmpClientPhyIntfLocalConfigMessage::~IgmpClientPhyIntfLocalConfigMessage ()
{
}

void  IgmpClientPhyIntfLocalConfigMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization ();
    addSerializableAttribute (new AttributeUI32(&m_lmqi,"lmqi"));
    addSerializableAttribute (new AttributeUI32(&m_qmrt,"qmrt"));
    addSerializableAttribute (new AttributeBool(&m_immediateLeave,"immediateLeave"));
    addSerializableAttribute (new AttributeUI32(&m_queryInterval,"queryInterval"));
    addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
    addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
    addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
}

void  IgmpClientPhyIntfLocalConfigMessage::setLmqi(const UI32 &lmqi)
{
    m_lmqi  =  lmqi;
}

UI32  IgmpClientPhyIntfLocalConfigMessage::getLmqi() const
{
    return (m_lmqi);
}

void  IgmpClientPhyIntfLocalConfigMessage::setQmrt(const UI32 &qmrt)
{
    m_qmrt  =  qmrt;
}

UI32  IgmpClientPhyIntfLocalConfigMessage::getQmrt() const
{
    return (m_qmrt);
}

void  IgmpClientPhyIntfLocalConfigMessage::setImmediateLeave(const bool &immediateLeave)
{
    m_immediateLeave  =  immediateLeave;
}

bool  IgmpClientPhyIntfLocalConfigMessage::getImmediateLeave() const
{
    return (m_immediateLeave);
}

void  IgmpClientPhyIntfLocalConfigMessage::setQueryInterval(const UI32 &queryInterval)
{
    m_queryInterval  =  queryInterval;
}

UI32  IgmpClientPhyIntfLocalConfigMessage::getQueryInterval() const
{
    return (m_queryInterval);
}

void  IgmpClientPhyIntfLocalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpClientPhyIntfLocalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  IgmpClientPhyIntfLocalConfigMessage::setIfIndex(const UI64 &ifIndex)
{
    m_ifIndex  =  ifIndex;
}

UI64  IgmpClientPhyIntfLocalConfigMessage::getIfIndex() const
{
    return (m_ifIndex);
}

void  IgmpClientPhyIntfLocalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpClientPhyIntfLocalConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  IgmpClientPhyIntfLocalConfigMessage::setOpCode(const UI32 &opCode)
{
    m_opCode  =  opCode;
}

UI32  IgmpClientPhyIntfLocalConfigMessage::getOpCode() const
{
    return (m_opCode);
}

void  IgmpClientPhyIntfLocalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  IgmpClientPhyIntfLocalConfigMessage::getNegation() const
{
    return (m_negation);
}

const void *IgmpClientPhyIntfLocalConfigMessage::getCStructureForInputs ()
{
    igmp_intf_cfg_msg_t *pInput = (igmp_intf_cfg_msg_t *) malloc(sizeof(igmp_intf_cfg_msg_t));

    pInput->lmqi            = m_lmqi;
    pInput->qmrt            = m_qmrt;
    pInput->immediate_leave = m_immediateLeave;
    pInput->query_interval  = m_queryInterval;
    strncpy (pInput->if_name, m_ifName.c_str(), IGMP_MAX_STR);
    pInput->if_type         = m_ifType;
    pInput->if_index        = m_ifIndex;
    pInput->opcode          = m_opCode;

    return (pInput);
}

void IgmpClientPhyIntfLocalConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}
}
