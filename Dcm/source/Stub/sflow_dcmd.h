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
 *  File name: sflow_dcmd.h
 *
 *  Description:
 *  This file contains declarations and definitions required for
 *  integrating SFLOW module with DCMD.
 *				
 */

#ifndef _SFLOW_DCMD_H_
#define _SFLOW_DCMD_H_

typedef enum sflow_error_code_ {
    /* starting enum value from 200 to avoid any conflict */
    SFLOW_DCM_SET_ERR_PORT_NOT_EXIST = 200,
    SFLOW_DCM_SET_ERR_COLLECTOR_NOT_CONFIGURED,
    SFLOW_DCM_SET_ERR_MAX_COLLECTORS_CONFIGURED
} sflow_error_code_t;
#endif /* _SFLOW_DCMD_H_ */

