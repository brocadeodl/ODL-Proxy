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
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#include "Utils/Capabilities.h"
#include "Vrf/Local/VrfLocalObjectManager.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "Vrf/Local/VrfAfIpv4UcastManagedObject.h"
#include "Vrf/Local/VrfLocalTypes.h"
#include "Vrf/Local/VrfRouteTargetManagedObject.h"
#include "Vrf/Local/VrfUtils.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Cluster/MultiPartition/Global/MultiPartitionMessages.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "ClientInterface/Nsm/nsmVrfIntf.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmCStatus.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
namespace DcmNs
{

VrfLocalObjectManager::VrfLocalObjectManager ()
    : WaveLocalObjectManager (getServiceName ())
{
    associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
    VrfLocalManagedObject   vrfLocalManagedObject(this);
    vrfLocalManagedObject.setupOrm ();
    addManagedClass (vrfLocalManagedObject.getClassName ());

    VrfAfIpv4UcastManagedObject vrfAfIpv4UcastManagedObject(this);
    vrfAfIpv4UcastManagedObject.setupOrm ();
    addManagedClass (vrfAfIpv4UcastManagedObject.getClassName ());

    VrfRouteTargetAfManagedObject vrfRouteTargetAfManagedObject(this);
    vrfRouteTargetAfManagedObject.setupOrm ();
    addManagedClass (vrfRouteTargetAfManagedObject.getClassName ());

    VrfRouteTargetManagedObject vrfRouteTargetManagedObject(this);
    vrfRouteTargetManagedObject.setupOrm ();
    addManagedClass (vrfRouteTargetManagedObject.getClassName ());

    addOperationMap (VRF_CREATE, reinterpret_cast<PrismMessageHandler> (&VrfLocalObjectManager::addVrfMessageHandler));
    addOperationMap (VRF_DELETE, reinterpret_cast<PrismMessageHandler> (&VrfLocalObjectManager::deleteVrfMessageHandler));
}

VrfLocalObjectManager::~VrfLocalObjectManager ()
{
}

string VrfLocalObjectManager::getServiceName ()
{
    return ("Vrf Local Service");
}

VrfLocalObjectManager *VrfLocalObjectManager::getInstance ()
{
    static VrfLocalObjectManager *pVrfLocalObjectManager = new VrfLocalObjectManager ();

    WaveNs::prismAssert (NULL != pVrfLocalObjectManager, __FILE__, __LINE__);

    return (pVrfLocalObjectManager);
}

PrismServiceId VrfLocalObjectManager::getPrismServiceId ()
{
    return ((getInstance ())->getServiceId ());
}

PrismMessage *VrfLocalObjectManager::createMessageInstance (const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {
        case VRF_CREATE :
            pPrismMessage = new VrfLocalAddVrfMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        case VRF_DELETE :
            pPrismMessage = new VrfLocalDeleteVrfMessage;
            prismAssert (NULL != pPrismMessage, __FILE__, __LINE__);
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *VrfLocalObjectManager::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (VrfLocalManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "VrfLocalObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new VrfLocalManagedObject (this);
    }
    else if(VrfAfIpv4UcastManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "VrfLocalObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new VrfAfIpv4UcastManagedObject(this);
    }

    else if(VrfRouteTargetAfManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "VrfLocalObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new VrfRouteTargetAfManagedObject(this);
    }
    else if(VrfRouteTargetManagedObject::getClassName() == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "VrfLocalObjectManager::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new VrfRouteTargetManagedObject(this);
    }
    else
    {
        trace(TRACE_LEVEL_ERROR, "VrfLocalObjectManager::createManagedObjectInstance : Incorrect managed object for this object manager : " + managedClassName);
    }

    return pWaveManagedObject;
}

void VrfLocalObjectManager::initialize (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager:initialize : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VrfLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId  status = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::install : Entering ...");

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VrfLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId  status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::boot : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("BootReason : ") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason));

    if (WAVE_BOOT_FIRST_TIME_BOOT != bootReason)
    {
         // TODO: Futures.
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VrfLocalObjectManager::haboot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    ResourceId status  = WAVE_MESSAGE_SUCCESS;

    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::haboot : Entering ...");

    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_INFO, string ("BootReason : ") + FrameworkToolKit::localizeToSourceCodeEnum (bootReason));

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    pWaveAsynchronousContextForBootPhases->callback ();
}

