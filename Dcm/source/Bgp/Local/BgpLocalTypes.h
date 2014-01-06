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
 *   Copyright (C) 2013-2018 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pbalacha                                                     *
 **************************************************************************/


#ifndef BGPTYPES_H
#define BGPTYPES_H



#include "User/Bgp/BgpLocalGroupCode.h"


using namespace WaveNs;


namespace DcmNs{

typedef enum
{
    BGP_LOCAL_ACTION_TSEND = 1,
    BGP_LOCAL_ACTION_TRECEIVE = 2
}BGP_ACTION_T_TYPES;

typedef enum
{
    BGP_LOCAL_INTERFACE_TYPE_TGIGABITETHERNET = 1,
    BGP_LOCAL_INTERFACE_TYPE_TTENGIGABITETHERNET = 2,
    BGP_LOCAL_INTERFACE_TYPE_TFORTYGIGABITETHERNET = 3
}BGP_INTERFACE_TYPE_T_TYPES;


typedef enum
{
    BGPLOCALOBJECTMANAGERMAX
}BGPLOCALOBJECTMANAGER_MESSAGE_TYPES;


}
#endif