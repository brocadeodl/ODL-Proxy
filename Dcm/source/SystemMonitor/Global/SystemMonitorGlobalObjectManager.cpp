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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : pahuja                                                     *
 **************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"
#include "SystemMonitor/Global/ShowMonitorGlobalWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/GlobalFanWorker.h"
#include "SystemMonitor/Global/GlobalPowerWorker.h"
#include "SystemMonitor/Global/GlobalTempWorker.h"
#include "SystemMonitor/Global/GlobalWnnCardWorker.h"
#include "SystemMonitor/Global/GlobalSfpWorker.h"
#include "SystemMonitor/Global/GlobalCompactFlashWorker.h"
#include "SystemMonitor/Global/GlobalFaultyPortsWorker.h"
#include "SystemMonitor/Global/GlobalFruMailWorker.h"
#include "SystemMonitor/Global/GlobalSecMailWorker.h"
#include "SystemMonitor/Global/GlobalSfpMailWorker.h"
#include "SystemMonitor/Global/GlobalIntMailWorker.h"
#include "SystemMonitor/Global/GlobalRelayIpWorker.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"
#include "SystemMonitor/Local/CompactFlashThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/FanAlertLocalManagedObject.h"
#include "SystemMonitor/Local/FanThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/LineCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/LineCardThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/MMThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/PowerAlertLocalManagedObject.h"
#include "SystemMonitor/Local/PowerThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SfmThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/SfpAlertLocalManagedObject.h"
#include "SystemMonitor/Local/TempThresholdLocalManagedObject.h"
#include "SystemMonitor/Local/WwnCardAlertLocalManagedObject.h"
#include "SystemMonitor/Local/WwnCardThresholdLocalManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
namespace DcmNs
{

    SystemMonitorGlobalObjectManager::SystemMonitorGlobalObjectManager ()
        : WaveObjectManager  (getClassName ())
    {
        m_pShowMonitorGlobalWorker = new ShowMonitorGlobalWorker (this);
        prismAssert (NULL != m_pShowMonitorGlobalWorker, __FILE__, __LINE__);
        m_pGlobalFanWorker = new GlobalFanWorker (this);
        prismAssert (NULL != m_pGlobalFanWorker, __FILE__, __LINE__);
        m_pGlobalPowerWorker = new GlobalPowerWorker (this);
        prismAssert (NULL != m_pGlobalPowerWorker, __FILE__, __LINE__);
        m_pGlobalTempWorker = new GlobalTempWorker (this);
        prismAssert (NULL != m_pGlobalTempWorker, __FILE__, __LINE__);
        m_pGlobalWnnCardWorker = new GlobalWnnCardWorker (this);
        prismAssert (NULL != m_pGlobalWnnCardWorker, __FILE__, __LINE__);
        m_pGlobalSfpWorker = new GlobalSfpWorker (this);
        prismAssert (NULL != m_pGlobalSfpWorker, __FILE__, __LINE__);
        m_pGlobalCompactFlashWorker = new GlobalCompactFlashWorker (this);
        prismAssert (NULL != m_pGlobalCompactFlashWorker, __FILE__, __LINE__);
        m_pGlobalFaultyPortsWorker = new GlobalFaultyPortsWorker (this);
        prismAssert (NULL != m_pGlobalFaultyPortsWorker, __FILE__, __LINE__);
        m_pGlobalFruMailWorker = new GlobalFruMailWorker (this);
        prismAssert (NULL != m_pGlobalFruMailWorker, __FILE__, __LINE__);
		m_pGlobalSecMailWorker = new GlobalSecMailWorker (this);
        prismAssert (NULL != m_pGlobalSecMailWorker, __FILE__, __LINE__);
        m_pGlobalSfpMailWorker = new GlobalSfpMailWorker (this);
        prismAssert (NULL != m_pGlobalSfpMailWorker, __FILE__, __LINE__);
        m_pGlobalIntMailWorker = new GlobalIntMailWorker (this);
        prismAssert (NULL != m_pGlobalIntMailWorker, __FILE__, __LINE__);
        m_pGlobalRelayIpWorker = new GlobalRelayIpWorker (this);
        prismAssert (NULL != m_pGlobalRelayIpWorker, __FILE__, __LINE__);

    }

