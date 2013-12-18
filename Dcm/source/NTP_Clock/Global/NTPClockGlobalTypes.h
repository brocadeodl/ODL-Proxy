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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef NTPCLOCKGLOBALTYPESH_H
#define NTPCLOCKGLOBALTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        NTPCLOCKGLOBALADDSERVER = 0 ,
        NTPCLOCKGLOBALCHANGESERVER,
        NTPCLOCKGLOBALREMOVESERVER,
        NTPCLOCKGLOBALGETSERVER,
        NTPCLOCKGLOBALSETCLOCK,
        NTPCLOCKGLOBALGETCLOCK,
        NTPCLOCKGLOBALSETZONE,
        NTPCLOCKGLOBALADDKEY,
        NTPCLOCKGLOBALREMOVEKEY
    }NTPClockGlobal;

#define ALL_IN_CLUSTER "all"
#define TS_LOCALHOST_IPV4 "127.0.0.1"
#define LOCAL_SWITCH_ID 0
#define NTPSERVER_NO_KEY  "NO_KEY"

}
#endif                                            //NTPCLOCKGLOBALTYPESH_H
