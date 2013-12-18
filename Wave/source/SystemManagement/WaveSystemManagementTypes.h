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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

namespace WaveNs
{

typedef enum
{
    WAVE_SYSTEM_MANAGEMENT_DISPLAY_CURRENT_CONFIGURATION_BY_QUALIFIED_YANG_PATH = 0,
    WAVE_SYSTEM_MANAGEMENT_GET_YANG_USER_INTERFACE                              = 1,
    WAVE_SYSTEM_MANAGEMENT_DISPLAY_CURRENT_CONFIGURATION_BY_TARGET_NODE_NAME    = 2,
    WAVE_SYSTEM_MANAGEMENT_REBUILD_YANG_INTERFACES                              = 3,
    WAVE_SYSTEM_MANAGEMENT_GET_POSSIBLE_COMPLETIONS_MESSAGE                     = 4,
    WAVE_SYSTEM_MANAGEMENT_GATEWAY_MESSAGE                                      = 5
} WaveSystemManagementTypes;

}
