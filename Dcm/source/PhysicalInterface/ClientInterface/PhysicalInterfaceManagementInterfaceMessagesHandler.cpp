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

/***************************************************************************
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include <iostream>
#include <strings.h>

#include "Framework/Core/Wave.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Shell/WaveCliShell.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

#include "PhysicalInterface/Local/PhysicalInterfaceMessages.h"
#include "PhysicalInterface/Local/PhysicalInterfaceLocalTypes.h"

using namespace std;
using namespace WaveNs;
using namespace DcmNs;

extern "C"
{

ResourceId clientManagementInterfaceMessageHandler (ManagementInterfaceMessage *pManagementInterfaceMessage)
{
                
    PhysicalInterfaceUpdateMessage *pPhysicalInterfaceUpdateMessage = NULL;

    trace (TRACE_LEVEL_INFO, string ("nsm Management Interface Messages Received "));

    switch(pManagementInterfaceMessage->getOperationCode())
    {
         case PHYSICAL_INTERFACE_LOCAL_UPDATE: 
            {
                trace (TRACE_LEVEL_INFO, string ("PHYSICAL_INTERFACE_LOCAL_UPDATE MESSAGE Received "));
                
                pPhysicalInterfaceUpdateMessage = dynamic_cast<PhysicalInterfaceUpdateMessage *> (pManagementInterfaceMessage);     
                    
                trace (TRACE_LEVEL_INFO, string ("111 PHYSICAL_INTERFACE_LOCAL_UPDATE MESSAGE Received "));
                if( pPhysicalInterfaceUpdateMessage != NULL)
                {
                    trace (TRACE_LEVEL_INFO, string ("111i222  PHYSICAL_INTERFACE_LOCAL_UPDATE MESSAGE Received "));

                    
                    switch(pPhysicalInterfaceUpdateMessage->getCmdCode()){
                    case PHYSICALINTERFACE_SET_MTU:
                        trace (TRACE_LEVEL_INFO, string ("PHYSICALINTERFACE_SET_MTU Cmd Received ") + pPhysicalInterfaceUpdateMessage->getMtu());
                        break;
                    case PHYSICALINTERFACE_SET_DESC:
                        trace (TRACE_LEVEL_INFO, string ("PHYSICALINTERFACE_SET_DESC Cmd Received ") + pPhysicalInterfaceUpdateMessage->getDescription());
                        break;
                    default:
                        trace (TRACE_LEVEL_INFO, string ("UNKNOWN Cmd Received "));
                    }
                 } else {
                    trace (TRACE_LEVEL_INFO, string ("33333  PHYSICAL_INTERFACE_LOCAL_UPDATE MESSAGE Received "));
                    return (WAVE_MESSAGE_SUCCESS);
                 }
                
                 
            break;
            }
        default:
            trace (TRACE_LEVEL_INFO, string ("UNKNOWN MESSAGE Received "));

    }
    return (WAVE_MESSAGE_SUCCESS);
}
    
int setManagementInterfaceMessageHandlerForClient()
{
     int status = 1 ;

     WaveUserInterfaceObjectManager::setManagementInterfaceMessageHandlerForClient (clientManagementInterfaceMessageHandler);
     return status;
}

}
