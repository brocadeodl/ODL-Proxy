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
 *  File name: rmon_dcmd.h
 *
 *  Description:
 *				This file contains declarations and definitions required for
 *				integrating RMON module with DCMD.
 *				
 */

#ifndef _RMON_DCMD_H_
#define _RMON_DCMD_H_

typedef enum rmon_error_code_ {
  	/* starting enum value from 100 to avoid any conflict */
  
	/* rmon event */
	RMON_DCM_EVENT_ALREADY_EXISTS = 100,
	RMON_DCM_CANT_SET_EVENT_COMMUNITY_WORD,
	RMON_DCM_CANT_SET_EVENT_DESCRIPTION,
	RMON_DCM_CANT_SET_EVENT_OWNER_NAME,
	RMON_DCM_CANT_SET_EVENT_TYPE,
	RMON_DCM_CANT_REMOVE_EVENT,
	RMON_DCM_SUCCESS,
	RMON_DCM_EVENT_ACTIVATION_FAILED,

	/* rmon stats */
	RMON_DCM_CANT_ENABLE_STATS_ON_MIRROR_PORT,
	RMON_DCM_INTERFACE_NOT_FOUND,
	RMON_DCM_FAILED_TO_ADD_STATISTICS,
	RMON_DCM_CANT_REMOVE_COLLECTION_STATS,

	/* rmon history */
	RMON_DCM_CANT_ENABLE_HISTORY_ON_MIRROR_PORT,
	RMON_DCM_CANT_SET_HISTORY_ENTRY,
	RMON_DCM_CANT_SET_HISTORY_BUCKET,
	RMON_DCM_CANT_SET_HISTORY_INTERVAL,
	RMON_DCM_CANT_SET_HISTORY_OWNER,
	RMON_DCM_CANT_REMOVE_HISTORY_ENTRY,

	/* rmon alarm */
	RMON_DCM_INVALID_ALARM_VARIABLE,
	RMON_DCM_INVALID_ALARM_INPUT,
	RMON_DCM_INVALID_ALARM_VARIABLE_INDEX,
	RMON_DCM_CANT_REMOVE_RMON_ALARM,
	RMON_DCM_ANOTHER_ALARM_ALREADY_EXISTS

} rmon_error_code_t;
#endif /* _RMON_DCMD_H_ */

