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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#include "Cluster/Local/HeartBeat/HeartBeatObjectManager.h"
#include "Cluster/Local/HeartBeat/HeartBeatTypes.h"
#include "Cluster/Local/HeartBeat/HeartBeatConfigLocalManagedObject.h"
#include "Cluster/Local/HeartBeat/HeartBeatFailureAgent.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/LocationManagement/LocationBase.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "ManagementInterface/ClientInterface/WaveClientTransportObjectManager.h"

namespace WaveNs
{

HeartBeatObjectManager::HeartBeatObjectManager ()
    : WaveLocalObjectManager ("Heart Beat Service")
{

    HeartBeatConfigLocalManagedObject heartBeatConfigLocalManagedObject    (this);
    heartBeatConfigLocalManagedObject.setupOrm (); 
    addManagedClass (HeartBeatConfigLocalManagedObject::getClassName ());

    addOperationMap (HEARTBEAT_START,      reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::startHeartBeat));
    addOperationMap (HEARTBEAT_STOP,       reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::stopHeartBeat));
    addOperationMap (HEARTBEAT_PAUSE,      reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::pauseHeartBeat));
    addOperationMap (HEARTBEAT_RESUME,     reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::resumeHeartBeat));
    addOperationMap (HEARTBEAT_CONFIG,     reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::configHeartBeat));
    addOperationMap (HEARTBEAT_REPORT,     reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::reportHeartBeat));
    addOperationMap (HEARTBEAT_GET_STAT,   reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::getHeartBeatStat));
    addOperationMap (HEARTBEAT_SHOW_STAT,  reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::showHeartBeatStat));
    addOperationMap (HEARTBEAT_DISCONNECT_FROM_NODE,  reinterpret_cast<PrismMessageHandler> (&HeartBeatObjectManager::disconnectFromNodeMessageHandler));
//	setTraceLevel (TRACE_LEVEL_UNKNOWN);
}

HeartBeatObjectManager *HeartBeatObjectManager::getInstance ()
{
    static HeartBeatObjectManager *pHeartBeatObjectManager = NULL;

    if (NULL == pHeartBeatObjectManager)
    {
        pHeartBeatObjectManager = new HeartBeatObjectManager ();
        WaveNs::prismAssert (NULL != pHeartBeatObjectManager, __FILE__, __LINE__);
    }

    return (pHeartBeatObjectManager);
}

HeartBeatObjectManager::~HeartBeatObjectManager ()
{
}

PrismServiceId HeartBeatObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *HeartBeatObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case HEARTBEAT_REPORT :
            pPrismMessage = new ReportHeartBeatMessage;
            break;

        case HEARTBEAT_START :
            pPrismMessage = new StartHeartBeatMessage;
            break;

        case HEARTBEAT_GET_STAT:
            pPrismMessage = new GetHeartBeatStatMessage;
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject  *HeartBeatObjectManager::createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((HeartBeatConfigLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new HeartBeatConfigLocalManagedObject(this);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "HeartBeatObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}

void HeartBeatObjectManager::initialize(WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();

    m_thisLocationIpAddr    = FrameworkToolKit::getThisLocationIpAddress ();
    m_thisLocationPort      = FrameworkToolKit::getThisLocationPort ();

    m_heartBeatDebug.initialize ();
}

void HeartBeatObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "HeartBeatObjectManager::install : Installing the HeartBeatConfigLocalManagedObject");
    UI32 interval = 3000;
    UI32 failureThreshold = 3;
    HeartBeatConfigLocalManagedObject *pHeartBeatConfigLocalManagedObject = NULL;

    startTransaction();
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
    
    if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
    {
        pHeartBeatConfigLocalManagedObject = new HeartBeatConfigLocalManagedObject(this, interval, failureThreshold);
    } 

    ResourceId status = commitTransaction();

    if (FRAMEWORK_SUCCESS == status)
    {
        trace (TRACE_LEVEL_INFO, "HeartBeatObjectManager::install: Installed HeartBeat Default Config Managed Object");

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "HeartBeatObjectManager::install: Failed to install HeartBeat Default Config Managed Object");
        prismAssert (false, __FILE__, __LINE__);
    }

    delete pHeartBeatConfigLocalManagedObject;
   
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

