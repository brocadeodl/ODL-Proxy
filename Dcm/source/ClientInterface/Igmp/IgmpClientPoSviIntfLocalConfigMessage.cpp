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

#include "ClientInterface/Igmp/IgmpClientPoSviIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpMessageDef.h"

namespace DcmNs
{

IgmpClientPoSviIntfLocalConfigMessage::IgmpClientPoSviIntfLocalConfigMessage ()
    : DcmManagementInterfaceMessage (IGMPCLIENTPOSVIINTFLOCALCONFIG),
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

}

IgmpClientPoSviIntfLocalConfigMessage::IgmpClientPoSviIntfLocalConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval,const string &ifName,const UI64 &ifIndex, const UI32 &ifType, const UI32 &opCode,const bool &negation)
    : DcmManagementInterfaceMessage (IGMPCLIENTPOSVIINTFLOCALCONFIG),
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
    WaveNs::trace(TRACE_LEVEL_INFO, __FUNCTION__);
}

IgmpClientPoSviIntfLocalConfigMessage::~IgmpClientPoSviIntfLocalConfigMessage ()
{
}

void  IgmpClientPoSviIntfLocalConfigMessage::setupAttributesForSerialization()
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

void  IgmpClientPoSviIntfLocalConfigMessage::setLmqi(const UI32 &lmqi)
{
    m_lmqi  =  lmqi;
}

UI32  IgmpClientPoSviIntfLocalConfigMessage::getLmqi() const
{
    return (m_lmqi);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setQmrt(const UI32 &qmrt)
{
    m_qmrt  =  qmrt;
}

UI32  IgmpClientPoSviIntfLocalConfigMessage::getQmrt() const
{
    return (m_qmrt);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setImmediateLeave(const bool &immediateLeave)
{
    m_immediateLeave  =  immediateLeave;
}

bool  IgmpClientPoSviIntfLocalConfigMessage::getImmediateLeave() const
{
    return (m_immediateLeave);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setQueryInterval(const UI32 &queryInterval)
{
    m_queryInterval  =  queryInterval;
}

UI32  IgmpClientPoSviIntfLocalConfigMessage::getQueryInterval() const
{
    return (m_queryInterval);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpClientPoSviIntfLocalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setIfIndex(const UI64 &ifIndex)
{
    m_ifIndex  =  ifIndex;
}

UI64  IgmpClientPoSviIntfLocalConfigMessage::getIfIndex() const
{
    return (m_ifIndex);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpClientPoSviIntfLocalConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setOpCode(const UI32 &opCode)
{
    m_opCode  =  opCode;
}

UI32  IgmpClientPoSviIntfLocalConfigMessage::getOpCode() const
{
    return (m_opCode);
}

void  IgmpClientPoSviIntfLocalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  IgmpClientPoSviIntfLocalConfigMessage::getNegation() const
{
    return (m_negation);
}

const void *IgmpClientPoSviIntfLocalConfigMessage::getCStructureForInputs ()
{
    igmp_intf_cfg_msg_t *pInput = (igmp_intf_cfg_msg_t *) malloc(sizeof(igmp_intf_cfg_msg_t));

    pInput->lmqi            = m_lmqi;
    pInput->qmrt            = m_qmrt;
    pInput->immediate_leave = m_immediateLeave;
    pInput->query_interval  = m_queryInterval;
    strncpy (pInput->if_name, m_ifName.c_str(), IGMP_MAX_STR);
    pInput->if_index        = m_ifIndex;
    pInput->if_type         = m_ifType;
    pInput->opcode          = m_opCode;

    return (pInput);
}

void IgmpClientPoSviIntfLocalConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
{
}
}