    SystemMonitorGlobalObjectManager::~SystemMonitorGlobalObjectManager ()
    {
         delete m_pShowMonitorGlobalWorker;
         delete m_pGlobalFanWorker;
         delete m_pGlobalPowerWorker;
         delete m_pGlobalTempWorker;
         delete m_pGlobalWnnCardWorker;
         delete m_pGlobalSfpWorker;
         delete m_pGlobalCompactFlashWorker;
         delete m_pGlobalFaultyPortsWorker;
         delete m_pGlobalFruMailWorker;
		 delete m_pGlobalSecMailWorker;
         delete m_pGlobalSfpMailWorker;
         delete m_pGlobalIntMailWorker;
         delete m_pGlobalRelayIpWorker;
    }

    SystemMonitorGlobalObjectManager  *SystemMonitorGlobalObjectManager::getInstance()
    {
        static SystemMonitorGlobalObjectManager *pSystemMonitorGlobalObjectManager = new SystemMonitorGlobalObjectManager ();

        WaveNs::prismAssert (NULL != pSystemMonitorGlobalObjectManager, __FILE__, __LINE__);

        return (pSystemMonitorGlobalObjectManager);
    }

    string  SystemMonitorGlobalObjectManager::getClassName()
    {
        return ("SystemMonitorGlobal");
    }

