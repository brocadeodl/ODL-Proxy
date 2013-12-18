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
 **************************************************************************/

#include <iostream>
#include <sstream>

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

#include "Beacon/Local/BeaconLocalObjectManager.h"
#include "Beacon/Local/BeaconServiceMessage.h"
#include "Beacon/Local/BeaconActTypes.h"


#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "DcmResourceIdEnums.h"


#include "ClientInterface/Nsm/NsmUtils.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"

#include "ClientInterface/Nsm/NsmBeaconMessage.h"
#include "ClientInterface/Nsm/NsmDefaultConfigurationCmdsMessage.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "fos_swbd.h"
#include "utils/slot.h"
extern "C" {
	#include <switch/switch.h>
	#include <switch/port.h>
	#include <em/em_if.h>
}


namespace DcmNs
{

   BeaconLocalObjectManager::BeaconLocalObjectManager ()
        : WaveLocalObjectManager  (getClassName ())
    {
	    addOperationMap (BEACONSERVICE, reinterpret_cast<PrismMessageHandler> (&BeaconLocalObjectManager::BeaconServiceMessageHandler));
    }

    BeaconLocalObjectManager::~BeaconLocalObjectManager ()
    {
    }

    BeaconLocalObjectManager  *BeaconLocalObjectManager::getInstance()
    {
        static BeaconLocalObjectManager *pBeaconLocalObjectManager = new BeaconLocalObjectManager ();

        WaveNs::prismAssert (NULL != pBeaconLocalObjectManager, __FILE__, __LINE__);

        return (pBeaconLocalObjectManager);
    }

    string  BeaconLocalObjectManager::getClassName()
    {
        return ("Beacon");
    }

    PrismServiceId  BeaconLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

