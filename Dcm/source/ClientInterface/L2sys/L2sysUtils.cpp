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

#include "ClientInterface/L2sys/L2sysUtils.h"
#include <iostream>
#include <sstream>
#include <string>
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"


namespace DcmNs
{

  ResourceId L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(int code)
  {
	switch(code)
	{

	  case L2SYS_DCM_SUCCESS:
		return WAVE_MESSAGE_SUCCESS;

	  case L2SYS_UNKNOWN_VLAN:
				return WRC_L2SYS_UNKNOWN_VLAN;
	  case L2SYS_NO_FORWARD_ENTRY:
				return WRC_L2SYS_NO_FORWARD_ENTRY;
	  case L2SYS_NO_BRIDGE_CONFIGURED:
				return WRC_L2SYS_NO_BRIDGE_CONFIGURED;
	  case L2SYS_NO_BRIDGE_FOUND:
				return WRC_L2SYS_NO_BRIDGE_FOUND;
	  case L2SYS_INVALID_MESSAGE:
				return WRC_L2SYS_INVALID_MESSAGE;
	  case L2SYS_NO_INTERFACE_FOUND:
				return WRC_L2SYS_NO_INTERFACE_FOUND;
	  case L2SYS_ERROR_ADDRESS_TRANSLATION:
				return WRC_L2SYS_ERROR_ADDRESS_TRANSLATION;
	  case L2SYS_UNABLE_CLEAR_MAC:
				return WRC_L2SYS_UNABLE_CLEAR_MAC;
	  case L2SYS_NO_DEFAULT_BRIDGE:
				return WRC_L2SYS_NO_DEFAULT_BRIDGE;
	  case L2SYS_NO_VALID_IFINDEX:
				return WRC_L2SYS_NO_VALID_IFINDEX;
	  case L2SYS_RESERVED_ADDRESS_NOT_ALLOWED:
				return WRC_L2SYS_RESERVED_ADDRESS_NOT_ALLOWED;
	  case L2SYS_RESERVED_BCAST_ADDRESS_NOT_ALLOWED:
				return WRC_L2SYS_RESERVED_BCAST_ADDRESS_NOT_ALLOWED;
	  case L2SYS_RESERVED_MCAST_ADDRESS_NOT_ALLOWED:
				return WRC_L2SYS_RESERVED_MCAST_ADDRESS_NOT_ALLOWED;
	  case L2SYS_INVALID_ADDRESS:
				return WRC_L2SYS_INVALID_ADDRESS;
	  case L2SYS_DUPLICATE_MAC_ADDRESS:
				return WRC_L2SYS_DUPLICATE_MAC_ADDRESS;
	  case L2SYS_FAILED_DELETE_MAC:
				return WRC_L2SYS_FAILED_DELETE_MAC;
	  case L2SYS_FAILED_ADD_MAC:
				return WRC_L2SYS_FAILED_ADD_MAC;
	  case L2SYS_DCM_MAC_ADDRESS_NOT_FOUND:
				return WRC_L2SYS_DCM_MAC_ADDRESS_NOT_FOUND;
	  case L2SYS_DCM_MAC_ADDR_NOT_ON_PORT:
				return WRC_L2SYS_DCM_MAC_ADDR_NOT_ON_PORT;
	  case L2SYS_DCM_MAC_TYPE_MISMATCH:
				return WRC_L2SYS_DCM_MAC_TYPE_MISMATCH;
	  case L2SYS_INVALID_LINECARD:
				return WRC_L2SYS_INVALID_LINECARD;
	  case L2SYS_INVALID_PORT:
				return WRC_L2SYS_INVALID_PORT;
	  case L2SYS_VLAN_NOT_EXIST:
				return WRC_L2SYS_VLAN_NOT_EXIST;
	  case L2SYS_MAC_NOT_FOUND:
				return WRC_L2SYS_MAC_NOT_FOUND;
	  case L2SYS_FAILED_DELETE_MAC_FROM_BRIDGE:
				return WRC_L2SYS_FAILED_DELETE_MAC_FROM_BRIDGE;
	  case L2SYS_FAILED_CLEAT_FORWARDING_TABLE:
				return WRC_L2SYS_FAILED_CLEAT_FORWARDING_TABLE;
	  case L2SYS_INVALID_PO_RANGE_VCS: 
	  			return WRC_L2SYS_INVALID_PO_RANGE_VCS; 
	  case L2SYS_INVALID_PO_RANGE_NON_VCS: 
				return WRC_L2SYS_INVALID_PO_RANGE_NON_VCS; 
      case L2SYS_FABRIC_FLUSH_IN_PROGRESS:
                return WRC_L2SYS_FABRIC_FLUSH_IN_PROGRESS;
	  default:
		  return WAVE_MESSAGE_SUCCESS;
	}	
  }	

  int L2sysUtils::tell_rbrid (const char *instr)
  {
    char teststr[32];
    strncpy(teststr, instr, sizeof(teststr)-1);
    teststr[31] = '\0';
    int i = 0, br_id = -1;
    char *token, *lasts;
    int id=0;
    int mapid=DcmToolKit::getLocalMappedId();
    int localId=DcmToolKit::getDomainIdFromMappedId(mapid);

    if ((token = strtok_r(teststr, "/", &lasts)) != NULL)
    {
      i++;
      br_id = atoi(token);

      while ((token = strtok_r(NULL, "/", &lasts)) != NULL)   
      {
        i++;
      }
    }

    trace (TRACE_LEVEL_INFO, string("L2sysUtils::tell_rbrid mapping id ") + br_id);
    id = DcmToolKit::getDomainIdFromMappedId(br_id);
    trace (TRACE_LEVEL_INFO, string("L2sysUtils::tell_rbrid location id ") + id);

    if (i > 2)
    {
      /* 3 tuple interface name - id is rbridge-id */
      return (id);
    } 
    else
    {
      return (localId);
    }
  }
}
