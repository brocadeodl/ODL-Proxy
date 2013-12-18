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
 *   Author : pbalacha                                                     *
 **************************************************************************/
#include "Utils/Capabilities.h"
#include "Utils/DceClusterUtils.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "DcmResourceIdEnums.h"
#include "Qos/Local/QosLocalShowMessage.h"
#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosLocalWorker.h"
#include "Qos/Local/QosCreateCeeRemapSPMessage.h"
#include "Qos/Local/QosCreateCeeMapSPMessage.h"
#include "Qos/Local/QosDeleteCeeMapSPMessage.h"
#include "Qos/Local/QosCreatePriorityGroupSPMessage.h"
#include "Qos/Local/QosDeletePriorityGroupSPMessage.h"
#include "Qos/Local/QosCreatePriorityMapSPMessage.h"
#include "Qos/Local/QosDeletePriorityMapSPMessage.h"
#include "Qos/Local/QosCreateQosQueueSchedulerSPMessage.h"
#include "Qos/Local/QosDeleteQosQueueSchedulerSPMessage.h"
#include "Qos/Local/QosCreateQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosDeleteQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosCreateCosToCosMutationMapMessage.h"
#include "Qos/Local/QosDeleteCosToCosMutationMapSPMessage.h"
#include "Qos/Local/QosCreateCosToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosDeleteCosToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosCreateQosRcvQueueMulticastSettingSPMessage.h"
#include "Qos/Local/QosDeleteQosRcvQueueMulticastSettingSPMessage.h"
/* NagP: New */
#include "Qos/Local/QosCreateDscpToDscpMutationMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpToDscpMutationMapSPMessage.h"
#include "Qos/Local/QosCreateDscpToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosCreateDscpToCosMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpToCosMapSPMessage.h"
#include "Qos/Local/QosCreateDscpMarkListSPMessage.h"
#include "Qos/Local/QosDeleteDscpMarkListSPMessage.h"
#include "Qos/Local/QosCreateRedProfileSPMessage.h"
#include "Qos/Local/QosDeleteRedProfileSPMessage.h"
/* NagP: New */
#include "Qos/Local/QosLocalNasMessage.h"
#include "Qos/Local/QosLocalNasShowMessage.h"
#include "Qos/Common/QosPluginDebug.h"
#include "ClientInterface/Ssm/QosClientCreateCeeRemapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCeeMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityGroupMessage.h"
#include "ClientInterface/Ssm/QosClientCreatePriorityMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCeeMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeletePriorityGroupMessage.h"
#include "ClientInterface/Ssm/QosClientDeletePriorityMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCosToCosMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateCosToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteCosToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosRcvQueueMulticastSettingMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosQueueMulticastSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateQosQueueSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteQosQueueSchedulerMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpMarkListMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpMarkListMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToCosMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToDscpMutationMapMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteDscpToTrafficClassMapMessage.h"
#include "ClientInterface/Ssm/QosClientCreateRedProfileMessage.h"
#include "ClientInterface/Ssm/QosClientDeleteRedProfileMessage.h"
#include "ClientInterface/Ssm/QosShowMessage.h"
#include "ClientInterface/Qos/QosClientNasMessage.h"
#include "ClientInterface/Qos/QosClientNasShowMessage.h"
#include "Qos/Local/QosTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Vrf/Local/VrfUtils.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"

namespace DcmNs
{