void VrfLocalObjectManager::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostbootPhase)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string passName = pWaveAsynchronousContextForPostbootPhase->getPassName();

    trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::postboot : Entering ... : passName [") + passName + string ("]"));

    // Vrf creation will be done only when the postboot pass is DCM_POSTBOOT_GLOBAL_STAGE1.
    // <Pass name = "DCM_POSTBOOT_GLOBAL_STAGE1">

    if (passName == "DCM_POSTBOOT_GLOBAL_STAGE1")
    {
        WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (VrfLocalManagedObject::getClassName ());
        LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
        waveManagedObjectSynchronousQueryContext.addSelectField("vrfName");

        vector<WaveManagedObject *> *pVrfLocalManagedObjectVector = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &waveManagedObjectSynchronousQueryContext);

        if ( NULL != pVrfLocalManagedObjectVector)
        {
            UI32 numberOfVrfLocalMOs = pVrfLocalManagedObjectVector->size();

            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::postboot : numberOfVrfLocalMOs [") + numberOfVrfLocalMOs + string ("]"));

            for (UI32 i = 0 ; i < numberOfVrfLocalMOs ; i++)
            {
                VrfLocalManagedObject *pVrfLocalManagedObject = dynamic_cast<VrfLocalManagedObject *> ((*pVrfLocalManagedObjectVector)[i]);
                prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);

                // Create Vrf in backend.
                string vrfName = pVrfLocalManagedObject->getVrfName ();

                ResourceId backendStatus = VrfUtils::backendVRFCreate (vrfName);

                trace(TRACE_LEVEL_INFO , string ("VrfLocalObjectManager::postboot vrfName [") + vrfName + string("], backend createstatus = [") + FrameworkToolKit::localizeToSourceCodeEnum (backendStatus) + string("]"));
                if (WAVE_MESSAGE_SUCCESS != backendStatus)
                {
                    status = backendStatus;
                    break;
                }
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pVrfLocalManagedObjectVector);
        }
    }

    // This is kludge, we are seperating VRF creation and vrf config restoration as backends are not handling
    // both being delivered at the same time.
    if (passName == "DCM_POSTBOOT_GLOBAL_STAGE2")
    {
        WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (VrfLocalManagedObject::getClassName ());
        LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
        vector<WaveManagedObject *> *pVrfLocalManagedObjectVector = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &waveManagedObjectSynchronousQueryContext);

        if ( NULL != pVrfLocalManagedObjectVector)
        {
            UI32 numberOfVrfLocalMOs = pVrfLocalManagedObjectVector->size();

            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::postboot : numberOfVrfLocalMOs [") + numberOfVrfLocalMOs + string ("]"));

            for (UI32 i = 0 ; i < numberOfVrfLocalMOs ; i++)
            {
                VrfLocalManagedObject *pVrfLocalManagedObject = dynamic_cast<VrfLocalManagedObject *> ((*pVrfLocalManagedObjectVector)[i]);
                prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);

                VrfUtils::restoreVrfRtmConfig(pVrfLocalManagedObject);
            }
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pVrfLocalManagedObjectVector);
        }
    }

    pWaveAsynchronousContextForPostbootPhase->setCompletionStatus (status);
    pWaveAsynchronousContextForPostbootPhase->callback ();
}

void VrfLocalObjectManager::addVrfMessageHandler (VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::validateInputsForAddVrf),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::requestBackendDaemonToCreateVRFStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::vrfMoStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext = new VrfLocalSynchronousLinearSequencerContext(pVrfLocalAddVrfMessage,
                                                                                                        this,
                                                                                                        sequencerSteps,
                                                                                                        sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

    pVrfLocalSynchronousLinearSequencerContext->execute ();
    return;
}

