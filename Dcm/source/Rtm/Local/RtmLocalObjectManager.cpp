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
 *   Author : hzhu                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"

#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmConfigLocalManagedObject.h"
#include "Rtm/Local/RtmNhProtoLocalManagedObject.h"
#include "Rtm/Local/NhStaticRouteLocalManagedObject.h"
#include "Rtm/Local/NhVrfLocalManagedObject.h"
#include "Rtm/Local/IfStaticRouteLocalManagedObject.h"
#include "Rtm/Local/RtmUpdateRtmConfigMessage.h"
#include "Rtm/Local/RtmUpdateStaticRouteMessage.h"
#include "Rtm/Local/RtmLocalShowMessage.h"
#include "Rtm/Local/RtmTypes.h"
#include "ClientInterface/Rtm/RtmMessageDef.h"
#include "ClientInterface/Rtm/RtmClientStaticRouteMessage.h"
#include "ClientInterface/Rtm/RtmClientConfigMessage.h"
#include "ClientInterface/Rtm/RtmShowMessage.h"
#include "ClientInterface/Rtm/RtmClientInterface.h"

#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmResourceIds.h"
#include "Vrf/Local/VrfUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include <ha/ipadm.h>
#include "Ipadm/Local/IpadmTypes.h"
#include "WaveCStatus.h"
#include "DcmCStatus.h"
#include "Vrf/Local/VrfUtils.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "brocade-rtm.h"

#define THIS_LOCATION  FrameworkToolKit::getThisLocationId()

namespace DcmNs
{

    RtmLocalObjectManager::RtmLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
        RtmConfigLocalManagedObject    RtmConfigLocalManagedObject    (this);
        RtmNhProtoLocalManagedObject   RtmNhProtoLocalManagedObject   (this);
        NhStaticRouteLocalManagedObject    NhStaticRouteLocalManagedObject    (this);
        NhVrfLocalManagedObject    NhVrfLocalManagedObject    (this);
        IfStaticRouteLocalManagedObject    IfStaticRouteLocalManagedObject    (this);
        RtmConfigLocalManagedObject.setupOrm ();
        addManagedClass (RtmConfigLocalManagedObject::getClassName ());
        RtmNhProtoLocalManagedObject.setupOrm ();
        addManagedClass (RtmNhProtoLocalManagedObject::getClassName ());
        NhStaticRouteLocalManagedObject.setupOrm ();
        NhVrfLocalManagedObject.setupOrm ();
        addManagedClass (NhStaticRouteLocalManagedObject::getClassName ());
        addManagedClass (NhVrfLocalManagedObject::getClassName ());
        IfStaticRouteLocalManagedObject.setupOrm ();
        addManagedClass (IfStaticRouteLocalManagedObject::getClassName ());