  QosLocalWorker::QosLocalWorker ( QosLocalObjectManager *pQosLocalObjectManager)
	: WaveWorker  (pQosLocalObjectManager)
	{
	  addOperationMap (QOSCREATECEEMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateCeeMapSPMessageHandler));
	  addOperationMap (QOSDELETECEEMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteCeeMapSPMessageHandler));
      addOperationMap (QOSCREATECEEREMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateCeeRemapSPMessageHandler));
	  addOperationMap (QOSCREATEPRIORITYGROUPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreatePriorityGroupSPMessageHandler));
	  addOperationMap (QOSDELETEPRIORITYGROUPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeletePriorityGroupSPMessageHandler));
	  addOperationMap (QOSCREATEPRIORITYMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreatePriorityMapSPMessageHandler));
	  addOperationMap (QOSDELETEPRIORITYMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeletePriorityMapSPMessageHandler));
	  addOperationMap (QOSCREATEQOSQUEUESCHEDULERSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateQosQueueSchedulerSPMessageHandler));
	  addOperationMap (QOSDELETEQOSQUEUESCHEDULERSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteQosQueueSchedulerSPMessageHandler));
	  addOperationMap (QOSCREATEQOSQUEUEMULTICASTSCHEDULERSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateQosQueueMulticastSchedulerSPMessageHandler));
	  addOperationMap (QOSDELETEQOSQUEUEMULTICASTSCHEDULERSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteQosQueueMulticastSchedulerSPMessageHandler));
	  addOperationMap (QOSCREATECOSTOCOSMUTATIONMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateCosToCosMutationMapSPMessageHandler));
	  addOperationMap (QOSDELETECOSTOCOSMUTATIONMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteCosToCosMutationMapSPMessageHandler));
	  addOperationMap (QOSCREATECOSTOTRAFFICCLASSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateCosToTrafficClassMapSPMessageHandler));
	  addOperationMap (QOSDELETECOSTOTRAFFICCLASSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteCosToTrafficClassMapSPMessageHandler));
	  addOperationMap (QOSCREATEQOSRCVQUEUEMULTICASTSETTINGSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateQosRcvQueueMulticastSettingSPMessageHandler));
	  addOperationMap (QOSDELETEQOSRCVQUEUEMULTICASTSETTINGSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteQosRcvQueueMulticastSettingSPMessageHandler));
	  addOperationMap (QOSLOCALSHOW, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosLocalShowMessageHandler));
	  addOperationMap (QOSCREATEDSCPTODSCPMUTATIONMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateDscpToDscpMutationMapSPMessageHandler));
	  addOperationMap (QOSDELETEDSCPTODSCPMUTATIONMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteDscpToDscpMutationMapSPMessageHandler));
	  addOperationMap (QOSCREATEDSCPTOTRAFFICCLASSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateDscpToTrafficClassMapSPMessageHandler));
	  addOperationMap (QOSDELETEDSCPTOTRAFFICCLASSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteDscpToTrafficClassMapSPMessageHandler));
	  addOperationMap (QOSCREATEDSCPTOCOSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateDscpToCosMapSPMessageHandler));
	  addOperationMap (QOSDELETEDSCPTOCOSMAPSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteDscpToCosMapSPMessageHandler));
	  addOperationMap (QOSCREATEDSCPMARKLISTSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateDscpMarkListSPMessageHandler));
	  addOperationMap (QOSDELETEDSCPMARKLISTSP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteDscpMarkListSPMessageHandler));
	  addOperationMap (QOSCREATEREDPROFILESP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosCreateRedProfileSPMessageHandler));
	  addOperationMap (QOSDELETEREDPROFILESP, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosDeleteRedProfileSPMessageHandler));
	  addOperationMap (QOSNASCONFIG, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosNasMessageHandler));
	  addOperationMap (QOSNASSHOW, reinterpret_cast<PrismMessageHandler> (&QosLocalWorker::QosNasShowMessageHandler));
	}

  QosLocalWorker::~QosLocalWorker ()
  {
  }

  PrismMessage  *QosLocalWorker::createMessageInstance(const UI32 &operationCode)
  {
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{

      case QOSCREATECEEREMAPSP :
        pPrismMessage = new QosCreateCeeRemapSPMessage ();
		break;
	  case QOSCREATECEEMAPSP :
		pPrismMessage = new QosCreateCeeMapSPMessage ();
		break;
	  case QOSDELETECEEMAPSP :
		pPrismMessage = new QosDeleteCeeMapSPMessage ();
		break;
	  case QOSCREATEPRIORITYGROUPSP :
		pPrismMessage = new QosCreatePriorityGroupSPMessage ();
		break;
	  case QOSDELETEPRIORITYGROUPSP :
		pPrismMessage = new QosDeletePriorityGroupSPMessage ();
		break;
	  case QOSCREATEPRIORITYMAPSP :
		pPrismMessage = new QosCreatePriorityMapSPMessage ();
		break;
	  case QOSDELETEPRIORITYMAPSP :
		pPrismMessage = new QosDeletePriorityMapSPMessage ();
		break;
	  case QOSCREATEQOSQUEUESCHEDULERSP :
		pPrismMessage = new QosCreateQosQueueSchedulerSPMessage ();
		break;
	  case QOSDELETEQOSQUEUESCHEDULERSP :
		pPrismMessage = new QosDeleteQosQueueSchedulerSPMessage ();
		break;
	  case QOSCREATEQOSQUEUEMULTICASTSCHEDULERSP :
		pPrismMessage = new QosCreateQosQueueMulticastSchedulerSPMessage ();
		break;
	  case QOSDELETEQOSQUEUEMULTICASTSCHEDULERSP :
		pPrismMessage = new QosDeleteQosQueueMulticastSchedulerSPMessage ();
		break;
	  case QOSCREATECOSTOCOSMUTATIONMAPSP :
		pPrismMessage = new QosCreateCosToCosMutationMapSPMessage ();
		break;
	  case QOSDELETECOSTOCOSMUTATIONMAPSP :
		pPrismMessage = new QosDeleteCosToCosMutationMapSPMessage ();
		break;
	  case QOSCREATECOSTOTRAFFICCLASSMAPSP :
		pPrismMessage = new QosCreateCosToTrafficClassMapSPMessage ();
		break;
	  case QOSDELETECOSTOTRAFFICCLASSMAPSP :
		pPrismMessage = new QosDeleteCosToTrafficClassMapSPMessage ();
		break;
	  case QOSCREATEQOSRCVQUEUEMULTICASTSETTINGSP :
		pPrismMessage = new QosCreateQosRcvQueueMulticastSettingSPMessage ();
		break;
	  case QOSDELETEQOSRCVQUEUEMULTICASTSETTINGSP :
		pPrismMessage = new QosDeleteQosRcvQueueMulticastSettingSPMessage ();
		break;
	  case QOSLOCALSHOW:
		pPrismMessage = new QosLocalShowMessage();
		break;
	  case QOSCREATEDSCPTODSCPMUTATIONMAPSP :
			pPrismMessage = new QosCreateDscpToDscpMutationMapSPMessage ();
			break;
	  case QOSDELETEDSCPTODSCPMUTATIONMAPSP :
			pPrismMessage = new QosDeleteDscpToDscpMutationMapSPMessage ();
			break;
	  case QOSCREATEDSCPTOTRAFFICCLASSMAPSP :
			pPrismMessage = new QosCreateDscpToTrafficClassMapSPMessage ();
			break;
	  case QOSDELETEDSCPTOTRAFFICCLASSMAPSP :
			pPrismMessage = new QosDeleteDscpToTrafficClassMapSPMessage ();
			break;
	  case QOSCREATEDSCPTOCOSMAPSP :
			pPrismMessage = new QosCreateDscpToCosMapSPMessage ();
			break;
	  case QOSDELETEDSCPTOCOSMAPSP :
			pPrismMessage = new QosDeleteDscpToCosMapSPMessage ();
			break;
	  case QOSCREATEDSCPMARKLISTSP :
			pPrismMessage = new QosCreateDscpMarkListSPMessage ();
			break;
	  case QOSDELETEDSCPMARKLISTSP :
			pPrismMessage = new QosDeleteDscpMarkListSPMessage ();
			break;
	  case QOSCREATEREDPROFILESP :
			pPrismMessage = new QosCreateRedProfileSPMessage ();
			break;
	  case QOSDELETEREDPROFILESP :
			pPrismMessage = new QosDeleteRedProfileSPMessage ();
			break;
	  case QOSNASCONFIG:
            pPrismMessage = new QosLocalNasMessage ();
            break;
	  case QOSNASSHOW:
            pPrismMessage = new QosLocalNasShowMessage ();
            break;
	  default :
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
  }

  WaveManagedObject  *QosLocalWorker::createManagedObjectInstance(const string &managedClassName)
  {
	WaveManagedObject *pWaveManagedObject = NULL;

	return (pWaveManagedObject);
  }

  ResourceId QosLocalWorker::createCeeRemapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {    
    ResourceId status;
    QosCreateCeeRemapSPMessage *pQosCreateCeeRemapSPMessage = dynamic_cast<QosCreateCeeRemapSPMessage*>(pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientCreateCeeRemapMessage *m = new QosClientCreateCeeRemapMessage();
    m->setCeeMapName(pQosCreateCeeRemapSPMessage->getCeeMapName());
    m->setRemapPriority(pQosCreateCeeRemapSPMessage->getRemapPriority());
    m->setRemapFabricP(pQosCreateCeeRemapSPMessage->getRemapFabricP());
    if (FrameworkToolKit::isConfigReplayInProgress()) {
	    m->setConfigReplay(1);
    }
	else {
		m->setConfigReplay(0);
	}
    trace (TRACE_LEVEL_INFO, "Sending QoS CEE remap create message to qosd");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosCreateCeeRemapSPMessage, m, status);
    delete m;
    return (status);
  }            

  void  QosLocalWorker::QosCreateCeeRemapSPMessageHandler( QosCreateCeeRemapSPMessage *pQosCreateCeeRemapSPMessage)
  {
    // Validations and Queries should go here

    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
//    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
      // Your configuration change code goes here

//    reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
      // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createCeeRemapStep),

      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateCeeRemapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createCeeMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    ResourceId status;
    QosCreateCeeMapSPMessage *pQosCreateCeeMapSPMessage = dynamic_cast<QosCreateCeeMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientCreateCeeMapMessage *m = new QosClientCreateCeeMapMessage();
	m->setCeeMapName(pQosCreateCeeMapSPMessage->getCeeMapName());
	m->setPrecedence(pQosCreateCeeMapSPMessage->getPrecedence());
	m->setConfigReplay(pQosCreateCeeMapSPMessage->getConfigReplay());
	trace(TRACE_LEVEL_INFO, string("QosLocalWorker:: CEE isconfigReplay<") + pQosCreateCeeMapSPMessage->getConfigReplay()+ ">");
    trace (TRACE_LEVEL_INFO, "Sending QoS CEE Map create message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateCeeMapSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosCreateCeeMapSPMessageHandler( QosCreateCeeMapSPMessage *pQosCreateCeeMapSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createCeeMapStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateCeeMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createCosToCosMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosCreateCosToCosMutationMapSPMessage *pQosCreateCosToCosMutationMapSPMessage = dynamic_cast<QosCreateCosToCosMutationMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientCreateCosToCosMutationMapMessage *m = new QosClientCreateCosToCosMutationMapMessage();

	m->setCosToCosMutationMapName(pQosCreateCosToCosMutationMapSPMessage->getCosToCosMutationMapName());
	m->setCos0MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos0MutatedCosValue());
	m->setCos1MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos1MutatedCosValue());
	m->setCos2MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos2MutatedCosValue());
	m->setCos3MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos3MutatedCosValue());
	m->setCos4MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos4MutatedCosValue());
	m->setCos5MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos5MutatedCosValue());
	m->setCos6MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos6MutatedCosValue());
	m->setCos7MutatedCosValue(pQosCreateCosToCosMutationMapSPMessage->getCos7MutatedCosValue());

    trace (TRACE_LEVEL_INFO, string("Sending QoS CoS-Mutation map <") 
            + pQosCreateCosToCosMutationMapSPMessage->getCosToCosMutationMapName() + "> creation message to qosd");
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateCosToCosMutationMapSPMessage, m, status);
	delete m;
	return (status);
  }

  void QosLocalWorker::QosCreateCosToCosMutationMapSPMessageHandler( QosCreateCosToCosMutationMapSPMessage *pQosCreateCosToCosMutationMapSPMessage)
  {
	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createCosToCosMutationMapStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateCosToCosMutationMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createCosToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosCreateCosToTrafficClassMapSPMessage *pQosCreateCosToTrafficClassMapSPMessage = dynamic_cast<QosCreateCosToTrafficClassMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientCreateCosToTrafficClassMapMessage *m = new QosClientCreateCosToTrafficClassMapMessage();

	m->setCosToTrafficClassMapName(pQosCreateCosToTrafficClassMapSPMessage->getCosToTrafficClassMapName());
	m->setCos0MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos0MappedTrafficClassValue());
	m->setCos1MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos1MappedTrafficClassValue());
	m->setCos2MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos2MappedTrafficClassValue());
	m->setCos3MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos3MappedTrafficClassValue());
	m->setCos4MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos4MappedTrafficClassValue());
	m->setCos5MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos5MappedTrafficClassValue());
	m->setCos6MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos6MappedTrafficClassValue());
	m->setCos7MappedTrafficClassValue(pQosCreateCosToTrafficClassMapSPMessage->getCos7MappedTrafficClassValue());

    trace (TRACE_LEVEL_INFO, string("Sending QoS CoS-TrafficClass map <")
            + pQosCreateCosToTrafficClassMapSPMessage->getCosToTrafficClassMapName() + "> creation message to qosd");

	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateCosToTrafficClassMapSPMessage, m, status);
	delete m;
	return (status);
  }
  void  QosLocalWorker::QosCreateCosToTrafficClassMapSPMessageHandler( QosCreateCosToTrafficClassMapSPMessage *pQosCreateCosToTrafficClassMapSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createCosToTrafficClassMapStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateCosToTrafficClassMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createPriorityGroupStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext) {

    ResourceId status;
    QosCreatePriorityGroupSPMessage *pQosCreatePriorityGroupSPMessage = dynamic_cast<QosCreatePriorityGroupSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientCreatePriorityGroupMessage *m = new QosClientCreatePriorityGroupMessage();

    m->setCeeMapName(pQosCreatePriorityGroupSPMessage->getCeeMapName());
	m->setPgId(pQosCreatePriorityGroupSPMessage->getPgId());
	m->setWeight(pQosCreatePriorityGroupSPMessage->getWeight());
	m->setPfc(pQosCreatePriorityGroupSPMessage->getPfc());
    if (FrameworkToolKit::isConfigReplayInProgress())
        m->setConfigReplay(1);
    else
        m->setConfigReplay(0);

    trace (TRACE_LEVEL_INFO, "Sending QoS CEE PGID create message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosCreatePriorityGroupSPMessage, m, status);
    delete m;
    return (status);
  }

  void  QosLocalWorker::QosCreatePriorityGroupSPMessageHandler( QosCreatePriorityGroupSPMessage *pQosCreatePriorityGroupSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createPriorityGroupStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreatePriorityGroupSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createPriorityMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    ResourceId status;
    QosCreatePriorityMapSPMessage *pQosCreatePriorityMapSPMessage = dynamic_cast<QosCreatePriorityMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientCreatePriorityMapMessage *m = new QosClientCreatePriorityMapMessage();

    m->setCeeMapName(pQosCreatePriorityMapSPMessage->getCeeMapName());
    m->setCos0ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos0ToPriorityGrouMap());
    m->setCos1ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos1ToPriorityGrouMap());
    m->setCos2ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos2ToPriorityGrouMap());
    m->setCos3ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos3ToPriorityGrouMap());
    m->setCos4ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos4ToPriorityGrouMap());
    m->setCos5ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos5ToPriorityGrouMap());
    m->setCos6ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos6ToPriorityGrouMap());
    m->setCos7ToPriorityGrouMap(pQosCreatePriorityMapSPMessage->getCos7ToPriorityGrouMap());
    if (FrameworkToolKit::isConfigReplayInProgress())
        m->setConfigReplay(1);
    else
        m->setConfigReplay(0);

    trace (TRACE_LEVEL_INFO, "Sending QoS Cee P-T create message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosCreatePriorityMapSPMessage, m, status);
    delete m;
    return (status);
  }

  void  QosLocalWorker::QosCreatePriorityMapSPMessageHandler( QosCreatePriorityMapSPMessage *pQosCreatePriorityMapSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createPriorityMapStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreatePriorityMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createMCastSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	QosCreateQosQueueMulticastSchedulerSPMessage *pQosCreateQosQueueMulticastSchedulerSPMessage= dynamic_cast<QosCreateQosQueueMulticastSchedulerSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	ResourceId status;
	QosClientCreateQosQueueMulticastSchedulerMessage *m = new QosClientCreateQosQueueMulticastSchedulerMessage();

	m->setDwrrTrafficClass0(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass6());
	m->setDwrrTrafficClass7(pQosCreateQosQueueMulticastSchedulerSPMessage->getDwrrTrafficClass7());
	trace (TRACE_LEVEL_INFO, "Sending QoS Multicast Scheduler message to qosd.");
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateQosQueueMulticastSchedulerSPMessage, m, status);
	delete m;
	return (status);
  }


  void  QosLocalWorker::QosCreateQosQueueMulticastSchedulerSPMessageHandler( QosCreateQosQueueMulticastSchedulerSPMessage *pQosCreateQosQueueMulticastSchedulerSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createMCastSchedulerStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateQosQueueMulticastSchedulerSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::createSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	QosCreateQosQueueSchedulerSPMessage *pQosCreateQosQueueSchedulerSPMessage =
	  dynamic_cast<QosCreateQosQueueSchedulerSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	ResourceId status;
	QosClientCreateQosQueueSchedulerMessage *m = new QosClientCreateQosQueueSchedulerMessage();

	m->setStrictPriority(pQosCreateQosQueueSchedulerSPMessage->getStrictPriority());
	m->setDwrrTrafficClass0(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass6());
	m->setDwrrTrafficClass7(pQosCreateQosQueueSchedulerSPMessage->getDwrrTrafficClass7());

    trace (TRACE_LEVEL_INFO, "Sending QoS Queue Scheduler message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateQosQueueSchedulerSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosCreateQosQueueSchedulerSPMessageHandler( QosCreateQosQueueSchedulerSPMessage *pQosCreateQosQueueSchedulerSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createSchedulerStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateQosQueueSchedulerSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }
  ResourceId QosLocalWorker::createMcastSettingStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	if (!Capabilities::instance()->isCapabilityPresent(
			QOS_RCV_QUEUE_MULTICAST_CAPABILITY)) {
		return (WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM);
	}

	QosCreateQosRcvQueueMulticastSettingSPMessage *pQosCreateQosRcvQueueMulticastSettingSPMessage= dynamic_cast<QosCreateQosRcvQueueMulticastSettingSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	ResourceId status;
	QosClientCreateQosRcvQueueMulticastSettingMessage *m = new QosClientCreateQosRcvQueueMulticastSettingMessage();

	m->setRateLimit(pQosCreateQosRcvQueueMulticastSettingSPMessage->getRateLimit());
	m->setBurst(pQosCreateQosRcvQueueMulticastSettingSPMessage->getBurst());
	m->setThresholdTrafficClass0(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass0());
	m->setThresholdTrafficClass1(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass1());
	m->setThresholdTrafficClass2(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass2());
	m->setThresholdTrafficClass3(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass3());
	m->setThresholdTrafficClass4(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass4());
	m->setThresholdTrafficClass5(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass5());
	m->setThresholdTrafficClass6(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass6());
	m->setThresholdTrafficClass7(pQosCreateQosRcvQueueMulticastSettingSPMessage->getThresholdTrafficClass7());
	m->setOpCode(pQosCreateQosRcvQueueMulticastSettingSPMessage->getOpCode());

	trace (TRACE_LEVEL_INFO, "Sending QoS rcv-queue multicast message to qosd.");
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosCreateQosRcvQueueMulticastSettingSPMessage, m, status);
	delete m;
	return (status);
  }


  void  QosLocalWorker::QosCreateQosRcvQueueMulticastSettingSPMessageHandler( QosCreateQosRcvQueueMulticastSettingSPMessage *pQosCreateQosRcvQueueMulticastSettingSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createMcastSettingStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateQosRcvQueueMulticastSettingSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteCeeMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    ResourceId status;
    QosDeleteCeeMapSPMessage *pQosDeleteCeeMapSPMessage = dynamic_cast<QosDeleteCeeMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientDeleteCeeMapMessage *m = new QosClientDeleteCeeMapMessage();

    m->setCeeMapName(pQosDeleteCeeMapSPMessage->getCeeMapName());
    trace (TRACE_LEVEL_INFO, "Sending dlete QoS Cee Map message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosDeleteCeeMapSPMessage, m, status);
    delete m;
    return (status);
  }

  void  QosLocalWorker::QosDeleteCeeMapSPMessageHandler( QosDeleteCeeMapSPMessage *pQosDeleteCeeMapSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteCeeMapStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteCeeMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteCosToCosMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosDeleteCosToCosMutationMapSPMessage *pQosDeleteCosToCosMutationMapSPMessage = dynamic_cast<QosDeleteCosToCosMutationMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientDeleteCosToCosMutationMapMessage *m = new QosClientDeleteCosToCosMutationMapMessage();

	m->setCosToCosMutationMapName(pQosDeleteCosToCosMutationMapSPMessage->getCosToCosMutationMapName());

    trace (TRACE_LEVEL_INFO, string("Sending QoS CoS-Mutation map <")
            + pQosDeleteCosToCosMutationMapSPMessage->getCosToCosMutationMapName() + "> deletion message to qosd");

	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosDeleteCosToCosMutationMapSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosDeleteCosToCosMutationMapSPMessageHandler( QosDeleteCosToCosMutationMapSPMessage *pQosDeleteCosToCosMutationMapSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteCosToCosMutationMapStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteCosToCosMutationMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteCosToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosDeleteCosToTrafficClassMapSPMessage *pQosDeleteCosToTrafficClassMapSPMessage = dynamic_cast<QosDeleteCosToTrafficClassMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientDeleteCosToTrafficClassMapMessage *m = new QosClientDeleteCosToTrafficClassMapMessage();

	m->setCosToTrafficClassMapName(pQosDeleteCosToTrafficClassMapSPMessage->getCosToTrafficClassMapName());

    trace (TRACE_LEVEL_INFO, string("Sending QoS CoS-TrafficClass map <")
            + pQosDeleteCosToTrafficClassMapSPMessage->getCosToTrafficClassMapName() + "> deletion message to qosd");
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosDeleteCosToTrafficClassMapSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosDeleteCosToTrafficClassMapSPMessageHandler( QosDeleteCosToTrafficClassMapSPMessage *pQosDeleteCosToTrafficClassMapSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteCosToTrafficClassMapStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteCosToTrafficClassMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deletePriorityGroupStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    ResourceId status;
    QosDeletePriorityGroupSPMessage *pQosDeletePriorityGroupSPMessage = dynamic_cast<QosDeletePriorityGroupSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientDeletePriorityGroupMessage *m = new QosClientDeletePriorityGroupMessage();

    m->setCeeMapName(pQosDeletePriorityGroupSPMessage->getCeeMapName());
    m->setPgId(pQosDeletePriorityGroupSPMessage->getPgId());

    trace (TRACE_LEVEL_INFO, "Sending QoS CEE PGID delete message to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosDeletePriorityGroupSPMessage, m, status);
    delete m;
    return (status);
  }

  void  QosLocalWorker::QosDeletePriorityGroupSPMessageHandler( QosDeletePriorityGroupSPMessage *pQosDeletePriorityGroupSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deletePriorityGroupStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeletePriorityGroupSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deletePriorityMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
    ResourceId status;
    QosDeletePriorityMapSPMessage *pQosDeletePriorityMapSPMessage = dynamic_cast<QosDeletePriorityMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    QosClientDeletePriorityMapMessage *m = new QosClientDeletePriorityMapMessage();

    m->setCeeMapName(pQosDeletePriorityMapSPMessage->getCeeMapName());

    trace (TRACE_LEVEL_INFO, "Sending QoS Cee P-T delete to qosd.");
    status = sendSynchronouslyToWaveClient ("qos",m);
    status = errorCode(pQosDeletePriorityMapSPMessage, m, status);
    delete m;
    return (status);
  }

  void  QosLocalWorker::QosDeletePriorityMapSPMessageHandler( QosDeletePriorityMapSPMessage *pQosDeletePriorityMapSPMessage)
  {
	// Validations and Queries should go here

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
	  // Your configuration change code goes here

//	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
	  // Programming Protocol Daemons goes here
      reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deletePriorityMapStep),

	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeletePriorityMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteMCastSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosDeleteQosQueueMulticastSchedulerSPMessage *pQosDeleteQosQueueMulticastSchedulerSPMessage = dynamic_cast<QosDeleteQosQueueMulticastSchedulerSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientDeleteQosQueueMulticastSchedulerMessage *m = new QosClientDeleteQosQueueMulticastSchedulerMessage();

	trace (TRACE_LEVEL_INFO, string("Sending delete QoS Multicast Scheduler message to qosd.")); 
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosDeleteQosQueueMulticastSchedulerSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosDeleteQosQueueMulticastSchedulerSPMessageHandler( QosDeleteQosQueueMulticastSchedulerSPMessage *pQosDeleteQosQueueMulticastSchedulerSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteMCastSchedulerStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteQosQueueMulticastSchedulerSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteSchedulerStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosDeleteQosQueueSchedulerSPMessage *pQosDeleteQosQueueSchedulerSPMessage = dynamic_cast<QosDeleteQosQueueSchedulerSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientDeleteQosQueueSchedulerMessage *m = new QosClientDeleteQosQueueSchedulerMessage();

	trace (TRACE_LEVEL_INFO, string("Sending QoS Queue Scheduler delete message to qosd.")); 
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosDeleteQosQueueSchedulerSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosDeleteQosQueueSchedulerSPMessageHandler( QosDeleteQosQueueSchedulerSPMessage *pQosDeleteQosQueueSchedulerSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteSchedulerStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteQosQueueSchedulerSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }

  ResourceId QosLocalWorker::deleteMCastSettingStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	ResourceId status;
	QosDeleteQosRcvQueueMulticastSettingSPMessage *pQosDeleteQosRcvQueueMulticastSettingSPMessage = 
		dynamic_cast<QosDeleteQosRcvQueueMulticastSettingSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	QosClientDeleteQosRcvQueueMulticastSettingMessage *m = new QosClientDeleteQosRcvQueueMulticastSettingMessage();

	m->setOpCode(pQosDeleteQosRcvQueueMulticastSettingSPMessage->getOpCode());
	trace (TRACE_LEVEL_DEBUG, string("Sending QoS multicast message to qosd:") + pQosDeleteQosRcvQueueMulticastSettingSPMessage->getOpCode());
	status = sendSynchronouslyToWaveClient ("qos",m);
	status = errorCode(pQosDeleteQosRcvQueueMulticastSettingSPMessage, m, status);
	delete m;
	return (status);
  }

  void  QosLocalWorker::QosDeleteQosRcvQueueMulticastSettingSPMessageHandler( QosDeleteQosRcvQueueMulticastSettingSPMessage *pQosDeleteQosRcvQueueMulticastSettingSPMessage)
  {

	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteMCastSettingStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
	  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
	};

	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteQosRcvQueueMulticastSettingSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismSynchronousLinearSequencerContext->execute ();
  }


  ResourceId QosLocalWorker::errorCode(PrismMessage* pMsg, DcmManagementInterfaceMessage* clientMsg, ResourceId status)
    {
		trace (TRACE_LEVEL_DEBUG, string("QosLocalWorker::errorCode :- Entering  with status: ") + status);

        pMsg->setCompletionStatus(status);

        if (WAVE_MESSAGE_SUCCESS == status) {
            if (WAVE_MESSAGE_SUCCESS != pMsg->getCompletionStatus()) {
                status = pMsg->getCompletionStatus();
            } else {
                status = clientMsg->getClientStatus();
				trace (TRACE_LEVEL_INFO, string("QosLocalWorker::errorCode :- clientMsg->getClientStatus() returned from qosd:<") + status + ">");
                pMsg->setCompletionStatus(status);
            }
        }
        return status;
    }

  void QosLocalWorker::QosLocalShowSendToClusterCallback (WaveSendToClusterContext *ctx_p) {

      PrismLinearSequencerContext             *seq_ctx_p = NULL;
      QosLocalShowMessage                     *req_msg_p = NULL,
                                              *resp_msg_p = NULL,
                                              *sw_resp_p = NULL;

      trace(TRACE_LEVEL_DEBUG,string("Entering QosLocalShowSendToClusterCallback") );

      ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);

      seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

      req_msg_p = dynamic_cast<QosLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());

      resp_msg_p =  dynamic_cast<QosLocalShowMessage *> (ctx_p->getPPrismMessageForPhase1());

      QOS_PLUG_ASSERT_PTR(req_msg_p);
      QOS_PLUG_ASSERT_PTR(resp_msg_p);

      if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {

          vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
          UI32    i = 0;

          for (i = 0; i < locations.size(); i++)
          {
              ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);

              if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {

                  trace(TRACE_LEVEL_DEBUG, string("Response processing: The location: <") + locations[i] + string(">"));
                  sw_resp_p = dynamic_cast<QosLocalShowMessage *> (ctx_p->getResultingMessageForPhase1(locations[i]));
                  QOS_PLUG_ASSERT_PTR(sw_resp_p);
                  req_msg_p->copyAllBuffers(*sw_resp_p);
              }
              else
              {
                  trace(TRACE_LEVEL_ERROR,string("Response processing: The location: ") + locations[i] + string("returned error: <")  + FrameworkToolKit::localize(statusPerLocationId) + ">");
              }
          }

      } else {
          trace(TRACE_LEVEL_ERROR,string("QosLocalShowSendToClusterCallback : ") + string("returned error:<")  + FrameworkToolKit::localize(sendToClusterCompletionStatus) + ">");
      }

      trace(TRACE_LEVEL_DEBUG,string("MC return status: ") + sendToClusterCompletionStatus);
      seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
      delete resp_msg_p;
      delete ctx_p;

      return;
  }
    
    void QosLocalWorker::sendShowCommandToCluster (PrismLinearSequencerContext *seq_ctx_p)
    {
        QosLocalShowMessage        *msg_p = NULL;
        QosLocalShowMessage        *loc_msg_p = NULL;
        WaveSendToClusterContext   *send_ctx_p = NULL;
	bool isGwRequest = false;
        UI32                        locationId = 0;
        SI32                        mappedId = 0;
        vector<UI32>                locations;
        bool                        sendToLocal = false;

        msg_p = dynamic_cast<QosLocalShowMessage *> (seq_ctx_p->getPPrismMessage ());
        QOS_PLUG_ASSERT_PTR(msg_p);

        loc_msg_p = new QosLocalShowMessage(msg_p->getIfName(),
			msg_p->getPoNum(), msg_p->getCmdCode(), isGwRequest,
			msg_p->getLineCard(), msg_p->getPortSet(),
			msg_p->getProfileId(),msg_p->getChipIndex(), msg_p->getPortIndex(),
			msg_p->getNoOfRecords());
        QOS_PLUG_ASSERT_PTR(loc_msg_p);

        trace(TRACE_LEVEL_DEBUG,
			string("Sending QoS show command to the cluster") + string(" CmdCode <") + msg_p->getCmdCode() + string (">ifName <") + msg_p->getIfName() + msg_p->getPoNum() + "> ChipIndex" + msg_p->getChipIndex() + "> PortIndex<" + msg_p->getPortIndex());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&QosLocalWorker::QosLocalShowSendToClusterCallback), seq_ctx_p);
        QOS_PLUG_ASSERT_PTR(send_ctx_p);
        if (!send_ctx_p)
        {
        	trace(TRACE_LEVEL_ERROR,string("The send_ctx_p is NULL in QosLocalShowSendToClusterCallback."));
            delete send_ctx_p;
            delete loc_msg_p;
            seq_ctx_p->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

        if((msg_p->getIfName())  == "po")
        {
            if(PORT_NOT_PART_OF_ANY_INTERFACE == NsmUtils::setLocationIdsForPoSubModeCmds(msg_p->getPoNum(), send_ctx_p))
            {
                trace(TRACE_LEVEL_INFO, string(" QosLocalWorker::sendShowCommandToCluster port-channel <") + msg_p->getPoNum() 
                    + "> doesnt have any members.");
                delete send_ctx_p;
                delete loc_msg_p;
                seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
                return;
            }
            send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        } else if (((msg_p->getIfName()) == "line")
                       || (msg_p->getIfName() == "c2cmap")
                       || (msg_p->getIfName() == "c2tmap")
                       || (msg_p->getIfName() == "ceemap")
                       || (msg_p->getIfName() == "d2dmap")
                       || (msg_p->getIfName() == "d2tmap")
                       || (msg_p->getIfName() == "d2cmap")) {

               send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
         }
        else if ((QOSSHOWQOSREDPROFILES == msg_p->getCmdCode())
			|| (QOSGETSHOWQOSREDPROFILES == msg_p->getCmdCode())) {
		send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        }
        else
        {
	    	send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

            if (NsmUtils::isValidTwoTuple(msg_p->getPoNum()))  
            {
                sendToLocal = true;
            } else if (NsmUtils::isValidThreeTuple(msg_p->getPoNum())) 
            {
                /* Send  to the targetted LocationId pointed to by the mappedId */
                sendToLocal = false;
                mappedId = NsmUtils::getMappedIdFromIfName(msg_p->getPoNum());
                locationId = DcmToolKit::getLocationIdFromMappedId(mappedId);
                if (locationId == 0) {
                    delete send_ctx_p;
                    delete loc_msg_p;
                    seq_ctx_p->executeNextStep(SSM_INTERFACE_DOESNOT_EXIST);
                    return;
                }
                locations.push_back(locationId);
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
			else if ((msg_p->getIfName()) == "all")
			{
				locations.push_back(msg_p->getLocationId());
				send_ctx_p->setLocationsToSendToForPhase1(locations);
			}
            else
            {
                sendToLocal = false;
            }

            if (sendToLocal)
       	    {
                locationId = NsmUtils::getLocalLocationId();
                if (locationId)
                {
                    locations.push_back(locationId);
                }
                send_ctx_p->setLocationsToSendToForPhase1(locations);
            }
        }

        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
        sendToWaveCluster(send_ctx_p);
        return;
    }
    
    ResourceId QosLocalWorker::sendShowToQosDaemon (PrismSynchronousLinearSequencerContext *ctx_p)
    {
        QosLocalShowMessage *msg_p = dynamic_cast<QosLocalShowMessage *> (ctx_p->getPPrismMessage ());
        QosShowMessage *cl_msg_p = NULL;
        ResourceId      status;
        SI32            mappedId = 0;
        string          ifName = "";

        mappedId = DcmToolKit::getLocalMappedId();

		/*The commands:
			 "show qos rcv-queue multicast interface te/gi/fo/po  x/y"
			 "show qos rcv-queue interface te/gi/fo/po  x/y"
			 should be disabled on the mercury platform.*/
		switch (msg_p->getCmdCode()){
            
            case QOSSHOWQOSRCVQUEUEINTERFACE: {
                if (!Capabilities::instance()->isCapabilityPresent(QOS_SHOW_RCV_QUEUE_CAPABILITY)) {
                    return WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM;
                }
            break;
            }

            case QOSSHOWQOSRCVQUEUEMULTICASTINTERFACE: {
                if (!Capabilities::instance()->isCapabilityPresent(QOS_SHOW_RCV_QUEUE_MULTICAST_CAPABILITY)) {
                    return WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM;
                }
            break;
            }

            case QOSSHOW_DROP_REASON_INTERFACE:
            case QOSSHOW_QOS_VOQ_INTERFACE:
            case QOSSHOW_QOS_CHIP_INFO:
            case QOSSHOW_QOS_INGQUEUE_INTERFACE:
            case QOSSHOW_QOS_INGBUFF_INTERFACE:
            case QOSSHOW_QOS_PLC_INGQUEUE_INTERFACE:
            case QOSSHOW_QOS_PLC_INGBUFF_INTERFACE:
            case QOSSHOW_QOS_ING_TMCBUFF_INTERFACE:
            case QOSSHOW_QOS_ING_TMCQUEUE_INTERFACE:
            case QOSSHOW_QOS_EGRQUEUE_INTERFACE:
            case QOSSHOW_QOS_EGRBUFF_INTERFACE:
            case QOSSHOW_QOS_PLC_EGRQUEUE_INTERFACE:
            case QOSSHOW_QOS_PLC_EGRBUFF_INTERFACE:
            case QOSSHOW_QOS_EGR_TMCBUFF_INTERFACE:
            case QOSSHOW_QOS_TMC_EGRQUEUE_INTERFACE:
            case QOSHSOW_QOS_TMC_EGRBUFF_INTERFACE:
            case QOSSHOW_QOS_TMC_INGQUEUE_INTERFACE:
            case QOSSHOW_QOS_TMC_INGBUFF_INTERFACE: {
                if (!(Capabilities::instance()->isCapabilityPresent(EXTENDED_QOS_STATS_SHOW))) {
                    return WRC_SSM_QOS_EXTENDED_QOS_STATS_NOT_SUPPORTED_ON_THIS_PLATORM;
                }
            break;
            }
        }


        if (mappedId == (-1))
        { 
            mappedId = 0;
        }

        if (NsmUtils::isValidThreeTuple(msg_p->getPoNum()))
        {
			NsmUtils::getTwoTupleIfName(msg_p->getPoNum(), ifName);
            trace(TRACE_LEVEL_DEBUG,string("The three tuple if is<")+ msg_p->getPoNum() + "> and the two tuple if is <" + ifName + ">");
        }
        else
        {
            ifName = msg_p->getPoNum();
            trace(TRACE_LEVEL_DEBUG, string("The two tuple if is <")+ ifName + ">");
        }
        cl_msg_p = new QosShowMessage(msg_p->getIfName(), ifName,
			msg_p->getCmdCode(), mappedId, msg_p->getLineCard(),
			msg_p->getPortSet(), msg_p->getChipIndex(), msg_p->getPortIndex(),
			msg_p->getNoOfRecords());

        status = sendSynchronouslyToWaveClient("qos", cl_msg_p);

        trace(TRACE_LEVEL_DEBUG, string("Sending qos show command to the cluster MsgId <")  + cl_msg_p->getMessageId()  
            + "> CmdCode <" + cl_msg_p->getCmdCode() + "> ifName <"  + cl_msg_p->getIfName() + " " + cl_msg_p->getPoNum() 
            + "> mappedId <" + cl_msg_p->getMappedId() + ">");

        if (status == WAVE_MESSAGE_SUCCESS)
       	{
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
            {
                trace(TRACE_LEVEL_ERROR,string("Response from qosd for the MsgId: <") + cl_msg_p->getMessageId() 
                    + "> , the completionstatus is <" +  FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()) + ">");
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return msg_p->getCompletionStatus();
            }

            if (cl_msg_p->getClientStatus() != 0)
            { 
                trace(TRACE_LEVEL_ERROR, string("Response from qosd for the MsgId: <") + cl_msg_p->getMessageId() 
                    + "> , the clientstatus is <" + FrameworkToolKit::localize(cl_msg_p->getClientStatus()) +">"); 
                msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				if(cl_msg_p){
					delete cl_msg_p;
				}
				return msg_p->getCompletionStatus();
            }
        }
        
        if (status != WAVE_MESSAGE_SUCCESS)
       	{
            trace(TRACE_LEVEL_ERROR, string("Client Message sending failed for MsgId: <") + cl_msg_p->getMessageId() + 
                    " status<" +  FrameworkToolKit::localize(status) + ">");
            //bpatel
			if(cl_msg_p){
				delete cl_msg_p;
			}
			return status;
        }

        trace(TRACE_LEVEL_DEBUG, string("Client Message processing succesful for MsgId <") + cl_msg_p->getMessageId() + ">");

        msg_p->copyAllBuffers(*cl_msg_p);
        //bpatel
		status = cl_msg_p->getCompletionStatus();
		delete cl_msg_p;
		return status;
    }

    void  
    QosLocalWorker::QosLocalShowMessageHandler (QosLocalShowMessage *pQosLocalShowMessage)
    {
        if (pQosLocalShowMessage->getIsGwRequest())
       	{
        	PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::sendShowCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::prismLinearSequencerFailedStep)
            };
        	PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pQosLocalShowMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
        	//pPrismLinearSequencerContext->holdAll();
        	pPrismLinearSequencerContext->start ();
            return;
        }
       	else
       	{ 
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::sendShowToQosDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pQosLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();
        }
    }

    void  QosLocalWorker::QosCreateDscpMarkListSPMessageHandler( QosCreateDscpMarkListSPMessage *pQosCreateDscpMarkListSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createDscpMarkListStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateDscpMarkListSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosCreateDscpToCosMapSPMessageHandler( QosCreateDscpToCosMapSPMessage *pQosCreateDscpToCosMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createDscpToCosMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateDscpToCosMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosCreateDscpToDscpMutationMapSPMessageHandler( QosCreateDscpToDscpMutationMapSPMessage *pQosCreateDscpToDscpMutationMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createDscpToDscpMutationMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateDscpToDscpMutationMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosCreateDscpToTrafficClassMapSPMessageHandler( QosCreateDscpToTrafficClassMapSPMessage *pQosCreateDscpToTrafficClassMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createDscpToTrafficClassMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateDscpToTrafficClassMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosCreateRedProfileSPMessageHandler( QosCreateRedProfileSPMessage *pQosCreateRedProfileSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::createRedProfileStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosCreateRedProfileSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosDeleteDscpMarkListSPMessageHandler( QosDeleteDscpMarkListSPMessage *pQosDeleteDscpMarkListSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteDscpMarkListStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteDscpMarkListSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosDeleteDscpToCosMapSPMessageHandler( QosDeleteDscpToCosMapSPMessage *pQosDeleteDscpToCosMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteDscpToCosMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteDscpToCosMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosDeleteDscpToDscpMutationMapSPMessageHandler( QosDeleteDscpToDscpMutationMapSPMessage *pQosDeleteDscpToDscpMutationMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteDscpToDscpMutationMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteDscpToDscpMutationMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosDeleteDscpToTrafficClassMapSPMessageHandler( QosDeleteDscpToTrafficClassMapSPMessage *pQosDeleteDscpToTrafficClassMapSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteDscpToTrafficClassMapStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteDscpToTrafficClassMapSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  QosLocalWorker::QosDeleteRedProfileSPMessageHandler( QosDeleteRedProfileSPMessage *pQosDeleteRedProfileSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::deleteRedProfileStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosDeleteRedProfileSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId QosLocalWorker::createDscpToDscpMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosCreateDscpToDscpMutationMapSPMessage *pQosCreateDscpToDscpMutationMapSPMessage = dynamic_cast<QosCreateDscpToDscpMutationMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientCreateDscpToDscpMutationMapMessage *m = new QosClientCreateDscpToDscpMutationMapMessage();

		m->setDscpMutationMapName(pQosCreateDscpToDscpMutationMapSPMessage->getDscpMutationMapName());
		trace (TRACE_LEVEL_DEVEL, "createDscpToDscpMutationMapStep: sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosCreateDscpToDscpMutationMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::deleteDscpToDscpMutationMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosDeleteDscpToDscpMutationMapSPMessage *pQosDeleteDscpToDscpMutationMapSPMessage = dynamic_cast<QosDeleteDscpToDscpMutationMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientDeleteDscpToDscpMutationMapMessage *m = new QosClientDeleteDscpToDscpMutationMapMessage();

		m->setDscpMutationMapName(pQosDeleteDscpToDscpMutationMapSPMessage->getDscpMutationMapName());
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosDeleteDscpToDscpMutationMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::createDscpToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosCreateDscpToTrafficClassMapSPMessage *pQosCreateDscpToTrafficClassMapSPMessage = dynamic_cast<QosCreateDscpToTrafficClassMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientCreateDscpToTrafficClassMapMessage *m = new QosClientCreateDscpToTrafficClassMapMessage();

		m->setDscpTrafficClassMapName(pQosCreateDscpToTrafficClassMapSPMessage->getDscpTrafficClassMapName());
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosCreateDscpToTrafficClassMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::deleteDscpToTrafficClassMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosDeleteDscpToTrafficClassMapSPMessage *pQosDeleteDscpToTrafficClassMapSPMessage = dynamic_cast<QosDeleteDscpToTrafficClassMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientDeleteDscpToTrafficClassMapMessage *m = new QosClientDeleteDscpToTrafficClassMapMessage();

		m->setDscpTrafficClassMapName(pQosDeleteDscpToTrafficClassMapSPMessage->getDscpTrafficClassMapName());
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosDeleteDscpToTrafficClassMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::createDscpToCosMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosCreateDscpToCosMapSPMessage *pQosCreateDscpToCosMapSPMessage = dynamic_cast<QosCreateDscpToCosMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientCreateDscpToCosMapMessage *m = new QosClientCreateDscpToCosMapMessage();

		m->setDscpCosMapName(pQosCreateDscpToCosMapSPMessage->getDscpCosMapName());
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosCreateDscpToCosMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::deleteDscpToCosMapStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosDeleteDscpToCosMapSPMessage *pQosDeleteDscpToCosMapSPMessage = dynamic_cast<QosDeleteDscpToCosMapSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientDeleteDscpToCosMapMessage *m = new QosClientDeleteDscpToCosMapMessage();

		m->setDscpCosMapName(pQosDeleteDscpToCosMapSPMessage->getDscpCosMapName());
		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosDeleteDscpToCosMapSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::createDscpMarkListStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosCreateDscpMarkListSPMessage *pQosCreateDscpMarkListSPMessage = dynamic_cast<QosCreateDscpMarkListSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientCreateDscpMarkListMessage *m = new QosClientCreateDscpMarkListMessage();

		m->setMarkString(pQosCreateDscpMarkListSPMessage->getMarkList());
		m->setDscpMapName(pQosCreateDscpMarkListSPMessage->getDscpMapName());
		m->setDscpMarkToValue(pQosCreateDscpMarkListSPMessage->getToValue());
		m->setQosMapType(pQosCreateDscpMarkListSPMessage->getQosMapType());

		trace (TRACE_LEVEL_DEBUG, string("Lwrk: input markList") + pQosCreateDscpMarkListSPMessage->getMarkList() + string("MapName: ") + pQosCreateDscpMarkListSPMessage->getDscpMapName() + string("toValue: ") + pQosCreateDscpMarkListSPMessage->getToValue() + string(" mapType: ") + pQosCreateDscpMarkListSPMessage->getQosMapType());
		trace (TRACE_LEVEL_DEBUG, string("LwrkMsg: input markList") + m->getMarkString() + string("MapName: ") + m->getDscpMapName() + string("toValue: ") + m->getDscpMarkToValue() + string(" mapType: ") + m->getQosMapType());

		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosCreateDscpMarkListSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::deleteDscpMarkListStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosDeleteDscpMarkListSPMessage *pQosDeleteDscpMarkListSPMessage = dynamic_cast<QosDeleteDscpMarkListSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientDeleteDscpMarkListMessage *m = new QosClientDeleteDscpMarkListMessage();

		/* A delete markList message never comes to client or at least the client has nothing to do with it
		 * because mark adjustment would already have been done. For now, the backend daemon ignores this
		 * message.
		 */
		m->setMarkString(pQosDeleteDscpMarkListSPMessage->getMarkList());
		m->setDscpMapName(pQosDeleteDscpMarkListSPMessage->getDscpMapName());

		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosDeleteDscpMarkListSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::createRedProfileStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosCreateRedProfileSPMessage *pQosCreateRedProfileSPMessage = dynamic_cast<QosCreateRedProfileSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientCreateRedProfileMessage *m = new QosClientCreateRedProfileMessage();

		m->setProfileId(pQosCreateRedProfileSPMessage->getProfileId());
		m->setMinThreshold(pQosCreateRedProfileSPMessage->getMinThreshold());
		m->setMaxThreshold(pQosCreateRedProfileSPMessage->getMaxThreshold());
		m->setDropProbability(pQosCreateRedProfileSPMessage->getDropProbability());

		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosCreateRedProfileSPMessage, m, status);
		delete m;
		return (status);
	}
	
	ResourceId QosLocalWorker::deleteRedProfileStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		QosDeleteRedProfileSPMessage *pQosDeleteRedProfileSPMessage = dynamic_cast<QosDeleteRedProfileSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		QosClientDeleteRedProfileMessage *m = new QosClientDeleteRedProfileMessage();

		m->setProfileId(pQosDeleteRedProfileSPMessage->getProfileId());

		trace (TRACE_LEVEL_DEVEL, "sending synchronously to qos client");
		status = sendSynchronouslyToWaveClient ("qos",m);
		status = errorCode(pQosDeleteRedProfileSPMessage, m, status);
		delete m;
		return (status);
	}

	 void  QosLocalWorker::QosNasMessageHandler ( QosLocalNasMessage *pQosLocalNasMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::NasValidationStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::NasUpdateBackendStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pQosLocalNasMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId QosLocalWorker::NasValidationStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
	  	return (WAVE_MESSAGE_SUCCESS);
    }
    ResourceId QosLocalWorker::NasUpdateBackendStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        QosLocalNasMessage *pQosLocalNasMessage = dynamic_cast<QosLocalNasMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        QosClientNasMessage *pQosClientNasMessage = new QosClientNasMessage();
		UI32 cmdCode = pQosLocalNasMessage->getCmdCode();
		if ((cmdCode == QOS_NAS_DCMD_SERVER_IP_VRF)|| (cmdCode == QOS_NAS_DCMD_SERVER_IP_VLAN)) {
			cmdCode = QOS_NAS_DCMD_SERVER_IP;
		}

		pQosClientNasMessage->setOpCode(pQosLocalNasMessage->getOpCode());
        pQosClientNasMessage->setCmdCode(cmdCode);
        pQosClientNasMessage->setCos(pQosLocalNasMessage->getCos());
        pQosClientNasMessage->setDscp(pQosLocalNasMessage->getDscp());
        pQosClientNasMessage->setServerIpAddressPrefix(pQosLocalNasMessage->getServerIpAddressPrefix());
        pQosClientNasMessage->setVrfName(pQosLocalNasMessage->getVrfName());
        pQosClientNasMessage->setVlanNumber(pQosLocalNasMessage->getVlanNumber());

        trace (TRACE_LEVEL_DEBUG, "NasUpdateBackendStep::sending NAS config to qos client");
        trace (TRACE_LEVEL_DEBUG, string("opcode=") + pQosLocalNasMessage->getOpCode() + string("cmdCode=") + cmdCode);
        status = sendSynchronouslyToWaveClient ("qos", pQosClientNasMessage);
        status = errorCode(pQosLocalNasMessage, pQosClientNasMessage, status);
        delete pQosClientNasMessage;

        return (status);
    }
	void
    QosLocalWorker::QosNasShowMessageHandler (QosLocalNasShowMessage *pQosLocalNasShowMessage)
    {
        if (pQosLocalNasShowMessage->getIsGwRequest())
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::sendNasShowToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&QosLocalWorker::prismLinearSequencerFailedStep)
            };
            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pQosLocalNasShowMessage, this,
                sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        }
        else
        {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::sendNasShowToQosDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&QosLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext(pQosLocalNasShowMessage, this,
                sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();
        }
    }
	void QosLocalWorker::sendNasShowToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        WaveSendToClusterContext   *send_ctx_p = NULL;
		QosLocalNasShowMessage *msg_p;
		bool isGwRequest = false;

        msg_p = dynamic_cast<QosLocalNasShowMessage*> (seq_ctx_p->getPPrismMessage ());
        QOS_PLUG_ASSERT_PTR(msg_p);
        QosLocalNasShowMessage *pQosLocalNasShowMessage = new QosLocalNasShowMessage();
        pQosLocalNasShowMessage->setIsGwRequest(isGwRequest);
        pQosLocalNasShowMessage->setOpCode(msg_p->getOpCode());
        pQosLocalNasShowMessage->setCmdCode(msg_p->getCmdCode());
        pQosLocalNasShowMessage->setCos(msg_p->getCos());
        pQosLocalNasShowMessage->setDscp(msg_p->getDscp());
        pQosLocalNasShowMessage->setServerIpAddressPrefix(msg_p->getServerIpAddressPrefix());
        pQosLocalNasShowMessage->setVrfName(msg_p->getVrfName());
        pQosLocalNasShowMessage->setVlanNumber(msg_p->getVlanNumber());
        send_ctx_p = new WaveSendToClusterContext(this,
			reinterpret_cast<PrismAsynchronousCallback> (&QosLocalWorker::QosLocalNasShowSendToClusterCallback), seq_ctx_p);
        send_ctx_p->setPPrismMessageForPhase1(pQosLocalNasShowMessage);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag(true);
        trace (TRACE_LEVEL_DEVEL, string("sending NAS show to all nodes"));
        sendToWaveCluster(send_ctx_p);
        return;
    }
    ResourceId QosLocalWorker::sendNasShowToQosDaemon(PrismSynchronousLinearSequencerContext *ctx_p)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 mappedId = 0;
        mappedId = DcmToolKit::getLocalMappedId();
        QosLocalNasShowMessage *pQosLocalNasShowMessage = dynamic_cast<QosLocalNasShowMessage*> (ctx_p->getPPrismMessage ());
        QosClientNasShowMessage *pQosClientNasShowMessage = new QosClientNasShowMessage();

        pQosClientNasShowMessage->setOpCode(pQosLocalNasShowMessage->getOpCode());
        pQosClientNasShowMessage->setCmdCode(pQosLocalNasShowMessage->getCmdCode());
        pQosClientNasShowMessage->setCos(pQosLocalNasShowMessage->getCos());
        pQosClientNasShowMessage->setDscp(pQosLocalNasShowMessage->getDscp());
        pQosClientNasShowMessage->setServerIpAddressPrefix(pQosLocalNasShowMessage->getServerIpAddressPrefix());
        pQosClientNasShowMessage->setVrfName(pQosLocalNasShowMessage->getVrfName());
        pQosClientNasShowMessage->setVlanNumber(pQosLocalNasShowMessage->getVlanNumber());
        pQosClientNasShowMessage->setMappedId(mappedId);

        trace (TRACE_LEVEL_DEBUG, string("sending NAS show to qos client.MappedId =") + mappedId);
        status = sendSynchronouslyToWaveClient ("qos", pQosClientNasShowMessage);
        status = errorCode(pQosLocalNasShowMessage, pQosClientNasShowMessage, status);
        trace (TRACE_LEVEL_DEBUG, string("copy buffer from nas client show . Status was=") + status);
		pQosLocalNasShowMessage->copyAllBuffers(*pQosClientNasShowMessage);
        delete pQosClientNasShowMessage;
        return (status);
    }
  	void QosLocalWorker::QosLocalNasShowSendToClusterCallback (WaveSendToClusterContext *ctx_p) {

      PrismLinearSequencerContext             *seq_ctx_p = NULL;
      QosLocalNasShowMessage				  *req_msg_p = NULL,
                                              *resp_msg_p = NULL,
                                              *sw_resp_p = NULL;

      trace(TRACE_LEVEL_DEBUG,string("Entering QosLocalNasShowSendToClusterCallback") );

      ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);

      seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

      req_msg_p = dynamic_cast<QosLocalNasShowMessage*> (seq_ctx_p->getPPrismMessage ());

      resp_msg_p =  dynamic_cast<QosLocalNasShowMessage*> (ctx_p->getPPrismMessageForPhase1());

      QOS_PLUG_ASSERT_PTR(req_msg_p);
      QOS_PLUG_ASSERT_PTR(resp_msg_p);

      if( WAVE_MESSAGE_ERROR_FAILED_ON_ALL_NODES != sendToClusterCompletionStatus ) {

          vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
          UI32    i = 0;

          for (i = 0; i < locations.size(); i++)
          {
              ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locations[i]);

              if(WAVE_MESSAGE_SUCCESS == statusPerLocationId) {

                  trace(TRACE_LEVEL_DEBUG, string("Response processing: The location: <") + locations[i] + string(">"));
                  sw_resp_p = dynamic_cast<QosLocalNasShowMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
                  QOS_PLUG_ASSERT_PTR(sw_resp_p);
                  req_msg_p->copyAllBuffers(*sw_resp_p);
              }
              else
              {
                  trace(TRACE_LEVEL_ERROR,string("Response processing: The location: ") + locations[i] + string("returned error: <")  + FrameworkToolKit::localize(statusPerLocationId) + ">");
              }
          }

      } else {
          trace(TRACE_LEVEL_ERROR,string("QosLocalNasShowSendToClusterCallback : ") + string("returned error:<")  + FrameworkToolKit::localize(sendToClusterCompletionStatus) + ">");
      }

      trace(TRACE_LEVEL_DEBUG,string("MC return status: ") + sendToClusterCompletionStatus);
      seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
      delete resp_msg_p;
      delete ctx_p;

      return;
  }
    


}