HeartBeatConfigLocalManagedObject *HeartBeatObjectManager::getConfigManagedObjectInstance () 
{
    HeartBeatConfigLocalManagedObject *pHeartBeatConfigLocalManagedObject = NULL;
    WaveManagedObjectSynchronousQueryContext synchronousQueryContext (HeartBeatConfigLocalManagedObject::getClassName());
    vector<WaveManagedObject *> *pResults = querySynchronously (&synchronousQueryContext);

    if (NULL != pResults)
    {
        UI32 numberOfResults = pResults->size ();

        if (0 == numberOfResults)
        {
            trace (TRACE_LEVEL_FATAL, "HeartBeatObjectManager::getConfigManagedObjectInstance : No Managed Objects returned from querySynchronously");
            prismAssert (false, __FILE__, __LINE__);
        }
        else
        {
            pHeartBeatConfigLocalManagedObject = dynamic_cast<HeartBeatConfigLocalManagedObject *>((*pResults)[0]);
        }
        pResults->clear ();
        delete pResults;
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "HeartBeatObjectManager::getConfigManagedObjectInstance : System failure");
        prismAssert (false, __FILE__, __LINE__);
    }

    return pHeartBeatConfigLocalManagedObject;
}

// Start heartbeat to an IP address and port.

void HeartBeatObjectManager::startHeartBeat (StartHeartBeatMessage *pMessage)
{

    if (NULL == pMessage)
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::startHeartBeat : StartHeartBeatMessage passed in is NULL.");
		prismAssert (false, __FILE__, __LINE__);
    }
    
    if (0 == pMessage->getDstPortNumber()) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::startHeartBeat : The port number provided as part of the StartHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_FAILED_TO_START_HEART_BEAT);
        reply (pMessage);     
        return;
    }

    /**
     * Defect Fix for defect 352592:
     * Do not add duplicate heartBeat entry
     */
    for(UI32 i = 0; i < m_heartBeatNodes.size(); i++)
    {
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
                trace (TRACE_LEVEL_FATAL, "HeartBeatObjectManager::startHeartBeat : Duplicate HeartBeat received for ip "+
                                        pMessage->getDstIpAddress().toString()+
                                        " Port "+pMessage->getDstPortNumber ());
                /**FIXME: We shouldn't be getting duplicate HeratBeat. This assert is for RCA if any.
                  */
                prismAssert (false, __FILE__, __LINE__);
        }
    }
			
		

    HeartBeatNodeInfo       *pHeartBeatNodeInfo = new HeartBeatNodeInfo ();

    pHeartBeatNodeInfo->m_ipAddress                 = pMessage->getDstIpAddress ();
    pHeartBeatNodeInfo->m_portNumber                = pMessage->getDstPortNumber ();

    pHeartBeatNodeInfo->m_heartBeatInterval         = pMessage->getHeartBeatInterval ();
    pHeartBeatNodeInfo->m_heartBeatFailureThreshold = pMessage->getHeartBeatFailureThreshold();

    // If the interval value and the failure threshold value has not been set, then use the default value in the HeartbeatConfigManagedObject
    if (!pHeartBeatNodeInfo->m_heartBeatInterval) 
    {
        HeartBeatConfigLocalManagedObject *pHeartBeatConfigLocalManagedObject = getConfigManagedObjectInstance();
        pHeartBeatNodeInfo->m_heartBeatInterval = pHeartBeatConfigLocalManagedObject->getDefaultHeartBeatInterval();
        delete pHeartBeatConfigLocalManagedObject;
    }
    
    if (!pHeartBeatNodeInfo->m_heartBeatFailureThreshold) 
    {
        HeartBeatConfigLocalManagedObject *pHeartBeatConfigLocalManagedObject = getConfigManagedObjectInstance();
        pHeartBeatNodeInfo->m_heartBeatFailureThreshold = pHeartBeatConfigLocalManagedObject->getDefaultHeartBeatFailureThreshold();
        delete pHeartBeatConfigLocalManagedObject;
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);

    // Keep information about all the heartbeat orginating from this node.
    m_heartBeatNodes.push_back (pHeartBeatNodeInfo);
    sendHeartBeat (pHeartBeatNodeInfo);
}

