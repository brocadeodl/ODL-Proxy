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

/**
 *   @file WyserUserInfo.cpp
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Description:    This file defines the implementation for WyserUserInfo.
 *   Author:         Brian Adaniya
 *   Date:           06/03/2011
 */

#include "WyserEaGateway/WyserUserInfo.h"

using namespace WaveNs;

namespace DcmNs
{

WyserUserInfo::WyserUserInfo (const DateTime &loginDateTime, const string &username, const string &role, const string &context, const string &protocol, const IpVxAddress &ipAddress, UI32 sessionId) :
    m_loginDateTime (loginDateTime),
    m_username      (username),
    m_role          (role),
    m_context       (context),
    m_protocol      (protocol),
    m_ipAddress     (ipAddress),
    m_sessionId     (sessionId)
{
}

WyserUserInfo::~WyserUserInfo ()
{
}

DateTime WyserUserInfo::getLoginDateTime () const
{
    return (m_loginDateTime);
}

string WyserUserInfo::getUsername () const
{
    return (m_username);
}

string WyserUserInfo::getRole () const
{
    return (m_role);
}

string WyserUserInfo::getContext () const
{
    return (m_context);
}

string WyserUserInfo::getProtocol () const
{
    return (m_protocol);
}

IpVxAddress WyserUserInfo::getIpAddress () const
{
    return (m_ipAddress);
}

UI32 WyserUserInfo::getSessionId () const
{
    return (m_sessionId);
}

}
