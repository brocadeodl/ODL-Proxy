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
 * UdldIntfConfigClientMessage.cpp
 *
 * Aug 09, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Udld/UdldIntfConfigClientMessage.h"
#include "ClientInterface/Udld/udld_dcm_def.h"

namespace DcmNs {

UdldIntfConfigClientMessage::UdldIntfConfigClientMessage()
    : DcmManagementInterfaceMessage (MSG_UDLD_PHY_INTF_CONF)
    , m_commandCode ( 0 )
    , m_ifType      ( IF_TYPE_INVALID )
    , m_udldDisable ( true )
{
}

UdldIntfConfigClientMessage::~UdldIntfConfigClientMessage()
{
}

void UdldIntfConfigClientMessage::setupAttributesForSerialization()
{
    DcmManagementInterfaceMessage::setupAttributesForSerialization();

    addSerializableAttribute(new AttributeUI32(&m_commandCode, "commandCode"));
    addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
    addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
    addSerializableAttribute(new AttributeBool(&m_udldDisable, "udldDisable"));
}

UI32 UdldIntfConfigClientMessage::getCommandCode () const
{
    return m_commandCode;
}

void UdldIntfConfigClientMessage::setCommandCode (const UI32 commandCode)
{
    m_commandCode = commandCode;
}

InterfaceType UdldIntfConfigClientMessage::getIfType () const
{
    return m_ifType;
}

string UdldIntfConfigClientMessage::getIfName () const
{
    return m_ifName;
}

void UdldIntfConfigClientMessage::setInterfaceInfo (const InterfaceType ifType, const string &ifName)
{
    m_ifType = ifType;
    m_ifName = ifName;
}

bool UdldIntfConfigClientMessage::getUdldDisable () const
{
    return m_udldDisable;
}

void UdldIntfConfigClientMessage::setUdldDisable (bool udldDisable)
{
    m_udldDisable = udldDisable;
}


const void* UdldIntfConfigClientMessage::getCStructureForInputs ()
{
    udld_mgmt_conf_req_t *req = (udld_mgmt_conf_req_t*) calloc(1, sizeof(udld_mgmt_conf_req_t));
    if (req == NULL) {
        tracePrintf(TRACE_LEVEL_ERROR, "UdldIntfConfigClientMessage::getCStructureForInputs : calloc returned NULL!!");
        return NULL;
    }

    req->cmd_code = m_commandCode;

    req->entry.intf_conf.if_type = m_ifType;

    // fill 2-tuple ifName
    const char *s = m_ifName.c_str();
    const char *t = strchr(s, '/');
    if (t != NULL && strchr(t+1, '/') != NULL) s = t+1;
    strncpy(req->entry.intf_conf.if_name, s, sizeof req->entry.intf_conf.if_name);

    req->entry.intf_conf.disable = ( m_udldDisable ? 1 : 0 );

    return req;
}

void UdldIntfConfigClientMessage::loadOutputsFromCStructure (const void *out)
{
}

}

