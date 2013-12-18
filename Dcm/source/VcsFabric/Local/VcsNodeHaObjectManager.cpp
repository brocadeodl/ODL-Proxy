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
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "VcsFabric/Local/VcsNodeHaObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/VcsRemoteEnqSwitchMessage.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"

#include "hasm/hasm.h"
#include "Cluster/PrismCluster.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Core/FrameworkObjectManagerMessages.h"
#include "fabric/fabric.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Core/PrismFrameworkObjectManagerHaSyncWorker.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Shell/ShellDebug.h"
#include "Shell/ShellPrism.h"

#include "sys/fabos/fabobj.h"
#include "DcmResourceIds.h"

using namespace std;

namespace DcmNs
{
UI32  g_maxTryCount = 5;    // max tries for sendCpState message to apps

VcsNodeHaObjectManager::VcsNodeHaObjectManager (): WaveLocalObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());
    
    m_bootComplete = false;
    m_receiveHbTimerRunningFlag = false;
    m_sendHbTimerRunningFlag = false;
    m_haPeerConnectionEstablished = false;
    m_peerHaPort = FrameworkToolKit::getHaPeerInterfaceReceiverPort ();
    m_localHaPort = FrameworkToolKit::getHaInterfaceReceiverPort ();
    m_peerIpAddress = getPeerIpAddress ();
    m_localIpAddress = getLocalIpAddress ();
    m_receiveHbTimerHandle = 0;
    m_sendHbTimerHandle = 0;
    m_nsmSendCpStateTimerHandle = 0;
    m_fcoeSendCpStateTimerHandle = 0;
    initializeCpState ();
    
    addDebugFunction ((ShellCmdFunction) (&VcsNodeHaObjectManager::debugSetPeerHaPortMessageHandler),  "setpeerhaport");
    addOperationMap (VCS_NODE_HA_PEER_ENQ_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeHaObjectManager::haPeerEnqMessageHandler));
    addOperationMap (VCS_NODE_HA_HEARTBEAT_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeHaObjectManager::receiveHeartbeatMessageHandler));
}

VcsNodeHaObjectManager::~VcsNodeHaObjectManager ()
{
}

void VcsNodeHaObjectManager::initializeCpState ()
{
    m_peerCpState = VCS_NODE_HA_CP_STATE_NOT_INITIALIZED;
    // in x86 env m_peerHaPort != m_localHaPort => standalone. In PPC env m_peerHaPort == m_localHaPort
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::initializeCpState:  peer port = ")+m_peerHaPort+" local port = "+m_localHaPort);
    if ((m_peerHaPort == 0) || (m_peerHaPort == m_localHaPort))
    {   // we are either in a PPC env or x86 env standalone
        m_localCpState = VCS_NODE_HA_CP_STANDALONE;
    }
    else
    {
        m_localCpState = VCS_NODE_HA_CP_STATE_NOT_INITIALIZED;
    }
}

PrismServiceId VcsNodeHaObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

VcsNodeHaObjectManager *VcsNodeHaObjectManager::getInstance ()
{
    static VcsNodeHaObjectManager *pVcsNodeHaObjectManager = new VcsNodeHaObjectManager ();
    WaveNs::prismAssert (NULL != pVcsNodeHaObjectManager, __FILE__, __LINE__);
    return pVcsNodeHaObjectManager;
}

void VcsNodeHaObjectManager::listenForEvents (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    listenForEvent (PrismFrameworkObjectManager::getPrismServiceId (), BOOT_COMPLETE_FOR_THIS_LOCATION, reinterpret_cast<PrismEventHandler> (&VcsNodeHaObjectManager::bootCompleteEventHandler));
    pWaveAsynchronousContextForBootPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForBootPhases->callback ();
}

ResourceId VcsNodeHaObjectManager::connectToHaPeer ()
{
    return  PrismFrameworkObjectManagerHaSyncWorker::getInstance ()->connectToHaPeer (m_peerIpAddress, m_peerHaPort);
}

/*
void VcsNodeHaObjectManager::disconnectFromHaPeer ()
{
	PrismFrameworkObjectManager *pPrismFrameworkObjectManager = PrismFrameworkObjectManager::getInstance ();
    LocationBase *pThisLocation = pPrismFrameworkObjectManager->getThisLocation ();
    ClientStreamingSocket *pClientStreamingSocket = pThisLocation->getClientStreamingSocketForHaPeer ();
    //pClientStreamingSocket->close ();
    //PrismFrameworkObjectManagerHaSyncWorker::getInstance ()->disconnectFromHaPeer (m_peerIpAddress, m_peerHaPort, true);
}

*/

