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
 *   Author : pbalacha													   *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Rmon/NsmRmonLocalWorker.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"

#include "Nsm/Local/Rmon/RmonStatisticsLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonStatisticsBookKeepLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonHistoryLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonHistoryBookKeepLocalManagedObject.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionStatisticsMessage.h"
#include "Nsm/Local/Rmon/RmonCreateRmonCollectionHistoryMessage.h"
#include "Nsm/Local/Rmon/RmonDeleteRmonCollectionStatisticsMessage.h"
#include "Nsm/Local/Rmon/RmonDeleteRmonCollectionHistoryMessage.h"

//ClientInterface
#include "ClientInterface/Rmon/RmonClientCreateCollectionHistoryMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteCollectionHistoryMessage.h"
#include "ClientInterface/Rmon/RmonClientCreateCollectionStatisticsMessage.h"
#include "ClientInterface/Rmon/RmonClientDeleteCollectionStatisticsMessage.h"
#include "ClientInterface/Rmon/RmonUtils.h"

#include "Rmon/Global/RmonAlarmManagedObject.h"

#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/InterfaceCommon.h"
#include "Framework/Utils/StringUtils.h"
#include "DcmCStatus.h"


namespace DcmNs
{

    NsmRmonLocalWorker::NsmRmonLocalWorker (NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager) 
    {
        RmonStatisticsLocalManagedObject    RmonStatisticsLocalManagedObject (pNsmLocalObjectManager);
        RmonHistoryLocalManagedObject    RmonHistoryLocalManagedObject (pNsmLocalObjectManager);
        RmonStatisticsBookKeepLocalManagedObject    RmonStatisticsBookKeepLocalManagedObject(pNsmLocalObjectManager);
		RmonHistoryBookKeepLocalManagedObject    RmonHistoryBookKeepLocalManagedObject(pNsmLocalObjectManager);

        RmonStatisticsLocalManagedObject.setupOrm ();
        addManagedClass (RmonStatisticsLocalManagedObject::getClassName (), this);
        RmonHistoryLocalManagedObject.setupOrm ();
        addManagedClass (RmonHistoryLocalManagedObject::getClassName (), this);
        RmonStatisticsBookKeepLocalManagedObject.setupOrm ();
        addManagedClass (RmonStatisticsBookKeepLocalManagedObject::getClassName (), this);
        RmonHistoryBookKeepLocalManagedObject.setupOrm ();
        addManagedClass (RmonHistoryBookKeepLocalManagedObject::getClassName (), this);


        addOperationMap (RMONCREATERMONCOLLECTIONSTATISTICS,
			reinterpret_cast<PrismMessageHandler> (&NsmRmonLocalWorker::RmonCreateRmonCollectionStatisticsMessageHandler));
        addOperationMap (RMONCREATERMONCOLLECTIONHISTORY,
			reinterpret_cast<PrismMessageHandler> (&NsmRmonLocalWorker::RmonCreateRmonCollectionHistoryMessageHandler));
        addOperationMap (RMONDELETERMONCOLLECTIONSTATISTICS,
			reinterpret_cast<PrismMessageHandler> (&NsmRmonLocalWorker::RmonDeleteRmonCollectionStatisticsMessageHandler));
        addOperationMap (RMONDELETERMONCOLLECTIONHISTORY,
			reinterpret_cast<PrismMessageHandler> (&NsmRmonLocalWorker::RmonDeleteRmonCollectionHistoryMessageHandler));
    }

    NsmRmonLocalWorker::~NsmRmonLocalWorker ()
    {
    }

