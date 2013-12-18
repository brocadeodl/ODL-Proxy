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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef SYSTEMMONITORGLOBALTYPESH_H
#define SYSTEMMONITORGLOBALTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        MONITORGLOBALSHOWSYSTEMMONITOR = 0,
        SYSTEMMONITORGLOBALCHANGEFANTHRESHOLD,
        SYSTEMMONITORGLOBALCHANGEFANALERT,
        SYSTEMMONITORGLOBALCHANGEPOWERTHRESHOLD,
        SYSTEMMONITORGLOBALCHANGEPOWERALERT,
        SYSTEMMONITORGLOBALCHANGETEMPTHRESHOLD,
        SYSTEMMONITORGLOBALCHANGEWWNCARDTHRESHOLD,
        SYSTEMMONITORGLOBALCHANGEWWNCARDALERT,
        SYSTEMMONITORGLOBALCHANGESFPALERT,
        SYSTEMMONITORGLOBALCHANGECOMPACTFLASHTHRESHOLD,
        SYSTEMMONITORGLOBALCHANGEFAULTYPORTTHRESHOLD,
        SYSTEMMONITORGLOBALFRUMAILSETCFG,
        SYSTEMMONITORGLOBALFRUMAILADDEMAIL,
        SYSTEMMONITORGLOBALFRUMAILREMOVEEMAIL,
		SYSTEMMONITORGLOBALSECMAILSETCFG,
        SYSTEMMONITORGLOBALSECMAILADDEMAIL,
        SYSTEMMONITORGLOBALSECMAILREMOVEEMAIL,
        SYSTEMMONITORGLOBALSFPMAILSETCFG,
        SYSTEMMONITORGLOBALSFPMAILADDEMAIL,
        SYSTEMMONITORGLOBALSFPMAILREMOVEEMAIL,
        SYSTEMMONITORGLOBALINTMAILSETCFG,
        SYSTEMMONITORGLOBALINTMAILADDEMAIL,
        SYSTEMMONITORGLOBALINTMAILREMOVEEMAIL,
		RELAYIPCONFIG,
		RELAYIPSET,
		RELAYIPDEL

    }SystemMonitorGlobal;

#define LOCAL_SWITCH_ID 0
#define FWEMAIL_TOKEN_SEPERATOR ","

}
#endif                                            //SYSTEMMONITORGLOBALTYPESH_H
