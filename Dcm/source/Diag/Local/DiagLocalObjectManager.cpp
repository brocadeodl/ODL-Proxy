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

#include <iostream>
#include <string>
#include <fstream>

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Diag/Global/DiagTypes.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Diag/Local/DiagLocalPostManagedObject.h"
#include "Diag/Local/DiagLocalObjectManager.h"
#include "Diag/Local/DiagLocalPostWorker.h"
#include "Diag/Local/DiagLocalTypes.h"
#include "Diag/Global/DiagObjectManager.h"

#include "dcm/fos_headers.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "DcmShell/DcmShell.h"

using namespace WaveNs;

namespace DcmNs
{

	DiagLocalObjectManager::DiagLocalObjectManager ()
		: WaveLocalObjectManager  (getClassName ())
	{
        associateWithVirtualWaveObjectManager (DiagObjectManager::getInstance ());
        DiagLocalPostManagedObject diagLocalPostMO (this);
        diagLocalPostMO.setupOrm ();
        addManagedClass (DiagLocalPostManagedObject::getClassName (), this);
        addOperationMap (DIAGLOCALPOST, reinterpret_cast<PrismMessageHandler> (&DiagLocalObjectManager::DiagPostEnableMessageHandler));
	    //addOperationMap (DIAGENQPOST, reinterpret_cast<PrismMessageHandler> (&DiagLocalObjectManager::DiagPostEnqMessageHandler));
	}

	DiagLocalObjectManager::~DiagLocalObjectManager ()
	{
	}

	DiagLocalObjectManager  *DiagLocalObjectManager::getInstance()
	{
		static DiagLocalObjectManager *pDiagLocalObjectManager = new DiagLocalObjectManager ();

		WaveNs::prismAssert (NULL != pDiagLocalObjectManager, __FILE__, __LINE__);

		return (pDiagLocalObjectManager);
	}

	string  DiagLocalObjectManager::getClassName()
	{
		return ("Diag Local Service");
	}

	PrismServiceId  DiagLocalObjectManager::getPrismServiceId()
	{
		return ((getInstance ())->getServiceId ());
	}

	PrismMessage  *DiagLocalObjectManager::createMessageInstance(const UI32 &operationCode)
	{
		PrismMessage *pPrismMessage = NULL;

		switch (operationCode)
		{
			case DIAGLOCALPOST:
		    	pPrismMessage = new DiagLocalPostMessage();
				break;	
			default :
				pPrismMessage = NULL;
		}

		return (pPrismMessage);
	}