// Stop heartbeat to an IP address and port.

void HeartBeatObjectManager::stopHeartBeat (StopHeartBeatMessage *pMessage)
{
    ResourceId  timerStatus;

    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::stopHeartBeat : Entering ...");

    // Validation of the StorHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::stopHeartBeat : The port number provided as part of the StopHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    vector<HeartBeatNodeInfo *>::iterator           theIterator;
    UI32                                            i;

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        // If IP is given make sure that we stop heartbeat to this node.
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            // Stop Timer so we do not send any more heartbeat.
            timerStatus = deleteTimer (m_heartBeatNodes [i]->m_timerHandle);
            if (FRAMEWORK_TIMER_CAN_NOT_DELETE == timerStatus) {
                trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::stopHeartBeat : Failed to delete timer.");
            }
            else
            {
                m_heartBeatNodes [i]->m_timerHandle = 0;
            }

            // Delete heartbeat data structure as it is no longer needed.
            delete (m_heartBeatNodes[i]);

            theIterator =  m_heartBeatNodes.begin() + i;
            m_heartBeatNodes.erase (theIterator);
            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            
            trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::stopHeartBeat : Deleted HB...");

            reply (pMessage);
            return;
        }
    }

    // We did not find hearbeat running to this IP & port. return error.
    trace (TRACE_LEVEL_ERROR, string ("HeartBeatObjectManager::stopHeartBeat : Failed to Delete HB. Could not find HB to IP: ") + pMessage->getDstIpAddress().toString() + string (" List Size:") + m_heartBeatNodes.size ());
    reply (pMessage);
}

// Pause heartbeat for an IP and port. You can continue heartbeat to this node
// by sending resumeHeartbeat message.

void HeartBeatObjectManager::pauseHeartBeat (PauseHeartBeatMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::pauseHeartBeat : Entering ...");

    UI32    i;

    // Validation of the PauseHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::pauseHeartBeat : The port number provided as part of the PauseHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        // If IP is given make sure that we pause heartbeat to this node.

        if (((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ())))
        {
            deleteTimer (m_heartBeatNodes[i]->m_timerHandle);
            m_heartBeatNodes[i]->m_timerHandle = 0;
            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pMessage);
            return;
        }
    }

    reply (pMessage);
}

void HeartBeatObjectManager::resumeHeartBeat (ResumeHeartBeatMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::resumeHeartBeat : Entering ...");

    UI32        i;

    // Validation of the ResumeHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::resumeHeartBeat : The port number provided as part of the ResumeHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        // If IP is given make sure that we pause heartbeat to this node.
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            if (0 == m_heartBeatNodes [i]->m_timerHandle)
            {
                // Start the timer only if there is no timer running.
                startTimer (m_heartBeatNodes [i]->m_timerHandle, m_heartBeatNodes [i]->m_heartBeatInterval, reinterpret_cast<PrismTimerExpirationHandler> (&HeartBeatObjectManager::heartBeatTimerHandler), m_heartBeatNodes[i]);
            }
            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

            reply (pMessage);
            return;
        }
    }

    reply (pMessage);
}

void HeartBeatObjectManager::configHeartBeat (ConfigHeartBeatMessage *pMessage)
{
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::configHeartBeat : Entering ...");

    UI32        i;
    
    // Validation of the ResumeHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::resumeHeartBeat : The port number provided as part of the ResumeHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        // If IP is given make sure that we configure the heartbeat to this node only.
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            m_heartBeatNodes [i]->m_heartBeatInterval         = pMessage->getHeartBeatInterval ();
            m_heartBeatNodes [i]->m_heartBeatFailureThreshold = pMessage->getHeartBeatFailureThreshold();

            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

            reply (pMessage);
            return;
        }
    }

    reply (pMessage);
}

