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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jitendra Singh                                               *
 ***************************************************************************/
/*
#include <sstream>
#include <iostream>
#include "VcsFabric/Local/VcsNodeRemoteMessagingObjectManager.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "VcsFabric/Local/VcsNodeFCPrincipalTrackingWorker.h"

#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "ValClientInterface/ValClientSynchronousConnection.h"
#include "DcmCore/DcmToolKit.h"
#include "ManagementInterface/ClientInterface/WaveClientSynchronousConnection.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"
#include "Cluster/ClusterMessages.h"
#include "Cluster/Local/WaveNode.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "fabric/fabric.h" 
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include <sstream>

namespace DcmNs
{

static UI32 s_connectionSequenceNumber = 0;

VcsNodeRemoteMessagingObjectManager::VcsNodeRemoteMessagingObjectManager () : WaveLocalObjectManager (getClassName ())
{
    PrismFrameworkObjectManager::excludeServiceForClusterValidationPhase (getServiceId ());


	addOperationMap (VCS_REMOTE_ENQ_SWITCH_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeRemoteMessagingObjectManager::VcsRemoteEnqSwitchMessageHandler));
	addOperationMap (VCS_ASYNC_CLUSTER_MERGE_MESSAGE, reinterpret_cast<PrismMessageHandler> (&VcsNodeRemoteMessagingObjectManager::VcsAsyncClusterMergeMessageHandler));
}

VcsNodeRemoteMessagingObjectManager::~VcsNodeRemoteMessagingObjectManager ()
{
}

string VcsNodeRemoteMessagingObjectManager::getClassName ()
{
    return ("VcsNodeRemoteMessagingObjectManager");
}

VcsNodeRemoteMessagingObjectManager *VcsNodeRemoteMessagingObjectManager::getInstance ()
{
    static VcsNodeRemoteMessagingObjectManager *pVcsNodeRemoteMessagingObjectManager = new VcsNodeRemoteMessagingObjectManager ();

    WaveNs::prismAssert (NULL != pVcsNodeRemoteMessagingObjectManager, __FILE__, __LINE__);

    return (pVcsNodeRemoteMessagingObjectManager);
}

PrismServiceId VcsNodeRemoteMessagingObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *VcsNodeRemoteMessagingObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_REMOTE_ENQ_SWITCH_MESSAGE :
            pPrismMessage = new VcsRemoteEnqSwitchMessage ();
            break;
        case VCS_ASYNC_CLUSTER_MERGE_MESSAGE :
            pPrismMessage = new VcsAsyncClusterMergeMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsNodeRemoteMessagingObjectManager::getLocalSwitchInfo (switchInfoParameters &localSwitchInfo)
{
    if (DcmToolKit::fcPrincipalEnabled())
    {
        DcmNs::VcsNodeFCPrincipalTrackingWorker::getInstance ()->getLocalSwitchInfo(localSwitchInfo);
    }
    else
    {
        VcsNodePrincipalSelectionWorker::getInstance ()->getLocalSwitchInfo (localSwitchInfo);
    }
}

UI32 VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo (switchInfoParameters &remoteSwitchInfo)
{

    ResourceId      status           = WAVE_MESSAGE_ERROR;
    ResourceId      connectionStatus;
    u_short         myDomainId = 0;
    u_short         remoteDomainId = remoteSwitchInfo.switchDomainId;
    string          myIpAddress = "";
    string          remoteMgmtIpAddress = "";
    UI32            port = 0;
    UI32            remoteMgmtPort = 0;

    if (DcmToolKit::fcPrincipalEnabled())
    {
        VcsNodeFCPrincipalTrackingWorker *pVcsNodeFCPrincipalTrackingWorker = VcsNodeFCPrincipalTrackingWorker::getInstance ();
        myDomainId = VcsNodeFCPrincipalTrackingWorker::getInstance ()->getMyDomainId();
        if (! pVcsNodeFCPrincipalTrackingWorker->validDomainId (myDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
            return GENERIC_ERROR;
        }
        if (! VcsNodeFCPrincipalTrackingWorker::getInstance ()->validDomainId (remoteDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo: invalid remoteDomainId returned invalid myDomainId = ")+remoteDomainId);
            return GENERIC_ERROR;
        }
        
        myIpAddress = pVcsNodeFCPrincipalTrackingWorker->getIpAddressFromDomainId (myDomainId);
        port = pVcsNodeFCPrincipalTrackingWorker->getPortFromDomainId (myDomainId);
        remoteMgmtIpAddress = pVcsNodeFCPrincipalTrackingWorker->getIpAddressFromDomainId (remoteDomainId);
        remoteMgmtPort = pVcsNodeFCPrincipalTrackingWorker->getMgmtPort (remoteDomainId);
    }
    else
    {
        VcsNodePrincipalSelectionWorker *pVcsNodePrincipalSelectionWorker = VcsNodePrincipalSelectionWorker::getInstance ();
        myDomainId = pVcsNodePrincipalSelectionWorker->getMyDomainId (); 
        if (! pVcsNodePrincipalSelectionWorker->validDomainId (myDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
            return GENERIC_ERROR;
        }
        if (! pVcsNodePrincipalSelectionWorker->validDomainId (remoteDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo: invalid remoteDomainId returned invalid myDomainId = ")+remoteDomainId);
            return GENERIC_ERROR;
        }
        myIpAddress = pVcsNodePrincipalSelectionWorker->getIpAddressFromDomainId (myDomainId);
        port = pVcsNodePrincipalSelectionWorker->getPortFromDomainId (myDomainId);
        remoteMgmtIpAddress = pVcsNodePrincipalSelectionWorker->getIpAddressFromDomainId (remoteDomainId);
        remoteMgmtPort = pVcsNodePrincipalSelectionWorker->getMgmtPort (remoteDomainId);
    }

    string myName = this->getClassName () + string (":") + myIpAddress + string (":") + port;

    // get remote switch information

    trace (TRACE_LEVEL_DEBUG, string("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo : setting up connection to remote wave server ")+ "From: " + myName + " To: " + " ipAddress: "+remoteMgmtIpAddress+" port: "+remoteMgmtPort);
    ValClientSynchronousConnection valClientSynchronousConnection (myName, remoteMgmtIpAddress, remoteMgmtPort);

    stringstream clientNameExt;
    clientNameExt << s_connectionSequenceNumber;

    string clientNameExtenstion = ":" + clientNameExt.str ();
    s_connectionSequenceNumber ++;
   
    connectionStatus = valClientSynchronousConnection.connect (clientNameExtenstion);

    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo : Error establishing connection to remote switch domainId = ")+remoteSwitchInfo.switchDomainId);
        return GET_REMOTE_INFO_ERROR;
    }

    getLocalSwitchInfo (remoteSwitchInfo);
    status = valClientSynchronousConnection.getRemoteSwitchInfo (remoteSwitchInfo);
    valClientSynchronousConnection.close ();
    if (valClientSynchronousConnection.isCurrentlyConnected ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo : Connection to server was not properly closed ipAddress = " )+remoteSwitchInfo.switchMgmtIpAddress);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo : Error getting remote switch information for domainId = ")+remoteDomainId);
        return GET_REMOTE_INFO_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, string("VcsNodeRemoteMessagingObjectManager::getRemoteSwitchInfo : responderSerialNumber = ") + remoteSwitchInfo.switchSerialNumber + " respSWVer = " +remoteSwitchInfo.switchSWVersion + " IpAddr : " + remoteSwitchInfo.switchIpAddress + " Port: " + remoteSwitchInfo.switchPort);
        return VCS_SUCCESS;
    }
}

UI32 VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage (switchInfoParameters &remoteSwitchInfo)
{

    ResourceId      status           = WAVE_MESSAGE_ERROR;
    ResourceId      connectionStatus;
    u_short         myDomainId = 0;
    u_short         remoteDomainId = remoteSwitchInfo.switchDomainId;
    string          myIpAddress = "";
    string          remoteMgmtIpAddress = "";
    UI32            port = 0;
    UI32            remoteMgmtPort = 0;

    if (DcmToolKit::fcPrincipalEnabled())
    {
        VcsNodeFCPrincipalTrackingWorker *pVcsNodeFCPrincipalTrackingWorker = VcsNodeFCPrincipalTrackingWorker::getInstance ();
        myDomainId = VcsNodeFCPrincipalTrackingWorker::getInstance ()->getMyDomainId();
        if (! pVcsNodeFCPrincipalTrackingWorker->validDomainId (myDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
            return GENERIC_ERROR;
        }
        if (! VcsNodeFCPrincipalTrackingWorker::getInstance ()->validDomainId (remoteDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage: invalid remoteDomainId = ")+remoteDomainId);
            return GENERIC_ERROR;
        }
        
        myIpAddress = pVcsNodeFCPrincipalTrackingWorker->getIpAddressFromDomainId (myDomainId);
        port = pVcsNodeFCPrincipalTrackingWorker->getPortFromDomainId (myDomainId);
        remoteMgmtIpAddress = pVcsNodeFCPrincipalTrackingWorker->getIpAddressFromDomainId (remoteDomainId);
        remoteMgmtPort = pVcsNodeFCPrincipalTrackingWorker->getMgmtPort (remoteDomainId);
    }
    else
    {
        VcsNodePrincipalSelectionWorker *pVcsNodePrincipalSelectionWorker = VcsNodePrincipalSelectionWorker::getInstance ();
        myDomainId = pVcsNodePrincipalSelectionWorker->getMyDomainId (); 
        if (! pVcsNodePrincipalSelectionWorker->validDomainId (myDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage: getMyDomainId () returned invalid myDomainId = ")+myDomainId);
            return GENERIC_ERROR;
        }
        if (! pVcsNodePrincipalSelectionWorker->validDomainId (remoteDomainId))
        {
            trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage: invalid remoteDomainId  = ")+remoteDomainId);
            return GENERIC_ERROR;
        }
        myIpAddress = pVcsNodePrincipalSelectionWorker->getIpAddressFromDomainId (myDomainId);
        port = pVcsNodePrincipalSelectionWorker->getPortFromDomainId (myDomainId);
        remoteMgmtIpAddress = pVcsNodePrincipalSelectionWorker->getIpAddressFromDomainId (remoteDomainId);
        remoteMgmtPort = pVcsNodePrincipalSelectionWorker->getMgmtPort (remoteDomainId);
    }

    string myName = this->getClassName () + string (":") + myIpAddress + string (":") + port;

    // send cluster merge message to remote switch

    trace (TRACE_LEVEL_INFO, string("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage : setting up connection to remote wave server ")+ "From: " + myName + " To: " + " ipAddress: "+remoteMgmtIpAddress+" port: "+remoteMgmtPort);
    ValClientSynchronousConnection valClientSynchronousConnection (myName, remoteMgmtIpAddress, remoteMgmtPort);
    connectionStatus = valClientSynchronousConnection.connect ();
    if (WAVE_MGMT_CLIENT_INTF_CONNECTION_ESTABLISHED_TO_SERVER != connectionStatus)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage : Error establishing connection to remote switch domainId = ")+remoteSwitchInfo.switchDomainId);
        return GET_REMOTE_INFO_ERROR;
    }

    getLocalSwitchInfo (remoteSwitchInfo);
    status = WAVE_MESSAGE_SUCCESS;
    //status = valClientSynchronousConnection.sendClusterMergeMessage (remoteSwitchInfo);
    valClientSynchronousConnection.close ();
    if (valClientSynchronousConnection.isCurrentlyConnected ())
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage : Connection to server was not properly closed ipAddress = " )+remoteSwitchInfo.switchMgmtIpAddress);
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        trace (TRACE_LEVEL_ERROR, string ("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage : Error getting remote switch information for domainId = ")+remoteDomainId);
        return GET_REMOTE_INFO_ERROR;
    }
    else
    {
        trace (TRACE_LEVEL_SUCCESS, string("VcsNodeRemoteMessagingObjectManager::sendClusterMergeMessage : responderSerialNumber = ") + remoteSwitchInfo.switchSerialNumber + " respSWVer = " +remoteSwitchInfo.switchSWVersion + " IpAddr : " + remoteSwitchInfo.switchIpAddress + " Port: " + remoteSwitchInfo.switchPort);
        return VCS_SUCCESS;
    }
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DCM Message Handlers ////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void VcsNodeRemoteMessagingObjectManager::VcsRemoteEnqSwitchMessageHandler (VcsRemoteEnqSwitchMessage *pVcsRemoteEnqSwitchMessage)
{
    switchInfoParameters switchInfo;
    pVcsRemoteEnqSwitchMessage->getSwitchInfo (switchInfo);
    UI32 retCode = getRemoteSwitchInfo (switchInfo);
	pVcsRemoteEnqSwitchMessage->setSwitchInfo (switchInfo);
    if (VCS_SUCCESS != retCode)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeRemoteMessagingObjectManager::vcsFabEnqSwitchMessageHandler: error retrieving remote switch info");
        pVcsRemoteEnqSwitchMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_DEBUG, "VcsNodeRemoteMessagingObjectManager::vcsFabEnqSwitchMessageHandler: remote switch info successfully retrieved");
        pVcsRemoteEnqSwitchMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    reply (pVcsRemoteEnqSwitchMessage);
}

void VcsNodeRemoteMessagingObjectManager::VcsAsyncClusterMergeMessageHandler (VcsAsyncClusterMergeMessage *pVcsAsyncClusterMergeMessage)
{
    switchInfoParameters switchInfo;
    pVcsAsyncClusterMergeMessage->getSwitchInfo (switchInfo);
    UI32 retCode = sendClusterMergeMessage (switchInfo);
	pVcsAsyncClusterMergeMessage->setSwitchInfo (switchInfo);
    if (VCS_SUCCESS != retCode)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VcsNodeRemoteMessagingObjectManager::VcsAsyncClusterMergeMessageHandler: error retrieving remote switch info");
        pVcsAsyncClusterMergeMessage->setCompletionStatus (WAVE_MESSAGE_ERROR);
    }
    else
    {
        WaveNs::trace (TRACE_LEVEL_SUCCESS, "VcsNodeRemoteMessagingObjectManager::VcsAsyncClusterMergeMessageHandler: remote switch info successfully retrieved");
        pVcsAsyncClusterMergeMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    }
    reply (pVcsAsyncClusterMergeMessage);
}


}
*/
