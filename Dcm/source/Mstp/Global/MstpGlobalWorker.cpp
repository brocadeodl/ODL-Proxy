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
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalWorker.h"
#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalDeleteVlanBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateVlanBridgeConfigMessage.h"
#include "Mstp/Global/MstpGlobalUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Global/XstpBridgeConfigManagedObject.h"
#include "Mstp/Global/PvstBridgeConfigManagedObject.h"
#include "Mstp/Global/RpvstBridgeConfigManagedObject.h"
#include "Mstp/Global/VlanBridgeStpConfigManagedObject.h"
#include "Mstp/Global/StpBridgeConfigManagedObject.h"
#include "Mstp/Global/RstpBridgeConfigManagedObject.h"
#include "Mstp/Global/InstanceBaseManagedObject.h"
#include "Mstp/Global/MstpBridgeConfigManagedObject.h"
#include "Mstp/Global/MstpInstanceConfigManagedObject.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Local/MstpUpdateVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalDeleteBridgeXstpConfigMessage.h"
#include "Nsm/Local/NsmDeleteBridgeXstpConfigMessage.h"
#include "Nsm/Common/NsmUpdateMstpInstanceConfigMessage.h"
#include "Utils/DceClusterUtils.h"
#include "Utils/DceLinearSeqContext.h"
#include "Utils/mo_iterator.h"
#include "DcmResourceIds.h"

#include <algorithm>


#define BR_MAC_LASTBYTE_KEY "bridgeMacLastByte"


#define DELETE_ALL( moClass ) \
            deleteMultipleWaveManagedObjects(new WaveManagedObjectSynchronousQueryContextForDeletion(moClass))

#define PUT_COMMAND_CODE( pDceContext, cmdCode ) \
            pDceContext->getCache()->put("cmdCode", (cmdCode))

#define COMMAND_CODE( pDceContext ) \
            ((UI32) pDceContext->getCache()->getIntValue("cmdCode"))

#define SEND_SYNCHRONOUSLY( pMessage, status ) \
            do { \
                status = sendSynchronously(pMessage); \
                if (status == WAVE_MESSAGE_SUCCESS) \
                    status = (pMessage)->getCompletionStatus(); \
            } while (0)


using namespace std;

namespace DcmNs
{

    MstpGlobalWorker::MstpGlobalWorker ( MstpGlobalObjectManager *pMstpGlobalObjectManager)
        : WaveWorker  (pMstpGlobalObjectManager)
    {
        XstpBridgeConfigManagedObject    XstpBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        PvstBridgeConfigManagedObject    PvstBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        RpvstBridgeConfigManagedObject    RpvstBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        VlanBridgeStpConfigManagedObject    VlanBridgeStpConfigManagedObject    (pMstpGlobalObjectManager);
        StpBridgeConfigManagedObject    StpBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        RstpBridgeConfigManagedObject    RstpBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        InstanceBaseManagedObject    InstanceBaseManagedObject    (pMstpGlobalObjectManager);
        MstpBridgeConfigManagedObject    MstpBridgeConfigManagedObject    (pMstpGlobalObjectManager);
        MstpInstanceConfigManagedObject    MstpInstanceConfigManagedObject    (pMstpGlobalObjectManager);
        XstpBridgeConfigManagedObject.setupOrm ();
        addManagedClass (XstpBridgeConfigManagedObject::getClassName (), this);
        PvstBridgeConfigManagedObject.setupOrm ();
        addManagedClass (PvstBridgeConfigManagedObject::getClassName (), this);
        RpvstBridgeConfigManagedObject.setupOrm ();
        addManagedClass (RpvstBridgeConfigManagedObject::getClassName (), this);
        VlanBridgeStpConfigManagedObject.setupOrm ();
        addManagedClass (VlanBridgeStpConfigManagedObject::getClassName (), this);
        StpBridgeConfigManagedObject.setupOrm ();
        addManagedClass (StpBridgeConfigManagedObject::getClassName (), this);
        RstpBridgeConfigManagedObject.setupOrm ();
        addManagedClass (RstpBridgeConfigManagedObject::getClassName (), this);
        InstanceBaseManagedObject.setupOrm ();
        addManagedClass (InstanceBaseManagedObject::getClassName (), this);
        MstpBridgeConfigManagedObject.setupOrm ();
        addManagedClass (MstpBridgeConfigManagedObject::getClassName (), this);
        MstpInstanceConfigManagedObject.setupOrm ();
        addManagedClass (MstpInstanceConfigManagedObject::getClassName (), this);

        addOperationMap (MSTPGLOBALUPDATEBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpGlobalWorker::MstpGlobalUpdateBridgeConfigMessageHandler));
        addOperationMap (MSTPGLOBALDELETEVLANBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpGlobalWorker::MstpGlobalDeleteVlanBridgeConfigMessageHandler));
        addOperationMap (MSTPGLOBALUPDATEVLANBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpGlobalWorker::MstpGlobalUpdateVlanBridgeConfigMessageHandler));
        addOperationMap (MSTPGLOBALUPDATEMSTPINSTANCECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpGlobalWorker::MstpGlobalUpdateMstpInstanceConfigMessageHandler));
    }

    MstpGlobalWorker::~MstpGlobalWorker ()
    {
    }

    PrismMessage  *MstpGlobalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case MSTPGLOBALCREATEBRIDGECONFIG :
