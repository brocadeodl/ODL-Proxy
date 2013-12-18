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


/*
 *  Copyright (c) 2007-2010 by Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  File name: ssm_dcmd.h
 *
 *  Description:
 *				This file contains declarations and definitions required for
 *				integrating SSM module with DCMD.
 *				
 */

#ifndef _SSM_DCMD_H_
#define _SSM_DCMD_H_

typedef enum ssm_error_code_ {

    SSM_DCM_SUCCESS = 0,
  	/* starting enum value from 100 to avoid any conflict */
  
	/* common */
	SSM_DCM_MEMORY_ALLOCATION_FAILED = 100,
    SSM_DCM_ERROR,
	SSM_DCM_UNKNOWN_INTERNAL_ERROR,
	SSM_DCM_QOS_NOT_IN_NON_CEE_MODE,
	SSM_DCM_ERR_INTERFACE_NOT_FOUND,
	SSM_NSM_INTERFACE_DOES_NOT_EXIST,
	SSM_NSM_INTERFACE_IS_DESTINATION_MIRROR_PORT,
	SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT,
	SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_OR_LAG_PORT,
	SSM_NSM_INTERFACE_IS_NOT_ACTIVE,
	SSM_NSM_INTERFACE_LAG_MEMBER_QOS_NOT_ALLOWED,
	SSM_QOS_TRUST_UNSUPPORTED,
	SSM_QOS_NO_PFC_TC_MAP_STILL_ACTIVATED,
	SSM_NSM_INTERFACE_IS_NOT_PHYSICAL_PORT_OR_TRUNK,
	SSM_QOS_MAP_NOT_FOUND,
	SSM_NSM_INTERFACE_COSTOCOSMAP_ALREADY_APPLIED,
	SSM_NSM_INTERFACE_COSTOTRAFFICCLASSMAP_ALREADY_APPLIED,
	SSM_NSM_INTERFACE_PAUSE_ALREADY_APPLIED,
	SSM_NSM_INTERFACE_PFC_ALREADY_ON_INTERFACE,
	SSM_QOS_ASYMMETRIC_PFC_NOT_SUPPORTED,
	SSM_CEE_MAP_EXIST_ON_INTERFACE,
	SSM_QOS_COMMAND_NOT_SUPPORTED_IN_VCS_NON_CEE,


    /* qos map cos-mutation */
	SSM_DCM_QOS_INVALID_MAP_NAME_PREFIX,
	SSM_DCM_QOS_COS7_MAP_NOT_ALLOWED_IN_VCS,
	SSM_DCM_QOS_COS_MAP_EXCEEDS_LIMIT,

	/* qos map cos-traffic-class */
	SSM_DCM_QOS_COS7_TC_NOT_ALLOWED_IN_VCS,
	SSM_DCM_QOS_COS_TC_EXCEEDS_LIMIT,

	/* qos scheduler */
	SSM_DCM_QOS_DWRR_WEIGHTS_SUM_NOT_100,

    /* qos CEE */
    SSM_DCM_QOS_CEE_MAP_EXCEEDS_LIMIT,
    SSM_DCM_QOS_CEE_INVALID_NAME,
    SSM_DCM_QOS_CEE_INVALID_PGID,
    SSM_DCM_QOS_CEE_PGID150_NOT_ALLOWED_IN_VCS,
    SSM_DCM_QOS_CEE_DWRR_SUM_OVER_100,
    SSM_DCM_QOS_CEE_PFC_RESTRICTION,
    SSM_DCM_QOS_CEE_REMAPPED_COS_NOT_ALLOW_PFC,
    SSM_DCM_QOS_CEE_VCS_ONLY,
    SSM_DCM_QOS_CEE_REMAPPED_COS_OUT_RANGE,

	/* QoS Interface */
	SSM_INTF_COS7_NOT_ALLOWED_IN_VCS,

    /* acl */
    SSM_DCM_ERR_ACL_EEXIST,
    SSM_DCM_ERR_ACL_INVALID_NAME,
	SSM_DCM_ERR_ACL_NOT_APPLIED,

    /* CEE Map */
    SSM_DCM_QOS_CEE_FCOE,

    /* Appm */
    SSM_DCM_ERR_APPM_PORT_PROFILE_NUMBER,
    SSM_DCM_ERR_APPM_PORT_PROFILE_CREATE_STATUS,
    SSM_DCM_ERR_APPM_PORT_PROFILE_EXIST,
    SSM_DCM_ERR_APPM_PORT_PROFILE_STATE,
    SSM_DCM_ERR_APPM_PORT_PROFILE_DELETE_STATUS,
    SSM_DCM_ERR_APPM_SEC_PROFILE_DUP_ACL,
    SSM_DCM_ERR_APPM_SEC_PROFILE_PROFILE_STATE,
    SSM_DCM_ERR_APPM_SEC_PROFILE_COMMIT_VALIDATION,
    SSM_DCM_ERR_APPM_SEC_PROFILE_APPLIED,
    SSM_DCM_ERR_APPM_SEC_PROFILE_ACL_PRESENT,
    SSM_DCM_ERR_APPM_PROFILED_PORT,
    SSM_DCM_ERR_APPM_QOS_PROFILE_CEE_MAP_EXIST,
    SSM_DCM_ERR_APPM_QOS_PROFILE_COS_MUT_MAP_EXIST,
    SSM_DCM_ERR_APPM_QOS_PROFILE_COS_TC_MAP_EXIST,
    SSM_DCM_ERR_APPM_QOS_PROFILE_MAP_NOT_APPLIED,
    SSM_DCM_ERR_APPM_QOS_PROFILE_QOS_MODE,
    SSM_DCM_ERR_APPM_QOS_PROFILE_PFC,
    SSM_DCM_ERR_APPM_QOS_PROFILE_PAUSE,
    SSM_DCM_ERR_APPM_QOS_PROFILE_SET_MAP,
    SSM_DCM_ERR_APPM_QOS_NON_DEFCFG,
    SSM_DCM_ERR_APPM_FCOE_PORT

} ssm_error_code_t;

