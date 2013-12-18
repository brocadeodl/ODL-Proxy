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

#include "ClientInterface/Dot1X/Dot1xUtils.h"
#include <iostream>
#include <sstream>
#include <string>
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmCStatus.h"


namespace DcmNs
{

  ResourceId Dot1xUtils::MapBackEndReturnCodeToDcmdReturnCode(int code)
  {
	switch(code)
	{
	  case AUTH_API_SET_ERROR:
		return WRC_AUTH_API_SET_ERROR;
	  case AUTH_API_SET_ERR_INVALID_VALUE:
		return WRC_AUTH_API_SET_ERR_INVALID_VALUE;
	  case AUTH_API_SET_ERR_VR_NOT_EXIST:
		return WRC_AUTH_API_SET_ERR_VR_NOT_EXIST;
	  case AUTH_API_SET_ERR_PORT_NOT_EXIST:
		return WRC_AUTH_API_SET_ERR_PORT_NOT_EXIST;
	  case AUTH_API_SET_ERR_HOST_NOT_CONFIGURED:
		return WRC_AUTH_API_SET_ERR_HOST_NOT_CONFIGURED;
	  case PORT_AUTH_EXIST:
		return WRC_PORT_AUTH_EXIST;
	  case AUTH_API_SET_ERR_IF_NOT_EXIST:
		return WRC_AUTH_API_SET_ERR_IF_NOT_EXIST;
	  case AUTH_API_SET_ERR_IF_IS_AGGREGATED:
		return WRC_AUTH_API_SET_ERR_IF_IS_AGGREGATED;
	  case AUTH_API_SET_ERR_PORT_IS_INTERNAL:
		return WRC_AUTH_API_SET_ERR_PORT_IS_INTERNAL;
	  case AUTH_API_SET_ERR_PORT_IS_DEST_MIRROR:
		return WRC_AUTH_API_SET_ERR_PORT_IS_DEST_MIRROR;

	  case MAC_AUTH_API_SET_ERR_VR_NOT_EXIST:
		return WRC_MAC_AUTH_API_SET_ERR_VR_NOT_EXIST;
	  case MAC_AUTH_EXIST:
		return WRC_MAC_AUTH_EXIST;
	  case MAC_AUTH_NOT_EXIST:
		return WRC_MAC_AUTH_NOT_EXIST;
	  case AUTH_MAC_NOT_ENABLED:
		return WRC_AUTH_MAC_NOT_ENABLED;
	  case AUTH_API_GENERIC_ERROR:
		return WRC_AUTH_API_GENERIC_ERROR;
	  case AUTH_API_READINESS_TEST_ERROR:
		return WRC_AUTH_API_READINESS_TEST_ERROR;
	  case AUTH_API_READINESS_NOT_ALLOWED:
		return WRC_AUTH_API_READINESS_NOT_ALLOWED;
	  case AUTH_API_INTF_DOWN_ERROR:
		return WRC_AUTH_API_INTF_DOWN_ERROR;
	  case AUTH_API_SET_PORT_TYPE_UNKNOWN:
		return WRC_AUTH_API_SET_PORT_TYPE_UNKNOWN;

#if 0
	  case NSM_AUTH_MAC_VLAN_NOT_FOUND:
		return WRC_NSM_AUTH_MAC_VLAN_NOT_FOUND;
	  case AUTH_API_SET_SUCCESS:
		return WRC_AUTH_API_SET_SUCCESS;

#endif

	  default:
		return WAVE_MESSAGE_SUCCESS;
	}	
  }	
}
