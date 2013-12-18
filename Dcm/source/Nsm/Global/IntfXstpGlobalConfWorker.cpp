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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Global/IntfXstpGlobalConfWorker.h"
#include "Nsm/Global/NsmGlobalUpdatePoStpConfigMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoVlanStpConfigMessage.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/NsmUpdatePoVlanStpConfigSPMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "Nsm/Global/NsmGlobalUpdatePoInstanceStpConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalWorker.h"
#include "Mstp/Global/MstpGlobalDeleteVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateVlanConfigMessage.h"
#include "Nsm/Global/NsmGlobalDeleteBridgeXstpConfigMessage.h"
#include "Nsm/Common/NsmUpdateMstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/DceClusterUtils.h"
#include "Utils/DceLinearSeqContext.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include <bitset>
#include <algorithm>


namespace DcmNs
{

    IntfXstpGlobalConfWorker::IntfXstpGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : IntfXstpBaseWorker  (pNsmGlobalObjectManager, IF_TYPE_PO)
    {
    /*    XstpPortConfigManagedObject    XstpPortConfigManagedObject    (pNsmGlobalObjectManager); 
		XstpPortConfigManagedObject.setupOrm ();
        addManagedClass (XstpPortConfigManagedObject::getClassName (), this); 

        XstpPortVlanConfigManagedObject    XstpPortVlanConfigManagedObject    (pNsmGlobalObjectManager);
        XstpPortVlanConfigManagedObject.setupOrm ();
        addManagedClass (XstpPortVlanConfigManagedObject::getClassName (), this);
	*/
        addOperationMap (NSMGLOBALUPDATEPOSTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpGlobalConfWorker::NsmGlobalUpdatePoStpConfigMessageHandler));
        addOperationMap (NSMGLOBALUPDATEPOVLANSTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpGlobalConfWorker::NsmGlobalUpdatePoVlanStpConfigMessageHandler));
        addOperationMap (NSMGLOBALDELETEBRIDGEXSTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpGlobalConfWorker::NsmGlobalDeleteBridgeXstpConfigMessageHandler));
        addOperationMap (NSMGLOBALDELETEBRIDGEINSTANCECONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpGlobalConfWorker::NsmUpdateMstpInstanceConfigMessageHandler));
        addOperationMap (NSMGLOBALUPDATEPOINSTANCESTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpGlobalConfWorker::NsmGlobalUpdatePoInstanceStpConfigMessageHandler));
    }

    IntfXstpGlobalConfWorker::~IntfXstpGlobalConfWorker ()
    {
    }

    PrismMessage  *IntfXstpGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMGLOBALUPDATEPOSTPCONFIG :
                pPrismMessage = new NsmGlobalUpdatePoStpConfigMessage ();
                break;
            case NSMGLOBALUPDATEPOVLANSTPCONFIG :
                pPrismMessage = new NsmGlobalUpdatePoVlanStpConfigMessage ();
                break;
            case NSMGLOBALDELETEBRIDGEXSTPCONFIG :
                pPrismMessage = new NsmGlobalDeleteBridgeXstpConfigMessage ();
                break;
            case NSMGLOBALDELETEBRIDGEINSTANCECONFIG :
                pPrismMessage = NsmUpdateMstpInstanceConfigMessage::forNsmGlobal();
                break;
            case NSMGLOBALUPDATEPOINSTANCESTPCONFIG :
                pPrismMessage = new NsmGlobalUpdatePoInstanceStpConfigMessage ();
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IntfXstpGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;
/*
        if ((XstpPortConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpPortConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((XstpPortVlanConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpPortVlanConfigManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IntfXstpGlobalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
*/
        return (pWaveManagedObject);
    }

    void  IntfXstpGlobalConfWorker::NsmGlobalUpdatePoStpConfigMessageHandler( NsmGlobalUpdatePoStpConfigMessage *pNsmGlobalUpdatePoStpConfigMessage)
    {
	    trace (TRACE_LEVEL_DEBUG, "IntfXstpGlobalConfWorker::NsmGlobalUpdatePoStpConfigMessageHandler: Entry ");

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::validateIntfXstpConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updateGlobalPoStpConfig),
// Your configuration change code goes here
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updateGlobalPoStpConfigMO),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pPrismLinearSequencerContext = new DceLinearSeqContext (pNsmGlobalUpdatePoStpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismLinearSequencerContext->holdAll();
        pPrismLinearSequencerContext->start();
    }

    void IntfXstpGlobalConfWorker::updatePoVlanStpConfig (DceLinearSeqContext *pContext)
    {
        NsmGlobalUpdatePoVlanStpConfigMessage *pGlobalMessage = dynamic_cast<NsmGlobalUpdatePoVlanStpConfigMessage*> (pContext->getPPrismMessage());

        trace (TRACE_LEVEL_DEBUG, "IntfXstpGlobalConfWorker::updatePoVlanStpConfig: Enter\n");
        /* Does Sanity check need here ??? */

        NsmUpdatePoVlanStpConfigSPMessage  *localMessage = new NsmUpdatePoVlanStpConfigSPMessage(*pGlobalMessage);

        sendPoConfigMessageToCluster(pGlobalMessage->getIfName(), localMessage, pContext);
    }

    void  IntfXstpGlobalConfWorker::updatePoVlanStpConfigMO (DceLinearSeqContext *pContext)
    {
        NsmGlobalUpdatePoVlanStpConfigMessage *pGlobalMessage = dynamic_cast<NsmGlobalUpdatePoVlanStpConfigMessage*> (pContext->getPPrismMessage());
        ResourceId status = updateXstpPortVlanMO(pGlobalMessage->getVid(), pGlobalMessage, pContext);

        pContext->executeNextStep(status);
	}

	void
    IntfXstpGlobalConfWorker::IntfXstpGlobalConfSendToClusterCallback(WaveSendToClusterContext  *ctx_p)
    {
        trace (TRACE_LEVEL_DEBUG, "Inside IntfXstpGlobalConfSendToClusterCallback");
        ResourceId sendToClusterCompletionStatus = ClusterGetStatusFromContext(ctx_p);
        PrismLinearSequencerContext *seq_ctx_p =
                reinterpret_cast<PrismLinearSequencerContext *>(ctx_p->getPCallerContext());
        trace (TRACE_LEVEL_DEBUG, string("IntfXstpGlobalConfSendToClusterCallback:") + sendToClusterCompletionStatus);
        cleanupAndExecuteNextStep(ctx_p, seq_ctx_p, sendToClusterCompletionStatus);
    }

    void IntfXstpGlobalConfWorker::cleanupAndExecuteNextStep(WaveSendToClusterContext *pClusterContext
                                                                            , PrismLinearSequencerContext *pContext
                                                                            , ResourceId execStatus)
    {
        delete pClusterContext->getPPrismMessageForPhase1();
        delete pClusterContext->getPPrismMessageForPhase2();
        delete pClusterContext;
        pContext->executeNextStep(execStatus);
    }

	void IntfXstpGlobalConfWorker::sendPoConfigMessageToCluster (const string &poId, PrismMessage *pMessage, PrismLinearSequencerContext *pContext)
	{
	    WaveSendToClusterContext *pClusterCtx = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IntfXstpGlobalConfWorker::IntfXstpGlobalConfSendToClusterCallback), pContext);
        prismAssert(NULL != pClusterCtx, __FILE__, __LINE__);

        pClusterCtx->setPPrismMessageForPhase1(pMessage);

        ResourceId execStatus = WAVE_MESSAGE_SUCCESS;
        int ret = NsmUtils::setLocationIdsForPoSubModeCmds(poId, pClusterCtx);

        switch (ret) {
        case SET_LOCATION_ID_API_SUCCESS:
            pClusterCtx->setPartialSuccessFlag(true);
            pClusterCtx->setTreatFailureOnFailingOverAsSuccessFlag(true); // ????
            sendToWaveCluster(pClusterCtx);
            execStatus = 0; // do executeNextStep
            break;

        case PORT_NOT_PART_OF_ANY_INTERFACE:
            tracePrintf(TRACE_LEVEL_INFO, "IntfXstpGlobalConfWorker::sendPoConfigMessageToCluster : Port channel %s has no members", poId.c_str());
            break;

        default:
            tracePrintf(TRACE_LEVEL_ERROR, "IntfXstpGlobalConfWorker::sendPoConfigMessageToCluster : Unexpected error %d, message dropped!", ret);
            execStatus = WAVE_MESSAGE_ERROR;
            break;
        }

        if (execStatus != 0) {
            cleanupAndExecuteNextStep(pClusterCtx, pContext, execStatus);
        }
	}

	void IntfXstpGlobalConfWorker::sendToCluster (PrismMessage *pMessage, PrismLinearSequencerContext *pSequencerContext)
	{
        WaveSendToClusterContext *pClusterCtx = new WaveSendToClusterContext(this,
            reinterpret_cast<PrismAsynchronousCallback>(&IntfXstpGlobalConfWorker::IntfXstpGlobalConfSendToClusterCallback), pSequencerContext);

        pClusterCtx->setPPrismMessageForPhase1(pMessage);
        pClusterCtx->setPartialSuccessFlag(true);
        pClusterCtx->setTreatFailureOnFailingOverAsSuccessFlag(true); // ????

        sendToWaveCluster(pClusterCtx);
	}

	/* 
	 * Verify and send bridge configure message to localplugin
	 */
	void IntfXstpGlobalConfWorker::updateGlobalPoStpConfig (DceLinearSeqContext *pPrismLinearSequencerContext)
	{
		NsmGlobalUpdatePoStpConfigMessage *pGlobalMessage = dynamic_cast<NsmGlobalUpdatePoStpConfigMessage*> (pPrismLinearSequencerContext->getPPrismMessage ());
        string poName = pGlobalMessage->getIfName();
        UI32 cmdCode = pGlobalMessage->getCmdCode();
        UI32 logicalPortId = 0;

		tracePrintf(TRACE_LEVEL_DEBUG, "IntfXstpGlobalConfWorker::updateGlobalPoStpConfig: Enter; po=%s, cmdCode=%u", poName.c_str(), cmdCode);	

		pGlobalMessage->printMsg();

        if (MstpToolKit::isVcsEnabled()) {
            ResourceId status = WAVE_MESSAGE_SUCCESS;
            switch (cmdCode) {
            case MSTP_MSG_IF_PROTO_ENABLE:
                // Allocate logical port ID
                status = MstpToolKit::allocatePortId(logicalPortId, IF_TYPE_PO, poName);

                if (status != WAVE_MESSAGE_SUCCESS && FrameworkToolKit::isConfigReplayInProgress()) {
                    MstpToolKit::logPortIdAllocError(status, "config-replay", IF_TYPE_PO, poName);
                    cmdCode = MSTP_MSG_IF_NO_PROTO_ENABLE; // Send shut to backend!!!
                    status = WAVE_MESSAGE_SUCCESS;
                }
                break;

            case MSTP_MSG_IF_NO_PROTO_ENABLE:
                status = MstpToolKit::freePortId(IF_TYPE_PO, poName);
                break;
            }

            if (status != WAVE_MESSAGE_SUCCESS) {
                pPrismLinearSequencerContext->executeNextStep(status);
                return;
            }
		}

        NsmUpdatePoStpConfigSPMessage *localMessage = new NsmUpdatePoStpConfigSPMessage(*pGlobalMessage);
        localMessage->setCmdCode(cmdCode);
        if (logicalPortId != 0) {
            localMessage->setPortId(logicalPortId);
            pPrismLinearSequencerContext->getCache()->put(PORT_UID_KEY, logicalPortId);
        }

        sendPoConfigMessageToCluster(poName, localMessage, pPrismLinearSequencerContext);
	}

	/* 
	 * Update the MO Database
	 */
	void IntfXstpGlobalConfWorker::updateGlobalPoStpConfigMO (DceLinearSeqContext *pSequencerContext)
	{
        NsmGlobalUpdatePoStpConfigMessage *pMessage = dynamic_cast<NsmGlobalUpdatePoStpConfigMessage*> (pSequencerContext->getPPrismMessage());
		ResourceId status = updateXstpPortConfigMO(pMessage, pSequencerContext);
		pSequencerContext->executeNextStep(status);
	}

    void  IntfXstpGlobalConfWorker::NsmGlobalUpdatePoVlanStpConfigMessageHandler( NsmGlobalUpdatePoVlanStpConfigMessage *pNsmGlobalUpdatePoVlanStpConfigMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::validateIntfXstpConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updatePoVlanStpConfig),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updatePoVlanStpConfigMO),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pContext = new DceLinearSeqContext (pNsmGlobalUpdatePoVlanStpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pContext->holdAll();
        pContext->start();
    }

	ResourceId IntfXstpGlobalConfWorker::deleteBridgeConfigMO (DceSynchronousLinearSeqContext *pSequencerContext)
	{
	    return clearIntfXstpConfigs("", false, pSequencerContext);
	}

    void  IntfXstpGlobalConfWorker::NsmGlobalDeleteBridgeXstpConfigMessageHandler(NsmGlobalDeleteBridgeXstpConfigMessage *pNsmGlobalDeleteBridgeXstpConfigMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpGlobalConfWorker::deleteBridgeConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmGlobalDeleteBridgeXstpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  IntfXstpGlobalConfWorker::updatePoInstanceStpConfig(DceLinearSeqContext *pContext)
    {
        NsmGlobalUpdatePoInstanceStpConfigMessage *pGlobalMessage = dynamic_cast<NsmGlobalUpdatePoInstanceStpConfigMessage*> (pContext->getPPrismMessage());

        trace (TRACE_LEVEL_DEBUG, "IntfXstpGlobalConfWorker::updatePoInstanceStpConfig: Enter\n");
        /* Does Sanity check need here ??? */

		pGlobalMessage->printMsg();
        NsmUpdatePoInstanceStpConfigSPMessage  *pMessage = new NsmUpdatePoInstanceStpConfigSPMessage(*pGlobalMessage);

        sendPoConfigMessageToCluster(pMessage->getIfName(), pMessage, pContext);
    }

    void  IntfXstpGlobalConfWorker::updatePoInstanceStpConfigMO(DceLinearSeqContext *pContext)
    {
        NsmGlobalUpdatePoInstanceStpConfigMessage *pGlobalMessage = dynamic_cast<NsmGlobalUpdatePoInstanceStpConfigMessage*> (pContext->getPPrismMessage());
        ResourceId status = updateXstpPortInstanceMO(pGlobalMessage->getInstanceId(), pGlobalMessage, pContext);

        pContext->executeNextStep(status);
	}


    void  IntfXstpGlobalConfWorker::NsmGlobalUpdatePoInstanceStpConfigMessageHandler( NsmGlobalUpdatePoInstanceStpConfigMessage *pNsmGlobalUpdatePoInstanceStpConfigMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::validateIntfXstpConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updatePoInstanceStpConfig),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerStartTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::updatePoInstanceStpConfigMO),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&IntfXstpGlobalConfWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pContext = new DceLinearSeqContext (pNsmGlobalUpdatePoInstanceStpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pContext->holdAll();
        pContext->start();
    }
	
	ResourceId IntfXstpGlobalConfWorker::deleteVlanIntfXstp(UI32 vid, DceLinearSeqContext *pSequencerContext)
	{
        ResourceId status = WAVE_MESSAGE_ERROR;
		ObjectId vlanOid = pSequencerContext->getWaveManagedObjectId();

        trace (TRACE_LEVEL_DEBUG, string("IntfXstpGlobalConfWorker::deleteVlanIntfXstp: Start"));

		status = handleVlanDelete(vid, vlanOid, pSequencerContext);

		if (status == WAVE_MESSAGE_SUCCESS) {
		// Notify Mstp plugin
            MstpGlobalDeleteVlanBridgeConfigMessage msg (vid, vlanOid);

        status = sendSynchronously(&msg);
            if (status == WAVE_MESSAGE_SUCCESS)
                status = msg.getCompletionStatus();
		}

		return status;
	}

	void IntfXstpGlobalConfWorker::updateVlanIntfXstp(UI32 vlanId, bool shut, PrismLinearSequencerContext *pSequencerContext)
	{
        tracePrintf(TRACE_LEVEL_DEBUG, true, false, "IntfXstpGlobalConfWorker::updateVlanIntfXstp : "
                "vlan=%u, spanning-tree state=%s", vlanId, (shut ? "shut" : "no shut"));

        MstpUpdateVlanConfigMessage *pMessage = new MstpUpdateVlanConfigMessage();
        pMessage->setId(vlanId);
        pMessage->setCmdCode(shut ? MSTP_MSG_VLAN_STP_DISABLE : MSTP_MSG_VLAN_NO_STP_DISABLE);

        sendToCluster(pMessage, pSequencerContext);
	}

	ResourceId IntfXstpGlobalConfWorker::createPortXstp(PoPhyIntfManagedObject *pIntfMo)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace (TRACE_LEVEL_INFO, string("IntfXstpGlobalConfWorker::createPortXstp: xSTP port config created by defaultconfigurd"));
		return status;
	}

    ResourceId IntfXstpGlobalConfWorker::deletePortXstp(PoIntfManagedObject *pIntfMo, DceLinearSeqContext *pSequencerContext)
    {
        string poId = pIntfMo->getIfName();
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        //ResourceId status = clearIntfXstpConfigs(poId, true, pSequencerContext);

        XstpPortConfigManagedObject *pMo = pIntfMo->m_portStpConfig.operator ->();
        if (pMo != NULL) {
            pMo->setDefaultAttribute(true, false);
            updateWaveManagedObject(pMo);
        }

        pIntfMo->deletePortVlanXstpConfigAll();
        pIntfMo->deletePortInstanceXstpConfigAll();

        if (status == WAVE_MESSAGE_SUCCESS && MstpToolKit::isVcsEnabled()) {
            status = MstpToolKit::freePortId(IF_TYPE_PO, poId, true, pSequencerContext);
        }

        return (status);
    }


    void IntfXstpGlobalConfWorker::validateIntfXstpConfigStep (DceLinearSeqContext *pContext)
    {
        NsmPortStpConfigMessage *m = dynamic_cast<NsmPortStpConfigMessage*>(pContext->getPPrismMessage());

        ResourceId status = validatePortXstpConfig(m, pContext);

        if (m->getOperationCode() == NSMGLOBALUPDATEPOVLANSTPCONFIG && status == WAVE_MESSAGE_SUCCESS) {
            UI32 vid = dynamic_cast<NsmGlobalUpdatePoVlanStpConfigMessage*>(m)->getVid();
            status = MstpToolKit::validateVlanForXstpConfig(vid, pContext);
        }

        pContext->executeNextStep(status);
    }

	ResourceId IntfXstpGlobalConfWorker::fillMo(XstpPortConfigManagedObject *pPortXstpMo, NsmPortStpConfigMessage *pMessage)
	{
	    return pPortXstpMo->setAttributeByMessage((NsmGlobalUpdatePoStpConfigMessage*) pMessage);
	}

	ResourceId IntfXstpGlobalConfWorker::fillMo(XstpPortInstanceConfigManagedObject *pPortInstMo, NsmPortStpConfigMessage *pMessage)
	{
	    return pPortInstMo->setAttributeByMessage((NsmGlobalUpdatePoInstanceStpConfigMessage*) pMessage);
	}

	ResourceId IntfXstpGlobalConfWorker::fillMo(XstpPortVlanConfigManagedObject *pPortVlanMo, NsmPortStpConfigMessage *pMessage)
	{
	    return pPortVlanMo->setAttributeByMessage((NsmGlobalUpdatePoVlanStpConfigMessage*) pMessage);
    }

}
