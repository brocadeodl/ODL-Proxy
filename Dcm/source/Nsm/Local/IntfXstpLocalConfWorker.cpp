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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/IntfXstpLocalConfWorker.h"
#include "Nsm/Local/NsmUpdatePhyStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePhyVlanStpConfigMessage.h"
#include "Nsm/Local/NsmUpdatePoStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePoVlanStpConfigSPMessage.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/XstpPortVlanConfigManagedObject.h"
#include "Nsm/Global/IntfXstpGlobalConfWorker.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "Nsm/Local/XstpPortInstanceConfigManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpPortConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpPortVlanConfigMessage.h"
#include "Nsm/Global/PoPhyIntfManagedObject.h"
#include "Nsm/Local/NsmDeleteBridgeXstpConfigMessage.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/NsmUpdatePoInstanceStpConfigSPMessage.h"
#include "Nsm/Local/NsmUpdatePhyInstanceStpConfigMessage.h"
#include "Nsm/Common/NsmUpdateMstpInstanceConfigMessage.h"
#include "Nsm/Common/PVlanUtils.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"

namespace DcmNs
{

    IntfXstpLocalConfWorker::IntfXstpLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : IntfXstpBaseWorker  (pNsmLocalObjectManager, IF_TYPE_PHY)
    {
        XstpPortConfigManagedObject::initDefaults();
        XstpPortConfigManagedObject    XstpPortConfigManagedObject    (pNsmLocalObjectManager);
        XstpPortConfigManagedObject.setupOrm ();
        addManagedClass (XstpPortConfigManagedObject::getClassName (), this);

	XstpPortInstanceConfigManagedObject    XstpPortInstanceConfigManagedObject    (pNsmLocalObjectManager);
        XstpPortInstanceConfigManagedObject.setupOrm ();
        addManagedClass (XstpPortInstanceConfigManagedObject::getClassName (), this);

        XstpPortVlanConfigManagedObject    XstpPortVlanConfigManagedObject (pNsmLocalObjectManager);
        XstpPortVlanConfigManagedObject.setupOrm ();
        addManagedClass (XstpPortVlanConfigManagedObject::getClassName (), this);

        addOperationMap (NSMUPDATEPHYSTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdatePhyStpConfigMessageHandler));
        addOperationMap (NSMUPDATEPHYVLANSTPCONFIG, reinterpret_cast<PrismMessageHandler>(&IntfXstpLocalConfWorker::NsmUpdatePhyVlanStpConfigMessageHandler));
        addOperationMap (NSMUPDATEPOSTPCONFIGSP, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdatePoStpConfigSPMessageHandler));
        addOperationMap (NSMUPDATEPOVLANSTPCONFIGSP, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdatePoVlanStpConfigSPMessageHandler));
        addOperationMap (NSMDELETEBRIDGEXSTPCONFIG, reinterpret_cast<PrismMessageHandler>(&IntfXstpLocalConfWorker::NsmDeleteBridgeXstpConfigMessageHandler));
        addOperationMap (NSMUPDATEPOINSTANCESTPCONFIGSP, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdatePoInstanceStpConfigSPMessageHandler));
        addOperationMap (NSMUPDATEPHYINSTANCESTPCONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdatePhyInstanceStpConfigMessageHandler));
        addOperationMap (NSM_DELETE_MSTP_INST_CONFIG, reinterpret_cast<PrismMessageHandler> (&IntfXstpLocalConfWorker::NsmUpdateMstpInstanceConfigMessageHandler));
    }

    IntfXstpLocalConfWorker::~IntfXstpLocalConfWorker ()
    {
    }

    PrismMessage  *IntfXstpLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
            case NSMUPDATEPHYSTPCONFIG :
                pPrismMessage = new NsmUpdatePhyStpConfigMessage ();
                break;
            case NSMUPDATEPHYVLANSTPCONFIG :
                pPrismMessage = new NsmUpdatePhyVlanStpConfigMessage ();
                break;
            case NSMUPDATEPOSTPCONFIGSP :
                pPrismMessage = new NsmUpdatePoStpConfigSPMessage ();
                break;
            case NSMUPDATEPOVLANSTPCONFIGSP :
                pPrismMessage = new NsmUpdatePoVlanStpConfigSPMessage ();
                break;
            case NSMDELETEBRIDGEXSTPCONFIG :
                pPrismMessage = new NsmDeleteBridgeXstpConfigMessage ();
                break;
            case NSMUPDATEPOINSTANCESTPCONFIGSP :
                pPrismMessage = new NsmUpdatePoInstanceStpConfigSPMessage ();
                break;
            case NSMUPDATEPHYINSTANCESTPCONFIG :
                pPrismMessage = new NsmUpdatePhyInstanceStpConfigMessage ();
                break;
            case NSM_DELETE_MSTP_INST_CONFIG:
                pPrismMessage = new NsmUpdateMstpInstanceConfigMessage(NsmLocalObjectManager::getPrismServiceId(), operationCode);
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *IntfXstpLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

		if ( (XstpPortConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpPortConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((XstpPortVlanConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpPortVlanConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
	else if ((XstpPortInstanceConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpPortInstanceConfigManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "IntfConfLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

#if 1

	/*
	 * Update the MO Database
	 */
	ResourceId  IntfXstpLocalConfWorker::updatePhyStpConfigMO(DceSynchronousLinearSeqContext *pSequencerContext)
	{
	    NsmUpdatePhyStpConfigMessage *pMessage = dynamic_cast<NsmUpdatePhyStpConfigMessage*> (pSequencerContext->getPPrismMessage());
	    return updateXstpPortConfigMO(pMessage, pSequencerContext);
	}

    ResourceId  IntfXstpLocalConfWorker::updatePhyPortUidStep (DceSynchronousLinearSeqContext *pSequencerContext)
    {
        NsmUpdatePhyStpConfigMessage *pMessage = dynamic_cast<NsmUpdatePhyStpConfigMessage*> (pSequencerContext->getPPrismMessage());
        UI32 size;
        mstp_msg_port_id_info *idBuff = (mstp_msg_port_id_info*) pMessage->findBuffer(MSTP_PORT_ID_ARRAY_BUFTAG, size);
        UI32 idCount = size / sizeof(mstp_msg_port_id_info);
        tracePrintf(TRACE_LEVEL_INFO, "IntfXstpLocalConfWorker::%s : Found %u ids in the message; buffSize=%u", __FUNCTION__, idCount, size);
        if (size == 0 || idBuff == NULL) {
            return WAVE_MESSAGE_SUCCESS;
        }

        char ifName[32] = { 0, };
        SI32 rbridge = DcmToolKit::getLocalMappedId();
        mstp_msg_port_id_info *info;
        if (rbridge < 1) {
            tracePrintf(TRACE_LEVEL_ERROR, "IntfXstpLocalConfWorker::%s : Cannot resolve rbridge id !!", __FUNCTION__);
            return WRC_MSTP_SYSTEM_ERROR;
        }

        for (UI32 i = 0; i < idCount; ++i) {
            info = idBuff + i;
            if (info->if_type == IF_TYPE_PO) {
                strncpy(ifName, (char*) info->if_name, sizeof(ifName)-1);
            } else {
                snprintf(ifName, sizeof(ifName), "%d/%s", rbridge, (char*) info->if_name);
            }

            tracePrintf(TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::%s : Processing idBuff[%u]; ifName=\"%s\", uid=%u", __FUNCTION__, i, ifName, info->port_id);

            WaveManagedObjectSynchronousQueryContextForUpdate *pUpdateCtx = new WaveManagedObjectSynchronousQueryContextForUpdate (XstpPortConfigManagedObject::getClassName());
            fillIntfXstpQueryCriteria(pUpdateCtx, ifName);
            XstpPortConfigManagedObject *pCriteriaMo = dynamic_cast<XstpPortConfigManagedObject*>(pUpdateCtx->getWaveManagedObjectToAddUpdateAttributes());
            if (pCriteriaMo == NULL) {
                tracePrintf(TRACE_LEVEL_ERROR, "IntfXstpLocalConfWorker::%s : NULL criteria MO!", __FUNCTION__);
                return WRC_MSTP_SYSTEM_ERROR;
            }

            pCriteriaMo->setPortUid(info->port_id);
            pCriteriaMo->addAttributeToBeUpdated("portUid");

            updateMultipleWaveManagedObjects(pUpdateCtx);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IntfXstpLocalConfWorker::updatePhyStpConfig (DceSynchronousLinearSeqContext *pSequencerContext)
	{
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		NsmUpdatePhyStpConfigMessage *pLocalMsg = dynamic_cast<NsmUpdatePhyStpConfigMessage*> (pSequencerContext->getPPrismMessage ());

		trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePhyStpConfig: Enter ");
		/* Does Sanity check need here ??? */
		pLocalMsg->printMsg();

        const char *client = MSTP_DCM_CLIENT_NAME;
		MstpPortConfigMessage  message = *pLocalMsg;
		bool vcsEnabled = MstpToolKit::isVcsEnabled();
		string ifName = pLocalMsg->getIfName();

		/*If it is "bpdu-guard enable" command, send it to SSM*/
		switch(pLocalMsg->getCmdCode()) {
		case DCM_SSM_BPDUGUARD_ENABLE:
		case DCM_SSM_BPDUGUARD_NO_ENABLE:
        case DCM_SSM_TAGGED_BPDU_ENABLE:
        case DCM_SSM_TAGGED_BPDU_NO_ENABLE:
			client = "ssm";
			break;

		case MSTP_MSG_IF_PROTO_ENABLE:
		    if (vcsEnabled) {
                status = MstpToolKit::allocatePortId(message.m_portId, pLocalMsg->getIfType(), ifName);

                if (status == WAVE_MESSAGE_SUCCESS) {
                    pSequencerContext->getCache()->put(PORT_UID_KEY, message.m_portId);
                }
                else if (FrameworkToolKit::isConfigReplayInProgress()) {
                    MstpToolKit::logPortIdAllocError(status, "config-replay", pLocalMsg->getIfType(), ifName);
                    message.setCmdCode(MSTP_MSG_IF_NO_PROTO_ENABLE); // Send shut to backend!!
                    status = WAVE_MESSAGE_SUCCESS;
                }
		    }
		    break;

		case MSTP_MSG_IF_NO_PROTO_ENABLE:
		    if (vcsEnabled) {
		        status = MstpToolKit::freePortId(pLocalMsg->getIfType(), ifName);
		    }
		    break;
		}
		if (status == WAVE_MESSAGE_SUCCESS) {
		    tracePrintf(TRACE_LEVEL_DEBUG, "Going to calling sendSynchToWaveClient(%s)", client);
            /*
            * Checkout whether the command is a NO command. If so, then
            * checkout the mode the interface is in.
            * If it is an ISL then return from there and dont passon the
            * request to the backend
            */

		    if (pLocalMsg->getIsNoCmd()) {
                bool isISL = false;
                if (WAVE_MESSAGE_SUCCESS != NsmUtils::checkIsISLLink(message.getIfName(), isISL)) {
                    return WAVE_MESSAGE_ERROR;
                }

                if (isISL) {
			        return status;
                }
		    }
            status = MstpToolKit::sendToBackend(client, &message);
		}
		return status;
	}

#endif
    void  IntfXstpLocalConfWorker::NsmUpdatePhyStpConfigMessageHandler( NsmUpdatePhyStpConfigMessage *pNsmUpdatePhyStpConfigMessage)
    {
        UI32 n = 0;
        PrismSynchronousLinearSequencerStep sequencerSteps[10];

        if (pNsmUpdatePhyStpConfigMessage->getCmdCode() == MSTP_MSG_SET_PORT_ID) {
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyPortUidStep);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep);
        }
        else {
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::validateIntfXstpConfigStep);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyStpConfig);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyStpConfigMO);
            sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep);
        }

        sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep);
        sequencerSteps[n++] = reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep);

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext(pNsmUpdatePhyStpConfigMessage, this, sequencerSteps, n);

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::updatePhyVlanStpConfig(DceSynchronousLinearSeqContext *pSequencerContext)
    {
        NsmUpdatePhyVlanStpConfigMessage *pLocalMessage = dynamic_cast<NsmUpdatePhyVlanStpConfigMessage*> (pSequencerContext->getPPrismMessage());

        /*
        * Checkout whether the command is a NO command. If so, then
        * checkout the mode the interface is in.
        * If it is an ISL then return from there and dont passon the
        * request to the backend
        */
	    if (pLocalMessage->getIsNoCmd()) {
            bool isISL = false;
            if (WAVE_MESSAGE_SUCCESS != NsmUtils::checkIsISLLink(pLocalMessage->getIfName(), isISL)) {
                return WAVE_MESSAGE_ERROR;
            }

            if (isISL) {
                return WAVE_MESSAGE_SUCCESS;
            }
	    }
        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePhyVlanStpConfig: Enter \n");
        /* Does Sanity check need here ??? */

        MstpPortVlanConfigMessage  message = *pLocalMessage;
        return MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);
    }

    ResourceId IntfXstpLocalConfWorker::updatePhyVlanStpConfigMO(DceSynchronousLinearSeqContext *pSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePhyVlanStpConfigMO: Enter");

        NsmUpdatePhyVlanStpConfigMessage *pLocalMessage = dynamic_cast<NsmUpdatePhyVlanStpConfigMessage*> (pSequencerContext->getPPrismMessage());
        return updateXstpPortVlanMO(pLocalMessage->getVid(), pLocalMessage, pSequencerContext);
    }

    void  IntfXstpLocalConfWorker::NsmUpdatePhyVlanStpConfigMessageHandler(NsmUpdatePhyVlanStpConfigMessage *pNsmUpdatePhyVlanStpConfigMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::validateIntfXstpConfigStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyVlanStpConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyVlanStpConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmUpdatePhyVlanStpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::updatePoStpConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
	NsmUpdatePoStpConfigSPMessage *pLocalMsg = dynamic_cast<NsmUpdatePoStpConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePoStpConfig: Enter ");
	/* Does Sanity check need here ??? */
	pLocalMsg->printMsg();
	MstpPortConfigMessage  message = *pLocalMsg;
    const char *client = MSTP_DCM_CLIENT_NAME;

	/*If it is "[no] bpdu-guard enable" command, send it to SSM*/
	switch(pLocalMsg->getCmdCode())
	{
	case DCM_SSM_BPDUGUARD_ENABLE:
	case DCM_SSM_BPDUGUARD_NO_ENABLE:
    case DCM_SSM_TAGGED_BPDU_ENABLE:
    case DCM_SSM_TAGGED_BPDU_NO_ENABLE:
        client = "ssm";
		break;
	}
        return MstpToolKit::sendToBackend(client, &message);
    }

    void  IntfXstpLocalConfWorker::NsmUpdatePoStpConfigSPMessageHandler( NsmUpdatePoStpConfigSPMessage *pNsmUpdatePoStpConfigSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePoStpConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdatePoStpConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::updatePoVlanStpConfigSP(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmUpdatePoVlanStpConfigSPMessage *pLocalMessage = dynamic_cast<NsmUpdatePoVlanStpConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePoVlanStpConfigSP: Enter \n");
        /* Does Sanity check need here ??? */

        MstpPortVlanConfigMessage  message = *pLocalMessage;

        return MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);
    }


    void  IntfXstpLocalConfWorker::NsmUpdatePoVlanStpConfigSPMessageHandler( NsmUpdatePoVlanStpConfigSPMessage *pNsmUpdatePoVlanStpConfigSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePoVlanStpConfigSP),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdatePoVlanStpConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId IntfXstpLocalConfWorker::deleteBridgeConfigMO (DceSynchronousLinearSeqContext *pSequencerContext)
	{
	    return clearIntfXstpConfigs("", false, pSequencerContext);
	}

    void  IntfXstpLocalConfWorker::NsmDeleteBridgeXstpConfigMessageHandler(NsmDeleteBridgeXstpConfigMessage *pNsmDeleteBridgeXstpConfigMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::deleteBridgeConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmDeleteBridgeXstpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::updatePoInstanceStpConfigSP(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        NsmUpdatePoInstanceStpConfigSPMessage *pLocalMessage = dynamic_cast<NsmUpdatePoInstanceStpConfigSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePoInstanceStpConfigSP: Enter \n");
        /* Does Sanity check need here ??? */

        MstpPortConfigMessage message = *pLocalMessage;
        message.printMsg();
        return MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);
    }


    void  IntfXstpLocalConfWorker::NsmUpdatePoInstanceStpConfigSPMessageHandler( NsmUpdatePoInstanceStpConfigSPMessage *pNsmUpdatePoInstanceStpConfigSPMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePoInstanceStpConfigSP),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pNsmUpdatePoInstanceStpConfigSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::updatePhyInstanceStpConfig(DceSynchronousLinearSeqContext *pSequencerContext)
    {
        NsmUpdatePhyInstanceStpConfigMessage *pLocalMessage = dynamic_cast<NsmUpdatePhyInstanceStpConfigMessage*> (pSequencerContext->getPPrismMessage());

        /*
        * Checkout whether the command is a NO command. If so, then
        * checkout the mode the interface is in.
        * If it is an ISL then return from there and dont passon the
        * request to the backend
        */
        if (pLocalMessage->getIsNoCmd()) {

            bool isISL = false;
            if (WAVE_MESSAGE_SUCCESS != NsmUtils::checkIsISLLink(pLocalMessage->getIfName(), isISL)) {
                return WAVE_MESSAGE_ERROR;
            }

            if (isISL) {
                return WAVE_MESSAGE_SUCCESS;
            }
        }
        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePhyInstanceStpConfig: Enter \n");
        /* Does Sanity check need here ??? */

        pLocalMessage->printMsg();
        MstpPortConfigMessage message = *pLocalMessage;

        message.printMsg();
        return MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);
    }

    ResourceId  IntfXstpLocalConfWorker::updatePhyInstanceStpConfigMO(DceSynchronousLinearSeqContext *pSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "IntfXstpLocalConfWorker::updatePhyInstanceStpConfigMO: Enter");

        NsmUpdatePhyInstanceStpConfigMessage *pLocalMessage = dynamic_cast<NsmUpdatePhyInstanceStpConfigMessage*> (pSequencerContext->getPPrismMessage());
        return updateXstpPortInstanceMO(pLocalMessage->getInstanceId(), pLocalMessage, pSequencerContext);
	}



    void  IntfXstpLocalConfWorker::NsmUpdatePhyInstanceStpConfigMessageHandler( NsmUpdatePhyInstanceStpConfigMessage *pNsmUpdatePhyInstanceStpConfigMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::validateIntfXstpConfigStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyInstanceStpConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::updatePhyInstanceStpConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfXstpLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new DceSynchronousLinearSeqContext (pNsmUpdatePhyInstanceStpConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId IntfXstpLocalConfWorker::deleteVlanIntfXstp(UI32 vid, DceSynchronousLinearSeqContext *pSequencerContext)
    {
        return handleVlanDelete(vid, pSequencerContext->getWaveManagedObjectId(), pSequencerContext);
    }

	// DUMMY METHOD
	ResourceId IntfXstpLocalConfWorker::createPortXstp(PhyIntfLocalManagedObject *pIntfMo)
	{
		return WAVE_MESSAGE_SUCCESS;
	}

	ResourceId IntfXstpLocalConfWorker::deletePortXstp(PhyIntfLocalManagedObject *pIntfMo, DceSynchronousLinearSeqContext *pSequencerContext)
	{
	    // TODO move the UID free logic to base class itself!!!

	    string ifName = pIntfMo->getId();
	    ResourceId status = clearIntfXstpConfigs(ifName, true, pSequencerContext);

	    if (status == WAVE_MESSAGE_SUCCESS && MstpToolKit::isVcsEnabled()) {
	        status = MstpToolKit::freePortId(pIntfMo->getPhyType(), ifName, true, pSequencerContext);
	    }

		return (status);
	}

    ResourceId IntfXstpLocalConfWorker::validateIntfXstpConfigStep (DceSynchronousLinearSeqContext *pContext)
    {
        NsmPortStpConfigMessage *m = dynamic_cast<NsmPortStpConfigMessage*>(pContext->getPPrismMessage());

        ResourceId status = validatePortXstpConfig(m, pContext);

        if (m->getOperationCode() == NSMUPDATEPHYVLANSTPCONFIG && status == WAVE_MESSAGE_SUCCESS) {
            UI32 vid = dynamic_cast<NsmUpdatePhyVlanStpConfigMessage*>(m)->getVid();
            status = MstpToolKit::validateVlanForXstpConfig(vid, pContext);
        }

        return status;
    }

    ResourceId IntfXstpLocalConfWorker::fillMo(XstpPortConfigManagedObject *pPortXstpMo, NsmPortStpConfigMessage *pMessage)
    {
        return pPortXstpMo->setAttributeByMessage((NsmUpdatePhyStpConfigMessage*) pMessage);
    }

    ResourceId IntfXstpLocalConfWorker::fillMo(XstpPortInstanceConfigManagedObject *pPortInstMo, NsmPortStpConfigMessage *pMessage)
    {
        return pPortInstMo->setAttributeByMessage((NsmUpdatePhyInstanceStpConfigMessage*) pMessage);
    }

    ResourceId IntfXstpLocalConfWorker::fillMo(XstpPortVlanConfigManagedObject *pPortVlanMo, NsmPortStpConfigMessage *pMessage)
    {
        return pPortVlanMo->setAttributeByMessage((NsmUpdatePhyVlanStpConfigMessage*) pMessage);
    }

}
