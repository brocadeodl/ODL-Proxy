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
 *   Author : sumank                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PortSecurityLocalConfWorker.h"
#include "Nsm/Local/NsmPortSecurityConfigMsgMessage.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "Nsm/Local/PortSecOuiLocalManagedObject.h"
#include "Nsm/Local/PortSecStickyMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecAllowedMacsLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/L2sys/L2sysUtils.h"
#include "ClientInterface/Nsm/PortSecurityClientMessage.h"
#include "DcmCStatus.h"
#include <set>

/* Include NsmUtils.h and ClientMessage.h*/

namespace DcmNs
{

    PortSecurityLocalConfWorker::PortSecurityLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        PortSecurityLocalManagedObject    PortSecurityLocalManagedObject    (pNsmLocalObjectManager);
        PortSecAllowedMacsLocalManagedObject    PortSecAllowedMacsLocalManagedObject    (pNsmLocalObjectManager);
        PortSecOuiLocalManagedObject    PortSecOuiLocalManagedObject    (pNsmLocalObjectManager);
		PortSecStickyMacsLocalManagedObject PortSecStickyMacsLocalManagedObject (pNsmLocalObjectManager);
        PortSecurityLocalManagedObject.setupOrm ();
        addManagedClass (PortSecurityLocalManagedObject::getClassName (), this);
        PortSecAllowedMacsLocalManagedObject.setupOrm ();
        addManagedClass (PortSecAllowedMacsLocalManagedObject::getClassName (), this);
        PortSecOuiLocalManagedObject.setupOrm ();
        addManagedClass (PortSecOuiLocalManagedObject::getClassName (), this);
		PortSecStickyMacsLocalManagedObject.setupOrm ();
		addManagedClass (PortSecStickyMacsLocalManagedObject::getClassName (), this);
        addOperationMap (NSMPORTSECURITYCONFIGMSG, reinterpret_cast<PrismMessageHandler> (&PortSecurityLocalConfWorker::NsmPortSecurityConfigMsgMessageHandler));
        addOperationMap (NSMPORTSECURITYSTICKYMACMSG, reinterpret_cast<PrismMessageHandler> (&PortSecurityLocalConfWorker::NsmEventPortSecLocalMessageHandler));
    }

    PortSecurityLocalConfWorker::~PortSecurityLocalConfWorker ()
    {
    }

    PrismMessage  *PortSecurityLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMPORTSECURITYCONFIGMSG :
                pPrismMessage = new NsmPortSecurityConfigMsgMessage ();
                break;
			case NSMPORTSECURITYSTICKYMACMSG :
				pPrismMessage = new NsmEventPortSecLocalMessage();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PortSecurityLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PortSecurityLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecurityLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PortSecAllowedMacsLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecAllowedMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PortSecOuiLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecOuiLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PortSecStickyMacsLocalManagedObject::getClassName ()) == managedClassName)
		{
			pWaveManagedObject = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
		}
        else
        {
            trace (TRACE_LEVEL_FATAL, "PortSecurityLocalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  PortSecurityLocalConfWorker::NsmEventPortSecLocalMessageHandler( NsmEventPortSecLocalMessage *pNsmEventPortSecLocalMessage)
    {
// Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::portSecurityStickyMacMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

		PortSecurityContext *pPortSecurityContext = new PortSecurityContext(pNsmEventPortSecLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	    pPortSecurityContext->execute();
    }

	ResourceId PortSecurityLocalConfWorker::portSecurityStickyMacMOStep(PortSecurityContext *pPortSecurityContext)
	{
		trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityStickyMacMOStep Entring...");

        NsmEventPortSecLocalMessage *pMsg = dynamic_cast<NsmEventPortSecLocalMessage*>(pPortSecurityContext->getPPrismMessage ());
		PortSecurityLocalManagedObject *pPortSecMO = NULL;
		PortSecStickyMacsLocalManagedObject *pStickyMacMo = NULL;
        ResourceId status = WAVE_MESSAGE_SUCCESS;
		PortSecurityStickyOutputMsg_t *data = NULL;
		UI32 size = 0, i;
		UI32 numOfRecords = 0;

		if (pMsg == NULL)
			return WAVE_MESSAGE_ERROR;

		data = (PortSecurityStickyOutputMsg_t *)pMsg->findBuffer(STICKY_MAC_CREATE_BULK, size);

		if (data == NULL)
			return WAVE_MESSAGE_ERROR;

		numOfRecords = data->num_records;

		trace(TRACE_LEVEL_INFO, string("Number of records in received buffer is: ") + numOfRecords + string("Records type: ") + data->records_type);

		for (i = 0; i < numOfRecords; i++) {
			PortSecurityStickyMacRecord_t macRecord = data->mac_records[i];
			InterfaceType ifType = (InterfaceType)(macRecord.if_type);

			trace(TRACE_LEVEL_INFO, string("The record data is: MAC: ") + macRecord.mac + string(" Interface name: ") + macRecord.if_name + string(" vlan: ") + macRecord.vlan + string(" ifType: ") + macRecord.if_type);

			/* If this record is not for a physical interface, ignore it */
			if (ifType != IF_TYPE_GI && ifType != IF_TYPE_TE && ifType != IF_TYPE_FO)
				continue;

			string ifName = macRecord.if_name;

			WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifName"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        	pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);
        	PhyIntfLocalManagedObject *pPhyIntfMO = NULL;

        	if (NULL != pResults) {
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	} else if (1 == numberOfResults) {
                	pPhyIntfMO = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
                	status = WAVE_MESSAGE_SUCCESS;
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
        	}

        	if (pPhyIntfMO == NULL) {
            	NSM_PLUG_DBG_ERR_S( string("Interface MO is not found ..:") + ifName);
            	pResults->clear();
            	delete pResults;
            	return (WAVE_MESSAGE_ERROR);
        	}

			trace(TRACE_LEVEL_INFO, "Got the respective interface MO");

			pPortSecMO = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfMO);
			if (pPortSecMO == NULL) {
				trace(TRACE_LEVEL_INFO, string("Port security is not enabled for interface: ") + ifName);
				pResults->clear();
                delete pResults;
				continue;
			} else {
				trace(TRACE_LEVEL_INFO, string("Port security MO is found for interface: ") + ifName);
			}

			pStickyMacMo = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));

			string mac = macRecord.mac;

            pStickyMacMo->setStickyMac(mac);
            pStickyMacMo->setVlanId(macRecord.vlan);
            pStickyMacMo->setIfName(ifName);

			trace(TRACE_LEVEL_INFO, string("Just before invoking addStickyMac, data is - MAC: ") + pStickyMacMo->m_stickyMac + string(" Vlan: ") + pStickyMacMo->m_vlanId);
			//pPortSecMO->addStickyMac(pStickyMacMo);
			pStickyMacMo->setOwnerManagedObjectId(pPortSecMO->getObjectId());
			addToComposition(PortSecurityLocalManagedObject::getClassName(), PortSecStickyMacsLocalManagedObject::getClassName(), "stickyMacs",
							 pPortSecMO->getObjectId(), pStickyMacMo->getObjectId());
			trace(TRACE_LEVEL_INFO, "Just after completion addStickyMac");

			//pPortSecMO->addAttributeToBeUpdated("stickyMacs");
			//updateWaveManagedObject(pPortSecMO);

        	pResults->clear();
        	delete pResults;
		}

		/*trace(TRACE_LEVEL_INFO, "All added sticky MACs are:");
		vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > stickyMacs;
        PortSecStickyMacsLocalManagedObject *pStickyMac = NULL;

        stickyMacs = pPortSecMO->getStickyMacs();
        size = stickyMacs.size();

        for (i = 0; i < size; i++) {
            pStickyMac = stickyMacs[i].operator->();
			trace(TRACE_LEVEL_INFO, string("MAC: ") + pStickyMac->m_stickyMac + string(" Vlan:") + pStickyMac->m_vlanId);
        }*/

        return status;
	}

    void  PortSecurityLocalConfWorker::NsmPortSecurityConfigMsgMessageHandler( NsmPortSecurityConfigMsgMessage *pNsmPortSecurityConfigMsgMessage)
    {
// Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::portSecurityValidationStep),
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::portSecurityMessageStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::portSecurityMessageMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

		PortSecurityContext *pPortSecurityContext = new PortSecurityContext(pNsmPortSecurityConfigMsgMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	    pPortSecurityContext->execute();
    }

	ResourceId PortSecurityLocalConfWorker::portSecurityValidationStep (PortSecurityContext *pPortSecurityContext)
	{
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;
		NsmPortSecurityConfigMsgMessage *pNsmPortSecurityConfigMsgMessage =
			dynamic_cast<NsmPortSecurityConfigMsgMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityValidationStep Entring...");

		string ifName = pNsmPortSecurityConfigMsgMessage->getIfName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifname"));

		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size();
			if (0 == numberOfResults) {
				status = WAVE_MESSAGE_ERROR;
			}else if (1 == numberOfResults) {
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		if (pPhyIntfLocalManagedObject == NULL) {
			NSM_PLUG_DBG_ERR_S( string("UP_PHY_L Physical Interface MO not found ..:") + ifName);
			pResults->clear();
			delete pResults;
			return (WAVE_MESSAGE_ERROR);
		}

		UI32 opCode = pNsmPortSecurityConfigMsgMessage->getOpCode();

		if (opCode == PORT_SECURITY_CREATE) {
			if (pPhyIntfLocalManagedObject->getSwModeFlag() == false) {
				trace(TRACE_LEVEL_ERROR, string("Interface: ") + ifName + string(" is not in L2 mode."));
				status = WRC_NSM_ERR_INTF_NOT_MODE_L2;
			}
		} else {
			if((pPhyIntfLocalManagedObject->getPortSecConfig()).operator->() == NULL ){
				trace(TRACE_LEVEL_ERROR, string("Port security is not enabled at interface: ") + ifName);
				status = WRC_NSM_ERR_PORT_SECURITY_NOT_ENABLED;
			}
		}

		PortSecurityLocalManagedObject *pPortSecMo = (pPhyIntfLocalManagedObject->getPortSecConfig()).operator->();

		if (status == WAVE_MESSAGE_SUCCESS && opCode == PORT_SECURITY_STICKY_MAC) {
			if (pPortSecMo->getSticky() == false) {
				trace(TRACE_LEVEL_ERROR, string("Port security sticky is not enabled at interface: ") + ifName);
				status = WRC_NSM_ERR_PORT_SECURITY_STICKY_NOT_ENABLED;
			}
		}

		if (status == WAVE_MESSAGE_SUCCESS && opCode == PORT_SECURITY_OUI) {
			if (pPortSecMo->m_numOuis >= 4) {
				trace(TRACE_LEVEL_ERROR, string("Maximum number of OUIs (4) are already configured at this interface") + ifName);
				status = WRC_NSM_ERR_PORT_SECURITY_MAX_OUIS;
			} else {
				PortSecOuiLocalManagedObject *pOui = getOuiObject(pPortSecMo, pNsmPortSecurityConfigMsgMessage->getOui());

				if (pOui != NULL) {
					trace(TRACE_LEVEL_ERROR, string("PortSecurityLocalConfWorker::portSecurityValidationStep OUI: ") + pNsmPortSecurityConfigMsgMessage->getOui() + string(" is already configured"));
					status = WRC_NSM_ERR_OUI_ALREADY_CONFIGURED;
				}
			}
		}

		trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityValidationStep Completed...");
		pResults->clear();
		delete pResults;
		return status;
	}

	ResourceId PortSecurityLocalConfWorker::portSecurityMessageStep(PortSecurityContext *pPortSecurityContext)
	{
		NsmPortSecurityConfigMsgMessage *pNsmPortSecurityConfigMsgMessage =
			dynamic_cast<NsmPortSecurityConfigMsgMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
		string finalIfName;

		trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageStep Entring...");

		string ifName = pNsmPortSecurityConfigMsgMessage->getIfName();
		UI32 opCode = pNsmPortSecurityConfigMsgMessage->getOpCode();

		trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::portSecurityMessageStep ifName: ") + ifName + string(" opCode: ") + opCode);

        if (NsmUtils::isValidThreeTuple(ifName))
			NsmUtils::getTwoTupleIfName(ifName, finalIfName);
		else
			finalIfName = ifName;

		trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::portSecurityMessageStep interface name going to client message is: ") + finalIfName + string(" and interface type is: ") + pNsmPortSecurityConfigMsgMessage->getIfType());

		PortSecurityClientMessage *m = new PortSecurityClientMessage();
		m->setIfName(finalIfName);
		m->setIfType(pNsmPortSecurityConfigMsgMessage->getIfType());
		m->setNumAllowedMacs(pNsmPortSecurityConfigMsgMessage->getNumAllowedMacs());
		m->setOpCode(opCode);
		m->setAction(pNsmPortSecurityConfigMsgMessage->getAction());
		m->setAllowedMac(pNsmPortSecurityConfigMsgMessage->getMac());
		m->setOui(pNsmPortSecurityConfigMsgMessage->getOui());
		m->setVlanId(pNsmPortSecurityConfigMsgMessage->getVlanId());
		m->setSticky(pNsmPortSecurityConfigMsgMessage->getSticky());
		m->setShutdownTime(pNsmPortSecurityConfigMsgMessage->getShutdownTime());

		m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		status = sendSynchronouslyToWaveClient ("l2sys", m);

		trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::portSecurityMessageStep : Send message to l2sys status: ") + status);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
        	if((status = m->getCompletionStatus()) != WAVE_MESSAGE_SUCCESS)
        	{
            	trace(TRACE_LEVEL_ERROR, string("PortSecurityLocalConfWorker::portSecurityMessageStep : Completion status=") + status);
        	}
        	else
        	{
            	status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(m->getClientStatus());
            	trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::portSecurityMessageStep : Client status=") + status);
        	}
    	} else {
			trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::portSecurityMessageStep : Send message to l2sys is failed, status is: ") + status);
		}

		pNsmPortSecurityConfigMsgMessage->setCompletionStatus(status);

#if 0
		if (opCode == PORT_SECURITY_STICKY) {
			UI32 size;
			void *data = NULL;

			m->getStickyMacRecordsOutputBuffer(STICKY_MAC_CREATE_BULK, size, data);
			if (data != NULL) {
				/*
				 * Get the buffer from client message and add it to the message which comes to plugin.
				 */
				pNsmPortSecurityConfigMsgMessage->addBuffer(STICKY_MAC_CREATE_BULK, size, (unsigned char *)data, false);
			}
		}
#endif

    	if(m){
        	delete m;
    	}

		return status;
	}

	PortSecOuiLocalManagedObject *PortSecurityLocalConfWorker::getOuiObject(PortSecurityLocalManagedObject *portSecMO, const string &oui)
	{
		vector<WaveManagedObjectPointer<PortSecOuiLocalManagedObject> > ouis;
		PortSecOuiLocalManagedObject *pOui = NULL;
		UI32 size, i;

		ouis = portSecMO->getOuis();
		size = ouis.size();

		for (i = 0; i < size; i++) {
			pOui = ouis[i].operator->();
			if (oui.compare(pOui->getOui()) == 0)
				return pOui;
		}

		return NULL;
	}

	PortSecAllowedMacsLocalManagedObject *PortSecurityLocalConfWorker::getAllowedMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId)
	{
		vector<WaveManagedObjectPointer<PortSecAllowedMacsLocalManagedObject> > allowedMacs;
		PortSecAllowedMacsLocalManagedObject *pAllowedMac = NULL;
		UI32 size, i;

		allowedMacs = portSecMO->getAllowedMacs();
		size = allowedMacs.size();

		for (i = 0; i < size; i++) {
			pAllowedMac = allowedMacs[i].operator->();
			if ((pAllowedMac->getVlanId() == vlanId) && (mac.compare(pAllowedMac->getAllowedMac()) == 0))
				return pAllowedMac;
		}

		return NULL;
	}

	PortSecStickyMacsLocalManagedObject *PortSecurityLocalConfWorker::getStickyMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId)
	{
		vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > stickyMacs;
		PortSecStickyMacsLocalManagedObject *pStickyMac = NULL;
		UI32 size, i;

		trace(TRACE_LEVEL_INFO, string("PortSecurityLocalConfWorker::getStickyMacObject MAC: ") + mac + string(" Vlan: ") + vlanId);

		stickyMacs = portSecMO->getStickyMacs();
		size = stickyMacs.size();

		for (i = 0; i < size; i++) {
			pStickyMac = stickyMacs[i].operator->();
			if ((pStickyMac->getVlanId() == vlanId) && (mac.compare(pStickyMac->getStickyMac()) == 0))
				break;
		}

		return pStickyMac;
	}

	PortSecurityLocalManagedObject *PortSecurityLocalConfWorker::getPortSecurityLocalMO(PhyIntfLocalManagedObject *intfMO)
	{
		WaveManagedObjectPointer<PortSecurityLocalManagedObject> portSecCfg;

		portSecCfg = intfMO->getPortSecConfig();

		return portSecCfg.operator->();
	}

	/* Create sticky MAC MOs out of data received from backend on execution of command "switchport port-security sticky" */
	void PortSecurityLocalConfWorker::createStickyMacMOs(NsmPortSecurityConfigMsgMessage *pMsg, PortSecurityLocalManagedObject *portSecMo)
	{
		PortSecurityStickyOutputMsg_t *data = NULL;
		UI32 size = 0, i;
		UI32 numOfRecords = 0;

		if (pMsg == NULL)
			return;

		data = (PortSecurityStickyOutputMsg_t *)pMsg->findBuffer(STICKY_MAC_CREATE_BULK, size);

		if (data == NULL)
			return;

		numOfRecords = data->num_records;

		for (i = 0; i < numOfRecords; i++) {
			PortSecurityStickyMacRecord_t macRecord = data->mac_records[i];
			PortSecStickyMacsLocalManagedObject *pStickyMacMo = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));

            pStickyMacMo->setStickyMac(string(macRecord.mac));
            pStickyMacMo->setVlanId(macRecord.vlan);
            pStickyMacMo->setIfName(string(macRecord.if_name));
			portSecMo->addStickyMac(pStickyMacMo);
		}
	}

	ResourceId PortSecurityLocalConfWorker::portSecurityMessageMOStep(PortSecurityContext *pPortSecurityContext)
	{
		trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep Entring...");

		NsmPortSecurityConfigMsgMessage *pNsmPortSecurityConfigMsgMessage =
			dynamic_cast<NsmPortSecurityConfigMsgMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		string ifName = pNsmPortSecurityConfigMsgMessage->getIfName();
		//InterfaceType ifType = pNsmPortSecurityConfigMsgMessage->getIfType();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PhyIntfLocalManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName,"ifName"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);
		PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject = NULL;

		if (NULL != pResults) {
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				status = WAVE_MESSAGE_ERROR;
			}else if (1 == numberOfResults) {
				pPhyIntfLocalManagedObject = dynamic_cast<PhyIntfLocalManagedObject *>((*pResults)[0]);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		if (pPhyIntfLocalManagedObject == NULL) {
			NSM_PLUG_DBG_ERR_S( string("Interface MO is not found ..:") + ifName);
			pResults->clear();
			delete pResults;
			return (WAVE_MESSAGE_ERROR);
		}

		NSM_PLUG_INFO_S( string ("UP_PHY_L Updating Physical Interface MO : ") + ifName + string("Cmdcode :") + pNsmPortSecurityConfigMsgMessage->getOpCode());


		PortSecurityLocalManagedObject *pNewPortSecurityLocalManagedObject = NULL;
		PortSecAllowedMacsLocalManagedObject *pPortSecAllowedMacsLocalManagedObject = NULL;
		PortSecOuiLocalManagedObject *pPortSecOuiLocalManagedObject = NULL;
		PortSecStickyMacsLocalManagedObject *pStickyMacMo = NULL;

		switch(pNsmPortSecurityConfigMsgMessage->getOpCode())
		{
			case PORT_SECURITY_CREATE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security MO Create");
				pNewPortSecurityLocalManagedObject = new PortSecurityLocalManagedObject ( dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject->setEnabled(true);
				pNewPortSecurityLocalManagedObject->setIfName(pNsmPortSecurityConfigMsgMessage->getIfName());
				pNewPortSecurityLocalManagedObject->setIfType(pNsmPortSecurityConfigMsgMessage->getIfType());
				pPhyIntfLocalManagedObject->setPortSecConfig(pNewPortSecurityLocalManagedObject);
				trace(TRACE_LEVEL_INFO, "Before Intf MO update call");
				pPhyIntfLocalManagedObject->addAttributeToBeUpdated("portSec");
				updateWaveManagedObject(pPhyIntfLocalManagedObject);
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pNewPortSecurityLocalManagedObject);
				trace(TRACE_LEVEL_INFO, "After Intf MO update and garbage collection for port-security MO call");
				break;

			case PORT_SECURITY_MAC_MAX:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security max-allowed MACs set");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pNewPortSecurityLocalManagedObject->setNumAllowedMacs(pNsmPortSecurityConfigMsgMessage->getNumAllowedMacs());
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("num_allowed_macs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				} else {
					status = WAVE_MESSAGE_ERROR;
				}
				break;

			case PORT_SECURITY_ACTION:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security set action for violation");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pNewPortSecurityLocalManagedObject->setAction(pNsmPortSecurityConfigMsgMessage->getAction());
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("newAction");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				} else {
					status = WAVE_MESSAGE_ERROR;
				}
				break;

			case PORT_SECURITY_MAC:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security add secure mac");
				pPortSecAllowedMacsLocalManagedObject = new PortSecAllowedMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pPortSecAllowedMacsLocalManagedObject->setAllowedMac(pNsmPortSecurityConfigMsgMessage->getMac());
					pPortSecAllowedMacsLocalManagedObject->setVlanId(pNsmPortSecurityConfigMsgMessage->getVlanId());
					pPortSecAllowedMacsLocalManagedObject->setIfName(pNsmPortSecurityConfigMsgMessage->getIfName());
					pNewPortSecurityLocalManagedObject->addAllowedMac(pPortSecAllowedMacsLocalManagedObject);
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("allowed_macs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				} else {
					status = WAVE_MESSAGE_ERROR;
				}
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pPortSecAllowedMacsLocalManagedObject);
				break;

			case PORT_SECURITY_MAC_REMOVE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security remove a secure mac");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pPortSecAllowedMacsLocalManagedObject = PortSecurityLocalConfWorker::getAllowedMacObject(pNewPortSecurityLocalManagedObject, pNsmPortSecurityConfigMsgMessage->getMac(),
																pNsmPortSecurityConfigMsgMessage->getVlanId());
					if (pPortSecAllowedMacsLocalManagedObject) {
						deleteFromComposition(PortSecurityLocalManagedObject::getClassName(), PortSecAllowedMacsLocalManagedObject::getClassName(),
                        		"allowed_macs", pNewPortSecurityLocalManagedObject->getObjectId(), pPortSecAllowedMacsLocalManagedObject->getObjectId());
					} else {
						/* TODO, return and display error message */
						status = WAVE_MESSAGE_ERROR;
					}
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("allowed_macs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pPortSecAllowedMacsLocalManagedObject);
				break;

			case PORT_SECURITY_STICKY_MAC:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security add sticky mac");
				pStickyMacMo = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pStickyMacMo->setStickyMac(pNsmPortSecurityConfigMsgMessage->getMac());
					pStickyMacMo->setVlanId(pNsmPortSecurityConfigMsgMessage->getVlanId());
					pStickyMacMo->setIfName(pNsmPortSecurityConfigMsgMessage->getIfName());
					pNewPortSecurityLocalManagedObject->addStickyMac(pStickyMacMo);
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("stickyMacs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				} else {
					status = WAVE_MESSAGE_ERROR;
				}
				break;

			case PORT_SECURITY_STICKY_MAC_REMOVE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security remove a sticky mac");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pStickyMacMo = PortSecurityLocalConfWorker::getStickyMacObject(pNewPortSecurityLocalManagedObject, pNsmPortSecurityConfigMsgMessage->getMac(),
																pNsmPortSecurityConfigMsgMessage->getVlanId());
					if (pStickyMacMo) {
						deleteFromComposition(PortSecurityLocalManagedObject::getClassName(), PortSecStickyMacsLocalManagedObject::getClassName(),
                        		"stickyMacs", pNewPortSecurityLocalManagedObject->getObjectId(), pStickyMacMo->getObjectId());
					} else {
						trace(TRACE_LEVEL_ERROR, "Sticky MAC MO is not found in port-security object");
						status = WAVE_MESSAGE_ERROR;
					}
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("stickyMacs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pPortSecAllowedMacsLocalManagedObject);
				break;

			case PORT_SECURITY_OUI:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security add oui");
				pPortSecOuiLocalManagedObject = new PortSecOuiLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pPortSecOuiLocalManagedObject->setOui(pNsmPortSecurityConfigMsgMessage->getOui());
					pPortSecOuiLocalManagedObject->setIfName(pNsmPortSecurityConfigMsgMessage->getIfName());
					pNewPortSecurityLocalManagedObject->addOui(pPortSecOuiLocalManagedObject);
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("ouis");
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("numOuis");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pPortSecOuiLocalManagedObject);
				break;

			case PORT_SECURITY_OUI_REMOVE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security remove an oui");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pPortSecOuiLocalManagedObject = PortSecurityLocalConfWorker::getOuiObject(pNewPortSecurityLocalManagedObject, pNsmPortSecurityConfigMsgMessage->getOui());
					if (pPortSecOuiLocalManagedObject) {
						deleteFromComposition(PortSecurityLocalManagedObject::getClassName(), PortSecOuiLocalManagedObject::getClassName(),
                        		"ouis", pNewPortSecurityLocalManagedObject->getObjectId(), pPortSecOuiLocalManagedObject->getObjectId());
					} else {
						/* TODO, return and display error message */
						status = WAVE_MESSAGE_ERROR;
					}
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("ouis");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				} else {
					status = WAVE_MESSAGE_ERROR;
				}
				//pPortSecurityContext->addManagedObjectForGarbageCollection(pPortSecAllowedMacsLocalManagedObject);
				break;

			case PORT_SECURITY_STICKY:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security sticky command");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pNewPortSecurityLocalManagedObject->setSticky(true);
					//createStickyMacMOs(pNsmPortSecurityConfigMsgMessage, pNewPortSecurityLocalManagedObject);
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				break;

			case PORT_SECURITY_SHUTDOWN_TIME:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security shutdown command");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pNewPortSecurityLocalManagedObject->setShutdownTime(pNsmPortSecurityConfigMsgMessage->getShutdownTime());
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("shutdownTime");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				break;

			case PORT_SECURITY_STICKY_REMOVE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security sticky remove command");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					set<string> compositionSet;
					compositionSet.insert("stickyMacs");
					deleteManagedObjectCompositions(pNewPortSecurityLocalManagedObject->getObjectId(), compositionSet);
					pNewPortSecurityLocalManagedObject->setSticky(false);
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				break;

			case PORT_SECURITY_REMOVE:
				trace(TRACE_LEVEL_INFO, "PortSecurityLocalConfWorker::portSecurityMessageMOStep : For port-security MO Remove");
				pNewPortSecurityLocalManagedObject = PortSecurityLocalConfWorker::getPortSecurityLocalMO(pPhyIntfLocalManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					set<string> compositionSet;
					compositionSet.insert("portSec");
					deleteManagedObjectCompositions(pPhyIntfLocalManagedObject->getObjectId(), compositionSet);
					pPhyIntfLocalManagedObject->addAttributeToBeUpdated("portSec");
					updateWaveManagedObject(pPhyIntfLocalManagedObject);
				} else {
					trace(TRACE_LEVEL_DEBUG, string("Port security configuration is not present for the interface: ") + ifName);
					status = WAVE_MESSAGE_SUCCESS;
				}
				break;

			default:
				break;
		}

		pResults->clear();
		delete pResults;
		return status;
	}
}
