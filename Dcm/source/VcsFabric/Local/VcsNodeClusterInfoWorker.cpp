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
 *   Copyright (C) 2010-2013 Brocade Communications Systems, Inc.           *
 *   All rights reserved.                                                   *
 *   Author : Anand Subramanian                                             *
 ***************************************************************************/

#include "VcsFabric/Local/VcsNodeClusterInfoWorker.h"
#include "VcsFabric/Local/VcsNodeLocalMessagingContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Ipadm/Local/IpadmChassisManagedObject.h"
#include "Ipadm/Local/IpadmLocalManagedObject.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "RAS/Local/LoggingLocalManagedObject.h"
#include "VcsFabric/Global/VirtualIpGlobalWorker.h"
#include "Framework/Attributes/AttributeResourceId.h"

namespace DcmNs
{

VcsNodeClusterInfoWorker::VcsNodeClusterInfoWorker ( WaveObjectManager* pWaveObjectManager )
    : WaveWorker ( pWaveObjectManager )
{
    addOperationMap (VCS_NODE_COLLECT_SWITCH_IDENTIFIER_INFO, reinterpret_cast<PrismMessageHandler> (&VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoMessageHandler));
    addOperationMap (VIRTUAL_IP_LOCAL_SHOW_VIP,    reinterpret_cast<PrismMessageHandler> (&VcsNodeClusterInfoWorker::virtualIpLocalShowVipMessageHandler));
}

VcsNodeClusterInfoWorker::~VcsNodeClusterInfoWorker ()
{
}

PrismMessage *VcsNodeClusterInfoWorker::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VCS_NODE_COLLECT_SWITCH_IDENTIFIER_INFO :
            pPrismMessage = new VcsFabricCollectSwitchIdentifierInfoMessage ();
            break;
        case VIRTUAL_IP_LOCAL_SHOW_VIP :
            pPrismMessage = new VirtualIpLocalShowVipMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

void VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoMessageHandler (VcsFabricCollectSwitchIdentifierInfoMessage *pVcsFabricCollectSwitchIdentifierInfoMessage)
{
    PrismLinearSequencerContext *pPrismLinearSequencerContext = NULL;

    trace (TRACE_LEVEL_DEVEL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoMessageHandler Begin");

    if (DCM_CCM == DcmToolKit::getDistributionMode ())
    {
        trace (TRACE_LEVEL_DEVEL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoMessageHandler Management cluster");
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerFailedStep),
        };

        pPrismLinearSequencerContext = new PrismLinearSequencerContext (pVcsFabricCollectSwitchIdentifierInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }
    else if (CCM_ONLY == DcmToolKit::getDistributionMode ())
    {
        trace (TRACE_LEVEL_DEVEL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoMessageHandler Fabric cluster");
        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerFailedStep),
        };

        pPrismLinearSequencerContext = new PrismLinearSequencerContext (pVcsFabricCollectSwitchIdentifierInfoMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    }

    pPrismLinearSequencerContext->start ();
}

void VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    vector<WaveManagedObject *> *pVcsLocalNodes = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

    prismAssert (NULL != pVcsLocalNodes, __FILE__, __LINE__);

    UI32                         numberOfNodes  = pVcsLocalNodes->size ();

    vector<UI32>                 rbridgeIdVector;
    vector<string>               switchnameVector;
    vector<string>               eth0IpAddressVector;
    vector<string>               eth0IPv6AddressVector;
    bool                         chassisFlag (false);
    string                       hostName;
    string                       vipAddress,vipV6Address,ipV4Address,ipV6Address;

    for (UI32 j = 0; j < numberOfNodes; j++)
    {
        VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast <VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[j]);
        vector<WaveManagedObject *> *pResults = NULL;

        // Initialize all to empty for each iteration
        vipAddress = vipV6Address = ipV4Address = ipV6Address = "";

        if (NULL != pVcsNodeLocalManagedObject)
        {
            SI32 rbridgeIdOfLocation = pVcsNodeLocalManagedObject->getDomainId ();

            UI32 locationId = pVcsNodeLocalManagedObject->getLocationId ();
    
            WaveManagedObjectSynchronousQueryContext    synchronousQueryContext   (WaveNode::getClassName ());
            synchronousQueryContext.addAndAttribute (new AttributeResourceId (WAVE_NODE_START_AS_CLIENT, "waveNodeStartMode"));
            synchronousQueryContext.addAndAttribute (new AttributeLocationId (locationId, "locationId"));

            vector<WaveManagedObject *>   *pWaveNodes = querySynchronously (&synchronousQueryContext);

            prismAssert (NULL != pWaveNodes, __FILE__, __LINE__);

            if (0 == pWaveNodes->size ())
            {
                pWaveNodes->clear ();
                delete pWaveNodes;
                
                continue;
            }
            else
            {
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pWaveNodes);
            }

            pResults = querySynchronouslyLocalManagedObjectsForLocationId (pVcsNodeLocalManagedObject->getLocationId (), IpadmChassisManagedObject::getClassName ());
            if ( (NULL != pResults) && (pResults->size ()) > 0)
            {
                // We get an entry if the switch is chassis
                chassisFlag = true;

                IpadmChassisManagedObject *pChassisMo = dynamic_cast<IpadmChassisManagedObject *> ((*pResults)[0]);
                if (pChassisMo)
                {
                    // Obtain chassis virtual ip address
                    vipAddress   = pChassisMo->getVipAddress ();
                    vipV6Address = pChassisMo->getVipV6Address ();
                }
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
            }
            else
            {
                // Not chassis case
                pResults = querySynchronouslyLocalManagedObjectsForLocationId (pVcsNodeLocalManagedObject->getLocationId (), IpadmLocalManagedObject::getClassName ());
                if ( (NULL != pResults) && (pResults->size ()) > 0)
                {
                    IpadmLocalManagedObject *pIpMo = dynamic_cast<IpadmLocalManagedObject *> ((*pResults)[0]);
                    if (pIpMo)
                    {
                        // Obtain ipv4, ipv6 address
                        ipV4Address = pIpMo->getIPv4Address ();
                        ipV6Address = pIpMo->getIPv6Address ();
                    }
                    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
                }
                else
                {
                    trace (TRACE_LEVEL_ERROR, string ("VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoStep : could not get ip details for locationId : ") + pVcsNodeLocalManagedObject->getLocationId ());
                }
            }

            pResults = querySynchronouslyLocalManagedObjectsForLocationId (pVcsNodeLocalManagedObject->getLocationId (), LoggingLocalManagedObject::getClassName ());
            if ( (NULL != pResults) && (pResults->size ()) > 0)
            {
                LoggingLocalManagedObject *pLogMo = dynamic_cast<LoggingLocalManagedObject *> ((*pResults)[0]);
                if (pLogMo)
                {
                    // Obtain hostname
                    hostName = pLogMo->getHostName ();
                }
                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
            }

            if (0 != rbridgeIdOfLocation)
            {
                rbridgeIdVector.push_back (rbridgeIdOfLocation);
                switchnameVector.push_back (hostName);

                if (chassisFlag)
                {
                    StringUtils::stripFromFirstOccuranceOf (vipAddress);   // Remove '/' from address
                    StringUtils::stripFromFirstOccuranceOf (vipV6Address);
                    eth0IpAddressVector.push_back   (vipAddress);
                    eth0IPv6AddressVector.push_back (vipV6Address);
                }
                else
                {
                    StringUtils::stripFromFirstOccuranceOf (ipV4Address);
                    StringUtils::stripFromFirstOccuranceOf (ipV6Address);
                    eth0IpAddressVector.push_back   (ipV4Address);
                    eth0IPv6AddressVector.push_back (ipV6Address);
                }
            }
            else
            {
                tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoStep : could not get valid rbridgeId for locationId : %d", pVcsNodeLocalManagedObject->getLocationId ());
            }
        }

