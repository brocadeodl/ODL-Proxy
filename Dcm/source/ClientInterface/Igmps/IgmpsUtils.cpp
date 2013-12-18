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

#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"
#include <iostream>
#include <sstream>
#include <string>
#include "DcmCStatus.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "igmp_defines.h" 
#include "ClientInterface/Nsm/NsmUtils.h"

using namespace WaveNs;

namespace DcmNs
{

    int IgmpsUtils::verifyPOValue (char *argument)
    {
        char *endp;
        unsigned int poid = 0;
        ResourceId vcs_status = DcmToolKit::isVcsEnabled ();

        poid = strtoul (argument, &endp, 10);
        if (*argument == '\0' || *endp != '\0')
            return -1;

        if (WAVE_PERSISTENCE_CLUSTER_DISABLED == vcs_status) {
            if (poid < 1 || poid > DCM_PORTCHANNEL_MAX_LAGS)
                return -1;
        } else {
            if (poid < 1 || poid > DCM_PORTCHANNEL_MAX_VLAGS)
                return -1;
        }
        return 0;
    }

    int IgmpsUtils::verifyGroupAddress (const string& group_addr)
    {			
		trace(TRACE_LEVEL_INFO, string ("IgmpsUtils::verifyGroupAdress: grp_addr = ") + group_addr);
        
		if ((group_addr < IGMP_MULTICAST_BEGIN_IP) || (group_addr > IGMP_MULTICAST_END_IP))
			return -1;

        return 0;
    }
    ResourceId IgmpsUtils::IgmpsStatusReturn(PrismMessage* msg_p, DcmManagementInterfaceMessage* cl_msg_p, ResourceId status)
    {
            SI32 cl_status = cl_msg_p->getClientStatus();
           
            trace(TRACE_LEVEL_DEBUG, string ("IgmpsUtils::IgmpsStatusReturn: Status = ") +
                FrameworkToolKit::localize(status) +
                string (" : client CompletionStatus = ") +
                cl_msg_p->getCompletionStatus() +
                string (" : ") +
                FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()) +
                string (" : client ClientStatus = ") +
                cl_status + 
                string (" : ") +
                FrameworkToolKit::localize(mapBackEndErrorsToDcmdErrors(cl_msg_p->getClientStatus())));

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR,
                    string("IgmpsUtils::IgmpsStatusReturn:Resp: Client Completion Error MsgId: ") +
                    cl_msg_p->getMessageId() +
                    FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                return msg_p->getCompletionStatus();
            }

            if (cl_status < 0) {
                trace(TRACE_LEVEL_DEBUG, string("IgmpsUtils::IgmpsStatusReturn:Resp:  Client Error MsgId: ") +
                    cl_msg_p->getMessageId() +
                    FrameworkToolKit::localize(cl_msg_p->getClientStatus()));

                status = mapBackEndErrorsToDcmdErrors (cl_status);
                msg_p->setCompletionStatus(status);
                return msg_p->getCompletionStatus();
            }
        }

        if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR,
                string("IgmpsUtils::IgmpsStatusReturn:Client Message sending failed for MsgId: ") +
                cl_msg_p->getMessageId() +
                string("reason:") +  FrameworkToolKit::localize(status));
                msg_p->setCompletionStatus(status);
            return status;
        }

        trace(TRACE_LEVEL_DEBUG,
            string("IgmpsUtils::IgmpsStatusReturn:Client Message processing succesful for MsgId: ") +
            cl_msg_p->getMessageId());

        msg_p->setCompletionStatus(status);
        return status;
    }

	void IgmpsUtils::getTwoTupleNameIfVcsEnabled(const string& ifName, string& twoTupleIfName) {
		if(NsmUtils::isValidThreeTuple(ifName)) {
        	NsmUtils::getTwoTupleIfName(ifName, twoTupleIfName);
        }
        else {
        	twoTupleIfName = ifName;
        }
	}

    ResourceId IgmpsUtils::mapBackEndErrorsToDcmdErrors(int err)
    {
        if (err < 0) {
            trace(TRACE_LEVEL_DEBUG, string("Entered IgmpsUtils::mapBackEndErrorsToDcmdErrors with error value") + err);
        }

        switch(err)
        {
        case IGMP_ERR_NONE:
            return WAVE_MESSAGE_SUCCESS;
            break;
        case IGMP_ERR_GENERIC:
            return WRC_IGMP_ERR_GENERIC;
            break;
        case IGMP_ERR_INVALID_COMMAND:
            return WRC_IGMP_ERR_INVALID_COMMAND;
            break;
        case IGMP_ERR_INVALID_VALUE:
            return WRC_IGMP_ERR_INVALID_VALUE;
            break;
        case IGMP_ERR_INVALID_FLAG:
            return WRC_IGMP_ERR_INVALID_FLAG;
            break;
        case IGMP_ERR_INVALID_AF:
            return WRC_IGMP_ERR_INVALID_AF;
            break;
        case IGMP_ERR_NO_SUCH_IFF:
            return WRC_IGMP_ERR_NO_SUCH_IFF;
            break;
        case IGMP_ERR_NO_SUCH_GROUP_REC:
            return WRC_IGMP_ERR_NO_SUCH_GROUP_REC;
            break;
        case IGMP_ERR_NO_SUCH_SOURCE_REC:
            return WRC_IGMP_ERR_NO_SUCH_SOURCE_REC;
            break;
        case IGMP_ERR_NO_SUCH_SVC_REG:
            return WRC_IGMP_ERR_NO_SUCH_SVC_REG;
            break;
        case IGMP_ERR_NO_CONTEXT_INFO:
            return WRC_IGMP_ERR_NO_CONTEXT_INFO;
            break;
        case IGMP_ERR_NO_VALID_CONFIG:
            return WRC_IGMP_ERR_NO_VALID_CONFIG;
            break;
        case IGMP_ERR_NO_SUCH_VALUE:
            return WRC_IGMP_ERR_NO_SUCH_VALUE;
            break;
        case IGMP_ERR_DOOM:
            return WRC_IGMP_ERR_DOOM;
            break;
        case IGMP_ERR_OOM:
            return WRC_IGMP_ERR_OOM;
            break;
        case IGMP_ERR_SOCK_JOIN_FAIL:
            return WRC_IGMP_ERR_SOCK_JOIN_FAIL;
            break;
        case IGMP_ERR_MALFORMED_ARG:
            return WRC_IGMP_ERR_MALFORMED_ARG;
            break;
        case IGMP_ERR_QI_LE_QRI:
            return WRC_IGMP_ERR_QI_LE_QRI;
            break;
        case IGMP_ERR_QRI_GT_QI:
            return WRC_IGMP_ERR_QRI_GT_QI;
            break;
        case IGMP_ERR_MALFORMED_MSG:
            return WRC_IGMP_ERR_MALFORMED_MSG;
            break;
        case IGMP_ERR_TEMP_INTERNAL:
            return WRC_IGMP_ERR_TEMP_INTERNAL;
            break;
        case IGMP_ERR_CFG_WITH_MROUTE_PROXY:
            return WRC_IGMP_ERR_CFG_WITH_MROUTE_PROXY;
            break;
        case IGMP_ERR_CFG_FOR_PROXY_SERVICE:
            return WRC_IGMP_ERR_CFG_FOR_PROXY_SERVICE;
            break;
        case IGMP_ERR_UNINIT_WITHOUT_DEREG:
            return WRC_IGMP_ERR_UNINIT_WITHOUT_DEREG;
            break;
        case IGMP_ERR_IF_GREC_LIMIT_REACHED:
            return WRC_IGMP_ERR_IF_GREC_LIMIT_REACHED;
            break;
        case IGMP_ERR_BUF_TOO_SHORT:
            return WRC_IGMP_ERR_BUF_TOO_SHORT;
            break;
        case IGMP_ERR_L2_PHYSICAL_IF:
            return WRC_IGMP_ERR_L2_PHYSICAL_IF;
            break;
        case IGMP_ERR_L3_NON_VLAN_IF:
            return WRC_IGMP_ERR_L3_NON_VLAN_IF;
            break;
        case IGMP_ERR_UNKNOWN_MSG:
            return WRC_IGMP_ERR_UNKNOWN_MSG;
            break;
        case IGMP_ERR_L2_SOCK_FAIL:
            return WRC_IGMP_ERR_L2_SOCK_FAIL;
            break;
        case IGMP_ERR_L3_SOCK_FAIL:
            return WRC_IGMP_ERR_L3_SOCK_FAIL;
            break;
        case IGMP_ERR_SNOOP_ENABLED:
            return WRC_IGMP_ERR_SNOOP_ENABLED;
            break;
        case IGMP_ERR_IGMP_ENABLED:
            return WRC_IGMP_ERR_IGMP_ENABLED;
            break;
        case IGMP_ERR_NOT_VLAN_MEMBER:
            return WRC_IGMP_ERR_NOT_VLAN_MEMBER;
            break;
        case IGMP_ERR_SNOOPQ_ENABLED:
            return WRC_IGMP_ERR_SNOOPQ_ENABLED;
            break;
        case IGMP_ERR_MRTR_ENABLED:
            return WRC_IGMP_ERR_MRTR_ENABLED;
            break;
        case IGMP_ERR_INVALID_SOURCE_IP:
            return WRC_IGMP_ERR_INVALID_SOURCE_IP;
            break;
        case IGMP_ERR_PIM_SOCK_FAIL:
            return WRC_IGMP_ERR_PIM_SOCK_FAIL;
            break;
        case IGMP_ERR_ILL_ADD:
            return WRC_IGMP_ERR_ILL_ADD;
            break;
        case IGMP_ERR_NO_NOT_ALLOWED_ON_SEC_VLAN:
            return WRC_IGMP_ERR_NO_NOT_ALLOWED_ON_SEC_VLAN;
            break;
        case IGMP_ERR_MAX_LIMIT_REACHED:
            return WRC_IGMP_ERR_MAX_LIMIT_REACHED;
            break;
        case IGMP_ERR_GROUP_LIMIT_REACHED:
	    return WRC_IGMP_ERR_GROUP_LIMIT_REACHED;
            break;
        default:
            return WAVE_MESSAGE_SUCCESS;
            break;
        }
    }

    int IgmpsUtils::compare(const void * a, const void * b)
    {
        return ( *(uint16_t*)a - *(uint16_t*)b );
    }

}
