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
 *   Author : skrastog                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/PortSecurityGlobalConfWorker.h"
#include "Nsm/Global/NsmPortSecurityGlobalConfigMessage.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "Nsm/Local/PortSecOuiLocalManagedObject.h"
#include "Nsm/Local/PortSecAllowedMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecStickyMacsLocalManagedObject.h"
#include "Nsm/Global/NsmGlobalTypes.h"
#include "Nsm/Global/PoIntfManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "ClientInterface/L2sys/L2sysUtils.h"
#include "ClientInterface/Nsm/PortSecurityClientMessage.h"
#include "Nsm/Global/NsmEventPortSecGlobalMessage.h"
#include "DcmCStatus.h"
//#include <set>

namespace DcmNs
{

    PortSecurityGlobalConfWorker::PortSecurityGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager)
        : WaveWorker  (pNsmGlobalObjectManager)
    {
        addOperationMap (NSMPORTSECURITYGLOBALCONFIGMSG, reinterpret_cast<PrismMessageHandler> (&PortSecurityGlobalConfWorker::NsmPortSecurityGlobalConfigMessageHandler));
		addOperationMap (NSMPORTSECURITYPOSTICKYMACMSG, reinterpret_cast<PrismMessageHandler> (&PortSecurityGlobalConfWorker::NsmEventPortSecGlobalMessageHandler));
    }

    PortSecurityGlobalConfWorker::~PortSecurityGlobalConfWorker ()
    {
    }

    PrismMessage  *PortSecurityGlobalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMPORTSECURITYGLOBALCONFIGMSG :
                pPrismMessage = new NsmPortSecurityGlobalConfigMessage ();
                break;
			case NSMPORTSECURITYPOSTICKYMACMSG:
				pPrismMessage = new NsmEventPortSecGlobalMessage();
				break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *PortSecurityGlobalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((PortSecurityLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecurityLocalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PortSecAllowedMacsLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecAllowedMacsLocalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((PortSecOuiLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecOuiLocalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
		else if ((PortSecStickyMacsLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "PortSecurityGlobalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

	void  PortSecurityGlobalConfWorker::NsmEventPortSecGlobalMessageHandler(NsmEventPortSecGlobalMessage  *pMsg)
    {
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::portSecurityStickyMacMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PortSecurityContext *pPortSecurityContext = new PortSecurityContext(pMsg, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPortSecurityContext->execute();
    }

	ResourceId PortSecurityGlobalConfWorker::portSecurityStickyMacMOStep(PortSecurityContext *pPortSecurityContext)
    {
        trace(TRACE_LEVEL_INFO, "PortSecurityGlobalConfWorker::portSecurityStickyMacMOStep Entring...");

        NsmEventPortSecGlobalMessage *pMsg = dynamic_cast<NsmEventPortSecGlobalMessage*>(pPortSecurityContext->getPPrismMessage ());
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

		for (i = 0; i < numOfRecords; i++) {
			PortSecurityLocalManagedObject *pPortSecMO = NULL;
			PortSecStickyMacsLocalManagedObject *pStickyMacMo = NULL;
            PortSecurityStickyMacRecord_t macRecord = data->mac_records[i];
			InterfaceType ifType = (InterfaceType)(macRecord.if_type);

			/* If this MAC record is not for PO interface, ignore it. */
			if (ifType != IF_TYPE_PO)
				continue;

			UI32 poId = atoi(macRecord.if_name);
			string ifName = macRecord.if_name;

        	WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
        	syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId, "id"));
        	vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

			PoIntfManagedObject *pPoIntfMO = NULL;

        	if (NULL != pResults) {
        		pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);
            	UI32 numberOfResults = pResults->size ();
            	if (0 == numberOfResults) {
                	status = WAVE_MESSAGE_ERROR;
            	} else if (1 == numberOfResults) {
                	pPoIntfMO = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
                	status = WAVE_MESSAGE_SUCCESS;
            	} else {
                	status = WAVE_MESSAGE_ERROR;
            	}
        	}

        	if (pPoIntfMO == NULL) {
            	NSM_PLUG_DBG_ERR_S( string("Interface MO is not found ..:") + poId);
            	pResults->clear();
            	delete pResults;
            	return (WAVE_MESSAGE_ERROR);
        	}

        	pPortSecMO = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfMO);

            if (pPortSecMO == NULL) {
                trace(TRACE_LEVEL_INFO, string("Port security is not enabled for interface: ") + poId);
                pResults->clear();
                delete pResults;
                continue;
            } else {
                trace(TRACE_LEVEL_INFO, string("Port security MO is found for interface: ") + poId);
            }

            pStickyMacMo = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));

			string mac = macRecord.mac;

            pStickyMacMo->setStickyMac(mac);
            pStickyMacMo->setVlanId(macRecord.vlan);
            pStickyMacMo->setIfName(ifName);

			pStickyMacMo->setOwnerManagedObjectId(pPortSecMO->getObjectId());
            addToComposition(PortSecurityLocalManagedObject::getClassName(), PortSecStickyMacsLocalManagedObject::getClassName(), "stickyMacs",
                             pPortSecMO->getObjectId(), pStickyMacMo->getObjectId());


        	pResults->clear();
        	delete pResults;
        }

        return status;
    }

    void  PortSecurityGlobalConfWorker::NsmPortSecurityGlobalConfigMessageHandler( NsmPortSecurityGlobalConfigMessage *pNsmPortSecurityGlobalConfigMessage)
    {
// Validations and Queries should go here
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {

			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::portSecurityValidationStep),
			 reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::portSecurityMessageStep),

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::portSecurityMessageMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&PortSecurityGlobalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

		PortSecurityContext *pPortSecurityContext = new PortSecurityContext(pNsmPortSecurityGlobalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

	    pPortSecurityContext->execute();
    }

	ResourceId PortSecurityGlobalConfWorker::portSecurityValidationStep (PortSecurityContext *pPortSecurityContext)
	{
		PoIntfManagedObject *pPoIntfManagedObject = NULL;
		NsmPortSecurityGlobalConfigMessage *pNsmPortSecurityGlobalConfigMessage =
			dynamic_cast<NsmPortSecurityGlobalConfigMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace(TRACE_LEVEL_DEBUG, "PortSecurityGlobalConfWorker::portSecurityValidationStep Entring...");

		string ifName = pNsmPortSecurityGlobalConfigMessage->getIfName();

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeString(&ifName, "ifname"));

		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

		if (NULL != pResults) {
			pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);
			UI32 numberOfResults = pResults->size();
			if (0 == numberOfResults) {
				status = WAVE_MESSAGE_ERROR;
			}else if (1 == numberOfResults) {
				pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		if (pPoIntfManagedObject == NULL) {
			NSM_PLUG_DBG_ERR_S( string("Port-channel Interface MO not found ..:") + ifName);
			pResults->clear();
			delete pResults;
			return WAVE_MESSAGE_ERROR;
		}

		UI32 opCode = pNsmPortSecurityGlobalConfigMessage->getOpCode();

		if (opCode == PORT_SECURITY_CREATE) {
			if (pPoIntfManagedObject->getSwModeFlag() == false) {
				trace(TRACE_LEVEL_ERROR, string("Interface: ") + ifName + string(" is not in L2 mode."));
				status =  WRC_NSM_ERR_INTF_NOT_MODE_L2;
			}
		} else {
			if((pPoIntfManagedObject->getPortSecConfig()).operator->() == NULL ){
				status = WRC_NSM_ERR_PORT_SECURITY_NOT_ENABLED;
			}
		}

		PortSecurityLocalManagedObject *pPortSecMo = (pPoIntfManagedObject->getPortSecConfig()).operator->();

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
				PortSecOuiLocalManagedObject *pOui = getOuiObject(pPortSecMo, pNsmPortSecurityGlobalConfigMessage->getOui());

                if (pOui != NULL) {
                    trace(TRACE_LEVEL_ERROR, string("PortSecurityLocalConfWorker::portSecurityValidationStep OUI: ") + pNsmPortSecurityGlobalConfigMessage->getOui() + string(" is already configured"));
                    status = WRC_NSM_ERR_OUI_ALREADY_CONFIGURED;
                }
			}
        }

		trace(TRACE_LEVEL_INFO, "PortSecurityGlobalConfWorker::portSecurityValidationStep Completed...");
		pResults->clear();
		delete pResults;
		return status;
	}

	ResourceId PortSecurityGlobalConfWorker::portSecurityMessageStep(PortSecurityContext *pPortSecurityContext)
	{
		NsmPortSecurityGlobalConfigMessage *pNsmPortSecurityGlobalConfigMessage =
			dynamic_cast<NsmPortSecurityGlobalConfigMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		trace(TRACE_LEVEL_INFO, "PortSecurityGlobalConfWorker::portSecurityMessageStep Entring...");

		string ifName = pNsmPortSecurityGlobalConfigMessage->getIfName();
		UI32 opCode = pNsmPortSecurityGlobalConfigMessage->getOpCode();

		trace(TRACE_LEVEL_INFO, string("PortSecurityGlobalConfWorker::portSecurityMessageStep ifName: ") + ifName + string(" opCode: ") + opCode);

		PortSecurityClientMessage *m = new PortSecurityClientMessage();
		m->setIfName(ifName);
		m->setIfType(IF_TYPE_PO);
		m->setNumAllowedMacs(pNsmPortSecurityGlobalConfigMessage->getNumAllowedMacs());
		m->setOpCode(opCode);
		m->setAction(pNsmPortSecurityGlobalConfigMessage->getAction());
		m->setAllowedMac(pNsmPortSecurityGlobalConfigMessage->getMac());
		m->setOui(pNsmPortSecurityGlobalConfigMessage->getOui());
		m->setVlanId(pNsmPortSecurityGlobalConfigMessage->getVlanId());
		m->setSticky(pNsmPortSecurityGlobalConfigMessage->getSticky());
		m->setShutdownTime(pNsmPortSecurityGlobalConfigMessage->getShutdownTime());

		m->setCompletionStatus(WAVE_MESSAGE_SUCCESS);
		status = sendSynchronouslyToWaveClient ("l2sys", m);

		trace(TRACE_LEVEL_INFO, string("PortSecurityGlobalConfWorker::portSecurityMessageStep : Send message to l2sys status: ") + status);

		if(status == WAVE_MESSAGE_SUCCESS)
		{
        	if((status = m->getCompletionStatus()) != WAVE_MESSAGE_SUCCESS)
        	{
            	trace(TRACE_LEVEL_ERROR, string("PortSecurityGlobalConfWorker::portSecurityMessageStep : Completion status=") + status);
        	}
        	else
        	{
            	status = L2sysUtils::MapBackEndReturnCodeToDcmdReturnCode(m->getClientStatus());
            	trace(TRACE_LEVEL_DEBUG, string("PortSecurityGlobalConfWorker::portSecurityMessageStep : Client status=") + status);
        	}
    	} else {
            trace(TRACE_LEVEL_INFO, string("PortSecurityGlobalConfWorker::portSecurityMessageStep : Send message to l2sys is failed, status is: ") + status);
        }

		pNsmPortSecurityGlobalConfigMessage->setCompletionStatus(status);

#if 0
		if (opCode == PORT_SECURITY_STICKY) {
			UI32 size;
            void *data = NULL;

            m->getStickyMacRecordsOutputBuffer(STICKY_MAC_CREATE_BULK, size, data);
            if (data != NULL) {
                /*
                 * Get the buffer from client message and add it to the message which comes to plugin.
                 */
                pNsmPortSecurityGlobalConfigMessage->addBuffer(STICKY_MAC_CREATE_BULK, size, (unsigned char *)data, false);
            }
		}
#endif

		if (m)
			delete m;

		return status;
	}

	PortSecOuiLocalManagedObject *PortSecurityGlobalConfWorker::getOuiObject(PortSecurityLocalManagedObject *portSecMO, const string &oui)
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

	PortSecAllowedMacsLocalManagedObject *PortSecurityGlobalConfWorker::getAllowedMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId)
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

	PortSecurityLocalManagedObject *PortSecurityGlobalConfWorker::getPortSecurityLocalMO(PoIntfManagedObject *intfMO)
	{
		WaveManagedObjectPointer<PortSecurityLocalManagedObject> portSecCfg;

		portSecCfg = intfMO->getPortSecConfig();

		return portSecCfg.operator->();
	}

	PortSecStickyMacsLocalManagedObject *PortSecurityGlobalConfWorker::getStickyMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId)
    {
        vector<WaveManagedObjectPointer<PortSecStickyMacsLocalManagedObject> > stickyMacs;
        PortSecStickyMacsLocalManagedObject *pStickyMac = NULL;
        UI32 size, i;

        stickyMacs = portSecMO->getStickyMacs();
        size = stickyMacs.size();

        for (i = 0; i < size; i++) {
            pStickyMac = stickyMacs[i].operator->();
            if ((pStickyMac->getVlanId() == vlanId) && (mac.compare(pStickyMac->getStickyMac()) == 0))
                break;
        }

        return pStickyMac;
    }

	/* Create sticky MAC MOs out of data received from backend on execution of command "switchport port-security sticky" */
    void PortSecurityGlobalConfWorker::createStickyMacMOs(NsmPortSecurityGlobalConfigMessage *pMsg, PortSecurityLocalManagedObject *portSecMo)
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

	ResourceId PortSecurityGlobalConfWorker::portSecurityMessageMOStep(PortSecurityContext *pPortSecurityContext)
	{

		NsmPortSecurityGlobalConfigMessage *pNsmPortSecurityGlobalConfigMessage =
			dynamic_cast<NsmPortSecurityGlobalConfigMessage*>(pPortSecurityContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;

		UI32 poId = atoi((pNsmPortSecurityGlobalConfigMessage->getIfName()).c_str());

		WaveManagedObjectSynchronousQueryContext syncQueryCtxt(PoIntfManagedObject::getClassName());
		syncQueryCtxt.addAndAttribute (new AttributeUI32(&poId, "id"));
		vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
		PoIntfManagedObject *pPoIntfManagedObject = NULL;

		if (NULL != pResults) {
			pPortSecurityContext->addManagedObjectsForGarbageCollection(*pResults);
			UI32 numberOfResults = pResults->size ();
			if (0 == numberOfResults) {
				status = WAVE_MESSAGE_ERROR;
			}else if (1 == numberOfResults) {
				 pPoIntfManagedObject = dynamic_cast<PoIntfManagedObject *>((*pResults)[0]);
				status = WAVE_MESSAGE_SUCCESS;
			} else {
				status = WAVE_MESSAGE_ERROR;
			}
		}

		if (pPoIntfManagedObject == NULL) {
			NSM_PLUG_DBG_ERR_S( string("Port-channel interface MO is not found ..:") + poId);
			pResults->clear();
			delete pResults;
			return (WAVE_MESSAGE_ERROR);
		}

		NSM_PLUG_INFO_S( string ("Updating port-channel Interface MO : ") + poId + string("Cmdcode :") + pNsmPortSecurityGlobalConfigMessage->getOpCode());


		PortSecurityLocalManagedObject *pNewPortSecurityLocalManagedObject = NULL;
		PortSecAllowedMacsLocalManagedObject *pPortSecAllowedMacsLocalManagedObject = NULL;
		PortSecStickyMacsLocalManagedObject *pStickyMacMo = NULL;
		PortSecOuiLocalManagedObject *pPortSecOuiLocalManagedObject = NULL;

		switch(pNsmPortSecurityGlobalConfigMessage->getOpCode())
		{
			case PORT_SECURITY_CREATE:
				pNewPortSecurityLocalManagedObject = new PortSecurityLocalManagedObject ( dynamic_cast<NsmGlobalObjectManager *>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject->setEnabled(true);
				pNewPortSecurityLocalManagedObject->setIfName(pNsmPortSecurityGlobalConfigMessage->getIfName());
				pNewPortSecurityLocalManagedObject->setIfType(pNsmPortSecurityGlobalConfigMessage->getIfType());
				pPoIntfManagedObject->setPortSecConfig(pNewPortSecurityLocalManagedObject);
				pPoIntfManagedObject->addAttributeToBeUpdated("portSec");
				updateWaveManagedObject(pPoIntfManagedObject);
				break;

			case PORT_SECURITY_MAC_MAX:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					pNewPortSecurityLocalManagedObject->setNumAllowedMacs(pNsmPortSecurityGlobalConfigMessage->getNumAllowedMacs());
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("num_allowed_macs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}  else {
                    status = WAVE_MESSAGE_ERROR;
                }
				break;

			case PORT_SECURITY_ACTION:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					pNewPortSecurityLocalManagedObject->setAction(pNsmPortSecurityGlobalConfigMessage->getAction());
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("newAction");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}  else {
                    status = WAVE_MESSAGE_ERROR;
                }
				break;

			case PORT_SECURITY_MAC:
				pPortSecAllowedMacsLocalManagedObject = new PortSecAllowedMacsLocalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					pPortSecAllowedMacsLocalManagedObject->setAllowedMac(pNsmPortSecurityGlobalConfigMessage->getMac());
					pPortSecAllowedMacsLocalManagedObject->setVlanId(pNsmPortSecurityGlobalConfigMessage->getVlanId());
					pPortSecAllowedMacsLocalManagedObject->setIfName(pNsmPortSecurityGlobalConfigMessage->getIfName());
					pNewPortSecurityLocalManagedObject->addAllowedMac(pPortSecAllowedMacsLocalManagedObject);
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("allowed_macs");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}  else {
                    status = WAVE_MESSAGE_ERROR;
                }
				break;

			case PORT_SECURITY_MAC_REMOVE:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					pPortSecAllowedMacsLocalManagedObject = PortSecurityGlobalConfWorker::getAllowedMacObject(pNewPortSecurityLocalManagedObject,
																pNsmPortSecurityGlobalConfigMessage->getMac(), pNsmPortSecurityGlobalConfigMessage->getVlanId());
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
				break;

			case PORT_SECURITY_STICKY_MAC:
                pStickyMacMo = new PortSecStickyMacsLocalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
                pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

                if (pNewPortSecurityLocalManagedObject != NULL) {
                    pStickyMacMo->setStickyMac(pNsmPortSecurityGlobalConfigMessage->getMac());
                    pStickyMacMo->setVlanId(pNsmPortSecurityGlobalConfigMessage->getVlanId());
                    pStickyMacMo->setIfName(pNsmPortSecurityGlobalConfigMessage->getIfName());
                    pNewPortSecurityLocalManagedObject->addStickyMac(pStickyMacMo);
                    pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("stickyMacs");
                    updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
                } else {
                    status = WAVE_MESSAGE_ERROR;
                }
                break;

            case PORT_SECURITY_STICKY_MAC_REMOVE:
                pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

                if (pNewPortSecurityLocalManagedObject != NULL) {
                    pStickyMacMo = PortSecurityGlobalConfWorker::getStickyMacObject(pNewPortSecurityLocalManagedObject, pNsmPortSecurityGlobalConfigMessage->getMac(),
                                                                pNsmPortSecurityGlobalConfigMessage->getVlanId());
                    if (pStickyMacMo) {
                        deleteFromComposition(PortSecurityLocalManagedObject::getClassName(), PortSecStickyMacsLocalManagedObject::getClassName(),
                                "stickyMacs", pNewPortSecurityLocalManagedObject->getObjectId(), pStickyMacMo->getObjectId());
                    } else {
                        /* TODO, return and display error message */
                        status = WAVE_MESSAGE_ERROR;
                    }
                    pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("stickyMacs");
                    updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
                }
                break;

			case PORT_SECURITY_OUI:
				pPortSecOuiLocalManagedObject = new PortSecOuiLocalManagedObject(dynamic_cast<NsmGlobalObjectManager*>(getPWaveObjectManager()));
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					pPortSecOuiLocalManagedObject->setOui(pNsmPortSecurityGlobalConfigMessage->getOui());
					pPortSecOuiLocalManagedObject->setIfName(pNsmPortSecurityGlobalConfigMessage->getIfName());
					pNewPortSecurityLocalManagedObject->addOui(pPortSecOuiLocalManagedObject);
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("ouis");
					pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("numOuis");
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}  else {
                    status = WAVE_MESSAGE_ERROR;
                }
				break;

			case PORT_SECURITY_OUI_REMOVE:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);

				if (pNewPortSecurityLocalManagedObject != NULL) {
					pPortSecOuiLocalManagedObject = PortSecurityGlobalConfWorker::getOuiObject(pNewPortSecurityLocalManagedObject,
														pNsmPortSecurityGlobalConfigMessage->getOui());
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
				break;

			case PORT_SECURITY_STICKY:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					trace(TRACE_LEVEL_INFO, "PortSecurityGlobalConfWorker::portSecurityMessageMOStep : Retrieved port-security MO");
					pNewPortSecurityLocalManagedObject->setSticky(true);
					updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
				}
				break;

            case PORT_SECURITY_SHUTDOWN_TIME:
                pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
                if (pNewPortSecurityLocalManagedObject != NULL) {
                    pNewPortSecurityLocalManagedObject->setShutdownTime(pNsmPortSecurityGlobalConfigMessage->getShutdownTime());
                    pNewPortSecurityLocalManagedObject->addAttributeToBeUpdated("shutdownTime");
                    updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
                }
                break;

			case PORT_SECURITY_STICKY_REMOVE:
                pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
                if (pNewPortSecurityLocalManagedObject != NULL) {
                    set<string> compositionSet;
                    compositionSet.insert("stickyMacs");
                    deleteManagedObjectCompositions(pNewPortSecurityLocalManagedObject->getObjectId(), compositionSet);
					pNewPortSecurityLocalManagedObject->setSticky(false);
                    updateWaveManagedObject(pNewPortSecurityLocalManagedObject);
                }
                break;

			case PORT_SECURITY_REMOVE:
				pNewPortSecurityLocalManagedObject = PortSecurityGlobalConfWorker::getPortSecurityLocalMO(pPoIntfManagedObject);
				if (pNewPortSecurityLocalManagedObject != NULL) {
					set<string> compositionSet;
                    compositionSet.insert("portSec");
					deleteManagedObjectCompositions(pPoIntfManagedObject->getObjectId(), compositionSet);
					pPoIntfManagedObject->addAttributeToBeUpdated("portSec");
					updateWaveManagedObject(pPoIntfManagedObject);
				} else {
					trace(TRACE_LEVEL_INFO, string("Port security configuration is not present for the interface: ") + poId);
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
