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
 *   Author : vkarnati                                                     *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Types/BitMap.h"


#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpConfigLocalWorker.h"
#include "Arp/Local/ArpTypes.h"
#include "ClientInterface/Arp/ArpStaticConfigClientMessage.h"
#include "Arp/Local/ArpConfigLocalManagedObject.h"
#include "Arp/Local/ArpStaticConfigHandler.h"
#include "Arp/Local/ArpStaticConfigLocalManagedObject.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForDeletion.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContextForUpdate.h"
#include "Framework/Database/DatabaseObjectManagerExecuteQueryMessage.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForMultiPartitionCleanup.h"

#include "Vrf/Local/VrfUtils.h"

namespace DcmNs
{

    ArpLocalObjectManager::ArpLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
        m_pArpConfigLocalWorker = new ArpConfigLocalWorker (this);
        prismAssert (NULL != m_pArpConfigLocalWorker, __FILE__, __LINE__);

        m_pArpStaticConfigHandler = new ArpStaticConfigHandler (this);
        prismAssert (NULL != m_pArpStaticConfigHandler, __FILE__, __LINE__);
    }

    ArpLocalObjectManager::~ArpLocalObjectManager ()
    {
    }

    ArpLocalObjectManager  *ArpLocalObjectManager::getInstance()
    {
        static ArpLocalObjectManager *pArpLocalObjectManager = new ArpLocalObjectManager ();

        WaveNs::prismAssert (NULL != pArpLocalObjectManager, __FILE__, __LINE__);

        return (pArpLocalObjectManager);
    }

    string  ArpLocalObjectManager::getClassName()
    {
        return ("Arp");
    }

    PrismServiceId  ArpLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *ArpLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ArpLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }
	void ArpLocalObjectManager::deleteStaticArp(UI32 slot)
    {
        int size = 0;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
 		ArpStaticConfigClientMessage *clientMsg;

        trace(TRACE_LEVEL_INFO, string("cleanup Arp interface static arp on slot: ") + slot);

        ArpStaticConfigLocalManagedObject *pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt("ArpStaticConfigLocalManagedObject");
        syncQueryCtxt.setSlotFilter(slot);
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            size = pResults->size ();
            if (size > 0) {
                startTransaction();
                for (int i = 0; i < size; i ++) {
                    pMo = dynamic_cast<ArpStaticConfigLocalManagedObject*> ((*pResults)[i]);
					int interfaceType;
    	            clientMsg = new ArpStaticConfigClientMessage();
		    	    clientMsg->setIpAddress(pMo->getIpAddress());
        			clientMsg->setMacAdress(pMo->getMacAdress());
			        clientMsg->setOpCode(true);
    	    		clientMsg->setInterfaceType(pMo->getInterfaceType());
					interfaceType = pMo->getInterfaceType();
	        		switch (interfaceType)
			        {
        			    case CONFIG_ARP_PORTCHANNEL:
            	    	clientMsg->setPortChannelValue(pMo->getPortChannelValue());
	            	    break;
    	
        		    	case CONFIG_ARP_TENGIGABIT:
	                	clientMsg->setTenGigabitValue(pMo->getTenGigabitValue());
		                break;
    	
    			        case CONFIG_ARP_VLAN:
            		    clientMsg->setVlanValue(pMo->getVlanValue());
                		break;
			        }

    	            status = sendSynchronouslyToWaveClient("arp", clientMsg);
        	        if (status == WAVE_MESSAGE_SUCCESS)
            	        status = clientMsg->getCompletionStatus();
                	if (status != WAVE_MESSAGE_SUCCESS)
                    	trace(TRACE_LEVEL_ERROR, "restore for Arp interface static arp failed");

	                delete clientMsg;
                    delete pMo;
                }
                commitTransaction();
            }
            pResults->clear();
            delete pResults;
        }

        trace(TRACE_LEVEL_DEBUG, string("cleanup RTM interface static route done: ") + size);
    }
 
    void ArpLocalObjectManager::multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup)
    {
        tracePrintf (TRACE_LEVEL_DEVEL, false, false,  "ArpLocalObjectManager::multiPartitionPartialCleanup : Entering ... ");

        ObjectId ownerPartitionManagedObjectId (pWaveAsynchronousContextForMultiPartitionCleanup->getOwnerPartitionManagedObjectId ());

        startTransaction();

        string managedClassName = ArpStaticConfigLocalManagedObject::getClassName ();

        WaveManagedObjectSynchronousQueryContextForDeletion *waveManagedObjectSynchronousQueryContextForDeletion = new WaveManagedObjectSynchronousQueryContextForDeletion (managedClassName);
        waveManagedObjectSynchronousQueryContextForDeletion->addAndAttribute (new AttributeObjectId (ownerPartitionManagedObjectId, "ownerPartitionManagedObjectId"));

        deleteMultipleWaveManagedObjects (waveManagedObjectSynchronousQueryContextForDeletion);

        ResourceId status =  commitTransaction(); 

        if (FRAMEWORK_SUCCESS == status)
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        }
        else 
        {
            pWaveAsynchronousContextForMultiPartitionCleanup->setCompletionStatus (status);
        }

        pWaveAsynchronousContextForMultiPartitionCleanup->callback ();
    }

  void ArpLocalObjectManager::restoreStaticArp(UI32 slot)
    {
 		ArpStaticConfigClientMessage *clientMsg;
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        int size = 0;

        trace (TRACE_LEVEL_DEBUG, string("ArpLocalObjectManager::restoreStaticArp") + slot);

    		ArpStaticConfigLocalManagedObject *pMo = NULL;
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt("ArpStaticConfigLocalManagedObject");
	    	syncQueryCtxt.setSlotFilter(slot);
        syncQueryCtxt.setLocationIdFilter(FrameworkToolKit::getThisLocationId ());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);

        if (pResults) {
            size = pResults->size (); 
            for (int i = 0; i < size; i ++) {
                pMo = dynamic_cast<ArpStaticConfigLocalManagedObject*> ((*pResults)[i]);
				int interfaceType;

                clientMsg = new ArpStaticConfigClientMessage();
                string vrfName = VrfUtils::getVrfNameFromOwnerPartitionManagedObjectId(pMo->getOwnerPartitionManagedObjectId ());
                VrfUtils::setPartitionName(clientMsg, vrfName);
		        clientMsg->setIpAddress(pMo->getIpAddress());
        		clientMsg->setMacAdress(pMo->getMacAdress());
		        clientMsg->setOpCode(false);
        		clientMsg->setInterfaceType(pMo->getInterfaceType());
				interfaceType = pMo->getInterfaceType();
        		switch (interfaceType)
		        {
        		    case CONFIG_ARP_PORTCHANNEL:
                	clientMsg->setPortChannelValue(pMo->getPortChannelValue());
	                break;
    	
        		    case CONFIG_ARP_TENGIGABIT:
                	clientMsg->setTenGigabitValue(pMo->getTenGigabitValue());
	                break;
    
    		        case CONFIG_ARP_VLAN:
            	    clientMsg->setVlanValue(pMo->getVlanValue());
                	break;
		        }

                status = sendSynchronouslyToWaveClient("arp", clientMsg);
                if (status == WAVE_MESSAGE_SUCCESS)
                    status = clientMsg->getCompletionStatus();
                if (status != WAVE_MESSAGE_SUCCESS)
                    trace(TRACE_LEVEL_ERROR, "restore for ARP interface static arp failed");

                delete clientMsg;
            }   
        }   

        trace(TRACE_LEVEL_DEBUG, string("restore for ARP interface static route done: ") + size);

        if(pResults != NULL)
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);

	}


	void ArpLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
    {
        string  name = pWaveAsynchronousContextForPostBootPhase->getPassName();

        if (name == "DCM_POSTBOOT_GLOBAL_STAGE5")
        {
            trace (TRACE_LEVEL_INFO, string("ArpLocalObjectManager::postboot Entered with pass:") + name);
            //restoreStaticArp(WAVE_INVALID_SLOT);
            trace (TRACE_LEVEL_INFO, string("ArpLocalObjectManager::postboot done:") + name);
        }

       if (name == "DCM_POSTBOOT_SLOT_STAGE5") {
            UI32 slot = pWaveAsynchronousContextForPostBootPhase->getSlotNum();

            trace (TRACE_LEVEL_INFO, 
                   string("ArpLocalObjectManager::postboot Entered with pass:") + name + " slot# = " + slot);

            restoreStaticArp(WAVE_INVALID_SLOT);

            trace (TRACE_LEVEL_INFO, 
                   string("ArpLocalObjectManager::postboot done:") + name + " slot# = " + slot);
        }
        pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForPostBootPhase->callback ();

    }
    void ArpLocalObjectManager::slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover)
    {
        UI32 slot = pWaveAsynchronousContextForSlotFailover->getSlotNumber();

		trace (TRACE_LEVEL_INFO, string("ArpLocalObjectManager::slotFailover"));
        deleteStaticArp(slot);
        pWaveAsynchronousContextForSlotFailover->
        setCompletionStatus(WAVE_MESSAGE_SUCCESS);
        pWaveAsynchronousContextForSlotFailover->callback();
    }


}
