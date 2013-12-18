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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientReceiverObjectManager.h"
#include "Regression/RegressionServiceMessages.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Core/PrismFrameworkMessages.h"
#include "Cluster/ClusterMessages.h"
#include "Framework/Core/Test/FrameworkTestabilityManagementInterfaceMessages.h"
#include "Framework/Persistence/PersistenceObjectManagerCompareDBMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerSetConfigurationValidMessage.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "SoftwareManagement/SoftwareManagementGetVersionMessage.h"
#include "Framework/Trace/TraceMessages.h"
#include "Framework/Types/UI32Range.h"
#include "Framework/DistributedLog/DistributedLogUpdateMaxLogEntriesMessage.h"
#include "Framework/DistributedLog/DistributedLogAddLogEntryMessage.h"
#include "Framework/Core/Test/FrameworkTestability4ObjectManagerGetManagedObjects.h"
#include "Cluster/Local/HeartBeat/HeartBeatMessages.h"
#include "Cluster/Local/HeartBeat/HeartBeatTypes.h"
#include "LockManagement/LockManagementObjectManagerAcquireLockMessage.h"
#include "LockManagement/LockManagementObjectManagerReleaseLockMessage.h"
#include "File/Local/Test/FileTestMessages.h"
#include "Framework/Zeroize/ZeroizeForFIPSMessage.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugParameters.h"
#include "Framework/DistributedDebugInfrastructure/DistributedDebugInfrastructureMessages.h"
#include "Framework/Persistence/PersistenceObjectManagerPrintOrmMessage.h"
#include "Framework/Persistence/PersistenceLocalObjectManagerDebugSchemaChangeMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerMessages.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "SystemManagement/WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage.h"
#include "SystemManagement/WaveSystemManagementRebuildYangInterfacesMessage.h"
#include "Framework/Persistence/PersistenceObjectManagerGetAllClassNamesMessage.h"
#include "Framework/Database/DatabaseObjectManagerGetDbAccessDetailsMessage.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/Database/DatabaseObjectManagerEmptyMessage.h"
#include "ManagementInterface/ManagementInterfaceTypes.h"
#include "ManagementInterface/ClientInterface/ManagementInterfaceClientListMessage.h"
#include "Cluster/Local/ClusterLocalSetControllerDetailsMessage.h"
#include "Framework/Database/DatabaseObjectManagerTypes.h"



namespace WaveNs
{

WaveClientSynchronousConnection::WaveClientSynchronousConnection (const string &waveClientName, const string &waveServerIpAddress, const UI32 &waveServerPort)
    : m_waveClientName      (waveClientName),
      m_waveServerIpAddress (waveServerIpAddress),
      m_waveServerPort      (waveServerPort),
      m_timeout             (0),
      m_waveServerId        (0),
      m_connectionStatus    (WAVE_MGMT_CLIENT_INTF_CONNECTION_NOT_ATTEMPTED_TO_SERVER)
{
}

WaveClientSynchronousConnection::WaveClientSynchronousConnection ()
    : m_waveClientName      (""),
      m_waveServerIpAddress (""),
      m_waveServerPort      (0),
      m_timeout             (0),
      m_waveServerId        (0),
      m_connectionStatus    (WAVE_MGMT_CLIENT_INTF_CONNECTION_NOT_ATTEMPTED_TO_SERVER)
{
}

WaveClientSynchronousConnection::~WaveClientSynchronousConnection ()
{
}

bool WaveClientSynchronousConnection::isALocalConnection () const
{
    if (("" == m_waveClientName) && ("" == m_waveServerIpAddress) && (0 == m_waveServerPort))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

UI32 WaveClientSynchronousConnection::getTimeout () const
{
    return (m_timeout);
}

void WaveClientSynchronousConnection::setTimeout (const UI32 &timeout)
{
    m_timeout = timeout;
}

string WaveClientSynchronousConnection::getWaveServerIpAddress () const
{
    return (m_waveServerIpAddress);
}

void WaveClientSynchronousConnection::setWaveServerIpAddress (const string &ServerIpAddress)
{
    m_waveServerIpAddress = ServerIpAddress;
}

WaveConnectionStatus WaveClientSynchronousConnection::connect (const string &waveClientNameExtension, const UI32 &numberOfMaxRetries)
{
    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == (getConnectionStatus ()))
    {
        return (m_connectionStatus);
    }

    if (true == (isALocalConnection ()))
    {
        m_connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;

        return (m_connectionStatus);
    }

    string waveClientNameWithoutExtension = m_waveClientName;

    m_waveClientName += waveClientNameExtension;


    trace (TRACE_LEVEL_DEBUG, string ("Management Client          Name                            : \"") + m_waveClientName + "\"");
    trace (TRACE_LEVEL_DEBUG, string ("Management Server Location Ip Address                      : \"") + m_waveServerIpAddress + "\"");
    trace (TRACE_LEVEL_DEBUG, string ("                           Port                            : \"") + m_waveServerPort + "\"");

    ResourceId           status                      = (WaveClientTransportObjectManager::getInstance ())->connectToNewManagementInterfaceServer (m_waveClientName, waveClientNameWithoutExtension, m_waveServerIpAddress, m_waveServerPort, m_waveServerId);
    WaveConnectionStatus connectionStatus;
    bool                 returnConnectionEstablished = false;
    UI32                 numberOfRetries             = 0;
    UI32                 numberOfMaxRetriesInternal  = numberOfMaxRetries * 1000;

    if (FRAMEWORK_SUCCESS == status)
    {
        connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;

        while (numberOfMaxRetriesInternal > numberOfRetries)
        {
            returnConnectionEstablished = WaveClientReceiverObjectManager::isACurrentlyConnectedLocation (m_waveServerIpAddress, m_waveServerPort);

            if (true == returnConnectionEstablished)
            {
                break;
            }

            prismUSleep (1000);

            numberOfRetries++;
        }

        if (true == returnConnectionEstablished)
        {
            connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER;
        }
        else
        {
            connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_FAILED_TO_SERVER;
			// Cleanup forward connection in case of return path connection failure
        	close ();
        }
    }
    else
    {
        connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_FAILED_TO_SERVER;
    }

	m_connectionStatus = connectionStatus;

    return (connectionStatus);
}

void WaveClientSynchronousConnection::close ()
{
    if (true == (isALocalConnection ()))
    {
        return;
    }

    bool returnConnectionEstablished = false;
    UI32 numberOfRetries             = 0;
    UI32 numberOfMaxRetries          = 15;

    (WaveClientTransportObjectManager::getInstance ())->disconnectFromServer (m_waveServerIpAddress, m_waveServerPort);

    while (numberOfMaxRetries > numberOfRetries)
    {
    	returnConnectionEstablished = WaveClientReceiverObjectManager::isACurrentlyConnectedLocation (m_waveServerIpAddress, m_waveServerPort);

        if (false == returnConnectionEstablished)
        {
        	break;
        }

        prismSleep (1);

        numberOfRetries++;
    }

    m_connectionStatus = WAVE_MGMT_CLIENT_INTF_CONNECTION_CLOSED_TO_SERVER;
}

UI32 WaveClientSynchronousConnection::getWaveServerId () const
{
    return (m_waveServerId);
}

WaveConnectionStatus WaveClientSynchronousConnection::getConnectionStatus () const
{
    return (m_connectionStatus);
}

bool WaveClientSynchronousConnection::isCurrentlyConnected () const
{
    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER == m_connectionStatus)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

WaveMessageStatus WaveClientSynchronousConnection::sendOneWayToWaveServer (ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    if (true == (isALocalConnection ()))
    {
        status = WaveObjectManagerToolKit::sendOneWay (pManagementInterfaceMessage, locationId);
    }
    else
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pManagementInterfaceMessage, locationId);
    }

    return (status);
}