void VcsNodeHaObjectManager::bootCompleteEventHandler (const WaveNs::BootCompleteForThisLocationEvent *&pBootCompleteForThisLocationEvent)
{
    
    reply (reinterpret_cast<const PrismEvent *&> (pBootCompleteForThisLocationEvent));
    VcsNodeHaObjectManager* pVcsNodeHaObjectManager = VcsNodeHaObjectManager::getInstance(); 
    if (VCS_NODE_HA_CP_STANDALONE != m_localCpState)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::bootCompleteEventHandler: starting arbitration, peer port = ")+m_peerHaPort+" local port = "+m_localHaPort);
        ResourceId status = pVcsNodeHaObjectManager->connectToHaPeer ();
        if (FRAMEWORK_SUCCESS == status)
        {
            m_haPeerConnectionEstablished = true;
            pVcsNodeHaObjectManager->establishActiveCpSteps ();
        }
        else
        {
            WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::bootCompleteEventHandler: error establishing peer connection to: ")+m_peerHaPort+" ipAddress = "+m_peerIpAddress+" "+FrameworkToolKit::localize (status));
        }
    }
    else if (m_peerHaPort != m_localHaPort)
    {
         // we are in x86 standalone mdoe
        sendCpActiveMessageToTestApps (); // since this is a standalone node signal test apps to proceed normaly with sending linecard events etc
    }
    else
    {
        // do nothing we are in PPC env
    }
}


UI32 VcsNodeHaObjectManager::convertIpAddrFromStringToBin (string ipAddress)
{
    UI32 binIpAddress = 0;
    string tempIpAddress = ipAddress;
    string subfield;
    UI32   subfieldInt = 0;
    UI32 i;
    size_t pos;
    
    for (i=0; i<4; i++)
    {
        pos = tempIpAddress.find (".");
        if (string::npos == pos)
        {
            subfield = tempIpAddress;
        }
        else
        {
            subfield = tempIpAddress.substr (0, pos);
        }
        subfieldInt = (atoi (subfield.c_str ()) << (8*i));
        binIpAddress |= subfieldInt;
        tempIpAddress = tempIpAddress.substr (pos+1);
    }
    char hexIpAddress[10];
    sprintf (hexIpAddress,"%x",binIpAddress);
    return binIpAddress;
}

string VcsNodeHaObjectManager::convertIpAddrFromHexToString (UI32 hexIpAddr)
{
    string ipAddress;
    UI32 loByteMask = 0xff;
    UI32 i;
    char strbuf[5];
    for (i=0; i < 4; i++)
    {
        sprintf (strbuf,"%d", (hexIpAddr & loByteMask));
        if (i < 3 )
        {
            ipAddress += string (strbuf)+".";
        }
        else
        {
            ipAddress += string (strbuf);
        }
        hexIpAddr >>=  8;
    }
    
    return ipAddress;
}

string VcsNodeHaObjectManager::getPeerIpAddress ()
{
    return  convertIpAddrFromHexToString (get_cp_ha_peerip ());
}

string VcsNodeHaObjectManager::getLocalIpAddress ()
{
    
    return convertIpAddrFromHexToString (get_cp_ha_localip ());
}

PrismMessage *VcsNodeHaObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_NODE_HA_PEER_ENQ_MESSAGE :
            pPrismMessage = new VcsNodeHaPeerEnqMessage ();
            break;
        case VCS_NODE_HA_HEARTBEAT_MESSAGE :
            pPrismMessage = new VcsNodeHaHeartbeatMessage ();
            break;
            
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsNodeHaObjectManager::debugSetPeerHaPortMessageHandler (UI32 argc, vector<string> argv)
{

    if (argc < 2)
    {
        WaveNs::trace (TRACE_LEVEL_INFO, "VcsNodeHaObjectManager::debugSetPeerHaPortMessageHandler: Usage: setpeerhaport <peer port>");
        return;
    }
    VcsNodeHaObjectManager* pVcsNodeHaObjectManager = VcsNodeHaObjectManager::getInstance(); 
    UI32 peerHaPort = atoi (argv[1].c_str ());
    UI32 localHaPort = pVcsNodeHaObjectManager->m_localHaPort;
    pVcsNodeHaObjectManager->m_peerHaPort = peerHaPort;

    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::debugSetPeerHaPortMessageHandler: peer port set = ")+peerHaPort+" local port = "+localHaPort);
    pVcsNodeHaObjectManager->establishActiveCpSteps ();
    return;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Send message to client apps to indicate CP in active state
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

VcsNodeCpStateChangeClientMessage *VcsNodeHaObjectManager::createCpActiveMessage (UI32 opCode)
{
    VcsNodeCpStateChangeClientMessage *pVcsNodeCpStateChangeClientMessage = NULL;
    string clientName ="";
    switch (opCode)
    {
        case NSM_CP_STATE_CHANGE_MSG:
        {
            pVcsNodeCpStateChangeClientMessage = new VcsNodeCpStateChangeClientMessage (NSM_CP_STATE_CHANGE_MSG, clientName);
            prismAssert (NULL != pVcsNodeCpStateChangeClientMessage, __FILE__, __LINE__);
            clientName = "nsm";
            pVcsNodeCpStateChangeClientMessage->setCpState (m_localCpState);
            pVcsNodeCpStateChangeClientMessage->setTryCount(0);
            pVcsNodeCpStateChangeClientMessage->setClientName (clientName);
            break;    
        }
        case FCOE_CP_STATE_CHANGE_MSG:
        {
            pVcsNodeCpStateChangeClientMessage = new VcsNodeCpStateChangeClientMessage (FCOE_CP_STATE_CHANGE_MSG, clientName);
            prismAssert (NULL != pVcsNodeCpStateChangeClientMessage, __FILE__, __LINE__);
            clientName = "fcoed";
            pVcsNodeCpStateChangeClientMessage->setCpState (m_localCpState);
            pVcsNodeCpStateChangeClientMessage->setTryCount(0);
            pVcsNodeCpStateChangeClientMessage->setClientName (clientName);
            break;
        }
        default:
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::sendCpActiveMessageToTestApps : Unknown message opCode = ")+opCode);
        }
    }
    return (pVcsNodeCpStateChangeClientMessage);
}

