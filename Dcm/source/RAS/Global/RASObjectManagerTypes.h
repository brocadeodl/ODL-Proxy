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

 /**************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : James Chen		                                    *
 ***************************************************************************/

#ifndef RASOBJECTMANAGERTYPES_H
#define  RASOBJECTMANAGERTYPES_H

namespace DcmNs
{
#define MAX_CLUSTER_SIZE 1023
#define MAPPED_ID_MAX 1023
#define MAPPED_ID_MIN 1

typedef enum
{
    RAS_SUCCESS = 0
} RASErrorCodes;

typedef enum
{
	RAS_COPY_SUPPORT = 0,
	RAS_CLUSTER_CFG,
	RAS_AUDIT_CLASS,
	RAS_FOSCLI
} RASObjectManagerTypes;

typedef enum
{
	NONE_CLASS	= 0,
	SECURITY 	= 1,
	CONFIGURATION 	= 2,
	FIRMWARE	= 3,
	ALL_CLASS	= 10
} RASAuditClassTypes;

typedef enum
{
    RAS_LOG_LOCAL0  = 0,
    RAS_LOG_LOCAL1  = 1,
    RAS_LOG_LOCAL2  = 2,
    RAS_LOG_LOCAL3  = 3,
    RAS_LOG_LOCAL4  = 4,
    RAS_LOG_LOCAL5  = 5,
    RAS_LOG_LOCAL6  = 6,
    RAS_LOG_LOCAL7  = 7
} RASSysFacilityTypes;

// match these enums in RAS.yang console severity
typedef enum
{
	DCM_RASLOG_INFO = 1,
	DCM_RASLOG_ERROR = 2,
	DCM_RASLOG_WARNING = 3,
	DCM_RASLOG_CRITICAL = 4,
	DCM_RASLOG_DEFAULT = 5
} Severity;

}

#endif // RASOBJECTMANAGERTYPES_H
