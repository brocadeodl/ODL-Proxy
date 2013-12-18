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

#include <stdlib.h>
#include <set>
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Attributes/AttributeSqlIn.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpLocalWorker.h"
#include "Mstp/Local/MstpCreateBridgeConfigMessage.h"
//#include "Mstp/Local/MstpDeleteBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "Mstp/Local/MstpCreateVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpDeleteVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateVlanBridgeConfigMessage.h"
#include "Mstp/Local/MstpUpdateVlanConfigMessage.h"
#include "Mstp/Local/MstpUpdateMstpInstanceConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"
#include "Nsm/Local/XstpPortConfigManagedObject.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpVlanConfigMessage.h"
#include "ClientInterface/Mstp/MstpVlanBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpInstanceConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Mstp/MstpToolKit.h"
#include "ClientInterface/Mstp/MstpPortIdConfigMessage.h"
#include "ClientInterface/Mstp/MstpShowMessage.h"
#include "ClientInterface/Mstp/MstpClearMessage.h"
#include "Mstp/Local/MstpLocalShowSTPMessage.h"
#include "Mstp/Local/MstpLocalClearMessage.h"
#include "Nsm/Local/NsmUpdatePhyStpConfigMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "Utils/mo_iterator.h"


namespace DcmNs
{

    MstpLocalWorker::MstpLocalWorker ( MstpLocalObjectManager *pMstpLocalObjectManager)
        : WaveWorker  (pMstpLocalObjectManager)
    {
        addOperationMap (MSTPCREATEBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpCreateBridgeConfigMessageHandler));
#if 0
        addOperationMap (MSTPDELETEBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpDeleteBridgeConfigMessageHandler));
#endif
        addOperationMap (MSTPUPDATEBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpUpdateBridgeConfigMessageHandler));
        addOperationMap (MSTPCREATEVLANBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpCreateVlanBridgeConfigMessageHandler));
        addOperationMap (MSTPDELETEVLANBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpDeleteVlanBridgeConfigMessageHandler));
        addOperationMap (MSTPUPDATEVLANBRIDGECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpUpdateVlanBridgeConfigMessageHandler));
        addOperationMap (MSTPUPDATEMSTPINSTANCECONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpUpdateMstpInstanceConfigMessageHandler));
        addOperationMap (MSTPUPDATEVLANCONFIG, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpUpdateVlanConfigMessageHandler));
        addOperationMap (MSTPLOCALSHOWSTP, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpLocalShowSTPMessageHandler));
        addOperationMap (MSTPLOCALCLEARSTP, reinterpret_cast<PrismMessageHandler> (&MstpLocalWorker::MstpLocalClearMessageHandler));

    }

    MstpLocalWorker::~MstpLocalWorker ()
    {
    }

    PrismMessage  *MstpLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case MSTPCREATEBRIDGECONFIG :
                pPrismMessage = new MstpCreateBridgeConfigMessage ();
                break;
#if 0
            case MSTPDELETEBRIDGECONFIG :
                pPrismMessage = new MstpDeleteBridgeConfigMessage ();
                break;