#if 0
            case MSTPGLOBALDELETEBRIDGECONFIG :
                pPrismMessage = new MstpGlobalDeleteBridgeConfigMessage ();
                break;
#endif
            case MSTPGLOBALUPDATEBRIDGECONFIG :
                pPrismMessage = new MstpGlobalUpdateBridgeConfigMessage ();
                break;
            case MSTPGLOBALDELETEVLANBRIDGECONFIG :
                pPrismMessage = new MstpGlobalDeleteVlanBridgeConfigMessage ();
                break;
            case MSTPGLOBALUPDATEVLANBRIDGECONFIG :
                pPrismMessage = new MstpGlobalUpdateVlanBridgeConfigMessage ();
                break;
            case MSTPGLOBALUPDATEMSTPINSTANCECONFIG :
                pPrismMessage = new MstpGlobalUpdateMstpInstanceConfigMessage ();
                break;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *MstpGlobalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((XstpBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new XstpBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PvstBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PvstBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((RpvstBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RpvstBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VlanBridgeStpConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VlanBridgeStpConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((StpBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new StpBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((RstpBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new RstpBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((InstanceBaseManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new InstanceBaseManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((MstpBridgeConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MstpBridgeConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((MstpInstanceConfigManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new MstpInstanceConfigManagedObject(dynamic_cast<MstpGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "MstpGlobalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }


    class MstiUpdateSequencerContext : public DceLinearSeqContext
    {
    public:
        MstiUpdateSequencerContext (MstpGlobalUpdateMstpInstanceConfigMessage *pMessage, MstpGlobalWorker *w, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
            : DceLinearSeqContext (pMessage, w, pSteps, numberOfSteps)
        {
            message     = pMessage;
            cmdCode     = pMessage->getCmdCode();
            instanceMo      = NULL;
        }

        ~MstiUpdateSequencerContext () {
            delete instanceMo;
        }

        MstpGlobalUpdateMstpInstanceConfigMessage *getMessage() {
            return message;
        }

        UI32 getCmdCode() {
            return cmdCode;
        }

        UI32 getInstanceId() {
            return message->getInstanceId();
        }

    public:
        UI32 cmdCode;
        MstpGlobalUpdateMstpInstanceConfigMessage *message;
        MstpInstanceConfigManagedObject *instanceMo;
    };


    /**
     * Compute v1 - v2 and store in out..
     */
    static void diff (const vector<ObjectId> &v1, const vector<ObjectId> &v2, vector<ObjectId> &out)
    {
        for (UI32 i = 0; i < v1.size(); ++i) {
            ObjectId oid = v1[i];
            if (find(v2.begin(), v2.end(), oid) == v2.end())
                out.push_back(oid);
        }
    }


    ResourceId MstpGlobalWorker::deleteVlanBridgeConfigMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::deleteVlanBridgeConfigMO:  Enter");

        MstpGlobalDeleteVlanBridgeConfigMessage *pMsg = dynamic_cast<MstpGlobalDeleteVlanBridgeConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        WaveManagedObjectSynchronousQueryContextForDeletion *pDeleteContext =
                new WaveManagedObjectSynchronousQueryContextForDeletion (VlanBridgeStpConfigManagedObject::getClassName());

        pDeleteContext->addAndAttribute(new AttributeUI32(pMsg->getId(), "vlan"));

        deleteMultipleWaveManagedObjects(pDeleteContext);

        return WAVE_MESSAGE_SUCCESS;
    }

    void  MstpGlobalWorker::MstpGlobalDeleteVlanBridgeConfigMessageHandler( MstpGlobalDeleteVlanBridgeConfigMessage *pMstpGlobalDeleteVlanBridgeConfigMessage)
    {

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::deleteVlanBridgeConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::deleteVlanFromInstanceBaseConfigMO),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpGlobalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpGlobalDeleteVlanBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void MstpGlobalWorker::validateBridgeConfigStep (DceLinearSeqContext *pSequencerContext)
    {
        MstpGlobalUpdateBridgeConfigMessage *pMessage = dynamic_cast<MstpGlobalUpdateBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());
        UI32 cmdCode = pMessage->getCmdCode();
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::%s : cmdCode = %u", __FUNCTION__, cmdCode);

        if (cmdCode == MSTP_MSG_BR_CREATE && MstpToolKit::isVcsEnabled()) {
            status = MstpToolKit::validateBpduTunnellingParamsNotConfigured(LOCATION_FILTER_CONNECTED);
            UI32 count = 0;
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = MstpToolKit::checkSTPEnabledGvlanExists(count);
                tracePrintf(TRACE_LEVEL_DEBUG, "Gvlan count is %d", count);

                if (count != 0) {
            	    tracePrintf(TRACE_LEVEL_ERROR, "Mstp cannt be configured while %d gvlan exists", count);
            	    status = WRC_NOT_ALLOWED_WITH_NON_DOT1Q_VLAN;
                }
            }
        }

        pSequencerContext->executeNextStep(status);
    }

    /*
     * Verify and send bridge configure message to localplugin
     */
    void MstpGlobalWorker::updateGlobalBridgeConfigStep (DceLinearSeqContext *pSequencerContext)
    {
        MstpGlobalUpdateBridgeConfigMessage *pGlobalMessage = dynamic_cast<MstpGlobalUpdateBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());
        int cmdCode = pGlobalMessage->getCmdCode();

        tracePrintf (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateGlobalBridgeConfig: cmdCode = %d", cmdCode);

        pGlobalMessage->printMsg();

        MstpUpdateBridgeConfigMessage *pLocalMessage =
                new MstpUpdateBridgeConfigMessage(*pGlobalMessage);

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        if ((cmdCode == MSTP_MSG_BR_CREATE || cmdCode == MSTP_MSG_BR_PROTO_ENABLE)
                && MstpToolKit::isVcsEnabled()) {
            status = MstpToolKit::allocateVcsBridgeMac(pLocalMessage->m_vcsBridgeMac);

            if (status == WAVE_MESSAGE_SUCCESS) {
                UI32 lastByte = (UI32) pLocalMessage->m_vcsBridgeMac[5];
                pSequencerContext->getCache()->put(BR_MAC_LASTBYTE_KEY, lastByte);
            }
            // Hope it is safe to use FrameworkToolKit::isConfigReplayInProgress here :)
            else if (FrameworkToolKit::isConfigReplayInProgress()) {
                MstpToolKit::logBridgeMacAllocError(status, "config-replay");
                //pLocalMessage->setStpDisable(true);
                status = WAVE_MESSAGE_SUCCESS;
            }
        }

        if (status == WAVE_MESSAGE_SUCCESS) {
            sendToCluster(pLocalMessage, pSequencerContext);
        } else {
            delete pLocalMessage;
            return pSequencerContext->executeNextStep(status);
        }
    }

    /* Notify Nsm plugin if delete bridge protocol */
    void MstpGlobalWorker::deleteGlobalBridgeConfigNsmStep (DceLinearSeqContext *pSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        bool notifyNsmGlobal = false, notifyNsmLocal = false;
        vector<LocationId> targetLocations;

        switch (COMMAND_CODE(pSequencerContext)) {
        case MSTP_MSG_BR_CREATE:
            if (MstpToolKit::isVcsEnabled()
                    && MstpToolKit::validateBpduTunnellingParamsNotConfigured(LOCATION_FILTER_DISCONNECTED) != WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::%s : BPDU tunnelling configs present on disconnected nodes..", __FUNCTION__);
                targetLocations = MstpToolKit::getDisconnectedLocations();
                if (targetLocations.empty()) {
                    tracePrintf(TRACE_LEVEL_WARN, "MstpGlobalWorker::%s : SHOULD NOT HAPPEN NORMALLY... No disconnected location found!!", __FUNCTION__);
                } else {
                    notifyNsmLocal = true;
                }
            }
            break;
        case MSTP_MSG_BR_NONE:
            notifyNsmGlobal = notifyNsmLocal = true;
            break;
        }

        if (notifyNsmGlobal) {
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::%s : Notifying NSM Global...", __FUNCTION__);
            NsmGlobalDeleteBridgeXstpConfigMessage msg;
            SEND_SYNCHRONOUSLY(&msg, status);
        }

        if (notifyNsmLocal && status == WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::%s : Notifying NSM Local...", __FUNCTION__);
            NsmDeleteBridgeXstpConfigMessage *pMessage = new NsmDeleteBridgeXstpConfigMessage();
            pMessage->setNeedSurrogateSupportFlag(true);
            sendToCluster(pMessage, pSequencerContext, &targetLocations);
            return;
        }

        pSequencerContext->executeNextStep(status);
    }

    void MstpGlobalWorker::releaseLogicalPortIdsStep (DceLinearSeqContext *pSequencerContext)
    {
        if (COMMAND_CODE(pSequencerContext) == MSTP_MSG_BR_NONE
                && MstpToolKit::isVcsEnabled()) {
            tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::releaseLogicalPortIdsStep : Releasing all logical port ids...");
            MstpGlobalUpdateBridgeConfigMessage *pGlobalMessage = dynamic_cast<MstpGlobalUpdateBridgeConfigMessage*>(pSequencerContext->getPPrismMessage());

            // TODO have a new API for this
            MstpToolKit::freePortId(pGlobalMessage->getMode(), IF_TYPE_INVALID, "");
        }

        pSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }

    void MstpGlobalWorker::validateMstpInstanceConfigStep(MstiUpdateSequencerContext *pSequencerContext)
    {
        MstpGlobalUpdateMstpInstanceConfigMessage *pMessage = pSequencerContext->getMessage();
        UI32 instId = (UI32) pMessage->getInstanceId();

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::validateMstpInstanceConfigStep : instanceId = %u", instId);
        pMessage->printMsg();

        UI32 count = 0;
        querySynchronouslyForCount(MstpBridgeConfigManagedObject::getClassName(), count);
        if (count == 0) {
            tracePrintf(TRACE_LEVEL_ERROR, "MstpGlobalWorker::validateMstpInstanceConfigStep : MSTP not configured..");
            pSequencerContext->executeNextStep(WRC_MSTP_BRIDGE_NOT_MSTP);
            return;
        }

        WaveManagedObjectSynchronousQueryContext qc (MstpInstanceConfigManagedObject::getClassName());
        qc.addAndAttribute(new AttributeUI8((UI8) instId, "instanceId"));
        qc.addLimitForQueryResults(1);

        vector<WaveManagedObject*> *mos = querySynchronously(&qc);
        if (mos != NULL && !mos->empty()) {
            pSequencerContext->instanceMo = dynamic_cast<MstpInstanceConfigManagedObject*> (mos->front());
        }

        delete mos;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        switch (pSequencerContext->getCmdCode()) {
        case MSTP_MSG_BR_INST_PRIORITY:
        case MSTP_MSG_BR_INST_NO_PRIORITY:
        case MSTP_MSG_BR_NO_INST:
            if (pSequencerContext->instanceMo == NULL) {
                tracePrintf (TRACE_LEVEL_ERROR, "MstpGlobalWorker::validateMstpInstanceConfigStep : No such instance %u", instId);
                status = WRC_MSTP_INST_NOT_EXIST;
            }
            break;

        case MSTP_MSG_BR_INST_ADD_VLAN:
            // Check if GVLAN is also present for adding to instance
            if (NsmUtils::isGvlanIdPresentInRange(pMessage->m_vlanIdRange)) {
                status = WRC_MSTP_INST_VLAN_NON_DOT1Q_VLAN_NOT_ALLOWED;
                break;
            }
        case MSTP_MSG_BR_INST_REMOVE_VLAN:
            // TODO store the vlan object ids in context, not in message
            if (NsmUtils::isAllUI32RangeVlanConfigured(pMessage->m_vlanIdRange,  pMessage->m_vlanid) == WAVE_MESSAGE_ERROR) {
                tracePrintf (TRACE_LEVEL_ERROR, "MstpGlobalWorker::updateGlobalMstpInstanceConfigStep : one or more vlans doesn't exist %s", pMessage->m_vlanIdRange.toString().c_str());
                status = WRC_MSTP_VLAN_NOT_FOUND_ERROR;
            } else {
                status = MstpToolKit::validateVlanForXstpConfig(pMessage->m_vlanIdRange, pSequencerContext);
            }
            break;
        }

        pSequencerContext->executeNextStep(status);
    }

    void MstpGlobalWorker::updateGlobalMstpInstanceConfigStep (MstiUpdateSequencerContext *pSequencerContext)
    {
        MstpUpdateMstpInstanceConfigMessage *pLocalMessage =
                new MstpUpdateMstpInstanceConfigMessage (*pSequencerContext->message);

        sendToCluster(pLocalMessage, pSequencerContext);
    }

    void MstpGlobalWorker::notifyNsmOnMstpInstanceUpdateStep (MstiUpdateSequencerContext *pSequencerContext)
    {
        if (pSequencerContext->instanceMo == NULL) {
            // Nothing to update..
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::notifyNsmOnMstpInstanceUpdateStep : MstpInstanceConfigMO is NULL, not notifying NSM");
            pSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        vector<ObjectId> newMemberVlans;

        switch (pSequencerContext->cmdCode) {
        case MSTP_MSG_BR_NO_INST:
            break;
        case MSTP_MSG_BR_INST_REMOVE_VLAN:
            diff(pSequencerContext->instanceMo->m_instanceVlans, pSequencerContext->message->m_vlanid, newMemberVlans);
            if (newMemberVlans.empty()) {
                pSequencerContext->cmdCode = MSTP_MSG_BR_NO_INST;
            }
            break;
        default:
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::notifyNsmOnMstpInstanceUpdateStep : No NSM update needed for command %u", pSequencerContext->getCmdCode());
            pSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);
            return;
        }

        NsmUpdateMstpInstanceConfigMessage* poInstMesaage = NsmUpdateMstpInstanceConfigMessage::forNsmGlobal();
        poInstMesaage->setInstanceInfo(pSequencerContext->getInstanceId(), newMemberVlans);

        SEND_SYNCHRONOUSLY(poInstMesaage, status);
        delete poInstMesaage;

        if (status == WAVE_MESSAGE_SUCCESS) {
            NsmUpdateMstpInstanceConfigMessage *phyInstMssage = NsmUpdateMstpInstanceConfigMessage::forNsmLocal();
            phyInstMssage->setInstanceInfo(pSequencerContext->getInstanceId(), newMemberVlans);
            phyInstMssage->setNeedSurrogateSupportFlag(true);

            sendToCluster(phyInstMssage, pSequencerContext);
        } else {
            pSequencerContext->executeNextStep(status);
        }
    }

    void MstpGlobalWorker::updateGlobalMstpInstanceConfigMOStep(MstiUpdateSequencerContext *pSequencerContext)
    {
        trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateGlobalMstpInstanceConfigMOStep: Entering...  ");

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        MstpInstanceConfigManagedObject *pInstMo = pSequencerContext->instanceMo;
        bool needsUpdate = true;

        switch (pSequencerContext->cmdCode) {
        case MSTP_MSG_BR_NO_INST:
            delete pInstMo;
            pSequencerContext->instanceMo = pInstMo = NULL;
            break;
        case MSTP_MSG_BR_INST_ADD_VLAN:
            if (pInstMo == NULL) {
                pInstMo = new MstpInstanceConfigManagedObject(
                        dynamic_cast<MstpGlobalObjectManager*>(getPWaveObjectManager()));
                pInstMo->setDefaultAttribute((UI8) pSequencerContext->getInstanceId());
                pSequencerContext->instanceMo = pInstMo;
                needsUpdate = false;
            }
            break;
        }

        if (pInstMo != NULL) {
            status = pInstMo->setAttributeByMessage(pSequencerContext->message);
            if (needsUpdate && status == WAVE_MESSAGE_SUCCESS) {
                updateWaveManagedObject(pInstMo);
            }
        }

        pSequencerContext->executeNextStep(status);
    }

    ResourceId MstpGlobalWorker::deleteBridgeConfigMO (int mode, DceLinearSeqContext *pSequencerContext)
    {
        string brClass = MstpToolKit::getClassNameForMode(mode);
        if (brClass.empty()) {
            tracePrintf(TRACE_LEVEL_ERROR, "MstpGlobalWorker::deleteBridgeConfigMO : Unknown bridge mode %d", mode);
            return WAVE_MESSAGE_ERROR;
        }

        switch (mode) {
        case MSTP_DCM_BRIDGE_MSTP:
            tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::deleteBridgeConfigMO : Deleteing all MstpInstanceConfigManagedObject instances");
            DELETE_ALL(MstpInstanceConfigManagedObject::getClassName());
            break;

        case MSTP_DCM_BRIDGE_PVSTP:
        case MSTP_DCM_BRIDGE_RPVSTP:
            tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::deleteBridgeConfigMO : Deleteing all VlanBridgeStpConfigManagedObject instances");
            DELETE_ALL(VlanBridgeStpConfigManagedObject::getClassName());
            break;
        }

        tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::deleteBridgeConfigMO : Deleting %s", brClass.c_str());
        DELETE_ALL(brClass);

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId MstpGlobalWorker::createBridgeConfigMO (MstpGlobalUpdateBridgeConfigMessage * pGlobalMessage, DceLinearSeqContext *pSequencerContext)
    {
        string brClass = MstpToolKit::getClassNameForMode(pGlobalMessage->getMode());
        tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::createBridgeConfigMO : Creating %s", brClass.c_str());

        XstpBridgeConfigManagedObject *pBridgeMo =
                dynamic_cast<XstpBridgeConfigManagedObject*> (createManagedObjectInstance(brClass));

        if (pBridgeMo == NULL) {
            tracePrintf(TRACE_LEVEL_ERROR, "*** Error creating MO");
            return WAVE_MESSAGE_ERROR;  // assert???
        }

        pBridgeMo->setDefaultAttribute();
        pSequencerContext->addManagedObjectForGarbageCollection(pBridgeMo);

        int brMacByte = pSequencerContext->getCache()->getIntValue(BR_MAC_LASTBYTE_KEY);
        if (brMacByte > 0) {
            pBridgeMo->setBridgeMacByte((UI8)(brMacByte & 0xFF));
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    /*
     *  Update xSTP bridge MO
     */
    ResourceId MstpGlobalWorker::updateBridgeConfigMO (MstpGlobalUpdateBridgeConfigMessage * pGlobalMessage, DceLinearSeqContext * pSequencerContext)
    {
        string brClass = MstpToolKit::getClassNameForMode(pGlobalMessage->getMode());
        tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::updateBridgeConfigMO : Updating %s", brClass.c_str());

        // TODO use cache???
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(brClass);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults == NULL || pResults->empty()) {
            delete pResults;
            tracePrintf(TRACE_LEVEL_ERROR, "MstpGlobalWorker::updateBridgeConfigMO : Did not find MO!!");
            return WAVE_MESSAGE_ERROR;  // TODO create new MO???
        }

        XstpBridgeConfigManagedObject *pBridgeMo =
                dynamic_cast<XstpBridgeConfigManagedObject*> (pResults->front());

        ResourceId status = pBridgeMo->setAttributeByMessage(pGlobalMessage);

        if (status == WAVE_MESSAGE_SUCCESS) {
            int brMacByte = pSequencerContext->getCache()->getIntValue(BR_MAC_LASTBYTE_KEY);
            if (brMacByte > 0) {
                pBridgeMo->setBridgeMacByte((UI8)(brMacByte & 0xFF));
            }

            updateWaveManagedObject(pBridgeMo);
        } else {
            tracePrintf(TRACE_LEVEL_ERROR, "MstpGlobalWorker::updateBridgeConfigMO : Update failed - %u", status);
        }

        pSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
        delete pResults;
        return status;
    }

    /*
     * Update the MO Database
     */
    void MstpGlobalWorker::updateGlobalBridgeConfigMOStep (DceLinearSeqContext *pSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_ERROR;

        MstpGlobalUpdateBridgeConfigMessage *pGlobalMessage = dynamic_cast<MstpGlobalUpdateBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());
        int mode = pGlobalMessage->getMode();
        int cmdCode = pGlobalMessage->getCmdCode();

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateGlobalBridgeConfigMOStep: Enter CMD : %d", cmdCode);
        switch (cmdCode) {
            case MSTP_MSG_BR_CREATE:
                status = createBridgeConfigMO(pGlobalMessage, pSequencerContext);
                break;
            case MSTP_MSG_BR_NONE:
                status = deleteBridgeConfigMO(mode, pSequencerContext);
                break;
            default:
                status = updateBridgeConfigMO(pGlobalMessage, pSequencerContext);
                break;
        }

        trace (TRACE_LEVEL_DEBUG, string("MstpGlobalWorker::updateGlobalBridgeConfigMOStep:  ") +FrameworkToolKit::localize(status));
        pSequencerContext->executeNextStep(status);
    }

    void  MstpGlobalWorker::MstpGlobalUpdateBridgeConfigMessageHandler( MstpGlobalUpdateBridgeConfigMessage *pMstpGlobalUpdateBridgeConfigMessage)
    {
        trace (TRACE_LEVEL_INFO, "MstpGlobalWorker::MstpGlobalUpdateBridgeConfigMessageHandler: Entering ...");

        PrismLinearSequencerStep steps[] = {
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::validateBridgeConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateGlobalBridgeConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::deleteGlobalBridgeConfigNsmStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateGlobalBridgeConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::releaseLogicalPortIdsStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pContext = new DceLinearSeqContext (pMstpGlobalUpdateBridgeConfigMessage, this, steps, (sizeof steps / sizeof steps[0]));

        PUT_COMMAND_CODE(pContext, pMstpGlobalUpdateBridgeConfigMessage->getCmdCode());
        pContext->holdAll();
        pContext->start();
    }

    void  MstpGlobalWorker::MstpGlobalUpdateMstpInstanceConfigMessageHandler( MstpGlobalUpdateMstpInstanceConfigMessage *pMstpGlobalUpdateMstpInstanceConfigMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::validateMstpInstanceConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateGlobalMstpInstanceConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::notifyNsmOnMstpInstanceUpdateStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateGlobalMstpInstanceConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerFailedStep)
        };

        MstiUpdateSequencerContext *pSequencerContext = new MstiUpdateSequencerContext (pMstpGlobalUpdateMstpInstanceConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSequencerContext->holdAll();
        pSequencerContext->start();
    }

    void MstpGlobalWorker::validateVlanBridgeConfigStep (DceLinearSeqContext *pSequencerContext)
    {
    	MstpGlobalUpdateVlanBridgeConfigMessage *pMessage = dynamic_cast<MstpGlobalUpdateVlanBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        switch (pMessage->getCmdCode()) {
        case MSTP_MSG_BR_VLAN_PRIORITY:
        case MSTP_MSG_BR_VLAN_FWD_DELAY:
        case MSTP_MSG_BR_VLAN_MAX_AGE:
        case MSTP_MSG_BR_VLAN_AGING_TIME:
        case MSTP_MSG_BR_VLAN_HELLO_TIME:
            status = MstpToolKit::validateVlanForXstpConfig(pMessage->getId(), pSequencerContext);
            break;
        }

        pSequencerContext->executeNextStep(status);
    }


    /*
     * Verify and send vlan bridge configure message to localplugin
     */
    void MstpGlobalWorker::updateGlobalVlanBridgeConfigStep (DceLinearSeqContext *pSequencerContext)
    {
        MstpGlobalUpdateVlanBridgeConfigMessage *pGlobalMessage = dynamic_cast<MstpGlobalUpdateVlanBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateGlobalVlanBridgeConfigStep: Enter \n");
        pGlobalMessage->printMsg();

        MstpUpdateVlanBridgeConfigMessage *pLocalMessage =
                new MstpUpdateVlanBridgeConfigMessage(*pGlobalMessage);

        sendToCluster(pLocalMessage, pSequencerContext);
    }

    void MstpGlobalWorker::updateVlanBridgeConfigMOStep (DceLinearSeqContext *pSequencerContext)
    {
        MstpGlobalUpdateVlanBridgeConfigMessage *pGlobalMessage = dynamic_cast<MstpGlobalUpdateVlanBridgeConfigMessage*> (pSequencerContext->getPPrismMessage ());
        ResourceId status = updateVlanBridgeConfigMO(pGlobalMessage, pSequencerContext);
        pSequencerContext->executeNextStep(status);
    }

    ResourceId MstpGlobalWorker::updateVlanBridgeConfigMO (MstpGlobalUpdateVlanBridgeConfigMessage *pGlobalMessage, DceLinearSeqContext *pSequencerContext)
    {
        bool isCreate = false;
        UI32 vid = pGlobalMessage->getId();

        tracePrintf(TRACE_LEVEL_INFO, "MstpGlobalWorker::updateVlanBridgeConfigMO : vid = %u", vid);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt (VlanBridgeStpConfigManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute(new AttributeUI32(vid, "vlan"));
        syncQueryCtxt.addLimitForQueryResults(1);

        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        VlanBridgeStpConfigManagedObject *pMo = (pResults == NULL || pResults->empty()) ? NULL
                : dynamic_cast<VlanBridgeStpConfigManagedObject*>(pResults->front());

        delete pResults;

        if (pMo == NULL) {
            /* If user configure default value, no need to create MO */
            if (VlanBridgeStpConfigManagedObject::isConfigDefault(pGlobalMessage)) {
                trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateVlanBridgeConfigMO:  default configure");
                return (WAVE_MESSAGE_SUCCESS);
            }

            ObjectId vlanOid;
            if (NsmUtils::getVlanIntfObjectIdByVlanIfName(vid, vlanOid, true, pSequencerContext) != WAVE_MESSAGE_SUCCESS) {\
                tracePrintf(TRACE_LEVEL_ERROR, "stpGlobalWorker::updateVlanBridgeConfigMO : No such vlan %u", vid);
                return WRC_NSM_VLAN_ERR_VLAN_NOT_FOUND;
            }

            trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateVlanBridgeConfigMO:  vlan bridge create");
            pMo = new VlanBridgeStpConfigManagedObject(dynamic_cast<MstpGlobalObjectManager*>(getPWaveObjectManager()));
            pMo->setDefaultAttribute(vid, vlanOid);

            isCreate = true;
        }

        // update vlan bridge
        pMo->setAttributeByMessage(pGlobalMessage);

        if (!isCreate) {
            if (pMo->isDefault()) {
                trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateVlanBridgeConfigMO:  vlan bridge delete");
                delete pMo;
                pMo = NULL;
            } else {
                trace (TRACE_LEVEL_DEBUG, "MstpGlobalWorker::updateVlanBridgeConfigMO:  vlan bridge update");
                updateWaveManagedObject(pMo);
            }
        }

        if (pMo)
            pSequencerContext->addManagedObjectForGarbageCollection(pMo);
        return WAVE_MESSAGE_SUCCESS;
    }

    void  MstpGlobalWorker::MstpGlobalUpdateVlanBridgeConfigMessageHandler( MstpGlobalUpdateVlanBridgeConfigMessage *pMstpGlobalUpdateVlanBridgeConfigMessage)
    {

        PrismLinearSequencerStep sequencerSteps[] =
        {
        	reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::validateVlanBridgeConfigStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateGlobalVlanBridgeConfigStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::updateVlanBridgeConfigMOStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerSucceededStep),
            reinterpret_cast<PrismLinearSequencerStep>(&MstpGlobalWorker::prismLinearSequencerFailedStep)
        };

        DceLinearSeqContext *pSequencerContext = new DceLinearSeqContext (pMstpGlobalUpdateVlanBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pSequencerContext->holdAll();
        pSequencerContext->start();
    }

    ResourceId MstpGlobalWorker::deleteVlanFromInstanceBaseConfigMO (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        MstpGlobalDeleteVlanBridgeConfigMessage *pMsg = dynamic_cast<MstpGlobalDeleteVlanBridgeConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        UI32 vid = pMsg->getId();
        ObjectId vlanOid = pMsg->getObjectId();

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::deleteVlanFromInstanceBaseConfigMO : vlan oid={%s}, vid=%u", vlanOid.toString().c_str(), vid);

        MstpInstanceConfigManagedObject *pMstpInstanceMo = MstpToolKit::getMstpInstanceByVlanOid(vlanOid);

        if (pMstpInstanceMo == NULL) {
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::%s : No MSTP instances associated with vlan %u", __FUNCTION__, vid);
            return WAVE_MESSAGE_SUCCESS;
        }

        UI32 instanceId = pMstpInstanceMo->getInstanceId();
        tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::deleteVlanFromInstanceBaseConfigMO: Removing vlan %u association from MSTP instance %u", vid, instanceId);

        pMstpInstanceMo->removeInstanceVlan(vlanOid);

        if (pMstpInstanceMo->m_instanceVlans.empty()) {
            tracePrintf(TRACE_LEVEL_DEBUG, "MstpGlobalWorker::deleteVlanFromInstanceBaseConfigMO : No more vlans associated to MSTP instance %u; deleting it too", instanceId);
            delete pMstpInstanceMo;
        }
        else {
            pMstpInstanceMo->addAttributeToBeUpdated("vlanid");
            updateWaveManagedObject(pMstpInstanceMo);
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMstpInstanceMo);
        }

        return WAVE_MESSAGE_SUCCESS;
    }


    void MstpGlobalWorker::sendToCluster (PrismMessage *pMessage, DceLinearSeqContext *pSequencerContext, vector<LocationId> *pLocations)
    {
        WaveSendToClusterContext *pClusterContext = new WaveSendToClusterContext (this,
            reinterpret_cast<PrismAsynchronousCallback>(&MstpGlobalWorker::sendToClusterCallback), pSequencerContext);
        prismAssert(NULL != pClusterContext, __FILE__, __LINE__);

        pClusterContext->setPPrismMessageForPhase1(pMessage);
        pClusterContext->setPartialSuccessFlag(true);
        pClusterContext->setTreatFailureOnFailingOverAsSuccessFlag(true); // ????

        if (pLocations && !pLocations->empty()) {
            pClusterContext->setLocationsToSendToForPhase1(*pLocations);
        }

        sendToWaveCluster(pClusterContext);
    }

    void MstpGlobalWorker::sendToClusterCallback (WaveSendToClusterContext  *pClusterContext)
    {
        ResourceId status = ClusterGetStatusFromContext(pClusterContext);
        tracePrintf (TRACE_LEVEL_INFO, "MstpGlobalWorker::sendToClusterCallback : status = %u", status);

        PrismLinearSequencerContext *pSequencerContext = reinterpret_cast<PrismLinearSequencerContext*> (pClusterContext->getPCallerContext());
        delete pClusterContext->getPPrismMessageForPhase1();
        delete pClusterContext->getPPrismMessageForPhase2();
        delete pClusterContext;

        pSequencerContext->executeNextStep(status);
    }

}