void HeartBeatObjectManager::getHeartBeatStat (GetHeartBeatStatMessage *pMessage)
{
    trace (TRACE_LEVEL_INFO, "HeartBeatObjectManager::getHeartBeatStat : Entering ...");
    UI32    i;

    // Validation of the ResumeHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::resumeHeartBeat : The port number provided as part of the ResumeHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        // If IP is given make sure that get the stat for the heartbeat to that node only.
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            pMessage->m_heartBeatSent           = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatSent;
            pMessage->m_heartBeatReplySent      = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReplySent;
            
            pMessage->m_heartBeatReceived       = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReceived;
            pMessage->m_heartBeatReplyReceived  = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReplyReceived;
            pMessage->m_lastHeartBeatReceived   = m_heartBeatNodes [i]->m_heartBeatStat.m_lastHeartBeatReceived;
            pMessage->m_lastHeartBeatReplyReceived = m_heartBeatNodes [i]->m_heartBeatStat.m_lastHeartBeatReplyReceived;

            pMessage->m_heartBeatLost           = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatLost;
            pMessage->m_heartBeatLostInRow      = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatLostInRow;
            
            pMessage->m_heartBeatDelayed        = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatDelayed;
            pMessage->m_heartBeatDuplicate      = m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatDuplicate;

            reply (pMessage);
            return;
        }
    }
    reply (pMessage);
}

void HeartBeatObjectManager::showHeartBeatStat (ShowHeartBeatStatMessage *pMessage)
{
    showStatistic (pMessage);
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

void HeartBeatObjectManager::showStatistic (ShowHeartBeatStatMessage *pMessage)
{
    UI32        i;
    
    // Validation of the ResumeHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::showStatistic : The port number provided as part of the ResumeHeartBeatMessage is not valid");
        return;
    }

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            cout << "        Node Name         :" << m_heartBeatNodes [i]->m_ipAddress.toString() << endl; 
            cout << "        Port              :" << m_heartBeatNodes [i]->m_portNumber<< endl;
            cout << "        HBSent            :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatSent << endl;
            cout << "        HBReplySent       :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReplySent << endl;
            cout << "        LastHBReceived    :" << m_heartBeatNodes [i]->m_heartBeatStat.m_lastHeartBeatReceived << endl;
            cout << "        HBReceived        :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReceived << endl;
            cout << "        LastHBReplyRcved  :" << m_heartBeatNodes [i]->m_heartBeatStat.m_lastHeartBeatReplyReceived << endl;
            cout << "        HBReplyRcved      :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReplyReceived << endl;
            cout << "        HBLost            :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatLost << endl;
            cout << "        HBLostInRow       :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatLostInRow << endl;
            cout << "        HBDelayed         :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatDelayed << endl;
            cout << "        HBDuplicate       :" << m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatDuplicate<< endl << endl;
        }
    }
}

// This function is called when a heartbeat is received for this node.

