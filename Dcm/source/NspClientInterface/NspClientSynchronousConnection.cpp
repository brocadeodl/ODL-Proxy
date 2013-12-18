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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "NspClientInterface/NspClientSynchronousConnection.h"
#include "Nsp/NetworkConfiguration/Global/NetworkConfigurationCreateNetworkMessage.h"

#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"


namespace NspNs
{

NspClientSynchronousConnection::NspClientSynchronousConnection (const string &nspClientName, const string &waveServerIpAddress, const UI32 &waveServerPort)
    : WaveClientSynchronousConnection (nspClientName, waveServerIpAddress, waveServerPort)
{
}

NspClientSynchronousConnection::~NspClientSynchronousConnection ()
{
}

ResourceId NspClientSynchronousConnection::createNetwork (const string& networkName)
{
    NetworkConfigurationCreateNetworkMessage message           (networkName);
    WaveMessageStatus                        status;
    ResourceId                              completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                              clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), &message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "NspClientSynchronousConnection::createNetwork : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_ERROR, "NspClientSynchronousConnection::createNetwork : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

}
