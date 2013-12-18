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
 *   Copyright (C) 20113 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "Nsm/Local/Dhcp/DhcpIntfLocalWorker.h"
#include "Nsm/Local/Dhcp/DhcpRelIntfLocalConfigMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelInterfaceLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "ClientInterface/Dhcp/DhcpRelIntfClientConfigMessage.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"
#include "ClientInterface/Dhcp/DhcpUtils.h"
#include "Utils/Layer3LicenseUtil.h"
#include "Utils/DceContext.h"
#include "Utils/DceSynchronousLinearSeqContext.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

DhcpRelayLocalWorker::DhcpRelayLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
    : WaveWorker  (pNsmLocalObjectManager)
{
    trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

    DhcpRelInterfaceLocalManagedObject    DhcpRelInterfaceLocalManagedObject(pNsmLocalObjectManager);

    DhcpRelInterfaceLocalManagedObject.setupOrm();
    addManagedClass (DhcpRelInterfaceLocalManagedObject::getClassName (), this);

    addOperationMap (DHCPRELINTFLOCALCONFIG, 
		reinterpret_cast<PrismMessageHandler> (&DhcpRelayLocalWorker::DhcpRelayIntfLocalConfigMessageHandler));
}

DhcpRelayLocalWorker::~DhcpRelayLocalWorker ()
{
}

PrismMessage  *DhcpRelayLocalWorker::createMessageInstance(const UI32 &operationCode)
{
    PrismMessage *pPrismMessage = NULL;

    trace(TRACE_LEVEL_INFO, string(__FUNCTION__));

    switch (operationCode)
    {

        case DHCPRELINTFLOCALCONFIG:
            /* This handles both physical and Ve interface config */
            pPrismMessage = new DhcpRelIntfLocalConfigMessage ();
            break;

        default :
            pPrismMessage = NULL;
    }

    return (pPrismMessage);
}

/* This function is used to create an MO for show commands, not for config */
WaveManagedObject  *DhcpRelayLocalWorker::createManagedObjectInstance(const string &managedClassName)
{
    WaveManagedObject *pWaveManagedObject = NULL;
    if ((DhcpRelInterfaceLocalManagedObject::getClassName ()) == managedClassName)
    {
        pWaveManagedObject = new DhcpRelInterfaceLocalManagedObject(
            dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
    }
    else
    {
        trace(TRACE_LEVEL_FATAL, string(__FUNCTION__) + string(": Unknown Managed Class Name : ") + managedClassName);
    }
    return (pWaveManagedObject);
}

/* Utility methods */

    NsmL3ReadyManagedObject* DhcpRelayLocalWorker::PhyIfNameGetL3ReadyMO(string ifName,
        PrismSynchronousLinearSequencerContext  *pContext,
        bool fromLocal)
    {
        NsmL3ReadyManagedObject *pMo = NULL;
        vector<WaveManagedObject *> *pResults;
        InterfaceType intf_type;
        vector<string> selectFields;
        selectFields.push_back("objectId");
        selectFields.push_back("ownerManagedObjectId");
        selectFields.push_back("dhcpRelIntfConf");

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

        delete pResults;
        pMo->setIntfType(intf_type);
        return pMo;
    }


/* Function defining the ConfWorker sequencer steps */
void  DhcpRelayLocalWorker::DhcpRelayIntfLocalConfigMessageHandler( DhcpRelIntfLocalConfigMessage *pDhcpRelIntfLocalConfigMessage)
{
    PrismSynchronousLinearSequencerStep sequencerSteps[] =
    {
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigMOStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::prismSynchronousLinearSequencerSucceededStep),
        reinterpret_cast<PrismSynchronousLinearSequencerStep>(&DhcpRelayLocalWorker::prismSynchronousLinearSequencerFailedStep)
    };

    PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pDhcpRelIntfLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

    pPrismSynchronousLinearSequencerContext->execute ();
}


