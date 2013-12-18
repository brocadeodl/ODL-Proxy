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
 *   Author : rhkumar                                                      *
 **************************************************************************/

#ifndef AGTYPESH_H
#define AGTYPESH_H

namespace DcmNs
{

    typedef enum
    {
        AG_LOCAL_CONFIG_MESSAGE = 0,
        AG_MAPPINGS_MESSAGE,
        AG_STATIC_MAPPINGS_MESSAGE,
        AG_FAILOVER_MESSAGE,
        AG_FAILBACK_MESSAGE,
        AG_PGCREATE_MESSAGE,
        AG_PGADD_MESSAGE,
        AG_PGDEL_MESSAGE,
        AG_PGMODES_MESSAGE,
        AG_POLICY_MESSAGE,
        AG_PGRENAME_MESSAGE,
        AG_PGREMOVE_MESSAGE,
        AG_LOCAL_SHOW_MESSAGE,
        AG_FNMTOV_MESSAGE,
        AG_RELCOUNTER_MESSAGE,
        AG_VFPORT_MESSAGE
    }AgMsgs;

}
#endif                                            //AGTYPESH_H