ResourceId VrfLocalObjectManager::validateInputsForAddVrf(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_ERROR;
    VrfLocalManagedObject *pVrfLocalManagedObject = NULL;
    UI32 vrfCount = 0, maxvrf = MAX_VRF_COUNT;
    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();

    if (Capabilities::instance()->isCapabilityPresent(MULTI_VRF_CAPABILITY)) {
		maxvrf = MAX_VRF_COUNT;
	} else if (Capabilities::instance()->isCapabilityPresent(MULTI_VRF_CAPABILITY_MAX_8)) {
		maxvrf = MIN_VRF_COUNT;
	} else {
        return (WRC_MULTI_VRF_CAPABILITY);
    }

    VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage = dynamic_cast<VrfLocalAddVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalAddVrfMessage, __FILE__, __LINE__);

	if (pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_RD_SET) {
		bool rdstatus = VrfUtils::isRdExists(pVrfLocalAddVrfMessage->getRd());
		if (rdstatus) {
			return (WRC_VRF_DUPLOCATE_RD_ERROR);
		}
	}
		

	if( pVrfLocalAddVrfMessage->getCmdCode() == NSM_MSG_VRF)
	{
		WaveManagedObjectSynchronousQueryContext ctxt (VrfLocalManagedObject::getClassName ());
		ctxt.setLocationIdFilter(thisLocationId);
		status = querySynchronouslyForCount(&ctxt, vrfCount);

		if(status !=  WAVE_MESSAGE_SUCCESS) {
			return status;
		}
		if(vrfCount >= maxvrf) {
			return WRC_VRF_ERR_MAX_VRF_COUNT;
    	}
	}


    // 1. get input parameters.
    string vrfName = pVrfLocalAddVrfMessage->getVrfName ();

    if (vrfName.empty())
    {
        return status;
    }

    if (vrfName.compare(DEFAULT_VRF_NAME) == 0)
    {
        return WRC_DEFAULT_VRF_RESERVE_ERROR;
    }


    // 2. query local MO for this location.

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (VrfLocalManagedObject::getClassName ());
    waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
    vector<WaveManagedObject *> *pVrfLocalManagedObjectVector = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &waveManagedObjectSynchronousQueryContext);

    // 3. If MO found, store in context.

    if (NULL == pVrfLocalManagedObjectVector)
    {
        return status;
    }

    UI32 numberOfVrfLocalManagedObjects = pVrfLocalManagedObjectVector->size ();

    trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForAddPartition : numberOfVrfLocalManagedObjects [") + numberOfVrfLocalManagedObjects + string ("]"));

    if (0 == numberOfVrfLocalManagedObjects)
    {
        trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForAddPartition : Need to create a new VrfLocalManagedObject"));
    }
    else if (1 == numberOfVrfLocalManagedObjects)
    {
        // store MO in context. (Cannot jump to success step from here !)
        trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForAddPartition : [") + vrfName + string ("] Managed Object alread exists"));

        pVrfLocalManagedObject = dynamic_cast<VrfLocalManagedObject *> ((*pVrfLocalManagedObjectVector)[0]);
        prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);


        pVrfLocalSynchronousLinearSequencerContext->setVrfLocalManagedObjectPointer (pVrfLocalManagedObject);
        //FIXME : Add to delete at the end of sequencer contxt
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pVrfLocalManagedObjectVector);
    }
    else
    {
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pVrfLocalManagedObjectVector);
        prismAssert (false, __FILE__, __LINE__);
    }
    if(pVrfLocalManagedObject)
    {
        if(pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_RD_SET && !pVrfLocalManagedObject->getRd().empty()  )
        {
            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputs Rd already configured"));
            return WRC_RTM_ERR_RD_CONFIGURED;
        }
        else if( (pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_AF_IPV4_SET || pVrfLocalAddVrfMessage->getCmdCode() 
                == RTM_MSG_CONFIG_MAX_ROUTES_SET) && (pVrfLocalManagedObject->getRd().empty() )   )
        {
            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputs Rd not configured"));
            return WRC_VRF_RTM_RD_NOT_CONFIGURED;
        }
    }

    delete pVrfLocalManagedObjectVector;

   return (WAVE_MESSAGE_SUCCESS);
}

ResourceId VrfLocalObjectManager::requestBackendDaemonToCreateVRFStep (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId backendStatus = WAVE_MESSAGE_SUCCESS;
    VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage = dynamic_cast<VrfLocalAddVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalAddVrfMessage, __FILE__, __LINE__);

    string vrfName = pVrfLocalAddVrfMessage->getVrfName ();

    trace(TRACE_LEVEL_INFO, string("VrfLocalObjectManager::requestBackendDaemonToCreateVRFStep: VRF= [") + vrfName + string("]") + string ("cmdcode:") + pVrfLocalAddVrfMessage->getCmdCode());

    switch(pVrfLocalAddVrfMessage->getCmdCode())
    {
    case NSM_MSG_VRF:
        backendStatus = VrfUtils::backendVRFCreate (vrfName);
        trace(TRACE_LEVEL_INFO, string("VrfLocalObjectManager::requestBackendDaemonToCreateVRFStep: Backend create VRF= [") + vrfName + string("] , VrfUtils::backendVRFCreate returned = [") + FrameworkToolKit::localizeToSourceCodeEnum (backendStatus) + string ("]"));
        break;
     default:
        backendStatus = VrfUtils::backendVRFUpdate (pVrfLocalAddVrfMessage);
        trace(TRACE_LEVEL_INFO, string("VrfLocalObjectManager::requestBackendDaemonToCreateVRFStep: Backend update VRF= [") + vrfName + string("] , VrfUtils::backendVRFUpdate returned = [") + FrameworkToolKit::localizeToSourceCodeEnum (backendStatus) + string ("]"));
        break;
    }


    return (backendStatus);
}