        chassisFlag = false; //Reset flag for next iteration
    }

    VcsFabricCollectSwitchIdentifierInfoMessage* pVcsFabricCollectSwitchIdentifierInfoMessage = dynamic_cast<VcsFabricCollectSwitchIdentifierInfoMessage *>(pPrismLinearSequencerContext->getPPrismMessage ());

    if (NULL != pVcsFabricCollectSwitchIdentifierInfoMessage)
    {
        pVcsFabricCollectSwitchIdentifierInfoMessage->setRbridgeIdVector (rbridgeIdVector);
        pVcsFabricCollectSwitchIdentifierInfoMessage->setSwitchnameVector (switchnameVector);
        pVcsFabricCollectSwitchIdentifierInfoMessage->setEth0IpAddressVector (eth0IpAddressVector);
        pVcsFabricCollectSwitchIdentifierInfoMessage->setEth0IPv6AddressVector (eth0IPv6AddressVector);
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pVcsLocalNodes);
    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    VcsFabricGetSwitchIdentifierInfoMessage *pVcsFabricGetSwitchIdentifierInfoMessage   = new VcsFabricGetSwitchIdentifierInfoMessage;
    WaveSendToClusterContext            *pWaveSendToClusterContext              = new WaveSendToClusterContext (this, reinterpret_cast<PrismAsynchronousCallback> (&VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback), pPrismLinearSequencerContext);

    pWaveSendToClusterContext->setPPrismMessageForPhase1 (pVcsFabricGetSwitchIdentifierInfoMessage);

    vector<LocationId> locationIdsForSendToWaveCluster;

    // 1. get FullyConnectedLocations

    vector<LocationId> fullyConnectedLocationIdVector;
    FrameworkToolKit::getFullyConnectedLocations (fullyConnectedLocationIdVector);
    UI32 numberOfFullyConnectedNodes = fullyConnectedLocationIdVector.size ();

    // 2. Collect location ids from FullyConnectedLocations's for which VcsNodeLocalManagedObject is present in db.
    vector<WaveManagedObject *> *pVcsLocalNodes = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

    prismAssert (NULL != pVcsLocalNodes, __FILE__, __LINE__);

    UI32                         numberOfNodes  = pVcsLocalNodes->size ();

    for (UI32 i = 0; i < numberOfFullyConnectedNodes; i++)
    {
        for (UI32 j = 0; j < numberOfNodes; j++)
        {
            VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast <VcsNodeLocalManagedObject *> ((*pVcsLocalNodes)[j]);

            if (NULL == pVcsNodeLocalManagedObject)
            {
                trace (TRACE_LEVEL_FATAL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterStep : dynamic cast to VcsNodeLocalManagedObject pointer failed.");
                prismAssert (false, __FILE__, __LINE__);
            }

            if (fullyConnectedLocationIdVector [i] == pVcsNodeLocalManagedObject->getLocationId ())
            {
                locationIdsForSendToWaveCluster.push_back (fullyConnectedLocationIdVector [i]);
            }
        }
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pVcsLocalNodes);

    // 3. Add Local location to vector

    locationIdsForSendToWaveCluster.push_back (FrameworkToolKit::getThisLocationId ());

    // 4. call sendToWaveCluster for filtered locations.

    pWaveSendToClusterContext->setLocationsToSendToForPhase1 (locationIdsForSendToWaveCluster);

    sendToWaveCluster (pWaveSendToClusterContext);
}

void VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
{
    VcsFabricGetSwitchIdentifierInfoMessage *pVcsFabricGetSwitchIdentifierInfoMessage = NULL;

    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());

    if (NULL != pPrismLinearSequencerContext)
    {
        VcsFabricCollectSwitchIdentifierInfoMessage* pVcsFabricCollectSwitchIdentifierInfoMessage = dynamic_cast<VcsFabricCollectSwitchIdentifierInfoMessage *>(pPrismLinearSequencerContext->getPPrismMessage ());
        if (NULL != pVcsFabricCollectSwitchIdentifierInfoMessage)
        {
            vector<LocationId>  locations           = pWaveSendToClusterContext->getLocationsToSendToForPhase1();
            UI32                numberOfLocations   = locations.size ();

            vector<UI32>        rbridgeIdVector;
            vector<string>      switchnameVector;
            vector<string>      eth0IpAddressVector;
            vector<string>      eth0IPv6AddressVector;

            for (UI32 i = 0; i < numberOfLocations; i++)
            {
                WaveMessageStatus sendStatusForPhase1 = pWaveSendToClusterContext->getSendStatusForPhase1 (locations[i]);

                if (WAVE_MESSAGE_SUCCESS != sendStatusForPhase1)
                {
                    tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : location [%d], sendStatusForPhase1 [0x%x]", locations[i], sendStatusForPhase1);

                    continue;
                }

                FrameworkStatus frameworkStatus = pWaveSendToClusterContext->getFrameworkStatusForPhase1 (locations[i]);

                if (FRAMEWORK_SUCCESS != frameworkStatus)
                {
                    tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : location [%d], frameworkStatus [0x%x]", locations[i], frameworkStatus);

                    continue;
                }

                pVcsFabricGetSwitchIdentifierInfoMessage = dynamic_cast<VcsFabricGetSwitchIdentifierInfoMessage *> (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));

                if (NULL == pVcsFabricGetSwitchIdentifierInfoMessage)
                {
                    trace (TRACE_LEVEL_FATAL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : dynamic cast from ResultingMessageForPhase1 failed.");
                    prismAssert (false, __FILE__, __LINE__);
                }

                if (WAVE_MESSAGE_SUCCESS == pVcsFabricGetSwitchIdentifierInfoMessage->getCompletionStatus ())
                {
                    SI32 rbridgeIdOfLocation    = DcmToolKit::getDomainIdFromLocationId (locations[i]);

                    if (0 != rbridgeIdOfLocation)
                    {
                        rbridgeIdVector.push_back (rbridgeIdOfLocation);
                        switchnameVector.push_back (pVcsFabricGetSwitchIdentifierInfoMessage->getSwitchname ());
                        eth0IpAddressVector.push_back (pVcsFabricGetSwitchIdentifierInfoMessage->getEth0IpAddress ());
                        eth0IPv6AddressVector.push_back (pVcsFabricGetSwitchIdentifierInfoMessage->getEth0IPv6Address ());
                    }
                    else
                    {
                        tracePrintf (TRACE_LEVEL_ERROR, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : could not get valid rbridgeId for locationId : %d", locations[i]);
                    }
                }
            }
            pVcsFabricCollectSwitchIdentifierInfoMessage->setRbridgeIdVector (rbridgeIdVector);
            pVcsFabricCollectSwitchIdentifierInfoMessage->setSwitchnameVector (switchnameVector);
            pVcsFabricCollectSwitchIdentifierInfoMessage->setEth0IpAddressVector (eth0IpAddressVector);
            pVcsFabricCollectSwitchIdentifierInfoMessage->setEth0IPv6AddressVector (eth0IPv6AddressVector);

            delete (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());
            delete pWaveSendToClusterContext;

            pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : dynamic cast to VcsFabricCollectSwitchIdentifierInfoMessage pointer failed.");
            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VcsNodeClusterInfoWorker::collectSwitchIdentifierInfoSendToClusterCallback : PrismLinearSequencerContext pointer is NULL.");
        prismAssert (false, __FILE__, __LINE__);
    }
}

void VcsNodeClusterInfoWorker::virtualIpLocalShowVipMessageHandler (VirtualIpLocalShowVipMessage *pVirtualIpLocalShowVipMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsNodeClusterInfoWorker::virtualIpLocalShowVipMessageHandler : IP address show message"));

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::getBackendVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VcsNodeClusterInfoWorker::prismLinearSequencerFailedStep)
    };

    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pVirtualIpLocalShowVipMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pPrismLinearSequencerContext, __FILE__, __LINE__);

    pPrismLinearSequencerContext->holdAll ();
    pPrismLinearSequencerContext->start ();
}

void VcsNodeClusterInfoWorker::getBackendVirtualIpStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    string                          virtualIpAddressBoundToThisNode = "";
    UI32                            pNodeId                         = DcmToolKit::getLocalMappedId();

    VirtualIpLocalShowVipMessage  *pVirtualIpLocalShowVipMessage  = dynamic_cast<VirtualIpLocalShowVipMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pVirtualIpLocalShowVipMessage, __FILE__, __LINE__);

    virtualIpAddressBoundToThisNode = VirtualIpGlobalWorker::getCurrentVirtualIpAddress();

//    trace (TRACE_LEVEL_INFO, string ("VcsNodeClusterInfoWorker::getBackendVirtualIpStep : Virtual IP address = ") + virtualIpAddressBoundToThisNode + ", node ID = " + pNodeId);

    pVirtualIpLocalShowVipMessage->setVirtualIpAddress (virtualIpAddressBoundToThisNode);
    pVirtualIpLocalShowVipMessage->setpNodeId (pNodeId);

    pPrismLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

}