/* SSM interface types. */
#define SSM_IF_TYPE_L3        0
#define SSM_IF_TYPE_L2        1

/*
 * This function converts internal SSM error message
 * to error codes being sent to dcmd.
 *
 * Input:
 *    ret : internal error code.
 *
 * Output:
 *    NA
 *
 * Return values:
 *    dcm Specific error code.
 */

int ssm_dcm_return(int );

#ifndef __cplusplus

/* Dcmd-Plugin: Qos Functions */
extern int execute_show_qos_command(const void *inp, void ** const out);
extern int createCosToCosMutationMap(const void *inp, void ** const out);
extern int deleteCosToCosMutationMap(const void *inp, void ** const out);
extern int createCosToTrafficClassMap(const void *inp, void ** const out);
extern int deleteCosToTrafficClassMap(const void *inp, void ** const out);
extern int createCeeMap(const void *inp, void ** const out);
extern int deleteCeeMap(const void *inp, void ** const out);
extern int createPriorityGroup(const void *inp, void ** const out);
extern int deletePriorityGroup(const void *inp, void ** const out);
extern int createPriorityMap(const void *inp, void ** const out);
extern int deletePriorityMap(const void *inp, void ** const out);
extern int configureRemapPriority(const void *inp, void ** const out);
extern int configure_mcast_setting(const void *inp, void ** const out);
extern int del_mcast_setting(const void *inp, void ** const out);
extern int create_mcast_scheduler(const void *inp, void ** const out);
extern int delete_mcast_scheduler(const void *inp, void ** const out);
extern int create_scheduler(const void *inp, void ** const out);
extern int delete_scheduler(const void *inp, void ** const out);
extern int printRcvqueueInterface(const void *inp, void **const out);
extern int printQueueInterface(const void *inp, void **const out);
extern int printFlowcontrolInterface(const void *inp, void **const out);

//Methods added to support QOS Statistics Show Commands : Begin
extern int getIngressQueueInterfaceStatus(const void *inp, void **const out);
extern int getIngressBufferInterfaceStatus(const void *inp, void **const out);
extern int getEgressQueueInterfaceStatus(const void *inp, void **const out);
extern int getEgressBufferInterfaceStatus(const void *inp, void **const out);
extern int getPLCIngressQueueInterfaceStatus(const void *inp, void **const out);
extern int getPLCIngressBufferInterfaceStatus(const void *inp, void **const out);
extern int getPLCEgressQueueInterfaceStatus(const void *inp, void **const out);
extern int getPLCEgressBufferInterfaceStatus(const void *inp, void **const out);
//Methods added to support QOS Statistics Show Commands : End


//Methods added to support QOS Hawk statistics commands : Begin
extern int getQOSChipInfo(const void *inp, void **const out);

extern int getTMCIngressQueueInterfaceStats(const void *inp, void **const out);
extern int getTMCIngressBufferInterfaceStats(const void *inp, void **const out);
extern int getTMCEgressQueueInterfaceStats(const void *inp, void **const out);
extern int getTMCEgressBufferInterfaceStats(const void *inp, void **const out);

