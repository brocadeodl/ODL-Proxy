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
 *   Author : Jitendra Singh                                    *
 ***************************************************************************/

#ifndef VCSFABRICINTERFACETYPES_H
#define VCSFABRICINTERFACETYPES_H
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/Types/BitMap.h"

using namespace WaveNs;

namespace DcmNs
{

const UI32 max_domain_list = 257;// size of buffer for getFabDomainList max domainId = 255
const UI32 cluster_size_max = 255;
const UI32 default_mgmt_port = 9110;
const UI32 default_ccmd_mgmt_port = 7110;
const UI32 defaultNspdManagementPort = 8110;
const UI32 max_retry_count = 1;
const UI32 wait_domain_lists_sync = 30000;
const UI32 max_principal_tracking_timer_restarts_on_domain_list_mismatch = 20;
const UI32 wait_for_principal = 120000;
const UI32 wait_for_rejoin = 120000;
const UI32 one_sec = 1000;
const UI32 one_minute = 60000;
const string my_sw_version = "1.0";
const UI32 principal_selection_worker_id = 1;

typedef enum
{
    VCS_SUCCESS = 0,
    GENERIC_ERROR,
    ADD_SWITCH_FAILED,
    REJOIN_SWITCH_FAILED,
    ADD_SWITCH_SERIAL_NUMBER_FAILED,
    CREATE_CLUSTER_FAILURE,
    REMOVE_SWITCH_SERIAL_NUMBER_FAILED,
    DEACTIVATE_SWITCH_SERIAL_NUMBER_FAILED,
    SWITCH_NOT_IN_VCS_MANAGED_OBJECT_DB,
    SWITCH_DOMAIN_ID_NOT_FOUND,
    CLUSTER_OBJECT_NOT_FOUND,
    SWITCH_NOT_IN_CLUSTER,
    NO_SWITCHES_IN_VCS_CLUSTER,
    MAX_SWITCH_COUNT_REACHED,
    MESSAGING_ERROR,
    GET_REMOTE_INFO_ERROR,
    SWITCH_CONFIG_ERROR,
    PARAMETER_OUT_OF_RANGE,
    CLUSTER_PARAMETERS_INVALID,
    REMOTE_SWITCH_COMMUNICATION_ERROR,
    DOMAIN_ID_INVALID
} VcsFabErrorCodes;

typedef enum
{
       VCS_FABRIC_ADD_SWITCH = 0,
       VCS_FABRIC_REJOIN_SWITCH,
       VCS_FABRIC_CREATE_CLUSTER,
       VCS_FABRIC_DELETE_CLUSTER,
       VCS_FABRIC_REMOVE_SWITCH,
       VCS_FABRIC_REPLACE_SWITCH,
       VCS_FABRIC_MAKE_PRINCIPAL,
       VCS_FABRIC_GET_CLUSTER_INFO,
       VCS_FABRIC_ADD_DOMAIN,
       VCS_FABRIC_GET_DOMAIN_LIST,
       VCS_FABRIC_UPDATE_PRINCIPAL,
       VIRTUAL_IP_GLOBAL_CONFIG_VIP,
       VIRTUAL_IP_GLOBAL_DELETE_VIP,
       VIRTUAL_IP_GLOBAL_BIND_VIP,
       VIRTUAL_IP_GLOBAL_UNBIND_VIP,
       VIRTUAL_IP_GLOBAL_SHOW_VIP,
       VCS_FAB_COLLECT_SWITCH_IDENTIFIER_INFO,
       VCS_FABRIC_SEGMENT_SWITCH,
       VCS_FABRIC_IS_IP_REACHABLE_FROM_CLUSTER,
       VCS_FABRIC_CONFIG_DISTRIBUTE,
       VCS_FAB_CONFIG_SNAPSHOT_CREATE,
       VCS_FAB_CONFIG_SNAPSHOT_RESTORE,
       VCS_FAB_CONFIG_SNAPSHOT_DELETE,
       VCS_FAB_CONFIG_SNAPSHOT_COPY_FROM_FTP
} VcsFabObjectManagerTypes;

typedef struct DomainList
{
       BitMap reachableDomainsBitMap;
       BitMap dcaDomainsBitMap;
       BitMap switchInfoReceivedBitMap;
       BitMap remoteKnownDCABitMap;
}scnDomainsStruct;

typedef struct IpAddressPingResponseParameters
{
	UI32 rbridgeId;
	bool isConnected;
}IpAddressPingResponseParameters;


}

#endif // VCSFABRICINTERFACETYPES_H
