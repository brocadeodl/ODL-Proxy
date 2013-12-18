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
 *	Copyright (C) 2005-2012 Brocade Communications Systems, Inc.		  *
 *	All rights reserved.												  *
 *	Author : cshah, Brian Adaniya									      *
 **************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Global/VirtualIpGlobalWorker.h"
#include "VcsFabric/Global/VirtualIpGlobalWorkerSequencerContext.h"
#include "VirtualIp/Global/VirtualIpGlobalManagedObject.h"
#include "VirtualIp/Global/VirtualIpGlobalConfigVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalShowVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalDeleteVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalBindVipMessage.h"
#include "VirtualIp/Global/VirtualIpGlobalUnbindVipMessage.h"
#include "VcsFabric/Global/VcsFabObjectManagerTypes.h"
#include "VcsFabric/Local/VcsNodePrincipalSelectionWorker.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include <ha/ipadm.h>

namespace DcmNs
{

VirtualIpGlobalWorker::VirtualIpGlobalWorker (VcsFabObjectManager* pWaveObjectManager)
    : WaveWorker (pWaveObjectManager)
{
    VirtualIpGlobalManagedObject	virtualIpGlobalManagedObject	(pWaveObjectManager);
    virtualIpGlobalManagedObject.setupOrm ();

    addManagedClass (VirtualIpGlobalManagedObject::getClassName (), this);

    addOperationMap (VIRTUAL_IP_GLOBAL_CONFIG_VIP,  reinterpret_cast<PrismMessageHandler> (&VirtualIpGlobalWorker::virtualIpGlobalConfigVipMessageHandler));
    addOperationMap (VIRTUAL_IP_GLOBAL_DELETE_VIP,  reinterpret_cast<PrismMessageHandler> (&VirtualIpGlobalWorker::virtualIpGlobalDeleteVipMessageHandler));
    addOperationMap (VIRTUAL_IP_GLOBAL_BIND_VIP,    reinterpret_cast<PrismMessageHandler> (&VirtualIpGlobalWorker::virtualIpGlobalBindVipMessageHandler));
    addOperationMap (VIRTUAL_IP_GLOBAL_UNBIND_VIP,  reinterpret_cast<PrismMessageHandler> (&VirtualIpGlobalWorker::virtualIpGlobalUnbindVipMessageHandler));

    m_pVirtualIpBindTimer = VirtualIpBindTimer::getInstance ();
}

VirtualIpGlobalWorker::~VirtualIpGlobalWorker ()
{
    delete m_pVirtualIpBindTimer;
}

PrismMessage *VirtualIpGlobalWorker::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    switch (operationCode)
    {

        case VIRTUAL_IP_GLOBAL_CONFIG_VIP :
            pPrismMessage = new VirtualIpGlobalConfigVipMessage ();
            break;
        case VIRTUAL_IP_GLOBAL_DELETE_VIP :
            pPrismMessage = new VirtualIpGlobalDeleteVipMessage ();
            break;
        case VIRTUAL_IP_GLOBAL_BIND_VIP :
            pPrismMessage = new VirtualIpGlobalBindVipMessage ();
            break;
        case VIRTUAL_IP_GLOBAL_UNBIND_VIP :
            pPrismMessage = new VirtualIpGlobalUnbindVipMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

WaveManagedObject *VirtualIpGlobalWorker::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if ((VirtualIpGlobalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new VirtualIpGlobalManagedObject(dynamic_cast<VcsFabObjectManager *>(getPWaveObjectManager ()));
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, "VirtualIpGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
    }
    return (pWaveManagedObject);
}

void  VirtualIpGlobalWorker::postboot (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
{
    SI8 PassNum  = pWaveAsynchronousContextForPostBootPhase->getPassNum();
    trace (TRACE_LEVEL_INFO, string("VirtualIpGlobalWorker::postboot  Pass ") + PassNum);

    //	WaveBootReason bootReason = pWaveAsynchronousContextForPostBootPhase->getBootReason();
    //	if ((Principal) || (clutserType == MGMT && no of nodes == 1))
    //	replay virtual ip address

    ResourceId clusterType = DcmToolKit::getThisVcsNodeClusterType();
    
    vector<WaveManagedObject* > *pResultVcsNodeLocalMO = querySynchronously(VcsNodeLocalManagedObject::getClassName());
    prismAssert(NULL!=pResultVcsNodeLocalMO, __FILE__, __LINE__);

    UI32 numVcsNodeLocalManagedObjects = pResultVcsNodeLocalMO->size ();

    VcsFabricLocalObjectManager *pVcsFabricLocalWorker = VcsFabricLocalObjectManager::getInstance();
    
    if (pVcsFabricLocalWorker->getThisVcsNodeLocalManagedObjectIsPrincipal () || ((clusterType == VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT) && (numVcsNodeLocalManagedObjects == 1)))
    {
        trace (TRACE_LEVEL_INFO, string("VirtualIpGlobalWorker::postboot single node clutser detected ") + pVcsFabricLocalWorker->getThisVcsNodeLocalManagedObjectIsPrincipal () + string(" clusterType = ") + clusterType + string(" numVcsNodeLocalManagedObjects ") + numVcsNodeLocalManagedObjects);

        WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(VirtualIpGlobalManagedObject::getClassName());

        vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

        if (NULL == pResults)
        {
            trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::postboot : No such managed object");
        } 
        else if (pResults->size() == 0)
        {
            trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::postboot : pResults size 0");
        }
        else
        {	
            VirtualIpGlobalManagedObject *pVipMo = dynamic_cast<VirtualIpGlobalManagedObject *> ((*pResults)[0]);
            prismAssert(pVipMo != NULL, __FILE__, __LINE__);

            string ipAddress = pVipMo->getVipAddress();

            trace (TRACE_LEVEL_INFO, string("VirtualIpGlobalWorker::postboot ipAddress from database: ") + ipAddress);

            int rc = virtualIpAddressBind (ipAddress);

            if (rc)
            {
                trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::postboot : VirtualIpAddressBind () failed");
                // 	TODO RASLOG
            }
            else 
            {
                trace (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::postboot : ipAdmIpAddrSet Succeeded!");
            }
        }

        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
        delete synchronousQueryContext;
    }

    WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResultVcsNodeLocalMO);
    
    pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForPostBootPhase->callback ();
}

void VirtualIpGlobalWorker::shutdown (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases)
{
    trace (TRACE_LEVEL_DEBUG, "VcsFabObjectManager::shutdown : Entering ...");

    if (FrameworkToolKit::isWarmHaRecoveryPreparationInProgress () == false)
    {
        int rc = virtualIpAddressUnBind ();

        if (rc)
        {
            trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::shutdown : virtualIpAddressUnBind () failed");
        } 
        else 
        {
            trace (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::shutdown : virtualIpAddressUnBind () success");
        }
    
        if (0 != m_pVirtualIpBindTimer->getTimerHandle ())
        {
            deleteTimer (m_pVirtualIpBindTimer->getTimerHandle ());
            m_pVirtualIpBindTimer->getTimerHandle () = 0;
        }
    }

    pWaveAsynchronousContextForShutDownPhases->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    pWaveAsynchronousContextForShutDownPhases->callback ();
}

void VirtualIpGlobalWorker::virtualIpGlobalConfigVipMessageHandler (VirtualIpGlobalConfigVipMessage *pVirtualIpGlobalConfigVipMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VirtualIpGlobalWorker::virtualIpGlobalConfigVipMessageHandler : IP address config message"));

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::queryVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::validateConfiguringVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::configureVirtualIpManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerFailedStep)
    };

    VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext = new VirtualIpGlobalWorkerSequencerContext (pVirtualIpGlobalConfigVipMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVirtualIpGlobalWorkerSequencerContext, __FILE__, __LINE__);

    pVirtualIpGlobalWorkerSequencerContext->setVirtualIpAddress (pVirtualIpGlobalConfigVipMessage->getVirtualIpAddress());

    pVirtualIpGlobalWorkerSequencerContext->holdAll ();
    pVirtualIpGlobalWorkerSequencerContext->start ();
}

void VirtualIpGlobalWorker::virtualIpGlobalDeleteVipMessageHandler (VirtualIpGlobalDeleteVipMessage *pVirtualIpGlobalDeleteVipMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VirtualIpGlobalWorker::virtualIpGlobalDeleteVipMessageHandler : Remove VIP"));
    
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::queryVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::validateDeletingVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::unbindVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::deleteVirtualIpManagedObjectStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerFailedStep)
    };

    VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext = new VirtualIpGlobalWorkerSequencerContext (pVirtualIpGlobalDeleteVipMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVirtualIpGlobalWorkerSequencerContext, __FILE__, __LINE__);

    pVirtualIpGlobalWorkerSequencerContext->holdAll ();
    pVirtualIpGlobalWorkerSequencerContext->start ();
}

void VirtualIpGlobalWorker::virtualIpGlobalBindVipMessageHandler (VirtualIpGlobalBindVipMessage *pVirtualIpGlobalBindVipMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VirtualIpGlobalWorker::virtualIpGlobalBindVipMessageHandler : Virtual IP bind message"));
    
    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::queryVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::validateBindingVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::bindVirtualIpWithTimerRetryStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerFailedStep)
    };
    
    VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext = new VirtualIpGlobalWorkerSequencerContext (pVirtualIpGlobalBindVipMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVirtualIpGlobalWorkerSequencerContext, __FILE__, __LINE__);
    
    pVirtualIpGlobalWorkerSequencerContext->holdAll ();
    pVirtualIpGlobalWorkerSequencerContext->start ();
}

void VirtualIpGlobalWorker::virtualIpGlobalUnbindVipMessageHandler (VirtualIpGlobalUnbindVipMessage *pVirtualIpGlobalUnbindVipMessage)
{
    trace (TRACE_LEVEL_DEVEL, string ("VirtualIpGlobalWorker::virtualIpGlobalUnbindVipMessageHandler : Virtual IP unbind message"));

    PrismLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::unbindVirtualIpStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerSucceededStep),
        reinterpret_cast<PrismLinearSequencerStep> (&VirtualIpGlobalWorker::prismLinearSequencerFailedStep)
    };

    VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext = new VirtualIpGlobalWorkerSequencerContext (pVirtualIpGlobalUnbindVipMessage, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
    prismAssert (NULL != pVirtualIpGlobalWorkerSequencerContext, __FILE__, __LINE__);

    pVirtualIpGlobalWorkerSequencerContext->holdAll ();
    pVirtualIpGlobalWorkerSequencerContext->start ();
}

