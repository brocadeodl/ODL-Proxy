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

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Types/UI32Range.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "Qos/Global/QosGlobalObjectManager.h"
#include "Qos/Global/QosGlobalWorker.h"
#include "Qos/Global/DscpBitMap.h"
#include "Qos/Global/QosGlobalCreateCosToCosMutationMapMessage.h"
#include "Qos/Global/QosGlobalDeleteCosToCosMutationMapMessage.h"
#include "Qos/Global/QosGlobalCreateCosToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalDeleteCosToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalCreateQosQueueSchedulerMessage.h"
#include "Qos/Global/QosGlobalDeleteQosQueueSchedulerMessage.h"
#include "Qos/Global/QosGlobalCreateQosQueueMulticastSchedulerMessage.h"
#include "Qos/Global/QosGlobalDeleteQosQueueMulticastSchedulerMessage.h"
#include "Qos/Global/QosGlobalCreateQosRcvQueueMulticastSettingMessage.h"
#include "Qos/Global/QosGlobalDeleteQosRcvQueueMulticastSettingMessage.h"
#include "Qos/Global/CosToCosMutationMapManagedObject.h"
#include "Qos/Global/CosToTrafficClassMapManagedObject.h"
#include "Qos/Global/QosRcvQueueMulticastSettingManagedObject.h"
#include "Qos/Global/QosQueueSchedulerManagedObject.h"
#include "Qos/Global/QosQueueMulticastSchedulerManagedObject.h"
#include "Qos/Global/CeeMapManagedObject.h"
#include "Qos/Global/PriorityMapManagedObject.h"
#include "Qos/Global/PriorityGroupManagedObject.h"
#include "Qos/Local/QosCreateCosToCosMutationMapMessage.h"
#include "Qos/Local/QosDeleteCosToCosMutationMapSPMessage.h"
#include "Qos/Local/QosCreateCosToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosDeleteCosToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosCreateQosRcvQueueMulticastSettingSPMessage.h"
#include "Qos/Local/QosDeleteQosRcvQueueMulticastSettingSPMessage.h"
#include "Qos/Local/QosCreateQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosDeleteQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosCreateQosQueueSchedulerSPMessage.h"
#include "Qos/Local/QosDeleteQosQueueSchedulerSPMessage.h"
#include "Qos/Local/QosCreateQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Local/QosDeleteQosQueueMulticastSchedulerSPMessage.h"
#include "Qos/Global/QosGlobalCreateDscpMarkListMessage.h"
#include "Qos/Global/QosGlobalDeleteDscpMarkListMessage.h"
#include "Qos/Global/QosGlobalCreateDscpToDscpMutationMapMessage.h"
#include "Qos/Global/QosGlobalDeleteDscpToDscpMutationMapMessage.h"
#include "Qos/Global/QosGlobalCreateDscpToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalDeleteDscpToTrafficClassMapMessage.h"
#include "Qos/Global/QosGlobalCreateDscpToCosMapMessage.h"
#include "Qos/Global/QosGlobalDeleteDscpToCosMapMessage.h"
#include "Qos/Global/QosGlobalCreateRedProfileMessage.h"
#include "Qos/Global/QosGlobalDeleteRedProfileMessage.h"
#include "Qos/Local/QosCreateDscpMarkListSPMessage.h"
#include "Qos/Local/QosCreateDscpToCosMapSPMessage.h"
#include "Qos/Local/QosCreateDscpToDscpMutationMapSPMessage.h"
#include "Qos/Local/QosCreateDscpToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpMarkListSPMessage.h"
#include "Qos/Local/QosDeleteDscpToCosMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpToDscpMutationMapSPMessage.h"
#include "Qos/Local/QosDeleteDscpToTrafficClassMapSPMessage.h"
#include "Qos/Local/QosCreateRedProfileSPMessage.h"
#include "Qos/Local/QosDeleteRedProfileSPMessage.h"
#include "Qos/Global/DscpMarkListManagedObject.h"
#include "Qos/Global/DscpToDscpMutationMapManagedObject.h"
#include "Qos/Global/DscpToTrafficClassMapManagedObject.h"
#include "Qos/Global/DscpToCosMapManagedObject.h"
#include "Qos/Global/RedProfileManagedObject.h"
#include "Qos/Global/QosGlobalTypes.h"
#include "Qos/Common/QosUtils.h"
#include "ClientInterface/Ssm/qoscommon.h"
#include "APPM/Global/QosProfileCfgManagedObject.h"
#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "DcmCStatus.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Utils/DceClusterUtils.h"
#include "Nsm/Global/NsmGlobalQosMapCleanUpMessage.h"
#include "Qos/Global/QosGlobalNasMessage.h"
#include "Qos/Local/QosLocalNasMessage.h"
#include "Qos/Global/NasServerIpManagedObject.h"
#include "Qos/Global/NasServerIpVlanManagedObject.h"
#include "Qos/Global/NasServerIpVrfManagedObject.h"
#include "Qos/Global/NasConfigManagedObject.h"
#include "ClientInterface/Qos/qos_nas_dcm_common.h"
#include "ClientInterface/Nsm/NsmUtils.h"


namespace DcmNs
{

  QosGlobalWorker::QosGlobalWorker ( QosGlobalObjectManager *pQosGlobalObjectManager)
	: WaveWorker  (pQosGlobalObjectManager)
	{
	  CosToCosMutationMapManagedObject    CosToCosMutationMapManagedObject    (pQosGlobalObjectManager);
	  CosToTrafficClassMapManagedObject    CosToTrafficClassMapManagedObject    (pQosGlobalObjectManager);
	  QosRcvQueueMulticastSettingManagedObject    QosRcvQueueMulticastSettingManagedObject    (pQosGlobalObjectManager);
	  QosQueueSchedulerManagedObject    QosQueueSchedulerManagedObject    (pQosGlobalObjectManager);
	  QosQueueMulticastSchedulerManagedObject    QosQueueMulticastSchedulerManagedObject    (pQosGlobalObjectManager);
	  CosToCosMutationMapManagedObject.setupOrm ();
	  addManagedClass (CosToCosMutationMapManagedObject::getClassName (), this);
	  CosToTrafficClassMapManagedObject.setupOrm ();
	  addManagedClass (CosToTrafficClassMapManagedObject::getClassName (), this);
	  QosRcvQueueMulticastSettingManagedObject.setupOrm ();
	  addManagedClass (QosRcvQueueMulticastSettingManagedObject::getClassName (), this);
	  QosQueueSchedulerManagedObject.setupOrm ();
	  addManagedClass (QosQueueSchedulerManagedObject::getClassName (), this);
	  QosQueueMulticastSchedulerManagedObject.setupOrm ();
	  addManagedClass (QosQueueMulticastSchedulerManagedObject::getClassName (), this);
      DscpMarkListManagedObject    DscpMarkListManagedObject    (pQosGlobalObjectManager);
      DscpToDscpMutationMapManagedObject    DscpToDscpMutationMapManagedObject    (pQosGlobalObjectManager);
      DscpToTrafficClassMapManagedObject    DscpToTrafficClassMapManagedObject    (pQosGlobalObjectManager);
      DscpToCosMapManagedObject    DscpToCosMapManagedObject    (pQosGlobalObjectManager);
      RedProfileManagedObject    RedProfileManagedObject    (pQosGlobalObjectManager);
      DscpMarkListManagedObject.setupOrm ();
      addManagedClass (DscpMarkListManagedObject::getClassName (), this);
      DscpToDscpMutationMapManagedObject.setupOrm ();
      addManagedClass (DscpToDscpMutationMapManagedObject::getClassName (), this);
      DscpToTrafficClassMapManagedObject.setupOrm ();
      addManagedClass (DscpToTrafficClassMapManagedObject::getClassName (), this);
      DscpToCosMapManagedObject.setupOrm ();
      addManagedClass (DscpToCosMapManagedObject::getClassName (), this);
      RedProfileManagedObject.setupOrm ();
	  addManagedClass (RedProfileManagedObject::getClassName (), this);
      NasConfigManagedObject NasConfigManagedObject(pQosGlobalObjectManager);
	  NasConfigManagedObject.setupOrm ();
      addManagedClass (NasConfigManagedObject::getClassName (), this);
	  NasServerIpManagedObject NasServerIpManagedObject(pQosGlobalObjectManager);
      NasServerIpManagedObject.setupOrm ();
      addManagedClass (NasServerIpManagedObject::getClassName (), this);
	  NasServerIpVlanManagedObject NasServerIpVlanManagedObject(pQosGlobalObjectManager);
      NasServerIpVlanManagedObject.setupOrm ();
      addManagedClass (NasServerIpVlanManagedObject::getClassName (), this);
	  NasServerIpVrfManagedObject NasServerIpVrfManagedObject(pQosGlobalObjectManager);
      NasServerIpVrfManagedObject.setupOrm ();
      addManagedClass (NasServerIpVrfManagedObject::getClassName (), this);

	  addOperationMap (QOSGLOBALCREATECOSTOCOSMUTATIONMAP,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateCosToCosMutationMapMessageHandler));
	  addOperationMap (QOSGLOBALDELETECOSTOCOSMUTATIONMAP,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteCosToCosMutationMapMessageHandler));
	  addOperationMap (QOSGLOBALCREATECOSTOTRAFFICCLASSMAP,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateCosToTrafficClassMapMessageHandler));
	  addOperationMap (QOSGLOBALDELETECOSTOTRAFFICCLASSMAP,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteCosToTrafficClassMapMessageHandler));
	  addOperationMap (QOSGLOBALCREATEQOSQUEUESCHEDULER,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateQosQueueSchedulerMessageHandler));
	  addOperationMap (QOSGLOBALDELETEQOSQUEUESCHEDULER,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteQosQueueSchedulerMessageHandler));
	  addOperationMap (QOSGLOBALCREATEQOSQUEUEMULTICASTSCHEDULER,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateQosQueueMulticastSchedulerMessageHandler));
	  addOperationMap (QOSGLOBALDELETEQOSQUEUEMULTICASTSCHEDULER,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteQosQueueMulticastSchedulerMessageHandler));
	  addOperationMap (QOSGLOBALCREATEQOSRCVQUEUEMULTICASTSETTING,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateQosRcvQueueMulticastSettingMessageHandler));
	  addOperationMap (QOSGLOBALDELETEQOSRCVQUEUEMULTICASTSETTING,
		  reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteQosRcvQueueMulticastSettingMessageHandler));
      addOperationMap (QOSGLOBALCREATEDSCPMARKLIST, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateDscpMarkListMessageHandler));
      addOperationMap (QOSGLOBALDELETEDSCPMARKLIST, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteDscpMarkListMessageHandler));
      addOperationMap (QOSGLOBALCREATEDSCPTODSCPMUTATIONMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateDscpToDscpMutationMapMessageHandler));
      addOperationMap (QOSGLOBALDELETEDSCPTODSCPMUTATIONMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteDscpToDscpMutationMapMessageHandler));
      addOperationMap (QOSGLOBALCREATEDSCPTOTRAFFICCLASSMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateDscpToTrafficClassMapMessageHandler));
      addOperationMap (QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteDscpToTrafficClassMapMessageHandler));
      addOperationMap (QOSGLOBALCREATEDSCPTOCOSMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateDscpToCosMapMessageHandler));
      addOperationMap (QOSGLOBALDELETEDSCPTOCOSMAP, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteDscpToCosMapMessageHandler));
      addOperationMap (QOSGLOBALCREATEREDPROFILE, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalCreateRedProfileMessageHandler));
      addOperationMap (QOSGLOBALDELETEREDPROFILE, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalDeleteRedProfileMessageHandler));
	  addOperationMap (QOSGLOBALNASCONFIG, reinterpret_cast<PrismMessageHandler> (&QosGlobalWorker::QosGlobalNasMessageHandler));
	}

  QosGlobalWorker::~QosGlobalWorker ()
  {
  }

  PrismMessage  *QosGlobalWorker::createMessageInstance(const UI32 &operationCode)
  {
	PrismMessage *pPrismMessage = NULL;

	switch (operationCode)
	{

	  case QOSGLOBALCREATECOSTOCOSMUTATIONMAP :
		pPrismMessage = new QosGlobalCreateCosToCosMutationMapMessage ();
		break;
	  case QOSGLOBALDELETECOSTOCOSMUTATIONMAP :
		pPrismMessage = new QosGlobalDeleteCosToCosMutationMapMessage ();
		break;
	  case QOSGLOBALCREATECOSTOTRAFFICCLASSMAP :
		pPrismMessage = new QosGlobalCreateCosToTrafficClassMapMessage ();
		break;
	  case QOSGLOBALDELETECOSTOTRAFFICCLASSMAP :
		pPrismMessage = new QosGlobalDeleteCosToTrafficClassMapMessage ();
		break;
	  case QOSGLOBALCREATEQOSQUEUESCHEDULER :
		pPrismMessage = new QosGlobalCreateQosQueueSchedulerMessage ();
		break;
	  case QOSGLOBALDELETEQOSQUEUESCHEDULER :
		pPrismMessage = new QosGlobalDeleteQosQueueSchedulerMessage ();
		break;
	  case QOSGLOBALCREATEQOSQUEUEMULTICASTSCHEDULER :
		pPrismMessage = new QosGlobalCreateQosQueueMulticastSchedulerMessage ();
		break;
	  case QOSGLOBALDELETEQOSQUEUEMULTICASTSCHEDULER :
		pPrismMessage = new QosGlobalDeleteQosQueueMulticastSchedulerMessage ();
		break;
	  case QOSGLOBALCREATEQOSRCVQUEUEMULTICASTSETTING :
		pPrismMessage = new QosGlobalCreateQosRcvQueueMulticastSettingMessage ();
		break;
	  case QOSGLOBALDELETEQOSRCVQUEUEMULTICASTSETTING :
		pPrismMessage = new QosGlobalDeleteQosRcvQueueMulticastSettingMessage ();
		break;
	  case QOSGLOBALCREATEDSCPMARKLIST :
		pPrismMessage = new QosGlobalCreateDscpMarkListMessage ();
		break;
	  case QOSGLOBALDELETEDSCPMARKLIST :
		pPrismMessage = new QosGlobalDeleteDscpMarkListMessage ();
		break;
	  case QOSGLOBALCREATEDSCPTODSCPMUTATIONMAP :
		pPrismMessage = new QosGlobalCreateDscpToDscpMutationMapMessage ();
		break;
	  case QOSGLOBALDELETEDSCPTODSCPMUTATIONMAP :
		pPrismMessage = new QosGlobalDeleteDscpToDscpMutationMapMessage ();
		break;
	  case QOSGLOBALCREATEDSCPTOTRAFFICCLASSMAP :
		pPrismMessage = new QosGlobalCreateDscpToTrafficClassMapMessage ();
		break;
	  case QOSGLOBALDELETEDSCPTOTRAFFICCLASSMAP :
		pPrismMessage = new QosGlobalDeleteDscpToTrafficClassMapMessage ();
		break;
	  case QOSGLOBALCREATEDSCPTOCOSMAP :
		pPrismMessage = new QosGlobalCreateDscpToCosMapMessage ();
		break;
	  case QOSGLOBALDELETEDSCPTOCOSMAP :
		pPrismMessage = new QosGlobalDeleteDscpToCosMapMessage ();
		break;
	  case QOSGLOBALCREATEREDPROFILE :
		pPrismMessage = new QosGlobalCreateRedProfileMessage ();
		break;
	  case QOSGLOBALDELETEREDPROFILE :
		pPrismMessage = new QosGlobalDeleteRedProfileMessage ();
		break;
	  case QOSGLOBALNASCONFIG:
        pPrismMessage = new QosGlobalNasMessage ();
        break;
	  default :
		pPrismMessage = NULL;
	}

	return (pPrismMessage);
  }

  WaveManagedObject  *QosGlobalWorker::createManagedObjectInstance(const string &managedClassName)
  {
	WaveManagedObject *pWaveManagedObject = NULL;

	if ((CosToCosMutationMapManagedObject::getClassName ()) == managedClassName)
	{
	  pWaveManagedObject = new CosToCosMutationMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((CosToTrafficClassMapManagedObject::getClassName ()) == managedClassName)
	{
	  pWaveManagedObject = new CosToTrafficClassMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((QosRcvQueueMulticastSettingManagedObject::getClassName ()) == managedClassName)
	{
	  pWaveManagedObject = new QosRcvQueueMulticastSettingManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((QosQueueSchedulerManagedObject::getClassName ()) == managedClassName)
	{
	  pWaveManagedObject = new QosQueueSchedulerManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((QosQueueMulticastSchedulerManagedObject::getClassName ()) == managedClassName)
	{
	  pWaveManagedObject = new QosQueueMulticastSchedulerManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((DscpMarkListManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new DscpMarkListManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((DscpToDscpMutationMapManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new DscpToDscpMutationMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((DscpToTrafficClassMapManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new DscpToTrafficClassMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((DscpToCosMapManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new DscpToCosMapManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((RedProfileManagedObject::getClassName ()) == managedClassName)
	{
		pWaveManagedObject = new RedProfileManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
	}
	else if ((NasServerIpManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NasServerIpManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
    }
	else if ((NasServerIpVlanManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NasServerIpVlanManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
    }
	else if ((NasServerIpVrfManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NasServerIpVrfManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((NasConfigManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new NasConfigManagedObject(dynamic_cast<QosGlobalObjectManager *>(getPWaveObjectManager()));
    }
	else
	{
	  trace (TRACE_LEVEL_FATAL, "QosGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
	}
	return (pWaveManagedObject);
  }

  void QosGlobalWorker::createCosToCosMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	QosGlobalCreateCosToCosMutationMapMessage *pQosGlobalCreateCosToCosMutationMapMessage =
		dynamic_cast<QosGlobalCreateCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

	WaveSendToClusterContext    *send_ctx_p;
	QosCreateCosToCosMutationMapSPMessage *m = new QosCreateCosToCosMutationMapSPMessage();

	// vcs sanity check
	int cosMutationBitmap = 0;
	int i = 0;	
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos0MutatedCosValue();
    cosMutationBitmap |= (1 << i);
	i = pQosGlobalCreateCosToCosMutationMapMessage->getCos1MutatedCosValue();
	cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos2MutatedCosValue();
    cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos3MutatedCosValue();
    cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos4MutatedCosValue();
    cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos5MutatedCosValue();
    cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos6MutatedCosValue();
    cosMutationBitmap |= (1 << i);
    i = pQosGlobalCreateCosToCosMutationMapMessage->getCos7MutatedCosValue();
    cosMutationBitmap |= (1 << i);

	trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName());

	ResourceId status;
	if ((status = QosUtils::vcsQosSanity(QOS_INTERFACE_COS_MUTATION, pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName(),
		false, cosMutationBitmap, 0,0)) != WAVE_MESSAGE_SUCCESS) {
		delete m;
		pPrismLinearSequencerContext->executeNextStep(status);
		return;
	}

	m->setCosToCosMutationMapName(pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName());
	m->setCos0MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos0MutatedCosValue());
	m->setCos1MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos1MutatedCosValue());
	m->setCos2MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos2MutatedCosValue());
	m->setCos3MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos3MutatedCosValue());
	m->setCos4MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos4MutatedCosValue());
	m->setCos5MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos5MutatedCosValue());
	m->setCos6MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos6MutatedCosValue());
	m->setCos7MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos7MutatedCosValue());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
	prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
	send_ctx_p->setPPrismMessageForPhase1(m);
	trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName());
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
	sendToWaveCluster(send_ctx_p);
	return ;
  }

  void QosGlobalWorker::createCosToCosMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	int createFlag = 1;
	QosGlobalCreateCosToCosMutationMapMessage *pQosGlobalCreateCosToCosMutationMapMessage =
	  dynamic_cast<QosGlobalCreateCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;
	string mapName = pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName();
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"cosToCosMutationMapName"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
	  UI32 numberOfResults = pResults->size ();
	  if (0 == numberOfResults) {
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
		createFlag = 1;
	  } else if (1 == numberOfResults) {
		pCosToCosMutationMapManagedObject  = dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults)[0]);
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
		createFlag = 0;
	  }
	}

	if (createFlag == 0) {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
	  updateWaveManagedObject (pCosToCosMutationMapManagedObject);
	  prismAssert (NULL != pCosToCosMutationMapManagedObject, __FILE__, __LINE__);

	} else {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
	  pCosToCosMutationMapManagedObject = new CosToCosMutationMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
	  prismAssert (NULL != pCosToCosMutationMapManagedObject, __FILE__, __LINE__);
	}

	pCosToCosMutationMapManagedObject->setCosToCosMutationMapName(pQosGlobalCreateCosToCosMutationMapMessage->getCosToCosMutationMapName());
	pCosToCosMutationMapManagedObject->setCos0MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos0MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos1MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos1MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos2MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos2MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos3MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos3MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos4MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos4MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos5MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos5MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos6MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos6MutatedCosValue());
	pCosToCosMutationMapManagedObject->setCos7MutatedCosValue(pQosGlobalCreateCosToCosMutationMapMessage->getCos7MutatedCosValue());

    if ( (createFlag) && (pCosToCosMutationMapManagedObject)) {
        //Sanjeev: mark the MO for GC
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pCosToCosMutationMapManagedObject);
    }
    // Sanjeev: mark the query results for GC.
    if (pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }
    

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
  }


  void  QosGlobalWorker::QosGlobalCreateCosToCosMutationMapMessageHandler( QosGlobalCreateCosToCosMutationMapMessage
	  *pQosGlobalCreateCosToCosMutationMapMessage)
  {
	PrismLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createCosToCosMutationMapStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createCosToCosMutationMapMOStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
	};

	PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateCosToCosMutationMapMessage,
		this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismLinearSequencerContext->holdAll();
	pPrismLinearSequencerContext->start();
  }

  void QosGlobalWorker::createCosToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	QosGlobalCreateCosToTrafficClassMapMessage *pQosGlobalCreateCosToTrafficClassMapMessage =
	  dynamic_cast<QosGlobalCreateCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	WaveSendToClusterContext    *send_ctx_p;

	QosCreateCosToTrafficClassMapSPMessage *m = new QosCreateCosToTrafficClassMapSPMessage();

	m->setCosToTrafficClassMapName(pQosGlobalCreateCosToTrafficClassMapMessage->getCosToTrafficClassMapName());
	m->setCos0MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos0MappedTrafficClassValue());
	m->setCos1MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos1MappedTrafficClassValue());
	m->setCos2MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos2MappedTrafficClassValue());
	m->setCos3MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos3MappedTrafficClassValue());
	m->setCos4MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos4MappedTrafficClassValue());
	m->setCos5MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos5MappedTrafficClassValue());
	m->setCos6MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos6MappedTrafficClassValue());
	m->setCos7MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos7MappedTrafficClassValue());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateCosToTrafficClassMapMessage->getCosToTrafficClassMapName());
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
  }

  void QosGlobalWorker::createCosToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	int createFlag = 1;
	QosGlobalCreateCosToTrafficClassMapMessage *pQosGlobalCreateCosToTrafficClassMapMessage =
	  dynamic_cast<QosGlobalCreateCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;
	string mapName = pQosGlobalCreateCosToTrafficClassMapMessage->getCosToTrafficClassMapName();
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
	syncQueryCtxt.addAndAttribute (new AttributeString(&mapName,"cosToTrafficClassMapName"));
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
	  UI32 numberOfResults = pResults->size ();
	  if (0 == numberOfResults) {
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
		createFlag = 1;
	  } else if (1 == numberOfResults) {
		pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject*>((*pResults)[0]);
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
		createFlag = 0;
	  }
	}

	if (createFlag == 0) {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
	  updateWaveManagedObject (pCosToTrafficClassMapManagedObject);
	  prismAssert (NULL != pCosToTrafficClassMapManagedObject, __FILE__, __LINE__);

	} else {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
	  pCosToTrafficClassMapManagedObject = new CosToTrafficClassMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
	  prismAssert (NULL != pCosToTrafficClassMapManagedObject, __FILE__, __LINE__);
	}

	pCosToTrafficClassMapManagedObject->setCosToTrafficClassMapName(pQosGlobalCreateCosToTrafficClassMapMessage->getCosToTrafficClassMapName());
	pCosToTrafficClassMapManagedObject->setCos0MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos0MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos1MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos1MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos2MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos2MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos3MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos3MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos4MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos4MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos5MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos5MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos6MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos6MappedTrafficClassValue());
	pCosToTrafficClassMapManagedObject->setCos7MappedTrafficClassValue(pQosGlobalCreateCosToTrafficClassMapMessage->getCos7MappedTrafficClassValue());

    if ( (createFlag) && (pCosToTrafficClassMapManagedObject)) {
        //Sanjeev: mark the MO for GC
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(
                pCosToTrafficClassMapManagedObject);
    }
    // Sanjeev: mark the query results for GC.
    if (pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }


	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
  }


  void  QosGlobalWorker::QosGlobalCreateCosToTrafficClassMapMessageHandler( QosGlobalCreateCosToTrafficClassMapMessage *pQosGlobalCreateCosToTrafficClassMapMessage)
  {
	PrismLinearSequencerStep sequencerSteps[] =
	{
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createCosToTrafficClassMapStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createCosToTrafficClassMapMOStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	  reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
	};

	PrismLinearSequencerContext *pPrismLinearSequencerContext =
	  new PrismLinearSequencerContext (pQosGlobalCreateCosToTrafficClassMapMessage, this, sequencerSteps,
		  sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	pPrismLinearSequencerContext->holdAll();
	pPrismLinearSequencerContext->start();
  }

  void QosGlobalWorker::createMcastSchedulerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	QosGlobalCreateQosQueueMulticastSchedulerMessage *pQosGlobalCreateQosQueueMulticastSchedulerMessage =
		dynamic_cast<QosGlobalCreateQosQueueMulticastSchedulerMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	WaveSendToClusterContext    *send_ctx_p;

	QosCreateQosQueueMulticastSchedulerSPMessage *m = new QosCreateQosQueueMulticastSchedulerSPMessage();

	m->setDwrrTrafficClass0(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass6());
	m->setDwrrTrafficClass7(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass7());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosCreateQosQueueMulticastSchedulerSPMessage"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
  }

  void QosGlobalWorker::createMcastSchedulerMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	int createFlag = 1;
	QosQueueMulticastSchedulerManagedObject *m = NULL;
	QosGlobalCreateQosQueueMulticastSchedulerMessage *pQosGlobalCreateQosQueueMulticastSchedulerMessage =
	  dynamic_cast<QosGlobalCreateQosQueueMulticastSchedulerMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueMulticastSchedulerManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
	  UI32 numberOfResults = pResults->size ();
	  if (0 == numberOfResults) {
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
		createFlag = 1;
	  } else if (1 == numberOfResults) {
		m = dynamic_cast<QosQueueMulticastSchedulerManagedObject*>((*pResults)[0]);
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
		createFlag = 0;
	  }
	}

	if (createFlag == 0) {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
	  updateWaveManagedObject (m);
	  prismAssert (NULL != m, __FILE__, __LINE__);

	} else {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");

	  m = new QosQueueMulticastSchedulerManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));

	  prismAssert (NULL != m, __FILE__, __LINE__);
	}
	m->setDwrrTrafficClass0(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass6());
	m->setDwrrTrafficClass7(pQosGlobalCreateQosQueueMulticastSchedulerMessage->getDwrrTrafficClass7());

    if ( (createFlag) && (m)) {
        //Sanjeev: mark the MO for GC
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(m);
    }
    // Sanjeev: mark the query results for GC.
    if (pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
  }