ResourceId VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (NULL != pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ())
    {
        return status;
    }

    VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage = dynamic_cast<VrfLocalAddVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalAddVrfMessage, __FILE__, __LINE__);

    string vrfName = pVrfLocalAddVrfMessage->getVrfName ();

    MultiPartitionAddPartitionMessage multiPartitionAddPartitionMessage (vrfName);

    ResourceId sendStatus = sendSynchronously (&multiPartitionAddPartitionMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = multiPartitionAddPartitionMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            // successfully created Wave Partition Managed Object.

            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject : success in MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("]"));
           ObjectId createdWavePartitionManagedObjectId = multiPartitionAddPartitionMessage.getCreatedWavePartitionManagedObjectId ();
           pVrfLocalSynchronousLinearSequencerContext->setReceivedWavePartitionManagedObjectId (createdWavePartitionManagedObjectId);
        }
        else
        {
            status = completionStatus;

            trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject : Failure in processing MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], completionStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
        }
    }
    else
    {
        status = sendStatus;

        trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject : Failure in sending MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], sendStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
    }

    return status;
}

ResourceId VrfLocalObjectManager::createVrfLocalManagedObjectStep (VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    string  vrfName  = pVrfLocalAddVrfMessage->getVrfName ();

    VrfLocalManagedObject *pVrfLocalManagedObject = new VrfLocalManagedObject(this);
    prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);

    pVrfLocalManagedObject->setVrfName(vrfName);
    ObjectId receivedWavePartitionManagedObjectId = pVrfLocalSynchronousLinearSequencerContext->getReceivedWavePartitionManagedObjectId ();
    pVrfLocalManagedObject->setOwnerPartitionManagedObjectId (receivedWavePartitionManagedObjectId);

    pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pVrfLocalManagedObject);
    pVrfLocalSynchronousLinearSequencerContext->setVrfLocalManagedObjectPointer(pVrfLocalManagedObject);

    return status;
}

ResourceId VrfLocalObjectManager::vrfMoStep(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Entering VrfLocalObjectManager::vrfMoStep");
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage = dynamic_cast<VrfLocalAddVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalAddVrfMessage, __FILE__, __LINE__);

    if (pVrfLocalAddVrfMessage->getCmdCode() == NSM_MSG_VRF && NULL != pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ())
    {
        return status;
    }

    trace (TRACE_LEVEL_INFO, "calling VrfLocalObjectManager::vrfMoStep cmdCode = " +  pVrfLocalAddVrfMessage->getCmdCode());
    switch(pVrfLocalAddVrfMessage->getCmdCode())
    {
    case NSM_MSG_VRF:
        status = createVrfLocalManagedObjectStep(pVrfLocalAddVrfMessage, pVrfLocalSynchronousLinearSequencerContext);
        break;
    case RTM_MSG_AF_IPV4_UNSET:
         requestDependentPluginsToCleanUpStaleEntrys(RTM_MSG_AF_IPV4_UNSET, pVrfLocalAddVrfMessage->getVrfName());
        /*fall through*/
    case RTM_MSG_AF_IPV4_SET:
    case RTM_MSG_CONFIG_MAX_ROUTES_SET:
    case RTM_MSG_CONFIG_MAX_ROUTES_UNSET:
        status = updateVrfAfIpv4Mo(pVrfLocalAddVrfMessage, pVrfLocalSynchronousLinearSequencerContext);
        break;
    case RTM_MSG_CONFIG_VRF_MODE_RT_SET:
    case RTM_MSG_CONFIG_VRF_MODE_RT_UNSET:
        status = updateVrfRtMo(pVrfLocalAddVrfMessage, pVrfLocalSynchronousLinearSequencerContext);
        break;
    case RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_SET:
    case RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_UNSET:
        status = updateVrfAfRtMo(pVrfLocalAddVrfMessage, pVrfLocalSynchronousLinearSequencerContext);
        break;
    default:
        status = updateVrfMo(pVrfLocalAddVrfMessage, pVrfLocalSynchronousLinearSequencerContext);
        break;
    }
    return status;
}