void HeartBeatObjectManager::reportHeartBeat (ReportHeartBeatMessage *pMessage)
{
    UI32        i;
   
	trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::reportHeartBeat")); 

    // Validation of the ResumeHeartBeatMessage for its IP address and Port Number
    if (pMessage->getDstPortNumber() == 0) 
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::reportHeartBeat : The port number provided as part of the ReportHeartBeatMessage is not valid");
        pMessage->setCompletionStatus (WAVE_MESSAGE_ERROR_DESTINATION_IP_ADDR_DOES_NOT_EXIST);
        reply (pMessage);
        return;
    }

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        if ((m_heartBeatNodes[i]->m_ipAddress.toString().compare(pMessage->getDstIpAddress().toString()) == 0) && 
               (m_heartBeatNodes [i]->m_portNumber == pMessage->getDstPortNumber ()))
        {
            trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::reportHeartBeat:: Heart beat received from ipaddress = ") + (m_heartBeatNodes[i]->m_ipAddress.toString()) + string (" port num = ") + m_heartBeatNodes [i]->m_portNumber + string (" Seq Num = ") + pMessage->getHeartBeatNum ());
            // If we have the node in our list update the stat.
            m_heartBeatNodes [i]->m_heartBeatStat.m_lastHeartBeatReceived = pMessage->getHeartBeatNum ();
            m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReceived++;
            m_heartBeatNodes [i]->m_heartBeatStat.m_heartBeatReplySent++;
            pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            reply (pMessage);
            return;
        }
    }

    trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::reportHeartBeat:: Heart beat received from ipaddress = ") + (pMessage->getDstIpAddress().toString()) + string (" port num = ") + pMessage->getDstPortNumber () + string (" Seq Num = ") + pMessage->getHeartBeatNum ());

    // Always reply to heartbeat regarding if we have it in our list or not.
    pMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pMessage);
}

// This function is called to send heartbeat to a node. Node details and heartbeat
// details are specified in pHeartBeatInfo.
void HeartBeatObjectManager::sendHeartBeat (HeartBeatNodeInfo *pHeartBeatInfo)
{
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::sendHeartBeat : Entering ...");

    UI32                    locationId;
    WaveMessageStatus        status       = WAVE_MESSAGE_ERROR;

    locationId = FrameworkToolKit::getLocationIdForIpAddressAndPort (pHeartBeatInfo->m_ipAddress.toString(), pHeartBeatInfo->m_portNumber);
    if (0 == locationId)
    {
        trace (TRACE_LEVEL_ERROR, string ("HeartBeatObjectManager::sendHeartBeat: Unknown destination: IP: ") + pHeartBeatInfo->m_ipAddress.toString() + string (" Port :") + pHeartBeatInfo->m_portNumber);
    }
    else
    {
        ReportHeartBeatMessage *pMessage = new ReportHeartBeatMessage ();
        pMessage->setPriority (WAVE_MESSAGE_PRIORITY_HIGH);
        pMessage->setHeartBeatNum (++(pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent));
        pMessage->setSenderIpAddress (m_thisLocationIpAddr);
        pMessage->setSenderPort (m_thisLocationPort);
        pMessage->setDstIpAddress (pHeartBeatInfo->m_ipAddress);
        pMessage->setDstPortNumber (pHeartBeatInfo->m_portNumber);

        pHeartBeatInfo->m_waitingForHeartBeatReply = 1;
    	trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::sendHeartBeat : Before Send"));
        status = send (pMessage, reinterpret_cast<PrismMessageResponseHandler> (&HeartBeatObjectManager::processHeartBeatReply), pHeartBeatInfo, 0, locationId);
    	trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::sendHeartBeat : Heart beat sent to ip address = ") + (pHeartBeatInfo->m_ipAddress.toString()) + string (" port num = ") + pHeartBeatInfo->m_portNumber + string ("Seq Num = ") + pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent);
        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace (TRACE_LEVEL_ERROR, string ("HeartBeatObjectManager::sendHeartBeat: Failed to send Heartbeat to ip address ") + (pHeartBeatInfo->m_ipAddress.toString()) + " port: " + pHeartBeatInfo->m_portNumber);
        }
    }

    // Start timer to send another heartbeat when it expires.
    // We could have used periodic timer.
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::sendHeartBeat : Before Start Timer");
    startTimer (pHeartBeatInfo->m_timerHandle, pHeartBeatInfo->m_heartBeatInterval, reinterpret_cast<PrismTimerExpirationHandler> (&HeartBeatObjectManager::heartBeatTimerHandler), pHeartBeatInfo);
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::sendHeartBeat : After Start Timer");

    trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::sendHeartBeat: Armed HeartBeat Timer ") + (pHeartBeatInfo->m_ipAddress.toString()) + " port: " + pHeartBeatInfo->m_portNumber + " TimerHandle: " + pHeartBeatInfo->m_timerHandle);
}