ResourceId VcsNodeHaObjectManager::startSendCpStateTimer (VcsNodeCpStateChangeClientMessage *&pVcsNodeCpStateChangeClientMessage, UI32 timeOutValue)
{
    ResourceId status = FRAMEWORK_ERROR;
    UI32 tryCount = pVcsNodeCpStateChangeClientMessage->getTryCount ();
    tryCount++;
    string clientName = pVcsNodeCpStateChangeClientMessage->getClientName ();
    TimerHandle timerHandle = 0;
    if ("nsm" == clientName)
    {
        delete (pVcsNodeCpStateChangeClientMessage);
        pVcsNodeCpStateChangeClientMessage = createCpActiveMessage (NSM_CP_STATE_CHANGE_MSG);
        pVcsNodeCpStateChangeClientMessage->setTryCount (tryCount);
        timerHandle = m_nsmSendCpStateTimerHandle;
        trace (TRACE_LEVEL_INFO, "VcsNodeHaObjectManager::startSendCpStateTimer: new message created for nsm");
    }
    if ("fcoed" == clientName)
    {
    
        delete (pVcsNodeCpStateChangeClientMessage);
        pVcsNodeCpStateChangeClientMessage = createCpActiveMessage (FCOE_CP_STATE_CHANGE_MSG);
        pVcsNodeCpStateChangeClientMessage->setTryCount (tryCount);
        timerHandle = m_fcoeSendCpStateTimerHandle;
        trace (TRACE_LEVEL_INFO, "VcsNodeHaObjectManager::startSendCpStateTimer: new message created for fcoe");
    }
    
    if (g_maxTryCount >= tryCount)
    {
        status = startTimer (timerHandle, timeOutValue, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodeHaObjectManager::sendCpActiveMessageToTestApp), reinterpret_cast<void *> (pVcsNodeCpStateChangeClientMessage));
        if (FRAMEWORK_SUCCESS == status)
        {
            trace (TRACE_LEVEL_INFO, "VcsNodeHaObjectManager::startSendCpStateTimer: sendCpActive message timer started for "+clientName);
            return (FRAMEWORK_SUCCESS);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::startSendCpStateTimer: sendCpActive message timer could not be started for "+clientName+" error = "+FrameworkToolKit::localize (status));
            return (FRAMEWORK_ERROR);
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::startSendCpStateTimer: sendCpActive retry count exceeded for: "+clientName);
        return (FRAMEWORK_ERROR);
    }
}

void VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack (FrameworkStatus frameworkStatus, VcsNodeCpStateChangeClientMessage *pVcsNodeCpStateChangeClientMessage, void *pContext)
{
    prismAssert (NULL != pVcsNodeCpStateChangeClientMessage, __FILE__, __LINE__);
    string clientName = pVcsNodeCpStateChangeClientMessage->getClientName ();
    ResourceId status = FRAMEWORK_ERROR;
    if (FRAMEWORK_SUCCESS != frameworkStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack CpStateChangeClientMessage send callback error = ")+FrameworkToolKit::localize (frameworkStatus)+" clientName = "+clientName);
        status = startSendCpStateTimer (pVcsNodeCpStateChangeClientMessage, 10*one_sec);
        if (FRAMEWORK_ERROR == status)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack : start timer error deleting message"));
            delete (pVcsNodeCpStateChangeClientMessage);
        }
    }
    else
    {
        status = pVcsNodeCpStateChangeClientMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS == status)
        {
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack: CpStateChangeClientMessage send callback received from: ")+clientName);
            delete (pVcsNodeCpStateChangeClientMessage);
        }
        else
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack CpStateChangeClientMessage send callback received from: ")+clientName+" completionStatus error = "+FrameworkToolKit::localize (status));
            status = startSendCpStateTimer (pVcsNodeCpStateChangeClientMessage, 10*one_sec);
            if (FRAMEWORK_ERROR == status)
            {
                trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack : start timer error deleting message"));
                delete (pVcsNodeCpStateChangeClientMessage);
            }
        }
    }
}

