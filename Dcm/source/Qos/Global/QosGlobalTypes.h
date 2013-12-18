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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSGLOBALTYPESH_H
#define QOSGLOBALTYPESH_H


#define DEFAULT_RATE_LIMIT 3000000
#define DEFAULT_BURST 4096
#define DEFAULT_THRESHOLD 64
#define QOS_DEFAULT_FLOWCONTROL 3
#define QOS_DEFAULT_QOS_TRUST false
#define QOS_DEFAULT_COS_VALUE 0
#define QOS_PLUGIN_GLOBAL_VERSION  0

namespace DcmNs
{

    typedef enum
    {
        QOSGLOBALCREATECOSTOCOSMUTATIONMAP = 0 ,
        QOSGLOBALDELETECOSTOCOSMUTATIONMAP,
        QOSGLOBALCREATECOSTOTRAFFICCLASSMAP,
        QOSGLOBALDELETECOSTOTRAFFICCLASSMAP,
        QOSGLOBALCREATEQOSQUEUESCHEDULER,
        QOSGLOBALDELETEQOSQUEUESCHEDULER,
        QOSGLOBALCREATEQOSQUEUEMULTICASTSCHEDULER,
        QOSGLOBALDELETEQOSQUEUEMULTICASTSCHEDULER,
        QOSGLOBALCREATEQOSRCVQUEUEMULTICASTSETTING,
        QOSGLOBALDELETEQOSRCVQUEUEMULTICASTSETTING,
		CEEGLOBALCREATECEEMAP,
        CEEGLOBALDELETECEEMAP,
        CEEGLOBALCREATEPRIORITYGROUP,
        CEEGLOBALDELETEPRIORITYGROUP,
        CEEGLOBALCREATEPRIORITYMAP,
        CEEGLOBALDELETEPRIORITYMAP,
		CEEGLOBALCREATECEEREMAP,
        QOSGLOBALCREATEDSCPMARKLIST,
        QOSGLOBALDELETEDSCPMARKLIST,
        QOSGLOBALCREATEDSCPTODSCPMUTATIONMAP,
        QOSGLOBALDELETEDSCPTODSCPMUTATIONMAP,
        QOSGLOBALCREATEDSCPTOTRAFFICCLASSMAP,
        QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAP,
        QOSGLOBALCREATEDSCPTOCOSMAP,
        QOSGLOBALDELETEDSCPTOCOSMAP,
        QOSGLOBALCREATEREDPROFILE,
        QOSGLOBALDELETEREDPROFILE,
		QOSGLOBALNASCONFIG
    }QosGlobal;

	typedef enum 
	{
		MCAST_THRESHOLD = 0,
		MCAST_RATELIMIT,
		MCAST_RATE_BURST
	} McastOpcode;

	typedef enum
	{
		COS_MUTATION_MAP = 0,
		COS_TRAFFIC_CLASS_MAP,
		DSCP_MUTATION_MAP,
		DSCP_TRAFFIC_CLASS_MAP,
		DSCP_COS_MAP,
		CEE_MAP,
		SFLOW_POLICY_MAP,
		MQC_POLICY_MAP
	} QosMapType;

}
#endif                                            //QOSGLOBALTYPESH_H