void  QosGlobalWorker::QosGlobalCreateQosQueueMulticastSchedulerMessageHandler( QosGlobalCreateQosQueueMulticastSchedulerMessage *pQosGlobalCreateQosQueueMulticastSchedulerMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createMcastSchedulerStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createMcastSchedulerMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateQosQueueMulticastSchedulerMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

  void QosGlobalWorker::createSchedulerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	QosGlobalCreateQosQueueSchedulerMessage *pQosGlobalCreateQosQueueSchedulerMessage =
	  dynamic_cast<QosGlobalCreateQosQueueSchedulerMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	WaveSendToClusterContext    *send_ctx_p;

	QosCreateQosQueueSchedulerSPMessage *m = new QosCreateQosQueueSchedulerSPMessage();

	m->setDwrrTrafficClass0(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass6());
	m->setDwrrTrafficClass7(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass7());
	m->setStrictPriority(pQosGlobalCreateQosQueueSchedulerMessage->getStrictPriority());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosCreateQosQueueSchedulerSPMessage"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
  }

  void QosGlobalWorker::createSchedulerMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	int createFlag = 1;
	QosQueueSchedulerManagedObject *m = NULL;
	QosGlobalCreateQosQueueSchedulerMessage *pQosGlobalCreateQosQueueSchedulerMessage =
	  dynamic_cast<QosGlobalCreateQosQueueSchedulerMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueSchedulerManagedObject::getClassName());
	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

	if (NULL != pResults) {
	  UI32 numberOfResults = pResults->size ();
	  if (0 == numberOfResults) {
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
		createFlag = 1;
	  } else if (1 == numberOfResults) {
		m = dynamic_cast<QosQueueSchedulerManagedObject*>((*pResults)[0]);
		trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
		createFlag = 0;
	  }
	}

	if (createFlag == 0) {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
	  updateWaveManagedObject (m);
	  prismAssert (NULL != m, __FILE__, __LINE__);

	} else {
	  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");

	  m = new QosQueueSchedulerManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));

	  prismAssert (NULL != m, __FILE__, __LINE__);
	}
	m->setDwrrTrafficClass0(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass0());
	m->setDwrrTrafficClass1(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass1());
	m->setDwrrTrafficClass2(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass2());
	m->setDwrrTrafficClass3(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass3());
	m->setDwrrTrafficClass4(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass4());
	m->setDwrrTrafficClass5(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass5());
	m->setDwrrTrafficClass6(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass6());
	m->setStrictPriority(pQosGlobalCreateQosQueueSchedulerMessage->getStrictPriority());
	switch (pQosGlobalCreateQosQueueSchedulerMessage->getStrictPriority())
	{
	  case 0:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass7());
		break;
	  case 1:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass6());
		m->setDwrrTrafficClass6(0);
		break;
	  case 2:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass5());
		m->setDwrrTrafficClass5(0);
		break;
	  case 3:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass4());
		m->setDwrrTrafficClass4(0);
		break;
	  case 4:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass3());
		m->setDwrrTrafficClass3(0);
		break;
	  case 5:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass2());
		m->setDwrrTrafficClass2(0);
		break;
	  case 6:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass1());
		m->setDwrrTrafficClass1(0);
		break;
	  case 7:
		m->setDwrrTrafficClassLast(pQosGlobalCreateQosQueueSchedulerMessage->getDwrrTrafficClass0());
		m->setDwrrTrafficClass0(0);
		break;
	}
    if ( (createFlag) && (m)) {
        //Sanjeev: mark the MO for GC
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(m);
    }
    // Sanjeev: mark the query results for GC.
    if (pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
  }

void  QosGlobalWorker::QosGlobalCreateQosQueueSchedulerMessageHandler( QosGlobalCreateQosQueueSchedulerMessage *pQosGlobalCreateQosQueueSchedulerMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createSchedulerStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createSchedulerMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateQosQueueSchedulerMessage,
	  this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::createMcastSettingStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalCreateQosRcvQueueMulticastSettingMessage *pQosGlobalCreateQosRcvQueueMulticastSettingMessage =
	dynamic_cast<QosGlobalCreateQosRcvQueueMulticastSettingMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
  WaveSendToClusterContext    *send_ctx_p;

  QosCreateQosRcvQueueMulticastSettingSPMessage *m = new QosCreateQosRcvQueueMulticastSettingSPMessage();

  m->setRateLimit(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getRateLimit());
  m->setBurst(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getBurst());
  m->setThresholdTrafficClass0(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass0());
  m->setThresholdTrafficClass1(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass1());
  m->setThresholdTrafficClass2(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass2());
  m->setThresholdTrafficClass3(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass3());
  m->setThresholdTrafficClass4(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass4());
  m->setThresholdTrafficClass5(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass5());
  m->setThresholdTrafficClass6(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass6());
  m->setThresholdTrafficClass7(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass7());
  m->setOpCode(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getOpCode());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosCreateQosRcvQueueMulticastSettingSPMessage"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
}

