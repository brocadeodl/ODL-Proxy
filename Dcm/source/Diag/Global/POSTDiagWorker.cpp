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
 *   Author : Subhani Shaik                                                     *
 **************************************************************************/

#include "Diag/Global/DiagObjectManager.h"
#include "Diag/Global/POSTDiagWorker.h"
#include "Diag/Global/DiagPostManagedObject.h"
#include "Diag/Global/DiagTypes.h"
#include "Diag/Global/DiagPostMessage.h"
#include "Diag/Local/DiagLocalPostMessage.h"

#include "DcmResourceIdEnums.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"

#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Boot/BootCompleteForThisLocationEvent.h"
#include "Framework/Boot/BootTypes.h"

namespace DcmNs
{

    POSTDiagWorker::POSTDiagWorker (DiagObjectManager *pDiagObjectManager)
        : WaveWorker  (pDiagObjectManager)
    {
        DiagPostManagedObject    DiagPostManagedObject    (pDiagObjectManager);
        DiagPostManagedObject.setupOrm ();
        addManagedClass (DiagPostManagedObject::getClassName (), this);
		addOperationMap (DIAGPOST, reinterpret_cast<PrismMessageHandler> (&POSTDiagWorker::DiagPostMessageHandler));
    }

    POSTDiagWorker::~POSTDiagWorker ()
    {
    }

    PrismMessage  *POSTDiagWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *POSTDiagWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((DiagPostManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new DiagPostManagedObject(dynamic_cast<DiagObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "POSTDiagWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	void POSTDiagWorker::DiagPostMessageHandler(DiagPostMessage *pDiagPostMessage)
	{
		//As per the new functionality - Diag Post is configured locally in
		//individual switches.The code base related to Global object is no
		//longer required.Need to clean up all these.  
		#if 0
    	trace (TRACE_LEVEL_DEBUG, "POSTDiagWorker::DiagPostMessageHandler()");

	    if (pDiagPostMessage == NULL)
    	{
        	trace (TRACE_LEVEL_FATAL, "NULL Message arrived in the handler");
        	prismAssert (false, __FILE__, __LINE__);
    	}

	    /* Retrieve the DiagPost Managed Object */
	    WaveManagedObjectSynchronousQueryContext *synchronousQueryContext = new WaveManagedObjectSynchronousQueryContext(DiagPostManagedObject::getClassName());
	    vector<WaveManagedObject *> *pResults = querySynchronously (synchronousQueryContext);
    	if ((NULL == pResults) || (pResults->size() != 1))
	    {
    	    if (pResults == NULL) trace (TRACE_LEVEL_FATAL, "Zero DiagPost instances found.");
        	else if (pResults->size() != 1) trace (TRACE_LEVEL_FATAL, "More than one DiagPost instances found.");
	        trace (TRACE_LEVEL_FATAL, "Unexpected number of DiagPost instances");
    	    pDiagPostMessage->setCompletionStatus (DIAG_POST_SET_ERROR);
        	reply (pDiagPostMessage);
	        delete synchronousQueryContext;
    	    if (pResults != NULL) {
        	    for (unsigned int j = 0; j < pResults->size(); j++)
            	    delete (*pResults)[j];

	            delete pResults;
    	    }
        	return;
	    }
	
	    DiagPostManagedObject *pDiagPostManagedObj = dynamic_cast<DiagPostManagedObject *> ((*pResults)[0]);

		// Validations and Queries should go here
		bool enablePost;
		ResourceId status;
	
		enablePost = pDiagPostMessage->getDiagPost();
			
		// Send message to all local services
    	char debugInfo[20];
	    vector<LocationId> connectedLocationsVector;

    	FrameworkToolKit::getConnectedLocations(connectedLocationsVector);
	    connectedLocationsVector.push_back(FrameworkToolKit::getThisLocationId());
		
	    for (unsigned int i = 0; i < connectedLocationsVector.size(); i++)
    	{
			sprintf(debugInfo, "%d", connectedLocationsVector[i]);
    		DiagLocalPostMessage *pMessage = new DiagLocalPostMessage ((UI32)connectedLocationsVector[i],enablePost);
	        status = sendSynchronously (pMessage, connectedLocationsVector[i]);
    	    if (WAVE_MESSAGE_SUCCESS == status &&
				 ((status = pMessage->getCompletionStatus()) == WAVE_MESSAGE_SUCCESS))		
        	{
            	trace (TRACE_LEVEL_DEBUG, string("Backend operation is successful on local node with location id ") + string(debugInfo));
	        }
    	    else
        	{
            	trace (TRACE_LEVEL_ERROR, string("Backend operation failed on local node with location id ") + string(debugInfo) + string(". Framework error - ") + FrameworkToolKit::localize (status));
            // ROLL BACK - Send 'delete' messages to all locations ids that were successful
	            ResourceId delStatus = WAVE_MESSAGE_SUCCESS;
        	    for (unsigned int j = 0; j < i; j++)
            	{
                	sprintf(debugInfo, "%d", connectedLocationsVector[j]);
		    		DiagLocalPostMessage *pRestoreMessage = new DiagLocalPostMessage ((UI32) connectedLocationsVector[j],pDiagPostManagedObj->getDiagPost());
	                delStatus = sendSynchronously (pRestoreMessage, connectedLocationsVector[j]);
    	            if (WAVE_MESSAGE_SUCCESS == delStatus &&
						((delStatus = pMessage->getCompletionStatus()) == WAVE_MESSAGE_SUCCESS))
        	        {
            	        trace (TRACE_LEVEL_DEBUG, string("Rollback operation is successful on local node with location id ") + string(debugInfo));
                	}
	                else
    	            {
        	            trace (TRACE_LEVEL_ERROR, string("Rollback operation failed on local node with location id ") + string(debugInfo) + string(". Framework error - ") + FrameworkToolKit::localize (status));
            	        trace (TRACE_LEVEL_FATAL, string("Local node with location id ") + string(debugInfo) + " is in DEVIL state. Please configure it manually to be in sync with the cluster.");
                	}
					delete pRestoreMessage;
    			}
				pDiagPostMessage->setCompletionStatus (DIAG_POST_SET_ERROR);
				delete pMessage;
	            break;        	
			}
			delete pMessage;
		}

	    // Update and commit to the database
    	if (WAVE_MESSAGE_SUCCESS == status)
    	{	
        	startTransaction ();

	        updateWaveManagedObject (pDiagPostManagedObj);

           	pDiagPostManagedObj->setDiagPost(enablePost);

	        status = commitTransaction ();
    	    if (FRAMEWORK_SUCCESS == status)
        	{
            	trace (TRACE_LEVEL_INFO, "Commiting Diag Post State to Wave database is successful.");
	            status = WAVE_MESSAGE_SUCCESS;
    	    }
        	else
        	{
            	status = DIAG_POST_SET_ERROR;
	            trace (TRACE_LEVEL_ERROR, "Commiting Diag Post failed. Framework error - " + FrameworkToolKit::localize (status));
    	    }
       		pDiagPostMessage->setCompletionStatus (status);
    	}
		delete pDiagPostManagedObj;
		delete pResults;
	    delete synchronousQueryContext;
	
		reply (pDiagPostMessage);
		#endif
	}

}