void VcsNodeHaObjectManager::sendCpActiveMessageToTestApp (TimerHandle timerHandle, void *pContext) // pContext points to the message to be sent
{
    VcsNodeCpStateChangeClientMessage *pVcsNodeCpStateChangeClientMessage = reinterpret_cast<VcsNodeCpStateChangeClientMessage *> (pContext);
    prismAssert (NULL != pVcsNodeCpStateChangeClientMessage, __FILE__, __LINE__);
    string clientName = pVcsNodeCpStateChangeClientMessage->getClientName ();
    ResourceId status = sendToWaveClient (clientName, pVcsNodeCpStateChangeClientMessage, reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeHaObjectManager::VcsNodeCpStateChangeClientMessageCallBack));
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::sendCpActiveMessageToTestApps : Failed to send cp State change message to: ")+clientName+" error = "+FrameworkToolKit::localize (status));
        status = startSendCpStateTimer (pVcsNodeCpStateChangeClientMessage, 10*one_sec);
        if (FRAMEWORK_ERROR == status)
        {
            trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::sendCpActiveMessageToTestApps : start timer error deleting message"));
            delete (pVcsNodeCpStateChangeClientMessage);
        }
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeHaObjectManager::sendCpActiveMessageToTestApps : cp State change message successfully sent to: ")+clientName);
    }
}

void VcsNodeHaObjectManager::sendCpActiveMessageToTestApps ()
{
    VcsNodeCpStateChangeClientMessage *pVcsNodeCpStateChangeClientMessage = createCpActiveMessage (NSM_CP_STATE_CHANGE_MSG);
    startSendCpStateTimer (pVcsNodeCpStateChangeClientMessage, 10*one_sec);
    //sendCpActiveMessageToTestApp (m_nsmSendCpStateTimerHandle, reinterpret_cast<void *> (pVcsNodeCpStateChangeClientMessage));
    
    pVcsNodeCpStateChangeClientMessage = createCpActiveMessage (FCOE_CP_STATE_CHANGE_MSG);
    //sendCpActiveMessageToTestApp (m_fcoeSendCpStateTimerHandle, reinterpret_cast<void *> (pVcsNodeCpStateChangeClientMessage));
    startSendCpStateTimer (pVcsNodeCpStateChangeClientMessage, 10*one_sec);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Arbitration and state change functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsNodeHaObjectManager::transitionUninitToActive ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::transitionUninitToActive: starting active role"));
    m_localCpState = VCS_NODE_HA_CP_ACTIVE;
    sendCpActiveMessageToTestApps ();
    startHaSendHeartbeatTimer (10*one_sec);
}

void VcsNodeHaObjectManager::transitionUninitToStandby ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::transitionUninitToStandby: starting standby role"));
    m_localCpState = VCS_NODE_HA_CP_STANDBY;
    startHaReceiveHeartbeatTimer (one_sec * 40);
}

void VcsNodeHaObjectManager::transitionStandbyToActive ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::transitionStandbyToActive: transitioning from standby to active role"));
    m_localCpState = VCS_NODE_HA_CP_ACTIVE;
    sendCpActiveMessageToTestApps ();
    disableHaReceiveHeartbeatTimer (); 
    startHaSendHeartbeatTimer (10*one_sec);
}

void VcsNodeHaObjectManager::transitionUninitToStandAlone ()
{
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::transitionUninitToStandAlone: transitioning from uninitialized to standalone role"));

    m_localCpState = VCS_NODE_HA_CP_STANDALONE;
    sendCpActiveMessageToTestApps (); // since this is a standalone node signal test apps to proceed normaly with sending linecard events etc
}