WaveMessageStatus WaveClientSynchronousConnection::sendSynchronouslyToWaveServer (ManagementInterfaceMessage *pManagementInterfaceMessage, const LocationId &locationId)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    if (true == (isALocalConnection ()))
    {
        status = WaveObjectManagerToolKit::sendSynchronously (pManagementInterfaceMessage, locationId);
    }
    else
    {
        status = (WaveUserInterfaceObjectManager::getInstance ())->sendSynchronouslyToWaveServer (getWaveServerId (), pManagementInterfaceMessage, locationId);
    }

    return (status);
}


WaveMessageStatus WaveClientSynchronousConnection::sendToWaveServer (ManagementInterfaceMessage *pManagementInterfaceMessage, WaveClientMessageCallbackHandler messageCallback, void *pInputContext, UI32 timeOutInMilliSeconds, LocationId locationId, PrismElement *pPrismMessageSender)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    status = (WaveUserInterfaceObjectManager::getInstance ())->sendToWaveServer (getWaveServerId (), pManagementInterfaceMessage, messageCallback, pInputContext, timeOutInMilliSeconds, locationId, pPrismMessageSender);

    return (status);
}

WaveMessageStatus WaveClientSynchronousConnection::sendToWaveServer (ManagementInterfaceMessage *pManagementInterfaceMessage, PrismMessageResponseHandler messageCallback, PrismElement *pPrismMessageSender, void *pInputContext, UI32 timeOutInMilliSeconds)
{
    WaveMessageStatus status = WAVE_MESSAGE_ERROR;

    status = WaveObjectManagerToolKit::sendToWaveServer (getWaveServerId (), pManagementInterfaceMessage, messageCallback, pPrismMessageSender, pInputContext, timeOutInMilliSeconds);

    return (status);
}

string WaveClientSynchronousConnection::getServiceNameById (const PrismServiceId &prismServiceId) const
{
    return ((WaveClientTransportObjectManager::getInstance ())->getPrismServiceNameForServiceId (getWaveServerId (), prismServiceId));
}

PrismServiceId WaveClientSynchronousConnection::getServiceIdByName (const string &prismServiceName) const
{
    return ((WaveClientTransportObjectManager::getInstance ())->getPrismServiceIdForServiceName (getWaveServerId (), prismServiceName));
}

