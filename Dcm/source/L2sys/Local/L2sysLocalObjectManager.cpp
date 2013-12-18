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
 *   Author : aantony                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/StringUtils.h"
#include "DcmCore/DcmToolKit.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "L2sys/Global/L2sysAgingTimeoutManagedObject.h"
#include "L2sys/Global/L2sysGlobalStaticMacManagedObject.h"
#include "L2sys/Global/L2sysGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalClearSPMessage.h"
#include "L2sys/Local/L2sysLocalObjectManager.h"
#include "L2sys/Local/L2sysUpdateL2sysGlobalStaticMacSPMessage.h"
#include "L2sys/Local/L2sysUpdateL2sysAgingTimeoutSPMessage.h"
#include "L2sys/Local/L2sysLocalShowMacProfileMessage.h"
#include "L2sys/Local/L2sysShowRbridgeMacMessage.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "L2sys/Local/L2sysTypes.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"
#include "ClientInterface/L2sys/L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage.h"
#include "ClientInterface/L2sys/L2sysClearMessage.h"
#include "ClientInterface/L2sys/L2sysShowMessage.h"
#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "ClientInterface/Nsm/NsmUtils.h"

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "DcmCStatus.h"

#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "L2sys/Global/L2sysGlobalObjectManager.h"

//PVLAN
#include "Nsm/Common/PVlanUtils.h"

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "ClientInterface/L2sys/L2sysUtils.h"



namespace DcmNs
{

