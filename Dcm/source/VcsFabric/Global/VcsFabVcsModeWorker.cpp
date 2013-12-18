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

/**
 *@file VcsFabVcsModeWorker.cpp 
 * Copyright (C) 2010 BrocadeCommunications Systems,Inc.
 * All rights reserved.
 * 
 * Description: This file has the implementation of the
 *              Worker class which is responsible for installing
 *              and updating the global VcsModeManagedObject. 
 * Author :     Brian Adaniya
 */

#include "DcmResourceIdEnums.h"
#include "DcmCore/DcmToolKit.h"
#include "VcsFabric/Global/VcsFabVcsModeWorker.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VcsModeManagedObject.h"
#include "VcsFabric/Global/VcsModeLinearSequencerContext.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

namespace DcmNs
{

/** 
 *Name:         VcsFabVcsModeWorker 
 *Description:  Constructor 
 * 
 * @param       VcsFabObjectManager* : Object Manager that owns this worker 
 */
VcsFabVcsModeWorker::VcsFabVcsModeWorker (VcsFabObjectManager *pVcsFabObjectManager)
    : WaveWorker (pVcsFabObjectManager)
{
    VcsModeManagedObject vcsModeManagedObject (pVcsFabObjectManager);
    vcsModeManagedObject.setupOrm ();
    addManagedClass (VcsModeManagedObject::getClassName (), this);
}

/** 
 *Name:         VcsFabVcsModeWorker
 *Description:  Destructor
 * 
 */
VcsFabVcsModeWorker::~VcsFabVcsModeWorker ()
{
}

WaveManagedObject * VcsFabVcsModeWorker::createManagedObjectInstance (const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;

    if (VcsModeManagedObject::getClassName () == managedClassName)
    {
        trace(TRACE_LEVEL_DEVEL, "VcsFabVcsModeWorker::createManagedObjectInstance : Created managed object instance : " + managedClassName);
        pWaveManagedObject = new VcsModeManagedObject (getPWaveObjectManager ());
    }
    else
    {
        trace(TRACE_LEVEL_ERROR, "VcsFabVcsModeWorker::createManagedObjectInstance : Incorrect managed object for this worker : " + managedClassName);
    }

    return pWaveManagedObject;
}

void VcsFabVcsModeWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_DEVEL, string ("VcsFabVcsModeWorker::install : Entering ..."));

    ResourceId      status      = WAVE_MESSAGE_SUCCESS;
    WaveBootReason  bootReason  = pWaveAsynchronousContextForBootPhases->getBootReason ();

    trace (TRACE_LEVEL_DEVEL, string ("VcsFabVcsModeWorker::install : BootReason : ") + FrameworkToolKit::localize (bootReason));

    if (WAVE_BOOT_FIRST_TIME_BOOT == bootReason)
    {
        // Create single instance of VcsModeManageObject and set vcs mode based on the VcsFabricLocalMO's vcs mode.

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::createVcsModeManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerFailedStep)
        };

        VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext = new VcsModeLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));
        
        if (NULL == pVcsModeLinearSequencerContext)
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsFabVcsModeWorker::install : VcsModeLinearSequencerContext is NULL."));

            prismAssert (false, __FILE__, __LINE__);
        }

        pVcsModeLinearSequencerContext->start ();
    }
    else if (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)
    {
        // Update the VcsModeManagedObject's vcs mode based on VcsFabricLocalMO's vcs mode.

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::updateVcsModeManagedObjectStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep> (&VcsFabVcsModeWorker::prismLinearSequencerFailedStep)
        };

        VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext = new VcsModeLinearSequencerContext (pWaveAsynchronousContextForBootPhases, this, sequencerSteps, sizeof (sequencerSteps) / sizeof (sequencerSteps[0]));

        if (NULL == pVcsModeLinearSequencerContext)
        {
            trace (TRACE_LEVEL_FATAL, string ("VcsFabVcsModeWorker::install : VcsModeLinearSequencerContext is NULL."));

            prismAssert (false, __FILE__, __LINE__);
        }

        pVcsModeLinearSequencerContext->start ();
    }
    else
    {
        // Bypass install

        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }
}

/**
 *Name:         getVcsModeFromQueriedVcsNodeLocalManagedObjectStep 
 *Description:  This sequencer step queries the VcsNodeLocalManagedObject of this node for the VCS mode.  The boot reasons where
 *              this step is used are FIRST_TIME_BOOT and PERSISTENT_BOOT_WITH_DEFAULT so it is expected that only one 
 *              VcsNodeLocalManagedObject exists.
 *
 * @param       VcsModeLinearSequencerContext : Inherited sequencer context is used to pass the queried VCS mode to other steps in the sequencer.
 * @return      void
 */
void VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL, "VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep : Entering ...");

    vector<WaveManagedObject *> *pResults = querySynchronously (VcsNodeLocalManagedObject::getClassName ());

    if (NULL != pResults)
    {
        UI32 numberOfResults = static_cast<UI32> (pResults->size ());

        if (numberOfResults == 1)
        {
            VcsNodeLocalManagedObject *pVcsNodeLocalManagedObject = dynamic_cast<VcsNodeLocalManagedObject *> ((*pResults)[0]);

            if (NULL != pVcsNodeLocalManagedObject)
            {
                bool vcsMode = pVcsNodeLocalManagedObject->getVcsMode ();

                trace(TRACE_LEVEL_INFO, string ("VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep : VcsMode : ") + vcsMode);

                // Get the vcs mode and store it in the newly created VcsModeMO
                pVcsModeLinearSequencerContext->setVcsMode (vcsMode);
            
                // Also get ThisVcsNodeClusterType and 
                // set vcsClusterMode flag to true in context iff vcsClusterType is VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT.

                ResourceId vcsClusterType = DcmToolKit::getThisVcsNodeClusterType ();

                bool vcsClusterMode = false;

                if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsClusterType)
                {
                    vcsClusterMode = true;
                }
            
                pVcsModeLinearSequencerContext->setVcsClusterMode (vcsClusterMode);

                WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector (pResults);
            }
            else
            {
                trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep : pVcsNodeLocalManagedObject is NULL.");

                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep : Expected only a single instance of VcsNodeLocalManagedObject for this boot phase.");

            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::getVcsModeFromQueriedVcsNodeLocalManagedObjectStep : Query for VcsNodeLocalManagedObject failed.");

        prismAssert (false, __FILE__, __LINE__);
    }

    pVcsModeLinearSequencerContext->executeNextStep (WAVE_MESSAGE_SUCCESS);
}

/**
 *Name:         createVcsModeManagedObjectStep
 *Description:  This sequencer step create a brand new global VcsModeManagedObject with the VCS mode queried from the
 *              VcsNodeLocalManagedObject.
 *
 * @param       VcsModeLinearSequencerContext : Inherited sequencer context is used to get the queried VCS mode set in 
 *              a previous sequencer step.
 * @return      void
 */
void VcsFabVcsModeWorker::createVcsModeManagedObjectStep (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL, "VcsFabVcsModeWorker::createVcsModeManagedObjectStep : Entering ...");

    bool                                    vcsMode                                 = pVcsModeLinearSequencerContext->getVcsMode ();
    bool                                    vcsClusterMode                          = pVcsModeLinearSequencerContext->getVcsClusterMode ();
    ResourceId                              status                                  = WAVE_MESSAGE_SUCCESS;
    WaveAsynchronousContextForBootPhases   *pWaveAsynchronousContextForBootPhases   = dynamic_cast<WaveAsynchronousContextForBootPhases *> (pVcsModeLinearSequencerContext->getPPrismAsynchronousContext());

    if (NULL == pWaveAsynchronousContextForBootPhases)
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::createVcsModeManagedObjectStep : pWaveAsynchronousContextForBootPhases is NULL.");

        prismAssert (false, __FILE__, __LINE__);
    }

    // Create new vcs mode managed object and set the vcs mode.
    VcsModeManagedObject *pVcsModeManagedObject = new VcsModeManagedObject (getPWaveObjectManager ());

    if (NULL == pVcsModeManagedObject)
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::createVcsModeManagedObjectStep : pVcsModeManageObject is NULL.");

        prismAssert (false, __FILE__, __LINE__);
    }

    pVcsModeManagedObject->setVcsMode (vcsMode);
    pVcsModeManagedObject->setVcsClusterMode (vcsClusterMode);

    pVcsModeLinearSequencerContext->addManagedObjectForGarbageCollection (pVcsModeManagedObject);

    pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);
    pVcsModeLinearSequencerContext->executeNextStep (status);
}

