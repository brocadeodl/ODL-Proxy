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

/**
 * Copyright (C) 2007 - Brocade Communications Systems, Inc.
 * All rights reserved
 */

#ifndef _NSM_DCM_H
#define _NSM_DCM_H_

/*
 * Error codes for NSM_APPM - DCM interface.
 */
typedef enum
{
    NSM_DCM_SUCCESS = 0,
    NSM_DCM_FAILURE,
    NSM_ERR_DCM_VLAN_GENERAL,
    NSM_ERR_DCM_INVALID_VLAN,
    NSM_ERR_DCM_APPM_PORT_PROFILE_NUMBER,
    NSM_ERR_DCM_APPM_PORT_PROFILE_CREATE_STATUS,
    NSM_ERR_DCM_APPM_PORT_PROFILE_EXIST,
    NSM_ERR_DCM_APPM_PORT_PROFILE_STATE,
    NSM_ERR_DCM_APPM_PORT_PROFILE_DELETE_STATUS,
    NSM_ERR_DCM_APPM_VLAN_PROFILE_MODE_INVALID,
    NSM_ERR_DCM_APPM_VLAN_PROFILE_ACCESS_MODE,
    NSM_ERR_DCM_APPM_VLAN_PROFILE_TRUNK_MODE,
    NSM_ERR_DCM_APPM_VLAN_PROFILE_INPUT,
    NSM_ERR_DCM_APPM_NO_PROFILE_MAC,
    NSM_ERR_DCM_APPM_MAC_ASSOC,
    NSM_ERR_DCM_APPM_MAC_NO_ASSOC,
    NSM_ERR_DCM_APPM_PROFILE_GROUP_OVERLAP,
    NSM_ERR_DCM_APPM_PORT_NOT_PROFILED,
    NSM_ERR_DCM_APPM_PORT_PROFILE_NOT_ACTIVE,
    NSM_ERR_DCM_MCAST_MAC,
    NSM_ERR_DCM_INVALID_MAC,
    NSM_ERR_DCM_FPMA_MAC,
	NSM_ERR_DCM_ACTIVE_PROFILES_ON_PORT
} nsm_dcm_err;

/*
 * This function converts internal NSM error message 
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

int nsm_dcm_return (int ret);
int dcm_map(int code);
#endif