void QosGlobalWorker::createMcastSettingMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  int createFlag = 1;
  SI32 defaultThreshold = DEFAULT_THRESHOLD;
  SI32 defaultRateLimit = DEFAULT_RATE_LIMIT;
  SI32 defaultBurst = DEFAULT_BURST;
  QosRcvQueueMulticastSettingManagedObject *m = NULL;
  QosGlobalCreateQosRcvQueueMulticastSettingMessage *pQosGlobalCreateQosRcvQueueMulticastSettingMessage =
	dynamic_cast<QosGlobalCreateQosRcvQueueMulticastSettingMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosRcvQueueMulticastSettingManagedObject::getClassName());
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
	  createFlag = 1;
	} else if (1 == numberOfResults) {
	  m = dynamic_cast<QosRcvQueueMulticastSettingManagedObject*>((*pResults)[0]);
	  trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
	  createFlag = 0;
	}
  }

  if (createFlag == 0) {
	trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
	updateWaveManagedObject (m);
	prismAssert (NULL != m, __FILE__, __LINE__);

  } else {
	trace (TRACE_LEVEL_DEVEL, "createFlag = 1");

	m = new QosRcvQueueMulticastSettingManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));

	prismAssert (NULL != m, __FILE__, __LINE__);
  }

  switch(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getOpCode())
  {
	case MCAST_THRESHOLD:
	  trace (TRACE_LEVEL_DEVEL, "case 0");
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass0=") + pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass0());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass1=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass1());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass2=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass2());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass3=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass3());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass4=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass4());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass5=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass5());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass6=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass6());
	  trace (TRACE_LEVEL_DEVEL, string("ThresholdTrafficClass7=") +
		  pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass7());

	  m->setThresholdTrafficClass0(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass0());
	  m->setThresholdTrafficClass1(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass1());
	  m->setThresholdTrafficClass2(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass2());
	  m->setThresholdTrafficClass3(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass3());
	  m->setThresholdTrafficClass4(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass4());
	  m->setThresholdTrafficClass5(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass5());
	  m->setThresholdTrafficClass6(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass6());
	  m->setThresholdTrafficClass7(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getThresholdTrafficClass7());
	  if (createFlag == 1) {
		m->setRateLimit(defaultRateLimit);
		m->setBurst(defaultBurst);
	  }
	  break;

	case MCAST_RATELIMIT: /* only rate-limit */
	  trace (TRACE_LEVEL_DEVEL, "case 1");
	  m->setRateLimit(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getRateLimit());
	  m->setBurst(defaultBurst);

	  if (createFlag == 1) {
		m->setThresholdTrafficClass0(defaultThreshold);
		m->setThresholdTrafficClass1(defaultThreshold);
		m->setThresholdTrafficClass2(defaultThreshold);
		m->setThresholdTrafficClass3(defaultThreshold);
		m->setThresholdTrafficClass4(defaultThreshold);
		m->setThresholdTrafficClass5(defaultThreshold);
		m->setThresholdTrafficClass6(defaultThreshold);
		m->setThresholdTrafficClass7(defaultThreshold);
		m->setBurst(defaultBurst);
	  }
	  break;

	case MCAST_RATE_BURST: /* rate-limit and burst */
	  trace (TRACE_LEVEL_DEVEL, "case 2");
	  m->setRateLimit(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getRateLimit());
	  m->setBurst(pQosGlobalCreateQosRcvQueueMulticastSettingMessage->getBurst());

	  if (createFlag == 1) {
		m->setThresholdTrafficClass0(defaultThreshold);
		m->setThresholdTrafficClass1(defaultThreshold);
		m->setThresholdTrafficClass2(defaultThreshold);
		m->setThresholdTrafficClass3(defaultThreshold);
		m->setThresholdTrafficClass4(defaultThreshold);
		m->setThresholdTrafficClass5(defaultThreshold);
		m->setThresholdTrafficClass6(defaultThreshold);
		m->setThresholdTrafficClass7(defaultThreshold);
	  }
	  break;


	default:
	  break;
  }

  if ( (createFlag) && (m)) {
      //Sanjeev: mark the MO for GC
      pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(m);
  }
  // Sanjeev: mark the query results for GC.
  if (pResults) {
      pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection( *pResults);
      delete pResults;
  }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void  QosGlobalWorker::QosGlobalCreateQosRcvQueueMulticastSettingMessageHandler( QosGlobalCreateQosRcvQueueMulticastSettingMessage
	*pQosGlobalCreateQosRcvQueueMulticastSettingMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createMcastSettingStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createMcastSettingMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateQosRcvQueueMulticastSettingMessage,
	  this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::deleteCosToCosMutationMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage =
    dynamic_cast<QosGlobalDeleteCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

  ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
  ObjectId cosMapObjectId ;
  UI32 poCount = 0;
  UI32 phyCount = 0;

  string cosMapName = pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName();
  status = QosUtils::getQosMapObjectIdByMapName(cosMapName, cosMapObjectId, COS_MUTATION_MAP, NULL);
  if (status != WAVE_MESSAGE_SUCCESS)
  {
    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName cosMap failed"));
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    return;
  }

  /* Check if map is activated in Port channel/Physical interface. If yes, throw error */
  WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
  poSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(cosMapObjectId, "qosCosMap", PoIntfManagedObject::getClassName()));
  status = querySynchronouslyForCount(&poSyncQueryCtxt, poCount);


  WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
  phyIntfSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(cosMapObjectId, "qosCosMap", PhyIntfLocalManagedObject::getClassName()));
  phyIntfSyncQueryCtxt.setFilterByConnectedNodes();
  status = querySynchronouslyForCount(&phyIntfSyncQueryCtxt, phyCount);


  if (status != WAVE_MESSAGE_SUCCESS)
  {
    trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosCosMap on PO failed"));
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    return;
  }
  else
  {
    if ( (0 != poCount) || ( 0 != phyCount))
    {
      pPrismLinearSequencerContext->executeNextStep(WRC_SSM_QOS_MAP_IS_STILL_ACTIVATED);
      return;
    }
  }
  pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void QosGlobalWorker::deleteCosToCosMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage =
	dynamic_cast<QosGlobalDeleteCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
  WaveSendToClusterContext    *send_ctx_p;

  QosDeleteCosToCosMutationMapSPMessage *m = new QosDeleteCosToCosMutationMapSPMessage();

  m->setCosToCosMutationMapName(pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName());
    send_ctx_p->setPartialSuccessFlag(true);
    send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;

}

void QosGlobalWorker::deleteCosToCosMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage =
	dynamic_cast<QosGlobalDeleteCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

  string cosToCosMutationMapName = pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName();
  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToCosMutationMapManagedObject::getClassName());
  syncQueryCtxt.addAndAttribute (new AttributeString(cosToCosMutationMapName,"cosToCosMutationMapName"));
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  CosToCosMutationMapManagedObject *pCosToCosMutationMapManagedObject = NULL;

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToCosMutationMapMOStep: MO not found");
	} else if (1 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToCosMutationMapMOStep: MO found");
	  pCosToCosMutationMapManagedObject = dynamic_cast<CosToCosMutationMapManagedObject*>((*pResults)[0]);
	} else {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToCosMutationMapMOStep: More than one MO found");
	}
  }

	prismAssert (NULL != pCosToCosMutationMapManagedObject, __FILE__, __LINE__);
	trace (TRACE_LEVEL_INFO, string ("\tmapname:: ") + pCosToCosMutationMapManagedObject->getCosToCosMutationMapName());
  
	delete pCosToCosMutationMapManagedObject;

    if (NULL != pResults) {
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void QosGlobalWorker::appmQosProfileMutationUpdateStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage = dynamic_cast<QosGlobalDeleteCosToCosMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    string cosToCosMutationMapName = pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName();
    ResourceId  AppmStatus   = WAVE_MESSAGE_ERROR;
    APPMGlobalAPPMQosMessage *AppmQosProfileMsg = NULL;

    AppmQosProfileMsg = new APPMGlobalAPPMQosMessage();

    if (!AppmQosProfileMsg)
    {
        pPrismLinearSequencerContext->executeNextStep(AppmStatus);
        return;
    }

    /*
     *Sending the message to APPM Plugin.
     */

    AppmQosProfileMsg->setOpCode(APPM_QOS_MSG);
    AppmQosProfileMsg->setCmdCode(QOS_NO_COS_TO_COS_MUTATION);
    AppmQosProfileMsg->setCosToCosMutationMap(cosToCosMutationMapName);
    AppmStatus = sendSynchronously(AppmQosProfileMsg);
    delete AppmQosProfileMsg;

    pPrismLinearSequencerContext->executeNextStep(AppmStatus);
    return;
}

/*
 * When a qos map is active on an interface and the node to which this interface belongs to is down,
 * the deletion of this map should be allowed.
 * And also stale entries from the database should be cleaned.
 */
void QosGlobalWorker::cleanupCosMutationMapReferenceInFailedOverNodes(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	QosGlobalDeleteCosToCosMutationMapMessage *pQosGlobalDeleteCosToCosMutationMapMessage =
			dynamic_cast<QosGlobalDeleteCosToCosMutationMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
	trace(TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupCosMutationMapReferenceInFailedOverNodes");

	string cosToCosMutationMapName =
			pQosGlobalDeleteCosToCosMutationMapMessage->getCosToCosMutationMapName();
	NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
	pMsg = new NsmGlobalQosMapCleanUpMessage(cosToCosMutationMapName,
			QOS_INTERFACE_COS_MUTATION);
	status = sendSynchronously(pMsg);
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupCosMutationMapReferenceInFailedOverNodes status ="
					+ status);
	delete (pMsg);
	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void QosGlobalWorker::cleanupCosToTrafficClassMapReferenceInFailedOverNodes(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage =
			dynamic_cast<QosGlobalDeleteCosToTrafficClassMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupCosToTrafficClassMapReferenceInFailedOverNodes");

	string cosToTrafficClassMapName =
			pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName();
	NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
	pMsg = new NsmGlobalQosMapCleanUpMessage(cosToTrafficClassMapName,
			QOS_INTERFACE_COS_TRAFFIC_CLASS);
	status = sendSynchronously(pMsg);
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupCosToTrafficClassMapReferenceInFailedOverNodes status ="
					+ status);
	delete (pMsg);
	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void QosGlobalWorker::cleanupDscpToCosMapReferenceInFailedOverNodes(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage =
			dynamic_cast<QosGlobalDeleteDscpToCosMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
	trace(TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpToCosMapReferenceInFailedOverNodes");

	string dscpToCoSMapName =
			pQosGlobalDeleteDscpToCosMapMessage->getDscpToCosMapName();
	NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
	pMsg = new NsmGlobalQosMapCleanUpMessage(dscpToCoSMapName,
			QOS_INTERFACE_DSCP_COS);
	status = sendSynchronously(pMsg);
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpToCosMapReferenceInFailedOverNodes status ="
					+ status);
	delete (pMsg);
	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void QosGlobalWorker::cleanupDscpMutationMapReferenceInFailedOverNodes(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage =
			dynamic_cast<QosGlobalDeleteDscpToDscpMutationMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpMutationMapReferenceInFailedOverNodes");

	string dscpMutationMapName =
			pQosGlobalDeleteDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName();
	NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
	pMsg = new NsmGlobalQosMapCleanUpMessage(dscpMutationMapName,
			QOS_INTERFACE_DSCP_MUTATION);
	status = sendSynchronously(pMsg);
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpMutationMapReferenceInFailedOverNodes status ="
					+ status);
	delete (pMsg);
	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}

void QosGlobalWorker::cleanupDscpToTrafficClassMapReferenceInFailedOverNodes(
		PrismLinearSequencerContext *pPrismLinearSequencerContext) {

	QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage =
			dynamic_cast<QosGlobalDeleteDscpToTrafficClassMapMessage*>(pPrismLinearSequencerContext->getPPrismMessage());
	ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpToTrafficClassMapReferenceInFailedOverNodes");

	string dscpToTrafficClassMapName =
			pQosGlobalDeleteDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName();
	NsmGlobalQosMapCleanUpMessage *pMsg = NULL;
	pMsg = new NsmGlobalQosMapCleanUpMessage(dscpToTrafficClassMapName,
			QOS_INTERFACE_DSCP_TRAFFIC_CLASS);
	status = sendSynchronously(pMsg);
	trace(
			TRACE_LEVEL_DEBUG,
			"QosGlobalWorker::cleanupDscpToTrafficClassMapReferenceInFailedOverNodes status ="
					+ status);
	delete (pMsg);
	pPrismLinearSequencerContext->executeNextStep(status);
	return;
}



void  QosGlobalWorker::QosGlobalDeleteCosToCosMutationMapMessageHandler( QosGlobalDeleteCosToCosMutationMapMessage
	*pQosGlobalDeleteCosToCosMutationMapMessage)
{
  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToCosMutationMapValidationStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToCosMutationMapStep),
    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::appmQosProfileMutationUpdateStep),
    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::cleanupCosMutationMapReferenceInFailedOverNodes),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToCosMutationMapMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteCosToCosMutationMapMessage,
	  this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::deleteCosToTrafficClassMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage =
    dynamic_cast<QosGlobalDeleteCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

  ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
  ObjectId tcMapObjectId ;
  UI32 poCount = 0;
  UI32 phyCount = 0;
  string tcMapName = pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName();
  status = QosUtils::getQosMapObjectIdByMapName (tcMapName, tcMapObjectId, COS_TRAFFIC_CLASS_MAP, NULL);

  if (status != WAVE_MESSAGE_SUCCESS)
  {
    trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName tcMap failed"));
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    return;
  }

  /* Check if map is activated in Port channel/Physical interface. If yes, throw error */
  WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
  poSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(tcMapObjectId, "qosTrafficClassMap", PoIntfManagedObject::getClassName()));
  status = querySynchronouslyForCount(&poSyncQueryCtxt, poCount);


  WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
  phyIntfSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(tcMapObjectId, "qosTrafficClassMap", PhyIntfLocalManagedObject::getClassName()));
  phyIntfSyncQueryCtxt.setFilterByConnectedNodes();
  status = querySynchronouslyForCount(&phyIntfSyncQueryCtxt, phyCount);

  if (status != WAVE_MESSAGE_SUCCESS)
  {
    trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosTrafficClassMap failed"));
    pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
    return;
  }
  else
  {
    if ((0 != poCount) || (0 != phyCount)) {
      pPrismLinearSequencerContext->executeNextStep(WRC_SSM_QOS_MAP_IS_STILL_ACTIVATED);
      return;
    }
  }
  pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void QosGlobalWorker::deleteCosToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage =
	dynamic_cast<QosGlobalDeleteCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
  WaveSendToClusterContext    *send_ctx_p;

  QosDeleteCosToTrafficClassMapSPMessage *m = new QosDeleteCosToTrafficClassMapSPMessage();

  m->setCosToTrafficClassMapName(pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName());

	send_ctx_p = new WaveSendToClusterContext(this,
		reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName());
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
}

void QosGlobalWorker::deleteCosToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage = dynamic_cast<QosGlobalDeleteCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

  string cosToTrafficClassMapName = pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName();
  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(CosToTrafficClassMapManagedObject::getClassName());
  syncQueryCtxt.addAndAttribute (new AttributeString(cosToTrafficClassMapName,"cosToTrafficClassMapName"));
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  CosToTrafficClassMapManagedObject *pCosToTrafficClassMapManagedObject = NULL;

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToTrafficClassMapMOStep: MO not found");
	} else if (1 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToTrafficClassMapMOStep: MO found");
	  pCosToTrafficClassMapManagedObject = dynamic_cast<CosToTrafficClassMapManagedObject*>((*pResults)[0]);
	} else {
	  trace (TRACE_LEVEL_ERROR, "deleteCosToTrafficClassMapMOStep: More than one MO found");
	}
  }

	prismAssert (NULL != pCosToTrafficClassMapManagedObject, __FILE__, __LINE__);

	trace (TRACE_LEVEL_INFO, string ("\tmapname:: ") + pCosToTrafficClassMapManagedObject->getCosToTrafficClassMapName());
	delete pCosToTrafficClassMapManagedObject;

    if (NULL != pResults) {
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void QosGlobalWorker::appmQosProfileTrafficClassUpdateStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
    QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage = dynamic_cast<QosGlobalDeleteCosToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

    string cosToTrafficClassMapName = pQosGlobalDeleteCosToTrafficClassMapMessage->getCosToTrafficClassMapName();
    ResourceId  AppmStatus   = WAVE_MESSAGE_ERROR;
    APPMGlobalAPPMQosMessage *AppmQosProfileMsg = NULL;

    AppmQosProfileMsg = new APPMGlobalAPPMQosMessage();

    if (!AppmQosProfileMsg)
    {
        pPrismLinearSequencerContext->executeNextStep(AppmStatus);
        return;
    }

    /*
     * Sending the message to APPM Plugin.
     */

    AppmQosProfileMsg->setOpCode(APPM_QOS_MSG);
    AppmQosProfileMsg->setCmdCode(QOS_NO_COS_TO_TRAFFIC_MAP);
    AppmQosProfileMsg->setCosToTrafficClassMap(cosToTrafficClassMapName);
    AppmStatus = sendSynchronously(AppmQosProfileMsg);
    delete AppmQosProfileMsg;

    pPrismLinearSequencerContext->executeNextStep(AppmStatus);
    return;
}

