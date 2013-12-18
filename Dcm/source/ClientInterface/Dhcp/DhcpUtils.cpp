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

#include "ClientInterface/Dhcp/DhcpUtils.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "DcmResourceIds.h"

using namespace WaveNs;

namespace DcmNs
{

ResourceId DhcpUtils::mapBackEndErrorsToDcmdErrors(int err)
{
    switch(err)
    {
       case IPHELPER_DUP_ERR:
         return WRC_IPHELPER_DUP_ERR; 

       case IPHELPER_MEM_ERR:
         return WRC_IPHELPER_MEM_ERR; 

       case IPHELPER_ARG_ERR:
         return WRC_IPHELPER_ARG_ERR; 

       case IPHELPER_INTERNAL_ERR:
         return WRC_IPHELPER_INTERNAL_ERR; 

       case IPHELPER_DCM_MSG_ERR:
         return WRC_IPHELPER_DCM_MSG_ERR; 

       case IPHELPER_ENTRY_NOT_FOUND:
         return WRC_IPHELPER_ENTRY_NOT_FOUND; 

       default:
         return WAVE_MESSAGE_SUCCESS;
    }
}

}
