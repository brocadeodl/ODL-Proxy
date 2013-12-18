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
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"

#include "Nsm/Global/Igmp/IgmpPoSviIntfGlobalConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

IgmpPoSviIntfGlobalConfigMessage::IgmpPoSviIntfGlobalConfigMessage ()
    : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), IGMPPOSVIINTFGLOBALCONFIG)
{
    m_lmqi = 0;   
    m_qmrt = 0;
    m_immediateLeave = false;
    m_queryInterval = 0;
    // m_ifName = <FIXME>;
    m_ifType = 0;
    m_ifIndex = 0;
    m_opCode = 0;
    m_negation = false;
}

IgmpPoSviIntfGlobalConfigMessage::IgmpPoSviIntfGlobalConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &immediateLeave,const UI32 &queryInterval,const string &ifName,const UI64 &ifIndex, const UI32 &ifType, const UI32 &opCode,const bool &negation)
    : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (), IGMPPOSVIINTFGLOBALCONFIG),
    m_lmqi    (lmqi),
    m_qmrt    (qmrt),
    m_immediateLeave    (immediateLeave),
    m_queryInterval    (queryInterval),
    m_ifName    (ifName),
    m_ifIndex    (ifIndex),
    m_ifType    (ifType),
    m_opCode    (opCode),
    m_negation    (negation)
{
}

IgmpPoSviIntfGlobalConfigMessage::~IgmpPoSviIntfGlobalConfigMessage ()
{
}

void  IgmpPoSviIntfGlobalConfigMessage::setupAttributesForSerialization()
{
    ManagementInterfaceMessage::setupAttributesForSerialization ();
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

void  IgmpPoSviIntfGlobalConfigMessage::setLmqi(const UI32 &lmqi)
{
    m_lmqi  =  lmqi;
}

UI32  IgmpPoSviIntfGlobalConfigMessage::getLmqi() const
{
    return (m_lmqi);
}

void  IgmpPoSviIntfGlobalConfigMessage::setQmrt(const UI32 &qmrt)
{
    m_qmrt  =  qmrt;
}

UI32  IgmpPoSviIntfGlobalConfigMessage::getQmrt() const
{
    return (m_qmrt);
}

void  IgmpPoSviIntfGlobalConfigMessage::setImmediateLeave(const bool &immediateLeave)
{
    m_immediateLeave  =  immediateLeave;
}

bool  IgmpPoSviIntfGlobalConfigMessage::getImmediateLeave() const
{
    return (m_immediateLeave);
}

void  IgmpPoSviIntfGlobalConfigMessage::setQueryInterval(const UI32 &queryInterval)
{
    m_queryInterval  =  queryInterval;
}

UI32  IgmpPoSviIntfGlobalConfigMessage::getQueryInterval() const
{
    return (m_queryInterval);
}

void  IgmpPoSviIntfGlobalConfigMessage::setIfName(const string &ifName)
{
    m_ifName  =  ifName;
}

string  IgmpPoSviIntfGlobalConfigMessage::getIfName() const
{
    return (m_ifName);
}

void  IgmpPoSviIntfGlobalConfigMessage::setIfIndex(const UI64 &ifIndex)
{
    m_ifIndex  =  ifIndex;
}

UI64  IgmpPoSviIntfGlobalConfigMessage::getIfIndex() const
{
    return (m_ifIndex);
}

void  IgmpPoSviIntfGlobalConfigMessage::setIfType(const UI32 &ifType)
{
    m_ifType  =  ifType;
}

UI32  IgmpPoSviIntfGlobalConfigMessage::getIfType() const
{
    return (m_ifType);
}

void  IgmpPoSviIntfGlobalConfigMessage::setOpCode(const UI32 &opCode)
{
    m_opCode  =  opCode;
}

UI32  IgmpPoSviIntfGlobalConfigMessage::getOpCode() const
{
    return (m_opCode);
}

void  IgmpPoSviIntfGlobalConfigMessage::setNegation(const bool &negation)
{
    m_negation  =  negation;
}

bool  IgmpPoSviIntfGlobalConfigMessage::getNegation() const
{
    return (m_negation);
}

}