/* Function to send the config to the backend */
ResourceId DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    int status = WAVE_MESSAGE_SUCCESS;

    // Check for Layer3 license before sending to backend
    LAYER_3_LICENSE_CHECK();

    DhcpRelIntfLocalConfigMessage *pDhcpRelIntfLocalConfigMessage = 
        dynamic_cast<DhcpRelIntfLocalConfigMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
    DhcpRelIntfClientConfigMessage *m = new DhcpRelIntfClientConfigMessage();
	UI32	tmp_ifType = pDhcpRelIntfLocalConfigMessage->getIfType();

    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

	if ((tmp_ifType == IF_TYPE_GI) || (tmp_ifType == IF_TYPE_TE) || 
		(tmp_ifType == IF_TYPE_FO)) 
	{
		trace(TRACE_LEVEL_INFO, string("Retrieving the PhyIntfLocalManagedObject for ")+ pDhcpRelIntfLocalConfigMessage->getIfName());
		PhyIntfLocalManagedObject *aPhyIntfLocalManagedObject = NsmL3Util::PhyIfNameGetPhyIfLocalMO
                    (pDhcpRelIntfLocalConfigMessage->getIfName(), pPrismSynchronousLinearSequencerContext);
        if (aPhyIntfLocalManagedObject) {
            //Ensure that this MO is garbage collected
        	pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(aPhyIntfLocalManagedObject);
            if(aPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId) {
            	trace(TRACE_LEVEL_ERROR, string("Cannot configure Dhcp Relay on an aggregator member"));
                return WRC_NSM_ERR_SW_NOT_ALLOWEED;
            }
            if ((DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED) && (aPhyIntfLocalManagedObject->getPortrole())) {
            	trace(TRACE_LEVEL_ERROR, string("Cannot configure Dhcp Relay on an ISL port "));
                return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
            }
		}

    	/* Pass 2 tuple name to backend if we are in VCS mode */
    	string ifname;
    	string intfName = pDhcpRelIntfLocalConfigMessage->getIfName();
    	if(NsmUtils::isValidThreeTuple(intfName)) {
        	NsmUtils::getTwoTupleIfName(intfName, ifname);
    	}
    	else {
        	ifname = intfName;
    	}
    	m->setIfName(ifname);
	}

    /* Check L3 readiness of interface */
    NsmL3ReadyManagedObject *pNsmL3ReadyManagedObject = NULL;
	UI32 vlanId = pDhcpRelIntfLocalConfigMessage->getVlanValue(); 
    SI32 mapId = DcmToolKit::getLocalMappedId();

    if (tmp_ifType == IF_TYPE_VE)  {
        pNsmL3ReadyManagedObject = NsmL3Util::VlanIdGetL3ReadyMO(vlanId, mapId, pPrismSynchronousLinearSequencerContext, false);
    } else if ((tmp_ifType == IF_TYPE_GI) || (tmp_ifType == IF_TYPE_TE) || (tmp_ifType == IF_TYPE_FO)) {
        pNsmL3ReadyManagedObject = PhyIfNameGetL3ReadyMO(pDhcpRelIntfLocalConfigMessage->getIfName(), 
									pPrismSynchronousLinearSequencerContext, true);
    }

    //Ensure that this MO is garbage collected
    if (pNsmL3ReadyManagedObject) {
	    pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pNsmL3ReadyManagedObject);
    }

    if (!pNsmL3ReadyManagedObject)
    {
    	trace (TRACE_LEVEL_ERROR, "DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigStep - L3ReadyMO not found for this "
              "interface, returning.");
        pDhcpRelIntfLocalConfigMessage->setCompletionStatus(WRC_DHCPREL_CLI_ERR_L3_NOT_READY);
        return (WRC_DHCPREL_CLI_ERR_L3_NOT_READY);
	}

	m->setVlanValue(pDhcpRelIntfLocalConfigMessage->getVlanValue());
    m->setIfType(tmp_ifType);
    m->setServerAddress(pDhcpRelIntfLocalConfigMessage->getServerAddress());
    m->setNegation(pDhcpRelIntfLocalConfigMessage->getNegation());

    status = sendSynchronouslyToWaveClient ("iphelpd", m);

	if(WAVE_MESSAGE_SUCCESS == status)
    {
    	status = m->getClientStatus();
		trace (TRACE_LEVEL_DEBUG, "DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigStep - DHCP interface-config message sent successfully to iphelpd backend"); 
        if ((0 != status) && (WAVE_MESSAGE_SUCCESS != status))
            status = DhcpUtils::mapBackEndErrorsToDcmdErrors(status);
	}
	else
		trace (TRACE_LEVEL_ERROR, "DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigStep - DHCP interface-config message not sent to iphelpd backend"); 

   	pDhcpRelIntfLocalConfigMessage->setCompletionStatus(status);
    delete m;
    return status;
}


