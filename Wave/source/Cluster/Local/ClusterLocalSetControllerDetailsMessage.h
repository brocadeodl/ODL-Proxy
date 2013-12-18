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

#ifndef CLUSTERLOCALSETCONTROLLERDETAILSMESSAGE
#define CLUSTERLOCALSETCONTROLLERDETAILSMESSAGE
#include "ManagementInterface/ManagementInterfaceMessage.h"

namespace WaveNs
{

class ClusterLocalSetControllerDetailsMessage : public ManagementInterfaceMessage
{
    private:
              void          setupAttributesForSerialization         (); 
    protected:
    public:
                            ClusterLocalSetControllerDetailsMessage (); 
              virtual      ~ClusterLocalSetControllerDetailsMessage (); 

              IpVxAddress   getControllerIpAddress                  ();
              void          setControllerIpAddress                  (const IpVxAddress &ipAddress);

              SI32          getControllerPort                       ();
              void          setControllerPort                       (const SI32 &port);
                
    //Data Members

    private:
              IpVxAddress   m_controllerIpAddress;
              SI32          m_controllerPort;
    protected:
    public:
    
};
 
}

#endif
