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



#ifndef EVENTDEFINES_H
#define EVENTDEFINES_H

#define TASK_EVENT ("TaskEvent")
// Datacenter related
#define DATACENTER_CREATE ("DatacenterCreatedEvent")
#define DATACENTER_RENAMED ("DatacenterRenamedEvent")
// VM related
#define VM_CREATE ("VmCreatedEvent")
#define VM_RENAMED ("VmRenamedEvent")
#define VM_DELETE ("VmRemovedEvent")
#define VM_RECONF ("VmReconfiguredEvent")
#define VM_BNG_HOT_MIGRATED ("VmBeingHotMigratedEvent")
#define VM_BNG_MIGRATED ("VmBeingMigratedEvent")
#define VM_MIGRATED ("VmMigratedEvent")

// DVS related
#define DVS_CREATE ("DvsCreatedEvent")
#define DVS_RENAMED ("DvsRenamedEvent")
#define DVS_DELETE ("DvsDestroyedEvent")
#define DVS_RECONF ("DvsReconfiguredEvent")
// DVPG related
#define DVPG_CREATE ("DVPortgroupCreatedEvent")
#define DVPG_RENAMED ("DVPortgroupRenamedEvent")
#define DVPG_DELETE ("DVPortgroupDestroyedEvent")
#define DVPG_RECONF ("DVPortgroupReconfiguredEvent")
// Hostrelated
#define HOST_ADD ("HostAddedEvent")
#define HOST_REMOVE ("HostRemovedEvent")


// TaskEvents
#define TASK_UPDATE_NET_CONF ("Task: Update network configuration")
#define TASK_ADD_VS ("Task: Add virtual switch")
#define TASK_REMOVE_VS ("Task: Remove virtual switch")
#define TASK_ADD_PG ("Task: Add port group")
#define TASK_REMOVE_PG ("Task: Remove port group")
#define TASK_RECONF_PG ("Task: Reconfigure port group")
#define TASK_REMOVE_VNIC ("Task: Remove virtual NIC")
#define TASK_ADD_VNIC ("Task: Add virtual NIC")
#define TASK_REMOVE_VMK ("Task: Remove service console virtual NIC")
#define TASK_REMOVE_DC ("Task: Remove datacenter")
#define TASK_UPDATE_DNS_CONF ("Task: Update DNS configuration")
#define TASK_REGISTER_VM ("Task: Register virtual machine")
#define TASK_RELOCATE_VM ("Task: Relocate virtual machine")
#define TASK_CLONE_VM ("Task: Clone virtual machine" )
#define TASK_MARK_VM_TEMPLATE ("Task: Mark virtual machine as template")
#define TASK_UPDATE_RESPOOL_DVS ("Task: Update network resource pools on vSphere Distributed Switch")
#define TASK_ADD_HOST ("Task: Add standalone host")
#define TASK_IMPORT_CONF ("Task: Import configuration of the entity")
#define TASK_VM_RECONF ("Task: Reconfigure virtual machine")

    static inline bool
isSupportedTaskEvent (string msg)
{
    return (msg == TASK_UPDATE_NET_CONF) ||
        (msg == TASK_ADD_VS) ||
        (msg == TASK_REMOVE_VS) ||
        (msg == TASK_ADD_PG) ||
        (msg == TASK_REMOVE_PG) ||
        (msg == TASK_RECONF_PG) ||
        (msg == TASK_REMOVE_VNIC) ||
        (msg == TASK_REMOVE_VMK) ||
        (msg == TASK_REMOVE_DC) ||
        (msg == TASK_UPDATE_DNS_CONF) ||
        (msg == TASK_REGISTER_VM) ||
        (msg == TASK_RELOCATE_VM) ||
        (msg == TASK_CLONE_VM) ||
        (msg == TASK_MARK_VM_TEMPLATE) ||
        (msg == TASK_UPDATE_RESPOOL_DVS) ||
        (msg == TASK_ADD_HOST) ||
        (msg == TASK_IMPORT_CONF) ||
        (msg == TASK_ADD_VNIC) ||
        (msg == TASK_VM_RECONF);
}

    static inline vector<string>
getSupportedEvents ()
{
    vector <string> event_list;
    event_list.push_back(TASK_EVENT);
    event_list.push_back(DATACENTER_CREATE);
    event_list.push_back(DATACENTER_RENAMED);
    event_list.push_back(VM_CREATE);
    event_list.push_back(VM_RENAMED);
    event_list.push_back(VM_DELETE);
    event_list.push_back(VM_RECONF);
    event_list.push_back(VM_MIGRATED);
    event_list.push_back(DVS_CREATE);
    event_list.push_back(DVS_RENAMED);
    event_list.push_back(DVS_DELETE);
    event_list.push_back(DVS_RECONF);
    event_list.push_back(DVPG_CREATE);
    event_list.push_back(DVPG_RENAMED);
    event_list.push_back(DVPG_DELETE);
    event_list.push_back(DVPG_RECONF);
    event_list.push_back(HOST_ADD);
    event_list.push_back(HOST_REMOVE);

    return event_list;
}

typedef enum
{
    VNM_CREATE = 0,
    VNM_UPDATE,
    VNM_DELETE,
    VNM_RECONF
} Operation;

typedef enum
{
    DATACENTER_EVENT = 10,
    VSWITCH_EVENT,
    PGHOSTVS_EVENT,
    VM_EVENT,
    DVS_EVENT,
    DVPG_EVENT,
    HOST_EVENT,
    VNIC_EVENT,
    VMNIC_EVENT,
    SPG_EVENT,
    SNIC_EVENT,
    DP_EVENT,
    EVENT_DISTRIBUTE,
    LOCAL_DATACENTER_EVENT,
    LOCAL_VSWITCH_EVENT,
    LOCAL_PGHOSTVS_EVENT,
    LOCAL_VM_EVENT,
    LOCAL_DVS_EVENT,
    LOCAL_DVPG_EVENT,
    LOCAL_HOST_EVENT,
    LOCAL_VNIC_EVENT,
    LOCAL_VMNIC_EVENT,
    LOCAL_SPG_EVENT,
    LOCAL_SNIC_EVENT,
    LOCAL_DP_EVENT,
    SIMPLE_EVENT,
    LOCAL_SIMPLE_EVENT

} DiscoverEvent;



#endif  //EVENTDEFINES_H

