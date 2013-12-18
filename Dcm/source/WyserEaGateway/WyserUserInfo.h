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
 *   @file WyserUserInfo.h
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Description:    This file defines the header for WyserUserInfo
 *   Author:         Brian Adaniya
 *   Date:           06/03/2011
 */

#ifndef WYSERUSERINFO_H
#define WYSERUSERINFO_H

#include "Framework/Types/Types.h"
#include "Framework/Types/DateTime.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

class WyserUserInfo
{
    private :
    protected :
    public :
                                    WyserUserInfo       (const DateTime &loginDateTime, const string &username, const string &role, const string &context, const string &protocol, const IpVxAddress &ipAddress, UI32 sessionId);
        virtual                    ~WyserUserInfo       ();

                DateTime            getLoginDateTime    () const;
                string              getUsername         () const;
                string              getRole             () const;
                string              getContext          () const;
                string              getProtocol         () const;
                IpVxAddress         getIpAddress        () const;
                UI32                getSessionId        () const;

    // Now the data members
    private :
                DateTime            m_loginDateTime;
                string              m_username;
                string              m_role;
                string              m_context;
                string              m_protocol;
                IpVxAddress         m_ipAddress;
                UI32                m_sessionId;    

    protected :
    public :
};

}

#endif //WYSERUSERINFO_H
