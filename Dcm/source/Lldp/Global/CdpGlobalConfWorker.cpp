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
 *   Author : mmohan                                                       *
 **************************************************************************/
#include "DcmCStatus.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"
#include "Utils/DceClusterUtils.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"

#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/CdpGlobalConfSetMessage.h"
#include "Lldp/Global/CdpGlobalConfManagedObject.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Lldp/Local/CdpConfSetMessage.h"
#include "Lldp/Common/LldpPluginDebug.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"

namespace DcmNs
{

    CdpGlobalConfWorker::CdpGlobalConfWorker (LldpGlobalObjectManager *pLldpGlobalObjectManager)
        : WaveWorker  (pLldpGlobalObjectManager)
    {
        CdpGlobalConfManagedObject    CdpGlobalConfManagedObject    (pLldpGlobalObjectManager);
        CdpGlobalConfManagedObject.setupOrm ();
        addManagedClass (CdpGlobalConfManagedObject::getClassName (), this);
        addOperationMap (CDPGLOBALCONFSET, reinterpret_cast<PrismMessageHandler> (&CdpGlobalConfWorker::CdpGlobalConfSetMessageHandler));
    }

    CdpGlobalConfWorker::~CdpGlobalConfWorker ()
    {
    }

    PrismMessage  *CdpGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case CDPGLOBALCONFSET :
                pPrismMessage = new CdpGlobalConfSetMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *CdpGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((CdpGlobalConfManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new CdpGlobalConfManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
			LLDP_PLUG_DBG(string("Unknown Managed Class Name:- ") + managedClassName);
        }
        return (pWaveManagedObject);
    }
	
	
    void
	CdpGlobalConfWorker::CdpGlobalConfigSendToClusterCallback (WaveSendToClusterContext *pWaveSendToClusterContext)
    {
		LLDP_PLUG_DBG_TRACE();
		ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(pWaveSendToClusterContext);
	    PrismLinearSequencerContext *pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>(pWaveSendToClusterContext->getPCallerContext());
		if(sendToClusterCompletionStatus == WAVE_MESSAGE_SUCCESS) {
			LLDP_PLUG_DBG(string("Phase 1 message succeeded on all locations"));
		} else if(pPrismLinearSequencerContext->getCompletionStatus() == WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on all locations"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_SUCCEEDED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback succeeded on all of the nodes"));
		} else if(sendToClusterCompletionStatus == WAVE_MESSAGE_ERROR_ROLLBACK_FAILED) {
			LLDP_PLUG_DBG(string("Phase 1 message failed on some of the locations and Rollback attempt on the succeeded nodes failed on some or all nodes"));
		} else {
			LLDP_PLUG_DBG(string("Unknown Error:- ") + sendToClusterCompletionStatus);
		}

        delete pWaveSendToClusterContext->getPPrismMessageForPhase1();
        delete pWaveSendToClusterContext;
		pPrismLinearSequencerContext->executeNextStep(sendToClusterCompletionStatus);
		LLDP_PLUG_DBG(string("Done"));
        return;
     }
	
    void CdpGlobalConfWorker::updateCdpGlobalConf (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		LLDP_PLUG_DBG_TRACE();
        CdpGlobalConfSetMessage *pCdpGlobalConfSetMessage = dynamic_cast<CdpGlobalConfSetMessage *> (pPrismLinearSequencerContext->getPPrismMessage ());

        CdpConfSetMessage *m = new CdpConfSetMessage();
        UI32 cmdcode = pCdpGlobalConfSetMessage->getCmdcode();
		WaveSendToClusterContext    *pWaveSendToClusterContext;
		
		/* create clustercontext and initialize with callback func */
		pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&CdpGlobalConfWorker::CdpGlobalConfigSendToClusterCallback), pPrismLinearSequencerContext);
		
        /* Read the header from received message and populate 
		 * sending message
		 */
		m->setMgmtFlags (pCdpGlobalConfSetMessage->getMgmtFlags());
        m->setMsgType (pCdpGlobalConfSetMessage->getMsgType());
        m->setCmdcode (pCdpGlobalConfSetMessage->getCmdcode());

        /* Read the data from received message and populate 
		 * sending message
		 */
        if (IS_CMDCODE_SET(cmdcode, CDP_GLOBAL_CONF_PROTO_STATUS))
            m->setProtoEnable (pCdpGlobalConfSetMessage->getProtoEnable());
        
		/* Enable surrogate support */
		m->setNeedSurrogateSupportFlag(true);
		/* Set Phase 1 message */
		pWaveSendToClusterContext->setPPrismMessageForPhase1(m);
		/* Enable partial success flag */
		pWaveSendToClusterContext->setPartialSuccessFlag(true);
		/* Enable flag to treat msg to failing over node as success */
		pWaveSendToClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true);
		/* Send the message to all nodes in cluster */
        sendToWaveCluster (pWaveSendToClusterContext);

        return;

    }

  void CdpGlobalConfWorker::updateCdpGlobalMO (PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
      LLDP_PLUG_DBG_TRACE();
      ResourceId status = WAVE_MESSAGE_ERROR;
      CdpGlobalConfManagedObject *m = NULL;
      CdpGlobalConfSetMessage *pCdpGlobalConfSetMessage = 
          dynamic_cast<CdpGlobalConfSetMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
      WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CdpGlobalConfManagedObject::getClassName());
      vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
      UI32 cmdcode = pCdpGlobalConfSetMessage->getCmdcode();
      
      if (NULL != pResults) {
          UI32 nResults = pResults->size ();
          if (nResults == 0) {
              LLDP_PLUG_DBG(string("Not Found MO"));
          } else if (nResults == 1) {
              m = dynamic_cast<CdpGlobalConfManagedObject*>((*pResults)[0]);
              prismAssert (NULL != m, __FILE__, __LINE__);
              LLDP_PLUG_DBG(string("Found MO: Update"));
              updateWaveManagedObject (m);
              status = WAVE_MESSAGE_SUCCESS;
          } else {
              LLDP_PLUG_DBG(string("Multiple MO: Count ") + nResults);
          }
      }
      
      if (status == WAVE_MESSAGE_SUCCESS) {
          if (IS_CMDCODE_SET(cmdcode, CDP_GLOBAL_CONF_PROTO_STATUS)) {
              m->setProtoEnable (pCdpGlobalConfSetMessage->getProtoEnable());
          }
      }
      
      if (pResults) {
          pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
          delete pResults;
      }
      
      pPrismLinearSequencerContext->executeNextStep(status);
      
      return;
  }
  
    void  CdpGlobalConfWorker::CdpGlobalConfSetMessageHandler(CdpGlobalConfSetMessage *pCdpGlobalConfSetMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			// Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::updateCdpGlobalConf),
			reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::prismLinearSequencerStartTransactionStep),
			// Your configuration change code goes here
			reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::updateCdpGlobalMO),
            reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&CdpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pCdpGlobalConfSetMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll ();
        pPrismLinearSequencerContext->start ();
    }
    
	void CdpGlobalConfWorker::install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases)
    {
		LLDP_PLUG_DBG_TRACE();
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_FIRST_TIME_BOOT) && (pWaveAsynchronousContextForBootPhases->getBootReason() != WAVE_BOOT_PERSISTENT_WITH_DEFAULT_BOOT)) {
            pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
            pWaveAsynchronousContextForBootPhases->callback ();
            return;
        }

		CdpGlobalConfManagedObject *m;

        startTransaction();

        m = new CdpGlobalConfManagedObject(dynamic_cast<LldpGlobalObjectManager *>(getPWaveObjectManager()));
        m->setProtoEnable (CDP_DEFAULT_PROTOCOL_STATUS);

        commitTransaction ();
        
		delete m;
    
        pWaveAsynchronousContextForBootPhases->setCompletionStatus (status);
        pWaveAsynchronousContextForBootPhases->callback ();
    }

}
