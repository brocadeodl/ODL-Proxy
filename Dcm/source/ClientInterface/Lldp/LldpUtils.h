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

#ifndef LLDP_UTILS_H 
#define LLDP_UTILS_H 

#include "Framework/Types/Types.h"



#define LLDP_API_SUCCESS                              	0
#define LLDP_API_ERR_ONM_IF_NOT_EXIST                 	-1
#define LLDP_API_ERR_LLDP_IF_NOT_EXIST                	-2
#define LLDP_API_ERR_LLDP_IF_MEMORY                   	-3
#define LLDP_API_ERR_LLDP_MASTER_NOT_EXIST            	-4
#define LLDP_API_ERR_LLDP_MAC_ADDR_ERR                	-5
#define LLDP_API_ERR_LLDP_IP_ADDR_ERR                 	-6
#define LLDP_API_ERR_LLDP_REM_PORT_MAC_NOT_EXIST      	-7
#define LLDP_API_ERR_UNSUPPORTED_TLV                  	-8
#define LLDP_API_ERR_UNKNOWN_PROFILE                  	-9
#define LLDP_API_ERR_ONMD_NOT_EXIST                  	-10
#define LLDP_API_ERR_NO_MEMORY						 	-11
#define LLDP_API_ERR_DELETE_OLD_PROFILE				 	-12
#define LLDP_API_ERR_INTERFACE_NOT_ETHERNET			 	-13
#define LLDP_API_ERR_IF_NOT_UP						 	-14
#define LLDP_API_ERR_VR_NOT_UP						 	-15
#define LLDP_API_ERR_NO_PROFILE_NAME				 	-16
#define LLDP_API_ERR_LLDP_NOT_ON_DESTINATION_MIRR_PORT	-17

#define LLDP_API_NO_CHANGE                            1



using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class LldpUtils
{
    private:
        LldpUtils();
    public:
	static ResourceId MapBackEndReturnCodeToDcmdReturnCode(int code);
    static void GetSystemDescriptionName(string &sysDesc);

};


}

#endif  //LLDP_UTILS_H