// This function is called when heartbeat reply is received.

void HeartBeatObjectManager::processHeartBeatReply (FrameworkStatus frameworkStatus, ReportHeartBeatMessage *pMessage, HeartBeatNodeInfo *pHeartBeatInfo)
{
    UI32    i;
    UI32    found = 0;
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::processHeartBeatReply : Entering ...");

    // If message failed do not do anything. Consider the message lost. Will deal with it within the timer process.
    if (FRAMEWORK_SUCCESS != frameworkStatus || (WAVE_MESSAGE_SUCCESS != pMessage->getCompletionStatus()))
    {
        trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::processHeartBeatReply : FrameworkStatus : ") + frameworkStatus + string (", Message Status : ") + pMessage->getCompletionStatus ());
        delete (pMessage);
        return;
    }

    if ((m_thisLocationIpAddr.toString().compare (pMessage->getSenderIpAddress ().toString()) != 0) && (m_thisLocationPort != pMessage->getSenderPort ()))
    {
        trace (TRACE_LEVEL_ERROR, string ("HeartBeatObjectManager::processHeartBeatReply: Got Reply for Msg did not send: Location Ip:  ")); 
    }

    // Make sure that we are still heartbeating to this node.
    // If heartbeat stopped after we sent this heartbeat.
    // The pHeartBeatInfo will not be in out heartbeat list.

    for (i = 0; i < m_heartBeatNodes.size (); i++)
    {
        if (m_heartBeatNodes [i] == pHeartBeatInfo)
        {
            found = 1;
        }
    }

    if (0 == found)
    {
        delete (pMessage);
        return;
    }

    if ((pHeartBeatInfo->m_ipAddress.toString().compare (pMessage->getDstIpAddress ().toString()) != 0) && (pHeartBeatInfo->m_portNumber != pMessage->getDstPortNumber ()))
    {
        trace (TRACE_LEVEL_ERROR, string ("HeartBeatObjectManager::processHeartBeatReply: Got Reply form wrong IP: dst Ip: ") + string (pMessage->getDstIpAddress().toString()) + string (" port ") + pMessage->getDstPortNumber ());
    }

    if (pMessage->getHeartBeatNum () == pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent)
    {
        if (pHeartBeatInfo->m_waitingForHeartBeatReply == 0)
        {
            pHeartBeatInfo->m_heartBeatStat.m_heartBeatDuplicate++;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::processHeartBeatReply: Heart beat reply received from ip address = ") + (pHeartBeatInfo->m_ipAddress.toString()) + string (" port num = ") + pHeartBeatInfo->m_portNumber + string ("Seq Num = ") + pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent);
            pHeartBeatInfo->m_heartBeatStat.m_heartBeatLostInRow = 0;
        }
 
        pHeartBeatInfo->m_heartBeatStat.m_lastHeartBeatReplyReceived = pMessage->getHeartBeatNum ();
        pHeartBeatInfo->m_heartBeatStat.m_heartBeatReplyReceived++;
        pHeartBeatInfo->m_waitingForHeartBeatReply   = 0;

    }
    else
    {
        if (pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent > pMessage->getHeartBeatNum ())
        {
            trace (TRACE_LEVEL_WARN, "HeartBeatObjectManager::processHeartBeatReply: Heartbeat reply delayed");
            pHeartBeatInfo->m_heartBeatStat.m_heartBeatDelayed++;
			pHeartBeatInfo->m_heartBeatStat.m_heartBeatLostInRow = 0;
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::processHeartBeatReply: getting a reply for message we did not send");
            prismAssert (false, __FILE__, __LINE__);
        }
    }

    delete (pMessage);

}

// This function is called when timer expired.
// It is used to send periodic heartbeat.