void  QosGlobalWorker::QosGlobalDeleteCosToTrafficClassMapMessageHandler( QosGlobalDeleteCosToTrafficClassMapMessage *pQosGlobalDeleteCosToTrafficClassMapMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToTrafficClassMapValidationStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToTrafficClassMapStep),
    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::appmQosProfileTrafficClassUpdateStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::cleanupCosToTrafficClassMapReferenceInFailedOverNodes),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteCosToTrafficClassMapMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteCosToTrafficClassMapMessage,
	  this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::deleteMcastSchedulerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  WaveSendToClusterContext    *send_ctx_p;

  QosDeleteQosQueueMulticastSchedulerSPMessage *m = new QosDeleteQosQueueMulticastSchedulerSPMessage();

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosDeleteQosQueueMulticastSchedulerSPMessage"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;

}

void QosGlobalWorker::deleteMcastSchedulerMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueMulticastSchedulerManagedObject::getClassName());
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  QosQueueMulticastSchedulerManagedObject *pQosQueueMulticastSchedulerManagedObject = NULL;

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteMcastSchedulerMOStep: MO not found");
	} else if (1 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteMcastSchedulerMOStep: MO found");
	  pQosQueueMulticastSchedulerManagedObject = dynamic_cast<QosQueueMulticastSchedulerManagedObject*>((*pResults)[0]);
	} else {
	  trace (TRACE_LEVEL_ERROR, "deleteMcastSchedulerMOStep: More than one MO found");
	}
  }

	prismAssert (NULL != pQosQueueMulticastSchedulerManagedObject, __FILE__, __LINE__);
	delete pQosQueueMulticastSchedulerManagedObject;

    if (NULL != pResults) {
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void  QosGlobalWorker::QosGlobalDeleteQosQueueMulticastSchedulerMessageHandler( QosGlobalDeleteQosQueueMulticastSchedulerMessage *pQosGlobalDeleteQosQueueMulticastSchedulerMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteMcastSchedulerStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteMcastSchedulerMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteQosQueueMulticastSchedulerMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::deleteSchedulerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  WaveSendToClusterContext    *send_ctx_p;

  QosDeleteQosQueueSchedulerSPMessage *m = new QosDeleteQosQueueSchedulerSPMessage();

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosDeleteQosQueueSchedulerSPMessage"));
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
}

void QosGlobalWorker::deleteSchedulerMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{

  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosQueueSchedulerManagedObject::getClassName());
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  QosQueueSchedulerManagedObject *pQosQueueSchedulerManagedObject = NULL;

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteSchedulerMOStep: MO not found");
	} else if (1 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteSchedulerMOStep: MO found");
	  pQosQueueSchedulerManagedObject = dynamic_cast<QosQueueSchedulerManagedObject*>((*pResults)[0]);
	} else {
	  trace (TRACE_LEVEL_ERROR, "deleteSchedulerMOStep: More than one MO found");
	}
  }

	prismAssert (NULL != pQosQueueSchedulerManagedObject, __FILE__, __LINE__);
	delete pQosQueueSchedulerManagedObject;

    if (NULL != pResults) {
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void  QosGlobalWorker::QosGlobalDeleteQosQueueSchedulerMessageHandler( QosGlobalDeleteQosQueueSchedulerMessage *pQosGlobalDeleteQosQueueSchedulerMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteSchedulerStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteSchedulerMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteQosQueueSchedulerMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::deleteMCastSettingStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  QosGlobalDeleteQosRcvQueueMulticastSettingMessage
	*pQosGlobalDeleteQosRcvQueueMulticastSettingMessage =
	dynamic_cast<QosGlobalDeleteQosRcvQueueMulticastSettingMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
  WaveSendToClusterContext    *send_ctx_p;

  QosDeleteQosRcvQueueMulticastSettingSPMessage *m = new
	QosDeleteQosRcvQueueMulticastSettingSPMessage();

  m->setOpCode(pQosGlobalDeleteQosRcvQueueMulticastSettingMessage->getOpCode());

	send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
    prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
    send_ctx_p->setPPrismMessageForPhase1(m);
    trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :QosDeleteQosRcvQueueMulticastSettingSPMessage") + pQosGlobalDeleteQosRcvQueueMulticastSettingMessage->getOpCode()); 
	send_ctx_p->setPartialSuccessFlag(true);
	send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
    sendToWaveCluster(send_ctx_p);
    return ;
}

void QosGlobalWorker::deleteMCastSettingMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
{
  SI32 defaultThreshold = DEFAULT_THRESHOLD;
  SI32 defaultRateLimit = DEFAULT_RATE_LIMIT;
  SI32 defaultBurst = DEFAULT_BURST;
  QosGlobalDeleteQosRcvQueueMulticastSettingMessage
	*pQosGlobalDeleteQosRcvQueueMulticastSettingMessage =
	dynamic_cast<QosGlobalDeleteQosRcvQueueMulticastSettingMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

  SI32 opCode= pQosGlobalDeleteQosRcvQueueMulticastSettingMessage->getOpCode();
  WaveManagedObjectSynchronousQueryContext syncQueryCtxt(QosRcvQueueMulticastSettingManagedObject::getClassName());
  vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
  QosRcvQueueMulticastSettingManagedObject *pQosRcvQueueMulticastSettingManagedObject= NULL;

  if (NULL != pResults) {
	UI32 numberOfResults = pResults->size ();
	if (0 == numberOfResults) {
	  trace (TRACE_LEVEL_ERROR, "deleteMCastSettingMOStep: MO not found");
	} else if (1 == numberOfResults) {
	  trace (TRACE_LEVEL_DEBUG, "deleteMCastSettingMOStep: MO found");
	  pQosRcvQueueMulticastSettingManagedObject =
		dynamic_cast<QosRcvQueueMulticastSettingManagedObject*>((*pResults)[0]);
	} else {
	  trace (TRACE_LEVEL_ERROR, "deleteMCastSettingMOStep: More than one MO found");
	}
  }

	prismAssert (NULL != pQosRcvQueueMulticastSettingManagedObject, __FILE__, __LINE__);
	trace (TRACE_LEVEL_INFO, string ("Setting MO to default.Opcode=") + opCode);
	updateWaveManagedObject (pQosRcvQueueMulticastSettingManagedObject);
	switch (opCode)
	{
	  case MCAST_THRESHOLD:
		trace (TRACE_LEVEL_DEBUG, "deleteMCastSettingMOStep: case 0");
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass0(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass1(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass2(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass3(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass4(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass5(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass6(defaultThreshold);
		pQosRcvQueueMulticastSettingManagedObject->setThresholdTrafficClass7(defaultThreshold);
		break;
	  case MCAST_RATELIMIT:
		trace (TRACE_LEVEL_DEBUG, "deleteMCastSettingMOStep: case 1");
		pQosRcvQueueMulticastSettingManagedObject->setRateLimit(defaultRateLimit);
		pQosRcvQueueMulticastSettingManagedObject->setBurst(defaultBurst);
		break;
	}

    if (NULL != pResults) {
        pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
    }

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

void  QosGlobalWorker::QosGlobalDeleteQosRcvQueueMulticastSettingMessageHandler( QosGlobalDeleteQosRcvQueueMulticastSettingMessage
	*pQosGlobalDeleteQosRcvQueueMulticastSettingMessage)
{

  PrismLinearSequencerStep sequencerSteps[] =
  {
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteMCastSettingStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteMCastSettingMOStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
  };

  PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteQosRcvQueueMulticastSettingMessage,
	  this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

  pPrismLinearSequencerContext->holdAll();
  pPrismLinearSequencerContext->start();
}

void QosGlobalWorker::QosGlobalSendToClusterCallback(WaveSendToClusterContext  *ctx_p) {
    
    trace (TRACE_LEVEL_DEBUG, "Inside QosGlobalSendToClusterCallback");
    ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
    PrismLinearSequencerContext             *seq_ctx_p;
    vector<LocationId>                      locationIdsInCluster;
    seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
    
    if ( WAVE_MESSAGE_SUCCESS == sendToClusterCompletionStatus ) {
        trace (TRACE_LEVEL_INFO, "QoS Global Command succeeded on all the nodes.");
    } else {
        trace (TRACE_LEVEL_INFO, "QoS Global Command failed on few or all the nodes.");
        locationIdsInCluster = ctx_p->getLocationsToSendToForPhase1();
        
        for (UI32 i = 0; i < locationIdsInCluster.size(); i++) {
            ResourceId statusPerLocationId = ClusterGetPhase1StatusFromContextForLocation(ctx_p, locationIdsInCluster[i]);
            if(WAVE_MESSAGE_SUCCESS != statusPerLocationId) {
                trace(TRACE_LEVEL_ERROR, string("QoS config command failed on the node<")
                        + locationIdsInCluster[i] + ">, with an error: " + FrameworkToolKit::localize(statusPerLocationId));
            }
        }
    }
    
    delete ctx_p->getPPrismMessageForPhase1();
    delete ctx_p->getPPrismMessageForPhase2();
    delete ctx_p;
    trace (TRACE_LEVEL_DEBUG, string("QosGlobalSendToClusterCallback:") + sendToClusterCompletionStatus);
    seq_ctx_p->executeNextStep(sendToClusterCompletionStatus);
    return;
}

    void  QosGlobalWorker::QosGlobalCreateDscpMarkListMessageHandler( QosGlobalCreateDscpMarkListMessage *pQosGlobalCreateDscpMarkListMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpMarkListStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpMarkListMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateDscpMarkListMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalCreateDscpToCosMapMessageHandler( QosGlobalCreateDscpToCosMapMessage *pQosGlobalCreateDscpToCosMapMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToCosMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToCosMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateDscpToCosMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalCreateDscpToDscpMutationMapMessageHandler( QosGlobalCreateDscpToDscpMutationMapMessage *pQosGlobalCreateDscpToDscpMutationMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToDscpMutationMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToDscpMutationMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateDscpToDscpMutationMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalCreateDscpToTrafficClassMapMessageHandler( QosGlobalCreateDscpToTrafficClassMapMessage *pQosGlobalCreateDscpToTrafficClassMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToTrafficClassMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createDscpToTrafficClassMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateDscpToTrafficClassMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalCreateRedProfileMessageHandler( QosGlobalCreateRedProfileMessage *pQosGlobalCreateRedProfileMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createRedProfileStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::createRedProfileMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalCreateRedProfileMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalDeleteDscpMarkListMessageHandler( QosGlobalDeleteDscpMarkListMessage *pQosGlobalDeleteDscpMarkListMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpMarkListStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpMarkListMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteDscpMarkListMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalDeleteDscpToCosMapMessageHandler( QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToCosMapValidationStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToCosMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
        	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::cleanupDscpToCosMapReferenceInFailedOverNodes),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToCosMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteDscpToCosMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalDeleteDscpToDscpMutationMapMessageHandler( QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToDscpMutationMapValidationStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToDscpMutationMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
        	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::cleanupDscpMutationMapReferenceInFailedOverNodes),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToDscpMutationMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteDscpToDscpMutationMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalDeleteDscpToTrafficClassMapMessageHandler( QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage)
    {
// Validations and Queries should go here

        PrismLinearSequencerStep sequencerSteps[] =
        {
		    reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToTrafficClassMapValidationStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToTrafficClassMapStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
        	reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::cleanupDscpToTrafficClassMapReferenceInFailedOverNodes),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteDscpToTrafficClassMapMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteDscpToTrafficClassMapMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void  QosGlobalWorker::QosGlobalDeleteRedProfileMessageHandler( QosGlobalDeleteRedProfileMessage *pQosGlobalDeleteRedProfileMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteRedProfileStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerStartTransactionStep),
			reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::deleteRedProfileMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalDeleteRedProfileMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

	void QosGlobalWorker::createDscpMarkListStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalCreateDscpMarkListMessage *pQosGlobalCreateDscpMarkListMessage = dynamic_cast<QosGlobalCreateDscpMarkListMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext *send_ctx_p;
		QosCreateDscpMarkListSPMessage *m = new QosCreateDscpMarkListSPMessage();

		trace(TRACE_LEVEL_DEBUG, string("input markList: ") + pQosGlobalCreateDscpMarkListMessage->getMarkList() + string(" input mapName: ") + pQosGlobalCreateDscpMarkListMessage->getDscpMapName() + string("toValue: ") + pQosGlobalCreateDscpMarkListMessage->getToValue() + string("mapType: ") + pQosGlobalCreateDscpMarkListMessage->getQosMapType());
		m->setMarkList(pQosGlobalCreateDscpMarkListMessage->getMarkList());
		m->setDscpMapName(pQosGlobalCreateDscpMarkListMessage->getDscpMapName());
		m->setQosMapType(pQosGlobalCreateDscpMarkListMessage->getQosMapType());
		m->setToValue(pQosGlobalCreateDscpMarkListMessage->getToValue());
		trace(TRACE_LEVEL_DEBUG, string("msg markList: ") + m->getMarkList() + string("msg mapName: ") + m->getDscpMapName() + string("toValue: ") + m->getToValue() + string("mapType: ") + m->getQosMapType());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster mark:") + pQosGlobalCreateDscpMarkListMessage->getMarkList() + string(" mapName: ") + pQosGlobalCreateDscpMarkListMessage->getDscpMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::createDscpMarkListMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		//bool createFlag = false;
		QosGlobalCreateDscpMarkListMessage *pQosGlobalCreateDscpMarkListMessage = dynamic_cast<QosGlobalCreateDscpMarkListMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		DscpMarkListManagedObject *pDscpMarkListManagedObject = NULL;
		DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
		DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
		DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
		string dscpMarkList = pQosGlobalCreateDscpMarkListMessage->getMarkList();
		string dscpMapName = pQosGlobalCreateDscpMarkListMessage->getDscpMapName();
		UI8	qosMapType = pQosGlobalCreateDscpMarkListMessage->getQosMapType();
		SI32 toValue = pQosGlobalCreateDscpMarkListMessage->getToValue();
		string className, mapStringName;
		vector<WaveManagedObjectPointer<DscpMarkListManagedObject> > markListVector;
		ObjectId markListObjectId;

		switch (qosMapType) {
			case DSCP_MUTATION_MAP:
				className = DscpToDscpMutationMapManagedObject::getClassName();
				mapStringName = "dscpToDscpMutationMapName";
				break;
			case DSCP_TRAFFIC_CLASS_MAP:
				className = DscpToTrafficClassMapManagedObject::getClassName();
				mapStringName = "dscpToTrafficClassMapName";
				break;
			case DSCP_COS_MAP:
				className = DscpToCosMapManagedObject::getClassName();
				mapStringName = "dscpToCosMapName";
				break;
			default:
				prismAssert(0, __FILE__, __LINE__);
				break;
		}

		trace(TRACE_LEVEL_DEBUG, string("QUerying for mapType: ") + className);
		
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(className);
		syncQueryCtxt.addAndAttribute (new AttributeString(&dscpMapName, mapStringName));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		/* The above query will return the map for which a new markString is entered.
		 * We have to check whether the map contains this markString or not.
		 */
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				/* Well, the map itself is not found, this cannot happen. Assert for now! */
				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
				prismAssert(0, __FILE__, __LINE__);
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
				/* The Map is present in the database. Get the list of markStrings associated
				* with it and find the markString in it. Retrieve the markList based on 
				* map type.
				*/
				switch (qosMapType) {
					case DSCP_MUTATION_MAP:
						pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
						markListVector = pDscpToDscpMutationMapManagedObject->getDscpMarkList();
						break;
					case DSCP_TRAFFIC_CLASS_MAP:
						pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
						markListVector = pDscpToTrafficClassMapManagedObject->getDscpMarkList();
						break;
					case DSCP_COS_MAP:
						pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
						markListVector = pDscpToCosMapManagedObject->getDscpMarkList();
						break;
				}

				trace(TRACE_LEVEL_DEBUG, string("size of marklist vector: ") + markListVector.size());

				bool matchFound = false, toMatchFound = false;
				uint32_t i;
				for (i=0; i<markListVector.size(); i++) {
					string markListExisting = markListVector[i]->getMarkList();
					DscpBitMap bmap1, bmap2;
					vector <UI32> vector1, vector2;
					SI32 toValueExisting;
					bmap1.reset();bmap2.reset();
					UI32Range(markListExisting).getUI32RangeVector(vector1);
					UI32Range(dscpMarkList).getUI32RangeVector(vector2);
					bmap1.setValuesFromVector(vector1);
					bmap2.setValuesFromVector(vector2);

					/* The above exercise is to normalize strings before comparing them so that 
					 * strings such as 4-6; 4,5,6; 6,5,4 etc. all map to same thing.
					 */
					if (bmap1 == bmap2) {
						matchFound = true;
						toValueExisting = markListVector[i]->getToValue();
						if (toValue == toValueExisting) {
							/* this is a no-op. Nothing needs to be done */
							break;
						} else {
							/* only the toValue needs to be updated. Nothing else needs to be done. */
						    startTransaction();
							markListVector[i]->setToValue(toValue);
							updateWaveManagedObject((markListVector[i]).operator ->());
							updateWaveManagedObject(dynamic_cast<WaveManagedObject*> ((*pResults)[0]));
							commitTransaction();
							break;
						}
					}
				}

				if (matchFound) {
					trace(TRACE_LEVEL_DEBUG, string("markList vector found in the list."));
					/* TODO setting status etc.? */
					goto exit;
				} else {
					/* A match is not found. We have to loop through all the markList elements
					* of this map and see if the old elements have to be modified or if a 
					* new element needs to be added to the list.
					*/
					trace(TRACE_LEVEL_DEBUG, string("markList vector not found in the list."));

					/* NAGP DEBUG....
					vector <UI32> dummyVector;
					string dummyString;
					 NAGP DEBUG.... */

					string markListOld, markListNew = dscpMarkList;
					SI32 toValueOld, toValueNew = toValue;
					DscpBitMap	dscpBmapNew(64);
					dscpBmapNew.reset();
					UI32Range dscpRangeNew = UI32Range(markListNew);
					vector <UI32> dscpRangeVectorNew;
					dscpRangeNew.getUI32RangeVector(dscpRangeVectorNew);
					dscpBmapNew.setValuesFromVector(dscpRangeVectorNew);

					/* NAGP DEBUG....
					dscpBmapNew.getValuesInVector(dummyVector); dummyString = dscpRangeNew.getUI32RangeStringFromVector(dummyVector);
					trace(TRACE_LEVEL_DEBUG, string("new vector is: ") + dummyString);
					 NAGP DEBUG.... */

					for (i=0; i<markListVector.size(); i++) {
						markListOld = markListVector[i]->getMarkList();
						toValueOld = markListVector[i]->getToValue();

						trace(TRACE_LEVEL_DEBUG, string("InLoop: markList at index ") + i + string(" is: ") + markListOld);

						DscpBitMap  dscpBmapOld(64), dscpBmapOldCopy(64);
						vector <UI32> dscpRangeVector;
						dscpBmapOld.reset();
						dscpBmapOldCopy.reset();
						UI32Range dscpRangeOld = UI32Range(markListOld);
						dscpRangeOld.getUI32RangeVector(dscpRangeVector);
						trace(TRACE_LEVEL_DEBUG, string("RangeVector size loaded to bitmap is: ") + dscpRangeVector.size());
						dscpBmapOld.setValuesFromVector(dscpRangeVector);
						dscpBmapOldCopy.setValuesFromVector(dscpRangeVector);

						/* Get the common bits */
						DscpBitMap dscpBmapAnd(dscpBmapNew);
						dscpBmapAnd.andWith(dscpBmapOld);
						
						/* NAGP DEBUG....
						dscpBmapAnd.getValuesInVector(dummyVector); dummyString = dscpRangeOld.getUI32RangeStringFromVector(dummyVector);
						trace(TRACE_LEVEL_DEBUG, string("and vector is: ") + dummyString);
						 NAGP DEBUG.... */

						/* Are the to values different, if so take out the common bits from old map */
						if (toValueOld != toValueNew) {
							if (!dscpBmapAnd.isBmapEmpty()) {
								trace(TRACE_LEVEL_DEBUG, "common entries found between" + markListOld + "and " + markListNew);
								dscpBmapOld.negateWith(dscpBmapAnd);
								
								/* NAGP DEBUG....
								dscpBmapOld.getValuesInVector(dummyVector); dummyString = dscpRangeOld.getUI32RangeStringFromVector(dummyVector);
								trace(TRACE_LEVEL_DEBUG, string("negated vector is: ") + dummyString);
								 NAGP DEBUG.... */

							} else {
								trace(TRACE_LEVEL_DEBUG, "No common entries found between" + markListOld + "and " + markListNew);
							}
						} else {
							dscpBmapOld.orWith(dscpBmapNew);
							toMatchFound = true;

							/* NAGP DEBUG....
							dscpBmapOld.getValuesInVector(dummyVector); dummyString = dscpRangeOld.getUI32RangeStringFromVector(dummyVector);
							trace(TRACE_LEVEL_DEBUG, string("or vector is: ") + dummyString);
							 NAGP DEBUG.... */
						}

						if (dscpBmapOldCopy != dscpBmapOld) {
							if (dscpBmapOld.isBmapEmpty()) {
								trace(TRACE_LEVEL_DEBUG, string("markList ") + markListOld + string(" is going to be deleted"));
								markListObjectId = ObjectId::NullObjectId;
							   	startTransaction();
								/* delete the markList from the vector, switch through MOs. */
								switch (qosMapType) {
									case DSCP_MUTATION_MAP:
										pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
										markListObjectId = pDscpToDscpMutationMapManagedObject->getDscpMarkListObjectId(markListOld);
										if (markListObjectId != ObjectId::NullObjectId) {
											deleteFromComposition(DscpToDscpMutationMapManagedObject::getClassName(),
												DscpMarkListManagedObject::getClassName(),
											"dscpMarkList", pDscpToDscpMutationMapManagedObject->getObjectId(), markListObjectId);
										} else {
											trace(TRACE_LEVEL_ERROR, string("ObjectId not available for marklist") + markListOld);
										}
										break;
									case DSCP_TRAFFIC_CLASS_MAP:
										pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
										markListObjectId = pDscpToTrafficClassMapManagedObject->getDscpMarkListObjectId(markListOld);
										if (markListObjectId != ObjectId::NullObjectId) {
											deleteFromComposition(DscpToTrafficClassMapManagedObject::getClassName(),
												DscpMarkListManagedObject::getClassName(),
											"dscpMarkList", pDscpToTrafficClassMapManagedObject->getObjectId(), markListObjectId);
										} else {
											trace(TRACE_LEVEL_ERROR, string("ObjectId not available for marklist") + markListOld);
										}
										break;
									case DSCP_COS_MAP:
										pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
										markListObjectId = pDscpToCosMapManagedObject->getDscpMarkListObjectId(markListOld);
										if (markListObjectId != ObjectId::NullObjectId) {
											deleteFromComposition(DscpToCosMapManagedObject::getClassName(),DscpMarkListManagedObject::getClassName(),
											"dscpMarkList", pDscpToCosMapManagedObject->getObjectId(), markListObjectId);
										} else {
											trace(TRACE_LEVEL_ERROR, string("ObjectId not available for marklist") + markListOld);
										}
										break;
								}
								commitTransaction();

								/* 'markListVector' used in this API is not a
								 * reference. It is a copy of data in the MO.
								 * Because of this, calling 'delDscpMarkList' inside a
								 * transaction wont delete the DB entry.
								 * (Framework maintains count of number of
								 * objects.Since there are 2 entries, one in
								 * markListVector and the other in the
								 * ManagedObject, it is not deleted from DB)
								 * Hence, using both deleteFromComposition and
								 * delDscpMarkList.
								 */
								switch (qosMapType) {
									case DSCP_MUTATION_MAP:
									  	pDscpToDscpMutationMapManagedObject->delDscpMarkList(markListOld);	
										break;
									case DSCP_TRAFFIC_CLASS_MAP:
										pDscpToTrafficClassMapManagedObject->delDscpMarkList(markListOld);
										break;
									case DSCP_COS_MAP:
										pDscpToCosMapManagedObject->delDscpMarkList(markListOld);
										break;
								}

							} else {
								trace(TRACE_LEVEL_DEBUG, string("markList ") + markListOld + string(" is going to be updated"));
								dscpBmapOld.getValuesInVector(dscpRangeVector);
								markListOld = dscpRangeOld.getUI32RangeStringFromVector(dscpRangeVector);
								trace(TRACE_LEVEL_DEBUG, string("markList is updated as ") + markListOld);
								/* Update the MarkList */
								startTransaction();
								markListVector[i]->setMarkList(markListOld);
								trace(TRACE_LEVEL_DEBUG, "Updating the modified object..");
								updateWaveManagedObject((markListVector[i]).operator ->());
								updateWaveManagedObject(dynamic_cast<WaveManagedObject*> ((*pResults)[0]));
								commitTransaction();
							}
						}
					}

					if (!dscpBmapNew.isBmapEmpty() && !toMatchFound) {
						//createFlag = true;
						/* Add new markListMO with markList and toValueNew. Switch through the MOs. */
						trace (TRACE_LEVEL_DEBUG, "create a new MarkList Object, markList: " + dscpMarkList + string(" toValue: ") + toValueNew);
						startTransaction();
						pDscpMarkListManagedObject = new DscpMarkListManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()),
						dscpMarkList, toValueNew);
						prismAssert (NULL != pDscpMarkListManagedObject, __FILE__, __LINE__);

						trace(TRACE_LEVEL_DEBUG, string("adding markList MO to ") + className); 
						switch (qosMapType) {
							case DSCP_MUTATION_MAP:
								pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
								pDscpToDscpMutationMapManagedObject->addDscpMarkList(pDscpMarkListManagedObject);
								break;
							case DSCP_TRAFFIC_CLASS_MAP:
								pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
								pDscpToTrafficClassMapManagedObject->addDscpMarkList(pDscpMarkListManagedObject);
								break;
							case DSCP_COS_MAP:
								pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
								pDscpToCosMapManagedObject->addDscpMarkList(pDscpMarkListManagedObject);
								break;
						}
						updateWaveManagedObject(dynamic_cast<WaveManagedObject*> ((*pResults)[0]));
						commitTransaction();
					}
				}
			}
		}

exit:
	/* No further updates now. Any marks will be done as part of its corresponding MO */

	// Mark the MO for GC
	trace(TRACE_LEVEL_DEBUG, "Marking pResult objects for GC.");
	if (pResults) {
		pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
	}

	if (pResults) {
		trace(TRACE_LEVEL_DEBUG, "Cleaning up pResults..");
		delete pResults;
	}

	pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
}

	void QosGlobalWorker::createDscpToCosMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalCreateDscpToCosMapMessage *pQosGlobalCreateDscpToCosMapMessage = dynamic_cast<QosGlobalCreateDscpToCosMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext *send_ctx_p;
		QosCreateDscpToCosMapSPMessage *m = new QosCreateDscpToCosMapSPMessage();

		trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pQosGlobalCreateDscpToCosMapMessage->getDscpToCosMapName());
		m->setDscpCosMapName(pQosGlobalCreateDscpToCosMapMessage->getDscpToCosMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateDscpToCosMapMessage->getDscpToCosMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::createDscpToCosMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		QosGlobalCreateDscpToCosMapMessage *pQosGlobalCreateDscpToCosMapMessage = dynamic_cast<QosGlobalCreateDscpToCosMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
		string dscpCosMapName = pQosGlobalCreateDscpToCosMapMessage->getDscpToCosMapName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&dscpCosMapName, "dscpToCosMapName"));
        syncQueryCtxt.addSelectField("dscpToCosMapName");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
		  UI32 numberOfResults = pResults->size ();
		  if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
			createFlag = 1;
		  } else if (1 == numberOfResults) {
			pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
			createFlag = 0;
		  }
		}

		if (createFlag == 0) {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
		  updateWaveManagedObject (pDscpToCosMapManagedObject);
		  prismAssert (NULL != pQosGlobalCreateDscpToCosMapMessage, __FILE__, __LINE__);
		} else {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
		  pDscpToCosMapManagedObject = new DscpToCosMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
		  prismAssert (NULL != pDscpToCosMapManagedObject, __FILE__, __LINE__);
		}

		pDscpToCosMapManagedObject->setDscpToCosMapName(pQosGlobalCreateDscpToCosMapMessage->getDscpToCosMapName());
        pDscpToCosMapManagedObject->addAttributeToBeUpdated("dscpToCosMapName");

		/* No further updates now. Any marks will be done as part of its corresponding MO */

		if (createFlag && pDscpToCosMapManagedObject) {
			// Mark the MO for GC
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToCosMapManagedObject);
		}

		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		}


		if (pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::createDscpToDscpMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalCreateDscpToDscpMutationMapMessage *pQosGlobalCreateDscpToDscpMutationMapMessage = dynamic_cast<QosGlobalCreateDscpToDscpMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext *send_ctx_p;
		QosCreateDscpToDscpMutationMapSPMessage *m = new QosCreateDscpToDscpMutationMapSPMessage();

		trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pQosGlobalCreateDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());
		m->setDscpMutationMapName(pQosGlobalCreateDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::createDscpToDscpMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		QosGlobalCreateDscpToDscpMutationMapMessage *pQosGlobalCreateDscpToDscpMutationMapMessage = dynamic_cast<QosGlobalCreateDscpToDscpMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
		string dscpMutationMapName = pQosGlobalCreateDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&dscpMutationMapName, "dscpToDscpMutationMapName"));
        syncQueryCtxt.addSelectField("dscpToDscpMutationMapName");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		trace(TRACE_LEVEL_DEBUG, string("QosGlobalWorker::createDscpToDscpMutationMapMOStep :- Entering..."));
		if (NULL != pResults) {
		  UI32 numberOfResults = pResults->size ();
		  if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
			createFlag = 1;
		  } else if (1 == numberOfResults) {
			pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
			createFlag = 0;
		  }
		}

		if (createFlag == 0) {
		  trace (TRACE_LEVEL_DEBUG, "createFlag = 0");
		  updateWaveManagedObject (pDscpToDscpMutationMapManagedObject);
		  prismAssert (NULL != pQosGlobalCreateDscpToDscpMutationMapMessage, __FILE__, __LINE__);
		} else {
		  trace (TRACE_LEVEL_DEBUG, "createFlag = 1");
		  pDscpToDscpMutationMapManagedObject = new DscpToDscpMutationMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
		  prismAssert (NULL != pDscpToDscpMutationMapManagedObject, __FILE__, __LINE__);
		}

		pDscpToDscpMutationMapManagedObject->setDscpToDscpMutationMapName(pQosGlobalCreateDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());
        pDscpToDscpMutationMapManagedObject->addAttributeToBeUpdated("dscpToDscpMutationMapName");

		/* No further updates now. Any marks will be done as part of its corresponding MO */

		if (createFlag && pDscpToDscpMutationMapManagedObject) {
			// Mark the MO for GC
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToDscpMutationMapManagedObject);
		}

		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		}

		if (pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::createDscpToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalCreateDscpToTrafficClassMapMessage *pQosGlobalCreateDscpToTrafficClassMapMessage = dynamic_cast<QosGlobalCreateDscpToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext *send_ctx_p;
		QosCreateDscpToTrafficClassMapSPMessage *m = new QosCreateDscpToTrafficClassMapSPMessage();

		trace(TRACE_LEVEL_DEBUG, string("input mapName ") + pQosGlobalCreateDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());
		m->setDscpTrafficClassMapName(pQosGlobalCreateDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::createDscpToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		QosGlobalCreateDscpToTrafficClassMapMessage *pQosGlobalCreateDscpToTrafficClassMapMessage = dynamic_cast<QosGlobalCreateDscpToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
		string dscpToTrafficClassMapName = pQosGlobalCreateDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&dscpToTrafficClassMapName, "dscpToTrafficClassMapName"));
        syncQueryCtxt.addSelectField("dscpToTrafficClassMapName");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
		  UI32 numberOfResults = pResults->size ();
		  if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
			createFlag = 1;
		  } else if (1 == numberOfResults) {
			pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
			createFlag = 0;
		  }
		}

		if (createFlag == 0) {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
		  updateWaveManagedObject (pDscpToTrafficClassMapManagedObject);
		  prismAssert (NULL != pQosGlobalCreateDscpToTrafficClassMapMessage, __FILE__, __LINE__);
		} else {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
		  pDscpToTrafficClassMapManagedObject = new DscpToTrafficClassMapManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
		  prismAssert (NULL != pDscpToTrafficClassMapManagedObject, __FILE__, __LINE__);
		}

		pDscpToTrafficClassMapManagedObject->setDscpToTrafficClassMapName(pQosGlobalCreateDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());
        pDscpToTrafficClassMapManagedObject->addAttributeToBeUpdated("dscpToTrafficClassMapName");

		/* No further updates now. Any marks will be done as part of its corresponding MO */

		if (createFlag && pDscpToTrafficClassMapManagedObject) {
			// Mark the MO for GC
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToTrafficClassMapManagedObject);
		}

		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		}


		if (pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::createRedProfileStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalCreateRedProfileMessage *pQosGlobalCreateRedProfileMessage = dynamic_cast<QosGlobalCreateRedProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext *send_ctx_p;
		QosCreateRedProfileSPMessage *m = new QosCreateRedProfileSPMessage();

		trace(TRACE_LEVEL_DEBUG, string("input profileId: ") + pQosGlobalCreateRedProfileMessage->getProfileId());
		m->setProfileId(pQosGlobalCreateRedProfileMessage->getProfileId());
		m->setMinThreshold(pQosGlobalCreateRedProfileMessage->getMinThreshold());
		m->setMaxThreshold(pQosGlobalCreateRedProfileMessage->getMaxThreshold());
		m->setDropProbability(pQosGlobalCreateRedProfileMessage->getDropProbability());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalCreateRedProfileMessage->getProfileId());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);

		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::createRedProfileMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		int createFlag = 1;
		QosGlobalCreateRedProfileMessage *pQosGlobalCreateRedProfileMessage = dynamic_cast<QosGlobalCreateRedProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		RedProfileManagedObject *pRedProfileManagedObject = NULL;
		SI32 redProfileId = pQosGlobalCreateRedProfileMessage->getProfileId();
		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RedProfileManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeSI32(&redProfileId, "profileId"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
		  UI32 numberOfResults = pResults->size ();
		  if (0 == numberOfResults) {
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 0");
			createFlag = 1;
		  } else if (1 == numberOfResults) {
			pRedProfileManagedObject = dynamic_cast<RedProfileManagedObject*>((*pResults)[0]);
			trace (TRACE_LEVEL_DEVEL, "numberOfResults = 1");
			createFlag = 0;
		  }
		}

		if (createFlag == 0) {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 0, updating existing object.");
		  prismAssert (NULL != pQosGlobalCreateRedProfileMessage, __FILE__, __LINE__);
          updateWaveManagedObject (pRedProfileManagedObject);
		} else {
		  trace (TRACE_LEVEL_DEVEL, "createFlag = 1, creating a new object.");
		  pRedProfileManagedObject = new RedProfileManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
		  prismAssert (NULL != pRedProfileManagedObject, __FILE__, __LINE__);
		}

		if (createFlag == 1) {
			pRedProfileManagedObject->setProfileId(pQosGlobalCreateRedProfileMessage->getProfileId());
		}

		pRedProfileManagedObject->setMinThreshold(pQosGlobalCreateRedProfileMessage->getMinThreshold());
		pRedProfileManagedObject->setMaxThreshold(pQosGlobalCreateRedProfileMessage->getMaxThreshold());
		pRedProfileManagedObject->setDropProbability(pQosGlobalCreateRedProfileMessage->getDropProbability());

		trace(TRACE_LEVEL_DEVEL, string("ProfileId: ") + pQosGlobalCreateRedProfileMessage->getProfileId() +
							string(" minThreshold: ") + pQosGlobalCreateRedProfileMessage->getMinThreshold() +
							string(" maxThreshold: ") + pQosGlobalCreateRedProfileMessage->getMaxThreshold() +
							string(" dropProbability: ") + pQosGlobalCreateRedProfileMessage->getDropProbability());

		if (createFlag && pRedProfileManagedObject) {
			// mark the MO for GC
			pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pRedProfileManagedObject);
		}

		if (pResults) {
			pPrismLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
		}

		if (pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::deleteDscpMarkListStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpMarkListMessage *pQosGlobalDeleteDscpMarkListMessage = dynamic_cast<QosGlobalDeleteDscpMarkListMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;

		QosDeleteDscpMarkListSPMessage *m = new QosDeleteDscpMarkListSPMessage();
		m->setDscpMapName(pQosGlobalDeleteDscpMarkListMessage->getDscpMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteDscpMarkListMessage->getDscpMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::deleteDscpMarkListMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpMarkListMessage *pQosGlobalDeleteDscpMarkListMessage = dynamic_cast<QosGlobalDeleteDscpMarkListMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string dscpMapName = pQosGlobalDeleteDscpMarkListMessage->getDscpMapName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpMarkListManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(dscpMapName, "dscpMapName"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		DscpMarkListManagedObject *pDscpMarkListManagedObject = NULL;
		
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpMarkListMOStep: MO not found");
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpMarkListMOStep: MO found");
				pDscpMarkListManagedObject = dynamic_cast<DscpMarkListManagedObject*>((*pResults)[0]);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteDscpMarkListMOStep: More than one MO found");
			}
		}

		prismAssert (NULL != pDscpMarkListManagedObject, __FILE__, __LINE__);
        //Delete the wave managed object without loading the MO completely.
        deleteWaveManagedObject(pDscpMarkListManagedObject->getObjectId());
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpMarkListManagedObject);

		if (NULL != pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::deleteDscpToCosMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage =
            dynamic_cast<QosGlobalDeleteDscpToCosMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
        ObjectId mapObjectId ;
        UI32 poCount = 0;
        UI32 phyCount = 0;

        string mapName = pQosGlobalDeleteDscpToCosMapMessage->getDscpToCosMapName();
        status = QosUtils::getQosMapObjectIdByMapName (mapName, mapObjectId, DSCP_COS_MAP, NULL);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName for DSCP_COS_MAP failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);                                                                                           return;
        }

        /* Check if map is activated in Port channel/Physical interface. If yes, throw error */
        WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
        poSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpCosMap", PoIntfManagedObject::getClassName()));
        status = querySynchronouslyForCount(&poSyncQueryCtxt, poCount);


        WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        phyIntfSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpCosMap", PhyIntfLocalManagedObject::getClassName()));
        phyIntfSyncQueryCtxt.setFilterByConnectedNodes();
        status = querySynchronouslyForCount(&phyIntfSyncQueryCtxt, phyCount);


        if (status != WAVE_MESSAGE_SUCCESS)                                                                                                                          {
            trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosDscpCosMap failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            if ( (0!=poCount) || (0!=phyCount)) {
                pPrismLinearSequencerContext->executeNextStep(WRC_SSM_QOS_MAP_IS_STILL_ACTIVATED);
                return;
            }
        }
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }


	void QosGlobalWorker::deleteDscpToCosMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage = dynamic_cast<QosGlobalDeleteDscpToCosMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;

		QosDeleteDscpToCosMapSPMessage *m = new QosDeleteDscpToCosMapSPMessage();
		m->setDscpCosMapName(pQosGlobalDeleteDscpToCosMapMessage->getDscpToCosMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteDscpToCosMapMessage->getDscpToCosMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::deleteDscpToCosMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToCosMapMessage *pQosGlobalDeleteDscpToCosMapMessage = dynamic_cast<QosGlobalDeleteDscpToCosMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string dscpToCosMapName = pQosGlobalDeleteDscpToCosMapMessage->getDscpToCosMapName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToCosMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(dscpToCosMapName, "dscpToCosMapName"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		DscpToCosMapManagedObject *pDscpToCosMapManagedObject = NULL;
		
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToCosMapMOStep: MO not found");
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToCosMapMOStep: MO found");
				pDscpToCosMapManagedObject = dynamic_cast<DscpToCosMapManagedObject*>((*pResults)[0]);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToCosMapMOStep: More than one MO found");
			}
		}

		prismAssert (NULL != pDscpToCosMapManagedObject, __FILE__, __LINE__);

		deleteWaveManagedObject(pDscpToCosMapManagedObject->getObjectId());
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToCosMapManagedObject);

		if (NULL != pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::deleteDscpToDscpMutationMapValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage =
            dynamic_cast<QosGlobalDeleteDscpToDscpMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;                                                                                                                ObjectId mapObjectId ;
        UI32 poCount = 0;
        UI32 phyCount = 0;

        string mapName = pQosGlobalDeleteDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName();
        status = QosUtils::getQosMapObjectIdByMapName (mapName, mapObjectId, DSCP_MUTATION_MAP, NULL);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName for DSCP_MUTATION_MAP failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

        /* Check if map is activated in Port channel/Physical interface. If yes, throw error */
        WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
        poSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpMap", PoIntfManagedObject::getClassName()));
        status = querySynchronouslyForCount(&poSyncQueryCtxt, poCount);


        WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        phyIntfSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpMap", PhyIntfLocalManagedObject::getClassName()));
        phyIntfSyncQueryCtxt.setFilterByConnectedNodes();
        status = querySynchronouslyForCount(&phyIntfSyncQueryCtxt, phyCount);


        if (status != WAVE_MESSAGE_SUCCESS)                                                                                                                          {
            trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosDscpMap failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            if ( (0!=poCount) || (0!=phyCount))  {
                pPrismLinearSequencerContext->executeNextStep(WRC_SSM_QOS_MAP_IS_STILL_ACTIVATED);
                return;
            }
        }
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }


	void QosGlobalWorker::deleteDscpToDscpMutationMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage = dynamic_cast<QosGlobalDeleteDscpToDscpMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;

		QosDeleteDscpToDscpMutationMapSPMessage *m = new QosDeleteDscpToDscpMutationMapSPMessage();
		m->setDscpMutationMapName(pQosGlobalDeleteDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::deleteDscpToDscpMutationMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToDscpMutationMapMessage *pQosGlobalDeleteDscpToDscpMutationMapMessage = dynamic_cast<QosGlobalDeleteDscpToDscpMutationMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string dscpToDscpMutationMapName = pQosGlobalDeleteDscpToDscpMutationMapMessage->getDscpToDscpMutationMapName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToDscpMutationMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(dscpToDscpMutationMapName, "dscpToDscpMutationMapName"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		DscpToDscpMutationMapManagedObject *pDscpToDscpMutationMapManagedObject = NULL;
		
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToDscpMutationMapMOStep: MO not found");
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToDscpMutationMapMOStep: MO found");
				pDscpToDscpMutationMapManagedObject = dynamic_cast<DscpToDscpMutationMapManagedObject*>((*pResults)[0]);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToDscpMutationMapMOStep: More than one MO found");
			}
		}

		prismAssert (NULL != pDscpToDscpMutationMapManagedObject, __FILE__, __LINE__);
        
        //Delete the object, without loading the complete object.
        deleteWaveManagedObject( pDscpToDscpMutationMapManagedObject->getObjectId());
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToDscpMutationMapManagedObject);

		if (NULL != pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::deleteDscpToTrafficClassMapValidationStep (PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage =
            dynamic_cast<QosGlobalDeleteDscpToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        ResourceId status = WRC_WAVE_MESSAGE_SUCCESS;
        ObjectId mapObjectId ;
        UI32 poCount = 0;
        UI32 phyCount = 0;

        string mapName = pQosGlobalDeleteDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName();
        status = QosUtils::getQosMapObjectIdByMapName (mapName, mapObjectId, DSCP_TRAFFIC_CLASS_MAP, NULL);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("getQosMapObjectIdByMapName for DSCP_TRAFFIC_CLASS_MAP failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }

        /* Check if map is activated in Port channel/Physical interface. If yes, throw error */
        WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
        poSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpTrafficClassMap", PoIntfManagedObject::getClassName()));
        status = querySynchronouslyForCount(&poSyncQueryCtxt, poCount);


        WaveManagedObjectSynchronousQueryContext phyIntfSyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        phyIntfSyncQueryCtxt.addAndAttribute(new AttributeObjectIdAssociation(mapObjectId, "qosDscpTrafficClassMap", PhyIntfLocalManagedObject::getClassName()));
        phyIntfSyncQueryCtxt.setFilterByConnectedNodes();
        status = querySynchronouslyForCount(&phyIntfSyncQueryCtxt, phyCount);

        if (status != WAVE_MESSAGE_SUCCESS)
        {
            trace (TRACE_LEVEL_ERROR, string ("querySynchronouslyForCount for qosDscpTrafficClassMap failed"));
            pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_ERROR);
            return;
        }
        else
        {
            if ( (0!=poCount) ||  (0!= phyCount) ) {
                pPrismLinearSequencerContext->executeNextStep(WRC_SSM_QOS_MAP_IS_STILL_ACTIVATED);
                return;
            }
        }
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }


	void QosGlobalWorker::deleteDscpToTrafficClassMapStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage = dynamic_cast<QosGlobalDeleteDscpToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;

		QosDeleteDscpToTrafficClassMapSPMessage *m = new QosDeleteDscpToTrafficClassMapSPMessage();
		m->setDscpTrafficClassMapName(pQosGlobalDeleteDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::deleteDscpToTrafficClassMapMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteDscpToTrafficClassMapMessage *pQosGlobalDeleteDscpToTrafficClassMapMessage = dynamic_cast<QosGlobalDeleteDscpToTrafficClassMapMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		string dscpToTrafficClassMapName = pQosGlobalDeleteDscpToTrafficClassMapMessage->getDscpToTrafficClassMapName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(DscpToTrafficClassMapManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(dscpToTrafficClassMapName, "dscpToTrafficClassMapName"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		DscpToTrafficClassMapManagedObject *pDscpToTrafficClassMapManagedObject = NULL;
		
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToTrafficClassMapMOStep: MO not found");
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToTrafficClassMapMOStep: MO found");
				pDscpToTrafficClassMapManagedObject = dynamic_cast<DscpToTrafficClassMapManagedObject*>((*pResults)[0]);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteDscpToTrafficClassMapMOStep: More than one MO found");
			}
		}

		prismAssert (NULL != pDscpToTrafficClassMapManagedObject, __FILE__, __LINE__);
        
        //Delete Mo without loading it completely.
        deleteWaveManagedObject(pDscpToTrafficClassMapManagedObject->getObjectId());
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pDscpToTrafficClassMapManagedObject);

		if (NULL != pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void QosGlobalWorker::deleteRedProfileStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteRedProfileMessage *pQosGlobalDeleteRedProfileMessage = dynamic_cast<QosGlobalDeleteRedProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		WaveSendToClusterContext    *send_ctx_p;

		QosDeleteRedProfileSPMessage *m = new QosDeleteRedProfileSPMessage();
		m->setProfileId(pQosGlobalDeleteRedProfileMessage->getProfileId());

		send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback), pPrismLinearSequencerContext);
		prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
		send_ctx_p->setPPrismMessageForPhase1(m);
		trace(TRACE_LEVEL_DEBUG, string("Sending msg to Cluster :") + pQosGlobalDeleteRedProfileMessage->getProfileId());
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
		sendToWaveCluster(send_ctx_p);
		return ;
	}

	void QosGlobalWorker::deleteRedProfileMOStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
	{
		QosGlobalDeleteRedProfileMessage *pQosGlobalDeleteRedProfileMessage = dynamic_cast<QosGlobalDeleteRedProfileMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		SI32 profileId = pQosGlobalDeleteRedProfileMessage->getProfileId();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RedProfileManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeSI32(profileId, "profileId"));
        syncQueryCtxt.addSelectField("objectId");
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		RedProfileManagedObject *pRedProfileManagedObject = NULL;
		
		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteRedProfileMOStep: MO not found");
			} else if (1 == numberOfResults) {
				trace (TRACE_LEVEL_ERROR, "deleteRedProfileMOStep: MO found");
				pRedProfileManagedObject = dynamic_cast<RedProfileManagedObject*>((*pResults)[0]);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteRedProfileMOStep: More than one MO found");
			}
		}

		prismAssert (NULL != pRedProfileManagedObject, __FILE__, __LINE__);

		deleteWaveManagedObject(pRedProfileManagedObject->getObjectId());
        pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pRedProfileManagedObject);

		if (NULL != pResults) {
			delete pResults;
		}

		pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
	}

	void  QosGlobalWorker::QosGlobalNasMessageHandler( QosGlobalNasMessage *pQosGlobalNasMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::NasValidationStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::NasSendToLocalWorkerStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::NasCommitDBStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&QosGlobalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext (pQosGlobalNasMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->start ();
    }

    void QosGlobalWorker::NasValidationStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 vlanCount = 0;
		UI32 vrfCount = 0;
        QosGlobalNasMessage *pQosGlobalNasMessage = dynamic_cast<QosGlobalNasMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		UI32 cmdCode = pQosGlobalNasMessage->getCmdCode();
		trace (TRACE_LEVEL_DEBUG, string ("Global NasValidationStep:cmdCode=")+cmdCode + string(" opCode=") + pQosGlobalNasMessage->getOpCode());

		if ((pQosGlobalNasMessage->getOpCode() != QOS_NAS_DCMD_CONFIG)) {
		  	// No validation for 'no' form of the command
			pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
			return;
		}

		/* 1. Check if cee-map set to default */
		if (cmdCode  == QOS_NAS_DCMD_AUTO) {
			status = QosUtils::isCeeMapSetToDefault();
			if (status != WAVE_MESSAGE_SUCCESS) {
				trace (TRACE_LEVEL_INFO, string ("NasValidationStep:isCeeMapSetToDefault returned ")+status);
				status = WRC_AUTO_NAS_CEE_MAP_NOT_DEFAULT;
			}
		} else if (cmdCode == QOS_NAS_DCMD_COS) {
		/* 2. Check if cos value conflicts with fcoe(3) or fabric priority(7) */
		  	UI32 cosValue = pQosGlobalNasMessage->getCos();
			if ((cosValue == CEE_FABRIC_PRIORITY_COS) || (cosValue == 3)) {
				status = WRC_AUTO_NAS_COS_CONFLICT;
				trace (TRACE_LEVEL_DEBUG, string ("NasValidationStep:cos value conflicts with fabric or fcoe priority.cos=") + cosValue);
			}
		} else if (cmdCode == (QOS_NAS_DCMD_SERVER_IP_VLAN) || (cmdCode == QOS_NAS_DCMD_SERVER_IP_VRF)) {
		/* 3. Check for max server-ip configuration. */
			WaveManagedObjectSynchronousQueryContext vlanQueryCtxt (NasServerIpVlanManagedObject::getClassName());
			status = querySynchronouslyForCount(&vlanQueryCtxt, vlanCount);
	
			if (status != WAVE_MESSAGE_SUCCESS)
			{
				trace (TRACE_LEVEL_ERROR, string ("NasValidationStep:querySynchronouslyForCount for NasServerIpVlanManagedObject failed"));
				status = WAVE_MESSAGE_ERROR;
			}

			WaveManagedObjectSynchronousQueryContext vrfQueryCtxt (NasServerIpVrfManagedObject::getClassName());
			status = querySynchronouslyForCount(&vrfQueryCtxt, vrfCount);
	
			if (status != WAVE_MESSAGE_SUCCESS)
			{
				trace (TRACE_LEVEL_ERROR, string ("NasValidationStep:querySynchronouslyForCount for NasServerIpVrfManagedObject failed"));
				status = WAVE_MESSAGE_ERROR;
			}

			if ((vrfCount + vlanCount) == AUTO_NAS_MAX_SERVER_IP_ALLOWED) {
				status = WRC_AUTO_NAS_MAX_SERVER_IP_REACHED;
			}

			/* 4. Check for vlan and vrf conflict in server-ip configuration */
			if (status == WAVE_MESSAGE_SUCCESS) {
				NasServerIpManagedObject *pNasServerIpManagedObject = NULL;
				IpV4AddressNetworkMask ipaddressPrefix = pQosGlobalNasMessage->getServerIpAddressPrefix();
				WaveManagedObjectSynchronousQueryContext syncQueryCtxt (NasServerIpManagedObject::getClassName());
				syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&ipaddressPrefix,"serverIpAddressPrefix"));
				vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
				if (NULL == pResults) {
					trace(TRACE_LEVEL_DEVEL, string("NasValidationStep Query on NasServerIpManagedObject returned success"));
					status = WAVE_MESSAGE_SUCCESS;
				} else if (pResults->size() == 0) {
					trace(TRACE_LEVEL_DEVEL, string("NasValidationStep Query on NasServerIpManagedObject returned size 0"));
					status = WAVE_MESSAGE_SUCCESS;
				} else {
					pNasServerIpManagedObject = dynamic_cast<NasServerIpManagedObject*>((*pResults)[0]);
					if (cmdCode == QOS_NAS_DCMD_SERVER_IP_VLAN) {
					  	vrfCount = pNasServerIpManagedObject->getVrfs().size();
						trace(TRACE_LEVEL_DEBUG, string("NasValidationStep vrf size=") + vrfCount);
						if (vrfCount != 0) {
							status = WRC_AUTO_NAS_SERVER_CONFIGURED_WITH_VRF; 
						}
					}
					if (cmdCode == QOS_NAS_DCMD_SERVER_IP_VRF) {
					  	vlanCount = pNasServerIpManagedObject->getVlans().size();
						trace(TRACE_LEVEL_DEBUG, string("NasValidationStep vlan size=") + vlanCount); 
						if (vlanCount != 0) {
							status = WRC_AUTO_NAS_SERVER_CONFIGURED_WITH_VLAN; 
						}
					}
				}
				if (pResults) {
					WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
				}
			}
		}
		pPrismLinearSequencerContext->executeNextStep(status);
    }

    void QosGlobalWorker::NasSendToLocalWorkerStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        QosGlobalNasMessage *pQosGlobalNasMessage = dynamic_cast<QosGlobalNasMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        WaveSendToClusterContext    *send_ctx_p;

        QosLocalNasMessage *pQosLocalNasMessage = new QosLocalNasMessage();
         pQosLocalNasMessage->setOpCode(pQosGlobalNasMessage->getOpCode());
         pQosLocalNasMessage->setCmdCode(pQosGlobalNasMessage->getCmdCode());
         pQosLocalNasMessage->setCos(pQosGlobalNasMessage->getCos());
         pQosLocalNasMessage->setDscp(pQosGlobalNasMessage->getDscp());
         pQosLocalNasMessage->setServerIpAddressPrefix(pQosGlobalNasMessage->getServerIpAddressPrefix());
         pQosLocalNasMessage->setVrfName(pQosGlobalNasMessage->getVrfName());
         pQosLocalNasMessage->setVlanNumber(pQosGlobalNasMessage->getVlanNumber());

        send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&QosGlobalWorker::QosGlobalSendToClusterCallback),
			pPrismLinearSequencerContext);
        prismAssert (NULL != send_ctx_p, __FILE__, __LINE__);
        send_ctx_p->setPPrismMessageForPhase1(pQosLocalNasMessage);
        send_ctx_p->setPartialSuccessFlag(true);
        send_ctx_p->setTreatFailureOnFailingOverAsSuccessFlag (true);
        sendToWaveCluster(send_ctx_p);
        return ;

    }

	void QosGlobalWorker::NasCommitDBStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        UI32 opCode = 0;
        UI32 cmdCode = 0;
        bool managedObjectPresent = false;
        QosGlobalNasMessage *pQosGlobalNasMessage = dynamic_cast<QosGlobalNasMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());

        opCode = pQosGlobalNasMessage->getOpCode();
        cmdCode = pQosGlobalNasMessage->getCmdCode();

        switch (cmdCode)
        {
            case QOS_NAS_DCMD_AUTO:
            case QOS_NAS_DCMD_COS:
            case QOS_NAS_DCMD_DSCP:
            {
                NasConfigManagedObject *pNasConfigManagedObject = NULL;
                WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NasConfigManagedObject::getClassName());
                vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
                if (pResults != NULL) {
                    UI32 numberOfResults = pResults->size ();
                    if (numberOfResults == 0) {
                        managedObjectPresent = false;
                    } else {
                        managedObjectPresent = true;
                    }
                }
                switch (opCode)
                {
                    case QOS_NAS_DCMD_CONFIG: // 'yes' form of command
                    {
                        if (!managedObjectPresent) { //case of create
							createNasConfigAndReflectInCeeMap(pPrismLinearSequencerContext);
                        }
                        else { //case of update
							pNasConfigManagedObject = dynamic_cast<NasConfigManagedObject*>((*pResults)[0]);
							if (cmdCode == QOS_NAS_DCMD_COS) {
								setNasCosValueAndReflectInCeeMap (pPrismLinearSequencerContext,
									pNasConfigManagedObject, pQosGlobalNasMessage->getCos());
							} else if (cmdCode == QOS_NAS_DCMD_DSCP) { 
								startTransaction();
									pNasConfigManagedObject->setDscp(pQosGlobalNasMessage->getDscp());
									updateWaveManagedObject (pNasConfigManagedObject);
								commitTransaction();
							}
                        }
                        break;
                    }
					case QOS_NAS_DCMD_DE_CONFIG: // 'no' form of command
                    {
                        	pNasConfigManagedObject = dynamic_cast<NasConfigManagedObject*>((*pResults)[0]);
                        	if (cmdCode == QOS_NAS_DCMD_AUTO) {
								deleteNasConfigAndReflectInCeeMap(pPrismLinearSequencerContext, pNasConfigManagedObject);
                        	} else if (cmdCode == QOS_NAS_DCMD_COS) {
								deleteNasCosValueAndReflectInCeeMap (pPrismLinearSequencerContext,
									pNasConfigManagedObject);
                        	} else if (cmdCode == QOS_NAS_DCMD_DSCP) {
								startTransaction();
									pNasConfigManagedObject->setDscp(DEFAULT_NAS_DSCP);
									updateWaveManagedObject (pNasConfigManagedObject);
								commitTransaction();
                        	}
                        break;
                    }
                }
				if (!((cmdCode == QOS_NAS_DCMD_AUTO) && (opCode == QOS_NAS_DCMD_DE_CONFIG))) {
					if (pResults) {
						WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
                    }
				}
                break;
            }

            case QOS_NAS_DCMD_SERVER_IP_VLAN:
            case QOS_NAS_DCMD_SERVER_IP_VRF:
            {
			  	if (opCode == QOS_NAS_DCMD_CONFIG) {
					createNasServerIpConfig(pPrismLinearSequencerContext);
				} else if (opCode == QOS_NAS_DCMD_DE_CONFIG) {
					deleteNasServerIpConfig(pPrismLinearSequencerContext);
				}
                break;
            }
        }
        pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }

	void QosGlobalWorker::createNasConfigAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
        /* 
         * 1. Create PGID 3 in ceemap
         * 2. Create NasConfigMO
		 * 3. Change PGID2 weight in cee-map
		 * 4. Change mapping for cos2 to PGID3 in cee-map.
         */
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
        PriorityGroupManagedObject *pPriorityGroup3ManagedObject = NULL;
        PriorityGroupManagedObject *pPriorityGroup2ManagedObject = NULL;
        WaveManagedObjectPointer<PriorityGroupManagedObject> *pPriorityGroupMO = NULL;
        NasConfigManagedObject *pNasConfigManagedObject = NULL;
		ObjectId pgid3ObjectId;

        WaveManagedObjectSynchronousQueryContext ceeMapQuery (CeeMapManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&ceeMapQuery);
        if (NULL != pResults && pResults->size ()== 1) {
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pResults)[0]);
        } else {
            trace (TRACE_LEVEL_ERROR, "createNasConfigAndReflectInCeeMap ceemap query failed");
        }

        startTransaction(); /* Start of Transaction */
			/* 1. Create PGID 3 in ceemap */
            pPriorityGroup3ManagedObject = new PriorityGroupManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
            string ceename = "default";
            pPriorityGroup3ManagedObject->setPgId(AUTO_NAS_PGID_VALUE);
            pPriorityGroup3ManagedObject->setWeight(AUTO_NAS_PGID_WEIGHT);
            pPriorityGroup3ManagedObject->setPfc(0);
            pPriorityGroup3ManagedObject->setCeeMapName(ceename);
			pPriorityGroup3ManagedObject->setOwnerManagedObjectId(pCeeMapManagedObject->getObjectId());
			pgid3ObjectId = pPriorityGroup3ManagedObject->getObjectId();
			addToComposition(CeeMapManagedObject::getClassName(), PriorityGroupManagedObject::getClassName(), "priorityGroup",
				pCeeMapManagedObject->getObjectId(), pgid3ObjectId);
			/* 2. create NasConfigManagedObject */
			pNasConfigManagedObject = new NasConfigManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()));
			/* 3. Change weight of PGID 2*/
			pPriorityGroupMO = pCeeMapManagedObject->findPriorityGroupByName(CEE_LAN_PGID_VALUE);
			pPriorityGroup2ManagedObject = (*pPriorityGroupMO).operator ->();
			pPriorityGroup2ManagedObject->setWeight(40);
			updateWaveManagedObject(pPriorityGroup2ManagedObject);
			/* 4. Map cos2 to PGID3*/
			WaveManagedObjectSynchronousQueryContext pmapQuery(PriorityMapManagedObject::getClassName());
			vector<WaveManagedObject *> *pPriorityMapResults = querySynchronously (&pmapQuery);
			PriorityMapManagedObject *pPriorityMapManagedObject = NULL;
			if (NULL != pPriorityMapResults && pPriorityMapResults->size ()== 1) {
				pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pPriorityMapResults)[0]);
				pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pgid3ObjectId);
				updateWaveManagedObject(pPriorityMapManagedObject);
			} else {
				trace (TRACE_LEVEL_ERROR, "createNasConfigAndReflectInCeeMap priority map query failed");
			}
        commitTransaction(); /* End of transaction */

		if (pPriorityMapResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPriorityMapResults);
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		if (pNasConfigManagedObject)
		  	delete pNasConfigManagedObject;
		if (pPriorityGroup3ManagedObject)
		  	delete pPriorityGroup3ManagedObject;
    }

	void QosGlobalWorker::setNasCosValueAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
		NasConfigManagedObject *pNasConfigManagedObject, const UI32 &newNasCosValue)
    {
		/* 1. Update cos value in NasConfigManaged Object.
		 *   2. In priority map, find the cos value that is currently mapping to PGID 3. Make it point to PGID 2.
		 *   3. In priority map, mark the new cos value's PGID as 3.
		 */
        PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
		ObjectId pgid2ObjectId, pgid3ObjectId;
		UI32 i = 0;

		/* Get Object IDs of PGID 2 and 3. */
		// PGIDs 2 and 3 will be present in DB.
        WaveManagedObjectSynchronousQueryContext pgQuery (PriorityGroupManagedObject::getClassName());
		pgQuery.addOrAttribute (new AttributeString (CEE_LAN_PGID_VALUE, "pgId"));
		pgQuery.addOrAttribute (new AttributeString (AUTO_NAS_PGID_VALUE, "pgId"));
		pgQuery.addSelectField("objectId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&pgQuery);

		if (pResults != NULL && pResults->size ()== 2) {
			for (i = 0; i < 2; i ++) {
				pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[i]);
				if (pPriorityGroupManagedObject->getPgId() == CEE_LAN_PGID_VALUE) {
					pgid2ObjectId = pPriorityGroupManagedObject->getObjectId();
				} else if (pPriorityGroupManagedObject->getPgId() == AUTO_NAS_PGID_VALUE) {
					pgid3ObjectId = pPriorityGroupManagedObject->getObjectId();
				}
			}
		} else {
			trace (TRACE_LEVEL_ERROR, "deleteNasCosValueAndReflectInCeeMap pgQuery failed");
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return;
		}


		startTransaction(); /* start transaction */

		/* 1. Update cos value in NasConfigManaged Object.*/
		pNasConfigManagedObject->setCos(newNasCosValue);
		updateWaveManagedObject (pNasConfigManagedObject);

		WaveManagedObjectSynchronousQueryContext pmapQuery(PriorityMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pPriorityMapResults = querySynchronously (&pmapQuery);
		PriorityMapManagedObject *pPriorityMapManagedObject = NULL;

		/* 2. In priority map, find the cos value that is currently mapping to PGID 3. Make it point to PGID 2.*/
		if (NULL != pPriorityMapResults && pPriorityMapResults->size ()== 1) {
			pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pPriorityMapResults)[0]);
			if (pPriorityMapManagedObject->getCos0ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos1ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos2ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos3ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos4ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos5ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos6ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos7ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pgid2ObjectId);


			/* 3. In priority map, mark the new cos value's PGID as 3.*/
			if (newNasCosValue == 0)
				pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 1)
				pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 2)
				pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 3)
				pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 4)
				pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 5)
				pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 6)
				pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pgid3ObjectId);
			else if (newNasCosValue == 7)
				pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pgid3ObjectId);

			updateWaveManagedObject(pPriorityMapManagedObject);
		} else {
			trace (TRACE_LEVEL_ERROR, "setNasCosValueAndReflectInCeeMap priority map query failed");
		}
		commitTransaction(); /* end of Transaction */

		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		if (pPriorityMapResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPriorityMapResults);
		
	}

	void QosGlobalWorker::deleteNasCosValueAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
		NasConfigManagedObject *pNasConfigManagedObject)
    {
		/*
		 * 1. set cos value to default in NasConfigManagedObject
		 * 2. In priority Map, make input 'cosValue' point to PGID 2.
		 * 3. In priority map, map cos 2 to PGID 3.
		 */
	  	UI32 i = 0;
		UI32 cosValue = pNasConfigManagedObject->getCos();
		ObjectId pgid2ObjectId, pgid3ObjectId;
		PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
		PriorityMapManagedObject *pPriorityMapManagedObject = NULL;

		/* Get Object IDs of PGID 2 and 3. */
		// PGIDs 2 and 3 will be present in DB.
        WaveManagedObjectSynchronousQueryContext pgQuery (PriorityGroupManagedObject::getClassName());
		pgQuery.addOrAttribute (new AttributeString (CEE_LAN_PGID_VALUE, "pgId"));
		pgQuery.addOrAttribute (new AttributeString (AUTO_NAS_PGID_VALUE, "pgId"));
		pgQuery.addSelectField("objectId");
        vector<WaveManagedObject *> *pResults = querySynchronously (&pgQuery);

		if (pResults != NULL && pResults->size ()== 2) {
			for (i = 0; i < 2; i ++) {
				pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[i]);
				if (pPriorityGroupManagedObject->getPgId() == CEE_LAN_PGID_VALUE) {
					pgid2ObjectId = pPriorityGroupManagedObject->getObjectId();
				} else if (pPriorityGroupManagedObject->getPgId() == AUTO_NAS_PGID_VALUE) {
					pgid3ObjectId = pPriorityGroupManagedObject->getObjectId();
				}
			}
		} else {
			trace (TRACE_LEVEL_ERROR, "deleteNasCosValueAndReflectInCeeMap pgQuery failed");
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return;
		}

		startTransaction(); /* start transaction */
			/* 1. set cos value to default in NasConfigManagedObject */
			pNasConfigManagedObject->setCos(DEFAULT_NAS_COS);
			updateWaveManagedObject (pNasConfigManagedObject);

			WaveManagedObjectSynchronousQueryContext pmapQuery(PriorityMapManagedObject::getClassName());
			vector<WaveManagedObject *> *pPriorityMapResults = querySynchronously (&pmapQuery);
			if (NULL != pPriorityMapResults && pPriorityMapResults->size ()== 1) {
				pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pPriorityMapResults)[0]);
				/* 2. In priority Map, make input 'cosValue' point to PGID 2.*/
				if (cosValue != DEFAULT_AUTO_NAS_COS) {
					if (cosValue == 0) {
						pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 1) {
						pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 3) {
						pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 4) {
						pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 5) {
						pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 6) {
						pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pgid2ObjectId);
					}
					if (cosValue == 7) {
						pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pgid2ObjectId);
					}
				}
				/* 3. In priority map, map cos 2 to PGID 3.*/
				if (cosValue != DEFAULT_AUTO_NAS_COS) {
					pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pgid3ObjectId);
				}
				updateWaveManagedObject(pPriorityMapManagedObject);
			} else {
				trace (TRACE_LEVEL_ERROR, "deleteNasCosValueAndReflectInCeeMap priority map query failed");
			}
		commitTransaction(); /* commit transaction */
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
	}

	void QosGlobalWorker::deleteNasConfigAndReflectInCeeMap(PrismLinearSequencerContext *pPrismLinearSequencerContext,
		NasConfigManagedObject *pNasConfigManagedObject)
    {
	  	/*
		 * 1. Delete NasConfigManagedObject.
		 * 2. In priority map, find the cos that maps to PGID 3. Mark that cos to PGID 2.
		 * 3. Delete PGID 3.
		 * 4. Add weight 20 (PGID 3's weight) to PGID 2.
		*/


		/* Get Object IDs of PGID 2 and 3. */
		UI32 i = 0;
		ObjectId pgid2ObjectId, pgid3ObjectId;
        CeeMapManagedObject *pCeeMapManagedObject = NULL;
		PriorityGroupManagedObject *pPriorityGroupManagedObject = NULL;
		PriorityGroupManagedObject *pPriorityGroup2ManagedObject = NULL;
		// PGIDs 2 and 3 will be present in DB.
        WaveManagedObjectSynchronousQueryContext pgQuery (PriorityGroupManagedObject::getClassName());
		pgQuery.addOrAttribute (new AttributeString (CEE_LAN_PGID_VALUE, "pgId"));
		pgQuery.addOrAttribute (new AttributeString (AUTO_NAS_PGID_VALUE, "pgId"));
        vector<WaveManagedObject *> *pResults = querySynchronously (&pgQuery);

		if (pResults != NULL && pResults->size ()== 2) {
			for (i = 0; i < 2; i ++) {
				pPriorityGroupManagedObject = dynamic_cast<PriorityGroupManagedObject*>((*pResults)[i]);
				if (pPriorityGroupManagedObject->getPgId() == CEE_LAN_PGID_VALUE) {
					pgid2ObjectId = pPriorityGroupManagedObject->getObjectId();
					pPriorityGroup2ManagedObject = pPriorityGroupManagedObject;
				} else if (pPriorityGroupManagedObject->getPgId() == AUTO_NAS_PGID_VALUE) {
					pgid3ObjectId = pPriorityGroupManagedObject->getObjectId();
				}
			}
		} else {
			trace (TRACE_LEVEL_ERROR, "deleteNasConfigAndReflectInCeeMap pgQuery failed");
			if (pResults)
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			return;
		}

		startTransaction (); /* start transaction */
		/* 1. Delete NasConfigManagedObject */ 
		delete pNasConfigManagedObject;

		WaveManagedObjectSynchronousQueryContext pmapQuery(PriorityMapManagedObject::getClassName());
		vector<WaveManagedObject *> *pPriorityMapResults = querySynchronously (&pmapQuery);
		PriorityMapManagedObject *pPriorityMapManagedObject = NULL;

		/* 2. In priority map, find the cos that maps to PGID 3. Mark that cos to PGID 2. */
		if (NULL != pPriorityMapResults && pPriorityMapResults->size ()== 1) {
			pPriorityMapManagedObject = dynamic_cast<PriorityMapManagedObject*>((*pPriorityMapResults)[0]);
			if (pPriorityMapManagedObject->getCos0ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos0ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos1ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos1ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos2ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos2ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos3ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos3ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos4ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos4ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos5ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos5ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos6ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos6ToPriorityGrouMap(pgid2ObjectId);
			else if (pPriorityMapManagedObject->getCos7ToPriorityGrouMap() == pgid3ObjectId)
				pPriorityMapManagedObject->setCos7ToPriorityGrouMap(pgid2ObjectId);
			updateWaveManagedObject(pPriorityMapManagedObject);
		} else {
			trace (TRACE_LEVEL_ERROR, "deleteNasConfigAndReflectInCeeMap priority map query failed");
		}

        WaveManagedObjectSynchronousQueryContext ceeMapQuery (CeeMapManagedObject::getClassName());
		ceeMapQuery.addSelectField("objectId");
        vector<WaveManagedObject *> *pCeeResults = querySynchronously (&ceeMapQuery);
        if (NULL != pCeeResults && pCeeResults->size () == 1) {
            pCeeMapManagedObject = dynamic_cast<CeeMapManagedObject*>((*pCeeResults)[0]);
        } else {
            trace (TRACE_LEVEL_ERROR, "deleteNasConfigAndReflectInCeeMap ceemap query failed");
        }

		/* 3. Delete PGID 3.*/
		deleteFromComposition(CeeMapManagedObject::getClassName(), PriorityGroupManagedObject::getClassName(), "priorityGroup",
			pCeeMapManagedObject->getObjectId(), pgid3ObjectId);

		/* 4. Add weight 20 (PGID 3's weight) to PGID 2*/
		SI32 currentPgid2Weight = pPriorityGroup2ManagedObject->getWeight();
		trace (TRACE_LEVEL_INFO, string("deleteNasConfigAndReflectInCeeMap currentPgid2Weight=") + currentPgid2Weight);
		pPriorityGroup2ManagedObject->setWeight(currentPgid2Weight + AUTO_NAS_PGID_WEIGHT);
		updateWaveManagedObject(pPriorityGroup2ManagedObject);

		commitTransaction(); /* end of Transaction */

		if (pCeeResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pCeeResults);
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		if (pPriorityMapResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pPriorityMapResults);
	}

	void QosGlobalWorker::createNasServerIpConfig(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		QosGlobalNasMessage *pQosGlobalNasMessage = dynamic_cast<QosGlobalNasMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		IpV4AddressNetworkMask serverIpAddressPrefix = pQosGlobalNasMessage->getServerIpAddressPrefix();
		UI32 vlanNumber = pQosGlobalNasMessage->getVlanNumber();
		string vrfName = pQosGlobalNasMessage->getVrfName();
		string serverIpString = serverIpAddressPrefix.toString();
		UI32 cmdCode = pQosGlobalNasMessage->getCmdCode();
		NasServerIpManagedObject *pNasServerIpManagedObject = NULL;
		NasServerIpVlanManagedObject *pNasServerIpVlanManagedObject = NULL;
		NasServerIpVrfManagedObject *pNasServerIpVrfManagedObject = NULL;
		bool isNasServerIpManagedObjectCreated = false;

		startTransaction(); /* start of transaction */

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt (NasServerIpManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&serverIpAddressPrefix,"serverIpAddressPrefix"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if ((pResults == NULL) || (pResults->size() == 0)) {
			trace(TRACE_LEVEL_DEBUG, string("createNasServerIpConfig:Server not present ip=") + serverIpString + string(".  creating the same")); 

			pNasServerIpManagedObject = new NasServerIpManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()),
				serverIpAddressPrefix);
			isNasServerIpManagedObjectCreated = true;
		} else {
			pNasServerIpManagedObject = dynamic_cast<NasServerIpManagedObject*>((*pResults)[0]);
		}

		switch (cmdCode)
		{
            case QOS_NAS_DCMD_SERVER_IP_VLAN:
			{
				pNasServerIpVlanManagedObject = new NasServerIpVlanManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()),
					vlanNumber);
				pNasServerIpVlanManagedObject->setOwnerManagedObjectId(pNasServerIpManagedObject->getObjectId());
				addToComposition(NasServerIpManagedObject::getClassName(), NasServerIpVlanManagedObject::getClassName(), "vlans",
					pNasServerIpManagedObject->getObjectId(), pNasServerIpVlanManagedObject->getObjectId());
				trace(TRACE_LEVEL_DEBUG, string("createNasServerIpConfig:Added vlan ") +
					vlanNumber + string(" to server ") + serverIpString);
				pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pNasServerIpVlanManagedObject);
				break;
			}
            case QOS_NAS_DCMD_SERVER_IP_VRF:
			{
				pNasServerIpVrfManagedObject = new NasServerIpVrfManagedObject(dynamic_cast<QosGlobalObjectManager*>(getPWaveObjectManager()),
					vrfName);
				pNasServerIpVrfManagedObject->setOwnerManagedObjectId(pNasServerIpManagedObject->getObjectId());
				addToComposition(NasServerIpManagedObject::getClassName(), NasServerIpVrfManagedObject::getClassName(), "vrfs",
					pNasServerIpManagedObject->getObjectId(), pNasServerIpVrfManagedObject->getObjectId());
				trace(TRACE_LEVEL_DEBUG, string("createNasServerIpConfig:Added vrf ") +
					vrfName + string("to server ") + serverIpString);
				pPrismLinearSequencerContext->addManagedObjectForGarbageCollection(pNasServerIpVrfManagedObject);
				break;
			}
		}
		commitTransaction(); /* End of Transaction */

		if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
		}
		if (isNasServerIpManagedObjectCreated) {
			delete pNasServerIpManagedObject;
		}
	}
	void QosGlobalWorker::deleteNasServerIpConfig(PrismLinearSequencerContext *pPrismLinearSequencerContext)
    {
		QosGlobalNasMessage *pQosGlobalNasMessage = dynamic_cast<QosGlobalNasMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
		IpV4AddressNetworkMask serverIpAddressPrefix = pQosGlobalNasMessage->getServerIpAddressPrefix();
		string serverIpString = serverIpAddressPrefix.toString();
		UI32 vlanNumber = pQosGlobalNasMessage->getVlanNumber();
		string vrfName = pQosGlobalNasMessage->getVrfName();
		UI32 cmdCode = pQosGlobalNasMessage->getCmdCode();
		NasServerIpManagedObject *pNasServerIpManagedObject = NULL;
		NasServerIpVlanManagedObject *pNasServerIpVlanManagedObject = NULL;
		NasServerIpVrfManagedObject *pNasServerIpVrfManagedObject = NULL;
		bool isServerDeleted = false;

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt (NasServerIpManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&serverIpAddressPrefix,"serverIpAddressPrefix"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		if (pResults == NULL || pResults->size() != 1) {
			trace(TRACE_LEVEL_ERROR, string("deleteNasServerIpConfig:Server not present ip=") + serverIpString); 
			if (pResults) {
				trace(TRACE_LEVEL_ERROR, string("deleteNasServerIpConfig:Server query size=") + pResults->size()); 
			}
			return;
		} else {
			pNasServerIpManagedObject = dynamic_cast<NasServerIpManagedObject*>((*pResults)[0]);
		}

		startTransaction(); /* start of transaction */

		switch (cmdCode)
		{
            case QOS_NAS_DCMD_SERVER_IP_VLAN:
			{
				if (pNasServerIpManagedObject->getVlans().size() == 1) {
				  	/* if this is the last configuration for the server, remove the server */
				  	isServerDeleted = true;
					trace(TRACE_LEVEL_DEBUG, string("deleteNasServerIpConfig:remove server ip=") + serverIpString); 
					delete pNasServerIpManagedObject;
				} else {
					pNasServerIpVlanManagedObject = pNasServerIpManagedObject->getVlanServerConfig(vlanNumber);
					if (pNasServerIpVlanManagedObject != NULL) {
						deleteFromComposition(NasServerIpManagedObject::getClassName(), NasServerIpVlanManagedObject::getClassName(),
							"vlans", pNasServerIpManagedObject->getObjectId(), pNasServerIpVlanManagedObject->getObjectId());	
					} else {
						trace(TRACE_LEVEL_ERROR, string("deleteNasServerIpConfig:Could not find vlan  entry for server ip=") +
							serverIpString + string("vlan=") + vlanNumber); 
					}
				}
				break;
			}
            case QOS_NAS_DCMD_SERVER_IP_VRF:
			{
				if (pNasServerIpManagedObject->getVrfs().size() == 1) {
				  	/* if this is the last configuration for the server, remove the server */
				  	isServerDeleted = true;
					trace(TRACE_LEVEL_DEBUG, string("deleteNasServerIpConfig:remove server ip=") + serverIpString); 
					delete pNasServerIpManagedObject;
				} else {
					pNasServerIpVrfManagedObject = pNasServerIpManagedObject->getVrfServerConfig(vrfName);
					if (pNasServerIpVrfManagedObject != NULL) {
						deleteFromComposition(NasServerIpManagedObject::getClassName(), NasServerIpVrfManagedObject::getClassName(),
							"vrfs", pNasServerIpManagedObject->getObjectId(), pNasServerIpVrfManagedObject->getObjectId());	
					} else {
						trace(TRACE_LEVEL_ERROR, string("deleteNasServerIpConfig:Could not find vrf entry for server ip=") +
							serverIpString + string("vrf=") + vrfName); 
					}
				}
				break;
			}
		}
		commitTransaction(); /* End of transaction */

		if (!isServerDeleted) {
			if (pResults) {
				WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
			}
		}
	}
}
