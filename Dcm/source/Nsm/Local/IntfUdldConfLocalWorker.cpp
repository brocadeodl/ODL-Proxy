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

/* **
 * IntfUdldConfLocalWorker.cpp
 *
 * Aug 9, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/IntfUdldConfLocalWorker.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/UdldIntfConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Udld/Global/UdldGlobalConfigManagedObject.h"
#include "ClientInterface/Udld/udld_dcm_def.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "ClientInterface/Udld/UdldIntfConfigClientMessage.h"
#include "Utils/mo_iterator.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "DcmResourceIds.h"

 #define  INTF_PORT_ROLE "portrole"

using namespace WaveNs;

namespace DcmNs
{

    IntfUdldConfLocalWorker::IntfUdldConfLocalWorker (NsmLocalObjectManager *om)
        : WaveWorker (om)
    {
        addOperationMap(NSM_UDLD_INTF_CONFIG, reinterpret_cast<PrismMessageHandler> (&IntfUdldConfLocalWorker::UdldIntfConfigHandler));
        addOperationMap(NSM_UDLD_RESET_ALL, reinterpret_cast<PrismMessageHandler> (&IntfUdldConfLocalWorker::UdldResetAllHandler));
    }

    IntfUdldConfLocalWorker::~IntfUdldConfLocalWorker ()
    {
    }

    PrismMessage* IntfUdldConfLocalWorker::createMessageInstance (const UI32 &operationCode)
    {
        switch (operationCode)
        {
            case NSM_UDLD_INTF_CONFIG:
            case NSM_UDLD_RESET_ALL:
                return new UdldIntfConfigMessage(operationCode);
        }
        return NULL;
    }

    WaveManagedObject* IntfUdldConfLocalWorker::createManagedObjectInstance (const string &managedClassName)
    {
        return NULL;
    }

    void IntfUdldConfLocalWorker::UdldIntfConfigHandler (UdldIntfConfigMessage *pMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::validateIntfConfigStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::updateIntfConfigStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::updateIntfMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pContext = new DceSynchronousLinearSeqContext(pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pContext->execute();
    }

    void IntfUdldConfLocalWorker::UdldResetAllHandler (UdldIntfConfigMessage *pMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::clearAllIntfMoStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&IntfUdldConfLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pContext = new DceSynchronousLinearSeqContext(pMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
        pContext->execute();
    }


    ResourceId IntfUdldConfLocalWorker::validateIntfConfigStep (DceSynchronousLinearSeqContext *pContext)
    {
        UdldIntfConfigMessage *pMessage = dynamic_cast<UdldIntfConfigMessage*> (pContext->getPPrismMessage());
        switch (pMessage->getIfType()) {
            case IF_TYPE_GI:
            case IF_TYPE_TE:
            case IF_TYPE_FO:
                break;
            default:
                tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::validateIntfConfigStep : Invalid interface type %u", pMessage->getIfType());
                return WRC_NSM_ERR_PHYINTF_INVALID;
        }

        mo_iterator<PhyIntfLocalManagedObject> phyIntfIter (pContext);
        phyIntfIter.criteria().And("id", pMessage->getIfName()).location(
			FrameworkToolKit::getThisLocationId()).select("objectId").select(
			"Portrole").select("isdestinationMirrorportEnable").select("poType");

        PhyIntfLocalManagedObject *intfMo = phyIntfIter.next();
        if (intfMo == NULL) {
            tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::validateIntfConfigStep : Interface %s not found", pMessage->getIfName().c_str());
            switch (pMessage->getIfType()) {
                case IF_TYPE_GI: return WRC_NSM_ERR_PHYINTF_INVALID_ONEGIGE;
                case IF_TYPE_TE: return WRC_NSM_ERR_PHYINTF_INVALID_TENGIGE;
                case IF_TYPE_FO: return WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
                default        : return WRC_NSM_ERR_PHYINTF_INVALID;
            }
        }

        if (intfMo->getPortrole() == true) {
            if(pMessage->getCommandCode() == UDLD_ENABLE_CMD) {
                tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::validateIntfConfigStep : Interface %s is an ISL", pMessage->getIfName().c_str());
                return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR; 
            }
            pContext->getCache()->put(INTF_PORT_ROLE, "true");
        }

        if (true == (intfMo->getDestinationmirrorportEnable())) {
			tracePrintf(
					TRACE_LEVEL_ERROR,
					"IntfUdldConfLocalWorker::validateIntfConfigStep : Interface %s is a destination mirror port.",
					pMessage->getIfName().c_str());
			return WRC_UDLD_API_ERR_UDLD_NOT_ON_DESTINATION_MIRR_PORT;
        }

        if(NSM_PO_TYPE_BROCADE == (intfMo->getPoType())) {
            tracePrintf(
                    TRACE_LEVEL_ERROR,
                    "IntfUdldConfLocalWorker::validateIntfConfigStep : Interface %s is a brocade trunk member.",
                    pMessage->getIfName().c_str());
            return WRC_UDLD_API_ERR_UDLD_NOT_ALLOWED_ON_BROCADE_TRUNK_MEMBER;
        }

        pContext->setPWaveManagedObject(intfMo);

        if (pMessage->getCommandCode() == UDLD_ENABLE_CMD) {
            mo_iterator<UdldGlobalConfigManagedObject> udldIter (pContext);
            udldIter.criteria().select("objectId");

            if (udldIter.hasNext()) {
                ObjectId udldOid = udldIter.next()->getObjectId();
                pContext->setWaveManagedObjectId(udldOid);
            } else {
                tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::validateIntfConfigStep : UDLD protocol is not configured!");
                return WRC_UDLD_API_ERR_UDLD_NOT_EXIST;
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IntfUdldConfLocalWorker::updateIntfConfigStep (DceSynchronousLinearSeqContext *pContext)
    {
        UdldIntfConfigMessage *pMessage = dynamic_cast<UdldIntfConfigMessage*> (pContext->getPPrismMessage());

        UdldIntfConfigClientMessage m;
        m.setInterfaceInfo(pMessage->getIfType(), pMessage->getIfName());
        m.setCommandCode(UDLD_INTF_CONF_DISABLE);
        m.setUdldDisable( pMessage->getCommandCode() != UDLD_ENABLE_CMD );

        //Disabling udld is allowed on an isl.
        if(0 == (string("true").compare(pContext->getCache()->getStringValue(INTF_PORT_ROLE)))
                && (UDLD_DISABLE_CMD == (pMessage->getCommandCode()))) {
            return WAVE_MESSAGE_SUCCESS;
        } else {
            return sendToUdldd(&m);
        }
    }

    static inline void fillIntf (PhyIntfLocalManagedObject *intfMo, bool udldEnable, const ObjectId &udldGlobalOid)
    {
        intfMo->setUdldEnable(udldEnable);
        intfMo->setUdldGlobalConfigOid(udldGlobalOid);
        intfMo->addAttributeToBeUpdated("udldEnable");
        intfMo->addAttributeToBeUpdated("udldGlobalAssoc");
    }

    ResourceId IntfUdldConfLocalWorker::updateIntfMoStep (DceSynchronousLinearSeqContext *pContext)
    {
        UdldIntfConfigMessage *pMessage = dynamic_cast<UdldIntfConfigMessage*> (pContext->getPPrismMessage());
        PhyIntfLocalManagedObject *intfMo = dynamic_cast<PhyIntfLocalManagedObject*> (pContext->getPWaveManagedObject());
        bool doUpdate = true;

        switch (pMessage->getCommandCode()) {
            case UDLD_ENABLE_CMD:
                fillIntf(intfMo, true, pContext->getWaveManagedObjectId());
                break;

            case UDLD_DISABLE_CMD:
                fillIntf(intfMo, false, ObjectId::NullObjectId);
                break;

            default:
                doUpdate = false;
                break;
        }

        if (doUpdate) {
            updateWaveManagedObject(intfMo);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId IntfUdldConfLocalWorker::clearAllIntfMoStep (DceSynchronousLinearSeqContext *pContext)
    {
        tracePrintf(TRACE_LEVEL_INFO, "IntfUdldConfLocalWorker::clearIntfMoStep : Clear all interface UDLD configs");
        WaveManagedObjectSynchronousQueryContextForUpdate *updateCtx = new WaveManagedObjectSynchronousQueryContextForUpdate(PhyIntfLocalManagedObject::getClassName());
        updateCtx->setLocationIdFilter(FrameworkToolKit::getThisLocationId());

        PhyIntfLocalManagedObject *criteriaMo = dynamic_cast<PhyIntfLocalManagedObject*> (updateCtx->getWaveManagedObjectToAddUpdateAttributes());
        fillIntf(criteriaMo, false, ObjectId::NullObjectId);

        updateMultipleWaveManagedObjects(updateCtx);

        return WAVE_MESSAGE_SUCCESS;
    }

    // Postboot..
    // TODO optimize by having bulk message..
    ResourceId IntfUdldConfLocalWorker::postbootIntfUdldConfig (PhyIntfLocalManagedObject *intfMo)
    {
        if (intfMo == NULL) {
            tracePrintf(TRACE_LEVEL_DEBUG, "IntfUdldConfLocalWorker::postbootIntfUdldConfig : NULL interface!");
            return WAVE_MESSAGE_SUCCESS;
        }

        string ifName = intfMo->getId();
        if (!intfMo->getUdldEnable()) {
            tracePrintf(TRACE_LEVEL_DEBUG, "IntfUdldConfLocalWorker::postbootIntfUdldConfig : UDLD is not enabled on %s, skipping playback..", ifName.c_str());
            return WAVE_MESSAGE_SUCCESS;
        }

        UdldIntfConfigClientMessage m;
        m.setInterfaceInfo(intfMo->getPhyType(), ifName);
        m.setCommandCode(UDLD_INTF_CONF_DISABLE);
        m.setUdldDisable(false);

        ResourceId status = sendToUdldd(&m);

        tracePrintf(TRACE_LEVEL_INFO, "IntfUdldConfLocalWorker::postbootIntfUdldConfig : replay of %s complte, status=%u", ifName.c_str(), status);
        return status;
    }

    ResourceId IntfUdldConfLocalWorker::sendToUdldd (DcmManagementInterfaceMessage *pMessage)
    {
        ResourceId status = sendSynchronouslyToWaveClient(UDLD_DCM_CLIENT_NAME, pMessage);
        if (status != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::sendToUdldd : Message send failed; opCode=%u, error=%u ( %s )", pMessage->getOperationCode(), status, FrameworkToolKit::localizeToSourceCodeEnum(status).c_str());
            return status;
        }

        if ((status = pMessage->getCompletionStatus()) != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::sendToUdldd : Message handling failed at framework; opCode=%u, error=%u ( %s )", pMessage->getOperationCode(), status, FrameworkToolKit::localizeToSourceCodeEnum(status).c_str());
            return status;
        }

        int retCode = pMessage->getClientStatus();
        if (retCode != 0 && retCode != WAVE_MESSAGE_SUCCESS) {
            status = (ResourceId) retCode; // Mapping is done in udldd itself!!
            tracePrintf(TRACE_LEVEL_ERROR, "IntfUdldConfLocalWorker::sendToUdldd : Message handling failed at backend; opCode=%u, error=%u ( %s )", pMessage->getOperationCode(), status, FrameworkToolKit::localizeToSourceCodeEnum(status).c_str());
            return status;
        }

        return WAVE_MESSAGE_SUCCESS;
    }

}
