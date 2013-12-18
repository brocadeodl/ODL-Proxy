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
 *   Author : agidwani                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Igmp/IgmpLocalWorker.h"
#include "ClientInterface/Igmp/IgmpToolkit.h"
#include "Nsm/Local/Igmp/IgmpPoSviIntfLocalConfigMessage.h"
#include "Nsm/Local/Igmp/IgmpSgIntfLocalConfigMessage.h"
#include "Nsm/Local/Igmp/IgmpPhyIntfLocalConfigMessage.h"
#include "Nsm/Local/Igmp/IgmpIntfConfManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "Nsm/Local/Igmp/IgmpSgIntfConfManagedObject.h"
#include "ClientInterface/Igmp/IgmpClientPhyIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpClientPoSviIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpClientSgIntfLocalConfigMessage.h"
#include "ClientInterface/Igmp/IgmpMessageDef.h"
#include "Nsm/Local/NsmTypes.h"
#include "igmp_defines.h"
#include "ClientInterface/Igmps/IgmpsUtils.h"
#include "Utils/Layer3LicenseUtil.h"
#include "Utils/DceContext.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

IgmpLocalWorker::IgmpLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
    : WaveWorker  (pNsmLocalObjectManager)
{
    trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

    IgmpIntfConfManagedObject    IgmpIntfConfManagedObject(pNsmLocalObjectManager);
    IgmpSgIntfConfManagedObject IgmpSgIntfConfManagedObject(pNsmLocalObjectManager);

    IgmpIntfConfManagedObject.setupOrm();
    addManagedClass (IgmpIntfConfManagedObject::getClassName (), this);

    IgmpSgIntfConfManagedObject.setupOrm();
    addManagedClass (IgmpSgIntfConfManagedObject::getClassName (), this);

    addOperationMap (IGMPPOSVIINTFLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpLocalWorker::IgmpPoSviIntfLocalConfigMessageHandler));
    addOperationMap (IGMPSGINTFLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpLocalWorker::IgmpSgIntfLocalConfigMessageHandler));
    addOperationMap (IGMPPHYINTFLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&IgmpLocalWorker::IgmpPhyIntfLocalConfigMessageHandler));
}

IgmpLocalWorker::~IgmpLocalWorker ()
{
}

