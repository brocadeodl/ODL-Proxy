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
 **************************************************************************/
/*
 * Capabilities.cpp
 *
 *  Created on: Dec 20, 2011
 *      Author: rmadapur
 */
#include "Capabilities.h"
using namespace WaveNs;

namespace DcmNs {

Capabilities::Capabilities() {
	cap = new BitMap(MAX_CAP_COUNT);
    cap->reset();
	tracePrintf(TRACE_LEVEL_INFO, "Number of bits %d ", cap->getNumberOfBits());
}

Capabilities::~Capabilities() {
	delete cap;
}

Capabilities * Capabilities::instance() {
	static Capabilities *s_instance = NULL;
	if (!s_instance) {
		s_instance = new Capabilities();
		s_instance->populateCapabilities();
	}
	return s_instance;
}

bool Capabilities::isCapabilityPresent_int(capability_t capability) {

    int index = getBitMapIndex(capability);

	if (index >= MAX_CAP_COUNT) {
		return false;
	}

	UI8 flag  = cap->getValue(index);

	return flag==1?true:false;
}

bool Capabilities::isCapabilityPresent(capability_t capability) {
	return Capabilities::instance()->isCapabilityPresent_int(capability);

}
void Capabilities::setCapability(capability_t capability, bool enable) {
	int index = getBitMapIndex(capability);

	if (index >= MAX_CAP_COUNT) {
		return;
	}
	UI8 flag = ((enable == true) ? 1 : 0);

	cap->setValue(index, flag);
}
void Capabilities::populateCapabilities() {

	int model = getSwModel();
	tracePrintf(TRACE_LEVEL_INFO, "Switch Model %d ", model);
	switch (model) {
	case SWBD1000:
		setCapability(FEAT_CAPABILITY_POLICER, true);
		setCapability(FEAT_CAPABILITY_LINECARD, true);
		setCapability(FEAT_CAPABILITY_STORMCONTROL, true);
        setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, true);
		setCapability(FEAT_CAPABILITY_192_PORT_CHANNEL, true);
		setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, true);
		setCapability(EXTENDED_QOS_STATS_SHOW, true);
		setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, true);
        setCapability(POLICER_DEBUG_COMMANDS, true);
		setCapability(PO_40G_SPEED_CAPABILITY, true);
		setCapability(PO_LOAD_BALANCE_CAPABILITY, true);
        setCapability(L3_SUPPORT_SA_MODE, true);
        setCapability(MULTI_VRF_CAPABILITY, true);
        setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, false);
	    setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, false);
		setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, true);
        setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, true);
        setCapability(QOS_RED_PROFILE_CAPABILITY, true);
	setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, true);
	setCapability(FEAT_CAPABILITY_IGMP,true);
	setCapability(IPV6_RA_GUARD, false);
		setCapability(VRRP_CAPABILITY_MULTI_MODE, true);
		setCapability(SFP_BREAKOUT_CAPABILITY, true);
		setCapability(FEAT_CAPABILITY_31_PVLAN_DOMAIN, false);
		setCapability(FEAT_CAPABILITY_1000_PVLAN_DOMAIN, true);
		setCapability(ROUTE_MAP_MATCH_ACL, true);
		setCapability(GVLAN_CAPABILITY, true);
		break;
	case SWBD1001:
		setCapability(FEAT_CAPABILITY_POLICER, true);
		setCapability(FEAT_CAPABILITY_LINECARD, true);
		setCapability(FEAT_CAPABILITY_STORMCONTROL, true);
        setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, true);
		setCapability(FEAT_CAPABILITY_384_PORT_CHANNEL, true);
		setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, true);
		setCapability(EXTENDED_QOS_STATS_SHOW, true);
		setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, true);
        setCapability(POLICER_DEBUG_COMMANDS, true);
		setCapability(PO_40G_SPEED_CAPABILITY, true);
        setCapability(PO_LOAD_BALANCE_CAPABILITY, true);
        setCapability(L3_SUPPORT_SA_MODE, true);
        setCapability(MULTI_VRF_CAPABILITY, true);
        setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, false);
	    setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, false);
	    setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, true);
        setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, true);
        setCapability(QOS_RED_PROFILE_CAPABILITY, true);
	setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, true);
	setCapability(FEAT_CAPABILITY_IGMP,true);
	setCapability(IPV6_RA_GUARD, false);
		setCapability(VRRP_CAPABILITY_MULTI_MODE, true);
		setCapability(SFP_BREAKOUT_CAPABILITY, true);
		setCapability(FEAT_CAPABILITY_31_PVLAN_DOMAIN, false);
		setCapability(FEAT_CAPABILITY_1000_PVLAN_DOMAIN, true);
		setCapability(ROUTE_MAP_MATCH_ACL, true);
		setCapability(GVLAN_CAPABILITY, true);
		break;
	case SWBD1002:
		setCapability(FEAT_CAPABILITY_POLICER, true);
		setCapability(FEAT_CAPABILITY_LINECARD, true);
		setCapability(FEAT_CAPABILITY_STORMCONTROL, true);
        setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, true);
		setCapability(FEAT_CAPABILITY_384_PORT_CHANNEL, true);
		setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, true);
		setCapability(EXTENDED_QOS_STATS_SHOW, true);
		setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, true);
        setCapability(POLICER_DEBUG_COMMANDS, true);
		setCapability(PO_40G_SPEED_CAPABILITY, true);
        setCapability(PO_LOAD_BALANCE_CAPABILITY, true);
        setCapability(L3_SUPPORT_SA_MODE, true);
        setCapability(MULTI_VRF_CAPABILITY, true);
        setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, false);
	setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, false);
	setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, true);
        setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, true);
        setCapability(QOS_RED_PROFILE_CAPABILITY, true);
	setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, true);
	setCapability(FEAT_CAPABILITY_IGMP,true);
	setCapability(IPV6_RA_GUARD, false);
		setCapability(VRRP_CAPABILITY_MULTI_MODE, true);
		setCapability(SFP_BREAKOUT_CAPABILITY, true);
		setCapability(FEAT_CAPABILITY_31_PVLAN_DOMAIN, false);
		setCapability(FEAT_CAPABILITY_1000_PVLAN_DOMAIN, true);
		setCapability(ROUTE_MAP_MATCH_ACL, true);
		setCapability(GVLAN_CAPABILITY, true);

		break;
	/**to support callisto and Carme platforms**/
	case SWBD97:
	case SWBD107:
	case SWBD116:
	case SWBD96:
	case SWBD95:
		setCapability(FEAT_CAPABILITY_31_PVLAN_DOMAIN, true);
		setCapability(FEAT_CAPABILITY_1000_PVLAN_DOMAIN, false);
		setCapability(FEAT_CAPABILITY_POLICER, false);
                setCapability(FEAT_CAPABILITY_LINECARD, false);
                setCapability(FEAT_CAPABILITY_STORMCONTROL, false);
        	setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, false);
                setCapability(FEAT_CAPABILITY_64_PORT_CHANNEL, true);
                setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, false);
                setCapability(EXTENDED_QOS_STATS_SHOW, false);
                setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, false);
        	setCapability(POLICER_DEBUG_COMMANDS, false);
                setCapability(PO_40G_SPEED_CAPABILITY, false);
        	setCapability(PO_LOAD_BALANCE_CAPABILITY, false);
        	setCapability(L3_SUPPORT_SA_MODE, false);
        	setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, true);
        	setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, true);
        	setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, true);
        	setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, true);
		setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, true);
		setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, true);
		setCapability(FEAT_CAPABILITY_IGMP,false);
		setCapability(IPV6_RA_GUARD, true);

        setCapability(MULTI_VRF_CAPABILITY, false);
        setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, false);
        setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, false);
        setCapability(QOS_RED_PROFILE_CAPABILITY, false);
			setCapability(VRRP_CAPABILITY_MULTI_MODE, true);
		setCapability(SFP_BREAKOUT_CAPABILITY, false);
		setCapability(ROUTE_MAP_MATCH_ACL, false);
		setCapability(GVLAN_CAPABILITY, false);
		break;

	/* Castor platform */
	case SWBD131:
	case SWBD137:
		setCapability(FEAT_CAPABILITY_POLICER, true);
		setCapability(FEAT_CAPABILITY_LINECARD, false);
		setCapability(FEAT_CAPABILITY_STORMCONTROL, true);
        setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, true);
		setCapability(FEAT_CAPABILITY_384_PORT_CHANNEL, true);
		setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, true);
		setCapability(EXTENDED_QOS_STATS_SHOW, true);
		setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, true);
        setCapability(POLICER_DEBUG_COMMANDS, true);
		setCapability(PO_40G_SPEED_CAPABILITY, true);
        setCapability(PO_LOAD_BALANCE_CAPABILITY, true);
        setCapability(L3_SUPPORT_SA_MODE, true);
        setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, true);
        setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, true);
	 	setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, true);
	 	setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, true);
	 	setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, true);
	 	setCapability(QOS_RED_PROFILE_CAPABILITY, true);
	    setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, true);
	    setCapability(FEAT_CAPABILITY_IGMP,true);
        setCapability(MULTI_VRF_CAPABILITY_MAX_8, true);
		setCapability(VRRP_CAPABILITY_MULTI_MODE, false);
		setCapability(SFP_BREAKOUT_CAPABILITY, true);
		setCapability(ROUTE_MAP_MATCH_ACL, true);
		setCapability(GVLAN_CAPABILITY, true);
		break;

	default:
		setCapability(FEAT_CAPABILITY_POLICER, false);
		setCapability(FEAT_CAPABILITY_LINECARD, false);
		setCapability(FEAT_CAPABILITY_STORMCONTROL, false);
        setCapability(MULTIPLE_ACL_ALLOWED_ON_A_PROFILED_PORT, false);
		setCapability(FEAT_CAPABILITY_64_PORT_CHANNEL, true);
		setCapability(FEAT_CAPABILITY_ACL_APPLIED_INTF_EGRESS, false);
		setCapability(EXTENDED_QOS_STATS_SHOW, false);
		setCapability(FEAT_CAPABILITY_MCAST_PIM_SUPPORT, false);
	        setCapability(POLICER_DEBUG_COMMANDS, false);
		setCapability(PO_40G_SPEED_CAPABILITY, false);
        setCapability(PO_LOAD_BALANCE_CAPABILITY, false);
        setCapability(L3_SUPPORT_SA_MODE, false);
        setCapability(QOS_SHOW_RCV_QUEUE_CAPABILITY, true);
        setCapability(QOS_RCV_QUEUE_MULTICAST_CAPABILITY, false);
        setCapability(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY, true);
        setCapability(QOS_RCV_QUEUE_PER_INTERFACE_CAPABILITY, true);
        setCapability(MULTI_VRF_CAPABILITY, true);
	setCapability(CHASSIS_FAN_AIRFLOW_DIRECTION, false);
	setCapability(FEAT_CAPABILITY_RSPAN_SOURCE, true);
	setCapability(FEAT_CAPABILITY_SPAN_OVER_VCS_SOURCE, true);
        setCapability(QOS_RED_PROFILE_CAPABILITY, false);
	setCapability(FEAT_CAPABILITY_IGMP_SNOOPING_RESTRICT_UNKNOWN_MCAST, false);
	setCapability(FEAT_CAPABILITY_IGMP,false);
	setCapability(IPV6_RA_GUARD, false);
		setCapability(VRRP_CAPABILITY_MULTI_MODE, true);
		setCapability(SFP_BREAKOUT_CAPABILITY, false);
		setCapability(GVLAN_CAPABILITY, false);
		break;
	}

}

int Capabilities::getSwModel() {
	return DcmToolKit::getSwBDModel();
}

int Capabilities::getBitMapIndex(capability_t capability){
	int index = capability - FEAT_CAPABILITY_INDEX -1;
	return index;
}



}