    L2sysLocalObjectManager::L2sysLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
		addOperationMap (L2SYSUPDATEL2SYSGLOBALSTATICMACSP, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysUpdateL2sysGlobalStaticMacSPMessageHandler));
		addOperationMap (L2SYSUPDATEL2SYSAGINGTIMEOUTSP, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysUpdateL2sysAgingTimeoutSPMessageHandler));
		addOperationMap (L2SYSUPDATEL2SYSGLOBALCLEARSP, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysUpdateL2sysGlobalClearSPMessageHandler));
		addOperationMap (L2SYSSHOWMACPROFILE, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysLocalShowMacProfileMessageHandler));
		addOperationMap (L2SYSSHOWRBRIDGEMAC, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysShowRbridgeMacMessageHandler));
		addOperationMap (L2SYSSHOWPORTSECURITY, reinterpret_cast<PrismMessageHandler> (&L2sysLocalObjectManager::L2sysLocalShowPortSecurityMessageHandler));
    }

    L2sysLocalObjectManager::~L2sysLocalObjectManager ()
    {
    }

    L2sysLocalObjectManager  *L2sysLocalObjectManager::getInstance()
    {
        static L2sysLocalObjectManager *pL2sysLocalObjectManager = new L2sysLocalObjectManager ();

        WaveNs::prismAssert (NULL != pL2sysLocalObjectManager, __FILE__, __LINE__);

        return (pL2sysLocalObjectManager);
    }

    string  L2sysLocalObjectManager::getClassName()
    {
        return ("L2sys");
    }

    PrismServiceId  L2sysLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *L2sysLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case L2SYSUPDATEL2SYSGLOBALSTATICMACSP :
                pPrismMessage = new L2sysUpdateL2sysGlobalStaticMacSPMessage ();
                break;
			case L2SYSUPDATEL2SYSAGINGTIMEOUTSP :
				pPrismMessage = new L2sysUpdateL2sysAgingTimeoutSPMessage ();
				break;
			case L2SYSUPDATEL2SYSGLOBALCLEARSP :
				pPrismMessage = new L2sysUpdateL2sysGlobalClearSPMessage ();
				break;
			case L2SYSSHOWMACPROFILE:
				pPrismMessage = new L2sysLocalShowMacProfileMessage ();
				break;
			case L2SYSSHOWRBRIDGEMAC:
				pPrismMessage = new L2sysShowRbridgeMacMessage ();
				break;
	    default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

	void L2sysLocalObjectManager::slotFailover (
		WaveAsynchronousContextForSlotFailover
		*pWaveAsynchronousContextForSlotFailover)
	{
		int32_t slotId = pWaveAsynchronousContextForSlotFailover->
			getSlotNumber();
		    ResourceId              status = WAVE_MESSAGE_SUCCESS;

        L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage *m = new L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage() ;
		L2sysGlobalUpdateL2sysGlobalStaticMacMessage *pL2sysGlobalUpdateL2sysGlobalStaticMacMessage = new L2sysGlobalUpdateL2sysGlobalStaticMacMessage();

		LocationId locId = FrameworkToolKit::getThisLocationId();

		trace (TRACE_LEVEL_INFO, string("L2syslocalObjectManager::slotFailover(): Entered with  SlotId = ") + slotId + string(" locId = ") + locId);


		m->setOpcode(L2SYS_NO_LINECARD);	
		m->setSlotid(slotId);	
		

		//SEND MESSAGE TO BACKEND TO DELETE
		trace (TRACE_LEVEL_INFO, "sending synchronously to l2sys client");
        status = sendSynchronouslyToWaveClient ("l2sys",m);

		if( status == WAVE_MESSAGE_SUCCESS) {
		  if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			  status = m->getCompletionStatus();
			  delete m;
			  return ;
		  }
		  int ClientStatus = m->getClientStatus();
		  status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
		  if (status != WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_INFO, "Error From l2sys client");
			m->setCompletionStatus(status);
			delete m;
			return ;
		  }
		}

		delete m;
		trace (TRACE_LEVEL_INFO, "Returning  Local L2SYS slotFailover........");


		pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->setOpcode(L2SYS_NO_LINECARD);
		pL2sysGlobalUpdateL2sysGlobalStaticMacMessage->setSlotid(slotId);

		//SEND MESSAGE TO GLOBAL PLUGIN
		status = sendSynchronously (pL2sysGlobalUpdateL2sysGlobalStaticMacMessage);
		trace (TRACE_LEVEL_INFO, "L2sysLocalObjectManager::slotFailover: status from Global Plugin is  " + status);


 
		delete pL2sysGlobalUpdateL2sysGlobalStaticMacMessage;
		pWaveAsynchronousContextForSlotFailover->
			setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForSlotFailover->callback();

		trace (TRACE_LEVEL_INFO, string("L2syslocalObjectManager::slotFailover(): DONE!  SlotId = ") + slotId + string(" LocationId = ") + locId);
	}



	
    WaveManagedObject  *L2sysLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    ResourceId L2sysLocalObjectManager::updateL2sysAging (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status;
        L2sysUpdateL2sysAgingTimeoutSPMessage *pGlobalMessage = dynamic_cast<L2sysUpdateL2sysAgingTimeoutSPMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		if(pGlobalMessage == NULL) {
					trace (TRACE_LEVEL_DEVEL, "pGlobalMessage is NULL; returning");
			return 0;
		}

        trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::updateL2sysAging: Enter \n");

        MstpBridgeConfigMessage  localMessage;
		localMessage.setAgingTime(pGlobalMessage->getAgingTimeout());
		localMessage.setCmdCode(MSTP_MSG_BR_MAC_AGING_TIME);
        status = sendSynchronouslyToWaveClient (MSTP_DCM_CLIENT_NAME, &localMessage);

        pGlobalMessage->setCompletionStatus(localMessage.getCompletionStatus());

		pGlobalMessage->setCompletionStatus(localMessage.getCompletionStatus());
		if( status == WAVE_MESSAGE_SUCCESS) {
		  if(  localMessage.getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			return  localMessage.getCompletionStatus();
		  }
		  int ClientStatus = localMessage.getClientStatus();
		  status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
		  if (status != WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_INFO, "Error From mstp client");
			localMessage.setCompletionStatus(status);
			return  localMessage.getCompletionStatus();
		  }
		}

        trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::updateL2sysAging: status "+FrameworkToolKit::localize(status));
        return (status);
    }

    void  L2sysLocalObjectManager::L2sysUpdateL2sysAgingTimeoutSPMessageHandler( L2sysUpdateL2sysAgingTimeoutSPMessage *pL2sysUpdateL2sysAgingTimeoutSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::updateL2sysAging),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pL2sysUpdateL2sysAgingTimeoutSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


    ResourceId L2sysLocalObjectManager:: getPhyIntfIfindexByPoIfName (string phyName, UI64 *ifindex)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeString(&phyName,"id"));
        syncQueryCtxt.addSelectField("ifIndex");
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults && pResults->size() > 0) {
            pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*pResults)[0]);
			if (pPhyIntfLocalManagedObject == NULL) {
					trace (TRACE_LEVEL_DEVEL, "pPhyIntfLocalManagedObject is NULL; returning");
				return status;
			}
            *ifindex = pPhyIntfLocalManagedObject->getIfIndex();
            status = WAVE_MESSAGE_SUCCESS;
        }

        if (pResults) {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
        return status;
    }

    ResourceId L2sysLocalObjectManager::L2sysUpdateMac(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status = 0;
        L2sysUpdateL2sysGlobalStaticMacSPMessage *pL2sysUpdateL2sysGlobalStaticMacSPMessage = dynamic_cast<L2sysUpdateL2sysGlobalStaticMacSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage *m = new L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage() ;

		if ((pL2sysUpdateL2sysGlobalStaticMacSPMessage == NULL) || (m == NULL)) {
			trace (TRACE_LEVEL_DEVEL, "pL2sysUpdateL2sysGlobalStaticMacSPMessage or m is NULL; returning");
			return 0;
		}

        string        ifName = pL2sysUpdateL2sysGlobalStaticMacSPMessage->getIfname();
		string        twoTuppleIfName;
		UI64		  ifindex=0;

	     trace (TRACE_LEVEL_INFO, string("Entering Local L2sysUpdateMac........") + pL2sysUpdateL2sysGlobalStaticMacSPMessage->getPortChannelId());
        m->setMac(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getMac());
        m->setForward(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getForward());
        m->setInterface_type(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getInterface_type());
        m->setPortChannelId(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getPortChannelId());
		cout<< " L2sysUpdateMac               ifname = "<<ifName <<endl;
		if(getPhyIntfIfindexByPoIfName(ifName,&ifindex)!=WAVE_MESSAGE_ERROR)
		{
			cout<< " L2sysUpdateMac               ifindex = "<<ifindex <<endl;
	        m->setIfindex(ifindex);
		}
		else
	        m->setIfindex(0);
		if(true  == NsmUtils::isValidThreeTuple(ifName) )
		{
			NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
			ifName = twoTuppleIfName;
		}	
	    // trace (TRACE_LEVEL_INFO, "Entering Local L2sysUpdateMac........"+ifName);
        m->setIfname(ifName);
		m->setVlan(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getVlan());
		m->setVlanid(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getVlanid());
        m->setOpcode(pL2sysUpdateL2sysGlobalStaticMacSPMessage ->getOpcode());
		m->setRbridgeid(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getRbridgeid());

		if(PVlanUtils::isSecondaryVlan(pL2sysUpdateL2sysGlobalStaticMacSPMessage->getVlanid())){
			status = WRC_NSM_PVLAN_SEC_VLAN;
			trace (TRACE_LEVEL_INFO, "L2sysLocalObjectManager::PVlanUtils::isSecondaryVlan ");
			return status;
		}

		trace(TRACE_LEVEL_DEVEL, "L2sysLocalObjectManager::L2sysUpdateMac sending synchronously to l2sys client");
        status = sendSynchronouslyToWaveClient ("l2sys",m);

		pL2sysUpdateL2sysGlobalStaticMacSPMessage->setCompletionStatus(m->getCompletionStatus());
		if( status == WAVE_MESSAGE_SUCCESS) {
		  if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			  //bpatel: To avoid memory leak
			  status = m->getCompletionStatus();
			  delete m;
			  return status;
		  }
		  int ClientStatus = m->getClientStatus();
		  status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
		  if (status != WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_INFO, "Error From l2sys client");
			m->setCompletionStatus(status);
			//bpatel: To avoid memory leak
			status = m->getCompletionStatus();
			delete m;
			return status;
		  }
		}

		delete m;
		trace (TRACE_LEVEL_INFO, "Returning  Local L2sysUpdateMac........");
        return  (status);
    }

    void  L2sysLocalObjectManager::L2sysUpdateL2sysGlobalStaticMacSPMessageHandler( L2sysUpdateL2sysGlobalStaticMacSPMessage *pL2sysUpdateL2sysGlobalStaticMacSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::L2sysUpdateMac),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pL2sysUpdateL2sysGlobalStaticMacSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

     void L2sysLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        trace (TRACE_LEVEL_INFO, string("L2sysLocalObjectManager::postboot Entered with pass:") + pWaveAsynchronousContextForPostBootPhase->getPassNum());
        string name = pWaveAsynchronousContextForPostBootPhase->getPassName();

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE3") {
            trace (TRACE_LEVEL_DEVEL, "ConfigReplayVlanClassifier: Replaying L2sys Global Configuration......");
            postBootConfigureL2sysAgingConfig();
            postBootConfigureL2sysIntefaceConfig();
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();

    }
	 void L2sysLocalObjectManager::postBootConfigureL2sysAgingConfig()
    {
        UI32 agingTime;
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        trace (TRACE_LEVEL_DEBUG, "Entering L2sysLocalObjectManager::postBootConfigureL2sysAgingConfig");
        L2sysAgingTimeoutManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults = querySynchronously(L2sysAgingTimeoutManagedObject::getClassName());
        if (pResults && pResults->size() > 0) {
            pMo = dynamic_cast<L2sysAgingTimeoutManagedObject*>((*pResults)[0]);
        }
        if (pMo) {
            agingTime = pMo->getAgingTimeout();
            trace (TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysAgingConfig: agingTime ") + agingTime);
            MstpBridgeConfigMessage  localMessage;
            localMessage.setAgingTime(agingTime);
            localMessage.setCmdCode(MSTP_MSG_BR_MAC_AGING_TIME);
            status = sendSynchronouslyToWaveClient (MSTP_DCM_CLIENT_NAME, &localMessage);

            if( status == WAVE_MESSAGE_SUCCESS) {
                if(localMessage.getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                    status = localMessage.getCompletionStatus();
                } else {
                    int ClientStatus = localMessage.getClientStatus();
                    status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
                }
            }
        }
        trace (TRACE_LEVEL_INFO, "L2sysLocalObjectManager::updateL2sysAging: status "+FrameworkToolKit::localize(status));
		if (pResults)
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
    }


      void L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig()
    {
        trace (TRACE_LEVEL_INFO, "Entering L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig");
        WaveManagedObjectSynchronousQueryContext SyncQueryCtxt(L2sysGlobalStaticMacManagedObject::getClassName());
        vector<WaveManagedObject * >*            pResults = querySynchronously(&SyncQueryCtxt);
        L2sysGlobalStaticMacManagedObject*                     pL2sysGlobalStaticMacManagedObject;
		string ifName,twoTuppleIfName;
		UI64 ifindex;
		UI32 po_id;
		int mapid=DcmToolKit::getLocalMappedId();
		int myrbrid=DcmToolKit::getDomainIdFromMappedId(mapid);

		if (pResults == NULL) {
			trace(TRACE_LEVEL_INFO, "L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig query returned NULL");
		}

        if (NULL != pResults && pResults -> size() > 0)
        {
            trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig results size ") + pResults->size());

            /* Process configuration replay of "mac-address-table static" configuration commands */
            for(unsigned int i = 0;i <  pResults -> size(); i++)
            {
				if (((*pResults)[i]) == NULL) {
					trace(TRACE_LEVEL_INFO, "L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig (*pResults)[i]) is NULL for %d; returning" + i);
					return;
				}
			  pL2sysGlobalStaticMacManagedObject = dynamic_cast<L2sysGlobalStaticMacManagedObject*>((*pResults)[i]);
			  if (pL2sysGlobalStaticMacManagedObject == NULL) {
				  trace(TRACE_LEVEL_INFO, "L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig pL2sysGlobalStaticMacManagedObject is NULL; returning");
				  return;
			  }

				/* If VCS is enabled, check if "mac-address-table static" commands is for port-channel */
				if (WAVE_PERSISTENCE_CLUSTER_ENABLED == DcmToolKit::isVcsEnabled()) {
					trace(TRACE_LEVEL_DEVEL, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig VCS enabled "));

					if (pL2sysGlobalStaticMacManagedObject->getInterface_type() == INTERFACE_TYPE_PORTCHANNEL) {
						trace (TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig port-channel MAC ") + pL2sysGlobalStaticMacManagedObject->getIfname());
						PoIntfManagedObject *pPoIntfManagedObject = NULL;
						UI32 port_channel_id;
						ObjectId poObjectId;
						PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
						string phyName;

						WaveManagedObjectSynchronousQueryContext poSyncQueryCtxt(PoIntfManagedObject::getClassName());
						port_channel_id = strtoul(pL2sysGlobalStaticMacManagedObject->getIfname().c_str(), NULL, 0);
						poSyncQueryCtxt.addAndAttribute (new AttributeUI32(&port_channel_id,"id"));
						vector<WaveManagedObject *> *poResults = querySynchronously(&poSyncQueryCtxt);

						if (poResults != NULL) {
							if (poResults->size() > 0) {
								pPoIntfManagedObject =  dynamic_cast<PoIntfManagedObject*>((*poResults)[0]);

								if (pPoIntfManagedObject != NULL) {	/* Port channel MO exists */
									trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig found port-channel MO ") + port_channel_id);
									poObjectId =  pPoIntfManagedObject->getObjectId();
									WaveManagedObjectSynchronousQueryContext phySyncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
									phySyncQueryCtxt.addAndAttribute (new AttributeObjectId(poObjectId,"poId"));
									vector<WaveManagedObject *> *phyResults = querySynchronously(&phySyncQueryCtxt);

									if (phyResults != NULL) {
										unsigned int j;

										if (phyResults->size() > 0) { /* Physical port assigned to port-channel */
											trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig physical ports assigned to port-channel ") + phyResults->size());

											/* Before executing "mac-address-table static" commands for a port-channel, */
											/*   check if this switch is participating in the vLAG */
											/*   Ignore "mac-address-table static" command if this switch is not */
											/*   participating in an active port-channel.  The static MAC address */
											/*   configuration will have been passed to this switch through eNS. */
											for (j = 0; j <  phyResults->size(); j++) {
												pPhyIntfLocalManagedObject =  dynamic_cast<PhyIntfLocalManagedObject*>((*phyResults)[j]);
												phyName = pPhyIntfLocalManagedObject->getIfName();

												if ((L2sysUtils::tell_rbrid((char *)phyName.c_str())) == myrbrid) {
													trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig this switch owns physical port ") + phyName);
													break;	/* This switch owns the physical port */
												} else {
													trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig other switch owns physical port ") + phyName);
												}
											}

											if (j == phyResults->size()) {	/* No physical ports owned by this switch */
												trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig No physical ports assigned to port-channel owned by this switch") + port_channel_id);
												WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyResults);
												WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poResults);
												continue;
											}
										}

										WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(phyResults);
									}
								}
							} else {
								trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig no port-channel MO returned ") + pL2sysGlobalStaticMacManagedObject->getIfname());
							}

							WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(poResults);
						} else {
							trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig no port-channel MO ") + pL2sysGlobalStaticMacManagedObject->getIfname());
						}
					} else {
						trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig physical port ") + pL2sysGlobalStaticMacManagedObject->getIfname());
					}
			  }

				L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage *m = new L2sysClientGlobalUpdateL2sysGlobalStaticMacMessage() ;

				m->setMac(pL2sysGlobalStaticMacManagedObject->getMac());
				m->setForward(pL2sysGlobalStaticMacManagedObject->getForward());
				m->setInterface_type(pL2sysGlobalStaticMacManagedObject->getInterface_type());
				ifName=pL2sysGlobalStaticMacManagedObject->getIfname();
				m->setRbridgeid(L2sysUtils::tell_rbrid((char *)ifName.c_str()));

				if (getPhyIntfIfindexByPoIfName(ifName,&ifindex) != WAVE_MESSAGE_ERROR) {
					trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig set ifindex ") + ifindex);
					m->setIfindex(ifindex);
				} else {
					trace(TRACE_LEVEL_DEBUG, string("L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig ifindex when no physical port ") + m->getIfindex());
				}

				if(true  == NsmUtils::isValidThreeTuple(ifName) )
				{
					NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
					ifName = twoTuppleIfName;
				}	
				//m->setIfname(pL2sysGlobalStaticMacManagedObject->getIfname());
				m->setIfname(ifName);
				m->setVlan(pL2sysGlobalStaticMacManagedObject->getVlan());
				m->setVlanid(pL2sysGlobalStaticMacManagedObject->getVlanid());

				/* Set the port-channel id, incase of PO */
				if (pL2sysGlobalStaticMacManagedObject->getInterface_type() == INTERFACE_TYPE_PORTCHANNEL) {
					po_id = strtoul(pL2sysGlobalStaticMacManagedObject->getIfname().c_str(), NULL, 0);
					m->setPortChannelId(po_id);
				}

				m->setOpcode(L2SYS_SET_OP);
				trace(TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::postBootConfigureL2sysInterfaceConfig sending synchronously to l2sys client");
				sendSynchronouslyToWaveClient ("l2sys",m);
				delete m;
            }
		} else {
            trace(TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::postBootConfigureL2sysIntefaceConfig no results");
        }
        if(pResults)
        {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }

    }

    ResourceId L2sysLocalObjectManager::L2sysClearMac(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int status = 0;
        PhyIntfLocalManagedObject* phyMO=NULL;
        UI32 ifIndex=0;

	     trace (TRACE_LEVEL_INFO, "Entering Local L2sysClearMac........");
        L2sysUpdateL2sysGlobalClearSPMessage *pL2sysUpdateL2sysGlobalClearSPMessage = dynamic_cast<L2sysUpdateL2sysGlobalClearSPMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
		if (pL2sysUpdateL2sysGlobalClearSPMessage == NULL)
			return status;

        L2sysClearMessage *m = new L2sysClearMessage() ;
        string        ifName = pL2sysUpdateL2sysGlobalClearSPMessage->getIfname();

        m->setMac(pL2sysUpdateL2sysGlobalClearSPMessage->getMac());
        m->setType(pL2sysUpdateL2sysGlobalClearSPMessage->getType());
        m->setIfName(ifName);
		m->setVlanid(pL2sysUpdateL2sysGlobalClearSPMessage->getVlanid());
		m->setBridge(pL2sysUpdateL2sysGlobalClearSPMessage->getRbridgeid());
        m->setCmdCode(pL2sysUpdateL2sysGlobalClearSPMessage ->getOpcode());
        m->setInterfaceType(pL2sysUpdateL2sysGlobalClearSPMessage->getInterfaceType());
        m->setPortChannelId(pL2sysUpdateL2sysGlobalClearSPMessage->getPortChannelId());
        m->setIfindex(0);

        /* Get ifindex  for phy intf*/
        if((L2SYS_CLEAR_DYNAMIC_INTERFACE_ONEGIG == m->getCmdCode()) || (L2SYS_CLEAR_DYNAMIC_INTERFACE_TENGIG == m->getCmdCode()) || (L2SYS_CLEAR_DYNAMIC_INTERFACE_FORTYGIG == m->getCmdCode()))
        {
            NsmUtils::getPhyMoByPhyName(m->getIfName(),phyMO);
            if(NULL == phyMO)
            {
                trace(TRACE_LEVEL_INFO,"Got NULL Phy MO\n");
            }else
            {
                trace(TRACE_LEVEL_INFO,"Got Phy MO\n");
                ifIndex = static_cast<UI32> (phyMO->getIfIndex());
                m->setIfindex(ifIndex);
            }
        }
        if(true  == NsmUtils::isValidThreeTuple(ifName) )
        {
            string twoTuppleIfName ;
            NsmUtils::getTwoTupleIfName(ifName, twoTuppleIfName);
            ifName = twoTuppleIfName;
            m->setIfName(ifName);
        }	

        trace(TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::L2sysClearMac sending synchronously to l2sys client");
        status = sendSynchronouslyToWaveClient ("l2sys",m);

		pL2sysUpdateL2sysGlobalClearSPMessage->setCompletionStatus(m->getCompletionStatus());
		if( status == WAVE_MESSAGE_SUCCESS) {
		  if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
			  //bpatel: To avoid memory leak
			  status =  m->getCompletionStatus();
			  delete m;
			  return status;
		  }
		  int ClientStatus = m->getClientStatus();
		  status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
		  if (status != WAVE_MESSAGE_SUCCESS) {
			trace(TRACE_LEVEL_INFO, "Error From l2sys client");
			m->setCompletionStatus(status);
			//bpatel: To avoid memory leak
			status = m->getCompletionStatus();
			delete m;
			return status;
		  }
		}

		delete m;
		delete phyMO;
		phyMO = NULL;
		trace (TRACE_LEVEL_INFO, "Returning  Local L2sysUpdateMac........");
        return  (status);
    }

    void  L2sysLocalObjectManager::L2sysUpdateL2sysGlobalClearSPMessageHandler( L2sysUpdateL2sysGlobalClearSPMessage *pL2sysUpdateL2sysGlobalClearSPMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::L2sysClearMac),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pL2sysUpdateL2sysGlobalClearSPMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

  /*Show mac-address-table port-profile message handler"*/
  void  L2sysLocalObjectManager::L2sysLocalShowMacProfileMessageHandler (L2sysLocalShowMacProfileMessage *pL2sysLocalShowMacProfileMessage) 
  {
	  trace(TRACE_LEVEL_INFO, "Entering L2sysLocalObjectManager::L2sysLocalShowMacProfileMessageHandler");
	  if (pL2sysLocalShowMacProfileMessage->getIsGwRequest()) {
		  PrismLinearSequencerStep sequencerSteps[] =
		  {
			  reinterpret_cast<PrismLinearSequencerStep>(&L2sysLocalObjectManager::showMessageClusterStep),
			  reinterpret_cast<PrismLinearSequencerStep>(&L2sysLocalObjectManager::prismLinearSequencerSucceededStep),
			  reinterpret_cast<PrismLinearSequencerStep>(&L2sysLocalObjectManager::prismLinearSequencerFailedStep)
		  };

		  PrismLinearSequencerContext *pPrismLinearSequencerContext =
			  new PrismLinearSequencerContext(pL2sysLocalShowMacProfileMessage,
				  this, sequencerSteps,
				  sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));

		  pPrismLinearSequencerContext->start ();
		  return;
	  } else {
		  PrismSynchronousLinearSequencerStep sequencerSteps[] =
		  {
			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::showMessageStep),
			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
			  reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
		  };

		  PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new 
			  PrismSynchronousLinearSequencerContext (pL2sysLocalShowMacProfileMessage, this,
				  sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

		  pPrismSynchronousLinearSequencerContext->execute ();
	  }
  }

  ResourceId L2sysLocalObjectManager::showMessageClusterStep(PrismLinearSequencerContext *pPrismLinearSequencerContext)
  {
	  int status = WAVE_MESSAGE_SUCCESS;
	  L2sysLocalShowMacProfileMessage *pMsg = NULL;
	  L2sysLocalShowMacProfileMessage *pMsgLocal = NULL;
	  WaveSendToClusterContext *pWaveSendToClusterContext = NULL;
	  vector<UI32> locations;

	  pMsg = dynamic_cast<L2sysLocalShowMacProfileMessage *> (pPrismLinearSequencerContext->getPPrismMessage());

	  if (pMsg == NULL) {
	      trace (TRACE_LEVEL_INFO, "showMessageClusterStep: getPPrismMessage returned NULL; returning.....");
	      return (WAVE_MESSAGE_ERROR);
	  }

	  trace(TRACE_LEVEL_INFO, "L2sysLocalShowMacProfileMessage::showMessageClusterStep enter");

	  l2sys_show_mac_output_msg_t *lastMacRecord = (l2sys_show_mac_output_msg_t *)malloc(sizeof(l2sys_show_mac_output_msg_t));
	  memset(lastMacRecord, 0, sizeof(l2sys_show_mac_output_msg_t));
	  pMsgLocal = new L2sysLocalShowMacProfileMessage(pMsg->getRbridgeid(),pMsg->getMac(),pMsg->getInterface_type(),pMsg->getIfname(),pMsg->getIfindex(),pMsg->getVlan(),pMsg->getVlanId(), pMsg->getCmdcode(),pMsg->getProfiled(),pMsg->getType(), false);
	  pMsgLocal->setIsFirstRequest(pMsg->getIsFirstRequest());
	  pMsg->getLastMacRecord(lastMacRecord);
	  pMsgLocal->setLastMacRecord(lastMacRecord);
	  pWaveSendToClusterContext = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> 

		  (&L2sysLocalObjectManager::showMessageClusterStepCallback), pPrismLinearSequencerContext);
	  free(lastMacRecord);

	  if (!pWaveSendToClusterContext) {
		  delete pMsgLocal;
		  return (status);
	  }

	  pWaveSendToClusterContext->setPPrismMessageForPhase1(pMsgLocal);
	  sendToWaveCluster(pWaveSendToClusterContext);
	  return (status);
  } 
    
  void L2sysLocalObjectManager::showMessageClusterStepCallback(WaveSendToClusterContext *pWaveSendToClusterContext)
  {
	  PrismLinearSequencerContext *pPrismLinearSequencerContext = NULL;
	  L2sysLocalShowMacProfileMessage *pReqMsg = NULL, *pRespMsg = NULL,*pResultMsg = NULL;


	  pPrismLinearSequencerContext = reinterpret_cast<PrismLinearSequencerContext *>
		  (pWaveSendToClusterContext->getPCallerContext());

	  pReqMsg = dynamic_cast<L2sysLocalShowMacProfileMessage *>
		  (pPrismLinearSequencerContext->getPPrismMessage ());

	  pRespMsg = dynamic_cast<L2sysLocalShowMacProfileMessage *>
		  (pWaveSendToClusterContext->getPPrismMessageForPhase1 ());


	  vector<LocationId> locations =  pWaveSendToClusterContext->getLocationsToSendToForPhase1();
	  UI32    i = 0;

	  trace(TRACE_LEVEL_INFO, string("L2sysLocalObjectManager::showMessageClusterStepCallback # of responses: ")
		  + locations.size());

	  if (locations.size() == 0) {
		  trace(TRACE_LEVEL_INFO, string("L2sysLocalObjectManager::showMessageClusterStepCallback no responses recceived"));
	  }

	  for (i = 0; i < locations.size(); i++) {
		  trace(TRACE_LEVEL_INFO, string("L2sysLocalObjectManager::showMessageClusterStepCallback location id") + locations[i]);
		  pResultMsg = dynamic_cast<L2sysLocalShowMacProfileMessage *>
			  (pWaveSendToClusterContext->getResultingMessageForPhase1(locations[i]));
		  if (WAVE_MESSAGE_SUCCESS == pResultMsg->getCompletionStatus()) {
			  pReqMsg->copyAllBuffers(*pResultMsg);
		  } else {
			  trace(TRACE_LEVEL_ERROR, string("Response processing: The location: ") + locations[i] + string("returned error: ")  +
				  FrameworkToolKit::localize(pResultMsg->getCompletionStatus()));
		  }
	  }

	  pPrismLinearSequencerContext->executeNextStep(WAVE_MESSAGE_SUCCESS);

	  delete pRespMsg;
	  delete pWaveSendToClusterContext;
	  trace(TRACE_LEVEL_INFO, string("L2sysLocalObjectManager::showMessageClusterStepCallback at the end"));

	  return;
  }
  ResourceId L2sysLocalObjectManager::showMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  {
	  int status = WAVE_MESSAGE_SUCCESS;
	  SI32 mappedId = 0;
	  l2sys_show_mac_output_msg_t *lastMacRecord = (l2sys_show_mac_output_msg_t *)malloc(sizeof(l2sys_show_mac_output_msg_t));
	  memset(lastMacRecord, 0, sizeof(l2sys_show_mac_output_msg_t));

	  trace(TRACE_LEVEL_INFO, "L2sysLocalObjectManager::showMessageStep Local message received");

	  L2sysLocalShowMacProfileMessage *pL2sysLocalShowMacProfileMessage =
		  dynamic_cast<L2sysLocalShowMacProfileMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

	  if (pL2sysLocalShowMacProfileMessage == NULL) {
	      trace (TRACE_LEVEL_INFO, "showMessageStep: getPPrismMessage returned NULL; returning.....");
		  free(lastMacRecord);
	      return (WAVE_MESSAGE_ERROR);
	  }

	  mappedId = DcmToolKit::getLocalMappedId();
	  if (mappedId == (-1)) {
		  mappedId = 0;
	  }

	  L2sysShowMessage *pMsg = new L2sysShowMessage();

	  prismAssert(NULL != pMsg, __FILE__, __LINE__);

	  pMsg->setInterface(" ");
	  pMsg->setIfName(" ");
	  if(pL2sysLocalShowMacProfileMessage->getCmdcode() == L2SYSSHOW_MAC_PORT_PROFILE_ADDRESS){
		  pMsg->setMac(pL2sysLocalShowMacProfileMessage->getMac());
	  }
	  else{
		  pMsg->setMac(" ");
	  }
	  if(pL2sysLocalShowMacProfileMessage->getCmdcode() == L2SYSSHOW_MAC_PORT_PROFILE_VLAN) {
		  pMsg->setVlanid(pL2sysLocalShowMacProfileMessage->getVlanId());
	  }
	  else {
		  pMsg->setVlanid(0);
	  }
	  pMsg->setType(0);
	  pMsg->setCmdCode(pL2sysLocalShowMacProfileMessage->getCmdcode());
	  pMsg->setBridge(0);
	  pMsg->setIfindex(0);
	  pMsg->setInterfaceType(0);
	  pMsg->setPortChannelId(0);
	  pMsg->setMappedId(mappedId);
	  pMsg->setIsFirstRequest(pL2sysLocalShowMacProfileMessage->getIsFirstRequest());
	  pL2sysLocalShowMacProfileMessage->getLastMacRecord(lastMacRecord);
	  pMsg->setLastMacRecord(lastMacRecord);

	  trace(TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::showMessageStep sending synchronously to l2sys client");
	  status = sendSynchronouslyToWaveClient("l2sys",pMsg);
	  free(lastMacRecord);

	  if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
		  status = pMsg->getCompletionStatus();
		  delete pMsg;
		  return status;
	  }

	  int ClientStatus = pMsg->getClientStatus();
	  status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
	  if (status != WAVE_MESSAGE_SUCCESS) {
		  trace(TRACE_LEVEL_INFO, "Error From l2sys client");
		  pMsg->setCompletionStatus(status);
		  //bpatel: To avoid memory leak
		  status = pMsg->getCompletionStatus();
		  delete pMsg;
		  return status;
	  }

	  trace (TRACE_LEVEL_INFO, "L2sysLocalObjectManager::showMessageStep message sent to L2sys backend");
	  pL2sysLocalShowMacProfileMessage->copyAllBuffers(*pMsg);	

	  delete pMsg;
	  return (status);
  } 
 
  void L2sysLocalObjectManager::L2sysShowRbridgeMacMessageHandler(L2sysShowRbridgeMacMessage* pL2sysShowRbridgeMacMessage)
  {     
      trace (TRACE_LEVEL_DEBUG, "L2sysShowRbridgeMacMessageHandler: entered");
      if (pL2sysShowRbridgeMacMessage->getIsGwRequest()) 
      {
          PrismLinearSequencerStep sequencerSteps[] =
          {
               reinterpret_cast<PrismLinearSequencerStep> (&L2sysLocalObjectManager::sendL2sysShowMacCommandToCluster),
               reinterpret_cast<PrismLinearSequencerStep> (&L2sysLocalObjectManager::prismLinearSequencerSucceededStep),
               reinterpret_cast<PrismLinearSequencerStep> (&L2sysLocalObjectManager::prismLinearSequencerFailedStep)
          };

          PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pL2sysShowRbridgeMacMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
          pPrismLinearSequencerContext->start ();
          return;
      } else {
          PrismSynchronousLinearSequencerStep sequencerSteps[] =
          {
              reinterpret_cast<PrismSynchronousLinearSequencerStep> (&L2sysLocalObjectManager::sendL2sysShowMacCommandToL2sysDaemon),
              reinterpret_cast<PrismSynchronousLinearSequencerStep> (&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
              reinterpret_cast<PrismSynchronousLinearSequencerStep> (&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
          };

          PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext( pL2sysShowRbridgeMacMessage, this, sequencerSteps,
          sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
          pPrismSynchronousLinearSequencerContext->execute ();
      }
  }

  void L2sysLocalObjectManager::sendL2sysShowMacCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
  {
      SI32 locationId = -1;
      vector<UI32>                locations;

        trace (TRACE_LEVEL_DEBUG, "sendL2sysShowMacCommandToCluster: entered");

        L2sysShowRbridgeMacMessage *msg_p = dynamic_cast<L2sysShowRbridgeMacMessage*> (seq_ctx_p->getPPrismMessage ()); 

        if (msg_p == NULL) {
            trace (TRACE_LEVEL_INFO, "sendL2sysShowMacCommandToCluster: getPPrismMessage returned NULL; returning.....");
            return;
        }

        L2sysShowRbridgeMacMessage *loc_msg_p = new L2sysShowRbridgeMacMessage(msg_p -> getIfName(), msg_p -> getCmdCode(), msg_p->getIfindex(), msg_p->getType(), false, msg_p->getFromRbridgeId(), msg_p->getForRbridgeId());

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&L2sysLocalObjectManager::L2sysShowMacSendToClusterCallback), seq_ctx_p);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);

        locationId = DcmToolKit::getLocationIdFromMappedId(msg_p->getFromRbridgeId());
        locations.push_back(locationId);
        send_ctx_p->setLocationsToSendToForPhase1(locations);

        trace(TRACE_LEVEL_DEBUG, "sendL2sysShowMacCommandToCluster: send to rbridge " + msg_p->getFromRbridgeId());
        sendToWaveCluster(send_ctx_p);
        return;
    }

	ResourceId L2sysLocalObjectManager::sendL2sysShowMacCommandToL2sysDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        trace (TRACE_LEVEL_DEBUG, "sendL2sysShowMacCommandToL2sysDaemon: entered");
        SI32 mappedId = DcmToolKit::getLocalMappedId();

        if (mappedId == -1)
            mappedId = 0;

        L2sysShowRbridgeMacMessage *msg_p = dynamic_cast<L2sysShowRbridgeMacMessage*> (seq_ctx_p->getPPrismMessage ());    

        if (msg_p == NULL) {
            trace (TRACE_LEVEL_INFO, "sendL2sysShowMacCommandToL2sysDaemon: getPPrismMessage returned NULL; returning.....");
            return (WAVE_MESSAGE_ERROR);
        }

        L2sysShowMessage *m = new L2sysShowMessage(msg_p->getIfName(), msg_p->getIfName(), "0", 0, msg_p->getType(), msg_p->getCmdCode(), msg_p->getForRbridgeId(), 0, 0, 0, 0);
        trace (TRACE_LEVEL_DEBUG, "sendL2sysShowMacCommandToL2sysDaemon: send L2sysShowMessage to WAVE client L2SYS");
        ResourceId status = sendSynchronouslyToWaveClient("l2sys", m);

        if (WAVE_MESSAGE_SUCCESS != status)
        {
            trace(TRACE_LEVEL_DEBUG, "sendL2sysShowMacCommandToL2sysDaemon: failed with status " + status);
            delete m;
            return status;
        }

		msg_p->copyAllBuffers(*m);
		 //bpatel: To avoid memory leak
        if (m){
            delete m;
        }
		return status;
	}

    void L2sysLocalObjectManager::L2sysShowMacSendToClusterCallback(WaveSendToClusterContext *ctx_p)
	{
        WaveNs::trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::L2sysShowMacSendToClusterCallback: Entering..");
        PrismLinearSequencerContext               *seq_ctx_p = NULL;
        L2sysShowRbridgeMacMessage                *req_msg_p = NULL;
        L2sysShowRbridgeMacMessage                *resp_msg_p = NULL;
        L2sysShowRbridgeMacMessage                *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        req_msg_p = dynamic_cast<L2sysShowRbridgeMacMessage *> (seq_ctx_p->getPPrismMessage());

        resp_msg_p =  dynamic_cast<L2sysShowRbridgeMacMessage *> (ctx_p->getPPrismMessageForPhase1());

        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::"
                "L2sysShowMacSendToClusterCallback  Response processing: Num resp:" + locations.size());

        for (i = 0; i < locations.size(); i++) {
            if (i == 0) {
                trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::"
                        "L2sysShowMacSendToClusterCallback locations:" + locations[i]);
            } else {
                trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::"
                        "L2sysShowMacSendToClusterCallback :" + locations[i]);
            }
            
        }

        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<L2sysShowRbridgeMacMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));

            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } 
            else 
            {
                trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::"
                        "L2sysShowMacSendToClusterCallback  Response processing: The location: " + locations[i]);
                trace (TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::"
                        "L2sysShowMacSendToClusterCallback returned error:" + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }

        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

	}

	void  L2sysLocalObjectManager::L2sysLocalShowPortSecurityMessageHandler (L2sysLocalShowPortSecurityMessage *pL2sysLocalShowPortSecurityMessage) 
  	{
		trace(TRACE_LEVEL_INFO, "Entering L2sysLocalObjectManager::L2sysLocalShowPortSecurityMessageHandler");
	  	PrismSynchronousLinearSequencerStep sequencerSteps[] =
	  	{
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::showPortSecurityMessageStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
		  	reinterpret_cast<PrismSynchronousLinearSequencerStep>(&L2sysLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
	  	};

	  	PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pL2sysLocalShowPortSecurityMessage, this,
			  																						sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	  	pPrismSynchronousLinearSequencerContext->execute ();
  	}
  
	ResourceId L2sysLocalObjectManager::showPortSecurityMessageStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
  	{
	  	ResourceId status = WAVE_MESSAGE_SUCCESS;
		UI32 cmdCode;
		l2sys_show_mac_output_msg_t *lastMacRecord = NULL;

	  	trace(TRACE_LEVEL_INFO, "L2sysLocalObjectManager::showPortSecurityMessageStep Local message received");

	  	L2sysLocalShowPortSecurityMessage *pL2sysLocalShowPortSecurityMessage =
		  dynamic_cast<L2sysLocalShowPortSecurityMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

	  	if (pL2sysLocalShowPortSecurityMessage == NULL) {
	      	trace (TRACE_LEVEL_INFO, "showPortSecurityMessageStep: getPPrismMessage returned NULL; returning.....");
	      	return (WAVE_MESSAGE_ERROR);
	  	}

		cmdCode = pL2sysLocalShowPortSecurityMessage->getCmdCode();

	  	L2sysPortSecShowMessage *pMsg = new L2sysPortSecShowMessage();

		if (cmdCode == L2SYSSHOW_PORT_SECURITY_INTERFACE || cmdCode == L2SYSSHOW_PORT_SECURITY_OUI) {
			pMsg->setIfName(pL2sysLocalShowPortSecurityMessage->getIfName());
			pMsg->setIfType(pL2sysLocalShowPortSecurityMessage->getIfType());
		}

	  	lastMacRecord = (l2sys_show_mac_output_msg_t *)malloc(sizeof(l2sys_show_mac_output_msg_t));
	  	memset(lastMacRecord, 0, sizeof(l2sys_show_mac_output_msg_t));

	  	pMsg->setCmdCode(cmdCode);
	  	pMsg->setIsFirstRequest(pL2sysLocalShowPortSecurityMessage->getIsFirstRequest());
	  	pL2sysLocalShowPortSecurityMessage->getLastPortSecurityRecord(lastMacRecord);
	  	pMsg->setLastPortSecurityAddressRecord(lastMacRecord);

	  	trace(TRACE_LEVEL_DEBUG, "L2sysLocalObjectManager::showPortSecurityMessageStep sending synchronously to l2sys client");
	  	status = sendSynchronouslyToWaveClient("l2sys",pMsg);
	  	free(lastMacRecord);

	  	if (status == WAVE_MESSAGE_SUCCESS && pMsg->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
		  	status = pMsg->getCompletionStatus();
		  	delete pMsg;
		  	return status;
	  	}

	  	int ClientStatus = pMsg->getClientStatus();
	  	status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(ClientStatus);
	  	if (status != WAVE_MESSAGE_SUCCESS) {
		  	trace(TRACE_LEVEL_ERROR, "Error From l2sys client");
		  	//bpatel: To avoid memory leak
		  	delete pMsg;
		  	return status;
	  	}

	  	pL2sysLocalShowPortSecurityMessage->copyAllBuffers(*pMsg);	

	  	delete pMsg;
	  	return status;
  	} 
}

