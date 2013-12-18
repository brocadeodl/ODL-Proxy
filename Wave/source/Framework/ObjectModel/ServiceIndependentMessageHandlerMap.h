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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author :  Anand Subramanian, Brian Adaniya                            *
 ***************************************************************************/

#ifndef SERVICEINDEPENDENTMESSAGEHANDLERMAP_H
#define SERVICEINDEPENDENTMESSAGEHANDLERMAP_H

#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/PrismMutex.h"
#include <map>

namespace WaveNs
{

class WaveServiceIndependentMessage;
class ManagementInterfaceServiceIndependentMessage;

class ServiceIndependentMessageHandlerMap
{
    private :

    protected :
    public :
            static void         addOperationMap         (UI32 serviceId, UI32 operationCode, WaveServiceIndependentMessageHandler pMessageHandler);
            static void         addOperationMap         (UI32 serviceId, UI32 operationCode, ManagementInterfaceServiceIndependentMessageHandler pMessageHandler);
            static void         execute                 (WaveServiceIndependentMessage *pWaveServiceIndependentMessage);
            static void         execute                 (ManagementInterfaceServiceIndependentMessage *pManagementInterfaceServiceIndependentMessage);
            static void         removeOperationMap      (UI32 serviceId, UI32 operationCode);


    // Now the data members

    private :
            static map <UI32, map <UI32, WaveServiceIndependentMessageHandler> *>                   s_waveServiceIndependentMessageHandlerMap;
            static map <UI32, map <UI32, ManagementInterfaceServiceIndependentMessageHandler> *>    s_managementInterfaceServiceIndependentMessageHandlerMap;
            static PrismMutex                                                                       s_serviceIndependentMessageHandlerMapMutex;

    protected :
    public :
};

}

#endif // SERVICEINDEPENDENTMESSAGEHANDLERMAP_H
