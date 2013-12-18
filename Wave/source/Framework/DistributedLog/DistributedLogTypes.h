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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef DISTRIBUTEDLOGTYPES_H
#define DISTRIBUTEDLOGTYPES_H

namespace WaveNs
{

typedef enum
{
    DISTRIBUTED_LOG_ADD_LOG_ENTRY = 0,
    DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES,
    DISTRIBUTED_LOG_ADD_LOG_ENTRY_MANAGEMENT_INTERFACE,
    DISTRIBUTED_LOG_UPDATE_MAX_LOG_ENTRIES_MANAGEMENT_INTERFACE,
    DISTRIBUTED_LOG_INVOKE_ADD_LOG_API,
    DISTRIBUTED_LOG_UPDATE_IN_MEMORY_LOG_ID
} DistributedLogTypes;

}

#endif // DISTRIBUTEDLOGTYPES_H
