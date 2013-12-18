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

#include <iostream>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/vCenter/vCenterClientHandler.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/vCenter/vCenterEventMessage.h"
#include "ValClientInterface/DcmClientSynchronousConnection.h"
#include "ValClientInterface/DcmClientInterface.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C" {

    ManagementInterfaceMessage *vCenterClientMsgInstantiator(
            const UI32 &operationCode) {
        DcmManagementInterfaceMessage *pDcmManagementInterfaceMessage = NULL;

        switch (operationCode) {
            case VCENTEREVENTS:
                pDcmManagementInterfaceMessage = new vCenterEventMessage();
                break;

            default:
                trace(
                        TRACE_LEVEL_INFO,
                        string(
                                "vCenterClientMsgInstantiator : Unknown Operation Code : ")
                                + operationCode);
                pDcmManagementInterfaceMessage = NULL;
        }
        return (pDcmManagementInterfaceMessage);
    }
    int setvCenterMsgHandlerForClient() {
        int status = 1;

        WaveUserInterfaceObjectManager::setManagementInterfaceMessageInstantiatorForClient(
                vCenterClientMsgInstantiator);
        return status;
    }

    int32_t retrievePowerStatus(vnm_req_t *req, uint32_t *message_id,
            vnm_resp_t **resp) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        SI32 clientStatus = 0;
        UI32 msg_id = 0;
        //TODO clean up printf

        if (true != (DcmClientSynchronousConnection::isCApiInitialized())) {
            trace(TRACE_LEVEL_INFO,
            "Fail to DcmClientSynchronousConnection::isCApiInitialized ()\n");

            return (-1);
        }

        DcmClientSynchronousConnection *pDcmClientSynchronousConnection =
                DcmClientSynchronousConnection::getDcmClientSynchronousConnectionForDualDaemons();

        prismAssert(NULL != pDcmClientSynchronousConnection, __FILE__,
                __LINE__);

        pDcmClientSynchronousConnection->retrievePowerStatus(req, msg_id, resp);

        if (WAVE_MESSAGE_SUCCESS == status) {
            clientStatus = 0;
            //TODO : Not Required, remove malloc
            if (!message_id) {
                message_id = (uint32_t *) malloc(sizeof(uint32_t));
            }
            *message_id = msg_id;

        }
        else {
            tracePrintf(TRACE_LEVEL_INFO,
            "Fail to DcmClientSynchronousConnection::rc=%d\n", status);

            clientStatus = -1;
        }
        return clientStatus;
    }

    void vnm_request(vnm_req_t req, uint32_t *message_id,
            vnm_resp_t **vnm_resp) {
        //TODO Remove printf
        switch (req.req_type) {
            case VNM_REQ_VM_POWER_STATUS_USING_COOKIE: {
                printf("vnm_requeust  Retrieving Power Status\n");

                SI32 clientStatus = retrievePowerStatus(&req, message_id,
                        vnm_resp);

                if ((clientStatus != WAVE_MESSAGE_SUCCESS) && (!(*vnm_resp))) {
                    *vnm_resp = formatErrorResponse(
                            VNM_ERR_DCMCLIENT_CONNECTIVITY_ERROR);
                }

            }
                break;

            default:
                //Send Operation not supported for all other request
                *vnm_resp = formatErrorResponse(
                        VNM_ERR_OPERATION_NOT_SUPPORTED);

                break;
        }
    }

    vnm_resp_t * formatErrorResponse(vnm_err_code_t error) {

        vnm_resp_t *errorResp = (vnm_resp_t *) calloc(1, sizeof(vnm_resp_t));
        //errorResp->rsp_hdr.req_msg_id = -1;
        errorResp->rsp_type = VNM_RSP_FAILURE;
        errorResp->msg.num_plds = 1;

        //Return Code is the first field in all payloads
        errorResp->msg.payload.vm_pwd_pld[0].ret_code = error;
        errorResp->size = sizeof(vnm_resp_t);
        return errorResp;
    }
}