ResourceId VrfLocalObjectManager::updateVrfAfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                  VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Entering VrfLocalObjectManager::updateVrfAfRtMo");
    VrfLocalManagedObject *mo = pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
    VrfRouteTargetAfManagedObject *rtMo[2] = {NULL,NULL}; /*if rtType=both 1 = import 2 = export else only one pointer*/
    VrfAfIpv4UcastManagedObject *afMo = mo->getVrfAfIpv4();

    /*Rt mo can only be delete or created it ant be updated*/
    switch(pVrfLocalAddVrfMessage->getCmdCode() ) {
    case  RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_SET:
    {
        rtMo[0] = new VrfRouteTargetAfManagedObject(this);
        rtMo[0]->setVrfName(pVrfLocalAddVrfMessage->getVrfName());
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(rtMo[0]);
        if(pVrfLocalAddVrfMessage->getRtType() == RTM_MSG_CONFIG_RT_TYPE_BOTH) {
            rtMo[1] = new VrfRouteTargetAfManagedObject(this);
            rtMo[0]->setRtType(RTM_MSG_CONFIG_RT_TYPE_IMPORT);
            rtMo[0]->setRt(pVrfLocalAddVrfMessage->getRt());
            rtMo[1]->setRtType(RTM_MSG_CONFIG_RT_TYPE_EXPORT);
            rtMo[1]->setRt(pVrfLocalAddVrfMessage->getRt());
            rtMo[0]->setOwnerManagedObjectId(afMo->getObjectId());

            rtMo[1]->setVrfName(pVrfLocalAddVrfMessage->getVrfName());
            addToComposition(VrfAfIpv4UcastManagedObject::getClassName(), VrfRouteTargetAfManagedObject::getClassName(), "vrfRouteTarget",afMo->getObjectId(), rtMo[0]->getObjectId() );


            rtMo[1]->setOwnerManagedObjectId(afMo->getObjectId());
            pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(rtMo[1]);
            addToComposition(VrfAfIpv4UcastManagedObject::getClassName(), VrfRouteTargetAfManagedObject::getClassName(), "vrfRouteTarget",afMo->getObjectId(), rtMo[1]->getObjectId() );

        }
        else {
            rtMo[0]->setRtType(pVrfLocalAddVrfMessage->getRtType());
            rtMo[0]->setRt(pVrfLocalAddVrfMessage->getRt());
            rtMo[0]->setVrfName(pVrfLocalAddVrfMessage->getVrfName());

            rtMo[0]->setOwnerManagedObjectId(afMo->getObjectId());
            addToComposition(VrfAfIpv4UcastManagedObject::getClassName(), VrfRouteTargetAfManagedObject::getClassName(), "vrfRouteTarget",afMo->getObjectId(), rtMo[0]->getObjectId() );
        }
    }
    break;
    case RTM_MSG_CONFIG_VRF_AF_IPV4_MODE_RT_UNSET:
    {
        VrfUtils::getVrfAfRtMo(pVrfLocalAddVrfMessage, rtMo);
        if(rtMo[0]) {
            deleteFromComposition(VrfAfIpv4UcastManagedObject::getClassName(), VrfRouteTargetAfManagedObject::getClassName(), "vrfRouteTarget",afMo->getObjectId(), rtMo[0]->getObjectId() );
        }
        if(rtMo[1]) {
            deleteFromComposition(VrfAfIpv4UcastManagedObject::getClassName(), VrfRouteTargetAfManagedObject::getClassName(), "vrfRouteTarget",afMo->getObjectId(), rtMo[1]->getObjectId() );
        }
    }
        break;
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VrfLocalObjectManager::updateVrfRtMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                  VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Entering VrfLocalObjectManager::updateVrfRtMo");
    VrfLocalManagedObject *mo = pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
    /*Rt mo can only be delete or created it ant be updated*/
    VrfRouteTargetManagedObject *rtMo[2] = {NULL,NULL}; /*if rtType=both 1 = import 2 = export else only one pointer*/

    /*Rt mo can only be delete or created it ant be updated*/
    switch(pVrfLocalAddVrfMessage->getCmdCode() ) {
    case  RTM_MSG_CONFIG_VRF_MODE_RT_SET:
    {
        rtMo[0] = new VrfRouteTargetManagedObject(this);
        rtMo[0]->setVrfName(pVrfLocalAddVrfMessage->getVrfName());
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(rtMo[0]);
        if(pVrfLocalAddVrfMessage->getRtType() == RTM_MSG_CONFIG_RT_TYPE_BOTH) {
            rtMo[1] = new VrfRouteTargetManagedObject(this);
            rtMo[0]->setRtType(RTM_MSG_CONFIG_RT_TYPE_IMPORT);
            rtMo[0]->setRt(pVrfLocalAddVrfMessage->getRt());
            rtMo[1]->setRtType(RTM_MSG_CONFIG_RT_TYPE_EXPORT);
            rtMo[1]->setRt(pVrfLocalAddVrfMessage->getRt());

            rtMo[0]->setOwnerManagedObjectId(mo->getObjectId());
            rtMo[1]->setOwnerManagedObjectId(mo->getObjectId());

            rtMo[1]->setVrfName(pVrfLocalAddVrfMessage->getVrfName());
            pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(rtMo[1]);
            addToComposition(VrfLocalManagedObject::getClassName(), VrfRouteTargetManagedObject::getClassName(), "vrfRouteTarget",mo->getObjectId(), rtMo[0]->getObjectId() );
            addToComposition(VrfLocalManagedObject::getClassName(), VrfRouteTargetManagedObject::getClassName(), "vrfRouteTarget",mo->getObjectId(), rtMo[1]->getObjectId() );
        }
        else {
            rtMo[0]->setRtType(pVrfLocalAddVrfMessage->getRtType());
            rtMo[0]->setRt(pVrfLocalAddVrfMessage->getRt());
        //    mo->setRt(rtMo[0]);
            rtMo[0]->setOwnerManagedObjectId(mo->getObjectId());
            addToComposition(VrfLocalManagedObject::getClassName(), VrfRouteTargetManagedObject::getClassName(), "vrfRouteTarget",mo->getObjectId(), rtMo[0]->getObjectId() );
        }
    }
    break;
    case RTM_MSG_CONFIG_VRF_MODE_RT_UNSET:
    {
        VrfUtils::getVrfRtMo(pVrfLocalAddVrfMessage, rtMo);
        if(rtMo[0]) {
            deleteFromComposition(VrfLocalManagedObject::getClassName(), VrfRouteTargetManagedObject::getClassName(), "vrfRouteTarget",mo->getObjectId(), rtMo[0]->getObjectId() );
        }
        if(rtMo[1]) {
            deleteFromComposition(VrfLocalManagedObject::getClassName(), VrfRouteTargetManagedObject::getClassName(), "vrfRouteTarget",mo->getObjectId(), rtMo[1]->getObjectId() );
        }
    }
        break;
    }
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VrfLocalObjectManager::updateVrfAfIpv4Mo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage,
                  VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    trace (TRACE_LEVEL_INFO, "Entering VrfLocalObjectManager::updateVrfAfIpv4Mo");
    VrfLocalManagedObject *mo = pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
    VrfAfIpv4UcastManagedObject *afMo = NULL;
    if(NULL == mo)
    {
        trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::vrfMO not found");
        return WAVE_MESSAGE_ERROR;
    }
    if(pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_AF_IPV4_SET)
    {
        trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager:: creating VrfAfIpv4Mo");
        afMo = new VrfAfIpv4UcastManagedObject(this, pVrfLocalAddVrfMessage);
    }
    else if(pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_CONFIG_MAX_ROUTES_SET ||
            pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_CONFIG_MAX_ROUTES_UNSET)
    {
        VrfUtils::getVrfAfIpv4Mo(pVrfLocalAddVrfMessage->getVrfName(), afMo);
        if(!afMo) {
            /*A case of add ipv4 max-route <num> */
            afMo = new VrfAfIpv4UcastManagedObject(this, pVrfLocalAddVrfMessage);
        }
        else {
            return WRC_RTM_MAX_ROUTES_CANT_BE_CONFIGURED;
        }
        afMo->setMaxRoutes(pVrfLocalAddVrfMessage->getMaxRoutes());
    }
    else if( pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_AF_IPV4_UNSET)
    {
        VrfUtils::getVrfAfIpv4Mo(pVrfLocalAddVrfMessage->getVrfName(), afMo);
        delete afMo;
        afMo = NULL;
    }
    mo->setVrfAfIpv4(afMo);
    updateWaveManagedObject(mo);
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VrfLocalObjectManager::updateVrfMo(VrfLocalAddVrfMessage *pVrfLocalAddVrfMessage, 
              VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId ret = WAVE_MESSAGE_ERROR;
    VrfLocalManagedObject *mo = NULL;
    string vrfName = pVrfLocalAddVrfMessage->getVrfName();
    mo =  pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
    //VrfUtils::getVrfMo(vrfName, mo);
    if(!mo) 
        return ret;
    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::updateVrfMo. RD set/UNSET cmd code" + pVrfLocalAddVrfMessage->getCmdCode());
    switch(pVrfLocalAddVrfMessage->getCmdCode())
    {
    case RTM_MSG_RD_UNSET:
         requestDependentPluginsToCleanUpStaleEntrys(RTM_MSG_RD_UNSET, vrfName);
         /*fall through*/ 
    case RTM_MSG_RD_SET:
        mo->setRd(pVrfLocalAddVrfMessage->getRd(), pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_RD_SET);
        
        break;
    case RTM_MSG_CONFIG_ROUTER_ID_SET:
    case RTM_MSG_CONFIG_ROUTER_ID_UNSET:
        mo->setRouterId(pVrfLocalAddVrfMessage->getRouterId(), pVrfLocalAddVrfMessage->getCmdCode() == RTM_MSG_CONFIG_ROUTER_ID_SET );
        break;
    }
    updateWaveManagedObject(mo);
    //this MO is added to garbage collector in validation step.
    //pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(mo);
    return WAVE_MESSAGE_SUCCESS;
}

ResourceId VrfLocalObjectManager::requestDependentPluginsToCleanUpStaleEntrys(UI32 tag, const string &vrfName)
{
   MultiPartitionDeletePartitionMessage multiPartitionDeletePartitionMessage (vrfName);
   multiPartitionDeletePartitionMessage.setPartialCleanupTag(tag);
   multiPartitionDeletePartitionMessage.setSenderServiceId(getPrismServiceId());

   ResourceId sendStatus = sendSynchronously (&multiPartitionDeletePartitionMessage);
   ResourceId status = sendStatus;

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = multiPartitionDeletePartitionMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            // successfully created Wave Partition Managed Object.

            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::requestDependentPluginsToCleanUpStaleEntrys : success in MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("]"));
        }
        else
        {
            status = completionStatus;
            trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestDependentPluginsToCleanUpStaleEntrys : Failure in processing MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], completionStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
        }
    }
    else
    {
        status = sendStatus;

        trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestDependentPluginsToCleanUpStaleEntrys : Failure in sending MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], sendStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
    }
    return status;
}