/* Function to populate the config in the MO */
ResourceId DhcpRelayLocalWorker::UpdateDhcpRelIntfLocalConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
{
    string ifName = "";
    NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
    DhcpRelInterfaceLocalManagedObject *pMo = NULL;
    UI32 ifType = IF_TYPE_INVALID;
	UI32 vlanId = 0;
    SI32 mapId = DcmToolKit::getLocalMappedId();
    bool negation = false;
    
    trace (TRACE_LEVEL_INFO, string(__FUNCTION__));

    DhcpRelIntfLocalConfigMessage *pMsg = dynamic_cast<DhcpRelIntfLocalConfigMessage *>
                                  (pPrismSynchronousLinearSequencerContext->getPPrismMessage());

    ifName = pMsg->getIfName();
    vlanId = pMsg->getVlanValue();
    ifType = pMsg->getIfType();
    if (ifType == IF_TYPE_VE)  {
        pL3ReadyMo = NsmL3Util::VlanIdGetL3ReadyMO(vlanId, mapId, pPrismSynchronousLinearSequencerContext, false);
    } else if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO)) {
        pL3ReadyMo = PhyIfNameGetL3ReadyMO(ifName, pPrismSynchronousLinearSequencerContext, true);
    }
    if (pL3ReadyMo == NULL) {
        trace(TRACE_LEVEL_INFO, 
              string(__FUNCTION__) + string(": L3ReadyMO not found !!"));
        return WAVE_MESSAGE_ERROR;
    }
            
    negation = pMsg->getNegation();

    if (negation) {
        trace(TRACE_LEVEL_INFO, 
              "DhcpLocalWorker::UpdateDhcpRelIntfLocalConfigMOStep: Deletion");
        vector<WaveManagedObjectPointer<DhcpRelInterfaceLocalManagedObject> > dhcpRelIntfList = pL3ReadyMo->getDhcpRelIntfConf();
        int size = dhcpRelIntfList.size();
        for(int i = 0; i < size; i++) {
        	pMo = dhcpRelIntfList[i].operator ->();
        	if(pMo != NULL) {
				if(pMo->getServerAddress() == pMsg->getServerAddress()) {
					deleteFromComposition(NsmL3ReadyManagedObject::getClassName(), DhcpRelInterfaceLocalManagedObject::getClassName(),
						"dhcpRelIntfConf", pMo->getOwnerManagedObjectId(), pMo->getObjectId());
				}
        	}
        }
    } else {
        trace(TRACE_LEVEL_INFO, 
              "DhcpLocalWorker::UpdateDhcpRelIntfLocalConfigMOStep: Creation");
        pMo = new DhcpRelInterfaceLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
        prismAssert (NULL != pMo, __FILE__, __LINE__);

        pMo->setServerAddress(pMsg->getServerAddress());
        pMo->setIfType(pMsg->getIfType());
        pMo->setIfName(pMsg->getIfName());
        pMo->setVlanId(pMsg->getVlanValue());
	    pMo->setOwnerManagedObjectId(pL3ReadyMo->getObjectId());
    	addToComposition(NsmL3ReadyManagedObject::getClassName(), DhcpRelInterfaceLocalManagedObject::getClassName(), "dhcpRelIntfConf", pL3ReadyMo->getObjectId(), pMo->getObjectId());		
        pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pMo);
    }

        
    if (pL3ReadyMo)
        pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection(pL3ReadyMo);

    return WAVE_MESSAGE_SUCCESS;
}

}
