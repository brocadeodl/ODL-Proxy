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

#ifndef DOT1X_UTILS_H 
#define DOT1X_UTILS_H 

#include <sstream>
#include <string>
#include <vector>
#include "Framework/Types/Types.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"

#define AUTH_API_SET_SUCCESS                             0
#define AUTH_API_SET_ERROR                              -1
#define AUTH_API_SET_ERR_INVALID_VALUE                  -2
#define AUTH_API_SET_ERR_VR_NOT_EXIST                   -3
#define AUTH_API_SET_ERR_PORT_NOT_EXIST                 -4
#define AUTH_API_SET_ERR_HOST_NOT_CONFIGURED            -5
#define PORT_AUTH_EXIST                                 -7
#define AUTH_API_SET_ERR_IF_NOT_EXIST                   -8
#define AUTH_API_SET_ERR_IF_IS_AGGREGATED               -9
#define AUTH_API_SET_ERR_PORT_IS_INTERNAL               -10
#define AUTH_API_SET_ERR_PORT_IS_DEST_MIRROR            -12

#define MAC_AUTH_API_SET_ERR_VR_NOT_EXIST               -6
#define MAC_AUTH_EXIST                                  -13
#define MAC_AUTH_NOT_EXIST                              -14
#define AUTH_MAC_NOT_ENABLED                            -15
#define NSM_AUTH_MAC_VLAN_NOT_FOUND                     -16

#define AUTH_API_GENERIC_ERROR                          -17
#define AUTH_API_READINESS_TEST_ERROR                   -18
#define AUTH_API_READINESS_NOT_ALLOWED                  -19
#define AUTH_API_INTF_DOWN_ERROR                        -20
#define AUTH_API_SET_PORT_TYPE_UNKNOWN                  -21


using namespace WaveNs;
using namespace std;

namespace DcmNs
{

class Dot1xUtils
{
    private:
        Dot1xUtils();
    public:
	static ResourceId MapBackEndReturnCodeToDcmdReturnCode(int code);

};


}

#endif

