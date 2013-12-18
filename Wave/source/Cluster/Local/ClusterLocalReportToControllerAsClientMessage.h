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

/**************************************************************************
*   Copyright (C) 2005-2014 Brocade Communications Systems, Inc.          *
*   All rights reserved.                                                  *
*   Author : Himanshu Varshney                                            *
***************************************************************************/

#ifndef CLUSTERLOCALREPORTTOCONTROLLERASCLIENTMESSAGE
#define CLUSTERLOCALREPORTTOCONTROLLERASCLIENTMESSAGE
#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class ClusterLocalReportToControllerAsClientMessage : public ManagementInterfaceMessage
{
    private:
              void          setupAttributesForSerialization ();
    protected:
    public:
                            ClusterLocalReportToControllerAsClientMessage ();
              virtual      ~ClusterLocalReportToControllerAsClientMessage ();

              IpVxAddress   getControllerIpAddress                        ();
              void          setControllerIpAddress                        (const IpVxAddress &ipAddress);

              SI32          getControllerPort                             ();
              void          setControllerPort                             (const SI32 &port);

              IpVxAddress   getControllerClientIpAddress                            ();
              void          setControllerClientIpAddress                            (const IpVxAddress &ipAddress);
          
              SI32          getControllerClientPort                       ();
              void          setControllerClientPort                       (const SI32 &port);

    //Data Members

    private:
              IpVxAddress   m_controllerIpAddress;
              SI32          m_controllerPort;
              IpVxAddress   m_controllerClientIpAddress;
              SI32          m_controllerClientPort;
    protected:
    public:
    
};
 
}


#endif

