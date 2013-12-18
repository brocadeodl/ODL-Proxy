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

#ifndef L2SYS_UTILS_H 
#define L2SYS_UTILS_H 

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"



#define    L2SYS_DCM_SUCCESS                               0
#define    L2SYS_DCM_MEMORY_ALLOCATION_FAILED				-1
#define    L2SYS_DCM_ERROR                                 -2
#define    L2SYS_UNKNOWN_VLAN                               -3
#define    L2SYS_NO_FORWARD_ENTRY                           -4
#define    L2SYS_NO_BRIDGE_CONFIGURED                       -5
#define    L2SYS_NO_BRIDGE_FOUND                            -6
#define    L2SYS_INVALID_MESSAGE                            -7
#define    L2SYS_NO_INTERFACE_FOUND                         -8
#define    L2SYS_ERROR_ADDRESS_TRANSLATION                  -9
#define    L2SYS_UNABLE_CLEAR_MAC                           -10
#define    L2SYS_NO_DEFAULT_BRIDGE                          -11
#define    L2SYS_NO_VALID_IFINDEX                           -12
#define    L2SYS_RESERVED_ADDRESS_NOT_ALLOWED               -13
#define    L2SYS_RESERVED_BCAST_ADDRESS_NOT_ALLOWED         -14
#define    L2SYS_RESERVED_MCAST_ADDRESS_NOT_ALLOWED         -15
#define    L2SYS_INVALID_ADDRESS                            -16
#define    L2SYS_DUPLICATE_MAC_ADDRESS                      -17
#define    L2SYS_FAILED_DELETE_MAC                          -18
#define    L2SYS_FAILED_ADD_MAC                             -19
#define    L2SYS_DCM_MAC_ADDRESS_NOT_FOUND                  -20
#define    L2SYS_DCM_MAC_ADDR_NOT_ON_PORT                   -21
#define    L2SYS_DCM_MAC_TYPE_MISMATCH                      -22
#define    L2SYS_INVALID_LINECARD                           -23
#define    L2SYS_INVALID_PORT                               -24
#define    L2SYS_VLAN_NOT_EXIST                             -25
#define    L2SYS_MAC_NOT_FOUND                              -26
#define    L2SYS_FAILED_DELETE_MAC_FROM_BRIDGE              -27
#define    L2SYS_FAILED_CLEAT_FORWARDING_TABLE 				-28
#define    L2SYS_INVALID_PO_RANGE_VCS                       -29
#define    L2SYS_INVALID_PO_RANGE_NON_VCS                   -30    
#define    L2SYS_FABRIC_FLUSH_IN_PROGRESS					-31



using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class L2sysUtils
{
    private:
        L2sysUtils();
    public:
	static ResourceId MapBackEndReturnCodeToDcmdReturnCode(int code);
	static int tell_rbrid (const char *instr);

};


}

#endif

