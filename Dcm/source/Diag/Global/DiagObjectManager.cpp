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
 *   Author : Subhani Shaik                                                *
 **************************************************************************/
#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Diag/Local/DiagLocalPostManagedObject.h"
#include "Diag/Local/DiagLocalObjectManager.h"
#include "Diag/Global/DiagObjectManager.h"
#include "Diag/Global/DiagPostManagedObject.h"
#include "Diag/Global/POSTDiagWorker.h"
#include "Diag/Global/DiagTypes.h"

namespace DcmNs
{

    DiagObjectManager::DiagObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pPOSTDiagWorker = new POSTDiagWorker (this);
        prismAssert (NULL != m_pPOSTDiagWorker, __FILE__, __LINE__);
    }

    DiagObjectManager::~DiagObjectManager ()
    {
    }

    DiagObjectManager  *DiagObjectManager::getInstance()
    {
        static DiagObjectManager *pDiagObjectManager = new DiagObjectManager ();

        WaveNs::prismAssert (NULL != pDiagObjectManager, __FILE__, __LINE__);

        return (pDiagObjectManager);
    }

    string  DiagObjectManager::getClassName()
    {
        return ("Diag");
    }

    PrismServiceId  DiagObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *DiagObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *DiagObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
	void DiagObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
    	trace (TRACE_LEVEL_INFO, "DiagObjectManager::install()");
	
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	
	    /* Node is out-of-the-box or is rebooting after net-install is performed */
    	if ((pWaveAsynchronousContextForBootPhases->getBootReason() == WAVE_BOOT_FIRST_TIME_BOOT) || (pWaveAsynchronousContextForBootPhases->getBootReason() == WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT))
    	{

	    	startTransaction ();

		    /* Install Authentication Login Mode singleton managed object */
    		DiagPostManagedObject *pDiagPostManagedObj = new DiagPostManagedObject(this);

	    	/* Commit to the Wave Database */
	    	status = commitTransaction ();
		    if (FRAMEWORK_SUCCESS == status) {
    		    trace (TRACE_LEVEL_INFO, "Installed DiagPost Managed Objects");
        		status = WAVE_MESSAGE_SUCCESS;
		    } else {
    		    trace (TRACE_LEVEL_FATAL, "Failed to install DiagPost Managed Objects");
        		prismAssert (false, __FILE__, __LINE__);
		    }

    		delete pDiagPostManagedObj;
		}
   		// Send message to all local services
	    pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
    	pWaveAsynchronousContextForBootPhases->callback ();
	}

void DiagObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
{
    FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
    vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

    ResourceId status = WAVE_MESSAGE_SUCCESS;
    
    trace (TRACE_LEVEL_DEVEL, string ("DiagObjectManager::failover reason") + FrameworkToolKit::localize(failoverReason));
    
    if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
    {   
        UI32 noOfFailedLocations = failedLocationIds.size ();
 
        for (UI32 i = 0; i < noOfFailedLocations; i++)
        {
            vector<WaveManagedObject *> *pWaveManagedObjects = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i],DiagLocalPostManagedObject::getClassName());   
    
            trace (TRACE_LEVEL_INFO, string ("DiagObjectManager::failover for Location Id ") + failedLocationIds[i]);
    
            startTransaction ();
    
            UI32 sizeOfManagedObjects = pWaveManagedObjects->size ();
            trace (TRACE_LEVEL_INFO, string ("DiagObjectManager::Size of MO ") + sizeOfManagedObjects);
    
            for (UI32 j = 0; j < sizeOfManagedObjects; j++)
            {
                delete (*pWaveManagedObjects)[j];
            }
 
            status = commitTransaction ();
 
            if (FRAMEWORK_SUCCESS != status)
            {
                trace (TRACE_LEVEL_FATAL, "DiagObjectManager::failover can not commit local managed object delete to database ");
                prismAssert (false, __FILE__, __LINE__);
            }
 
            pWaveManagedObjects->clear ();
 
            delete pWaveManagedObjects;
        }
    }
    trace (TRACE_LEVEL_INFO, string ("DiagObjectManager::Winding UP "));
    pFailoverAsynchronousContext->setCompletionStatus (status);
    pFailoverAsynchronousContext->callback ();
}
}