    PrismMessage  *NsmRmonLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case RMONCREATERMONCOLLECTIONSTATISTICS :
                pPrismMessage = new RmonCreateRmonCollectionStatisticsMessage ();
                break;
            case RMONCREATERMONCOLLECTIONHISTORY :
                pPrismMessage = new RmonCreateRmonCollectionHistoryMessage ();
                break;
            case RMONDELETERMONCOLLECTIONSTATISTICS :
                pPrismMessage = new RmonDeleteRmonCollectionStatisticsMessage ();
                break;
            case RMONDELETERMONCOLLECTIONHISTORY :
                pPrismMessage = new RmonDeleteRmonCollectionHistoryMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *NsmRmonLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RmonStatisticsLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonStatisticsLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((RmonHistoryLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonHistoryLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((RmonStatisticsBookKeepLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonStatisticsBookKeepLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((RmonHistoryBookKeepLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RmonHistoryBookKeepLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "NsmRmonLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId NsmRmonLocalWorker::createRmonHistoryValidationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage =
          dynamic_cast<RmonCreateRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
        SI32 historyIndex = pRmonCreateRmonCollectionHistoryMessage->getHistoryIndex();
		UI32 count = 0;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonHistoryBookKeepLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeSI32(historyIndex, "historyIndex"));
		querySynchronouslyForCount (&syncQueryCtxt, count);

        if (count > 0) {
            status = WRC_RMON_DCM_HISTORY_INDEX_ALREADY_ENABLED_MC;
        }

        return (status);
    }


	ResourceId NsmRmonLocalWorker::createRmonHistoryStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage =
          dynamic_cast<RmonCreateRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
		string ifName = pRmonCreateRmonCollectionHistoryMessage->getIfName();

		if (NsmUtils::isValidThreeTuple(ifName))
		{
			NsmUtils::getTwoTupleIfName(pRmonCreateRmonCollectionHistoryMessage->getIfName(), ifName);
		}

		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pRmonCreateRmonCollectionHistoryMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
		RMON_PLUGIN_TRACE_DEBUG(string("ifName=") + ifName);


		RmonClientCreateCollectionHistoryMessage *m = new RmonClientCreateCollectionHistoryMessage();
		m->setIfName(ifName);
		m->setHistoryIndex(pRmonCreateRmonCollectionHistoryMessage->getHistoryIndex());
		m->setBuckets(pRmonCreateRmonCollectionHistoryMessage->getBuckets());
		m->setInterval(pRmonCreateRmonCollectionHistoryMessage->getInterval());
		m->setOwner(pRmonCreateRmonCollectionHistoryMessage->getOwner());

		status = sendSynchronouslyToWaveClient ("rmon", m);
        status = errorCode(pRmonCreateRmonCollectionHistoryMessage, m, status);
        delete m;
        return (status);
    }

	ResourceId NsmRmonLocalWorker::createRmonHistoryMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage =
          dynamic_cast<RmonCreateRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		RMON_PLUGIN_TRACE();
		string ifName = pRmonCreateRmonCollectionHistoryMessage->getIfName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectFieldForObjectId("objectId");
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults)
		{
			if ( 1 == pResults->size())
			{
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			}
			else 
			{
				RMON_PLUGIN_TRACE_FATAL(string("PhyInt MO search error:") + pResults->size());
			}
		}
		prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

		RmonHistoryLocalManagedObject *historyMO =
			new RmonHistoryLocalManagedObject ( dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()),
           	pRmonCreateRmonCollectionHistoryMessage->getHistoryIndex(), pRmonCreateRmonCollectionHistoryMessage->getBuckets(),
			pRmonCreateRmonCollectionHistoryMessage->getInterval(), pRmonCreateRmonCollectionHistoryMessage->getOwner());

		historyMO->setOwnerManagedObjectId(pPhyIntfLocalManagedObject->getObjectId());
		addToComposition(PhyIntfLocalManagedObject::getClassName(), RmonHistoryLocalManagedObject::getClassName(),"rmonHistory", pPhyIntfLocalManagedObject->getObjectId(), historyMO->getObjectId());
		status = pRmonCreateRmonCollectionHistoryMessage->getCompletionStatus ();
		RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
		if (pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
		return status;
	}

    ResourceId NsmRmonLocalWorker::createRmonHistoryBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage =
          dynamic_cast<RmonCreateRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        SI32 historyIndex = pRmonCreateRmonCollectionHistoryMessage->getHistoryIndex();
        string ifName = pRmonCreateRmonCollectionHistoryMessage->getIfName();

        RmonHistoryBookKeepLocalManagedObject *historyBookKeepMO =
            new RmonHistoryBookKeepLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()), historyIndex, ifName);
        RMON_PLUGIN_TRACE_DEBUG(string("statsIndex in book keep=") + historyBookKeepMO->getHistoryIndex());

		if (historyBookKeepMO)
		{
			pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(historyBookKeepMO);
		}
        return (WRC_WAVE_MESSAGE_SUCCESS);
    }


    void  NsmRmonLocalWorker::RmonCreateRmonCollectionHistoryMessageHandler(
		RmonCreateRmonCollectionHistoryMessage *pRmonCreateRmonCollectionHistoryMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonHistoryValidationStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonHistoryStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonHistoryMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonHistoryBookKeepMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonCreateRmonCollectionHistoryMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId NsmRmonLocalWorker::createRmonStatsValidationStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage =
          dynamic_cast<RmonCreateRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
		SI32 statsIndex = pRmonCreateRmonCollectionStatisticsMessage->getStatsIndex();
		UI32 count = 0;

    	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonStatisticsBookKeepLocalManagedObject::getClassName());
    	syncQueryCtxt.addAndAttribute (new AttributeSI32(statsIndex, "statsIndex"));
		querySynchronouslyForCount (&syncQueryCtxt, count);

        if (count > 0) {
            status = WRC_RMON_DCM_STATS_INDEX_ALREADY_ENABLED_MC;
        }	
        return (status);
	}

	ResourceId NsmRmonLocalWorker::createRmonStatsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage =
          dynamic_cast<RmonCreateRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
		string ifName = pRmonCreateRmonCollectionStatisticsMessage->getIfName();

		if (NsmUtils::isValidThreeTuple(ifName))
		{
			NsmUtils::getTwoTupleIfName(pRmonCreateRmonCollectionStatisticsMessage->getIfName(), ifName);
		}

		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pRmonCreateRmonCollectionStatisticsMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
		RMON_PLUGIN_TRACE_DEBUG(string("ifName=") + ifName);


		RmonClientCreateCollectionStatisticsMessage *m = new RmonClientCreateCollectionStatisticsMessage();
		m->setIfName(ifName);
		m->setStatsIndex(pRmonCreateRmonCollectionStatisticsMessage->getStatsIndex());
		m->setOwner(pRmonCreateRmonCollectionStatisticsMessage->getOwner());

		status = sendSynchronouslyToWaveClient ("rmon", m);
        status = errorCode(pRmonCreateRmonCollectionStatisticsMessage, m, status);
        delete m;
        return (status);
    }

	ResourceId NsmRmonLocalWorker::createRmonStatsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage =
          dynamic_cast<RmonCreateRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		RMON_PLUGIN_TRACE();
		string ifName = pRmonCreateRmonCollectionStatisticsMessage->getIfName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addSelectFieldForObjectId("objectId");
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifname"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults)
		{
			if ( 1 == pResults->size())
			{
				RMON_PLUGIN_TRACE_DEBUG("PhyInt MO found");
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
			}
			else 
			{
				RMON_PLUGIN_TRACE_FATAL(string("PhyInt MO search error:") + pResults->size());
			}
		}
		prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

		RmonStatisticsLocalManagedObject *statsMO =
			new RmonStatisticsLocalManagedObject( dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()),
           	pRmonCreateRmonCollectionStatisticsMessage->getStatsIndex(), pRmonCreateRmonCollectionStatisticsMessage->getOwner());
		statsMO->setOwnerManagedObjectId(pPhyIntfLocalManagedObject->getObjectId());
		addToComposition(PhyIntfLocalManagedObject::getClassName(), RmonStatisticsLocalManagedObject::getClassName(),"rmonStats", pPhyIntfLocalManagedObject->getObjectId(), statsMO->getObjectId());
		
		status = pRmonCreateRmonCollectionStatisticsMessage->getCompletionStatus ();
		RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
		if (pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
		return status;
	}

	ResourceId NsmRmonLocalWorker::createRmonStatsBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage =
          dynamic_cast<RmonCreateRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		SI32 statsIndex = pRmonCreateRmonCollectionStatisticsMessage->getStatsIndex();
		string ifName = pRmonCreateRmonCollectionStatisticsMessage->getIfName();

		RmonStatisticsBookKeepLocalManagedObject *statsBookKeepMO =
			new RmonStatisticsBookKeepLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()), statsIndex, ifName);
		RMON_PLUGIN_TRACE_DEBUG(string("statsIndex in book keep=") + statsBookKeepMO->getStatsIndex());

		if (statsBookKeepMO)
		{
			pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(statsBookKeepMO);
		}
		return (WRC_WAVE_MESSAGE_SUCCESS);
	}

    void  NsmRmonLocalWorker::RmonCreateRmonCollectionStatisticsMessageHandler(
		RmonCreateRmonCollectionStatisticsMessage *pRmonCreateRmonCollectionStatisticsMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonStatsValidationStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonStatsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonStatsMOStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::createRmonStatsBookKeepMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonCreateRmonCollectionStatisticsMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId NsmRmonLocalWorker::deleteRmonHistoryStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionHistoryMessage *pRmonDeleteRmonCollectionHistoryMessage =
          dynamic_cast<RmonDeleteRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
		string ifName = pRmonDeleteRmonCollectionHistoryMessage->getIfName();

		if (NsmUtils::isValidThreeTuple(ifName))
		{
			NsmUtils::getTwoTupleIfName(pRmonDeleteRmonCollectionHistoryMessage->getIfName(), ifName);
		}

		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pRmonDeleteRmonCollectionHistoryMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
		RMON_PLUGIN_TRACE_DEBUG(string("ifName=") + ifName);

		RmonClientDeleteCollectionHistoryMessage *m = new RmonClientDeleteCollectionHistoryMessage();
		m->setIfName(ifName);
		m->setHistoryIndex(pRmonDeleteRmonCollectionHistoryMessage->getHistoryIndex());

		status = sendSynchronouslyToWaveClient ("rmon", m);
        status = errorCode(pRmonDeleteRmonCollectionHistoryMessage, m, status);
        delete m;
        return (status);
    }

	ResourceId NsmRmonLocalWorker::deleteRmonHistoryMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionHistoryMessage *pRmonDeleteRmonCollectionHistoryMessage =
          dynamic_cast<RmonDeleteRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		UI32 numberOfResults = 0;

		int historyIndex = pRmonDeleteRmonCollectionHistoryMessage->getHistoryIndex();
		WaveManagedObjectSynchronousQueryContext queryCtxtForChild(RmonHistoryLocalManagedObject::getClassName());
		queryCtxtForChild.addSelectFieldForObjectId("objectId");
		queryCtxtForChild.addSelectFieldForObjectId("ownerManagedObjectId");
        queryCtxtForChild.addAndAttribute (new AttributeUI64 (historyIndex, "historyindex"));

        vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);
        if (NULL != pResultsChildMO) {
		  	numberOfResults = pResultsChildMO->size ();
            if (0 == numberOfResults) {
			  	RMON_PLUGIN_TRACE_DEBUG(string("Can't find the MO"));
                status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                RmonHistoryLocalManagedObject *historyMO = dynamic_cast<RmonHistoryLocalManagedObject *>((*pResultsChildMO)[0]);
                deleteFromComposition(PhyIntfLocalManagedObject::getClassName(), RmonHistoryLocalManagedObject::getClassName(),
                    "rmonHistory", historyMO->getOwnerManagedObjectId(), historyMO->getObjectId());
            } 
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsChildMO);
            delete pResultsChildMO;
        }

		status = pRmonDeleteRmonCollectionHistoryMessage->getCompletionStatus ();
		RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
		return status;
	}

	ResourceId NsmRmonLocalWorker::deleteRmonHistoryBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionHistoryMessage *pRmonDeleteRmonCollectionHistoryMessage =
          dynamic_cast<RmonDeleteRmonCollectionHistoryMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status;
        RmonHistoryBookKeepLocalManagedObject *historyBookKeepMO = NULL;
        SI32 historyIndex = pRmonDeleteRmonCollectionHistoryMessage->getHistoryIndex();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonHistoryBookKeepLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeSI32(&historyIndex,"historyIndex"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            if ( 1 == pResults->size())
            {
                historyBookKeepMO = dynamic_cast<RmonHistoryBookKeepLocalManagedObject *>((*pResults)[0]);
            }
            else
            {
                RMON_PLUGIN_TRACE_FATAL(string("RMON MO search error:") + pResults->size());
            }
        }
        prismAssert (NULL != historyBookKeepMO, __FILE__, __LINE__);
        delete historyBookKeepMO;

        if (NULL != pResults) {
		  	pResults->clear ();
            delete pResults;
        }

        status = pRmonDeleteRmonCollectionHistoryMessage->getCompletionStatus ();
        RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
        return status;
    }	

    void  NsmRmonLocalWorker::RmonDeleteRmonCollectionHistoryMessageHandler
	  ( RmonDeleteRmonCollectionHistoryMessage *pRmonDeleteRmonCollectionHistoryMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonHistoryStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonHistoryMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonHistoryBookKeepMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonDeleteRmonCollectionHistoryMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId NsmRmonLocalWorker::deleteRmonStatsValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
		UI32 count = 0;
        RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage =
          dynamic_cast<RmonDeleteRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;;
        SI32 statsIndex = pRmonDeleteRmonCollectionStatisticsMessage->getStatsIndex();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonAlarmManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI64 (statsIndex, "statsindex"));
        querySynchronouslyForCount (&syncQueryCtxt, count);

        if(count > 0) {
            status = WRC_RMON_DCM_CANT_REMOVE_ALARM_CONFIGURED_COLLECTION_STATS;
        }

        return (status);
    }
	

	ResourceId NsmRmonLocalWorker::deleteRmonStatsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage =
          dynamic_cast<RmonDeleteRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		char buffer[25];
		buffer[24] = '\0';
		UI32 bufferSize = sizeof (buffer);
		string ifName = pRmonDeleteRmonCollectionStatisticsMessage->getIfName();

		if (NsmUtils::isValidThreeTuple(ifName))
		{
			NsmUtils::getTwoTupleIfName(pRmonDeleteRmonCollectionStatisticsMessage->getIfName(), ifName);
		}

		GENERATE_SHORT_IF_NAME_FOR_BACK_END(pRmonDeleteRmonCollectionStatisticsMessage->getIfType(), ifName.c_str(), buffer, bufferSize);
		ifName = buffer;
		RMON_PLUGIN_TRACE_DEBUG(string("ifName=") + ifName);


		RmonClientDeleteCollectionStatisticsMessage *m = new RmonClientDeleteCollectionStatisticsMessage();
		m->setIfName(ifName);
		m->setStatsIndex(pRmonDeleteRmonCollectionStatisticsMessage->getStatsIndex());

		status = sendSynchronouslyToWaveClient ("rmon", m);
        status = errorCode(pRmonDeleteRmonCollectionStatisticsMessage, m, status);
        delete m;
        return (status);
    }

	ResourceId NsmRmonLocalWorker::deleteRmonStatsMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage =
          dynamic_cast<RmonDeleteRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		ResourceId status;
		UI32 numberOfResults = 0;

		int statsIndex = pRmonDeleteRmonCollectionStatisticsMessage->getStatsIndex();
        WaveManagedObjectSynchronousQueryContext queryCtxtForChild(RmonStatisticsLocalManagedObject::getClassName());
        queryCtxtForChild.addSelectFieldForObjectId("objectId");
        queryCtxtForChild.addSelectFieldForObjectId("ownerManagedObjectId");
        queryCtxtForChild.addAndAttribute (new AttributeUI64 (statsIndex, "statsindex"));
		vector<WaveManagedObject *> *pResultsChildMO = querySynchronously (&queryCtxtForChild);

	 	if (NULL != pResultsChildMO) {
		  	numberOfResults = pResultsChildMO->size ();
            if (0 == numberOfResults) {
				RMON_PLUGIN_TRACE_DEBUG(string("can't find the MO"));
                status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                RmonStatisticsLocalManagedObject *statsMO = dynamic_cast<RmonStatisticsLocalManagedObject *>((*pResultsChildMO)[0]);
                deleteFromComposition(PhyIntfLocalManagedObject::getClassName(), RmonStatisticsLocalManagedObject::getClassName(),
                    "rmonStats", statsMO->getOwnerManagedObjectId(), statsMO->getObjectId());
            } 
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResultsChildMO);
            delete pResultsChildMO;
        }

		status = pRmonDeleteRmonCollectionStatisticsMessage->getCompletionStatus ();
		RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
		return status;
	}

	ResourceId NsmRmonLocalWorker::deleteRmonStatsBookKeepMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage =
          dynamic_cast<RmonDeleteRmonCollectionStatisticsMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ResourceId status;
        RmonStatisticsBookKeepLocalManagedObject *statsBookKeepMO = NULL;
        RMON_PLUGIN_TRACE();
        SI32 statsIndex = pRmonDeleteRmonCollectionStatisticsMessage->getStatsIndex();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RmonStatisticsBookKeepLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeSI32(&statsIndex,"statsIndex"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (NULL != pResults)
        {
            if ( 1 == pResults->size())
            {
                statsBookKeepMO = dynamic_cast<RmonStatisticsBookKeepLocalManagedObject *>((*pResults)[0]);
            }
            else
            {
                RMON_PLUGIN_TRACE_FATAL(string("RMON MO search error:") + pResults->size());
            }
        }
        prismAssert (NULL != statsBookKeepMO, __FILE__, __LINE__);
        delete statsBookKeepMO;

        if (NULL != pResults) {
		  	pResults->clear ();
            delete pResults;
        }

        status = pRmonDeleteRmonCollectionStatisticsMessage->getCompletionStatus ();
        RMON_PLUGIN_TRACE_DEBUG(string("status=") + status);
        return status;
    }

    void  NsmRmonLocalWorker::RmonDeleteRmonCollectionStatisticsMessageHandler
	  ( RmonDeleteRmonCollectionStatisticsMessage *pRmonDeleteRmonCollectionStatisticsMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonStatsValidationStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonStatsStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonStatsMOStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::deleteRmonStatsBookKeepMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&NsmRmonLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext =
		  new PrismSynchronousLinearSequencerContext (pRmonDeleteRmonCollectionStatisticsMessage, this, sequencerSteps,
			  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId NsmRmonLocalWorker::errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
        pMsg->setCompletionStatus(status);
        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                status = clientMsg->getClientStatus();
                trace (TRACE_LEVEL_DEBUG, string("clientMsg->getClientStatus()") + status);
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
    }

    ResourceId NsmRmonLocalWorker::mapBackEndErrorToDcmdError(int rc)
    {
        ResourceId errorId = WAVE_MESSAGE_SUCCESS;
        return errorId;
    }
}