	WaveManagedObject  *DiagLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
	{
		 WaveManagedObject *pWaveManagedObject = NULL;
		 
        if ((DiagLocalPostManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DiagLocalPostManagedObject(dynamic_cast<DiagLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "POSTDiagWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
	}

	DiagLocalPostManagedObject* DiagLocalObjectManager::QueryDiagLocalPostManagedObject(UI32 rbridgeId)
	{
		WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DiagLocalPostManagedObject::getClassName ());

		trace (TRACE_LEVEL_INFO, "DiagLocalPostManagedObject::queryLMObjFromId:Entering");

		synchronousQueryContext.addAndAttribute (new AttributeUI32 (rbridgeId, "rbridgeId"));

		vector<WaveManagedObject *> *pResults =
		       querySynchronously (&synchronousQueryContext);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();

			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "DiagLocalPostManagedObject::QueryDiagLocalPostManagedObject: Cannot find DiagPost entry");
				pResults->clear ();
				delete pResults;
				return (NULL);
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_INFO, "DiagLocalPostManagedObject::QueryDiagLocalPostManagedObject::found 1 DiagPost entry");
				DiagLocalPostManagedObject* pDiagLocalPostManagedObject =
				              dynamic_cast<DiagLocalPostManagedObject*> ((*pResults)[0]);

				pResults->clear ();
				delete pResults;
				return (pDiagLocalPostManagedObject);
			} else {
				trace (TRACE_LEVEL_INFO, "DiagLocalPostManagedObject::QueryDiagLocalPostManagedObject: There can be only one local node.");
				pResults->clear ();
				delete pResults;
				prismAssert (false, __FILE__, __LINE__);
				return (NULL);
			}
		} else {
			trace (TRACE_LEVEL_FATAL, "DiagLocalPostManagedObject::QueryDiagLocalPostManagedObject: System failure");
			prismAssert (false, __FILE__, __LINE__);
			return (NULL);
		}

		return (NULL);
	}


	
	void DiagLocalObjectManager::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
        trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::install()");
		
        int localId = 0;
        int FosDiag;
        DiagLocalPostManagedObject *pDiagPostManagedObj;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        WaveBootReason bootReason = pWaveAsynchronousContextForBootPhases->getBootReason ();
	    localId = DcmToolKit::getLocalMappedId();

        trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install:boot reason=")
               + bootReason + string (" MID=") + localId);

        if ((WAVE_BOOT_FIRST_TIME_BOOT == bootReason) ||
        (WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT == bootReason)) {

            configInit ();
            if (localId == 0) {
                if (configGet("fabric.domain", ResourceInt, &localId) != 0) {
                    trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::install: configGet of fabric.domain key failed.");
                    /* Default to Local Switch */
                    localId = 1;
                }
                trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install: configGet of fabric.domain key value = ") + localId);
            }

            //If we are here veryfirst time install the DB with FOS config values
            trace (TRACE_LEVEL_INFO, string("DiagLocalObjectManager::install: create default entries for localId ") + localId);
            pDiagPostManagedObj = QueryDiagLocalPostManagedObject(localId);

            if (configGet("diag.postDisable", ResourceInt, &FosDiag) != 0) {
                trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::install: configGet of diag.postDisable key failed.");
                /* Disable POST on error */
                FosDiag = 1;
            }
            trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install: configGet of diag.postDisable key value = ") + FosDiag);
            
            startTransaction();

            /* Install Authentication Login Mode singleton managed object */
            pDiagPostManagedObj = new DiagLocalPostManagedObject(this);
			pDiagPostManagedObj->setrbridgeId(localId);

            /* Config DB -> DCM DB 
            *  Config DB '0' - POST Enable; '1' - POST Disable
            *  DCM DB    '0' - POST Disable; '1' - POST Enable */
            if(0 == FosDiag)
                pDiagPostManagedObj->setDiagPost(1);  /* Enable POST */
            else
                pDiagPostManagedObj->setDiagPost(0); /* Disable POST */

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
        }  else if (WAVE_BOOT_SECONDARY_NODE_CONFIGURE_BOOT == bootReason) {
			
		string id = string ("") + localId;
        trace(TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install secondary boot SID=") + id);

        WaveManagedObjectSynchronousQueryContext synchronousQueryContext (DiagLocalPostManagedObject::getClassName ());
        
        trace (TRACE_LEVEL_INFO, "DiagLocalPostManagedObject::queryLMObjFromId:Entering");
        
        synchronousQueryContext.addAndAttribute (new AttributeUI32 (localId, "rbridgeId"));
        
        vector<WaveManagedObject *> *pResults =
               querySynchronously (&synchronousQueryContext);

        if ((NULL == pResults) || (pResults->size() == 0)) {
            trace(TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install secondary boot no MO SID=") + id);
            // Need to create this node if principal node DB does not have it yet.
            startTransaction();

            /* Install Authentication Login Mode singleton managed object */
            pDiagPostManagedObj = new DiagLocalPostManagedObject(this);
            pDiagPostManagedObj->setrbridgeId(localId);

			/* Value of FosDiag should be get from the backdoor  of remote switch */
            if (configGet("diag.postDisable", ResourceInt, &FosDiag) != 0) {
                trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::install: configGet of diag.postDisable key failed.");
                /* Disable POST on error */
                FosDiag = 1;
            }
            trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::install: configGet of diag.postDisable key value = ") + FosDiag);

            if(0 == FosDiag)
                pDiagPostManagedObj->setDiagPost(1);  /* Enable POST */
            else
                pDiagPostManagedObj->setDiagPost(0); /* Disable POST */

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

          } else {
              WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
          }
	   } else {
			 trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::install() DB exists");
             status = WAVE_MESSAGE_SUCCESS;
	   }
		
       pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
       pWaveAsynchronousContextForBootPhases->callback ();
   }
    
	#define DIAGCLI_FILE "/var/log/diagcli.data"

/* 
*   This API will get the read the POST status of FOS config DB and sync the changes with conf DB
*/
    void DiagLocalObjectManager::SyncPOSTStatus() 
    {

        UI32 mappedId = 0;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        UI32 FosDiag;

        mappedId = DcmToolKit::getLocalMappedId();
        trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: getLocalMappedId switch id = ") + mappedId);
        if (mappedId == 0) {
            configInit ();
            if (configGet("fabric.domain", ResourceInt, &mappedId) != 0) {
                trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::SyncPOSTStatus: configGet of fabric.domain key failed.");
                /* Local Switch as Default */
                mappedId = 1;
            }
            trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: configGet of fabric.domain key value = ") + mappedId);
        }

        DiagLocalPostManagedObject* pDiagLocalPostManagedObject =
                                             QueryDiagLocalPostManagedObject(mappedId);
        trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: switch id ") + mappedId);
 
        if (pDiagLocalPostManagedObject) 
        {
            trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: pDiagLocalPostManagedObject exists"));
 
            startTransaction();
            
            string result = "";
            ifstream in;
            configInit ();
            if (configGet("diag.postDisable", ResourceInt, &FosDiag) != 0) {
                trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::SyncPOSTStatus: configGet of diag.postDisable key failed.");
                /* Disable POST on error */
                FosDiag = 1;
            }
            trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: configGet of diag.postDisable key value = ") + FosDiag); 
            pDiagLocalPostManagedObject->setrbridgeId(mappedId);
            /* Config DB -> DCM DB
                       *  Config DB '0' - POST Enable; '1' - POST Disable
                       *  DCM DB    '0' - POST Disable; '1' - POST Enable 
                     */
            if(0 == FosDiag) {
                pDiagLocalPostManagedObject->setDiagPost(1);  /* Enable POST */
                trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: diag enable POST"));
            }
            else {
                pDiagLocalPostManagedObject->setDiagPost(0); /* Disable POST */
                trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::SyncPOSTStatus: diag disable POST"));
            }

            /* Commit to the Wave Database */
            status = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status) {
                trace (TRACE_LEVEL_INFO, "Installed DiagPost Managed Objects");
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace (TRACE_LEVEL_FATAL, "Failed to install DiagPost Managed Objects");
                prismAssert (false, __FILE__, __LINE__);
            }
 
            in.open(DIAGCLI_FILE);
            if ( ! in ) {
                result = "Fail to run ";
                result += '\n';
            } else {
                string str;
                getline(in, str);  // Get the frist line from the file, if any.
                
                while ( in ) {  // Continue if the line was sucessfully read.
                    result = result + str + '\n';  // Process the line.
                    trace (TRACE_LEVEL_DEBUG, string ("fosCLI=") + str);
                    getline(in, str, in.widen ('\n'));   // Try to get another line.
                }
                in.close();
            }
            
            delete pDiagLocalPostManagedObject;
        } else { 
            trace (TRACE_LEVEL_FATAL, string ("DiagLocalObjectManager::SyncPOSTStatus: POST update Failure"));
        }
    }


	ResourceId DiagLocalObjectManager::updatePostEnable (PrismSynchronousLinearSequencerContext *pDiagUpdatePostEnableMessageContext)
	{
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		DiagLocalPostMessage *pDiagUpdatePostEnableMsg = reinterpret_cast<DiagLocalPostMessage *> (pDiagUpdatePostEnableMessageContext->getPPrismMessage ());

		trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::updatePostEnable: switch id") + pDiagUpdatePostEnableMsg ->getrbridgeId() + string ("POST Enable") + pDiagUpdatePostEnableMsg ->getDiagPost());

	    DiagLocalPostManagedObject* pDiagLocalPostManagedObject =
			                                 QueryDiagLocalPostManagedObject(pDiagUpdatePostEnableMsg ->getrbridgeId());


		if (pDiagLocalPostManagedObject ) {
			trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::updatePostEnable : within UPDT POST EnableMO_exists"));

            startTransaction();
	
			pDiagLocalPostManagedObject->setrbridgeId(pDiagUpdatePostEnableMsg ->getrbridgeId());
			pDiagLocalPostManagedObject->setDiagPost(pDiagUpdatePostEnableMsg ->getDiagPost());

			updateWaveManagedObject (pDiagLocalPostManagedObject);

            /* Commit to the Wave Database */
            status = commitTransaction ();
            if (FRAMEWORK_SUCCESS == status) {
                trace (TRACE_LEVEL_INFO, "Updated DiagPost Managed Objects");
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                trace (TRACE_LEVEL_FATAL, "Failed  to update DiagPost Managed Objects");
                prismAssert (false, __FILE__, __LINE__);
            }

			string fosCommand;
			string result = "";
	        char cmdline[512];
    	    ifstream in;
			
			if(pDiagLocalPostManagedObject->getDiagPost()) {
				fosCommand = "/fabos/link_sbin/diagenablepost";
			}
			else
			{
				fosCommand = "/fabos/link_sbin/diagdisablepost";
			}

	        snprintf(cmdline, 512, "%s > %s 2>/dev/null", (char *)fosCommand.data(), DIAGCLI_FILE);

        	(void)system(cmdline);

        	in.open(DIAGCLI_FILE);
        	if ( ! in ) {
            	trace (TRACE_LEVEL_INFO, string ("DiagLocalPostMessageHandler::fosCLI fail:") + cmdline);
            	result = "Fail to run ";
            	result += fosCommand;
            	result += '\n';
        	} else {

				 string str;
            	 getline(in, str);  // Get the frist line from the file, if any.
            while ( in ) {  // Continue if the line was sucessfully read.
                result = result + str + '\n';  // Process the line.
                trace (TRACE_LEVEL_DEBUG, string ("fosCLI=") + str);
                getline(in, str, in.widen ('\n'));   // Try to get another line.
            }
            in.close();
        }

			delete pDiagLocalPostManagedObject;
			trace(TRACE_LEVEL_INFO, string("DiagLocalObjectManager::updatePostEnable : UPDT MO done "));
		}

		return WAVE_MESSAGE_SUCCESS;
	}

	void DiagLocalObjectManager::boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
	{
		int status = WAVE_MESSAGE_SUCCESS;
		trace (TRACE_LEVEL_INFO, "DiagLocalObjectManager::boot: Entering Boot");

        SyncPOSTStatus();

		pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
		pWaveAsynchronousContextForBootPhases->callback ();
	}

 	void  DiagLocalObjectManager::DiagPostEnableMessageHandler( DiagLocalPostMessage* pDiagPostMessage)
	{

    	PrismSynchronousLinearSequencerStep sequencerSteps[] =
    	{
//        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::updatePostEnable),

//        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
        	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
    	};

    	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pDiagPostMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		pPrismSynchronousLinearSequencerContext->execute ();
	}
