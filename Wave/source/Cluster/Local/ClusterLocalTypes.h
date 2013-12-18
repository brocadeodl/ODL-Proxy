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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLUSTEROCALTYPES_H
#define CLUSTEROCALTYPES_H

namespace WaveNs
{

typedef enum
{
    CLUSTER_LOCAL_SET_THIS_NODE_IP_ADDRESS           = 0,
    CLUSTER_LOCAL_REPORT_PRIMARY_NODE_CHANGED        = 1,
    CLUSTER_LOCAL_REPORT_NODE_REMOVED_FROM_CLUSTER   = 2,
    CLUSTER_LOCAL_REPORT_REPLACE                     = 3,
    CLUSTER_LOCAL_SET_HARDWARE_SYNCHRONIZATION_STATE = 4,
	CLUSTER_LOCAL_SET_HA_ROLE                        = 5,
	CLUSTER_LOCAL_SLOT_ONLINE_MESSAGE				 = 6,
	CLUSTER_LOCAL_SLOT_OFFLINE_MESSAGE				 = 7,
	CLUSTER_LOCAL_SLOT_REMOVE_MESSAGE				 = 8,
	CLUSTER_LOCAL_SLOT_EVENT_MESSAGE				 = 9,
    CLUSTER_LOCAL_GET_SLOT_OBJECTID                  = 10,
    CLUSTER_LOCAL_GET_LOCATION_OBJECTID              = 11,
    CLUSTER_LOCAL_GET_SLOTNUMBER                     = 12,
    CLUSTER_LOCAL_GET_LOCATIONID                     = 13,
    CLUSTER_LOCAL_SLOT_REMOVE_ALL_MESSAGE            = 14,
    CLUSTER_LOCAL_REPORT_AS_CLIENT_MESSAGE           = 15,
    CLUSTER_LOCAL_SET_CONTROLLER_DETAILS_MESSAGE     = 16
} ClusterLocaTypes;

}

#endif // CLUSTEROCALTYPES_H