void VirtualIpGlobalWorker::queryVirtualIpStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    ResourceId                                  status                  = WAVE_MESSAGE_SUCCESS;
    WaveManagedObjectSynchronousQueryContext   *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext (VirtualIpGlobalManagedObject::getClassName());
    prismAssert (NULL != synchronousQueryContext, __FILE__, __LINE__);

    vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);

    delete synchronousQueryContext;

    if (NULL != pResults)
    {
        // Store the query results for the virtual ip global MO on the context for later use.  These results are automatically deleted during the sequencer context's destructor.

        pVirtualIpGlobalWorkerSequencerContext->setPQueryResults (pResults);

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::queryAndValidateVirtualIpStep : Query failed for the VirtualIpGlobalManagedObject.");

        status = VIP_QUERY_FAILED;
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::validateConfiguringVirtualIpStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    string                              virtualIpAddress                    = pVirtualIpGlobalWorkerSequencerContext->getVirtualIpAddress ();
    string                              virtualIpAddressBoundToThisNode     = getCurrentVirtualIpAddress ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;

    if (true == isVirtualIpDuplicate (virtualIpAddress))
    {
        trace (TRACE_LEVEL_WARN, "VirtualIpGlobalWorker::validateConfiguringVirtualIpStep : Host ip address is in use.");

        status = VIP_DUPLICATE_HOST_IP;
    }
    
    pVirtualIpGlobalWorkerSequencerContext->setBoundVirtualIpAddress (virtualIpAddressBoundToThisNode);

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::validateDeletingVirtualIpStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    vector<WaveManagedObject *>        *pResults                            = pVirtualIpGlobalWorkerSequencerContext->getPQueryResults ();
    string                              virtualIpAddress                    = pVirtualIpGlobalWorkerSequencerContext->getVirtualIpAddress ();
    string                              virtualIpAddressBoundToThisNode     = VirtualIpGlobalWorker::getCurrentVirtualIpAddress ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;

    if (0 == pResults->size())
    {
        status = VIP_DOES_NOT_EXIST; 
    
        trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::validateDeletingVirtualIpStep : ") + FrameworkToolKit::localize (status));
    }
    else if (1 == pResults->size())
    {
        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        status = VIP_MULTIPLE_EXIST;

        trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::validateDeletingVirtualIpStep : ") + FrameworkToolKit::localize (status));
    }

    pVirtualIpGlobalWorkerSequencerContext->setBoundVirtualIpAddress (virtualIpAddressBoundToThisNode);

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::validateBindingVirtualIpStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    vector<WaveManagedObject *>        *pResults                            = pVirtualIpGlobalWorkerSequencerContext->getPQueryResults ();
    string                              virtualIpAddress                    = "none";
    string                              virtualIpAddressBoundToThisNode     = VirtualIpGlobalWorker::getCurrentVirtualIpAddress ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;


    // Validate virtual ip configured database results.

    if (0 == pResults->size())
    {
        status = VIP_DOES_NOT_EXIST;
    
        trace (TRACE_LEVEL_WARN, string ("VirtualIpGlobalWorker::validateBindingVirtualIpStep : Status : ") + FrameworkToolKit::localize (status));

        pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);

        return;
    }
    else if (1 == pResults->size())
    {
        // We found the already configured virtual ip to retry binding.       

        VirtualIpGlobalManagedObject *pVipMo = dynamic_cast<VirtualIpGlobalManagedObject *> ((*pResults)[0]);
        prismAssert (pVipMo != NULL, __FILE__, __LINE__);

        virtualIpAddress = pVipMo->getVipAddress ();

        pVirtualIpGlobalWorkerSequencerContext->setVirtualIpAddress (virtualIpAddress);
    }
    else
    {
        status = VIP_MULTIPLE_EXIST;

        trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::validateBindingVirtualIpStep : Status : ") + FrameworkToolKit::localize (status));

        pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);

        return;
    }

    // Validate virtual ip backend with database configuration.

    if (virtualIpAddress == virtualIpAddressBoundToThisNode)
    {
        tracePrintf (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::validateBindingVirtualIpStep : Virtual ip address %s is already bound to this node.  Returning with early reply.", virtualIpAddressBoundToThisNode.c_str ());

        VirtualIpGlobalBindVipMessage *pVirtualIpGlobalBindVipMessage = dynamic_cast<VirtualIpGlobalBindVipMessage *> (pVirtualIpGlobalWorkerSequencerContext->getPPrismMessage ());
        prismAssert (NULL != pVirtualIpGlobalBindVipMessage, __FILE__, __LINE__);        

        // Do an early message reply when backend virtual ip is already bound to the configured virtual ip in the database.

        pVirtualIpGlobalBindVipMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);

        reply (pVirtualIpGlobalBindVipMessage);

        // Failing the linear sequencer here on purpose since we have already replied to the message with a success.  We do not want the linear sequencer to proceed to the next step.  Just go to the end.

        pVirtualIpGlobalWorkerSequencerContext->setPPrismMessage (NULL);

        if (true == (pVirtualIpGlobalWorkerSequencerContext->getIsHoldAllRequested ()))
        {
            pVirtualIpGlobalWorkerSequencerContext->unholdAll ();
        }

        pVirtualIpGlobalWorkerSequencerContext->executeNextStep (WAVE_MESSAGE_ERROR);

        return;
    }

    // For the duplicate ip case, we will allow this validation to pass to next step so that binding can be retried with the timer.

    bool isDuplicate = isVirtualIpDuplicate (virtualIpAddress);

    if (true == isDuplicate)
    {
        trace (TRACE_LEVEL_WARN, "VirtualIpGlobalWorker::validateBindingVirtualIpStep : Host ip address is in use.");

        status = WAVE_MESSAGE_SUCCESS;
    }

    pVirtualIpGlobalWorkerSequencerContext->setIsVirtualIpDuplicate (isDuplicate);
    pVirtualIpGlobalWorkerSequencerContext->setBoundVirtualIpAddress (virtualIpAddressBoundToThisNode);

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::configureVirtualIpManagedObjectStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    vector<WaveManagedObject *>        *pResults                            = pVirtualIpGlobalWorkerSequencerContext->getPQueryResults ();
    VirtualIpGlobalConfigVipMessage    *pVirtualIpGlobalConfigVipMessage    = dynamic_cast<VirtualIpGlobalConfigVipMessage *> (pVirtualIpGlobalWorkerSequencerContext->getPPrismMessage ());
    prismAssert (NULL != pVirtualIpGlobalConfigVipMessage, __FILE__, __LINE__);

    string                              virtualIpAddress                    = pVirtualIpGlobalConfigVipMessage->getVirtualIpAddress ();

    if (0 == pResults->size())
    {
        trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::configureVirtualIpManagedObjectStep : pResults size 0");

        VirtualIpGlobalManagedObject *pVipMo = new VirtualIpGlobalManagedObject (dynamic_cast<VcsFabObjectManager *> (getPWaveObjectManager ()));
        prismAssert(NULL != pVipMo, __FILE__, __LINE__);

        pVipMo->setVipAddress (virtualIpAddress);
    }
    else
    {
        trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::configureVirtualIpManagedObjectStep : pResults exists, changing");

        VirtualIpGlobalManagedObject *pVipMo = dynamic_cast<VirtualIpGlobalManagedObject *> ((*pResults)[0]);
        prismAssert(pVipMo != NULL, __FILE__, __LINE__);

        updateWaveManagedObject (pVipMo);

        pVipMo->setVipAddress (virtualIpAddress);
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

void VirtualIpGlobalWorker::deleteVirtualIpManagedObjectStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    vector<WaveManagedObject *>    *pResults    = pVirtualIpGlobalWorkerSequencerContext->getPQueryResults ();
    ResourceId                      status      = WAVE_MESSAGE_SUCCESS;

    if (1 == pResults->size())
    {
        VirtualIpGlobalManagedObject *pVipMo = dynamic_cast<VirtualIpGlobalManagedObject *> ((*pResults)[0]);
        prismAssert(NULL != pVipMo, __FILE__, __LINE__);

        delete pVipMo;

        pVipMo = NULL;
        (*pResults)[0] = NULL;

        status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {
        // Previous validation step would have already caught this case.
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    SI32                                returnCode                          = 0;
    string                              virtualIpAddress                    = pVirtualIpGlobalWorkerSequencerContext->getVirtualIpAddress ();
    string                              virtualIpAddressBoundToThisNode     = pVirtualIpGlobalWorkerSequencerContext->getBoundVirtualIpAddress ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;

    if (("NULL" != virtualIpAddressBoundToThisNode) && (virtualIpAddressBoundToThisNode != virtualIpAddress))
    {
        trace (TRACE_LEVEL_INFO, string ("VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep : Update case.  Unbinding previously bound virtual ip address \"") + virtualIpAddressBoundToThisNode + "\".");

        returnCode = virtualIpAddressUnBind ();

        if (0 == returnCode)
        {
            trace (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep : Virtual ip unbind successful for update case.");
        }
        else
        {
            status = VIP_UNBIND_FAILED_FOR_UPDATE;    

            trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep : Update case.  Status : ") + FrameworkToolKit::localize (status));
        }
    }

    // Configure/Update the vcs virtual ip and bind

    returnCode = virtualIpAddressBindLocal (virtualIpAddress);

    if (0 == returnCode)
    {
        status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep : Virtual ip bind successful.");
    } 
    else
    {
        status = VIP_BIND_FAILED;

        trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::bindVirtualIpForConfigurationStep : ") + FrameworkToolKit::localize (status));
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::bindVirtualIpWithTimerRetryStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    SI32                                returnCode                          = 0;
    string                              virtualIpAddress                    = pVirtualIpGlobalWorkerSequencerContext->getVirtualIpAddress ();
    bool                                isVirtualIpAddressDuplicate         = pVirtualIpGlobalWorkerSequencerContext->getIsVirtualIpDuplicate ();
    ResourceId                          status                              = WAVE_MESSAGE_SUCCESS;

    // Only bind the already configured vcs virtual ip.  This will asynchronously launch a timer to keep retrying the bind when a duplicate ip is detected.

    if (false == isVirtualIpAddressDuplicate)
    {
        returnCode = virtualIpAddressBindLocal (virtualIpAddress);

        if (0 == returnCode)
        {
            status = WAVE_MESSAGE_SUCCESS;

            trace (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::bindVirtualIpWithTimerRetryStep : Virtual ip bind successful.  Timer will not be started.");
        }
        else
        {
            returnCode = virtualIpAddressBindWithRetryTimer (virtualIpAddress);
        }
    }
    else
    {
        returnCode = virtualIpAddressBindWithRetryTimer (virtualIpAddress);
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

void VirtualIpGlobalWorker::unbindVirtualIpStep (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext)
{
    SI32        returnCode  = 0;
    ResourceId  status      = WAVE_MESSAGE_SUCCESS;

    returnCode = virtualIpAddressUnBind ();

    if (0 == returnCode)
    {
        status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::unbindVirtualIpStep : Virtual ip unbind successful.");
    }
    else
    {
        status = VIP_UNBIND_FAILED;

        trace (TRACE_LEVEL_ERROR, string ("VirtualIpGlobalWorker::unbindVirtualIpStep : ") + FrameworkToolKit::localize (status));
    }

    pVirtualIpGlobalWorkerSequencerContext->executeNextStep (status);
}

ResourceId VirtualIpGlobalWorker::virtualIpAddressBindWithRetryTimer (const string &ipAddress)
{
    ResourceId  status  = FRAMEWORK_SUCCESS;

    // Start the timer only if there is no timer running.

    if (0 != m_pVirtualIpBindTimer->getTimerHandle ())
    {
        deleteTimer (m_pVirtualIpBindTimer->getTimerHandle ());
        m_pVirtualIpBindTimer->getTimerHandle () = 0;
    }

    // Time is in milliseconds - 30 seconds

    m_pVirtualIpBindTimer->setCacheInterval (30000);
    m_pVirtualIpBindTimer->setVirtualIpAddress (ipAddress);

    trace (TRACE_LEVEL_DEBUG, string("VirtualIpGlobalWorker::virtualIpTryIpAddressBind : Starting timer with time ") + m_pVirtualIpBindTimer->getCacheInterval ());

    status = startTimer (m_pVirtualIpBindTimer->getTimerHandle (), m_pVirtualIpBindTimer->getCacheInterval (), reinterpret_cast<PrismTimerExpirationHandler> (&VirtualIpGlobalWorker::virtualIpBindTimerHandler));

    return (status);
}

void VirtualIpGlobalWorker::virtualIpBindTimerHandler (TimerHandle &timerHandle)
{
    string  virtualIpAddress    = "none";

    // Clear timer handle since it has expired

    m_pVirtualIpBindTimer->getTimerHandle () = 0;

    virtualIpAddress = m_pVirtualIpBindTimer->getVirtualIpAddress ();

    if ("none" != virtualIpAddress)
    {
        trace (TRACE_LEVEL_INFO, string ("VirtualIpGlobalWorker::virtualIpBindTimerHandler : Trying to bind vcs virtual ip : ") + virtualIpAddress);

        if (false == isVirtualIpBoundToThisNode (virtualIpAddress))  
        {
            if (true == isVirtualIpDuplicate (virtualIpAddress))
            {
                trace (TRACE_LEVEL_WARN, string ("VirtualIpGlobalWorker::virtualIpBindTimerHandler : The vcs virtual ip \"") + virtualIpAddress + "\" is detected as a duplicate.  Binding will be retried.");

                
    
                virtualIpAddressBindWithRetryTimer (virtualIpAddress);
            }
            else
            {
                virtualIpAddressBindLocal (virtualIpAddress);
            }
        }
        else
        {
            trace (TRACE_LEVEL_INFO, string ("VirtualIpGlobalWorker::virtualIpBindTimerHandler : The vcs virtual ip : \"") + virtualIpAddress + "\" is already bound to this node.  Binding will not be retried.");
        }
    }
    else
    {
        trace (TRACE_LEVEL_WARN, string ("VirtualIpGlobalWorker::virtualIpBindTimerHandler : Cached vcs virtual ip is \"") + virtualIpAddress + "\".  Binding will not be retried.");
    }
}

bool VirtualIpGlobalWorker::isVirtualIpBoundToThisNode (const string &ipAddress)
{   
    string  currentBoundVirtualIpAddress    = VirtualIpGlobalWorker::getCurrentVirtualIpAddress ();
    bool    isBound                         = true;        

    if (ipAddress == currentBoundVirtualIpAddress)
    {
        isBound = true;
    }
    else
    {
        isBound = false;
    }

    return (isBound);
}

bool VirtualIpGlobalWorker::isVirtualIpDuplicate (const string &ipAddress)
{
    string          ipAddressSubString  = ""; 
    string          pingCommand         = "";
    size_t          pos;
    vector<string>  output;
    int             returnCode          = 0;
    bool            isDuplicate         = false;

    trace (TRACE_LEVEL_DEBUG, string("VirtualIpGlobalWorker::isVirtualIpDuplicate ipAddress: ") + ipAddress);
    
    pos = ipAddress.find("/");
    ipAddressSubString = ipAddress.substr(0, pos);

    // Execute the ping command to test if the virtual ip is already bound somewhere else.
    // Ping command arguments used:
    // -c 2 : 2 packets will be sent for the test.
    // -W 1 : 1 second timeout for absense of any reponses.
    // -f   : flood is used so there is no delay between sending the ping packets.  Otherwise there is a 1 second delay between ping packets sent.
    // Note:  These arguments were adjusted from previous settings because the previous ping check was taking 4.5 seconds to complete, causing issue with configuration and binding.  Now with above settings ping check will take at most 1 second.
    // -w 3 : stop after 3 seconds. This is being added to fix the CLI freeze issue Defect TR000467594.

    pingCommand = string ("/bin/ping -c 2 -W 1 -w 3 -f ") + ipAddressSubString + string (" 1>/dev/null");

    trace (TRACE_LEVEL_DEBUG, string("VirtualIpGlobalWorker::isVirtualIpDuplicate ") + pingCommand);

    returnCode = FrameworkToolKit::systemCommandOutput (pingCommand, output); 

    if (0 == returnCode)
    {
        // ping worked
	trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::isVirtualIpDuplicate isDuplicate  true");
        isDuplicate = true;
    }
    else
    {
        // ping failed
	trace (TRACE_LEVEL_DEBUG, "VirtualIpGlobalWorker::isVirtualIpDuplicate isDuplicate false");
        isDuplicate = false;
    }

    return (isDuplicate);
}

int VirtualIpGlobalWorker::virtualIpAddressBind (const string &ipAddress)
{
    string  virtualIpAddressBoundToThisNode = VirtualIpGlobalWorker::getCurrentVirtualIpAddress ();
    int     returnCode                      = 0;

    if ("none" == ipAddress)
    {
        trace (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::virtualIpAddressBind : No virtual ip configured.");
        return (0);
    }

    if (ipAddress == virtualIpAddressBoundToThisNode)
    {
        tracePrintf (TRACE_LEVEL_INFO, "VirtualIpGlobalWorker::virtualIpAddressBind : Virtual ip address %s is already bound to this node.  Returning.", virtualIpAddressBoundToThisNode.c_str ());
        return (0);
    }

    if (true == isVirtualIpDuplicate (ipAddress))
    {
        trace (TRACE_LEVEL_WARN, "VirtualIpGlobalWorker::virtualIpAddressBind : Host ip address is in use.");
        return (2);
    }

    returnCode = virtualIpAddressBindLocal (ipAddress);

    return (returnCode);
}

int VirtualIpGlobalWorker::virtualIpAddressBindLocal (const string &ipAddress)
{
    ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
    struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
    addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
    struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
    addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
    ipadm_ip_get_set_t current = {
        0, /* num_locals */
        local_addr_ids,
        local_addrs,
        local_prefixes,
        local_flags,
        0, /* num_gws */
        gw_addr_ids,
        gw_addrs,
        gw_flags,
        0 /* dhcp_flag */
    };

    current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
    current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

    string str_prefix, str_ipaddr;
    size_t pos;

    pos = ipAddress.find("/");
    str_ipaddr = ipAddress.substr(0, pos);
    str_prefix = ipAddress.substr(pos+1);
    int prefix = atoi(str_prefix.c_str());

    trace (TRACE_LEVEL_INFO, string("VirtualIpGlobalWorker::virtualIpAddressBindLocal : address/prefix = ") + str_ipaddr + string("/") + prefix);

    int rc = -1;
    rc = ipAdmLocalAddrIdGet (local_addr_ids, &(current.num_locals));

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressBindLocal ipAdmLocalAddrIdGet() failed");
    }

    rc = ipAdmGwAddrIdGet (gw_addr_ids, &(current.num_gws));

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressBindLocal ipAdmGwAddrIdGet() failed");
    }

    rc = ipAdmIpAddrGet (&current);

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressBindLocal ipAdmIpAddrGet() failed");
    }
    
    int eth_idx = -1;

    for (int i = 0; i < current.num_locals; i++)
    {
        if (current.local_addr_ids[i].host_type == CLUSTER_HOST)
        {
            eth_idx = i;
            break;
        }
    }

    if (eth_idx < 0)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressBindLocal eth_idx < 0");
        return (-1);
    }

    //Change current
    SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr.s_addr = inet_addr(str_ipaddr.c_str());
    current.local_prefixes[eth_idx] = prefix;
    current.local_flags[eth_idx] &= ~(IPADM_ADDR_NONE | IPADM_MASK_NONE);

    rc = ipAdmIpAddrSet (&current, 0, 0);

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, string("VirtualIpGlobalWorker::virtualIpAddressBindLocal ipAdmIpAddrSet() failed rc = ") + rc);
        return (-1);
    }

    // Store cached virtual ip address for bind retry timer.

    m_pVirtualIpBindTimer->setVirtualIpAddress (ipAddress);

    return (0);
}

int VirtualIpGlobalWorker::virtualIpAddressUnBind ()
{
    ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
    struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
    addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
    struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
    addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
    ipadm_ip_get_set_t current = {
        0, /* num_locals */
        local_addr_ids,
        local_addrs,
        local_prefixes,
        local_flags,
        0, /* num_gws */
        gw_addr_ids,
        gw_addrs,
        gw_flags,
        0 /* dhcp_flag */
    };

    current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
    current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

    // If a timer is going on to assign VIP, VIP is deleted in between its attempt to retry, delete the timer first

    if (0 != m_pVirtualIpBindTimer->getTimerHandle ())
    {
        deleteTimer (m_pVirtualIpBindTimer->getTimerHandle ());
        m_pVirtualIpBindTimer->getTimerHandle () = 0;
    }

    int rc = -1;

    rc = ipAdmLocalAddrIdGet (local_addr_ids, &(current.num_locals));

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressUnBind ipAdmLocalAddrIdGet() failed");
    }

    rc = ipAdmGwAddrIdGet (gw_addr_ids, &(current.num_gws));

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressUnBind ipAdmGwAddrIdGet() failed");
    }

    rc = ipAdmIpAddrGet (&current);

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressUnBind ipAdmIpAddrGet() failed");
    }

    int eth_idx = -1;

    for (int i = 0; i < current.num_locals; i++)
    {
        if (current.local_addr_ids[i].host_type == CLUSTER_HOST)
        {
            eth_idx = i;
            break;
        }
    }

    if (eth_idx < 0)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressUnBind eth_idx < 0");
        return (-1);
    }

    //Change current
    current.local_flags[eth_idx] |= (IPADM_ADDR_NONE | IPADM_MASK_NONE);
    
    rc = ipAdmIpAddrSet (&current, 0, 0);

    if (rc)
    {
        trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::virtualIpAddressUnBind ipAdmIpAddrSet() failed");
        return (-1);
    }

    // Store cached virtual ip address for bind retry timer.

    m_pVirtualIpBindTimer->setVirtualIpAddress ("none");

    return (0);
}