void VrfLocalObjectManager::deleteVrfMessageHandler(VrfLocalDeleteVrfMessage *pVrfLocalDeleteVrfMessage)
{
   // TODO: Delete sequencer/trigger failover code.
     PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::validateInputsForDeleteVrf),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::requestBackendDaemonToDeleteVRFStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::requestFrameworkToCleanupPartitionManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::cleanupVrfManagedObject),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VrfLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    };

    VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext = new VrfLocalSynchronousLinearSequencerContext(pVrfLocalDeleteVrfMessage,
                                                                                                        this,
                                                                                                        sequencerSteps,
                                                                                                        sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    pVrfLocalSynchronousLinearSequencerContext->execute ();
    return;
}


ResourceId VrfLocalObjectManager::validateInputsForDeleteVrf(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    VrfLocalDeleteVrfMessage *pVrfLocalDeleteVrfMessage = dynamic_cast<VrfLocalDeleteVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());

    prismAssert(NULL != pVrfLocalDeleteVrfMessage , __FILE__, __LINE__);
    string       vrfName              = pVrfLocalDeleteVrfMessage->getVrfName ();
    ResourceId   status  = WAVE_MESSAGE_ERROR;

    // Add a new entry into the log
    trace (TRACE_LEVEL_INFO, "VrfLocalObjectManager::validateInputsForDeletePartition called.");

    if (vrfName.empty())
    {
        return status;
    }

    WaveManagedObjectSynchronousQueryContext waveManagedObjectSynchronousQueryContext (VrfLocalManagedObject::getClassName ());
    waveManagedObjectSynchronousQueryContext.addAndAttribute (new AttributeString (&vrfName, "vrfName"));
    LocationId thisLocationId = FrameworkToolKit::getThisLocationId ();
    vector<WaveManagedObject *> *pVrfLocalManagedObjectVector = querySynchronouslyLocalManagedObjectsForLocationId (thisLocationId, &waveManagedObjectSynchronousQueryContext);

    // 3. If MO found, store in context.
    if (NULL == pVrfLocalManagedObjectVector)
    {
        return status;
    }

    UI32 numberOfVrfLocalManagedObjects = pVrfLocalManagedObjectVector->size ();

    trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForDeleteVrf : numberOfVrfLocalManagedObjects [") + numberOfVrfLocalManagedObjects + string ("]"));

    if (0 == numberOfVrfLocalManagedObjects)
    {
        trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForDeleteVrf : VrfLocalManagedObject [") + vrfName + string ("] already deleted."));
    }
    else if (1 == numberOfVrfLocalManagedObjects)
    {
        // Store this Object in the context to be deleted in the next sequencer step.
        trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::validateInputsForDeletePartition : [") + vrfName + string ("] Managed Object exists."));

        VrfLocalManagedObject *pVrfLocalManagedObject = dynamic_cast<VrfLocalManagedObject *> ((*pVrfLocalManagedObjectVector)[0]);
        prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);

        pVrfLocalSynchronousLinearSequencerContext->setVrfLocalManagedObjectPointer (pVrfLocalManagedObject);
    //  pVrfLocalSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pVrfLocalManagedObjectVector);
    }
    else
    {
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection (*pVrfLocalManagedObjectVector);
        prismAssert (false, __FILE__, __LINE__);
    }

    delete pVrfLocalManagedObjectVector;

   return (WAVE_MESSAGE_SUCCESS);

}

