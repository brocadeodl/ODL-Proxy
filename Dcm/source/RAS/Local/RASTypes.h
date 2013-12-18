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
 *   Author : chenj                                                     *
 **************************************************************************/

#ifndef RASTYPESH_H
#define RASTYPESH_H

namespace DcmNs
{

typedef enum
{
        RASLOGGING = 0,
	ERRDUMP,
	FOSCLI,
	IPVXCFG,
	SUPPORTSAVE,
    RAS_NETCONF,
    SUPPORTSAVESTATUS,
    RASLOGSHOW,
    AUDITCLASSCFG,
	RASLCCLI,
    SYSFACILITYCFG,
	AUTOUPLOADCFG,
	AUTOUPLOADCFGPARAM,
	RASGLOBALCONFIG,
	RASLOGSEVCFG,
	RASLOGSYSLOGCFG,
	RASLOGMSGCFG,
	RASLOGMODCFG,
	RASMANCLI,
	RAS_NODE_SPECIFIC_LOGGING
} RAS;

typedef enum
{
    NC_INVALID_CMD = 0,
    NC_SHOW_CHASSIS,
    NC_SHOW_ENV_SENSOR,
	NC_SHOW_SYS_UPTIME,
    NC_MAX_CMD = NC_SHOW_SYS_UPTIME
} NCSubCmd;

}
#endif                                            //RASTYPESH_H
