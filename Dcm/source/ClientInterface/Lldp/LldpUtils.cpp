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

#include "ClientInterface/Lldp/LldpUtils.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"
#include <iostream>
#include <sstream>
#include <string>
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"
#include <utils/node.h>



namespace DcmNs
{

ResourceId LldpUtils::MapBackEndReturnCodeToDcmdReturnCode(int code)
{
	switch(code)
	{
        case LLDP_API_SUCCESS:
			return WAVE_MESSAGE_SUCCESS;

        case LLDP_API_ERR_ONMD_NOT_EXIST:
			return WRC_LLDP_API_ERR_ONMD_NOT_EXIST;

        case LLDP_API_ERR_LLDP_MASTER_NOT_EXIST:
			return WRC_LLDP_API_ERR_LLDP_MASTER_NOT_EXIST;

        case LLDP_API_ERR_LLDP_IF_NOT_EXIST:
			return WRC_LLDP_API_ERR_LLDP_IF_NOT_EXIST;

        case LLDP_API_ERR_ONM_IF_NOT_EXIST:
			return WRC_LLDP_API_ERR_ONM_IF_NOT_EXIST;

        case LLDP_API_ERR_IF_NOT_UP:
			return WRC_LLDP_API_ERR_IF_NOT_UP;

        case LLDP_API_ERR_VR_NOT_UP:
			return WRC_LLDP_API_ERR_VR_NOT_UP;

        case LLDP_API_ERR_LLDP_IF_MEMORY:
			return WRC_LLDP_API_ERR_LLDP_IF_MEMORY;

        case LLDP_API_ERR_NO_MEMORY:
			return WRC_LLDP_API_ERR_NO_MEMORY;

        case LLDP_API_ERR_UNSUPPORTED_TLV:
			return WRC_LLDP_API_ERR_UNSUPPORTED_TLV;

        case LLDP_API_ERR_LLDP_MAC_ADDR_ERR:
			return WRC_LLDP_API_ERR_LLDP_MAC_ADDR_ERR;

        case LLDP_API_ERR_LLDP_IP_ADDR_ERR:
			return WRC_LLDP_API_ERR_LLDP_IP_ADDR_ERR;

        case LLDP_API_ERR_DELETE_OLD_PROFILE:
			return WRC_LLDP_API_ERR_DELETE_OLD_PROFILE;

        case LLDP_API_ERR_INTERFACE_NOT_ETHERNET:
			return WRC_LLDP_API_ERR_INTERFACE_NOT_ETHERNET;

        case LLDP_API_ERR_UNKNOWN_PROFILE:
			return WRC_LLDP_API_ERR_UNKNOWN_PROFILE;
        
		case LLDP_API_ERR_NO_PROFILE_NAME:
			return WRC_LLDP_API_ERR_NO_PROFILE_NAME;

        case LLDP_API_ERR_LLDP_NOT_ON_DESTINATION_MIRR_PORT:
			return WRC_LLDP_API_ERR_LLDP_NOT_ON_DESTINATION_MIRR_PORT;

		default:
			return WAVE_MESSAGE_SUCCESS;

	}	
}	

void LldpUtils :: GetSystemDescriptionName(string &sysdescname) {
    char sysdesc[LLDP_DESCR_MAX_LEN] = "";
    if(DcmToolKit::isVcsEnabled()) {
        string name = "Brocade-VDX-VCS ";
        sysdescname = string(name+DcmToolKit::getVcsId());
    } else {
        get_system_description(sysdesc);
        sysdescname = sysdesc;
    }
}

}