PrismMessage  *IgmpLocalWorker::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

    switch (operationCode)
    {

        case IGMPPOSVIINTFLOCALCONFIG :
            /* This handles SVI & port-channel config */
            pPrismMessage = new IgmpPoSviIntfLocalConfigMessage ();
            break;
        case IGMPPHYINTFLOCALCONFIG :
            trace(TRACE_LEVEL_INFO, string(__FUNCTION__));
            /* This handles physical port config */
            pPrismMessage = new IgmpPhyIntfLocalConfigMessage ();
            break;
        case IGMPSGINTFLOCALCONFIG :
            /* This handles SG on an SVI & port-channel */
            pPrismMessage = new IgmpSgIntfLocalConfigMessage ();
            break;
        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

/* This function is used to create an MO for show not for config */
WaveManagedObject  *IgmpLocalWorker::createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((IgmpIntfConfManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new IgmpIntfConfManagedObject(
            dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
    }
    else if ((IgmpSgIntfConfManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new IgmpSgIntfConfManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace(TRACE_LEVEL_FATAL, string(__FUNCTION__) + string(": Unknown Managed Class Name : ") + managedClassName);
    }
    return (pWaveManagedObject);
}

/* Utility methods */

    NsmL3ReadyManagedObject* IgmpLocalWorker::PoIdGetL3ReadyMO(UI32 poId,
            const SI32 &mapId, PrismSynchronousLinearSequencerContext *pContext,
            bool fromGlobal) {
        PoIntfManagedObject* pPoIntfMo = NsmL3Util::PoIdGetPoIfMO(poId, mapId, pContext,
                fromGlobal);


        if (pPoIntfMo == NULL) {
            return NULL;
        }
        NsmL3ReadyManagedObject *pMo = PoMoGetL3ReadyMO(pPoIntfMo, pContext);

        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pPoIntfMo);
        } else {
            delete pPoIntfMo;
        }
        return pMo;
    }

    NsmL3ReadyManagedObject* IgmpLocalWorker::PoMoGetL3ReadyMO(
        PoIntfManagedObject* poIntfMO,
        PrismSynchronousLinearSequencerContext *pContext) {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        vector<string> selectFields;
        selectFields.push_back("objectId");
        selectFields.push_back("ownerManagedObjectId");
        selectFields.push_back("igmpIntfConf");
        selectFields.push_back("igmpSgIntfConf");


        NsmNodePoManagedObject *pNodePoMo = (WaveManagedObjectPointer<
                NsmNodePoManagedObject>(poIntfMO->getNodePo())).operator->();
        if (pNodePoMo == NULL) {
            return NULL;
        }

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute( new AttributeObjectId(pNodePoMo->getObjectId(), "ownerManagedObjectId"));
		syncQueryCtxt.addSelectFields(selectFields);
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        UI32 numberOfResults = pResults->size();
        if (numberOfResults == 1) {
            pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);
            // user should add it to GC
            // pContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
            pMo->setIntfType(IF_TYPE_PO);
        } else {
		    delete pResults;
			delete pNodePoMo;
            //TODO: Need to take care of cleaning up the results fetched
        }
        return pMo;
    }

    NsmL3ReadyManagedObject* IgmpLocalWorker::PhyIfNameGetL3ReadyMO(string ifName,
        PrismSynchronousLinearSequencerContext  *pContext,
        bool fromLocal)
    {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        InterfaceType intf_type;
        vector<string> selectFields;
        selectFields.push_back("objectId");
        selectFields.push_back("ownerManagedObjectId");
        selectFields.push_back("igmpIntfConf");
        selectFields.push_back("igmpSgIntfConf");


        PhyIntfLocalManagedObject *pPhyIntfMo =
            NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName, pContext, fromLocal);

        if (pPhyIntfMo == NULL) {
            return NULL;
        }
        intf_type = pPhyIntfMo->getPhyType();
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(
            NsmL3ReadyManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(
            new AttributeObjectId(pPhyIntfMo->getObjectId(),
            "ownerManagedObjectId"));
        syncQueryCtxt.addSelectFields(selectFields);
        pResults = WaveObjectManagerToolKit::querySynchronously(&syncQueryCtxt);

        //Only add pPhyIntfMo to GC
        if (pContext) {
            pContext->addManagedObjectForGarbageCollection(pPhyIntfMo);
        }
        else delete pPhyIntfMo;

        UI32 numberOfResults = pResults->size ();
		NSM_PLUG_DBG_S(string("presults->size() ")+numberOfResults);
        if (numberOfResults == 0 || numberOfResults > 1) {
		    delete pResults;
            return NULL;
        }
        pMo = dynamic_cast<NsmL3ReadyManagedObject*>((*pResults)[0]);

        // user shoudld add it to GC
        // pContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
        pMo->setIntfType(intf_type);
        return pMo;
    }

/***************************************************************************/
/*     Functions for handling config(except SG) on SVIs & port-channels    */
/***************************************************************************/

void  IgmpLocalWorker::IgmpPoSviIntfLocalConfigMessageHandler( IgmpPoSviIntfLocalConfigMessage *pIgmpPoSviIntfLocalConfigMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpPoSviIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpPoSviIntfLocalConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pIgmpPoSviIntfLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId IgmpLocalWorker::UpdateIgmpPoSviIntfLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    int status = WAVE_MESSAGE_ERROR;

    // Check for Layer3 license before sending to backend
    LAYER_3_LICENSE_CHECK();

    IgmpPoSviIntfLocalConfigMessage *pIgmpPoSviIntfLocalConfigMessage = 
        dynamic_cast<IgmpPoSviIntfLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));
    IgmpClientPoSviIntfLocalConfigMessage *m = new IgmpClientPoSviIntfLocalConfigMessage();

    m->setLmqi(pIgmpPoSviIntfLocalConfigMessage->getLmqi());
    m->setQmrt(pIgmpPoSviIntfLocalConfigMessage->getQmrt());
    m->setImmediateLeave(pIgmpPoSviIntfLocalConfigMessage->getImmediateLeave());
    m->setQueryInterval(pIgmpPoSviIntfLocalConfigMessage->getQueryInterval());
    m->setIfName(pIgmpPoSviIntfLocalConfigMessage->getIfName());
    m->setIfType(pIgmpPoSviIntfLocalConfigMessage->getIfType());
    m->setOpCode(pIgmpPoSviIntfLocalConfigMessage->getOpCode());

    trace(TRACE_LEVEL_INFO, string(__FUNCTION__) + string("5")); 
    status = sendSynchronouslyToWaveClient ("IGMP", m);

    ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pIgmpPoSviIntfLocalConfigMessage, m, status);
    delete m;
    return retStatus;

}