extern int getIngressTMCBuffersInterfaceStats(const void *inp, void **const out);
extern int getIngressTMCQueuesInterfaceStats(const void *inp, void **const out);

extern int getEgressTMCBuffersInterfaceStats(const void *inp, void **const out);

extern int getVOQInterfaceStats(const void *inp, void **const out);

extern int getDropReasonInterfaceStats(const void *inp, void **const out);

//Methods added to support QOS Hawk statistics commands : End

extern int printQosInterface(const void *inp, void **const out);
extern int printRcvqueueMulticastInterface(const void *inp, void **const out);
extern int printMaps(const void *inp, void **const out);
extern int printMapNames(const void *inp, void **const out);
extern int interface_qos_config_handler(const void *inp, void ** const out);
extern int configure_interface_qos_default_cos(char *ifname, unsigned int cos_value);
extern int delete_interface_qos_default_cos(char *ifname);
extern int configure_interface_qos_trust(char *ifname);
extern int delete_interface_qos_trust (char *ifname);
extern int configure_interface_flow_control(char *ifname,
			bool_t tx_enable, bool_t rx_enable);
extern int delete_interface_qos_flow_control(char *ifname);
extern int configure_interface_cos_threshold(char *ifname,
		int cos_threshold[8]);
extern int delete_interface_cos_threshold(char *ifname);
extern int configure_interface_qos_cos_mutation(char *ifname, char *cos_to_cos_map);
extern int delete_interface_qos_cos_mutation(char *ifname);
extern int configure_interface_qos_cos_traffic_class (char *ifname,
			char *cos_traffic_class_map);
extern int delete_interface_qos_cos_traffic_class (char *ifname);
extern int delete_interface_qos_pfc_flowcontrol (char *ifname, int cos);
extern int configure_interface_qos_pfc_flowcontrol (char *ifname, int cos,
			bool_t tx_enable, bool_t rx_enable);
extern int configure_interface_qos_cee(char *ifname, char *cee_map);
extern int configure_interface_priority_tag(char *ifname, bool_t set_req);
extern int delete_interface_qos_cee(char *ifname);
extern int ssm_dcm_clear_counters_handler(const void *inp, void ** const out);
extern int mac_acl_dcm_api_clear_stats_all (void);
extern int mac_acl_dcm_api_clear_stats_by_grp (char *, char *);
extern int printDscpMaps (const void *inp, void **const out);
extern int printMapNames (const void *inp, void ** const out);
extern int configure_interface_qos_dscp_mutation(char *ifname,
		char *dscp_to_dscp_map);
extern int configure_interface_qos_dscp_traffic_class (char *ifname,
	char *dscp_traffic_class_map);
extern int configure_interface_qos_dscp_cos(char *ifname,
	char *dscp_cos_map);
extern int configure_interface_qos_dscp_trust (char *ifname);
extern int delete_interface_qos_dscp_mutation(char *ifname);
extern int delete_interface_qos_dscp_traffic_class(char *ifname);
extern int delete_interface_qos_dscp_cos(char *ifname);
extern int delete_interface_qos_dscp_trust(char *ifname);
extern int createRedProfile(const void *inp, void ** const out);
extern int deleteRedProfile(const void *inp, void ** const out);
extern int configure_interface_qos_red_profile(char *ifname, int cos,
		int red_profile);
extern int delete_interface_qos_red_profile(char *ifname, int cos);
extern int createDscpMutationMap(const void *inp, void ** const out);
extern int createDscpTrafficClassMap(const void *inp, void ** const out);
extern int createDscpCosMap(const void *inp, void ** const out);
extern int createDscpMarkList(const void *inp, void ** const out);
extern int deleteDscpMarkList(const void *inp, void ** const out);
extern int deleteDscpMutationMap(const void *inp, void ** const out);
extern int deleteDscpTrafficClassMap(const void *inp, void ** const out);
extern int deleteDscpCosMap(const void *inp, void ** const out);
extern int printRedProfiles(const void *inp, void ** const out);
extern int printRedProfileInterface(const void *inp, void ** const out);
extern int configReplayDscpCosMap(const void *inp, void ** const out);
extern int configReplayDscpMutationMap(const void *inp, void ** const out);
extern int configReplayDscpTcMap(const void *inp, void ** const out);
extern int printRedInterfaceStats (const void *inp, void ** const out);
extern int configure_interface_link_long_distance(char *ifname, unsigned int distance);
extern int delete_interface_link_long_distance(char *ifname);

