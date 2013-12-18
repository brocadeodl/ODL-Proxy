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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCore/DcmToolKit.h"

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Dot1xLocalWorker.h"
#include "Nsm/Local/NsmDot1xUpdateConfigSPMessage.h"
#include "Nsm/Local/NsmDot1xUpdatePortConfigMessage.h"
#include "Nsm/Local/Dot1xPortConfigLocalManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "ClientInterface/Dot1X/NsmClientDot1xUpdatePortConfigMessage.h"
#include "ClientInterface/Dot1X/Dot1XMessageDef.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Local/NsmTypes.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCStatus.h"

namespace DcmNs
{

    Dot1xLocalWorker::Dot1xLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        Dot1xPortConfigLocalManagedObject    Dot1xPortConfigLocalManagedObject    (pNsmLocalObjectManager);
        PhyIntfLocalManagedObject    PhyIntfLocalManagedObject    (pNsmLocalObjectManager);

        Dot1xPortConfigLocalManagedObject.setupOrm ();
        addManagedClass (Dot1xPortConfigLocalManagedObject::getClassName (), this);

        addOperationMap (NSMDOT1XUPDATECONFIGSP, reinterpret_cast<PrismMessageHandler> (&Dot1xLocalWorker::NsmDot1xUpdateConfigSPMessageHandler));
        addOperationMap (NSMDOT1XUPDATEPORTCONFIG, reinterpret_cast<PrismMessageHandler> (&Dot1xLocalWorker::NsmDot1xUpdatePortConfigMessageHandler));
    }

    Dot1xLocalWorker::~Dot1xLocalWorker ()
    {
    }

    PrismMessage  *Dot1xLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;
	  trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker createMessageInstance:: "));

        switch (operationCode)
        {

            case NSMDOT1XUPDATECONFIGSP :
                pPrismMessage = new NsmDot1xUpdateConfigSPMessage ();
                break;
            case NSMDOT1XUPDATEPORTCONFIG :
                pPrismMessage = new NsmDot1xUpdatePortConfigMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *Dot1xLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
	  trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker createManagedObjectInstance:: "));

        if ((Dot1xPortConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new Dot1xPortConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
#if 0
        else if ((PhyIntfLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PhyIntfLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
#endif
        else
        {
            trace (TRACE_LEVEL_FATAL, "Dot1xLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  Dot1xLocalWorker::NsmDot1xUpdateConfigSPMessageHandler( NsmDot1xUpdateConfigSPMessage *pNsmDot1xUpdateConfigSPMessage)
    {
// Validations and Queries should go here

	  trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker NsmDot1xUpdateConfigSPMessageHandler:: "));

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDot1xUpdateConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId Dot1xLocalWorker::Dot1xUpdateMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
        NsmDot1xUpdatePortConfigMessage *pNsmDot1xUpdatePortConfigMessage = dynamic_cast<NsmDot1xUpdatePortConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker Entering Dot1xUpdateMOStep:: ----> "));
        string ifName = pNsmDot1xUpdatePortConfigMessage->getIfname();
        //ResourceId  status   = WAVE_MESSAGE_ERROR;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> pDot1xPortConfigLocalManagedObject;
		Dot1xPortConfigLocalManagedObject *pDot1xPortConfigLocalManagedObject_new = NULL;
		Dot1xPortConfigLocalManagedObject *pDot1xManagedObject = NULL;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                //status = WAVE_MESSAGE_ERROR;
            }else if (1 == numberOfResults) {
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				pDot1xPortConfigLocalManagedObject = WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pPhyIntfLocalManagedObject->getDot1xIntfConfig());

                //status = WAVE_MESSAGE_SUCCESS;
            } else {
                //status = WAVE_MESSAGE_ERROR;
            }

			if(pPrismSynchronousLinearSequencerContext)
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            pResults->clear ();
            delete pResults;
        }

        if (pPhyIntfLocalManagedObject == NULL) {
            return (WAVE_MESSAGE_ERROR);
		} else {

		  pDot1xManagedObject =  pDot1xPortConfigLocalManagedObject.operator-> ();
		  if (pDot1xManagedObject){
			int opCode = pNsmDot1xUpdatePortConfigMessage->getOpcode();

			switch(pNsmDot1xUpdatePortConfigMessage->getCmdcode())
			{
			  case DOT1X_ENABLED :
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setEnabled(pNsmDot1xUpdatePortConfigMessage->getEnabled());
				else if(opCode == DOT1X_DELETE_OP) {
				  pDot1xPortConfigLocalManagedObject->setEnabled(false);
				  pDot1xPortConfigLocalManagedObject->setPort_ctrl_type(1);
				  pDot1xPortConfigLocalManagedObject->setProto_version(2);
				  pDot1xPortConfigLocalManagedObject->setQuiet_interval(60);
				  pDot1xPortConfigLocalManagedObject->setMax_reauth_attempt(2);
				  pDot1xPortConfigLocalManagedObject->setEnable_auth(false);
				  pDot1xPortConfigLocalManagedObject->setReauth_timeout_interval(3600);
				  pDot1xPortConfigLocalManagedObject->setServer_timeout_interval(30);
				  pDot1xPortConfigLocalManagedObject->setSupp_timeout_interval(30);
				  pDot1xPortConfigLocalManagedObject->setTx_timeout_interval(30);
				  pPhyIntfLocalManagedObject->setDot1xIntfConfig (WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (NULL));
				  updateWaveManagedObject (pPhyIntfLocalManagedObject);

				  trace (TRACE_LEVEL_DEBUG, string ("---------> Inside Dot1xUpdateMOStep:: CLEANED UPPPP and RETURNING --->"));
				  return WAVE_MESSAGE_SUCCESS;
				}
				break;
			  case DOT1X_PORT_CTRL_TYPE:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setPort_ctrl_type(pNsmDot1xUpdatePortConfigMessage->getPort_ctrl_type());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setPort_ctrl_type(1);
				break;
			  case DOT1X_PROTO_VERSION:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setProto_version(pNsmDot1xUpdatePortConfigMessage->getProto_version());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setProto_version(2);
				break;
			  case DOT1X_QUIET_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setQuiet_interval(pNsmDot1xUpdatePortConfigMessage->getQuiet_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setQuiet_interval(60);
				break;
			  case DOT1X_MAX_REAUTH_ATTEMPT:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setMax_reauth_attempt(pNsmDot1xUpdatePortConfigMessage->getMax_reauth_attempt());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setMax_reauth_attempt(2);
				break;
			  case DOT1X_ENABLE_AUTH:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setEnable_auth(pNsmDot1xUpdatePortConfigMessage->getEnable_auth());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setEnable_auth(false);
				break;
			  case DOT1X_REAUTH_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setReauth_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getReauth_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setReauth_timeout_interval(3600);
				break;
			  case DOT1X_SERVER_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setServer_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getServer_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setServer_timeout_interval(30);
				break;
			  case DOT1X_SUPP_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setSupp_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getSupp_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setSupp_timeout_interval(30);
				break;
			  case DOT1X_TX_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject->setTx_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getTx_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject->setTx_timeout_interval(30);
				break;
			}

			pPhyIntfLocalManagedObject->setDot1xIntfConfig (WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (NULL));

			//this need not be added to garbage collection separately because it will be in composition to PhyIntfMo. So this will automatically be garbage collected as part of presults.
			pDot1xPortConfigLocalManagedObject_new = new Dot1xPortConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

			pDot1xPortConfigLocalManagedObject_new->setIfName(pNsmDot1xUpdatePortConfigMessage->getIfname());
			pDot1xPortConfigLocalManagedObject_new->setEnabled(pDot1xPortConfigLocalManagedObject->getEnabled());
			pDot1xPortConfigLocalManagedObject_new->setPort_ctrl_type(pDot1xPortConfigLocalManagedObject->getPort_ctrl_type());
			pDot1xPortConfigLocalManagedObject_new->setProto_version(pDot1xPortConfigLocalManagedObject->getProto_version());
			pDot1xPortConfigLocalManagedObject_new->setQuiet_interval(pDot1xPortConfigLocalManagedObject->getQuiet_interval());
			pDot1xPortConfigLocalManagedObject_new->setMax_reauth_attempt(pDot1xPortConfigLocalManagedObject->getMax_reauth_attempt());
			pDot1xPortConfigLocalManagedObject_new->setEnable_auth(pDot1xPortConfigLocalManagedObject->getEnable_auth());
			pDot1xPortConfigLocalManagedObject_new->setSupp_timeout_interval(pDot1xPortConfigLocalManagedObject->getSupp_timeout_interval());
			pDot1xPortConfigLocalManagedObject_new->setTx_timeout_interval(pDot1xPortConfigLocalManagedObject->getTx_timeout_interval());
			pDot1xPortConfigLocalManagedObject_new->setReauth_timeout_interval(pDot1xPortConfigLocalManagedObject->getReauth_timeout_interval());
			pDot1xPortConfigLocalManagedObject_new->setServer_timeout_interval(pDot1xPortConfigLocalManagedObject->getServer_timeout_interval());

			prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

			trace (TRACE_LEVEL_DEBUG, string ("Inside Dot1xUpdateMOStep:: updating MO") + pNsmDot1xUpdatePortConfigMessage->getCmdcode());

			pPhyIntfLocalManagedObject->setDot1xIntfConfig(WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pDot1xPortConfigLocalManagedObject_new));
			updateWaveManagedObject (pPhyIntfLocalManagedObject);
		  }else {
			int opCode = pNsmDot1xUpdatePortConfigMessage->getOpcode();
			int cmdCode = pNsmDot1xUpdatePortConfigMessage->getCmdcode();

			if (opCode == DOT1X_DELETE_OP && (cmdCode == DOT1X_ENABLED)) {
			  trace (TRACE_LEVEL_DEBUG, string ("---------> Inside Dot1xUpdateMOStep:: returning without updatinggggg --->"));
			  return WAVE_MESSAGE_SUCCESS;
			}

			 //this need not be added to garbage collection separately because it will be in composition to PhyIntfMo. So this will automatically be garbage collected as part of presults.

			pDot1xPortConfigLocalManagedObject_new = new Dot1xPortConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));

			pDot1xPortConfigLocalManagedObject_new->setIfName(pNsmDot1xUpdatePortConfigMessage->getIfname());
			pDot1xPortConfigLocalManagedObject_new->setEnabled(true);
			pDot1xPortConfigLocalManagedObject_new->setPort_ctrl_type(1);
			pDot1xPortConfigLocalManagedObject_new->setProto_version(2);
			pDot1xPortConfigLocalManagedObject_new->setQuiet_interval(60);
			pDot1xPortConfigLocalManagedObject_new->setMax_reauth_attempt(2);
			pDot1xPortConfigLocalManagedObject_new->setEnable_auth(false);
			pDot1xPortConfigLocalManagedObject_new->setSupp_timeout_interval(30);
			pDot1xPortConfigLocalManagedObject_new->setTx_timeout_interval(30);
			pDot1xPortConfigLocalManagedObject_new->setReauth_timeout_interval(3600);
			pDot1xPortConfigLocalManagedObject_new->setServer_timeout_interval(30);

			switch(pNsmDot1xUpdatePortConfigMessage->getCmdcode())
			{
			  case DOT1X_ENABLED :
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setEnabled(pNsmDot1xUpdatePortConfigMessage->getEnabled());
				else if(opCode == DOT1X_DELETE_OP) {
				  pDot1xPortConfigLocalManagedObject_new->setEnabled(false);
				}
				break;
			  case DOT1X_PORT_CTRL_TYPE:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setPort_ctrl_type(pNsmDot1xUpdatePortConfigMessage->getPort_ctrl_type());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setPort_ctrl_type(1);
				break;
			  case DOT1X_PROTO_VERSION:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setProto_version(pNsmDot1xUpdatePortConfigMessage->getProto_version());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setProto_version(2);
				break;
			  case DOT1X_QUIET_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setQuiet_interval(pNsmDot1xUpdatePortConfigMessage->getQuiet_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setQuiet_interval(60);
				break;
			  case DOT1X_MAX_REAUTH_ATTEMPT:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setMax_reauth_attempt(pNsmDot1xUpdatePortConfigMessage->getMax_reauth_attempt());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setMax_reauth_attempt(2);
				break;
			  case DOT1X_ENABLE_AUTH:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setEnable_auth(pNsmDot1xUpdatePortConfigMessage->getEnable_auth());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setEnable_auth(false);
				break;
			  case DOT1X_REAUTH_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setReauth_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getReauth_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setReauth_timeout_interval(3600);
				break;
			  case DOT1X_SERVER_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setServer_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getServer_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setServer_timeout_interval(30);
				break;
			  case DOT1X_SUPP_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setSupp_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getSupp_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setSupp_timeout_interval(30);
				break;
			  case DOT1X_TX_TIMEOUT_INTERVAL:
				if (opCode == DOT1X_SET_OP)
				  pDot1xPortConfigLocalManagedObject_new->setTx_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getTx_timeout_interval());
				else if(opCode == DOT1X_DELETE_OP)
				  pDot1xPortConfigLocalManagedObject_new->setTx_timeout_interval(30);
				break;
			}


			prismAssert (NULL != pPhyIntfLocalManagedObject, __FILE__, __LINE__);

			trace (TRACE_LEVEL_DEBUG, string ("NEWWW Inside Dot1xUpdateMOStep:: updating MO") + pNsmDot1xUpdatePortConfigMessage->getCmdcode());
			pPhyIntfLocalManagedObject->setDot1xIntfConfig(WaveManagedObjectPointer<Dot1xPortConfigLocalManagedObject> (pDot1xPortConfigLocalManagedObject_new));
			updateWaveManagedObject (pPhyIntfLocalManagedObject);
		  }
	
		}

		trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker Leaving Dot1xUpdateMOStep"));

        return WAVE_MESSAGE_SUCCESS;
	}

    ResourceId Dot1xLocalWorker::Dot1xUpdate(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status = 0;

		trace (TRACE_LEVEL_DEBUG, string ("Dot1xLocalWorker Entering Dot1xUpdate"));
        NsmDot1xUpdatePortConfigMessage *pNsmDot1xUpdatePortConfigMessage = dynamic_cast<NsmDot1xUpdatePortConfigMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

		string ifName =  pNsmDot1xUpdatePortConfigMessage->getIfname();
        /* ISL port role change case : Nsm sends two tuple name */
    /*    if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (false  == NsmUtils::isValidThreeTuple(ifName))) 
		{
                NsmUtils::getThreeTupleIfName( pNsmUpdatePhyIntfMessage->getIfName(),ifName, 0);
                pNsmUpdatePhyIntfMessage->setIfName(ifName);
        }
*/
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        if (NULL != pResults)
	   	{
            UI32 numberOfResults = pResults->size();
            if (0 == numberOfResults) 
			{
            	return WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE;
            }else if (1 == numberOfResults) 
			{
                pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                status = WAVE_MESSAGE_SUCCESS;
            }
		   	else
		   	{
                return WAVE_MESSAGE_ERROR;
            }
        }

        if (pPhyIntfLocalManagedObject == NULL)
	   	{
            NSM_PLUG_DBG_ERR_S( string("Interface MO not found ..:") + ifName);
            return WRC_NSM_API_SET_ERR_NO_SUCH_INTERFACE;
        }
	   	else
	   	{
            NSM_PLUG_INFO_S(string("Validating Physical Interface MO : ") + ifName);

            /*Check ISL Port or Not */
            if ((DcmToolKit::isVcsEnabled() == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (pPhyIntfLocalManagedObject->getPortrole()))
		   	{
				return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
			}
		}

		if(true  == NsmUtils::isValidThreeTuple(ifName))
		{
			string twoTuppleIfName;
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			 ifName = twoTuppleIfName;
		}

        NsmClientDot1xUpdatePortConfigMessage *m = new NsmClientDot1xUpdatePortConfigMessage() ;
        m->setIfType(pNsmDot1xUpdatePortConfigMessage->getIfType());
        m->setIfName(ifName);
        m->setEnabled(pNsmDot1xUpdatePortConfigMessage->getEnabled());
        m->setPort_ctrl_type(pNsmDot1xUpdatePortConfigMessage->getPort_ctrl_type());
        m->setProto_version(pNsmDot1xUpdatePortConfigMessage->getProto_version());
		m->setQuiet_interval(pNsmDot1xUpdatePortConfigMessage->getQuiet_interval());
        m->setMax_reauth_attempt(pNsmDot1xUpdatePortConfigMessage->getMax_reauth_attempt());
        m->setEnable_auth(pNsmDot1xUpdatePortConfigMessage->getEnable_auth());
        m->setReauth_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getReauth_timeout_interval());
        m->setServer_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getServer_timeout_interval());
        m->setSupp_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getSupp_timeout_interval());
        m->setTx_timeout_interval(pNsmDot1xUpdatePortConfigMessage->getTx_timeout_interval());
        m->setOpcode(pNsmDot1xUpdatePortConfigMessage->getOpcode());
        m->setCmdCode(pNsmDot1xUpdatePortConfigMessage->getCmdcode());


		trace (TRACE_LEVEL_DEBUG, string ("------> Values: ") + ifName + pNsmDot1xUpdatePortConfigMessage->getEnabled() + pNsmDot1xUpdatePortConfigMessage->getPort_ctrl_type()+ pNsmDot1xUpdatePortConfigMessage->getProto_version() + pNsmDot1xUpdatePortConfigMessage->getQuiet_interval() + pNsmDot1xUpdatePortConfigMessage->getMax_reauth_attempt() + pNsmDot1xUpdatePortConfigMessage->getEnable_auth() + pNsmDot1xUpdatePortConfigMessage->getReauth_timeout_interval() + pNsmDot1xUpdatePortConfigMessage->getServer_timeout_interval() + pNsmDot1xUpdatePortConfigMessage->getSupp_timeout_interval() + pNsmDot1xUpdatePortConfigMessage->getTx_timeout_interval() + pNsmDot1xUpdatePortConfigMessage->getOpcode() + pNsmDot1xUpdatePortConfigMessage->getCmdcode() );

        trace (TRACE_LEVEL_DEBUG, "sending synchronously to authd client");
        status = sendSynchronouslyToWaveClient ("dauthd",m);

        if (NULL != pResults)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

		return errorCode(pNsmDot1xUpdatePortConfigMessage, m, status);
    }


    void  Dot1xLocalWorker::NsmDot1xUpdatePortConfigMessageHandler( NsmDot1xUpdatePortConfigMessage *pNsmDot1xUpdatePortConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::Dot1xUpdate),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::Dot1xUpdateMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&Dot1xLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmDot1xUpdatePortConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId Dot1xLocalWorker::errorCode(PrismMessage* loc_msg_p, DcmManagementInterfaceMessage* c_msg_p, ResourceId status)
    {
        NSM_PLUG_DBG_TRACE();
        loc_msg_p -> setCompletionStatus(status);
        if(WAVE_MESSAGE_SUCCESS == status)
        {
            if(WAVE_MESSAGE_SUCCESS != c_msg_p -> getCompletionStatus())
            {
                status = c_msg_p -> getCompletionStatus();
				loc_msg_p->setCompletionStatus(status);
            }
            else if(WAVE_MESSAGE_SUCCESS != c_msg_p -> getClientStatus())
            {
                status =  c_msg_p -> getClientStatus();
                loc_msg_p -> setCompletionStatus(status);
                NSM_PLUG_DBG_S(string("Error From Dot1X Client") + status);
            }
        }
        delete c_msg_p;
        return status;
    }	
}
