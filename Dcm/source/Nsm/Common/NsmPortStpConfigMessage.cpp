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

/* **
 * NsmPortStpConfigMessage.cpp
 *
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Nsm/Common/NsmPortStpConfigMessage.h"
#include "Framework/Attributes/AttributeEnum.h"


namespace DcmNs
{


NsmPortStpConfigMessage::NsmPortStpConfigMessage(const string &targetSvc, UI32 opCode)
    : ManagementInterfaceMessage ( targetSvc, opCode )
    , m_cmdCode ( 0 )
    , m_ifType  ( IF_TYPE_INVALID )
    , m_ifName  ( "" )
    , m_cost    ( 0 )
    , m_priority( 0 )
    , m_isNoCmd( false )
{
}

NsmPortStpConfigMessage::NsmPortStpConfigMessage(const string &targetSvc, UI32 opCode, UI32 cmdCode, const InterfaceType &ifType, const string &ifName, UI32 cost, UI32 priority)
    : ManagementInterfaceMessage ( targetSvc, opCode )
    , m_cmdCode ( cmdCode )
    , m_ifType  ( ifType )
    , m_ifName  ( ifName )
    , m_cost    ( cost )
    , m_priority( priority )
    , m_isNoCmd( false )
{
}

NsmPortStpConfigMessage::~NsmPortStpConfigMessage()
{
}

void NsmPortStpConfigMessage::setupAttributesForSerialization()
{
    ManagementInterfaceMessage::setupAttributesForSerialization();

    addSerializableAttribute(new AttributeUI32(&m_cmdCode, "cmdCode"));
    addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
    addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
    addSerializableAttribute(new AttributeUI32(&m_cost, "cost"));
    addSerializableAttribute(new AttributeUI32(&m_priority, "priority"));
    addSerializableAttribute(new AttributeBool(&m_isNoCmd, "NoCmd"));
}

void NsmPortStpConfigMessage::setInterfaceInfo(const InterfaceType &ifType, const string &ifName)
{
    m_ifType = ifType;
    m_ifName = ifName;
}

void NsmPortStpConfigMessage::setPriority(const UI32 &priority)
{
    m_priority = priority;
}

void NsmPortStpConfigMessage::setCost(const UI32 &cost)
{
    m_cost = cost;
}

void NsmPortStpConfigMessage::setCmdCode(const UI32 &cmdCode)
{
    m_cmdCode = cmdCode;
}

void NsmPortStpConfigMessage::setIsNoCmd(const bool &isNoCmd)
{
    m_isNoCmd = isNoCmd;
}

InterfaceType NsmPortStpConfigMessage::getIfType() const
{
    return m_ifType;
}

string NsmPortStpConfigMessage::getIfName() const
{
    return m_ifName;
}

UI32   NsmPortStpConfigMessage::getCmdCode() const 
{ 
	return m_cmdCode;
}

UI32   NsmPortStpConfigMessage::getPriority() const 
{ 
	return m_priority;
}

UI32   NsmPortStpConfigMessage::getCost() const 
{ 
	return m_cost;
}

bool    NsmPortStpConfigMessage::getIsNoCmd() const 
{ 
	return m_isNoCmd;
}

}