ResourceId VrfLocalObjectManager::requestBackendDaemonToDeleteVRFStep (VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    VrfLocalDeleteVrfMessage *pVrfLocalDeleteVrfMessage = dynamic_cast<VrfLocalDeleteVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalDeleteVrfMessage, __FILE__, __LINE__);

    string vrfName = pVrfLocalDeleteVrfMessage->getVrfName ();

    trace(TRACE_LEVEL_INFO, string("VrfLocalObjectManager::requestBackendDaemonToDeleteVRFStep: Requesting Backend to delete VRF = [") + vrfName + string("]"));

    ResourceId backendStatus = VrfUtils::backendVRFDelete (vrfName);

    trace(TRACE_LEVEL_INFO, string("VrfLocalObjectManager::requestBackendDaemonToDeleteVRFStep: Backend Delete VRF= [") + vrfName + string("] , VrfUtils::doBackendVRFDelete returned = [") + FrameworkToolKit::localizeToSourceCodeEnum (backendStatus) + string ("]"));

    return (backendStatus);
}
ResourceId VrfLocalObjectManager::requestFrameworkToCleanupPartitionManagedObject(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{

    ResourceId status = WAVE_MESSAGE_SUCCESS;

    if (NULL == pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ())
    {
        // Already deleted.
        return status;
    }

    VrfLocalDeleteVrfMessage *pVrfLocalDeleteVrfMessage = dynamic_cast<VrfLocalDeleteVrfMessage *> (pVrfLocalSynchronousLinearSequencerContext->getPPrismMessage ());
    prismAssert(NULL != pVrfLocalDeleteVrfMessage, __FILE__, __LINE__);

    string vrfName = pVrfLocalDeleteVrfMessage->getVrfName ();

    MultiPartitionDeletePartitionMessage multiPartitionDeletePartitionMessage (vrfName);
    multiPartitionDeletePartitionMessage.setSenderServiceId(getPrismServiceId());

    ResourceId sendStatus = sendSynchronously (&multiPartitionDeletePartitionMessage);

    if (WAVE_MESSAGE_SUCCESS == sendStatus)
    {
        ResourceId completionStatus = multiPartitionDeletePartitionMessage.getCompletionStatus ();

        if (WAVE_MESSAGE_SUCCESS == completionStatus)
        {
            // successfully created Wave Partition Managed Object.

            trace (TRACE_LEVEL_INFO, string ("VrfLocalObjectManager::requestFrameworkToCleanupPartitionManagedObject : success in MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("]"));
        }
        else
        {
            status = completionStatus;
            trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestFrameworkToCleanupPartitionManagedObject : Failure in processing MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], completionStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (completionStatus) + string ("]"));
        }
    }
    else
    {
        status = sendStatus;

        trace (TRACE_LEVEL_ERROR, string ("VrfLocalObjectManager::requestFrameworkToCreatePartitionManagedObject : Failure in sending MultiPartitionAddPartitionMessage, for vrf [") + vrfName + string ("], sendStatus [")+ FrameworkToolKit::localizeToSourceCodeEnum (sendStatus) + string ("]"));
    }

    if (WAVE_MESSAGE_SUCCESS != status)
    {
        VrfLocalManagedObject *pVrfLocalManagedObject = pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
        prismAssert(NULL != pVrfLocalManagedObject, __FILE__, __LINE__);
        pVrfLocalSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pVrfLocalManagedObject);
    }
    return status;
}



ResourceId VrfLocalObjectManager::cleanupVrfManagedObject(VrfLocalSynchronousLinearSequencerContext *pVrfLocalSynchronousLinearSequencerContext)
{
    ResourceId status = WAVE_MESSAGE_SUCCESS;

    VrfLocalManagedObject *pVrfLocalManagedObject =  pVrfLocalSynchronousLinearSequencerContext->getVrfLocalManagedObjectPointer ();
    if (NULL != pVrfLocalManagedObject)
    {
        delete pVrfLocalManagedObject;
    }

    return (status);
}


}