#endif
            case MSTPUPDATEBRIDGECONFIG :
                pPrismMessage = new MstpUpdateBridgeConfigMessage ();
                break;
            case MSTPCREATEVLANBRIDGECONFIG :
                pPrismMessage = new MstpCreateVlanBridgeConfigMessage ();
                break;
            case MSTPDELETEVLANBRIDGECONFIG :
                pPrismMessage = new MstpDeleteVlanBridgeConfigMessage ();
                break;
            case MSTPUPDATEVLANBRIDGECONFIG :
                pPrismMessage = new MstpUpdateVlanBridgeConfigMessage ();
                break;
            case MSTPUPDATEMSTPINSTANCECONFIG :
                pPrismMessage = new MstpUpdateMstpInstanceConfigMessage ();
                break;
            case MSTPUPDATEVLANCONFIG :
                pPrismMessage = new MstpUpdateVlanConfigMessage ();
                break;
            case MSTPLOCALSHOWSTP :
                pPrismMessage = new MstpLocalShowSTPMessage();
                break;
            case MSTPLOCALCLEARSTP:
                pPrismMessage = new MstpLocalClearMessage();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *MstpLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void  MstpLocalWorker::MstpCreateBridgeConfigMessageHandler( MstpCreateBridgeConfigMessage *pMstpCreateBridgeConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpCreateBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  MstpLocalWorker::MstpCreateVlanBridgeConfigMessageHandler( MstpCreateVlanBridgeConfigMessage *pMstpCreateVlanBridgeConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpCreateVlanBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

#if 0
    void  MstpLocalWorker::MstpDeleteBridgeConfigMessageHandler( MstpDeleteBridgeConfigMessage *pMstpDeleteBridgeConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpDeleteBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }
#endif

    void  MstpLocalWorker::MstpDeleteVlanBridgeConfigMessageHandler( MstpDeleteVlanBridgeConfigMessage *pMstpDeleteVlanBridgeConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpDeleteVlanBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	/*
	 * Send bridge configure message to backend daemon
	 */
	ResourceId MstpLocalWorker::updateBridgeConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		MstpUpdateBridgeConfigMessage *pLocalMessage = dynamic_cast<MstpUpdateBridgeConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	
		trace (TRACE_LEVEL_DEBUG, "MstpLocalWorker::updateBridgeConfig: Enter \n");	
		/* Does Sanity check need here ??? */

		pLocalMessage->printMsg();

		MstpBridgeConfigMessage  message = *pLocalMessage;
		status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);
	
		return status;
    }

    ResourceId MstpLocalWorker::updateLogicalPortIdConfig(PrismSynchronousLinearSequencerContext *seqContext)
    {
        MstpUpdateBridgeConfigMessage *msg = dynamic_cast<MstpUpdateBridgeConfigMessage*>(seqContext->getPPrismMessage());
        if (!MstpToolKit::isVcsEnabled() || msg->getCmdCode() != MSTP_MSG_BR_CREATE) {
            return WAVE_MESSAGE_SUCCESS;
        }

        vector<mstp_msg_port_id_info> portIdList;

        ResourceId status = allocateLogicalPortIds(portIdList, msg->getMode());

        if (status != WAVE_MESSAGE_SUCCESS || portIdList.empty()) {
            return status;
        }

        MstpPortIdConfigMessage portIdMessage;

        // TODO have pagination...
        portIdMessage.setPortIdInfo(portIdList);

        status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &portIdMessage);

        if (status != WAVE_MESSAGE_SUCCESS) {
            // TODO free port ids???
            return status;
        }

        // Send to NSM for DB update...
        UI32 idCount = portIdList.size();
        UI32 buffSize = idCount * sizeof(mstp_msg_port_id_info);
        mstp_msg_port_id_info *idBuff = (mstp_msg_port_id_info*) calloc(1, buffSize);
        if (idBuff == NULL) {
            tracePrintf(TRACE_LEVEL_FATAL, "MstpLocalWorker::updateLogicalPortIdConfig : Out of memory!!");
            return WRC_MSTP_SYSTEM_ERROR;
        }

        for (UI32 i = 0; i < idCount; ++i) {
            mstp_msg_port_id_info &info = portIdList[i];
            memcpy(idBuff+i, &info, sizeof(mstp_msg_port_id_info));
        }

        NsmUpdatePhyStpConfigMessage uidSaveMsg;
        uidSaveMsg.setCmdCode(MSTP_MSG_SET_PORT_ID);
        uidSaveMsg.addBuffer(MSTP_PORT_ID_ARRAY_BUFTAG, buffSize, (void*) idBuff, true);

        status = sendSynchronously(&uidSaveMsg);

        if (status != WAVE_MESSAGE_SUCCESS || (status = uidSaveMsg.getCompletionStatus()) != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR, "MstpLocalWorker::updateLogicalPortIdConfig : Error saving port ids into database! err=%u", status);
        }

        return status;
    }

    ResourceId MstpLocalWorker::allocateLogicalPortIds(vector<mstp_msg_port_id_info> &portIdList, int brMode)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        LocationId thisLoc = FrameworkToolKit::getThisLocationId();
        bool isEdgePort = false;

        char phyIntfNamePattern[16];
        snprintf(phyIntfNamePattern, sizeof phyIntfNamePattern, "%d/%%",
                DcmToolKit::getMappedIdFromLocationId(thisLoc));

        mo_iterator<XstpPortConfigManagedObject> phyXstpIter;
        phyXstpIter.criteria().select("port").select("ownerManagedObjectId")
                .And("stpDisable", false)
                .And("port", string(phyIntfNamePattern), WAVE_ATTRIBUTE_CONDITION_OPERATOR_LIKE);
        while (status == WAVE_MESSAGE_SUCCESS && phyXstpIter.hasNext()) {
            XstpPortConfigManagedObject *mo = phyXstpIter.next();
            string ifName = mo->getPort();
            InterfaceType ifType = NsmUtils::getIfType(mo->getOwnerManagedObjectId());

            mo_criteria edgePortCriteria (PhyIntfLocalManagedObject::getClassName());
            edgePortCriteria.And("id", ifName).And("Portrole", false).And("active", true);
            if ((status = edgePortCriteria.queryExists(isEdgePort)) != WAVE_MESSAGE_SUCCESS) {
                tracePrintf(TRACE_LEVEL_ERROR, "MstpLocalWorker::allocateLogicalPortIds : Could not query state of %s; skip port id allocation..", ifName.c_str());
                continue;
            }
            if (!isEdgePort) {
                tracePrintf(TRACE_LEVEL_INFO, "MstpLocalWorker::allocateLogicalPortIds : Interface %s is an ISL or is inactive; skip port id allocation..", ifName.c_str());
                continue;
            }

            mstp_msg_port_id_info info;

            status = MstpToolKit::allocatePortId(info.port_id, brMode, ifType, ifName);
            if (status == WAVE_MESSAGE_SUCCESS) {
                info.if_type = ifType;
                strncpy((char*)info.if_name, (strchr(ifName.c_str(), '/')+1), MSTP_DCM_IF_NAME_LEN);
                portIdList.push_back(info);
            } else {
                // TODO revert all allocations..
                return status;
            }
        }

        set<UI64> usedInstanceIds;
        vector<Attribute*> poIdAttribs;
        mo_iterator<PhyIntfLocalManagedObject> phyIter;
        phyIter.criteria().select("poId").location(thisLoc).And("poMode", (UI32) 0, WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL).And("active", true);
        while (phyIter.hasNext()) {
            UI64 iid = phyIter.next()->getPoId().getInstanceId();
            if (usedInstanceIds.count(iid) == 0) {
                tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::allocateLogicalPortIds : PO instanceId = %llu", iid);
                poIdAttribs.push_back(new AttributeUI64(iid, "ownerManagedObjectIdInstanceId"));
                usedInstanceIds.insert(iid);
            }
        }

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::allocateLogicalPortIds : Found %u POs with members in this node", poIdAttribs.size());
        if (poIdAttribs.empty()) {
            return WAVE_MESSAGE_SUCCESS;
        }

        mo_iterator<XstpPortConfigManagedObject> poXstpIter;
        poXstpIter.criteria().select("port")
                .And("stpDisable", false)
                .And(new AttributeSqlIn(poIdAttribs), WAVE_ATTRIBUTE_CONDITION_OPERATOR_IN);
        while (poXstpIter.hasNext()) {
            string poId = poXstpIter.next()->getPort();
            mstp_msg_port_id_info info;
            status = MstpToolKit::allocatePortId(info.port_id, brMode, IF_TYPE_PO, poId);

            if (status == WAVE_MESSAGE_SUCCESS) {
                info.if_type = IF_TYPE_PO;
                strncpy((char*)info.if_name, poId.c_str(), MSTP_DCM_IF_NAME_LEN);
                portIdList.push_back(info);
            } else {
                // TODO revert all allocations..
                return status;
            }
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    void  MstpLocalWorker::MstpUpdateBridgeConfigMessageHandler( MstpUpdateBridgeConfigMessage *pMstpUpdateBridgeConfigMessage)
    {
	    trace (TRACE_LEVEL_INFO,"MstpLocalWorker::MstpUpdateBridgeConfigMessageHandler: Entering ... "); 

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::updateLogicalPortIdConfig),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::updateBridgeConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpUpdateBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	ResourceId MstpLocalWorker::updateVlanConfig(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		MstpUpdateVlanConfigMessage *pLocalMessage = dynamic_cast<MstpUpdateVlanConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	
		trace (TRACE_LEVEL_DEBUG, "MstpLocalWorker::updateVlanConfig: Enter \n");	
		/* Does Sanity check need here ??? */

		MstpVlanConfigMessage  message;
		message.setCmdCode(pLocalMessage->getCmdCode());
		message.setId(pLocalMessage->getId());

		status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);

		return status;
    }


    void  MstpLocalWorker::MstpUpdateVlanConfigMessageHandler(MstpUpdateVlanConfigMessage *pMstpUpdateVlanConfigMessage)
    {
	    trace (TRACE_LEVEL_INFO,"MstpLocalWorker::MstpUpdateVlanConfigMessageHandler : Entering ... "); 

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::updateVlanConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpUpdateVlanConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
	}

    ResourceId MstpLocalWorker::updateMstpInstanceConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
	    ResourceId status = WAVE_MESSAGE_SUCCESS;
	    MstpUpdateMstpInstanceConfigMessage *pLocalMessage = dynamic_cast<MstpUpdateMstpInstanceConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

	    trace (TRACE_LEVEL_DEBUG, "MstpLocalWorker::updateMstpInstanceConfigStep: Enter \n");	

	    pLocalMessage->printMsg();

	    MstpInstanceConfigMessage  message = *pLocalMessage;

