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

#ifndef AUTHPROTOCOLENUMERATION_H
#define AUTHPROTOCOLENUMERATION_H

#include <iostream>

using namespace WaveNs;

namespace DcmNs
{
    /* enum must match yang/common/brocade-fc-auth.yang authtype enum */
    /* and auth/authipc.h */
    typedef enum
    {
        dhchap,
        fcap,
        fcap_dhchap,
        FCAUTH_PROTOCOLTYPE_MAX
    } AuthProtocolType;

    typedef string AuthProtocolGroup;

    /* enum must match yang/common/brocade-fc-auth.yang hashtype enum */
    /* and auth/authipc.h */
    typedef enum
    {
        sha1,
        md5,
        all,
        FCAUTH_PROTOCOLHASHTYPE_MAX
    } AuthProtocolHash;

}
#endif