/* Function to populate the MO */
ResourceId IgmpLocalWorker::UpdateIgmpPoSviIntfLocalConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    string ifName;
    UI32 ifType;
    NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
    IgmpIntfConfManagedObject *pMo = NULL;
    UI32 cmdCode = 0;
    SI32 mapId = DcmToolKit::getLocalMappedId();

    trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

    IgmpPoSviIntfLocalConfigMessage *pMsg = dynamic_cast<IgmpPoSviIntfLocalConfigMessage *>
        (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

    ifName = pMsg->getIfName();
    ifType = pMsg->getIfType();

    if (ifType == INTERFACE_VLAN_IGMP)  {
        pL3ReadyMo = NsmL3Util::VlanIdGetL3ReadyMO(toUI32(ifName), mapId, pPrismSynchronousLinearSequencerContext, IGMP_FALSE);
    } else if (ifType == INTERFACE_PORT_CHANNEL_IGMP)  {
        pL3ReadyMo = PoIdGetL3ReadyMO(toUI32(ifName), mapId, pPrismSynchronousLinearSequencerContext, IGMP_FALSE);
    }

    if (pL3ReadyMo == NULL) {
        trace(TRACE_LEVEL_INFO, 
              string(__FUNCTION__) + string(": L3ReadyMO not found !!"));
        return WRC_IGMP_ERR_NO_SUCH_IFF;
    }

    pMo = pL3ReadyMo->getIgmpIntfConf();
    if (pMo == NULL) {
        trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": IgmpIntfConfMO does not exist. Creating one."));

        /* Create a new MO since we didn't find one */
        pMo = new IgmpIntfConfManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));

        /* Create the link between the IGMP intf MO and the L3 ready MO */
		pL3ReadyMo->addAttributeToBeUpdated("igmpIntfConf");
        pL3ReadyMo->setIgmpIntfConf(pMo);
        /* Now mark the L3 ready MO for updation */ 
        updateWaveManagedObject (pL3ReadyMo);
    } else {
        trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": IgmpIntfConfMO exists."));
        updateWaveManagedObject (pMo);
    }

    /* Update the fields of the MO if we were able to find it */
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": Updating IgmpIntfConfMO."));

    cmdCode = pMsg->getOpCode();

    if (IS_CMDCODE_SET(cmdCode, IGMP_LMQI)) 
        pMo->setLmqi(pMsg->getLmqi(), pMsg->getLmqi() == IGMP_LAST_MEMBER_QUERY_INTERVAL_DEF  ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_QMRT))
        pMo->setQmrt(pMsg->getQmrt(), pMsg->getQmrt() == IGMP_QUERY_RESPONSE_INTERVAL_DEF ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_QUERY_INTERVAL))
        pMo->setQueryInterval(pMsg->getQueryInterval(), pMsg->getQueryInterval() == IGMP_QUERY_INTERVAL_DEF ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_IMMEDIATE_LEAVE))
        pMo->setImmediateLeave(pMsg->getImmediateLeave(), pMsg->getImmediateLeave() == false ? false : true);

  if (pL3ReadyMo)
      pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
  //if(pMo)
	//	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);

    return WAVE_MESSAGE_SUCCESS;
}

/*********************************************************************/
/*    Functions for handling config(except SG) on physical ports     */
/*********************************************************************/

void  IgmpLocalWorker::IgmpPhyIntfLocalConfigMessageHandler( IgmpPhyIntfLocalConfigMessage *pIgmpPhyIntfLocalConfigMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::validateIgmpPhyIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpPhyIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpPhyIntfLocalConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pIgmpPhyIntfLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}