void HeartBeatObjectManager::heartBeatTimerHandler (TimerHandle timerHandle, HeartBeatNodeInfo *pHeartBeatInfo)
{
    vector<HeartBeatNodeInfo *>::iterator  heartBeatNodesIterator;
    UI32                                   i                       = 0;

    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::heartBeatTimerHandler : Entering ...");

    // check if this timer for a heartbeat that was cancelled.
    if (0 == pHeartBeatInfo->m_timerHandle)
    {
        trace (TRACE_LEVEL_ERROR, "HeartBeatObjectManager::heartBeatTimerHandler : got cb for deleted timer ...");
        return;
    }

    if (pHeartBeatInfo->m_heartBeatStat.m_lastHeartBeatReplyReceived < pHeartBeatInfo->m_heartBeatStat.m_heartBeatSent)
    {
        // lost HB
        pHeartBeatInfo->m_heartBeatStat.m_heartBeatLost++;
        pHeartBeatInfo->m_heartBeatStat.m_heartBeatLostInRow++;

        trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::heartBeatTimerHandler : Heart beat failed with the node containing Ipaddress = ") + (pHeartBeatInfo->m_ipAddress).toString () + string (" portno = ") + pHeartBeatInfo->m_portNumber + string (" and loss count = ") + pHeartBeatInfo->m_heartBeatStat.m_heartBeatLostInRow);

        if ((pHeartBeatInfo->m_heartBeatStat.m_heartBeatLostInRow >= pHeartBeatInfo->m_heartBeatFailureThreshold)) 
        {
            (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation ((pHeartBeatInfo->m_ipAddress).toString (), (SI32) (pHeartBeatInfo->m_portNumber));

            // This is where the HeartBeatFailure message should be sent to all the services as a message
            HeartBeatFailureAgent  *pHeartBeatFailureAgent = new HeartBeatFailureAgent (m_pWaveObjectManager, pHeartBeatInfo->m_ipAddress, pHeartBeatInfo->m_portNumber);

            ResourceId status = pHeartBeatFailureAgent->execute ();

            if (WAVE_MESSAGE_SUCCESS != status) 
            {
                trace (TRACE_LEVEL_ERROR, " HeartBeatObjectManager::heartBeatTimerHandler : SendHeartBeatFailure to all the services has failed");
            }

            delete pHeartBeatFailureAgent;
            
            // Clean up the failed heart beat, do not delete timer since it has already expired
            for (i = 0; i < m_heartBeatNodes.size (); i++)
            {
                if ((m_heartBeatNodes[i]->m_ipAddress.toString ().compare(pHeartBeatInfo->m_ipAddress.toString ()) == 0) &&
                    (m_heartBeatNodes[i]->m_portNumber == pHeartBeatInfo->m_portNumber))
                {
                    trace (TRACE_LEVEL_DEBUG, string ("HeartBeatObjectManager::heartBeatTimerHandler: Deleted the failed heart beat to ") + (pHeartBeatInfo->m_ipAddress.toString()) + " port: " + pHeartBeatInfo->m_portNumber);
                    delete (m_heartBeatNodes[i]);
            
                    heartBeatNodesIterator = m_heartBeatNodes.begin () + i;
                    m_heartBeatNodes.erase (heartBeatNodesIterator);

                    /*FIXME: as part of defect fix for defect 352592 we do not allow duplicate HB. We dont have to loop
                    * to search other entries matching the entry for given IP and port.
                    * If duplicate entries are allowed you should revisit this and handle the removal cleanly.
                    */
                    break;
                    
                }
            }

            return;
        }
    }

    sendHeartBeat (pHeartBeatInfo);
}

void HeartBeatObjectManager::setThisLocationIpAddr (const string &thisLocationIpAddr)
{
    m_thisLocationIpAddr = thisLocationIpAddr;
}

//Disconnect from a Node handling

void HeartBeatObjectManager::disconnectFromNodeMessageHandler(DisconnectFromNodeMessage *pDisconnectFromNodeMessage)
{

    trace (TRACE_LEVEL_DEVEL, "PrismFrameworkObjectManager:: disconnectFromNodeMessageHandler: Entering ...");

    PrismLinearSequencerStep sequencerSteps[] =
    {
    reinterpret_cast<PrismLinearSequencerStep> (&HeartBeatObjectManager::validateDisconnectFromNodeRequest),
    reinterpret_cast<PrismLinearSequencerStep> (&HeartBeatObjectManager::processDisconnectFromNodeMessage),
    reinterpret_cast<PrismLinearSequencerStep> (&HeartBeatObjectManager::prismLinearSequencerSucceededStep),
    reinterpret_cast<PrismLinearSequencerStep> (&HeartBeatObjectManager::prismLinearSequencerFailedStep),
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pDisconnectFromNodeMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
    
}

void HeartBeatObjectManager::validateDisconnectFromNodeRequest (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL, "HeartBeatObjectManager::validateDisconnectFromNodeRequest..Entering");
    /* Nothing fro now */
    DisconnectFromNodeMessage *pDisconnectFromNodeMessage = dynamic_cast<DisconnectFromNodeMessage *>(pPrismLinearSequencerContext->getPPrismMessage());
    prismAssert (NULL != pDisconnectFromNodeMessage, __FILE__, __LINE__);

    LocationId locationId = pDisconnectFromNodeMessage->getLocationId ();
    LocationId  thisLocationId = FrameworkToolKit::getThisLocationId ();

    if (locationId == thisLocationId) {
        trace(TRACE_LEVEL_ERROR, "HeartBeatObjectManager::validateDisconnectFromNodeRequest: Validation Failed");
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    } else {
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }

}

void HeartBeatObjectManager::processDisconnectFromNodeMessage (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL, "HeartBeatObjectManager::processDisconnectFromNodeMessage..Entering");

    DisconnectFromNodeMessage *pDisconnectFromNodeMessage = dynamic_cast<DisconnectFromNodeMessage *>(pPrismLinearSequencerContext->getPPrismMessage());
    prismAssert (NULL != pDisconnectFromNodeMessage, __FILE__, __LINE__);

    LocationId locationId = pDisconnectFromNodeMessage->getLocationId ();

    trace (TRACE_LEVEL_INFO, string ("HeartBeatObjectManager::processDisconnectFromNodeMessage::Disconnecting from Node: Loc: ") + locationId);

    (PrismFrameworkObjectManager::getInstance ())->disconnectFromLocation (locationId);

    (WaveClientTransportObjectManager::getInstance ())->replyToPendingMessagesForServer (pDisconnectFromNodeMessage->getServerIpAddress (), (SI32) pDisconnectFromNodeMessage->getServerPort ());

    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

 
}
void HeartBeatObjectManager::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{

    /**
     * Defect Fix for defect 352592:
     * As part of failovers, HB OM is shutdown but no HB entries are removed. This might cause duplicate HB to
     * get added. Though we avoid duplicate entries in startHeartBeat, cleaning the existing HB is important at
     * shutdown. This ensure, right interval and threshold for different cluster phases of the node.
     */
    ResourceId  timerStatus;
    trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::shutdown: Entering ..");
    for(UI32 i = 0; i < m_heartBeatNodes.size(); i++)
    {
        timerStatus = deleteTimer (m_heartBeatNodes [i]->m_timerHandle);
        if (FRAMEWORK_TIMER_CAN_NOT_DELETE == timerStatus) {
            trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::shutdown: Failed to delete timer. for "+
                        m_heartBeatNodes [i]->m_ipAddress.toString() +
                        "Port " + m_heartBeatNodes[i]->m_portNumber );
        }
        else
        {
            m_heartBeatNodes [i]->m_timerHandle = 0;
        }
        trace (TRACE_LEVEL_DEBUG, "HeartBeatObjectManager::shutdown: Stopped HB for Ip "+
                                  m_heartBeatNodes [i]->m_ipAddress.toString() +
                                  "Port " + m_heartBeatNodes[i]->m_portNumber );
        delete m_heartBeatNodes[i];
    }
    
    m_heartBeatNodes.clear();
    
    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}


}
