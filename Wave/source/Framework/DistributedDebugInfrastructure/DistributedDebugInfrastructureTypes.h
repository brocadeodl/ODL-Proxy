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

 /*@file DistributedDebugInfrastructureTypes.h 
 * Copyright (C) 2011 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * Description: This file contains the declaration of all types
 *              used by the distributed debug infrastructure 
 *          
 *              
 * 
 * Author :     Aashish Akhouri 
 * Date :       06/18/2011 
 */
#ifndef DISTRIBUTEDDEBUGINFRASTRUCTURETYPES_H
#define DISTRIBUTEDDEBUGINFRASTRUCTURETYPES_H

namespace WaveNs
{

typedef enum
{
    RUN_DEBUG_SCRIPT_MESSAGE,
    RUN_DEBUG_SCRIPT_ON_CLUSTER_MEMBER_MESSAGE
} DistributedDebugInfrastructureTypes;

}

#endif // DISTRIBUTEDDEBUGINFRASTRUCTURETYPES_H