void VcsNodeHaObjectManager::arbitrateActiveCp ()
{
    UI32 myIpAddressPlusPort = convertIpAddrFromStringToBin (m_localIpAddress) + m_localHaPort;
    UI32 peerIpAddressPlusPort = convertIpAddrFromStringToBin (m_peerIpAddress) + m_peerHaPort;
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: myIpAddressPlusPort = ")+myIpAddressPlusPort+" peerIpAddressPlusPort = "+peerIpAddressPlusPort+" myCpState = "+FrameworkToolKit::localize (m_localCpState));
    if (myIpAddressPlusPort == peerIpAddressPlusPort)
    {
    
        WaveNs::trace (TRACE_LEVEL_FATAL, string ("VcsNodeHaObjectManager::arbitrateActiveCp: myIpAddressPlusPort = ")+myIpAddressPlusPort+" peerIpAddressPlusPort = "+peerIpAddressPlusPort+" myCpState = "+FrameworkToolKit::localize (m_localCpState));
        prismAssert (0, __FILE__, __LINE__);
    }
    switch (m_localCpState)
    {
        case VCS_NODE_HA_CP_STATE_NOT_INITIALIZED:
        {
            switch (m_peerCpState)
            {
                case VCS_NODE_HA_CP_STATE_NOT_INITIALIZED:
                {
                    if (myIpAddressPlusPort < peerIpAddressPlusPort)
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP selected to be active")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                        transitionUninitToActive ();
                    }
                    else
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will be standby")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                        transitionUninitToStandby ();
                    }
                    break;
                }
                case VCS_NODE_HA_CP_STANDBY:
                {
                    if (myIpAddressPlusPort < peerIpAddressPlusPort)
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP selected to be active")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                        transitionUninitToActive ();
                    }
                    else
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will be standby")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                        transitionUninitToStandby ();
                    }
                    break;
                }
                case VCS_NODE_HA_CP_ACTIVE:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will be standby")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                    transitionUninitToStandby ();
                    break;
                }
                case VCS_NODE_HA_CP_STANDALONE:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: peer node is standalone so this CP will be standalone")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                    transitionUninitToStandAlone ();
                    break;
                }
                
                
                default:
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: peer CP state is undefined - this should never happen: CP state = ")+m_peerCpState);
                }
                
            }
            break;
            
        }
        case VCS_NODE_HA_CP_STANDBY:
        {
            switch (m_peerCpState)
            {
                case VCS_NODE_HA_CP_STATE_NOT_INITIALIZED:
                {
                    if (myIpAddressPlusPort < peerIpAddressPlusPort)
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP selected to be active")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                        transitionStandbyToActive ();
                    }
                    else
                    {
                        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will continue to be standby")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                    }
                    break;
                }
                case VCS_NODE_HA_CP_STANDBY:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: both CPs are standy, this should never happen, so asserting..."));
                    prismAssert (0, __FILE__, __LINE__);
                    break;
                }
                case VCS_NODE_HA_CP_ACTIVE:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will continue to be standby")+"peer CP state = "+FrameworkToolKit::localize (m_peerCpState));
                    break;
                }
                
                default:
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: peer CP state is invalid - this should never happen: CP state = ")+m_peerCpState);
                }
                
            }
            break;
        }
        case VCS_NODE_HA_CP_ACTIVE:
        {
            switch (m_peerCpState)
            {
                case VCS_NODE_HA_CP_STATE_NOT_INITIALIZED:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will continue to be active"));
                    startHaSendHeartbeatTimer (10*one_sec);
                    break;
                }
                case VCS_NODE_HA_CP_STANDBY:
                {
                    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::arbitrateActiveCp: this CP will continue to be active"));
                    startHaSendHeartbeatTimer (10*one_sec);
                    break;
                }
                case VCS_NODE_HA_CP_ACTIVE:
                {
                    WaveNs::trace (TRACE_LEVEL_FATAL, string ("VcsNodeHaObjectManager::arbitrateActiveCp: both CPs are active, this should never happen, asserting..."));
                    prismAssert (0, __FILE__, __LINE__);
                    break;
                }
                case VCS_NODE_HA_CP_STANDALONE:
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: peer CP is in standalone mode no action will be taken"));
                    break;
                }
                
                
                default:
                {
                    WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: peer CP state is invalid - this should never happen: CP state = ")+m_peerCpState);
                }
                
            }
            break;
        }
        
        case VCS_NODE_HA_CP_STANDALONE:
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: local CP state is standalone no action will be taken"));
            break; 
        }
        default:
        {
            WaveNs::trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::arbitrateActiveCp: local CP state is undefined - this should never happen: CP state = ")+m_localCpState);
        }
            
    }

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Respond to peer CP request for information
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ResourceId VcsNodeHaObjectManager::savePeerCpInfo (VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext)
{
    VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage = reinterpret_cast<VcsNodeHaPeerEnqMessage *> (pVcsNodeHaMessagingContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeHaPeerEnqMessage, __FILE__, __LINE__);

    // Save peer CP info
    m_peerIpAddress = pVcsNodeHaPeerEnqMessage->getSenderIpAddress(); 
    m_peerHaPort = pVcsNodeHaPeerEnqMessage->getSenderPort ();
    m_peerCpState = pVcsNodeHaPeerEnqMessage->getSenderCpState();
    if (VCS_NODE_HA_CP_STANDALONE == m_localCpState)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::savePeerCpInfo this node is standalone but received PeerEnqMsg from CP ipAddress = ")+m_peerIpAddress + " port = "+m_peerHaPort+" CP state = "+m_peerCpState+" i.e. "+FrameworkToolKit::localize (m_peerCpState));
        return WAVE_MESSAGE_ERROR;
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsNodeHaObjectManager::getLocalCpInfo (VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext)
{
    VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage = reinterpret_cast<VcsNodeHaPeerEnqMessage *> (pVcsNodeHaMessagingContext->getPPrismMessage ());
    prismAssert (NULL != pVcsNodeHaPeerEnqMessage, __FILE__, __LINE__);
    string localIpAddress = getLocalIpAddress ();
  
    // Send local CP info
    pVcsNodeHaPeerEnqMessage->setReceiverIpAddress(localIpAddress);
    pVcsNodeHaPeerEnqMessage->setReceiverPort(m_localHaPort);
    pVcsNodeHaPeerEnqMessage->setReceiverCpState(m_localCpState);
    
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsNodeHaObjectManager::selectActiveCpOnPeerEnqStep (VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext)
{
    arbitrateActiveCp ();
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VcsNodeHaObjectManager::reopenPeerConnectionIfFailedStep (VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext)
{
    if (VCS_NODE_HA_CP_FAILED != m_peerCpState)
    {
        return WAVE_MESSAGE_SUCCESS;
    }
    ResourceId status = connectToHaPeer ();
    if (FRAMEWORK_SUCCESS == status)
    {
        m_haPeerConnectionEstablished = true;
        return WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::reopenPeerConnectionIfFailedStep: error establishing peer connection to: ")+m_peerHaPort+" ipAddress = "+m_peerIpAddress+" "+FrameworkToolKit::localize (status));
        return WAVE_MESSAGE_ERROR;
    }
}

void VcsNodeHaObjectManager::haPeerEnqMessageHandler (VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage)
{

    string peerCpIpAddress = pVcsNodeHaPeerEnqMessage->getSenderIpAddress ();
    UI32   peerPort = pVcsNodeHaPeerEnqMessage->getSenderPort ();
    UI32   peerCpState = pVcsNodeHaPeerEnqMessage->getSenderCpState ();
    trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::haPeerEnqMessageHandler received PeerEnqMsg from CP ipAddress = ")+peerCpIpAddress + " port = "+peerPort+" CP state = "+peerCpState+" i.e. "+FrameworkToolKit::localize (peerCpState));
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::reopenPeerConnectionIfFailedStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::savePeerCpInfo),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::selectActiveCpOnPeerEnqStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::getLocalCpInfo),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VcsNodeHaObjectManager::prismSynchronousLinearSequencerFailedStep)
    };
    VcsNodeHaMessagingContext *pVcsNodeHaMessagingContext = new VcsNodeHaMessagingContext (reinterpret_cast<PrismMessage *> (pVcsNodeHaPeerEnqMessage), this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeHaMessagingContext, __FILE__, __LINE__);
    pVcsNodeHaMessagingContext->execute ();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Get peer CP information
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void VcsNodeHaObjectManager::getHaPeerCpInfoCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext)
{
    ResourceId completionStatus;
    VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage = reinterpret_cast<VcsNodeHaPeerEnqMessage *> (pPrismMessage);
    if (FRAMEWORK_SUCCESS == frameworkStatus) 
    {
        prismAssert ((NULL != pVcsNodeHaPeerEnqMessage), __FILE__, __LINE__);
        completionStatus = pVcsNodeHaPeerEnqMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::getHaPeerCpInfoCallback: VcsNodeHaPeerEnqMessage completionStatus error  = ") +FrameworkToolKit::localize (completionStatus));
            m_peerCpState = pVcsNodeHaPeerEnqMessage->getSenderCpState ();
            pVcsNodeAsyncHaMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
        } 
        else 
        {
            m_peerCpState = pVcsNodeHaPeerEnqMessage->getReceiverCpState ();
            trace (TRACE_LEVEL_SUCCESS, string ("VcsNodeHaObjectManager::getHaPeerCpInfoCallback: VcsNodeHaPeerEnqMessage completionStatus = success"));
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::getHaPeerCpInfoCallback: framework status error = ")+ FrameworkToolKit::localize (frameworkStatus));
    }
    delete (pVcsNodeHaPeerEnqMessage);
    --(*pVcsNodeAsyncHaMessagingContext);
    pVcsNodeAsyncHaMessagingContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeHaObjectManager::sendPeerCpEnqMessageStep    (VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext)
{
       
   VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage =  reinterpret_cast <VcsNodeHaPeerEnqMessage *> (pVcsNodeAsyncHaMessagingContext->getPPrismMessage ());
   pVcsNodeHaPeerEnqMessage->setSenderIpAddress (m_localIpAddress);
   pVcsNodeHaPeerEnqMessage->setSenderPort (m_localHaPort);
   pVcsNodeHaPeerEnqMessage->setSenderCpState(m_localCpState);
   
   ++(*pVcsNodeAsyncHaMessagingContext);
   trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::sendPeerCpEnqMessageStep: sending async message to ipAddress: ")+ m_peerIpAddress+":"+m_peerHaPort+ "local CP state = "+m_localCpState+" i.e. "+FrameworkToolKit::localize (m_localCpState));
   ResourceId status = send (pVcsNodeHaPeerEnqMessage, 
       reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeHaObjectManager::getHaPeerCpInfoCallback), 
       pVcsNodeAsyncHaMessagingContext, 0, 1);
   
   if (WAVE_MESSAGE_SUCCESS != status)
   {
       trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::sendPeerCpEnqMessageStep: send async get Ha peer info message failed, status = " + FrameworkToolKit::localize (status));
       --(*pVcsNodeAsyncHaMessagingContext);
       pVcsNodeAsyncHaMessagingContext->executeNextStep (WAVE_MESSAGE_ERROR);
   }
   else
   {
       trace (TRACE_LEVEL_SUCCESS, "VcsNodeHaObjectManager::sendPeerCpEnqMessageStep: send async get Ha peer info message successfully sent");
   }
}