string VirtualIpGlobalWorker::getCurrentVirtualIpAddress ()
{
    char ipbuf[INET6_ADDRSTRLEN];
    char prefixbuf[8];

    ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
    struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
    addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
    ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
    struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
    addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
    ipadm_ip_get_set_t current = {
        0, /* num_locals */
        local_addr_ids,
        local_addrs,
        local_prefixes,
        local_flags,
        0, /* num_gws */
        gw_addr_ids,
        gw_addrs,
        gw_flags,
        0 /* dhcp_flag */
    };

    current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
    current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

    int rc = -1;

    rc = ipAdmLocalAddrIdGet (local_addr_ids, &(current.num_locals));

    if (rc)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::getCurrentVirtualIpAddress ipAdmLocalAddrIdGet() failed");
    }

    rc = ipAdmGwAddrIdGet (gw_addr_ids, &(current.num_gws));

    if (rc)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::getCurrentVirtualIpAddress ipAdmGwAddrIdGet() failed");
    }

    rc = ipAdmIpAddrGet (&current);

    if (rc)
    {
        WaveNs::trace (TRACE_LEVEL_ERROR, "VirtualIpGlobalWorker::getCurrentVirtualIpAddress ipAdmIpAddrGet() failed");
    }

    int eth_idx = -1;

    for (int i = 0; i < current.num_locals; i++)
    {
        if (current.local_addr_ids[i].host_type == CLUSTER_HOST)
        {
            eth_idx = i;
            break;
        }
    }

    if (eth_idx < 0)
    {
        WaveNs::trace (TRACE_LEVEL_WARN, "VirtualIpGlobalWorker::getCurrentVirtualIpAddress eth_idx < 0");
        return ("NULL");
    }

    string ipAddress;

    if (current.local_flags[eth_idx] & (IPADM_ADDR_NONE | IPADM_MASK_NONE))
    {
        ipAddress.assign("NULL");
    }
    else
    {
        inet_ntop(AF_INET, &(SOCKADDR_INP(current.local_addrs[eth_idx])->sin_addr), ipbuf, sizeof(ipbuf));
        sprintf(prefixbuf, "/%d", current.local_prefixes[eth_idx]);
        strcat(ipbuf, prefixbuf);
        ipAddress.assign(ipbuf);
    }
    
    return ipAddress;
}

}
