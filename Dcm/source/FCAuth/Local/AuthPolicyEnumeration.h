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

#ifndef AUTHPOLICYENUMERATION_H
#define AUTHPOLICYENUMERATION_H

#include <iostream>

using namespace WaveNs;

namespace DcmNs
{
    /* enum must match yang/common/brocade-fc-auth.yang policy-state enum */
    /* and auth/authipc.h */
    typedef enum
    {
        on,
        off,
        active,
        passive,
        FCAUTH_ENFORCE_MAX
    } AuthEnforceType;

    /* from sys/include/system/system.h */
    typedef enum {
        AUTH_PASSIVE = 0,   /* Authentication Optional */
        AUTH_ACTIVE,        /* Authentication Optional */
        AUTH_ON,            /* Authentication Required */
        AUTH_OFF,           /* Authentication Not Required */
        AUTH_MAX
    } authOptions_e;

}
#endif