void VcsNodeHaObjectManager::selectActiveCpStep (VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext)
{
    arbitrateActiveCp ();
}

void VcsNodeHaObjectManager::establishActiveCpSteps ()
{

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeHaObjectManager::sendPeerCpEnqMessageStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeHaObjectManager::selectActiveCpStep),    
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeHaObjectManager::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeHaObjectManager::prismLinearSequencerFailedStep)
    };
    VcsNodeHaPeerEnqMessage *pVcsNodeHaPeerEnqMessage = new VcsNodeHaPeerEnqMessage ();
    prismAssert (NULL != pVcsNodeHaPeerEnqMessage, __FILE__, __LINE__);
    
    VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext = new VcsNodeAsyncHaMessagingContext (pVcsNodeHaPeerEnqMessage , this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVcsNodeAsyncHaMessagingContext, __FILE__, __LINE__);
    pVcsNodeAsyncHaMessagingContext->start ();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Heartbeat and timer related functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void  VcsNodeHaObjectManager::activeCpHeartbeatFailure ()
{
    m_receiveHbTimerRunningFlag = false;
    WaveNs::trace (TRACE_LEVEL_INFO, string ("VcsNodeHaObjectManager::doActiveFailover: CP state set to: ")+FrameworkToolKit::localize (m_localCpState));
    transitionStandbyToActive ();
    //disconnectFromHaPeer ();
    m_peerCpState = VCS_NODE_HA_CP_FAILED;
}