    PrismServiceId  SystemMonitorGlobalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }
	void SystemMonitorGlobalObjectManager::failover (FailoverAsynchronousContext *pFailoverAsynchronousContext)
	{
		FrameworkObjectManagerFailoverReason    failoverReason                      = pFailoverAsynchronousContext->getfailoverReason ();
		vector<LocationId>                      failedLocationIds                   = pFailoverAsynchronousContext->getfailedLocationIds ();

		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace (TRACE_LEVEL_DEVEL, string ("SystemMonitorGlobalObjectManager::failover failover reason") + FrameworkToolKit::localize(failoverReason));
    	if (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_CONTROLLED == failoverReason)
		{
        	UI32 noOfFailedLocations = failedLocationIds.size ();


        	for (UI32 i = 0; i < noOfFailedLocations; i++)
        	{
            	vector<WaveManagedObject *> *pCFMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], CompactFlashThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pFanAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], FanAlertLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pFanThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], FanThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pLineCardAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], LineCardAlertLocalManagedObject::getClassName ());

				vector<WaveManagedObject *> *pLineCardThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], LineCardThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pMMThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], MMThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pPowerAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], PowerAlertLocalManagedObject::getClassName ());
	
				vector<WaveManagedObject *> *pPowerThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], PowerThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSfmThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SfmThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pSfpAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], SfpAlertLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pTempThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], TempThresholdLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pWwnCardAlertMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], WwnCardAlertLocalManagedObject::getClassName ());
				vector<WaveManagedObject *> *pWwnCardThresholdMO = querySynchronouslyLocalManagedObjectsForLocationId (failedLocationIds[i], WwnCardThresholdLocalManagedObject::getClassName ());

				startTransaction ();

	            UI32 sizeOfCFMO = pCFMO->size ();
				for (UI32 j = 0; j < sizeOfCFMO; j++)
	            {
	                delete (*pCFMO)[j];
	            } 
				UI32 sizeOfFanAlertMO = pFanAlertMO->size ();
				for (UI32 j = 0; j < sizeOfFanAlertMO; j++)
	            {
	                delete (*pFanAlertMO)[j];
	            } 
				UI32 sizeOfFanThresholdMO = pFanThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfFanThresholdMO; j++)
	            {
	                delete (*pFanThresholdMO)[j];
	            } 
	 			UI32 sizeOfLineCardAlertMO = pLineCardAlertMO->size ();
				for (UI32 j = 0; j < sizeOfLineCardAlertMO; j++)
	            {
	                delete (*pLineCardAlertMO)[j];
	            } 
				UI32 sizeOfLineCardThresholdMO = pLineCardThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfLineCardThresholdMO; j++)
	            {
    	            delete (*pLineCardThresholdMO)[j];
        	    } 
				UI32 sizeOfMMThresholdMO = pMMThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfMMThresholdMO; j++)
	            {
	                delete (*pMMThresholdMO)[j];
	            } 
	
				UI32 sizeOfPowerAlertMO = pPowerAlertMO->size ();
				for (UI32 j = 0; j < sizeOfPowerAlertMO; j++)
	            {
	                delete (*pPowerAlertMO)[j];
	            } 
				UI32 sizeOfPowerThresholdMO = pPowerThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfPowerThresholdMO; j++)
	            {
	                delete (*pPowerThresholdMO)[j];
	            } 
				UI32 sizeOfSfmThresholdMO = pSfmThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfSfmThresholdMO; j++)
        	    {
            	    delete (*pSfmThresholdMO)[j];
	            } 
				UI32 sizeOfSfpAlertMO = pSfpAlertMO->size ();
				for (UI32 j = 0; j < sizeOfSfpAlertMO; j++)
            	{
	                delete (*pSfpAlertMO)[j];
    	        } 
				UI32 sizeOfTempThresholdMO = pTempThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfTempThresholdMO; j++)
	            {
    	            delete (*pTempThresholdMO)[j];
        	    } 
				UI32 sizeOfWwnCardAlertMO = pWwnCardAlertMO->size ();
				for (UI32 j = 0; j < sizeOfWwnCardAlertMO; j++)
    	        {
        	        delete (*pWwnCardAlertMO)[j];
	           	} 
				UI32 sizeOfWwnCardThresholdMO = pWwnCardThresholdMO->size ();
				for (UI32 j = 0; j < sizeOfWwnCardThresholdMO; j++)
            	{
	                delete (*pWwnCardThresholdMO)[j];
    	        } 
	
    	        status = commitTransaction ();
	
    	        if (FRAMEWORK_SUCCESS != status)
        	    {
            	    trace (TRACE_LEVEL_FATAL, "ThresholdMonitorGlobalObjectManager::failover can not commit local managed object delete to database ");
	            }
	
    	        pCFMO->clear ();
				delete pCFMO;
            	pFanAlertMO->clear ();
				delete pFanAlertMO; 
    	        pFanThresholdMO->clear ();
				delete pFanThresholdMO; 
	           	pLineCardAlertMO->clear ();
				delete pLineCardAlertMO; 
        	    pLineCardThresholdMO->clear ();
				delete pLineCardThresholdMO; 
	            pMMThresholdMO->clear ();
				delete pMMThresholdMO; 
        	    pPowerAlertMO->clear ();
				delete pPowerAlertMO; 
	            pPowerThresholdMO->clear ();
				delete pPowerThresholdMO; 
        	    pSfmThresholdMO->clear ();
				delete pSfmThresholdMO; 
	            pSfpAlertMO->clear ();
				delete pSfpAlertMO; 
    		    pTempThresholdMO->clear ();
				delete pTempThresholdMO; 
	            pWwnCardAlertMO->clear ();
				delete pWwnCardAlertMO; 
        	    pWwnCardThresholdMO->clear ();
				delete pWwnCardThresholdMO; 
			}
		}
		pFailoverAsynchronousContext->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
	    pFailoverAsynchronousContext->callback ();
	}
    PrismMessage  *SystemMonitorGlobalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *SystemMonitorGlobalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

}
