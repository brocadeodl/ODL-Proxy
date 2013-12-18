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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWOBJECTMANAGERTYPES_H
#define OPENFLOWOBJECTMANAGERTYPES_H

namespace OpenFlowNs
{

typedef enum
{
    OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_SWITCH,
    OPENFLOW_OBJECT_MANAGER_DELETE_OPENFLOW_SWITCH,
    OPENFLOW_OBJECT_MANAGER_CREATE_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_DELETE_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_ADD_OPENFLOW_LOGICAL_SWITCH_TO_OPENFLOW_SWITCH,
    OPENFLOW_OBJECT_MANAGER_REMOVE_OPENFLOW_LOGICAL_SWITCH_FROM_OPENFLOW_SWITCH,
    OPENFLOW_OBJECT_MANAGER_ADD_OPENFLOW_CONTROLLER_TO_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_CONNECT_OPENFLOW_LOGICAL_SWITCH_TO_OPENFLOW_CONTROLLER,
    OPENFLOW_OBJECT_MANAGER_REMOVE_OPENFLOW_CONTROLLER_FROM_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_ADD_PHYSICAL_PORT_TO_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_REMOVE_PHYSICAL_PORT_FROM_OPENFLOW_LOGICAL_SWITCH,
    OPENFLOW_OBJECT_MANAGER_UPDATE_PHYSICAL_PORT_STATUS_IN_OPENFLOW_LOGICAL_SWITCH,
    OPEN_FLOW_OBJECT_MANAGER_ADD_FLOW_MESSAGE
} OpenFlowObjectManagerTypes;

}
#endif // OPENFLOWOBJECTMANAGERTYPES_H