ResourceId IgmpLocalWorker::validateIgmpPhyIntfLocalConfigStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
{
    IgmpPhyIntfLocalConfigMessage *pIgmpPhyIntfLocalConfigMessage =
        dynamic_cast<IgmpPhyIntfLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());   

    string intfName = pIgmpPhyIntfLocalConfigMessage->getIfName();
    PhyIntfLocalManagedObject *pPhyIntfMo =
        NsmL3Util::PhyIfNameGetPhyIfLocalMO(intfName, pPrismSynchronousLinearSequencerContext, IGMP_TRUE);

    if (pPhyIntfMo == NULL) {
        return WRC_IGMP_ERR_INTERFACE_DOES_NOT_EXIST;
    }

    pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfMo);

    if(pPhyIntfMo->getPoId() != ObjectId::NullObjectId) {
       return WRC_IGMP_ERR_L2_CONFIG_PRESENT; 
    }

    if(pPhyIntfMo->getL3ReadyMO() == NULL) {
       return WRC_IGMP_ERR_L3_NOT_READY;
    }

    pPrismSynchronousLinearSequencerContext->setPWaveManagedObject(pPhyIntfMo);
    return WAVE_MESSAGE_SUCCESS; 
}

/* Function to send the physical port config to the backend */
ResourceId IgmpLocalWorker::UpdateIgmpPhyIntfLocalConfigStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
{
    int status = WAVE_MESSAGE_ERROR;

    // Check for Layer3 license before sending to backend
    LAYER_3_LICENSE_CHECK();

    IgmpPhyIntfLocalConfigMessage *pIgmpPhyIntfLocalConfigMessage = 
        dynamic_cast<IgmpPhyIntfLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));
    IgmpClientPhyIntfLocalConfigMessage *m = new IgmpClientPhyIntfLocalConfigMessage();

    m->setLmqi(pIgmpPhyIntfLocalConfigMessage->getLmqi());
    m->setQmrt(pIgmpPhyIntfLocalConfigMessage->getQmrt());
    m->setImmediateLeave(pIgmpPhyIntfLocalConfigMessage->getImmediateLeave());
    m->setQueryInterval(pIgmpPhyIntfLocalConfigMessage->getQueryInterval());
	/* Pass 2 tuple name to backend if we are in VCS mode */
	string ifname;
	string intfName = pIgmpPhyIntfLocalConfigMessage->getIfName();
    if(NsmUtils::isValidThreeTuple(intfName)) {
    	NsmUtils::getTwoTupleIfName(intfName, ifname);
    }
    else {
    	ifname = intfName;
    }

    m->setIfName(ifname);
    m->setIfType(pIgmpPhyIntfLocalConfigMessage->getIfType());
    m->setOpCode(pIgmpPhyIntfLocalConfigMessage->getOpCode());

    status = sendSynchronouslyToWaveClient ("IGMP", m);
     
    ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pIgmpPhyIntfLocalConfigMessage, m, status); 
    delete m;
    return retStatus;
}

/* Function to populate the MO */
ResourceId IgmpLocalWorker::UpdateIgmpPhyIntfLocalConfigMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
{
    string ifName;
    NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
    IgmpIntfConfManagedObject *pMo = NULL;
    UI32 cmdCode = 0;
    
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

    IgmpPhyIntfLocalConfigMessage *pMsg = dynamic_cast<IgmpPhyIntfLocalConfigMessage *>
                                  (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
    ifName = pMsg->getIfName();

    PhyIntfLocalManagedObject *pPhyIntfMo = dynamic_cast<PhyIntfLocalManagedObject *> 
        (pPrismSynchronousLinearSequencerContext->getPWaveManagedObject());
    pL3ReadyMo = pPhyIntfMo->getL3ReadyMO();
    pMo = pL3ReadyMo->getIgmpIntfConf();
    if (pMo == NULL) {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": IgmpIntfConfMO does not exist."));

        /* Create a new MO since we didnt find one */
        pMo = new IgmpIntfConfManagedObject(
                  dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
            
        /* Create the link between the IGMP intf MO and the L3 ready MO */
		pL3ReadyMo->addAttributeToBeUpdated("igmpIntfConf");
        pL3ReadyMo->setIgmpIntfConf(pMo);
        /* Now mark the L3 ready MO for updation */ 
        updateWaveManagedObject (pL3ReadyMo);
    } else {
        trace(TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": IgmpIntfConfMO exists"));
        updateWaveManagedObject (pMo);
    }
    
    /* Update the fields of the MO if we were able to find the MO */
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__) + string(": Updating IgmpIntfConfMO"));

    cmdCode = pMsg->getOpCode();

    if (IS_CMDCODE_SET(cmdCode, IGMP_LMQI)) 
        pMo->setLmqi(pMsg->getLmqi(), pMsg->getLmqi() == IGMP_LAST_MEMBER_QUERY_INTERVAL_DEF  ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_QMRT))
        pMo->setQmrt(pMsg->getQmrt(), pMsg->getQmrt() == IGMP_QUERY_RESPONSE_INTERVAL_DEF ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_QUERY_INTERVAL))
        pMo->setQueryInterval(pMsg->getQueryInterval(), pMsg->getQueryInterval() == IGMP_QUERY_INTERVAL_DEF ? false : true);
    else if (IS_CMDCODE_SET(cmdCode, IGMP_IMMEDIATE_LEAVE))
        pMo->setImmediateLeave(pMsg->getImmediateLeave(), pMsg->getImmediateLeave() == false ? false : true);

    //if (pL3ReadyMo)
    //    pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
	//if(pMo)
	  //	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);

    return WAVE_MESSAGE_SUCCESS;
}

