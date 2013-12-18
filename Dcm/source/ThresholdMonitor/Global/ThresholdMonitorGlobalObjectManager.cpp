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
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalObjectManager.h"
#include "ThresholdMonitor/Local/CpuLocalManagedObject.h"
#include "ThresholdMonitor/Local/MemoryLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpMonitorAreaLocalManagedObject.h"
#include "ThresholdMonitor/Local/SfpMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecLocalManagedObject.h"
#include "ThresholdMonitor/Local/SecMonitorPolicyLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceConfigLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceMonitorAreaLocalManagedObject.h"
#include "ThresholdMonitor/Local/InterfaceMonitorTypeLocalManagedObject.h"
#include "ThresholdMonitor/Global/GlobalSfpShowWorker.h"
#include "ThresholdMonitor/Global/GlobalSecShowWorker.h"
#include "ThresholdMonitor/Global/GlobalInterfaceShowWorker.h"
#include "ThresholdMonitor/Global/ThresholdMonitorGlobalTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"

namespace DcmNs
{

    ThresholdMonitorGlobalObjectManager::ThresholdMonitorGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
		m_pGlobalSfpShowWorker =  new GlobalSfpShowWorker (this);
		prismAssert (NULL != m_pGlobalSfpShowWorker, __FILE__, __LINE__);
		m_pGlobalSecShowWorker =  new GlobalSecShowWorker (this);
		prismAssert (NULL != m_pGlobalSecShowWorker, __FILE__, __LINE__);
		m_pGlobalInterfaceShowWorker = new GlobalInterfaceShowWorker (this);
		prismAssert (NULL != m_pGlobalInterfaceShowWorker, __FILE__, __LINE__);
    }

    ThresholdMonitorGlobalObjectManager::~ThresholdMonitorGlobalObjectManager ()
    {
    }

    ThresholdMonitorGlobalObjectManager  *ThresholdMonitorGlobalObjectManager::getInstance()
    {
        static ThresholdMonitorGlobalObjectManager *pThresholdMonitorGlobalObjectManager = new ThresholdMonitorGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pThresholdMonitorGlobalObjectManager, __FILE__, __LINE__);

        return (pThresholdMonitorGlobalObjectManager);
    }

    string  ThresholdMonitorGlobalObjectManager::getClassName()
    {
        return ("ThresholdMonitorGlobal");
    }

    PrismServiceId  ThresholdMonitorGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

	void ThresholdMonitorGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
	{
		FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
		vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace (TRACE_LEVEL_DEVEL, string ("ThresholdMonitorGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));
    	if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
		{
        	UI32 noOfFailedLocations = failedLocationIds.size ();


        	for (UI32 i = 0; i < noOfFailedLocations; i++)
			{
				vector<WaveManagedObject *> *pCPUMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], CpuLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pMemoryMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], MemoryLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSfpConfigMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SfpConfigLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSfpAreaMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SfpMonitorAreaLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSfpTypeMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SfpMonitorTypeLocalManagedObject::getClassName ());

				vector<WaveManagedObject *> *pSecConfigMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SecConfigLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSecMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SecLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSecPolicyMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SecMonitorPolicyLocalManagedObject::getClassName ());

				vector<WaveManagedObject *> *pIntConfigMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceConfigLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pIntAreaMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceMonitorAreaLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pIntTypeMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], InterfaceMonitorTypeLocalManagedObject::getClassName ());

				startTransaction ();
				
				UI32 sizeOfCPUMO = pCPUMO->size ();
				for (UI32 j = 0; j < sizeOfCPUMO; j++)
				{
					delete (*pCPUMO)[j];
				} 
				UI32 sizeOfMemoryMO = pMemoryMO->size ();
				for (UI32 j = 0; j < sizeOfMemoryMO; j++)
				{
					delete (*pMemoryMO)[j];
				} 
				UI32 sizeOfSfpConfigMO = pSfpConfigMO->size ();
				for (UI32 j = 0; j < sizeOfSfpConfigMO; j++)
				{
					delete (*pSfpConfigMO)[j];
				} 
				UI32 sizeOfSfpAreaMO = pSfpAreaMO->size ();
				for (UI32 j = 0; j < sizeOfSfpAreaMO; j++)
				{
					delete (*pSfpAreaMO)[j];
				} 
				UI32 sizeOfSfpTypeMO = pSfpTypeMO->size ();
				for (UI32 j = 0; j < sizeOfSfpTypeMO; j++)
				{
					delete (*pSfpTypeMO)[j];
				} 
				UI32 sizeOfSecConfigMO = pSecConfigMO->size ();
				for (UI32 j = 0; j < sizeOfSecConfigMO; j++)
				{
					delete (*pSecConfigMO)[j];
				} 
				UI32 sizeOfSecMO = pSecMO->size ();
				for (UI32 j = 0; j < sizeOfSecMO; j++)
				{
					delete (*pSecMO)[j];
				} 
                UI32 sizeOfSecPolicyMO = pSecPolicyMO->size ();
                for (UI32 j = 0; j < sizeOfSecPolicyMO; j++)
                {
                    delete (*pSecPolicyMO)[j];
                }
				UI32 sizeOfIntConfigMO = pIntConfigMO->size ();
				for (UI32 j = 0; j < sizeOfIntConfigMO; j++)
				{
					delete (*pIntConfigMO)[j];
				} 
				UI32 sizeOfIntAreaMO = pIntAreaMO->size ();
				for (UI32 j = 0; j < sizeOfIntAreaMO; j++)
				{
					delete (*pIntAreaMO)[j];
				} 
				UI32 sizeOfIntTypeMO = pIntTypeMO->size ();
				for (UI32 j = 0; j < sizeOfIntTypeMO; j++)
				{
					delete (*pIntTypeMO)[j];
				} 

				status = commitTransaction ();

				if (FRAMEWORK_SUCCESS != status)
				{
					trace (TRACE_LEVEL_FATAL, "ThresholdMonitorGlobalObjectManager::failover can not commit local managed object delete to database ");
				}

				pCPUMO->clear ();
				delete pCPUMO;
				pMemoryMO->clear ();
				delete pMemoryMO; 
				pSfpConfigMO->clear ();
				delete pSfpConfigMO; 
				pSfpAreaMO->clear ();
				delete pSfpAreaMO; 
				pSfpTypeMO->clear ();
				delete pSfpTypeMO; 
				pSecConfigMO->clear ();
				delete pSecConfigMO; 
				pSecMO->clear ();
				delete pSecMO; 
				pSecPolicyMO->clear ();
				delete pSecPolicyMO; 
				pIntConfigMO->clear ();
				delete pIntConfigMO; 
				pIntAreaMO->clear ();
				delete pIntAreaMO; 
				pIntTypeMO->clear ();
				delete pIntTypeMO; 
			}
		}
		pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	    pFailoverAsynchronousContext->callback ();
	}
    PrismMessage  *ThresholdMonitorGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
    WaveManagedObject  *ThresholdMonitorGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