        addOperationMap (RTMUPDATERTMCONFIG, reinterpret_cast<PrismMessageHandler> (&RtmLocalObjectManager::RtmUpdateRtmConfigMessageHandler));
        addOperationMap (RTMUPDATESTATICROUTE, reinterpret_cast<PrismMessageHandler> (&RtmLocalObjectManager::RtmUpdateStaticRouteMessageHandler));
        addOperationMap (RTMLOCALSHOWMESSAGE, reinterpret_cast<PrismMessageHandler> (&RtmLocalObjectManager::RtmShowMessageHandler));
    }

    RtmLocalObjectManager::~RtmLocalObjectManager ()
    {
    }

    RtmLocalObjectManager  *RtmLocalObjectManager::getInstance()
    {
        static RtmLocalObjectManager *pRtmLocalObjectManager = new RtmLocalObjectManager ();

        WaveNs::prismAssert (NULL != pRtmLocalObjectManager, __FILE__, __LINE__);

        return (pRtmLocalObjectManager);
    }

    PrismServiceId  RtmLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *RtmLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case RTMUPDATERTMCONFIG :
                pPrismMessage = new RtmUpdateRtmConfigMessage ();
                break;
            case RTMUPDATESTATICROUTE :
                pPrismMessage = new RtmUpdateStaticRouteMessage ();
                break;
            case RTMLOCALSHOWMESSAGE:
                pPrismMessage = new RtmLocalShowMessage();
                break;
        }

        return (pPrismMessage);
    }

    void RtmLocalObjectManager::multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "RtmLocalObjectManager::multiPartitionPartialCleanup : Entering ... ");

        ObjectId ownerPartitionManagedObjectId (pWaveAsynchronousContextForMultiPartitionCleanup->getOwnerPartitionManagedObjectId ());

        startTransaction();

        vector<string>  managedClasses;
        managedClasses.push_back(NhStaticRouteLocalManagedObject::getClassName ());
        managedClasses.push_back(NhVrfLocalManagedObject::getClassName ());

        string managedClassName;

        for (unsigned int i = 0; i < managedClasses.size(); i++)
        {
            managedClassName = managedClasses[i];
            WaveManagedObjectSynchronousQueryContextForDeletion *waveManagedObjectSynchronousQueryContextForDeletion = new WaveManagedObjectSynchronousQueryContextForDeletion (managedClassName);
            waveManagedObjectSynchronousQueryContextForDeletion->addAndAttribute (new AttributeObjectId (ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));

            deleteMultipleWaveManagedObjects (waveManagedObjectSynchronousQueryContextForDeletion);
        }

        ResourceId status =  commitTransaction(); 

        if (FRAMEWORK_SUCCESS == status)
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        }
        else 
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (status);
        }

        pWaveAsynchronousContextForMultiPartitionCleanup->callback ();
    }

    WaveManagedObject  *RtmLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((RtmConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RtmConfigLocalManagedObject(this);
        }
        else if ((RtmNhProtoLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RtmNhProtoLocalManagedObject(this);
        }
        else if ((NhStaticRouteLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NhStaticRouteLocalManagedObject(this);
        }
        else if ((IfStaticRouteLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new IfStaticRouteLocalManagedObject(this);
        }
        else if ((NhVrfLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new NhVrfLocalManagedObject(this);
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "RtmLocalObjectManager::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    ResourceId RtmLocalObjectManager::getRtmConfigMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
                                                      RtmConfigLocalManagedObject **pMo, bool create)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmConfigMo:  "+RtmConfigLocalManagedObject::getClassName());

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RtmConfigLocalManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            if (pResults->size () == 1) {
                *pMo = dynamic_cast<RtmConfigLocalManagedObject*> ((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmConfigMo: Successfully query RTM config MO \n");
                status = WAVE_MESSAGE_SUCCESS;
                create = false;
            } 
            pResults->clear();
            delete pResults;
        } else {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmConfigMo: Fail to query \n");
        }

        if (*pMo == NULL && create == true) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmConfigMo: create RTM config MO \n");
            *pMo = new RtmConfigLocalManagedObject(dynamic_cast<RtmLocalObjectManager*>(getPWaveObjectManager()));
            status = WAVE_MESSAGE_SUCCESS;
        }

        return (status);
    }

    ResourceId RtmLocalObjectManager::getNhProtoMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
                                                    RtmNhProtoLocalManagedObject **pMo, 
                                                    RtmUpdateRtmConfigMessage *pMsg, bool create)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getNhProtoMo:  "+RtmNhProtoLocalManagedObject::getClassName());

        UI8 nhProto = pMsg->getNhProto();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(RtmNhProtoLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeUI8(&nhProto,"nhProto"));
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            if (pResults->size () == 1) {
                *pMo = dynamic_cast<RtmNhProtoLocalManagedObject*> ((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getNhProtoMo: Successfully query RTM nhProto MO \n");
                status = WAVE_MESSAGE_SUCCESS;
                create = false;
            } 
            pResults->clear();
            delete pResults;
        } else {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getNhProtoMo: Fail to query \n");
        }

        if (*pMo == NULL && create == true) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getNhProtoMo: create nhProto MO \n");
            *pMo = new RtmNhProtoLocalManagedObject(dynamic_cast<RtmLocalObjectManager*>(getPWaveObjectManager()));
            status = WAVE_MESSAGE_SUCCESS;
        }

        return (status);
    }

    ResourceId RtmLocalObjectManager::getNhVrfRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
                                                          NhVrfLocalManagedObject **pMo, 
                                                          RtmUpdateStaticRouteMessage *pMsg, bool create)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhVrfRouteMo:  "+NhVrfLocalManagedObject::getClassName());

        IpV4AddressNetworkMask dest = pMsg->getDest();
        ObjectId nextHopVrf ;
        VrfUtils::getObjectIdByVrfName(pMsg->getNextHopVrf(), nextHopVrf);
        IpV4Address nhIp = pMsg->getNhIp();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NhVrfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&dest,"dest"));
        syncQueryCtxt.addAndAttribute (new AttributeObjectId(&nextHopVrf, "vrf"));
        syncQueryCtxt.addAndAttribute (new AttributeIpV4Address(&nhIp,"nhIp"));
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            if (pResults->size () == 1) {
                *pMo = dynamic_cast<NhVrfLocalManagedObject*> ((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhVrfLocalMo: Successfully query RTM config MO \n");
                status = WAVE_MESSAGE_SUCCESS;
                create = false;
            } 
   
            pResults->clear();
            delete pResults;
        } else {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhVrfMo: Not Present \n");
        }

        if (*pMo == NULL && create == true) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhVrfMo: create nh vrf route MO \n");
            *pMo = new NhVrfLocalManagedObject(dynamic_cast<RtmLocalObjectManager*>(getPWaveObjectManager()));
            status = WAVE_MESSAGE_SUCCESS;
        }

        return (status);
    }

    ResourceId RtmLocalObjectManager::getNhStaticRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
                                                          NhStaticRouteLocalManagedObject **pMo, 
                                                          RtmUpdateStaticRouteMessage *pMsg, bool create)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhStaticRouteMo:  "+NhStaticRouteLocalManagedObject::getClassName());

        IpV4AddressNetworkMask dest = pMsg->getDest();
        IpV4Address nhIp = pMsg->getNhIp();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NhStaticRouteLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&dest,"dest"));
        syncQueryCtxt.addAndAttribute (new AttributeIpV4Address(&nhIp,"nhIp"));
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            if (pResults->size () == 1) {
                *pMo = dynamic_cast<NhStaticRouteLocalManagedObject*> ((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhStaticRouteMo: Successfully query RTM config MO \n");
                status = WAVE_MESSAGE_SUCCESS;
                create = false;
            } 
   
            pResults->clear();
            delete pResults;
        } else {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmNhStaticRouteMo: Fail to query \n");
        }

        if (*pMo == NULL && create == true) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getNhStaticRouteMo: create nh static route MO \n");
            *pMo = new NhStaticRouteLocalManagedObject(dynamic_cast<RtmLocalObjectManager*>(getPWaveObjectManager()));
            status = WAVE_MESSAGE_SUCCESS;
        }

        return (status);
    }

    ResourceId RtmLocalObjectManager::getIfStaticRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
                                                          IfStaticRouteLocalManagedObject **pMo, 
                                                          RtmUpdateStaticRouteMessage *pMsg, bool create)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmIfStaticRouteMo:  "+IfStaticRouteLocalManagedObject::getClassName());

        IpV4AddressNetworkMask dest = pMsg->getDest();
        UI32 ifType = pMsg->getIfType();
        string ifName = pMsg->getIfName();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IfStaticRouteLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeIpV4AddressNetworkMask(&dest,"dest"));
        syncQueryCtxt.addAndAttribute (new AttributeUI32(&ifType,"ifType"));
        syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            if (pResults->size () == 1) {
                *pMo = dynamic_cast<IfStaticRouteLocalManagedObject*> ((*pResults)[0]);
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmIfStaticRouteMo: Successfully query RTM config MO \n");
                status = WAVE_MESSAGE_SUCCESS;
                create = false;
            } 
   
            pResults->clear();
            delete pResults;
        } else {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getRtmIfStaticRouteMo: Fail to query \n");
        }

        if (*pMo == NULL && create == true) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::getIfStaticRouteMo: create if static route MO \n");
            *pMo = new IfStaticRouteLocalManagedObject(dynamic_cast<RtmLocalObjectManager*>(getPWaveObjectManager()));
            status = WAVE_MESSAGE_SUCCESS;
        }

        return (status);
    }

    ResourceId RtmLocalObjectManager::processResult(PrismMessage* pLocMsg, DcmManagementInterfaceMessage* pClientMsg, ResourceId status)
    {
        ResourceId clientStatus;

        if(status == WAVE_MESSAGE_SUCCESS)
        {
            status = pClientMsg->getCompletionStatus();
            if(status == WAVE_MESSAGE_SUCCESS) {
                clientStatus = ::mapRtmBackEndErrorsToDcmdErrors(pClientMsg->getClientStatus());
                trace (TRACE_LEVEL_DEBUG, string("RtmLocalObjectManager::processResult: client error code") + clientStatus);
                trace (TRACE_LEVEL_DEBUG, string("RtmLocalObjectManager::processResult: dcmd error code") + status);
                status = clientStatus;
            }
        }

        pLocMsg->setCompletionStatus(status);
        return status;
    }

    ResourceId RtmLocalObjectManager::validateRtmUpdateConfig (PrismSynchronousLinearSequencerContext 
                                                                *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateRtmConfigMessage *pMsg =
            dynamic_cast<RtmUpdateRtmConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::validateRtmUpdateConfig: validate Rtm Config \n");

        if (pMsg->getOpCode() == RTM_MSG_CONFIG_NEXTHOP_SET) {
            string vrfName = pMsg->getPartitionName();
            if (false == vrfName.empty() && rtm_bgp == pMsg->getNhProto()) {
                status = WRC_VRF_BGP_SUPPORT_ERROR;
            }
        }
        return status;
    }


    ResourceId RtmLocalObjectManager::updateAllRtmConfigToBackend (PrismSynchronousLinearSequencerContext 
                                                                   *pPrismSynchronousLinearSequencerContext)
    {

        RtmUpdateRtmConfigMessage *pMsg =
            dynamic_cast<RtmUpdateRtmConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateAllRtmConfigToBackend: Update rtmd \n");

        RtmClientConfigMessage* clientMsg = new RtmClientConfigMessage(*pMsg);

        status = sendSynchronouslyToWaveClient (RTM_DCM_CLIENT_NAME, clientMsg);
        status = processResult(pMsg, clientMsg, status);

        delete clientMsg;
        return status;
    }

    ResourceId RtmLocalObjectManager::updateAllRtmConfig (PrismSynchronousLinearSequencerContext 
                                                          *pPrismSynchronousLinearSequencerContext)
    {

        RtmUpdateRtmConfigMessage *pMsg =
            dynamic_cast<RtmUpdateRtmConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        UI32 opCode;
        ResourceId status;

        opCode = pMsg->getOpCode();

        if (opCode == RTM_MSG_CONFIG_NEXTHOP_SET || opCode == RTM_MSG_CONFIG_NEXTHOP_UNSET) {
            status = updateNhProto(pPrismSynchronousLinearSequencerContext);
        } else {
            status = updateRtmConfig(pPrismSynchronousLinearSequencerContext);
        }
        return status;
    }

    ResourceId RtmLocalObjectManager::updateRtmConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateRtmConfigMessage *pMsg =
            dynamic_cast<RtmUpdateRtmConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        RtmConfigLocalManagedObject *pMo = NULL;
        int status;


        if ((status = getRtmConfigMo(pPrismSynchronousLinearSequencerContext, &pMo, true)) != WAVE_MESSAGE_SUCCESS) {

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: failed to get RTM config MO \n");
            return status;
        }
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Got RTM config MO \n");
        switch(pMsg->getOpCode())
        {
        case RTM_MSG_CONFIG_ROUTER_ID_SET: 
            pMo->setRouterID(pMsg->getRouterId(), true);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Update Router-id \n");
            break;          
        case RTM_MSG_CONFIG_LOADSHARING_SET:
            pMo->setLoadSharing(pMsg->getLoadSharing(), true);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Update load sharing \n");
            break;          
        case RTM_MSG_CONFIG_ENABLEDEFAULT_SET:
            pMo->setNhDefault(pMsg->getNhDefault());
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Update nh default \n");
            break;          
        case RTM_MSG_CONFIG_NEXTHOPRECURSION_SET:
            pMo->setNhRecur(pMsg->getNhRecur(), true);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Update nh recur \n");
            break;          
        case RTM_MSG_CONFIG_ROUTER_ID_UNSET:
            pMo->unsetRouterID();
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Unset router id \n");
            break;
        case RTM_MSG_CONFIG_LOADSHARING_UNSET:
            pMo->setLoadSharing(RTM_CONFIG_DEFAULT_LOADSHARING);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Unset load sharing \n");
            break;
        case RTM_MSG_CONFIG_ENABLEDEFAULT_UNSET:
            pMo->setNhDefault(false);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Unset nh default \n");
            break;
        case RTM_MSG_CONFIG_NEXTHOPRECURSION_UNSET:
            pMo->setNhRecur(RTM_CONFIG_DEFAULT_NH_RECUR);
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateRtmConfig: Unset nh recur \n");
            break;
        }

        if (pMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
            updateWaveManagedObject(pMo);
        }

        pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId RtmLocalObjectManager::updateNhProto (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateRtmConfigMessage *pMsg =
            dynamic_cast<RtmUpdateRtmConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        RtmNhProtoLocalManagedObject *pMo = NULL;
        int status;
        UI32 opCode;

        opCode = pMsg->getOpCode();
        if ((status = getNhProtoMo(pPrismSynchronousLinearSequencerContext, 
                                     &pMo, pMsg, opCode == RTM_MSG_CONFIG_NEXTHOP_SET)) != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhProto: failed to get nhProto MO \n");
            return status;
        }       
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhProto: Got nh Proto MO \n");
        switch(opCode)
        {
        case RTM_MSG_CONFIG_NEXTHOP_SET:
            pMo->setNhProto(pMsg->getNhProto());
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhProto: Update nh proto \n");
            break;          
        case RTM_MSG_CONFIG_NEXTHOP_UNSET:
            if (pMo == NULL) {
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhProto: trying to remove unexist nhProto \n");
            } else {
                delete pMo;
                pMo = NULL;
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhProto: Delete nhProto \n");
            }
            break;
        }

        if (pMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
            updateWaveManagedObject(pMo);
        }

        pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return WAVE_MESSAGE_SUCCESS;
    }

	int32_t RtmLocalObjectManager::getSlotFromIfName (UI32 ifType, string ifName)
	{
		if (ifType == 3 || ifType == 4 || ifType == 5) {
			return WAVE_INVALID_SLOT;
		}

		return NsmUtils::getSlotId(ifName);
	}

    ResourceId RtmLocalObjectManager::updateIfStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateStaticRouteMessage *pMsg =
            dynamic_cast<RtmUpdateStaticRouteMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        IfStaticRouteLocalManagedObject *pMo = NULL;
        int status;
        UI32 opCode;
        UI32UC cost, distance, tag;
		int32_t slot_id;

        opCode = pMsg->getOpCode();
        if ((status = getIfStaticRouteMo(pPrismSynchronousLinearSequencerContext, 
                                         &pMo, pMsg, opCode == RTM_MSG_IF_STATIC_ROUTE_SET)) != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateIfRtmStaticRoute: failed to get RTM static route MO \n");
            return status;
        }       
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateIfRtmStaticRoute: Got RTM static route MO \n");
        switch(pMsg->getOpCode())
        {
        case RTM_MSG_IF_STATIC_ROUTE_SET: 
            pMo->setDest(pMsg->getDest());
            pMo->setIfType(pMsg->getIfType());
            pMo->setIfName(pMsg->getIfName());
			slot_id = getSlotFromIfName(pMo->getIfType(), pMo->getIfName());
			if (slot_id != (int32_t)WAVE_INVALID_SLOT) {
				pMo->associateWithSlot(slot_id);
			}

            cost = pMsg->getCost();
            distance = pMsg->getDistance();
            tag = pMsg->getTag();
            if (cost.getIsUserConfigured()) {
                pMo->setCost(cost.getUI32Value(), true);
            } else {
                pMo->setCost(RTM_STATIC_ROUTE_DEFAULT_METRIC);
            }   
            if (distance.getIsUserConfigured()) {
                pMo->setDistance(distance.getUI32Value(), true);                
            } else {
                pMo->setDistance(RTM_STATIC_ROUTE_DEFAULT_DISTANCE);
            }
            if (tag.getIsUserConfigured()) {
                pMo->setTag(tag.getUI32Value(), true);              
            } else {
                pMo->setTag(RTM_STATIC_ROUTE_DEFAULT_TAG);
            }

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Update RTM static route \n");
            break;          
        case RTM_MSG_IF_STATIC_ROUTE_UNSET:
            if (pMo == NULL) {
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: trying to remove unexist RTM static route \n");
            } else {
                delete pMo;
                pMo = NULL;
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Delete RTM static route \n");
            }
            break;

        }

        if (pMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
            updateWaveManagedObject(pMo);
        }

        pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return WAVE_MESSAGE_SUCCESS;
    }
    ResourceId RtmLocalObjectManager::updateNhVrf(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateStaticRouteMessage *pMsg =
            dynamic_cast<RtmUpdateStaticRouteMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        NhVrfLocalManagedObject *pMo = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        UI32 opCode;
        opCode = pMsg->getOpCode();
        if ((status = getNhVrfRouteMo(pPrismSynchronousLinearSequencerContext, 
                                         &pMo, pMsg, opCode == RTM_MSG_NH_VRF_SET)) != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmVrfRoute: failed to get RTM NhVrf route MO \n");
            return status;
        }       
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Got RTM NhVrf route MO \n");
        switch(opCode)
        {
        case RTM_MSG_NH_VRF_SET:
            {
                pMo->setDest(pMsg->getDest());
                ObjectId nextHopVrf;
                VrfUtils::getObjectIdByVrfName(pMsg->getNextHopVrf(), nextHopVrf);
                pMo->setNextHopVrf(nextHopVrf);
                pMo->setNhIp(pMsg->getNhIp());
                pMo->setVrf(pMsg->getPartitionName());
            }
            break;
        case RTM_MSG_NH_VRF_UNSET:
            if(pMo)
                delete pMo;
            pMo=NULL;
            break;
        }
        if (pMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
            updateWaveManagedObject(pMo);
        }

        pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId RtmLocalObjectManager::updateNhStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateStaticRouteMessage *pMsg =
            dynamic_cast<RtmUpdateStaticRouteMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        NhStaticRouteLocalManagedObject *pMo = NULL;
        int status;
        UI32 opCode;
        UI32UC cost, distance, tag;

        opCode = pMsg->getOpCode();
        if ((status = getNhStaticRouteMo(pPrismSynchronousLinearSequencerContext, 
                                         &pMo, pMsg, opCode == RTM_MSG_NH_STATIC_ROUTE_SET)) != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: failed to get RTM static route MO \n");
            return status;
        }       
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Got RTM static route MO \n");
        switch(pMsg->getOpCode())
        {
        case RTM_MSG_NH_STATIC_ROUTE_SET: 
            pMo->setDest(pMsg->getDest());
            pMo->setNhIp(pMsg->getNhIp());

            cost = pMsg->getCost();
            distance = pMsg->getDistance();
            tag = pMsg->getTag();
            if (cost.getIsUserConfigured()) {
                pMo->setCost(cost.getUI32Value(), true);
            } else {
                pMo->setCost(RTM_STATIC_ROUTE_DEFAULT_METRIC);
            }   
            if (distance.getIsUserConfigured()) {
                pMo->setDistance(distance.getUI32Value(), true);                
            } else {
                pMo->setDistance(RTM_STATIC_ROUTE_DEFAULT_DISTANCE);
            }
            if (tag.getIsUserConfigured()) {
                pMo->setTag(tag.getUI32Value(), true);                
            } else {
                pMo->setTag(RTM_STATIC_ROUTE_DEFAULT_TAG);
            }

            trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Update RTM static route \n");
            break;          
        case RTM_MSG_NH_STATIC_ROUTE_UNSET:
            if (pMo == NULL) {
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: trying to remove unexist RTM static route \n");
            } else {
                delete pMo;
                pMo = NULL;
                trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateNhRtmStaticRoute: Delete RTM static route \n");
            }
            break;

        }

        if (pMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
            updateWaveManagedObject(pMo);
        }

        pMsg->setCompletionStatus(WAVE_MESSAGE_SUCCESS);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId RtmLocalObjectManager::validateStaticRouteConfig (PrismSynchronousLinearSequencerContext *syncSeqContext)
    {
        RtmUpdateStaticRouteMessage *msgPtr =
                    dynamic_cast<RtmUpdateStaticRouteMessage*> (syncSeqContext->getPPrismMessage());
        switch(msgPtr->getOpCode()) {
        case RTM_MSG_IF_STATIC_ROUTE_SET:
            return validateIfName(msgPtr->getIfType(), msgPtr->getIfName(), syncSeqContext);
        case RTM_MSG_NH_VRF_SET:
            if(false == VrfUtils::isVrfExists(msgPtr->getNextHopVrf(), THIS_LOCATION))
                return WRC_RTM_ERR_INVALID_VRF;
            break;
        }
        return WAVE_MESSAGE_SUCCESS;
    }


    static bool isValidNumber (const string &val, UI32 min, UI32 max)
    {
        char *endp = NULL;
        UI32 n = strtoul(val.c_str(), &endp, 10);
        return (endp != NULL) && (*endp == 0) && (n >= min) && (n <= max);
    }

    ResourceId RtmLocalObjectManager::validateIfName (UI32 ifType, const string& ifName, PrismSynchronousLinearSequencerContext *syncSeqContext)
    {
        tracePrintf (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::validateIfName : ifName=%s, ifType=%d", ifName.c_str(), ifType);
        ResourceId status = WAVE_MESSAGE_SUCCESS;


        switch (ifType) {
        case RTM_IF_TYPE_GIGABIT:
            if (!NsmUtils::isValidInterface(ifName, IF_TYPE_GI, THIS_LOCATION)) {
                trace(TRACE_LEVEL_ERROR, "RtmLocalObjectManager::validateIfName : No such interface - Gi " + ifName);
                status = WRC_NSM_ERR_PHYINTF_INVALID_ONEGIGE;
            }
            break;

        case RTM_IF_TYPE_TENGIGABIT:
            if (!NsmUtils::isValidInterface(ifName, IF_TYPE_TE, THIS_LOCATION)) {
                trace(TRACE_LEVEL_ERROR, "RtmLocalObjectManager::validateIfName : No such interface - Te " + ifName);
                status = WRC_NSM_ERR_PHYINTF_INVALID_TENGIGE;
            }
            break;

        case RTM_IF_TYPE_FORTYGIGABIT:
            if (!NsmUtils::isValidInterface(ifName, IF_TYPE_FO, THIS_LOCATION)) {
                trace(TRACE_LEVEL_ERROR, "RtmLocalObjectManager::validateIfName : No such interface - Fo " + ifName);
                status = WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
            }
            break;

        case RTM_IF_TYPE_PORTCHANNEL:
            if (!isValidNumber(ifName, 1, DCM_PORTCHANNEL_MAX_VLAGS)) {
                trace(TRACE_LEVEL_ERROR, "RtmLocalObjectManager::validateIfName : Invalid port-channel id " + ifName);
                status = WRC_RTM_ERR_PO_OUT_OF_RANGE;
            }
            break;

        case RTM_IF_TYPE_VE:
            if (!isValidNumber(ifName, 2, MAX_ALLOWED_VLAN_ID)) {
                trace(TRACE_LEVEL_ERROR, "RtmLocalObjectManager::validateIfName : Invalid VE id " + ifName);
                status = WRC_RTM_ERR_VE_OUT_OF_RANGE; 
            }
            break;

        case RTM_IF_TYPE_NULL:
            if (ifName.compare("0")) {
                status = WRC_WRC_RTM_ERR_STATIC_INVALID_NULL_INTF;
            }
            break;
        }

        return status;
    }

    ResourceId RtmLocalObjectManager::updateStaticRouteToBackend (PrismSynchronousLinearSequencerContext 
                                                                  *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateStaticRouteMessage *pMsg =
            dynamic_cast<RtmUpdateStaticRouteMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::updateStaticRouteToBackend: Update rtmd \n");
        RtmClientStaticRouteMessage* clientMsg = new RtmClientStaticRouteMessage(*pMsg);

        status = sendSynchronouslyToWaveClient (RTM_DCM_CLIENT_NAME, clientMsg);
        status = processResult(pMsg, clientMsg, status);

        delete clientMsg;

        return status;
    }

    ResourceId RtmLocalObjectManager::updateStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        RtmUpdateStaticRouteMessage *pMsg =
            dynamic_cast<RtmUpdateStaticRouteMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        ResourceId status = WAVE_MESSAGE_ERROR;

        switch(pMsg->getOpCode())
        {
        case RTM_MSG_NH_STATIC_ROUTE_SET: 
        case RTM_MSG_NH_STATIC_ROUTE_UNSET:
            status = updateNhStaticRoute(pPrismSynchronousLinearSequencerContext);
            break;
        case RTM_MSG_NH_VRF_SET:
        case RTM_MSG_NH_VRF_UNSET:
            status = updateNhVrf(pPrismSynchronousLinearSequencerContext);
            break;
        case RTM_MSG_IF_STATIC_ROUTE_SET:
        case RTM_MSG_IF_STATIC_ROUTE_UNSET:
            status = updateIfStaticRoute(pPrismSynchronousLinearSequencerContext);
            break;
        }

        return status;
    }

    void  RtmLocalObjectManager::RtmUpdateRtmConfigMessageHandler( RtmUpdateRtmConfigMessage *pRtmUpdateRtmConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::validateRtmUpdateConfig),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::updateAllRtmConfigToBackend),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::updateAllRtmConfig),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pRtmUpdateRtmConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  RtmLocalObjectManager::RtmUpdateStaticRouteMessageHandler( RtmUpdateStaticRouteMessage *pRtmUpdateStaticRouteMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::validateStaticRouteConfig),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::updateStaticRouteToBackend),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::updateStaticRoute),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&RtmLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pRtmUpdateStaticRouteMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void RtmLocalObjectManager::restoreRtmConfig(void)
    {
        RtmClientConfigMessage *msg = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string vrfName = "";
        int i, size;

        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::restoreRtmConfig");

        RtmConfigLocalManagedObject* pCfgMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryRtmCfgCtxt(RtmConfigLocalManagedObject::getClassName());
        syncQueryRtmCfgCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryRtmCfgCtxt);

        if (pResults) {
            size = pResults->size ();
            for (i = 0; i < size; i++) {
                pCfgMo = dynamic_cast<RtmConfigLocalManagedObject*> ((*pResults)[i]);
                if(pCfgMo) {
                    vrfName = "";
                    msg = new RtmClientConfigMessage();
                    if (msg == NULL) {
                        trace(TRACE_LEVEL_ERROR, "Memory allocation for msg pointer failed");
                        return;
                    }
                    vrfName = VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(pCfgMo->getOwnerPartitionManagedObjectId());
                    msg->setRtmConfig(pCfgMo, vrfName);
                    status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, msg);
                    if (status == WAVE_MESSAGE_SUCCESS)
                        status = msg->getCompletionStatus();
                    if (status == WAVE_MESSAGE_SUCCESS)
                        status = ::mapRtmBackEndErrorsToDcmdErrors(msg->getClientStatus());
                    if (status != WAVE_MESSAGE_SUCCESS)
                        trace(TRACE_LEVEL_ERROR, string("restore for RTM nh-recursion failed: ") + status);
                    else
                        trace(TRACE_LEVEL_DEVEL, string ("restore for RTM next-hop-recursion succeed for vrf:") + vrfName);
                    delete msg;
                    msg = NULL;
                }
            }
        }

        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);


        RtmNhProtoLocalManagedObject *pNhProtoMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryNhProtoCtxt(RtmNhProtoLocalManagedObject::getClassName());
        syncQueryNhProtoCtxt.setLocationIdFilter(THIS_LOCATION);
        pResults = querySynchronously (&syncQueryNhProtoCtxt);

        if (pResults) {
            size = pResults->size ();
            for (i = 0; i < size; i++) {
                pNhProtoMo = dynamic_cast<RtmNhProtoLocalManagedObject*> ((*pResults)[i]);
                if(pNhProtoMo) {
                    vrfName = "";
                    msg = new RtmClientConfigMessage();
                    if (msg == NULL) {
                        trace(TRACE_LEVEL_ERROR, "Memory allocation for msg pointer failed");
                        return;
                    }
                    vrfName = VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(pNhProtoMo->getOwnerPartitionManagedObjectId()); 
                    msg->setRtmConfig(pNhProtoMo, vrfName);
                    status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, msg);
                    if (status == WAVE_MESSAGE_SUCCESS)
                        status = msg->getCompletionStatus();
                    if (status == WAVE_MESSAGE_SUCCESS)
                        status = ::mapRtmBackEndErrorsToDcmdErrors(msg->getClientStatus());

                    if (status != WAVE_MESSAGE_SUCCESS)
                        trace(TRACE_LEVEL_ERROR, string("restore for RTM next-hop proto failed--") + status);
                    else
                        trace(TRACE_LEVEL_DEVEL, string ("restore for RTM next-hop proto for vrf:") + vrfName);
                    delete msg;
                    msg = NULL;
                }
            }
        }

        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        trace(TRACE_LEVEL_INFO, "restore for RTM next-hop-recurstion and next-hop proto Config succeed");

    }


	int locate_gw_idx(ipadm_ip_get_set_t *p_getset, ipadm_host_t host_type, int host_instance, int addr_family)
	{
		int i;

		for (i = 0; i < p_getset->num_gws; ++i) {
			if ((p_getset->gw_addr_ids[i].host_instance == host_instance) &&
				(p_getset->gw_addr_ids[i].host_type == host_type) &&
				(p_getset->gw_addr_ids[i].addr_family == addr_family)) {
				return (i);
			}
		}
		return (-1);
	}

	void getIPadmGw(IpV4Address& ip_gateway)
	{
		int rc = 0;  

		ipadm_local_addr_id_t local_addr_ids[MAX_LOCAL_ADDR_TABLE_SIZE];
		struct sockaddr_storage local_addrs[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipv4_prefix_len_t local_prefixes[MAX_LOCAL_ADDR_TABLE_SIZE];
		addr_flags_t local_flags[MAX_LOCAL_ADDR_TABLE_SIZE];
		ipadm_gw_addr_id_t gw_addr_ids[MAX_GW_ADDR_TABLE_SIZE];
		struct sockaddr_storage gw_addrs[MAX_GW_ADDR_TABLE_SIZE];
		addr_flags_t gw_flags[MAX_GW_ADDR_TABLE_SIZE];
		ipadm_ip_get_set_t current; 

		current.num_locals = MAX_LOCAL_ADDR_TABLE_SIZE;
		current.num_gws = MAX_GW_ADDR_TABLE_SIZE;

		current.local_addr_ids = local_addr_ids;
		current.local_addrs = local_addrs;
		current.local_prefixes = local_prefixes;
		current.local_flags = local_flags;
		current.gw_addr_ids = gw_addr_ids;
		current.gw_addrs = gw_addrs;
		current.gw_flags = gw_flags;

		rc = ipAdmLocalAddrIdGet(local_addr_ids, &(current.num_locals));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmLocalAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}
		rc = ipAdmIpAddrGet(&current);
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string ("ipAdmIpAddrGet() returned ") + rc);
			//return;
		}

		//if pizza
		int pizza = !(current.dhcp_flag & IPADM_CHASSIS_MODEL);
		int end_cp_num = 1;

		if (pizza)
			end_cp_num = 0;
		rc = ipAdmGwAddrIdGet(gw_addr_ids, &(current.num_gws));
		if (rc) {
			trace (TRACE_LEVEL_ERROR, string("ipAdmGwAddrIdGet() returned ") + rc);
			//return;
		}

		int j;

		for (j = 0; j <= end_cp_num; j++)
		{
			char *addr_c;
			int idx = 0;
			string ip_str;

			idx = locate_gw_idx(&current, SPECIFIC_CP_HOST, j, AF_INET);
			if (idx < 0) {
				trace (TRACE_LEVEL_ERROR, "RtmLocalObjectManager :could not locate gateway");
			} else {
				if ((current.gw_flags[idx] & IPADM_ADDR_NONE) == IPADM_ADDR_NONE) {
					ip_str = "0.0.0.0";
				}
				else {
					addr_c = inet_ntoa(SOCKADDR_INP(current.gw_addrs[idx])->sin_addr);
					ip_str = addr_c;
				}
				ip_gateway.fromString(ip_str);
				trace (TRACE_LEVEL_INFO, string("rtm: ipAdmGwAddrIdGet() returned ") + ip_gateway.toString());
			}
		}
	}
