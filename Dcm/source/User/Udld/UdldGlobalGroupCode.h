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
 *   Copyright (C) 2012-2017 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : sholla                                                     *
 **************************************************************************/


#ifndef UDLDGLOBALGROUPCODE_H
#define UDLDGLOBALGROUPCODE_H




typedef enum
{
    UDLD_GROUP_CODE_DEFAULT                                     = 0x000000,
    UDLD_GROUP_CODE_GLOBALS                                     = 0x120000,
    UDLD_GROUP_CODE_PROTO_UDLD                                  = 0x120001,
    UDLD_GROUP_CODE_HELLO                                       = 0x120002,
    UDLD_GROUP_CODE_MULTIPLIER                                  = 0x120003,
    UDLD_GROUP_CODE_SHUTDOWN                                    = 0x120004
}UDLD_GROUP_CODE_TYPES;

#endif