ResourceId WaveClientSynchronousConnection::getListOfTestServices (vector<RegressionTestEntry> &testServices)
{
    RegressionTestObjectManagerGetTestServiceEntriesMessage message;
    WaveMessageStatus                                       status;
    ResourceId                                              completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                              clientStatus     = WAVE_MESSAGE_ERROR;

    testServices.clear ();

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getListOfTestServices : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getListOfTestServices : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                UI32 numberOfTestServices = message.getNumberOfTestServiceEntries ();
                UI32 i                    = 0;

                for (i = 0; i < numberOfTestServices; i++)
                {
                    testServices.push_back (message.getTestServiceEntryAt (i));
                }
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::prepareTestForAService (const PrismServiceId &prismServiceId, const vector<string> &inputStrings )
{
    RegressionTestObjectManagerPrepareTestForAServiceMessage message (prismServiceId, inputStrings);
    WaveMessageStatus         status;
    ResourceId                completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::runTestForAService (const PrismServiceId &prismServiceId, const UI32 &numberOfTimesToRunTheTest)
{
    RegressionTestObjectManagerRunTestForAServiceMessage message            (prismServiceId, numberOfTimesToRunTheTest);
    WaveMessageStatus                                    status;
    ResourceId                                           completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                           clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::getServicesInformation (vector<PrismServiceId> &serviceIds, vector<string> &serviceNames, vector<bool> &enabledStates, vector<bool> &localServiceStates, vector<string> &cpuAffinities)
{
    FrameworkObjectManagerServiceControlListMessage message;
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getServicesInformation : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getServicesInformation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                UI32           numberOfServices = message.getNumberOfServices ();
                UI32           i                = 0;
                PrismServiceId serviceId        = 0;
                string         serviceName;
                bool           isEnabled        = false;
                bool           isLocal          = false;
                string         cpuAffinity;

                serviceIds.clear         ();
                serviceNames.clear       ();
                enabledStates.clear      ();
                localServiceStates.clear ();
                cpuAffinities.clear      ();

                for (i = 0; i < numberOfServices; i++)
                {
                    message.getServiceAtIndex (i, serviceId, serviceName, isEnabled, isLocal, cpuAffinity);

                    serviceIds.push_back         (serviceId);
                    serviceNames.push_back       (serviceName);
                    enabledStates.push_back      (isEnabled);
                    localServiceStates.push_back (isLocal);
                    cpuAffinities.push_back      (cpuAffinity);
                }
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::getClientInformation (vector<string> &serviceNames)
{
    ManagementInterfaceClientListMessage            message;
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getClientInformation : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getClientInformation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                vector<string> connectedClients = message.getConnectedClients ();
 
                for (UI32 i = 0; i < connectedClients.size(); i++)
                {
                    serviceNames.push_back  (connectedClients[i]);
                } 
                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getClientInformation : Not Connected");
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::setCpuAffinityForService (const PrismServiceId &prismServiceId, const vector<UI32> &cpuAffinityVector)
{
    FrameworkObjectManagerServiceSetCpuAffinityMessage message            (prismServiceId, cpuAffinityVector);
    WaveMessageStatus                                  status;
    ResourceId                                         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                         clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::setCpuAffinityForService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::setCpuAffinityForService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::getHeartBeatStats (IpV4Address dstIpAddress, UI16 dstPortNumber, HeartBeatStat &heartBeatStat)
{
    GetHeartBeatStatMessage                  message;
    WaveMessageStatus                        status;
    ResourceId                               completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                               clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        message.setDstIpAddress (dstIpAddress);
        message.setDstPortNumber (dstPortNumber);

        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::getHeartBeatStats : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::getHeartBeatStats : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;
                heartBeatStat.m_heartBeatSent = message.m_heartBeatSent;
                heartBeatStat.m_heartBeatReplySent = message.m_heartBeatReplySent;
                heartBeatStat.m_heartBeatReceived = message.m_heartBeatReceived;
                heartBeatStat.m_heartBeatReplyReceived = message.m_heartBeatReplyReceived;
                heartBeatStat.m_lastHeartBeatReceived = message.m_lastHeartBeatReceived;
                heartBeatStat.m_lastHeartBeatReplyReceived = message.m_lastHeartBeatReplyReceived;
                heartBeatStat.m_heartBeatLost = message.m_heartBeatLost;
                heartBeatStat.m_heartBeatLostInRow = message.m_heartBeatLostInRow;
                heartBeatStat.m_heartBeatDelayed = message.m_heartBeatDelayed;
                heartBeatStat.m_heartBeatDuplicate = message.m_heartBeatDuplicate;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}

ResourceId WaveClientSynchronousConnection::createCluster (const vector<string> &serverIpAddresses, const vector<SI32> &serverPorts)
{
    UI32 numberOfServerIpAddresses = serverIpAddresses.size ();
    UI32 numberOfServerPorts       = serverPorts.size ();

    if (numberOfServerIpAddresses != numberOfServerPorts)
    {
        return (WAVE_MESSAGE_ERROR);
    }

    ClusterObjectManagerCreateClusterMessage message            (serverIpAddresses, serverPorts);
    WaveMessageStatus                        status;
    ResourceId                               completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                               clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::createCluster : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::createCluster : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::testWaveManagementInterface ()
{
    FrameworkTestabilityManagementInterfaceMessage1 message;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testWaveManagementInterface : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testWaveManagementInterface : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface ()
{
    FrameworkTestabilityManagementInterfaceMessage2 message;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::testAsynchronousMultipleResponseWaveManagementInterface ()
{
    FrameworkTestabilityManagementInterfaceMessage3 message;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::testSendToWaveClientsManagementInterface (vector<string> arguments)
{
    FrameworkTestabilityManagementInterfaceMessage4 message;
    ResourceId                                      clientStatus     = WAVE_MESSAGE_ERROR;
    string                                          clientName;
    int i = 0;
    if (arguments.size () == 0) {
	    WaveNs:: trace (TRACE_LEVEL_ERROR, string("WaveClientSynchronousConnection::testSendToWaveClientsManagementInterface: clientName missing"));
	    return (clientStatus);
    }

    for ( i = 0; i < (int)arguments.size (); i++)
	clientName.append (arguments [i]);

    WaveNs:: trace (TRACE_LEVEL_INFO, string("WaveClientSynchronousConnection::testSendToWaveClientsManagementInterface: clientName : ") + clientName);

    message.setClientName ("Wave Client");

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
        else
        {
            clientStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != clientStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::testAsynchronousWaveManagementInterface : Message Processing failed : " + FrameworkToolKit::localize (clientStatus));
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::getVersion (string &version)
{
    SoftwareManagementGetVersionMessage message;
    WaveMessageStatus                   status;
    ResourceId                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getVersion : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getVersion : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                version = message.getVersion ();
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::debugPrint (const string &debugMessage)
{
    TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage *pmessage = new TraceObjectManagerPrintToAllWaveDebugClientSessionsMessage (debugMessage);

    ResourceId                                                 clientStatus = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        clientStatus = (WaveUserInterfaceObjectManager::getInstance ())->sendOneWayToWaveServer (getWaveServerId (), pmessage);

        if (WAVE_MESSAGE_SUCCESS != clientStatus)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::debugPrint : Sending message failed : " + FrameworkToolKit::localize (clientStatus));
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::getListOfTraceServices (vector<TraceClientId> &traceClientIDs,
                                                                    vector<TraceLevel> &traceLevels,
                                                                    vector<string> &serviceNames)
{
    TraceObjectManagerGetClientsInformationMessage          message;
    WaveMessageStatus                                       status;
    ResourceId                                              completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                              clientStatus     = WAVE_MESSAGE_ERROR;

    traceClientIDs.clear ();
    traceLevels.clear ();
    serviceNames.clear ();

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getListOfTestServices : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getListOfTestServices : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                UI32 numberOfTraceServices = message.getNumberOfTraceClients ();
                UI32 i                    = 0;

                for (i = 0; i < numberOfTraceServices; i++)
                {
                    traceClientIDs.push_back (message.getTraceClientIdAt (i));
                    traceLevels.push_back (message.getTraceLevelAt (i));
                    serviceNames.push_back (message.getServiceNameAt (i));
                }
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::setTraceLevelForAService (TraceClientId &traceClientID, TraceLevel &traceLevel)
{
    TraceObjectManagerSetClientTraceLevelMessage         message            (traceClientID, traceLevel);
    WaveMessageStatus                                    status;
    ResourceId                                           completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                           clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::runTestForAService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::setTraceLevelForAllServices (TraceLevel &traceLevel)
{
    TraceClientId                                        traceClientID    = 0;
    TraceObjectManagerSetClientTraceLevelMessage         message            (traceClientID, traceLevel, true);
    WaveMessageStatus                                    status;
    ResourceId                                           completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                           clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::setTraceLevelForAllServices : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::setTraceLevelForAllServices : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::addLogEntry (const vector<string> &arguments)
{
    WaveMessageStatus                                    status;
    ResourceId                                           completionStatus  = WAVE_MESSAGE_SUCCESS;
    ResourceId                                           clientStatus      = WAVE_MESSAGE_ERROR;
    UI32                                                 numberOfArguments = arguments.size();
    string                                               description       = "";
    string                                               username          = "root";

    trace (TRACE_LEVEL_DEVEL, "WaveClientSynchronousConnection::addLogEntry : Entering...");

    if (1 == numberOfArguments)
    {
        description = arguments[0];
    }
    else if (2 == numberOfArguments)
    {
        description = arguments[0];
        username = arguments[1];
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::addLogEntry : Invalid number of arguments.");
        return (WAVE_MESSAGE_ERROR);
    }

    DistributedLogAddLogEntryMessage         message            (DISTRIBUTED_LOG_TYPE_AUDIT_LOG, DISTRIBUTED_LOG_SUB_TYPE_COMMAND, description, username);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::addLogEntry : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::addLogEntry : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::updateMaxLogEntries (const vector<string> &arguments)
{
    WaveMessageStatus                                    status;
    ResourceId                                           completionStatus  = WAVE_MESSAGE_SUCCESS;
    ResourceId                                           clientStatus      = WAVE_MESSAGE_ERROR;
    UI32                                                 numberOfArguments = arguments.size();
    UI32                                                 maxEntries        = 10000;

    trace (TRACE_LEVEL_DEVEL, "WaveClientSynchronousConnection::updateMaxLogEntries : Entering...");

    if (1 == numberOfArguments)
    {
        maxEntries = (UI32) atoi(arguments[0].c_str());
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::updateMaxLogEntries : Invalid number of arguments.");
        return (WAVE_MESSAGE_ERROR);
    }

    DistributedLogUpdateMaxLogEntriesMessage         message            (maxEntries);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::updateMaxLogEntries : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::updateMaxLogEntries : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

ResourceId WaveClientSynchronousConnection::showManagedObjects (const string &managedObjectClassName, const string &schemaName, UI32 &numberOfManagedObjects, vector<string> &managedObjects)
{
    FrameworkTestability4ObjectManagerGetManagedObjects message            (managedObjectClassName, schemaName);
    WaveMessageStatus                                   status;
    ResourceId                                          completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                          clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::showManagedObjects : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::showManagedObjects : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                numberOfManagedObjects = message.getNumberOfManagedObjects ();
                managedObjects         = message.getManagedObjects ();

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

ResourceId WaveClientSynchronousConnection::showConfigurationByQualifiedYangPath (const string &qualifiedYangPath, const string &schemaName)
{
    WaveSystemManagementDisplayConfigurationByQualifiedYangPathMessage message            (qualifiedYangPath, schemaName);
    WaveMessageStatus                                                  status;
    ResourceId                                                         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                                         clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::showConfigurationByQualifiedYangPath : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::showConfigurationByQualifiedYangPath : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

/************************************************************************************************************
* Name        : compareDBWithPrimary ()                                                                     *
* Description : compares the databases on each nodes in a cluster that with Primary.                        *
*               It firsts check if the nodes is in cluster else returns with an error.                      *
*               Sends a message VcsNodeVerifyDBMessage which gets the updated vetors for locations          *
*               with matching, nonMatching dbs with Primary and failed nodes. Prints the status and returns *
* @param      : None                                                                                        *
* @return     : status                                                                                      *
*************************************************************************************************************/
ResourceId WaveClientSynchronousConnection::compareDBWithPrimary ()
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;


    PersistenceObjectManagerCompareDBMessage message;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::compareDBWithPrimary : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();
            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::compareDBWithPrimary : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
            }
            else
            {
                clientStatus = WAVE_MESSAGE_SUCCESS;

                vector<UI32> nonMatchingLocations = message.getNonMatchingDbLocations ();
                vector<UI32> matchingLocations    = message.getMatchingDbLocations    ();
                vector<UI32> failedLocations      = message.getFailedLocations        ();

                if ((0 == nonMatchingLocations.size ()) && (0 == failedLocations.size ()) && (1 < matchingLocations.size ()))
                {
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "All locations have matching databases with Primary");
                }
                else if (1 == matchingLocations.size ())
                {
                    tracePrintf (TRACE_LEVEL_ERROR, true, false, "No Node has matching database that with a Primary Or there is only Primary in a cluster.");
                }

                // Print the locations in each vector
                if (1 < matchingLocations.size ())
                {
                    vector<string> ipAddresses = message.getIpAndPortsForMatchingDbLocations ();
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "Nodes with matching databases with Primary :");
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "------------------------------------------------------ ");
                    for (UI32 i = 0; i < matchingLocations.size (); i++)
                    {
                       // tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "LocationId : %d", matchingLocations [i]);
                        trace (TRACE_LEVEL_SUCCESS, "IpAddress#Port : " + ipAddresses [i] + " location-id : " + matchingLocations [i]);
                    }
                    tracePrintf (TRACE_LEVEL_SUCCESS, true, false, "------------------------------------------------------ ");
                }

                if (0 < nonMatchingLocations.size ())
                {
                    vector<string> ipAddresses = message.getIpAndPortsForNonMatchingDbLocations ();
                    tracePrintf (TRACE_LEVEL_ERROR, true, false, "Follwing Nodes have different database than a Primary :");
                    tracePrintf (TRACE_LEVEL_ERROR, true, false, "------------------------------------------------------ ");
                    for (UI32 i = 0; i < nonMatchingLocations.size (); i++)
                    {
                        //tracePrintf (TRACE_LEVEL_ERROR, true, false, "LocationId : %d", nonMatchingLocations [i]);
                        trace (TRACE_LEVEL_ERROR, "IpAddress#Port : " + ipAddresses [i] + " location-id : " + nonMatchingLocations [i]);
                    }
                    tracePrintf (TRACE_LEVEL_ERROR, true, false, "------------------------------------------------------ ");
                    clientStatus = WAVE_MESSAGE_ERROR;
                }

                if (0 < failedLocations.size ())
                {
                    vector<string> ipAddresses = message.getIpAndPortsForFailedLocations ();
                    tracePrintf (TRACE_LEVEL_WARN, true, false, "Failed to compare databases of following Nodes with a Primary :");
                    tracePrintf (TRACE_LEVEL_WARN, true, false, "------------------------------------------------------ ");
                    for (UI32 i = 0; i < failedLocations.size (); i++)
                    {
                        //tracePrintf (TRACE_LEVEL_WARN, true, false, "LocationId : %d", failedLocations [i]);
                        trace (TRACE_LEVEL_WARN, "IpAddress#Port : " + ipAddresses [i] + " location-id : " + failedLocations [i]);
                    }
                    tracePrintf (TRACE_LEVEL_WARN, true, false, "------------------------------------------------------ ");
                    clientStatus = WAVE_MESSAGE_ERROR;
                }
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return (clientStatus);

}


ResourceId WaveClientSynchronousConnection::dumpMessageHistoryOfAService (const PrismServiceId &prismServiceId)
{
    WaveMessageStatus                                       status;
    ResourceId                                              completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                              clientStatus     = WAVE_MESSAGE_ERROR;
    TraceObjectManagerMessageHistoryDumpForAServiceMessage  message (prismServiceId);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::dumpMessageHistoryOfAService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_INFO, string ("WaveClientSynchronousConnection::dumpMessageHistoryOfAService : ") + message.getMessageString ());
                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::dumpMessageHistoryOfAService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                vector<string> receivedMessaageHistoryStringVector;

                receivedMessaageHistoryStringVector = message.getMessageHistoryDumpStringVector ();

                UI32 numberOfHistoryEntries = receivedMessaageHistoryStringVector.size ();
                
                if (0 == numberOfHistoryEntries)
                {
                    trace (TRACE_LEVEL_INFO, "No enteries found. Please Check if Message history dump for this service is enable or not.");
                }
                else
                {
                    for (UI32 i = 0; i < numberOfHistoryEntries; i++)
                    {
                        tracePrintf (TRACE_LEVEL_INFO, true, true, receivedMessaageHistoryStringVector[i].c_str ());
                    } 
                }

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::configMessageHistoryOfAService (const PrismServiceId &prismServiceId, bool messageHistoryState, const UI32 &messageHistorySize)
{
    WaveMessageStatus                                        status;
    ResourceId                                               completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                               clientStatus     = WAVE_MESSAGE_ERROR;

    TraceObjectManagerMessageHistoryConfigForAServiceMessage message (prismServiceId);

    message.setMessageHistoryState   (messageHistoryState);
    message.setMessageHistoryMaxSize (messageHistorySize);

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::configMessageHistoryOfAService : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, string ("WaveClientSynchronousConnection::configMessageHistoryOfAService : ") + message.getMessageString ());
                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::configMessageHistoryOfAService : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::configMessageHistoryOfAService : successfully updated Messgae History Config." );

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::acquireLock (const string &serviceString)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        LockManagementObjectManagerAcquireLockMessage message (serviceString);
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::acquireLock: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                // trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::acquireLock: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::releaseLock (const string &serviceString)
{
    WaveMessageStatus              status           = WAVE_MESSAGE_ERROR;
    ResourceId                     completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                     clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        LockManagementObjectManagerReleaseLockMessage message (serviceString);
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::releaseLock: Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::releaseLock: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::FIPSZeroize()
{

    ResourceId              completionStatus    = WAVE_MESSAGE_SUCCESS ;
    ResourceId              clientStatus        = WAVE_MESSAGE_ERROR;

    ZeroizeForFIPSMessage   message;

    if (true == (isCurrentlyConnected ()))
    {
        WaveMessageStatus status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::FIPSZeroize: Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, string ("WaveClientSynchronousConnection::FIPSZeroize: ")
                    + message.getMessageString ());
                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::FIPSZeroize: Message Processing failed : "
                    + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::FIPSZeroize: Node zeroized successfully." );

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::getDebugInformation (const string &serviceName, string &debugInformation)
{
    WaveObjectManagerGetDebugInformationMessage     message (serviceName);
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus        = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus            = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getDebugInformation : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::getDebugInformation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                debugInformation = message.getDebugInformation ();

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

ResourceId WaveClientSynchronousConnection::resetDebugInformation (const string &serviceName)
{
    WaveObjectManagerResetDebugInformationMessage   message (serviceName);
    WaveMessageStatus                               status;
    ResourceId                                      completionStatus        = WAVE_MESSAGE_SUCCESS;
    ResourceId                                      clientStatus            = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::resetDebugInformation : Sending message failed : " + FrameworkToolKit::localize (status));

            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;

                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::resetDebugInformation : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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
ResourceId  WaveClientSynchronousConnection::setPrismConfigurationValid (const bool &validity, const string schemaFile)
{
    WaveMessageStatus  status           = WAVE_MESSAGE_SUCCESS;
    ResourceId         completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId         clientStatus     = WAVE_MESSAGE_ERROR;

    PersistenceLocalObjectManagerSetConfigurationValidMessage message ( validity, schemaFile );

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::setPrismConfigurationValid : Sending message failed : " + FrameworkToolKit::localize (status));
            clientStatus = status;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::setPrismConfigurationValid : Message Processing Failed: " + FrameworkToolKit::localize (clientStatus));
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

/**
 * Name: runDebugScript
 * Description:
 *
 * @param DistributedDebugParameters: Object with embedded parameters
 *
 * @return ResourceId: success or failure
 */
ResourceId   WaveClientSynchronousConnection::runDebugScript (DistributedDebugParameters & distributedDebugParameters)
{
    string  inputStringFromScript       = distributedDebugParameters.getInputStringFromScript ();
    string  nosOrbash                   = distributedDebugParameters.getNosOrBash ();
    string  commaDelimitedLocationIds   = distributedDebugParameters.getCommaDelimitedLocationIds ();

    trace (TRACE_LEVEL_DEBUG, string ("WaveClientSynchronousConnection::runDebugScript: inputStringFromScript: ") + inputStringFromScript + ", nosOrbash: " + nosOrbash + ", commaDelimitedLocationIds: " + commaDelimitedLocationIds);

    bool    isNos                   = false;

    if(nosOrbash=="noscli")
    {
	    isNos = true;
    }
    else
    {
	    isNos = false;
    }

    RunDebugScriptMessage   message(isNos);

    if ("" == commaDelimitedLocationIds)
    {
        trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::runDebugScript: No valid locations specified to run command or script.");

        return (WAVE_MESSAGE_ERROR);
    }
    else
    {
        if ("all" == commaDelimitedLocationIds)
        {
            // Let us send to all locations
        }
        else
        {
            // We will only send to the specified locations

            UI32Range locationIdRange = UI32Range (commaDelimitedLocationIds);

            vector<LocationId> locationsToRunDebugScript;
            locationIdRange.getUI32RangeVector (locationsToRunDebugScript);

            message.setLocationsToRunDebugScript (locationsToRunDebugScript);
        }
    }


    ResourceId              completionStatus    = WAVE_MESSAGE_SUCCESS ;
    ResourceId              clientStatus        = WAVE_MESSAGE_ERROR;

    message.setScriptString(inputStringFromScript);

    if (true == (isCurrentlyConnected ()))
    {
        WaveMessageStatus status = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::runDebugScript: Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, string ("WaveClientSynchronousConnection::runDebugScript: ")
                    + message.getMessageString ());
                trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::runDebugScript: Message Processing failed : "
                    + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::runDebugScript: Debug script run successfully." );

                distributedDebugParameters.setLocationsToRunDebugScript (message.getLocationsToRunDebugScript ());
                distributedDebugParameters.setOutputStringVector (message.getOutputStringVector());

                clientStatus = WAVE_MESSAGE_SUCCESS;
            }
        }
    }
    else
    {
        clientStatus = getConnectionStatus ();
    }

    return clientStatus;
}

ResourceId WaveClientSynchronousConnection::copyFile (const string &sourceFileName, const string &destinationFileName, UI32 &copyFileFlag, const vector<string> &vectorOfDestinationIpAddressesAndPorts)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    CopyFileWaveClientMessage copyFileWaveClientMessage (sourceFileName, destinationFileName, copyFileFlag, vectorOfDestinationIpAddressesAndPorts);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&copyFileWaveClientMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::copyFile : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = copyFileWaveClientMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::copyFile : Message Processing Failed: " + FrameworkToolKit::localize (completionStatus));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::copyFileToHaPeer (const string &sourceFileName, const string &destinationFileName)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    CopyFileToHaPeerWaveClientMessage copyFileToHaPeerWaveClientMessage (sourceFileName, destinationFileName);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&copyFileToHaPeerWaveClientMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::copyFileToHaPeer : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = copyFileToHaPeerWaveClientMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::copyFileToHaPeer : Message Processing Failed: " + FrameworkToolKit::localize (completionStatus));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::printOrm (const vector<string> &arguments)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;


    if (true == (isCurrentlyConnected ()))
    {
        PersistenceObjectManagerPrintOrmMessage message (arguments);
        sendStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::printOrm : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::printOrm : Message Processing Failed: " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::printOrm : Object Relation Mapping Information:\n" + message.getOrmInfo ());
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::debugSchemaChange (const vector<string> &arguments)
{
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        PersistenceLocalObjectManagerDebugSchemaChangeMessage message (arguments);
        WaveMessageStatus sendStatus = sendSynchronouslyToWaveServer (&message);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::debugSchemaChange : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            ResourceId completionStatus = message.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_DEBUG, "WaveClientSynchronousConnection::debugSchemaChange : Message Processing Failed: " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::debugSchemaChange: Output:\n" + message.getResultPrint ());

            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::addXPathStrings (const vector<string> &xPathStrings)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerAddXPathStringsMessage persistenceObjectManagerAddXPathStringsMessage (xPathStrings);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&persistenceObjectManagerAddXPathStringsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::addXPathStrings : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = persistenceObjectManagerAddXPathStringsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveClientSynchronousConnection::addXPathStrings : Message Processing Failed: [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::deleteXPathStrings (const vector<string> &xPathStrings)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerDeleteXPathStringsMessage persistenceObjectManagerDeleteXPathStringsMessage (xPathStrings);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&persistenceObjectManagerDeleteXPathStringsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::deleteXPathStrings : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = persistenceObjectManagerDeleteXPathStringsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveClientSynchronousConnection::deleteXPathStrings : Message Processing Failed: [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::getLastUpdateTimestampsForXPathStrings (vector<string> &xPathStrings, vector<UI64> &lastUpdatedTimestamps)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage (xPathStrings);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getLastUpdateTimestampsForXPathStrings : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveClientSynchronousConnection::getLastUpdateTimestampsForXPathStrings : Message Processing Failed: [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
            }

            xPathStrings            = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getXPathStrings  ();
            lastUpdatedTimestamps   = persistenceObjectManagerGetLastUpdateTimestampsForXPathStringsMessage.getLastUpdatedTimestamps ();

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::resetLastUpdateTimestampsForXPathStrings ()
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerResetXPathStringsTimestampsMessage persistenceObjectManagerResetXPathStringsTimestampsMessage;

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&persistenceObjectManagerResetXPathStringsTimestampsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::resetLastUpdateTimestampsForXPathStrings : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = persistenceObjectManagerResetXPathStringsTimestampsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveClientSynchronousConnection::resetLastUpdateTimestampsForXPathStrings : Message Processing Failed: [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::rebuildYangInterfaces ()
{
    WaveSystemManagementRebuildYangInterfacesMessage *pMessage = new WaveSystemManagementRebuildYangInterfacesMessage ();
    WaveMessageStatus                           status;
    ResourceId                                  completionStatus = WAVE_MESSAGE_SUCCESS;
    ResourceId                                  clientStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        status = sendSynchronouslyToWaveServer (pMessage);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::rebuildYangInterfaces : Sending message failed : " + FrameworkToolKit::localize (status));
        }
        else
        {
            completionStatus = pMessage->getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                clientStatus = completionStatus;
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::rebuildYangInterfaces : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
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

    delete pMessage;
    return (clientStatus);
}

ResourceId WaveClientSynchronousConnection::getAllManagedObjectNamesAndConfigurationSegmentNames (set<string> &allManagedObjectNames, vector<string> &configurationSegmentNames, vector<string> &classNamesForConfigurationSegmentNames)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    PersistenceObjectManagerGetAllClassNamesMessage persistenceObjectManagerGetAllClassNamesMessage;

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&persistenceObjectManagerGetAllClassNamesMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getAllClassNames : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = persistenceObjectManagerGetAllClassNamesMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::getAllClassNames : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                persistenceObjectManagerGetAllClassNamesMessage.getAllClassNames                          (allManagedObjectNames);
                persistenceObjectManagerGetAllClassNamesMessage.getConfigurationSegmentNames              (configurationSegmentNames);
                persistenceObjectManagerGetAllClassNamesMessage.getClassNamesForConfigurationSegmentNames (classNamesForConfigurationSegmentNames);
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::getDatabaseAccessDetails (string &databaseName, UI32 &port)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    DatabaseObjectManagerGetDbAccessDetailsMessage databaseObjectManagerGetDbAccessDetailsMessage;

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&databaseObjectManagerGetDbAccessDetailsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getAllClassNames : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = databaseObjectManagerGetDbAccessDetailsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::getAllClassNames : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                databaseName = databaseObjectManagerGetDbAccessDetailsMessage.getDatabaseName ();
                port         = databaseObjectManagerGetDbAccessDetailsMessage.getPort         ();
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::debugChangeQuerySettings (const bool &useRecursiveQuery, const UI32 &batchSize)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    DatabaseObjectManagerDebugChangeQuerySettingsMessage databaseObjectManagerDebugChangeQuerySettingsMessage (useRecursiveQuery, batchSize);

    if (true == (isCurrentlyConnected ()))
    {
        sendStatus = sendSynchronouslyToWaveServer (&databaseObjectManagerDebugChangeQuerySettingsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::debugChangeQuerySettings : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = databaseObjectManagerDebugChangeQuerySettingsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_ERROR, string ("WaveClientSynchronousConnection::debugChangeQuerySettings : Message Processing Failed: [") + FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::getEmptyDatabaseParameter (UI32 &thresholdValue, UI32 &numberOfEntriesInDatabase)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage databaseObjectManagerEmptyDatabaseParameterConfigureMessage;

        sendStatus = sendSynchronouslyToWaveServer (&databaseObjectManagerEmptyDatabaseParameterConfigureMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::getEmptyDatabaseParameter : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = databaseObjectManagerEmptyDatabaseParameterConfigureMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::getEmptyDatabaseParameter : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
                thresholdValue            = databaseObjectManagerEmptyDatabaseParameterConfigureMessage.getEmptyDatabaseAutoDetectionThresholdValue ();
                numberOfEntriesInDatabase = databaseObjectManagerEmptyDatabaseParameterConfigureMessage.getNumberOfEntriesInDatabase ();
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);

}

ResourceId WaveClientSynchronousConnection::configureEmptyDatabaseParameter (const UI32 &thresholdValue)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ()))
    {
        DatabaseObjectManagerEmptyDatabaseParameterConfigureMessage databaseObjectManagerEmptyDatabaseParameterConfigureMessage;

        databaseObjectManagerEmptyDatabaseParameterConfigureMessage.setEmptyDatabaseAutoDetectionThresholdValue (thresholdValue);

        sendStatus = sendSynchronouslyToWaveServer (&databaseObjectManagerEmptyDatabaseParameterConfigureMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::configureEmptyDatabaseParameter : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = databaseObjectManagerEmptyDatabaseParameterConfigureMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::configureEmptyDatabaseParameter : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::emptyDatabase (const string &emptyType, string &sqlString)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;
    UI32               emptyTypeValue;

    if ("-delete" == emptyType)
    {
        emptyTypeValue = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_DELETE;
    }
    else if ("-truncate" == emptyType)
    {
        emptyTypeValue = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_TRUNCATE;
    }
    else if ("-auto" == emptyType)
    {
        emptyTypeValue = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_USING_AUTO_DETECTION;
    }
    else
    {
        emptyTypeValue = DATABASE_OBJECT_MANAGER_EMPTY_DATABASE_NOT_DEFINED;
    }

    if (true == (isCurrentlyConnected ()))
    {
        DatabaseObjectManagerEmptyMessage databaseObjectManagerEmptyMessage;

        databaseObjectManagerEmptyMessage.setDatabaseEmptyType (emptyTypeValue);
        databaseObjectManagerEmptyMessage.setReturnSqlForEmptyDatabase (true);

        sendStatus = sendSynchronouslyToWaveServer (&databaseObjectManagerEmptyMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::emptyDatabase : Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = databaseObjectManagerEmptyMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::emptyDatabase : Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }
            else
            {
               databaseObjectManagerEmptyMessage.getSqlForEmpty (sqlString);
            }

            returnStatus = completionStatus;
        }
    }
    else
    {
        returnStatus = getConnectionStatus ();
    }

    return (returnStatus);
}

ResourceId WaveClientSynchronousConnection::setControllerIpAddress (const string &ipAddress, const SI32 &port)
{
    WaveMessageStatus  sendStatus       = WAVE_MESSAGE_ERROR;
    ResourceId         completionStatus = WAVE_MESSAGE_ERROR;
    ResourceId         returnStatus     = WAVE_MESSAGE_ERROR;

    if (true == (isCurrentlyConnected ())) 
    {
        ClusterLocalSetControllerDetailsMessage clusterLocalSetControllerDetailsMessage;

        IpVxAddress ipVxAddress (ipAddress);

        clusterLocalSetControllerDetailsMessage.setControllerIpAddress (ipVxAddress);
        clusterLocalSetControllerDetailsMessage.setControllerPort (port);

        sendStatus = sendSynchronouslyToWaveServer (&clusterLocalSetControllerDetailsMessage);

        if (WAVE_MESSAGE_SUCCESS != sendStatus)
        {
            trace (TRACE_LEVEL_ERROR, "WaveClientSynchronousConnection::setControllerIpAddress: Sending message failed : " + FrameworkToolKit::localize (sendStatus));
            returnStatus = sendStatus;
        }
        else
        {
            completionStatus = clusterLocalSetControllerDetailsMessage.getCompletionStatus ();

            if (WAVE_MESSAGE_SUCCESS != completionStatus)
            {
                trace (TRACE_LEVEL_INFO, "WaveClientSynchronousConnection::setControllerIpAddress: Message Processing failed : " + FrameworkToolKit::localize (completionStatus));
            }

            returnStatus = completionStatus;
        }
    }    
    else 
    {    
        returnStatus = getConnectionStatus ();
    }    

    return (returnStatus);
}

}