void VcsFabVcsModeWorker::dbInconsistencyCheck (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
{
    trace (TRACE_LEVEL_INFO, "VcsFabVcsModeWorker::dbInconsistencyCheck : Entering ...");

//    ResourceId  status = WAVE_MESSAGE_SUCCESS;
    WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
    trace (TRACE_LEVEL_INFO, string ("VcsFabVcsModeWorker::dbInconsistencyCheck : BootReason : ") + FrameworkToolKit::localize (bootReason));

    WaveManagedObjectSynchronousQueryContext synchronousQueryContext   (VcsModeManagedObject::getClassName ());

/*
    vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronously (&synchronousQueryContext);

    prismAssert (NULL != pWaveManagedObjects, __FILE__, __LINE__);

    if (0 == pWaveManagedObjects->size ())
    {   
        trace (TRACE_LEVEL_ERROR, string ("VcsFabVcsModeWorker::dbInconsistencyCheck : Zero entries found in VcsModeManagedObject."));
        status = WAVE_MESSAGE_ERROR;
    }
    else if (1 < pWaveManagedObjects->size ())
    {
        VcsModeManagedObject *pVcsModeManagedObject1 = NULL;
        VcsModeManagedObject *pVcsModeManagedObject2 = NULL;

        pVcsModeManagedObject1 = dynamic_cast<VcsModeManagedObject *> ((*pWaveManagedObjects)[0]);
        pVcsModeManagedObject2 = dynamic_cast<VcsModeManagedObject *> ((*pWaveManagedObjects)[1]);

        prismAssert (NULL != pVcsModeManagedObject1, __FILE__, __LINE__);
        prismAssert (NULL != pVcsModeManagedObject2, __FILE__, __LINE__);

        ObjectId object1 = pVcsModeManagedObject1->getObjectId ();
        ObjectId object2 = pVcsModeManagedObject2->getObjectId ();

        trace (TRACE_LEVEL_ERROR, string ("VcsFabVcsModeWorker::dbInconsistencyCheck : Duplicate entries found in VcsModeManagedObject. The ObjectIds are ") + object1.toString () + " and " + object2.toString ());
        status = WAVE_MESSAGE_ERROR;

        delete pVcsModeManagedObject1;
        delete pVcsModeManagedObject2;
    }

    pWaveManagedObjects->clear ();
    delete pWaveManagedObjects;
*/

    pWaveAsynchronousContextForBootPhases->setCompletionStatus (FrameworkToolKit::checkForExistenceOfOneMO (&synchronousQueryContext, VcsModeManagedObject::getClassName ()));
    pWaveAsynchronousContextForBootPhases->callback ();
}

/**
 *Name:         updateVcsModeManagedObjectStep
 *Description:  This sequencer step update the global VcsModeManagedObject with the VCS mode queried from the
 *              VcsNodeLocalManagedObject.
 *
 * @param       VcsModeLinearSequencerContext : Inherited sequencer context is used to get the queried VCS mode set in 
 *              a previous sequencer step.
 * @return      void
 */
void VcsFabVcsModeWorker::updateVcsModeManagedObjectStep (VcsModeLinearSequencerContext *pVcsModeLinearSequencerContext)
{
    trace(TRACE_LEVEL_DEVEL, "VcsFabVcsModeWorker::updateVcsModeManagedObjectStep : Entering ...");

    ResourceId                              status                                  = WAVE_MESSAGE_SUCCESS;
    WaveAsynchronousContextForBootPhases   *pWaveAsynchronousContextForBootPhases   = dynamic_cast<WaveAsynchronousContextForBootPhases *> (pVcsModeLinearSequencerContext->getPPrismAsynchronousContext());

    if (NULL == pWaveAsynchronousContextForBootPhases)
    {
        trace (TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::createVcsModeManagedObjectStep : pWaveAsynchronousContextForBootPhases is NULL.");

        prismAssert (false, __FILE__, __LINE__);
    }

    vector<WaveManagedObject *> *pResults   = querySynchronously (VcsModeManagedObject::getClassName ());

    if (NULL != pResults)
    {
        UI32 numberOfResults = static_cast<UI32> (pResults->size ());

        if (numberOfResults == 1)
        {
            VcsModeManagedObject *pVcsModeManagedObject = dynamic_cast<VcsModeManagedObject *> ((*pResults)[0]);

            if (NULL != pVcsModeManagedObject)
            {
                // Get the vcs mode and update existing VcsModeMO
                bool vcsMode = pVcsModeLinearSequencerContext->getVcsMode ();

                pVcsModeManagedObject->setVcsMode (vcsMode);

                ResourceId vcsClusterType = DcmToolKit::getThisVcsNodeClusterType ();

                bool vcsClusterMode = false;

                if (VCS_FABRIC_LOCAL_CLUSTER_TYPE_MGMT == vcsClusterType)
                {
                    vcsClusterMode = true;
                }
            
                pVcsModeManagedObject->setVcsClusterMode (vcsClusterMode);

                updateWaveManagedObject (pVcsModeManagedObject);

                trace(TRACE_LEVEL_INFO, string ("VcsFabVcsModeWorker::updateVcsModeManagedObjectStep : VcsMode : ") + vcsMode);

                pVcsModeLinearSequencerContext->addManagedObjectsForGarbageCollection (*pResults);
            }
            else
            {
                trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::updateVcsModeManagedObjectStep : pVcsModeManagedObject is NULL.");

                prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::updateVcsModeManagedObjectStep : Expected only a single instance of VcsModeManagedObject for this boot phase.");

            prismAssert (false, __FILE__, __LINE__);
        }
    }
    else
    {
        trace(TRACE_LEVEL_FATAL, "VcsFabVcsModeWorker::updateVcsModeManagedObjectStep : Query for VcsModeManagedObject failed.");

        prismAssert (false, __FILE__, __LINE__);
    }

    pWaveAsynchronousContextForBootPhases->setCompletionStatus(status);
    pVcsModeLinearSequencerContext->executeNextStep (status);
}

}