#if 0
	//code changes for diagd integration
	ResourceId DiagLocalObjectManager::enqPostEnable (PrismSynchronousLinearSequencerContext *pDiagEnqPostEnableMessageContext)
    {
		UI32 mappedId = DcmToolKit::getLocalMappedId();
        DiagPostEnqMessage *pEnqPostEnableMsg = reinterpret_cast<DiagPostEnqMessage*> (pDiagEnqPostEnableMessageContext->getPPrismMessage ());

        trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::enqPostEnable : switch id") + pEnqPostEnableMsg ->getrbridgeId());

        DiagLocalPostManagedObject *pDiagLocalPostManagedObject =
                                             QueryDiagLocalPostManagedObject(mappedId);


        if (pDiagLocalPostManagedObject ) {
			pEnqPostEnableMsg->setDiagPost(pDiagLocalPostManagedObject->getDiagPost());
        } else {
	 		trace (TRACE_LEVEL_INFO, string ("DiagLocalObjectManager::enqPostEnable  Failed : switch id") + pEnqPostEnableMsg ->getrbridgeId());
        }
        return WAVE_MESSAGE_SUCCESS;
    }


	 void  DiagLocalObjectManager::DiagPostEnqMessageHandler( DiagPostEnqMessage* pDiagPostEnqMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::enqPostEnable),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DiagLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pDiagPostEnqMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pPrismSynchronousLinearSequencerContext->execute ();
    }

#endif
}