//	    message.printMsg();

	    status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);

	    return status;
    }
            
    void  MstpLocalWorker::MstpUpdateMstpInstanceConfigMessageHandler( MstpUpdateMstpInstanceConfigMessage *pMstpUpdateMstpInstanceConfigMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::updateMstpInstanceConfigStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpUpdateMstpInstanceConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

	/*
	 * Send vlan bridge configure message to backend daemon
	 */
	ResourceId MstpLocalWorker::updateVlanBridgeConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
	{
		ResourceId status;
		MstpUpdateVlanBridgeConfigMessage *pLocalMessage = dynamic_cast<MstpUpdateVlanBridgeConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
	
		trace (TRACE_LEVEL_DEBUG, "MstpLocalWorker::updateVlanBridgeConfig: Enter \n");	
		/* Does Sanity check need here ??? */

		MstpVlanBridgeConfigMessage  message = *pLocalMessage;

        status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &message);

        return status;
    }

    void  MstpLocalWorker::MstpUpdateVlanBridgeConfigMessageHandler( MstpUpdateVlanBridgeConfigMessage *pMstpUpdateVlanBridgeConfigMessage)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::updateVlanBridgeConfig),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pMstpUpdateVlanBridgeConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void MstpLocalWorker::MstpLocalShowSTPMessageHandler(MstpLocalShowSTPMessage* pMstpLocalShowSTPMessage)
    {     	

	    if (pMstpLocalShowSTPMessage->getIsGwRequest()) 
	    {
		    PrismLinearSequencerStep sequencerSteps[] =
		    {
			    reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::sendMstpShowSTPCommandToCluster),
			    reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::prismLinearSequencerSucceededStep),
			    reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::prismLinearSequencerFailedStep)
		    };

		    PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pMstpLocalShowSTPMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
		    pPrismLinearSequencerContext->start ();
		    return;
	    } else {
		    PrismSynchronousLinearSequencerStep sequencerSteps[] =
		    {
			    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MstpLocalWorker::sendMstpShowSTPCommandToDaemon),
			    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MstpLocalWorker::prismSynchronousLinearSequencerSucceededStep),
			    reinterpret_cast<PrismSynchronousLinearSequencerStep> (&MstpLocalWorker::prismSynchronousLinearSequencerFailedStep)
		    };

		    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pMstpLocalShowSTPMessage, this, sequencerSteps,
				    sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
		    pPrismSynchronousLinearSequencerContext->execute ();

	    }
    }


    void MstpLocalWorker::sendMstpShowSTPCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
	    MSTP_PLUG_DBG("Entering ...");

	    MstpLocalShowSTPMessage *msg_p = dynamic_cast<MstpLocalShowSTPMessage*> (seq_ctx_p->getPPrismMessage ()); 
	    MSTP_PLUG_ASSERT_PTR(msg_p);

	    MstpLocalShowSTPMessage *loc_msg_p = new MstpLocalShowSTPMessage(msg_p);
	    MSTP_PLUG_ASSERT_PTR(loc_msg_p);

	    WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&MstpLocalWorker::MstpLocalSendToClusterCallback), seq_ctx_p);
	    MSTP_PLUG_ASSERT_PTR(send_ctx_p);

	    loc_msg_p->setIsGwRequest(false);
	    send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

	    switch( msg_p -> getCmdCode())
	    {
		    case DCM_SSM_SHOW_BPDUGUARD:
            case DCM_SSM_SHOW_TAGGED_BPDU:
			    {
			    MSTP_PLUG_DBG("DCM_SSM_SHOW_BPDUGUARD cmd code");

			    break;
			    }
		    case DCM_SSM_SHOW_BPDUGUARD_IF:
            case DCM_SSM_SHOW_TAGGED_BPDU_IF:
			    {
					MSTP_PLUG_DBG("DCM_SSM_SHOW_BPDUGUARD_IF cmd code");
				    if (true  == NsmUtils::isValidTwoTuple(msg_p -> getIfName()))
				    {
					    NSM_PLUG_DBG("It is tengig interface");
					    vector<LocationId> locationId;
					    if(msg_p -> getLocationId() != 0)
					    {	
						    locationId.push_back(msg_p -> getLocationId());
						    send_ctx_p->setLocationsToSendToForPhase1(locationId);
					    }	
				    }
				    else
				    {
					    vector<LocationId> temp;
					    vector<LocationId> locationId;
					    NSM_PLUG_DBG("It is po interface");
					    string poName = msg_p -> getIfName();
					    string ifName = poName;
					    NsmUtils::getPoMemberLocationIds(ifName, temp);
					    locationId.push_back(temp[0]);
					    send_ctx_p->setLocationsToSendToForPhase1(locationId);
				    }

				    break;
			    }
		    default:
			    MSTP_PLUG_DBG_ERR_S("In default case...");
			    break;
	    } 
	    sendToWaveCluster(send_ctx_p);
	    return;
    }


    ResourceId MstpLocalWorker::sendMstpShowSTPCommandToDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
	    ResourceId status = WAVE_MESSAGE_ERROR;

	    MSTP_PLUG_DBG("Entering...");

	    MstpLocalShowSTPMessage *msg_p = dynamic_cast<MstpLocalShowSTPMessage*> (seq_ctx_p->getPPrismMessage ());    

	    MstpShowMessage *m = new MstpShowMessage(msg_p);
        const char *clientName = MSTP_DCM_CLIENT_NAME;

	    switch (msg_p->getCmdCode()) {
	    case DCM_SSM_SHOW_BPDUGUARD:
	    case DCM_SSM_SHOW_BPDUGUARD_IF:
        case DCM_SSM_SHOW_TAGGED_BPDU:
        case DCM_SSM_SHOW_TAGGED_BPDU_IF:
	        m->setMappedId(DcmToolKit::getLocalMappedId());
            clientName = "ssm";
	        break;
	    }

        status = MstpToolKit::sendToBackend(clientName, m);
	    if (status == WAVE_MESSAGE_SUCCESS) {
	        m->transferAllBuffers(msg_p);
	    }

		delete m;
	    return status;
    }

    void MstpLocalWorker::MstpLocalClearMessageHandler (MstpLocalClearMessage *pMessage)
    {
        PrismLinearSequencerStep steps[] = {
                reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::processClearCommandStep),
                reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&MstpLocalWorker::prismLinearSequencerFailedStep)
        };

        PrismLinearSequencerContext *pSequencerContext = 
                new PrismLinearSequencerContext(pMessage, this, steps, (sizeof(steps) / sizeof(steps[0])));
        pSequencerContext->start();
    }

    void MstpLocalWorker::processClearCommandStep (PrismLinearSequencerContext *pSequencerContext)
    {
        MstpLocalClearMessage *pMessage = dynamic_cast<MstpLocalClearMessage*>( pSequencerContext->getPPrismMessage());
        tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::processClearCommandStep : MstpLocalClearMessage received.. cmdCode=%u, ifType=%u, ifName=\"%s\", distribute=%d", pMessage->getCmdCode(), pMessage->getIfType(), pMessage->getIfName().c_str(), pMessage->isDistribute());

        if (!pMessage->isDistribute()) {
            MstpClearMessage clearMessage;
            pMessage->copyTo(&clearMessage);

            ResourceId status = MstpToolKit::sendToBackend(MSTP_DCM_CLIENT_NAME, &clearMessage);
            if (status == WAVE_MESSAGE_SUCCESS) {
                clearMessage.transferAllBuffers(pMessage);
            }

            pSequencerContext->executeNextStep(status);
            return;
        }

        MstpLocalClearMessage *pRelayMessage = new MstpLocalClearMessage();
        pRelayMessage->copyFrom(pMessage);

        WaveSendToClusterContext *pClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback>(&MstpLocalWorker::MstpLocalSendToClusterCallback), pSequencerContext);
        pClusterContext->setPPrismMessageForPhase1(pRelayMessage);

        if (pMessage->getIfType() == IF_TYPE_PO) {
            int res = NsmUtils::setLocationIdsForPoSubModeCmds(pMessage->getIfName(), pClusterContext);
            if (res != SET_LOCATION_ID_API_SUCCESS) {
                if (res == PORT_NOT_PART_OF_ANY_INTERFACE) {
                    tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::processClearCommandStep : PO %s has no members", pMessage->getIfName().c_str());
                    res = WAVE_MESSAGE_SUCCESS;
                } else {
                    tracePrintf(TRACE_LEVEL_ERROR, "MstpLocalWorker::processClearCommandStep : Error %d", res);
                }
                delete pRelayMessage;
                delete pClusterContext;
                pSequencerContext->executeNextStep((ResourceId) res);
                return;
            }
        }

        sendToWaveCluster(pClusterContext);
        tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::processClearCommandStep : MstpLocalClearMessage have been relayed to cluster..");
    }


    void MstpLocalWorker::MstpLocalSendToClusterCallback (WaveSendToClusterContext *ctx_p)
    {
	    PrismLinearSequencerContext            *seq_ctx_p = NULL;
	    PrismMessage                *req_msg_p = NULL;
	    PrismMessage                *sw_resp_p = NULL;

	    seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());
	    MSTP_PLUG_ASSERT_PTR(seq_ctx_p);

	    req_msg_p = seq_ctx_p->getPPrismMessage ();
	    MSTP_PLUG_ASSERT_PTR(req_msg_p);

	    vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
	    UI32    i = 0;

        tracePrintf(TRACE_LEVEL_DEBUG, "MstpLocalWorker::MstpLocalSendToClusterCallback : found response from %u locations", locations.size());

	    for (i = 0; i < locations.size(); i++)
	    {
		    sw_resp_p = ctx_p->getResultingMessageForPhase1(locations[i]);
		    MSTP_PLUG_ASSERT_PTR(sw_resp_p);
		    if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
		    {
		        sw_resp_p->transferAllBuffers(req_msg_p);
		    } 
		    else 
		    {
                ResourceId err = sw_resp_p->getCompletionStatus();
                tracePrintf(TRACE_LEVEL_ERROR, "MstpLocalWorker::MstpLocalSendToClusterCallback : Error response received from location %u, error=%u ( %s )", locations[i], err, FrameworkToolKit::localize(err).c_str());
		    }
	    }

        delete ctx_p->getPPrismMessageForPhase1();
	    delete ctx_p;

	    seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);
    }

}