/************************************************************/
/*    Functions for handling SG config                      */
/************************************************************/

void  IgmpLocalWorker::IgmpSgIntfLocalConfigMessageHandler( IgmpSgIntfLocalConfigMessage *pIgmpSgIntfLocalConfigMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::validateIgmpSgIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IgmpLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pIgmpSgIntfLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}


/* Function to validate operation versus DB */
ResourceId IgmpLocalWorker::validateIgmpSgIntfLocalConfigStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
{
    int status = WAVE_MESSAGE_SUCCESS;
    NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
    SI32 mapId = DcmToolKit::getLocalMappedId();
    
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

    IgmpSgIntfLocalConfigMessage *pMsg = dynamic_cast<IgmpSgIntfLocalConfigMessage *>
                                  (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

    string ifName = pMsg->getIfName();
    UI32 ifType = pMsg->getIfType();
    if (ifType == INTERFACE_VLAN_IGMP)  {
        pL3ReadyMo = NsmL3Util::VlanIdGetL3ReadyMO(toUI32(ifName), mapId, pPrismSynchronousLinearSequencerContext, IGMP_FALSE);
        if (pL3ReadyMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
        }
    } else if (ifType == INTERFACE_PORT_CHANNEL_IGMP)  {
        pL3ReadyMo = PoIdGetL3ReadyMO(toUI32(ifName), mapId, pPrismSynchronousLinearSequencerContext, IGMP_FALSE);
        if (pL3ReadyMo) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);
        }
    } else if (ifType == INTERFACE_ONEGIG_IGMP || ifType == INTERFACE_TENGIG_IGMP || ifType == INTERFACE_FORTYGIG_IGMP) {
        vector<string> selectFields;
        selectFields.push_back ("ifName");
        selectFields.push_back ("poId");
        selectFields.push_back ("layer3Ready");

        PhyIntfLocalManagedObject *pPhyIntfMo =
           NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName, pPrismSynchronousLinearSequencerContext, IGMP_TRUE, &selectFields);

        if (pPhyIntfMo == NULL)
        {
            return WRC_IGMP_ERR_INTERFACE_DOES_NOT_EXIST;
        }

        pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pPhyIntfMo);

        if(pPhyIntfMo->getPoId() != ObjectId::NullObjectId) {
             return WRC_IGMP_ERR_L2_CONFIG_PRESENT; 
        }

        pL3ReadyMo = pPhyIntfMo->getL3ReadyMO();

        if(pL3ReadyMo == NULL) {
           return WRC_IGMP_ERR_L3_NOT_READY;
        }
    }

    if (pL3ReadyMo == NULL) {
        trace(TRACE_LEVEL_INFO, 
              string(__FUNCTION__) + string(": L3ReadyMO not found !!"));
        return WRC_IGMP_ERR_NO_SUCH_IFF;
    }

    pPrismSynchronousLinearSequencerContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo);

    vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > sgIntfList = pL3ReadyMo->getIgmpSgIntfConf();
    IgmpSgIntfConfManagedObject *pMo = NULL;
    bool found = false;

    int size = sgIntfList.size();
    for(int i = 0; i < size; i++) {
        pMo = sgIntfList[i].operator ->();
        if(pMo != NULL) {
            if(pMo->getGroupAddress() == pMsg->getGroupAddress()) {
                found = true;
                break;
            }
        }
    }

    bool negation = pMsg->getNegation();

    if (found) {
        if(!negation) {
            trace(TRACE_LEVEL_INFO, "IgmpLocalWorker::validateIgmpSgIntfLocalConfigStep: Create for existing object");

            // To prevent a request to create an already-existing object from causing any issues (which is needed for
            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
        }
    }
    else
    {
        if(negation) {
            trace(TRACE_LEVEL_INFO, "IgmpLocalWorker::validateIgmpSgIntfLocalConfigStep: Delete for nonexistent object");

            // To prevent a request to delete a nonexistent object from causing any issues (which is needed for
            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
        }
    }

    return status;
}

