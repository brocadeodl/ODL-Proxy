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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sholla                                                     *
 **************************************************************************/

#ifndef UIDSERVICETYPESH_H
#define UIDSERVICETYPESH_H

namespace DcmNs
{

    typedef enum {

        // Allocate a UID for an entity
        UID_ALLOC = 1,

        // Release the UID (marks the UID entry as stale)
        UID_FREE,

        // Delete the UID
        UID_REMOVE,

        // Explicitly set an UID
        UID_SET,

        // Reqtieve ids of given type
        UID_QUERY,

        UID_FREE_BULK,

        UID_OP_MAX  // Keep this last
    } UidServiceMessageCode;


    typedef enum {
        IDTYPE_INVALID_ID = 0,
        // Last byte of spanning-tree logical bridge MAC.
        IDTYPE_BRIDGE_ID_LSB = 100,

        // Logical port id for STP/PVST
        IDTYPE_STP_PORT_ID,

        // Logical port id for RSTP/MSTP/RPVST
        IDTYPE_RSTP_PORT_ID

    } IdType;

}
#endif                                            //UIDSERVICETYPESH_H