/*------------------*/
    void RtmLocalObjectManager::restoreNhVrfStaticRoute(void)
    {
        RtmClientStaticRouteMessage *clientMsg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int size = 0;

        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::restoreNhVrfStaticRoute");

        NhVrfLocalManagedObject * pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NhVrfLocalManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            size = pResults->size ();
            for (int i = 0; i < size; i ++) {
                pMo = dynamic_cast<NhVrfLocalManagedObject*> ((*pResults)[i]);

                clientMsg = new RtmClientStaticRouteMessage();
                VrfUtils::setPartitionName(clientMsg, pMo->getVrf());
                clientMsg->m_nhIp = pMo->m_nhIp;
                clientMsg->m_nextHopVrf = VrfUtils::getVrfNameByObjectId(pMo->m_nextHopVrf);
                clientMsg->m_dest = pMo->m_dest;
                clientMsg->m_ifType = 0;
                clientMsg->m_ifName = "";

                status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, clientMsg);
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = clientMsg->getCompletionStatus();
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = ::mapRtmBackEndErrorsToDcmdErrors(clientMsg->getClientStatus());
                if (status != WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_ERROR, string("restore for RTM nh static route failed--") + status);

                delete clientMsg;

            }
        }

		if(pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    }
/*------------------*/
    void RtmLocalObjectManager::restoreNhStaticRoute(void)
    {
        RtmClientStaticRouteMessage *clientMsg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        int size = 0;
		IpV4Address ip_gateway("0.0.0.0"), invalid_ip_gateway("0.0.0.0");
		IpV4AddressNetworkMask prefix("0.0.0.0/0");;
		bool gw_configured = false, gw_exist = false;

        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::restoreNhStaticRoute");

		getIPadmGw(ip_gateway);
		if (ip_gateway != invalid_ip_gateway) {
			gw_configured = true;
		}

        NhStaticRouteLocalManagedObject* pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NhStaticRouteLocalManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            size = pResults->size ();
            for (int i = 0; i < size; i ++) {
                pMo = dynamic_cast<NhStaticRouteLocalManagedObject*> ((*pResults)[i]);
                string vrfName = "";
                clientMsg = new RtmClientStaticRouteMessage();
                clientMsg->setStaticRoute(pMo);
                vrfName = VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(pMo->getOwnerPartitionManagedObjectId ());
                trace (TRACE_LEVEL_INFO, "RtmLocalObjectManager::restoreNhStaticRoute vrf Name " + vrfName + "dest ip " + pMo->getDest().toString() );
                VrfUtils::setPartitionName(clientMsg, vrfName);
                status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, clientMsg);
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = clientMsg->getCompletionStatus();
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = ::mapRtmBackEndErrorsToDcmdErrors(clientMsg->getClientStatus());
                if (status != WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_ERROR, string("restore for RTM nh static route failed--") + status);

                delete clientMsg;

				if (gw_configured && pMo->m_dest == prefix && pMo->m_nhIp == ip_gateway) {
					gw_exist = true;
				}
            }
        }
		if(pResults != NULL)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

        trace(TRACE_LEVEL_INFO, string("restore for RTM nh static route done: ") + size);

		if (gw_configured == true && gw_exist == false) {

			pMo = new NhStaticRouteLocalManagedObject(this);

			pMo->setDest(prefix);
			pMo->setNhIp(ip_gateway);
            pMo->setCost(1, false);
            pMo->setDistance(1, false);
            pMo->setTag(0, false);
				 
			clientMsg = new RtmClientStaticRouteMessage();
			clientMsg->setStaticRoute(pMo);

			status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, clientMsg);
			delete pMo;

			if (status == WAVE_MESSAGE_SUCCESS)
				status = clientMsg->getCompletionStatus();
			if (status == WAVE_MESSAGE_SUCCESS)
				status = ::mapRtmBackEndErrorsToDcmdErrors(clientMsg->getClientStatus());
			if (status != WAVE_MESSAGE_SUCCESS) {
				trace(TRACE_LEVEL_ERROR, string("restore for RTM nh static route failed--") + status);
			}
			else {
				startTransaction ();
				pMo = new NhStaticRouteLocalManagedObject(this);
				pMo->setDest(prefix);
				pMo->setNhIp(ip_gateway);
				pMo->setCost(1, false);
				pMo->setDistance(1, false);
				pMo->setTag(0, false);
				commitTransaction ();

				delete pMo;
			}
			delete clientMsg;
		}
    }

    void RtmLocalObjectManager::restoreIfStaticRoute(UI32 slot)
    {
        RtmClientStaticRouteMessage *clientMsg;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string vrfName = "";
        int size = 0;

        trace (TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::restoreIfStaticRoute");

        IfStaticRouteLocalManagedObject* pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IfStaticRouteLocalManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        syncQueryCtxt.setSlotFilter(slot);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            size = pResults->size ();
            for (int i = 0; i < size; i ++) {
                pMo = dynamic_cast<IfStaticRouteLocalManagedObject*> ((*pResults)[i]);

                clientMsg = new RtmClientStaticRouteMessage();
                clientMsg->setStaticRoute(pMo);
                vrfName = VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(pMo->getOwnerPartitionManagedObjectId ());
                trace (TRACE_LEVEL_INFO, "RtmLocalObjectManager::restoreIfStaticRoute vrf Name " + vrfName);
                VrfUtils::setPartitionName(clientMsg, vrfName);

                status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, clientMsg);
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = clientMsg->getCompletionStatus();
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = ::mapRtmBackEndErrorsToDcmdErrors(clientMsg->getClientStatus());
                if (status != WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_ERROR, string("restore for RTM interface static route failed--") + status);

                delete clientMsg;
            }
        }

        trace(TRACE_LEVEL_INFO, string("restore for RTM interface static route done: ") + size);

        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

    }

    void RtmLocalObjectManager::flush_mgmt_routes(void)
    {
        char command[] = "ip route flush proto static";
        system(command);

    }

    void RtmLocalObjectManager::deleteIfStaticRoute(UI32 slot)
    {
        int size = 0;

        trace(TRACE_LEVEL_INFO, string("cleanup RTM interface static route on slot: ") + slot);

        IfStaticRouteLocalManagedObject* pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(IfStaticRouteLocalManagedObject::getClassName());
        syncQueryCtxt.setLocationIdFilter(THIS_LOCATION);
        syncQueryCtxt.setSlotFilter(slot);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
			size = pResults->size ();
			if (size > 0) {
				startTransaction();
				for (int i = 0; i < size; i ++) {
					pMo = dynamic_cast<IfStaticRouteLocalManagedObject*> ((*pResults)[i]);              
					delete pMo;
				}
				commitTransaction();
			}
			pResults->clear();
			delete pResults;
        }

        trace(TRACE_LEVEL_INFO, string("cleanup RTM interface static route done: ") + size);
    }


    void RtmLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string  name = pWaveAsynchronousContextForPostBootPhase->getPassName();

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE5")
        {
            trace (TRACE_LEVEL_INFO, string("RtmLocalObjectManager::postboot Entered with pass:") + name);
            restoreRtmConfig();
            restoreNhStaticRoute();
            restoreIfStaticRoute(WAVE_INVALID_SLOT);
            restoreNhVrfStaticRoute();
            flush_mgmt_routes();
            trace (TRACE_LEVEL_INFO, string("RtmLocalObjectManager::postboot done:") + name);
        }
        if (name == "DCM_POSTBOOT_SLOT_STAGE5") {
            UI32 slot = pWaveAsynchronousContextForPostBootPhase->getSlotNum();

            trace (TRACE_LEVEL_INFO, 
				   string("RtmLocalObjectManager::postboot Entered with pass:") + name + " slot# = " + slot);

            restoreIfStaticRoute(slot);

            trace (TRACE_LEVEL_INFO, 
				   string("RtmLocalObjectManager::postboot done:") + name + " slot# = " + slot);
        }

        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();

    }


    void RtmLocalObjectManager::slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
    {
        UI32 slot = pWaveAsynchronousContextForSlotFailover->getSlotNumber();

        deleteIfStaticRoute(slot);
        pWaveAsynchronousContextForSlotFailover->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();
    }

    void RtmLocalObjectManager::RtmShowMessageHandler (RtmLocalShowMessage *pMessage)
    {
        tracePrintf(TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::RtmShowMessageHandler : Received show message request...");

        RtmShowMessage msg (pMessage);
        ResourceId status = sendSynchronouslyToWaveClient(RTM_DCM_CLIENT_NAME, &msg);

        status = processResult(pMessage, &msg, status);
        msg.transferAllBuffers(pMessage);

        tracePrintf(TRACE_LEVEL_DEBUG, "RtmLocalObjectManager::RtmShowMessageHandler : status = %u", status);

        reply(pMessage);
    }

}