/* Dcmd-Plugin: APPM Functions */
extern int ssm_appm_port_profile_handler(const void *inp, void ** const out);
extern int ssm_appm_sec_profile_handler(const void *inp, void ** const out);
extern int ssm_appm_qos_profile_handler(const void *inp, void ** const out);
extern int ssm_appm_fcoe_profile_handler(const void *inp, void ** const out);
extern int ssm_appm_profiled_port_handler(const void *inp, void ** const out);


extern int ssm_bpdu_guard_handler(const void *inp, void ** const out);
extern int ssm_show_bpdu_guard_handler(const void *inp, void ** const out);


// IPI CLI backend handles
extern int ssm_debug_debug_ssm(struct cli*, int, char**);
extern int ssm_debug_no_debug_ssm(struct cli*, int, char**);
extern int ssm_ctlpkt_debug(struct cli*, int, char**);
extern int ssm_ctlpkt_no_debug(struct cli*, int, char**);
extern int ssm_apptype_debug(struct cli*, int, char**);
extern int ssm_apptype_no_debug(struct cli*, int, char**);
extern int ssm_dnld_debug(struct cli*, int, char**);
extern int ssm_dnld_no_debug(struct cli*, int, char**);
extern int ssm_l2gid_debug(struct cli*, int, char**);
extern int ssm_l2gid_no_debug(struct cli*, int, char**);
extern int ssm_msgq_debug(struct cli*, int, char**);
extern int ssm_msgq_no_debug(struct cli*, int, char**);
extern int ssm_netlink_debug(struct cli*, int, char**);
extern int ssm_netlink_no_debug(struct cli*, int, char**);
extern int ssm_nsm_debug(struct cli*, int, char**);
extern int ssm_nsm_no_debug(struct cli*, int, char**);
extern int ssm_portMgr_debug(struct cli*, int, char**);
extern int ssm_portMgr_no_debug(struct cli*, int, char**);
extern int ssm_l2sysTcam_debug(struct cli*, int, char**);
extern int ssm_l2sysTcam_no_debug(struct cli*, int, char**);
extern int ssm_l2ctrl_debug(struct cli*, int, char**);
extern int ssm_l2ctrl_no_debug(struct cli*, int, char**);
extern int ssm_l3ctrl_debug(struct cli*, int, char**);
extern int ssm_l3ctrl_no_debug(struct cli*, int, char**);

extern int ssm_debug_show(struct cli*, int, char**);

extern int ssm_l2gid_show(struct cli*, int, char**);
extern int ssm_l2gid_show_chip_asic(struct cli*, int, char**);
extern int show_ssm_interface_config(struct cli*, int, char**);
extern int ssm_portMgr_portVlanMapAll_show(struct cli*, int, char**);
extern int ssm_apptype_show(struct cli*, int, char**);
extern int ssm_apptype_show_chip_asic(struct cli*, int, char**);
extern int ssm_ctlpkt_show(struct cli*, int, char**);
extern int ssm_ctlpkt_show_chip(struct cli*, int, char**);
extern int ssm_dnld_show(struct cli*, int, char**);
extern int ssm_dnld_show_chip_asic(struct cli*, int, char**);
extern int ssm_msgq_show(struct cli*, int, char**);
extern int ssm_l2sysTcam_show_chip_all(struct cli*, int, char**);
extern int ssm_l2sysTcam_show_chip(struct cli*, int, char**);
extern int ssm_l2sysTcam_show(struct cli*, int, char**);
extern int ssm_l2ctrlTcam_show(struct cli*, int, char**);
extern int ssm_l3ctrlTcam_show(struct cli*, int, char**);
extern int ssm_chip_show(struct cli*, int, char**);

extern int ssm_mac_access_group(struct cli*, int, char**);
extern int ssm_no_mac_access_group(struct cli*, int, char**);

extern int mac_nacl_std(struct cli*, int, char**);
extern int no_mac_nacl_std(struct cli*, int, char**);
extern int mac_nacl_ext(struct cli*, int, char**);
extern int no_mac_nacl_ext(struct cli*, int, char**);

int ssm_dcm_start();

extern int ssm_writer_no_output(struct cli* cli);
extern int ssm_bpratelimit_set_mode(uint32_t mode,uint32_t slot);
extern int ssmAcl_dcmd_handle_show_next_request(const void *inp, void ** const out);
extern int ssmAcl_dcmd_handle_show_and_clear_request(const void *inp, void ** const out);

#endif // __cplusplus
#endif /* _SSM_DCMD_H_ */