void VcsNodeHaObjectManager::receiveHeartbeatMessageHandler (VcsNodeHaHeartbeatMessage *pVcsNodeHaHeartbeatMessage)
{
    static UI32 hbReceiveMessageCount = 0;
           UI32 sentMessageCount      = pVcsNodeHaHeartbeatMessage->getMessageCount ();
           UI32 peerCpState           = pVcsNodeHaHeartbeatMessage->getSenderCpState ();

    hbReceiveMessageCount++;


    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeHaObjectManager::receiveHeartbeatMessageHandler: hb receive count = ")+hbReceiveMessageCount+" peer sent message count ="+sentMessageCount+" peer state ="+peerCpState+" "+FrameworkToolKit::localize(peerCpState));

    switch (m_localCpState)
    {
        case VCS_NODE_HA_CP_STANDBY:
        {
            startHaReceiveHeartbeatTimer (35*one_sec);
            break;
        }
        case VCS_NODE_HA_CP_STATE_NOT_INITIALIZED:
        {
            m_peerHaPort = pVcsNodeHaHeartbeatMessage->getSenderPort ();
            m_peerCpState = pVcsNodeHaHeartbeatMessage->getSenderCpState ();
            arbitrateActiveCp ();   // this node probably rebooted and the peer node became active
            break;
        }
        case VCS_NODE_HA_CP_ACTIVE:
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsNodeHaObjectManager::receiveHeartbeatMessageHandler: both CPs in active state, this should never happen so asserting = ")+
                hbReceiveMessageCount+" peer sent message count ="+sentMessageCount+" peerCpState = "+FrameworkToolKit::localize (m_peerCpState));
            prismAssert (0, __FILE__, __LINE__);
        }
        case VCS_NODE_HA_CP_STANDALONE:
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::receiveHeartbeatMessageHandler: received unexpected heartbeat message in standalone mode = ")+
                hbReceiveMessageCount+" peer sent message count ="+sentMessageCount+" peerCpState = "+FrameworkToolKit::localize (m_peerCpState));
            break;
        }
        
       default:
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsNodeHaObjectManager::receiveHeartbeatMessageHandler: invalid local cp state so asserting = ")+
                hbReceiveMessageCount+" peer sent message count ="+sentMessageCount+" peerCpState = "+m_localCpState);
            prismAssert (0, __FILE__, __LINE__);
            
        }
    }
    
    pVcsNodeHaHeartbeatMessage->setReceiverCpState (m_localCpState);
    
    pVcsNodeHaHeartbeatMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    reply (pVcsNodeHaHeartbeatMessage);
}

void VcsNodeHaObjectManager::heartbeatMessageCallback (FrameworkStatus frameworkStatus, PrismMessage *pPrismMessage, VcsNodeAsyncHaMessagingContext *pVcsNodeAsyncHaMessagingContext)
{
    static UI32 hbMessageCount = 0;
    hbMessageCount++;
    static UI32 hbMessageFailureCount = 0;
    
    ResourceId completionStatus;
    VcsNodeHaHeartbeatMessage *pVcsNodeHaHeartbeatMessage = reinterpret_cast<VcsNodeHaHeartbeatMessage *> (pPrismMessage);
    if (FRAMEWORK_SUCCESS == frameworkStatus) 
    {
        prismAssert ((NULL != pVcsNodeHaHeartbeatMessage), __FILE__, __LINE__);
        completionStatus = pVcsNodeHaHeartbeatMessage->getCompletionStatus ();
        if (WAVE_MESSAGE_SUCCESS != completionStatus) 
        {
            hbMessageFailureCount++;
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::heartbeatMessageCallback: VcsNodeHaHeartbeatMessage failure count = ")+hbMessageFailureCount+ " completion status = "+FrameworkToolKit::localize (completionStatus));
            if (3 <= hbMessageFailureCount )
            {
                disableHaSendHeartbeatTimer ();
                m_peerCpState = VCS_NODE_HA_CP_FAILED;
                hbMessageFailureCount = 0;
            }
            else
            {
                startHaSendHeartbeatTimer (one_sec * 10); 
            }
        } 
        else 
        {
            hbMessageFailureCount = 0;
            startHaSendHeartbeatTimer (one_sec * 10); 
        }
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeHaObjectManager::heartbeatMessageCallback: framework status error = ")+ FrameworkToolKit::localize (frameworkStatus));
        if (3 <= hbMessageFailureCount )
        {
            disableHaSendHeartbeatTimer ();
        }
    }
    delete (pVcsNodeHaHeartbeatMessage);
}