/* Function to send the SG config to the backend */
ResourceId IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    int status = WAVE_MESSAGE_ERROR;

    // Check for Layer3 license before sending to backend
    LAYER_3_LICENSE_CHECK();

    IgmpSgIntfLocalConfigMessage *pIgmpSgIntfLocalConfigMessage = 
        dynamic_cast<IgmpSgIntfLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    IgmpClientSgIntfLocalConfigMessage *m = new IgmpClientSgIntfLocalConfigMessage();

    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

    /* Pass 2 tuple name to backend if we are in VCS mode */
    string ifname;
    string intfName = pIgmpSgIntfLocalConfigMessage->getIfName();
    if(NsmUtils::isValidThreeTuple(intfName)) {
        NsmUtils::getTwoTupleIfName(intfName, ifname);
    }
    else {
        ifname = intfName;
    }

    m->setIfName(ifname);
    m->setIfType(pIgmpSgIntfLocalConfigMessage->getIfType());
    m->setGroupAddress(pIgmpSgIntfLocalConfigMessage->getGroupAddress());
    m->setNegation(pIgmpSgIntfLocalConfigMessage->getNegation());

    status = sendSynchronouslyToWaveClient ("IGMP", m);
    pIgmpSgIntfLocalConfigMessage->setCompletionStatus(m->getCompletionStatus());

    ResourceId retStatus = IgmpsUtils::IgmpsStatusReturn (pIgmpSgIntfLocalConfigMessage, m, status);

    delete m;
    return retStatus;
}

/* Function to populate the SG config in the MO */
ResourceId IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigMOStep (DceSynchronousLinearSeqContext *pPrismSynchronousLinearSequencerContext)
{
    NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
    IgmpSgIntfConfManagedObject *pMo = NULL;
    bool negation;
    
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

    IgmpSgIntfLocalConfigMessage *pMsg = dynamic_cast<IgmpSgIntfLocalConfigMessage *>
                                  (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

    pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *>
        (pPrismSynchronousLinearSequencerContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
    if (pL3ReadyMo == NULL) {
        trace(TRACE_LEVEL_INFO, 
              string(__FUNCTION__) + string(": L3ReadyMO not found !!"));
        return WAVE_MESSAGE_ERROR;
    }
            
    negation = pMsg->getNegation();

    if (negation) {
        trace(TRACE_LEVEL_INFO, 
              "IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigMOStep: Deletion");
        vector<WaveManagedObjectPointer<IgmpSgIntfConfManagedObject> > sgIntfList = pL3ReadyMo->getIgmpSgIntfConf();
        int size = sgIntfList.size();
        for(int i = 0; i < size; i++) {
        	pMo = sgIntfList[i].operator ->();
        	if(pMo != NULL) {
				if(pMo->getGroupAddress() == pMsg->getGroupAddress()) {
					deleteFromComposition(NsmL3ReadyManagedObject::getClassName(), IgmpSgIntfConfManagedObject::getClassName(),
						"igmpSgIntfConf", pMo->getOwnerManagedObjectId(), pMo->getObjectId());
				}
        	}
        }
    } else {
        trace(TRACE_LEVEL_INFO, 
              "IgmpLocalWorker::UpdateIgmpSgIntfLocalConfigMOStep: Creation");
        pMo = new IgmpSgIntfConfManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        prismAssert (NULL != pMo, __FILE__, __LINE__);

        pMo->setGroupAddress(pMsg->getGroupAddress());
        pMo->setIfType(pMsg->getIfType());
        pMo->setIfName(pMsg->getIfName());
	    pMo->setOwnerManagedObjectId(pL3ReadyMo->getObjectId());
    	addToComposition(NsmL3ReadyManagedObject::getClassName(), IgmpSgIntfConfManagedObject::getClassName(), "igmpSgIntfConf", pL3ReadyMo->getObjectId(), pMo->getObjectId());		
        pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
    }

    return WAVE_MESSAGE_SUCCESS;
}

}
