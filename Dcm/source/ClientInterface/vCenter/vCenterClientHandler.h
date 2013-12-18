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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#ifndef VCENTERMSGHANDLER_H
#define VCENTERMSGHANDLER_H
#include "ClientInterface/vCenter/vCenterMessageDef.h"

//typedef unsigned            int  UI32;

#ifdef __cplusplus
extern "C" {
#endif

    /* Client initialization related API */
    int setvCenterMsgHandlerForClient();

    /**
     * Sample Usage
     */
    void vnm_request(vnm_req_t req, uint32_t *message_id,
            vnm_resp_t **vnm_resp);

    /**
     * Internal function used by the library to fetch the Power Status
     */
    int32_t retrievePowerStatus(vnm_req_t *req, uint32_t *message_id,
            vnm_resp_t **resp);
    /**
     * Internal function used to format Error Response
     */
    vnm_resp_t * formatErrorResponse(vnm_err_code_t error);

#ifdef __cplusplus
}
#endif

#endif /* VCENTERMSGHANDLER_H */