void VcsNodeHaObjectManager::sendHeartbeat ()
{
    m_sendHbTimerRunningFlag = false;
    static UI32 sendHBCount = 0;
    sendHBCount++;
    trace (TRACE_LEVEL_DEBUG, string ("VcsNodeHaObjectManager::hbSendTimerHandler: send HB count = ")+sendHBCount);
    
    VcsNodeHaHeartbeatMessage *pVcsNodeHaHeartbeatMessage = new VcsNodeHaHeartbeatMessage (sendHBCount);
    prismAssert (NULL != pVcsNodeHaHeartbeatMessage, __FILE__, __LINE__);
    pVcsNodeHaHeartbeatMessage->setSenderCpState(m_localCpState);

    ResourceId status = send (pVcsNodeHaHeartbeatMessage, 
        reinterpret_cast<PrismMessageResponseHandler> (&VcsNodeHaObjectManager::heartbeatMessageCallback), 
        NULL, 0, 1);
    
    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::sendHeartbeat: send async heartbeat message status = " + FrameworkToolKit::localize (status));
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::sendHeartbeat: send async heartbeat message successfully sent");
    }
    
}


bool VcsNodeHaObjectManager::startHaReceiveHeartbeatTimer (UI32 timeoutValue)
{

    if (!disableHaReceiveHeartbeatTimer ())        // disable timer if it's currently running
    {
        return false;
    }
    ResourceId status = startTimer (m_receiveHbTimerHandle, timeoutValue, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodeHaObjectManager::activeCpHeartbeatFailure), NULL);

    if (FRAMEWORK_SUCCESS == status)
    {
        VcsNodeHaObjectManager::m_receiveHbTimerRunningFlag = true;
        trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::startHaReceiveHeartbeatTimer: Ha receive HB timer starting");
        return true;
    }
    else
    {
        VcsNodeHaObjectManager::m_receiveHbTimerRunningFlag = false;
        trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::startHaReceiveHeartbeatTimer: Ha receive HB timer could not be started error = "+FrameworkToolKit::localize (status));
        return false;
    }
}

bool VcsNodeHaObjectManager::startHaSendHeartbeatTimer (UI32 timeoutValue)
{

    if (!disableHaSendHeartbeatTimer())        // disable timer if it's currently running
    {
        return false;
    }
    
    ResourceId status = startTimer (m_sendHbTimerHandle, timeoutValue, reinterpret_cast<PrismTimerExpirationHandler> (&VcsNodeHaObjectManager::sendHeartbeat), NULL);

    if (FRAMEWORK_SUCCESS == status)
    {
        VcsNodeHaObjectManager::m_sendHbTimerRunningFlag = true;
        trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::startSendHeartbeatTimer: Ha send HB timer starting");
        return true;
    }
    else
    {
        VcsNodeHaObjectManager::m_sendHbTimerRunningFlag = false;
        trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::startSendHeartbeatTimer: Ha send HB timer could not be started error = "+FrameworkToolKit::localize (status));
        return false;
    }
}

bool VcsNodeHaObjectManager::disableHaSendHeartbeatTimer ()
{

    if (m_sendHbTimerRunningFlag)
    {                                         
        if (FRAMEWORK_SUCCESS != deleteTimer (m_sendHbTimerHandle))
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::disableHaSendHeartbeatTimer: deleteTimer FAILED.");
            return false;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::disableHaSendHeartbeatTimer deleteTimer SUCCEEDED.");
            m_sendHbTimerRunningFlag = false;
            return true;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::disableHaSendHeartbeatTimer: timer not running.");
        return true;
    }
}


bool VcsNodeHaObjectManager::disableHaReceiveHeartbeatTimer ()
{

    if (m_receiveHbTimerRunningFlag)
    {                                         
        if (FRAMEWORK_SUCCESS != deleteTimer (m_receiveHbTimerHandle))
        {
            trace (TRACE_LEVEL_ERROR, "VcsNodeHaObjectManager::disableHaReceiveHeartbeatTimer: deleteTimer failed.");
            return false;
        }
        else
        {
            trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::disableHaReceiveHeartbeatTimer deleteTimer succeeded.");
            m_receiveHbTimerRunningFlag = false;
            return true;
        }
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "VcsNodeHaObjectManager::disableHaReceiveHeartbeatTimer: timer not running.");
        return true;
    }
}

}