	PrismMessage  *BeaconLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {
	    case BEACONSERVICE :
                pPrismMessage = new BeaconServiceMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }
	ResourceId  BeaconLocalObjectManager::BeaconService (PrismSynchronousLinearSequencerContext *pBeaconServiceMessageContext)
	{
	    BeaconServiceMessage *pBeaconServiceMessage = reinterpret_cast<BeaconServiceMessage *> (pBeaconServiceMessageContext->getPPrismMessage ());
		ResourceId status = WAVE_MESSAGE_SUCCESS;
        int ret = 0, phySlot = 0, bladeId = 0, maxSlot = 0, isSlotBase = 0;
		unsigned int bladeNo, bladePort, mapId, cmd, maxPort = 0;
		NsmBeaconMessage *msg_p = NULL;
		char if_name[50];
		string ifname_str = "";
		void *data = NULL;
		UI32 size = 0;

	/* convert the port identifier string to a switch-specific port number
	 * the number of entries in the string (i.e. n/n or n/n/n) is
	 * determined by finding
	 * the first "/" character both from the front and
	 * back of the string.  If the index
	 * is different, there are three numbers, else there are two.
	 */

	 cmd = pBeaconServiceMessage->getCmdId();
	 if (cmd == BEACON_INTERFACE_ON || cmd == BEACON_INTERFACE_OFF) {
	        if (pBeaconServiceMessage->getIf().find("/") != pBeaconServiceMessage->getIf().rfind("/")) {
				trace (TRACE_LEVEL_INFO, string("***beacon local manager : three tuples") + pBeaconServiceMessage->getIf().c_str());
    	        sscanf(pBeaconServiceMessage->getIf().c_str(), "%u/%u/%u", &mapId, &bladeNo, &bladePort);
        	} else {
				trace (TRACE_LEVEL_INFO, string("***beacon local manager : two tuples") + pBeaconServiceMessage->getIf().c_str());
            	sscanf(pBeaconServiceMessage->getIf().c_str(), "%u/%u", &bladeNo, &bladePort);
            }

            trace (TRACE_LEVEL_INFO, string("***beacon local manager : mapid") + mapId + string (" bladeNo:") + bladeNo +
                                string(" bladePort:") + bladePort);
			msg_p = new NsmBeaconMessage();
			msg_p->setCliCode(NSMBEACONMSG);
			if (cmd == BEACON_INTERFACE_ON) {
			   msg_p->setCmdId(1);
			} else { /* off case */
			   msg_p->setCmdId(0);
			}
			phySlot = slot_id2no(SYS_SLOT_LC, bladeNo);
			maxSlot = chassisNumSlots();
			if (maxSlot > 1) {
				isSlotBase = 1;
			}
			/* pizza box has slot = 0 */
			if (isSlotBase == 0 && bladeNo !=0) {
				trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid slot \n") + bladeNo);
				pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
				return (WAVE_MESSAGE_SUCCESS);
			}
			/* slotbase chassis */	 
			if (isSlotBase && (phySlot < 0 || phySlot > maxSlot)) {
				trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid physical slot \n") + phySlot);
				pBeaconServiceMessage->setStatus(BEACON_INVALID_LINECARD);
				return (WAVE_MESSAGE_SUCCESS);
			}
			trace(TRACE_LEVEL_INFO, string("***beacon pluggin slotBase ") + isSlotBase + string("maxslot ") + maxSlot);

			/* get Blade id to figure out num ports per blade */
			ret = getBladeID(phySlot, &bladeId);
			if (ret < 0) {
				trace(TRACE_LEVEL_INFO, string("***beacon pluggin no inserted linecard \n") + bladeNo);
				pBeaconServiceMessage->setStatus(BEACON_INVALID_LINECARD);
				return (WAVE_MESSAGE_SUCCESS);
			}
			trace(TRACE_LEVEL_INFO, string("***beacon pluggin physlot ") + phySlot + string("blade type ") +
			                       bladeId + string("ret ") + ret);
			switch (bladeId) {
			case ELARA2E_BLADE_ID:
				maxPort = 24;
				break;
			case ELARA2F_BLADE_ID:
				maxPort = 32;
				break;
			case CALLISTOE_BLADE_ID:
				maxPort = 60;
				break;
			case CALLISTOF_BLADE_ID:
				maxPort = 76;
				break;
			case CARME_E_BLADE_ID:
				maxPort = 54;
				break;
			case MERCURY_LC48X10G_BLADE_ID:
				maxPort = 48;
				break;
			case MERCURY_LC12X40G_BLADE_ID:
				maxPort = 12;
				break;
			case MERCURY_LC72X1G_BLADE_ID:
				maxPort = 72;
				break;
			case MERCURY_LC48X1G_BLADE_ID:
				maxPort = 48;
				break;
			case CASTOR_BLADE_ID:
				maxPort = 64;
				break;
			case CASTORT_BLADE_ID:
				maxPort = 64;
				break;
			default:
				trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid bladeid \n") + bladeNo);
				pBeaconServiceMessage->setStatus(BEACON_INVALID_LINECARD);
				return (WAVE_MESSAGE_SUCCESS);
				break;
			}
			/* blade port start from 1 to maxPort */          
			if (bladePort < 1 || bladePort > maxPort) {
				trace(TRACE_LEVEL_INFO, string("***beacon pluggin  invalid interface num \n") + bladePort);
				pBeaconServiceMessage->setStatus(BEACON_INVALID_INTERFACE);
				return (WAVE_MESSAGE_SUCCESS);
			}

			switch (pBeaconServiceMessage->getIfType()) {

				case beacon_tengigabitethernet :
					if (isSlotBase && (bladeId != MERCURY_LC48X10G_BLADE_ID)) {
						trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid 10G blade \n") + bladeId);
						pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
						return (WAVE_MESSAGE_SUCCESS);
					}
					sprintf(if_name, "te%u/%u", bladeNo, bladePort);
					break;

				case beacon_fortygigabitethernet:
					if (isSlotBase && (bladeId != MERCURY_LC12X40G_BLADE_ID)) {
						trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid 40G blade \n") + bladeId);
						pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
						return (WAVE_MESSAGE_SUCCESS);
					}
					sprintf(if_name, "fo%u/%u", bladeNo, bladePort);
					break;
				case beacon_gigabitethernet:
					if (isSlotBase && (bladeId != MERCURY_LC48X1G_BLADE_ID) && (bladeId != MERCURY_LC72X1G_BLADE_ID)) {
						trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid 1G blade \n") + bladeId);
						pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
						return (WAVE_MESSAGE_SUCCESS);
					}
					sprintf(if_name, "gi%u/%u", bladeNo, bladePort);
					break;
				default :
					trace(TRACE_LEVEL_INFO, string("***beacon pluggin invalid interface type\n") + bladeId);
					pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
					return (WAVE_MESSAGE_SUCCESS);
					break;
			}

			ifname_str.append(if_name);
	        msg_p->setIf(ifname_str);
			trace (TRACE_LEVEL_INFO, string ("beacon pluggin ifname : ") + if_name);
			status = sendSynchronouslyToWaveClient("nsm", msg_p);
	        if (status != WAVE_MESSAGE_SUCCESS) {
	           pBeaconServiceMessage->setStatus(BEACON_MSG_SEND_FAIL);
	           trace(TRACE_LEVEL_INFO, string("***beacon port pluggin send fail***\n"));
	        } else {
	           NsmBeaconMsgOut_t *outMsg;
	          /* get backend msg error */
	           msg_p->getDuplicateInterfaceOutputBuffer(NSMBEACONMSG,
	                  size, data);
	           if (data == NULL) {
	             pBeaconServiceMessage->setStatus(BEACON_MSG_GEN_ERROR);
	             trace(TRACE_LEVEL_INFO, string("***beacon pluggin no data\n"));
	             delete(msg_p);
	             return (WAVE_MESSAGE_SUCCESS); 
	           }
	           outMsg = (NsmBeaconMsgOut_t *)data;
	           trace(TRACE_LEVEL_INFO, string("beacon port pluggin ret ") +
	                      outMsg->ret); 
	           if (outMsg->ret == NSM_API_SET_ERR_NO_SUCH_INTERFACE) {
	             pBeaconServiceMessage->setStatus(BEACON_INVALID_INPUT);
	           } else if (outMsg->ret != 0) {
	             pBeaconServiceMessage->setStatus(BEACON_MSG_GEN_ERROR);
	           } else { 
	             pBeaconServiceMessage->setStatus(BEACON_MSG_SUCCESS);
	           }
	        }
	        delete(msg_p);
	 } else if (cmd == BEACON_CHASSIS_ON) { 
		ret = chassisBeaconSet(1);
		if (ret == 0) {
		   pBeaconServiceMessage->setStatus(BEACON_MSG_SUCCESS);
		   trace(TRACE_LEVEL_INFO, string("***beacon chassis on success\n"));
		} else {
		   pBeaconServiceMessage->setStatus(BEACON_MSG_GEN_ERROR);
		   trace(TRACE_LEVEL_INFO, string("***beacon chassis on failed\n"));
		}
	}  else if (cmd == BEACON_CHASSIS_OFF) { 
		ret = chassisBeaconSet(0);
		if (ret == 0) {
		   pBeaconServiceMessage->setStatus(BEACON_MSG_SUCCESS);
		   trace(TRACE_LEVEL_INFO, string("***beacon chassis off success\n"));
		} else {
		   pBeaconServiceMessage->setStatus(BEACON_MSG_GEN_ERROR);
		   trace(TRACE_LEVEL_INFO, string("***beacon chassis off failed\n"));
		}
	} else {
	   trace(TRACE_LEVEL_INFO, string("***beacon pluggin unknown cmd \n") + cmd);
	   pBeaconServiceMessage->setStatus(BEACON_UNKNOWN_CMD);
	}

	return (WAVE_MESSAGE_SUCCESS);
}


void BeaconLocalObjectManager::BeaconServiceMessageHandler(BeaconServiceMessage *pBeaconServiceMessage)
{
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&BeaconLocalObjectManager::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&BeaconLocalObjectManager::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&BeaconLocalObjectManager::BeaconService),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&BeaconLocalObjectManager::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&BeaconLocalObjectManager::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pBeaconServiceMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
}

}
