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
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Arp/Local/ArpLocalObjectManager.h"
#include "Arp/Local/ArpConfigLocalWorker.h"
#include "Arp/Local/ArpStaticConfigLocalMessage.h"
#include "Arp/Local/ArpConfigLocalMessage.h"
#include "Arp/Local/ArpResetLocalMessage.h"
#include "Arp/Local/ArpClearLocalMessage.h"
#include "Arp/Local/ArpShowLocalMessage.h"
#include "Arp/Local/ArpDebugLocalMessage.h"
#include "Arp/Local/ArpStaticConfigLocalManagedObject.h"
#include "Arp/Local/ArpConfigLocalManagedObject.h"
#include "Arp/Local/ArpTypes.h"
#include "ClientInterface/Arp/ArpDebugClientMessage.h"
#include "ClientInterface/Arp/ArpConfigClientMessage.h"
#include "ClientInterface/Arp/ArpStaticConfigClientMessage.h"
#include "ClientInterface/Arp/ArpMessageDef.h"
#include "ClientInterface/Arp/ArpShowMessage.h"
#include "ClientInterface/Arp/ArpClearMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "Utils/DceClusterUtils.h"

#define ARP_PLUG_ASSERT_PTR(exp) \
            prismAssert ((exp) != NULL, __FILE__, __LINE__);


namespace DcmNs
{

    ArpConfigLocalWorker::ArpConfigLocalWorker ( ArpLocalObjectManager *pArpLocalObjectManager)
        : WaveWorker  (pArpLocalObjectManager)
    {
        ArpStaticConfigLocalManagedObject    ArpStaticConfigLocalManagedObject    (pArpLocalObjectManager);
        ArpConfigLocalManagedObject    ArpConfigLocalManagedObject    (pArpLocalObjectManager);
        ArpStaticConfigLocalManagedObject.setupOrm ();
        addManagedClass (ArpStaticConfigLocalManagedObject::getClassName (), this);
        ArpConfigLocalManagedObject.setupOrm ();
        addManagedClass (ArpConfigLocalManagedObject::getClassName (), this);
        addOperationMap (ARPSTATICCONFIGLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpStaticConfigLocalMessageHandler));
        addOperationMap (ARPCONFIGLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpConfigLocalMessageHandler));
        addOperationMap (ARPRESETLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpResetLocalMessageHandler));
        addOperationMap (ARPCLEARLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpClearLocalMessageHandler));
        addOperationMap (ARPSHOWLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpShowLocalMessageHandler));
        addOperationMap (ARPDEBUGLOCAL, reinterpret_cast<PrismMessageHandler> (&ArpConfigLocalWorker::ArpDebugLocalMessageHandler));
    }

    ArpConfigLocalWorker::~ArpConfigLocalWorker ()
    {
    }

    PrismMessage  *ArpConfigLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case ARPSTATICCONFIGLOCAL :
                pPrismMessage = new ArpStaticConfigLocalMessage ();
                break;
            case ARPCONFIGLOCAL :
                pPrismMessage = new ArpConfigLocalMessage ();
                break;
            case ARPRESETLOCAL :
                pPrismMessage = new ArpResetLocalMessage ();
                break;
            case ARPCLEARLOCAL :
                pPrismMessage = new ArpClearLocalMessage ();
                break;
            case ARPSHOWLOCAL :
                pPrismMessage = new ArpShowLocalMessage ();
                break;
            case ARPDEBUGLOCAL :
                pPrismMessage = new ArpDebugLocalMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *ArpConfigLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((ArpStaticConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ArpStaticConfigLocalManagedObject(dynamic_cast<ArpLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((ArpConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new ArpConfigLocalManagedObject(dynamic_cast<ArpLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "ArpConfigLocalWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

   ResourceId ArpConfigLocalWorker::ArpClearHandler(PrismSynchronousLinearSequencerContext *ctx_p)
   {
       ArpClearLocalMessage *im = dynamic_cast<ArpClearLocalMessage *>
                                                 (ctx_p->getPPrismMessage ());
       ResourceId      status;

       ArpClearMessage *m = new ArpClearMessage(im->getIfName() , im->getCmdCode() , im->getIp_addr(), im->getMac_addr() );
       m->setPartitionName(im->getPartitionName());

       PLUG_INFO_S(string("vrf Name = ") + im->getPartitionName() + " ifName = " + im->getIfName() + " cmd code "  + im->getCmdCode());
       PLUG_INFO_S(string("ipaddress = ") + im->getIp_addr().toString() + " mac address " + im->getMac_addr() );

       status = sendSynchronouslyToWaveClient("arp", m);
       
       if (status == WAVE_MESSAGE_SUCCESS) {
            if (m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                im->setCompletionStatus(m->getCompletionStatus());
                status = im->getCompletionStatus();
                delete m;
                return status;
            }

            if ((m->getClientStatus() != WAVE_MESSAGE_SUCCESS) && (m->getClientStatus() != 0)) {
                FrameworkToolKit::localize(m->getClientStatus());
                PLUG_INFO_S(string("client error ") + FrameworkToolKit::localize(m->getClientStatus()));
                im->setCompletionStatus(m->getClientStatus());
                status = im->getCompletionStatus();
            }
            delete m;
            return status;
       }
       if (status != WAVE_MESSAGE_SUCCESS) {
            trace (TRACE_LEVEL_ERROR, 
            string("Client Message sending failed: ") +
                    string("reason:") +  FrameworkToolKit::localize(status));
            delete m;
       }
      return status;
   }


    void  ArpConfigLocalWorker::ArpClearLocalMessageHandler( ArpClearLocalMessage *pArpClearLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::ArpClearHandler),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pArpClearLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId ArpConfigLocalWorker::updateArpConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
        ArpConfigLocalMessage *pArpConfigLocalMessage = dynamic_cast<ArpConfigLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        if (! pArpConfigLocalMessage)
            return WAVE_MESSAGE_ERROR;

        int status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "ArpConfigLocalWorker::updateArpConfigStep: Entered....");

        ArpConfigClientMessage *m = new ArpConfigClientMessage();
        m->setSystemMaxArp(pArpConfigLocalMessage->getSystemMaxArp()); 
    
        status = sendSynchronouslyToWaveClient ("arp", m);

        delete m; 
        return status; 

    }

    ResourceId ArpConfigLocalWorker::updateArpConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int createFlag = 1;
        int status = 0;

        ArpConfigLocalManagedObject *pArpConfigLocalManagedObject = NULL;
        ArpConfigLocalMessage *pArpConfigLocalMessage = dynamic_cast<ArpConfigLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ARP_PLUG_ASSERT_PTR (pArpConfigLocalMessage);

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ArpConfigLocalManagedObject::getClassName());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 0");
                createFlag = 1;
                status = WAVE_MESSAGE_SUCCESS;
            } else if (1 == numberOfResults) {
                pArpConfigLocalManagedObject  = dynamic_cast<ArpConfigLocalManagedObject*>((*pResults)[0]);
                ARP_PLUG_ASSERT_PTR (pArpConfigLocalManagedObject);

                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
                createFlag = 0;
                status = WAVE_MESSAGE_SUCCESS;
            } else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        if (createFlag == 0) {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 0");
            updateWaveManagedObject (pArpConfigLocalManagedObject);
            prismAssert (NULL != pArpConfigLocalManagedObject, __FILE__, __LINE__);
        } else {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 1");
            pArpConfigLocalManagedObject = new ArpConfigLocalManagedObject(dynamic_cast<ArpLocalObjectManager*>(getPWaveObjectManager()));
            prismAssert (NULL != pArpConfigLocalManagedObject, __FILE__, __LINE__);
        }

        trace (TRACE_LEVEL_DEVEL, "ArpConfigLocalWorkercase MSG_ENABLE ");
        pArpConfigLocalManagedObject->setSystemMaxArp(pArpConfigLocalMessage->getSystemMaxArp());

        if (pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        if (createFlag) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pArpConfigLocalManagedObject);
        }

        return status;
    }

    ResourceId ArpConfigLocalWorker::updateArpStaticConfigStep (ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage)
    {
        int interfaceType;
        if (! pArpStaticConfigLocalMessage)
            return WAVE_MESSAGE_ERROR;

        int status = WAVE_MESSAGE_SUCCESS;
        trace (TRACE_LEVEL_DEBUG, "ArpConfigLocalWorker::updateArpStaticArpConfigStep: Entered....");

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();

        ArpStaticConfigClientMessage *m = new ArpStaticConfigClientMessage();
    

        m->setIpAddress(pArpStaticConfigLocalMessage->getIpAddress());
        trace (TRACE_LEVEL_DEBUG, "ArpConfigLocalWorker::updateArpStaticArpConfigStep: ...." + pArpStaticConfigLocalMessage->getMacAdress() + ",.....'");
        m->setMacAdress(pArpStaticConfigLocalMessage->getMacAdress());
        m->setOpCode(pArpStaticConfigLocalMessage->getOpCode());
        interfaceType = pArpStaticConfigLocalMessage->getInterfaceType();
        switch (interfaceType)
        {
            case CONFIG_ARP_PORTCHANNEL:
                m->setPortChannelValue(pArpStaticConfigLocalMessage->getPortChannelValue());
        		m->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;
                
            case CONFIG_ARP_GIGABIT:
                m->setGigabitValue(pArpStaticConfigLocalMessage->getGigabitValue());
        		m->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;

            case CONFIG_ARP_TENGIGABIT:
                m->setTenGigabitValue(pArpStaticConfigLocalMessage->getTenGigabitValue());
        		m->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;
            
            case CONFIG_ARP_FORTYGIGABIT:
                m->setFortyGigabitValue(pArpStaticConfigLocalMessage->getFortyGigabitValue());
        		m->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;

            case CONFIG_ARP_VLAN:
                m->setVlanValue(pArpStaticConfigLocalMessage->getVlanValue());
        		m->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;
        }
        
        status = sendSynchronouslyToWaveClient ("arp", m);

        trace (TRACE_LEVEL_DEBUG, string("ArpConfigLocalWorker::updateArpStaticArpConfigStep: Ended with status  = ") + status);
        if( status == WAVE_MESSAGE_SUCCESS)
        {
            if(  m->getCompletionStatus() != WAVE_MESSAGE_SUCCESS){
                //bpatel : To avoid memory leak
                status = m->getCompletionStatus();
                trace (TRACE_LEVEL_DEBUG, string("ArpConfigLocalWorker::updateArpStaticArpConfigStep: Ended with completion status = ") + m->getCompletionStatus());
                delete m;
                return status ;
            }

            if ( (0 != m->getClientStatus() ) && ( WAVE_MESSAGE_SUCCESS != m->getClientStatus() ) )  {
                WaveNs::trace(TRACE_LEVEL_ERROR, string("ArpConfigLocalWorker::updateArpStaticArpConfigStep: Resp:  DL_PO_L Client Error MsgId: ") +
                    m->getMessageId() +
                    FrameworkToolKit::localize(m->getClientStatus()));
                //bpatel : To avoid memory leak
                trace (TRACE_LEVEL_INFO, string("ArpConfigLocalWorker::updateArpStaticArpConfigStep: Ended with client status = ") +  m->getClientStatus());
                status = m->getClientStatus();
                delete m;
                return status;
            }
             trace (TRACE_LEVEL_INFO, string("ArpConfigLocalWorker::updateArpStaticArpConfigStep: Ended with client status = ") +  m->getClientStatus());
        }
        delete m; 
        return status; 
    }

    int32_t ArpConfigLocalWorker::getSlotFromIfName (UI32 ifType, string ifName)
    {   
        if (ifType == 0 || ifType == 2) {
            return WAVE_INVALID_SLOT;
        }   

        return NsmUtils::getSlotId(ifName);
    }

    ResourceId ArpConfigLocalWorker::updateArpStaticConfigMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        int createFlag = 1;
        int status = 0;
		int32_t slot_id = WAVE_INVALID_SLOT;
        int interfaceType;
        IpV4Address IpAddress;

        ArpStaticConfigLocalManagedObject *pArpStaticConfigLocalManagedObject = NULL;
        ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage = dynamic_cast<ArpStaticConfigLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());
        ARP_PLUG_ASSERT_PTR (pArpStaticConfigLocalMessage);

        IpAddress = pArpStaticConfigLocalMessage->getIpAddress();

        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ArpStaticConfigLocalManagedObject::getClassName());
        syncQueryCtxt.addAndAttribute (new AttributeIpV4Address(&IpAddress,"IpAddress"));
        syncQueryCtxt.setPartitionFilter(pArpStaticConfigLocalMessage->getPartitionName(), FrameworkToolKit::getThisLocationId ());
        vector<WaveManagedObject *> *pResults = querySynchronously (&syncQueryCtxt);
        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                trace (TRACE_LEVEL_DEBUG, "numberOfResults for the arp = 0");
                createFlag = 1;
                status = WAVE_MESSAGE_SUCCESS;
            } 
            else if (1 == numberOfResults) {
                pArpStaticConfigLocalManagedObject  = dynamic_cast<ArpStaticConfigLocalManagedObject*>((*pResults)[0]);

                trace (TRACE_LEVEL_DEBUG, "numberOfResults = 1");
                createFlag = 0;
                if(!pArpStaticConfigLocalMessage->getOpCode()) {
                    status = WRC_ARP_STATIC_EXISTS;
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                    return status;
                }
            } 

            else {
                status = WAVE_MESSAGE_ERROR;
            }
        }
        if (createFlag == 0) {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 0");

            if (pArpStaticConfigLocalMessage->getOpCode() == true) {
                trace (TRACE_LEVEL_DEVEL, "createFlag = 0:: opcode = true:: Deletion");
                ResourceId res = updateArpStaticConfigStep (pArpStaticConfigLocalMessage);
                delete pArpStaticConfigLocalManagedObject;
                delete pResults;
                return res;
            }
            else {
                /*Should not be hitting this case */
                pArpStaticConfigLocalMessage->setCompletionStatus(WRC_ARP_STATIC_EXISTS);
                return WRC_ARP_STATIC_EXISTS;
                //updateWaveManagedObject (pArpStaticConfigLocalManagedObject);
                //prismAssert (NULL != pArpStaticConfigLocalManagedObject, __FILE__, __LINE__);
            }
        } else {
            trace (TRACE_LEVEL_DEVEL, "createFlag = 1");

            if (pArpStaticConfigLocalMessage->getOpCode() == true) {
                trace (TRACE_LEVEL_WARN, "ArpConfigLocalWorker:: Attempted delete of a non-existent arp.");
                if (pResults) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
                return WAVE_MESSAGE_SUCCESS;
            }
            int res = updateArpStaticConfigStep (pArpStaticConfigLocalMessage);
            if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == res || WRC_LAYER_3_NOT_SUPPORTED_SA_MODE == res) {
                trace (TRACE_LEVEL_INFO, "ArpConfigLocalWorker::updateArpStaticConfigMOStep. LAYER_3_LICENSE not present.");
                delete pArpStaticConfigLocalManagedObject;
                if (pResults) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
                return res;
            }
            if (ARP_INTERFACE_DOES_NOT_EXIST == res) {
                trace (TRACE_LEVEL_INFO, "ArpConfigLocalWorker::updateArpStaticConfigMOStep. Error: ARP_INTERFACE_DOES_NOT_EXIST.");
                delete pArpStaticConfigLocalManagedObject;
                if (pResults) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
                pArpStaticConfigLocalMessage->setCompletionStatus(WRC_ARP_INTERFACE_DOES_NOT_EXIST);

                return (WRC_ARP_INTERFACE_DOES_NOT_EXIST);
            }
            if (ARP_INVALID_IF_TYPE_ERROR == res) {
                trace (TRACE_LEVEL_INFO, "ArpConfigLocalWorker::updateArpStaticConfigMOStep. Error:ARP_INVALID_IF_TYPE_ERROR.");
                delete pArpStaticConfigLocalManagedObject;
                if (pResults) {
                    pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
                    delete pResults;
                }
                pArpStaticConfigLocalMessage->setCompletionStatus(WRC_ARP_INVALID_IF_TYPE_ERROR);

                return (WRC_ARP_INVALID_IF_TYPE_ERROR);
            }
            pArpStaticConfigLocalManagedObject = new ArpStaticConfigLocalManagedObject(dynamic_cast<ArpLocalObjectManager*>(getPWaveObjectManager()));
			prismAssert (NULL != pArpStaticConfigLocalManagedObject, __FILE__, __LINE__);
        }

        trace (TRACE_LEVEL_DEVEL, "ArpConfigLocalWorkercase Static MSG_ENABLE ");
        pArpStaticConfigLocalManagedObject->setIpAddress(pArpStaticConfigLocalMessage->getIpAddress());
        pArpStaticConfigLocalManagedObject->setMacAdress(pArpStaticConfigLocalMessage->getMacAdress());
        interfaceType = pArpStaticConfigLocalMessage->getInterfaceType();
        switch (interfaceType)
        {
            case CONFIG_ARP_PORTCHANNEL:
                pArpStaticConfigLocalManagedObject->setPortChannelValue(pArpStaticConfigLocalMessage->getPortChannelValue());
        		pArpStaticConfigLocalManagedObject->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;
                
            case CONFIG_ARP_GIGABIT:
                pArpStaticConfigLocalManagedObject->setGigabitValue(pArpStaticConfigLocalMessage->getGigabitValue());
        		pArpStaticConfigLocalManagedObject->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
				slot_id = getSlotFromIfName(interfaceType, pArpStaticConfigLocalMessage->getGigabitValue());
                break;
            
            case CONFIG_ARP_TENGIGABIT:
                pArpStaticConfigLocalManagedObject->setTenGigabitValue(pArpStaticConfigLocalMessage->getTenGigabitValue());
        		pArpStaticConfigLocalManagedObject->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
				slot_id = getSlotFromIfName(interfaceType, pArpStaticConfigLocalMessage->getTenGigabitValue());
                break;
            
            case CONFIG_ARP_FORTYGIGABIT:
                pArpStaticConfigLocalManagedObject->setFortyGigabitValue(pArpStaticConfigLocalMessage->getFortyGigabitValue());
        		pArpStaticConfigLocalManagedObject->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
				slot_id = getSlotFromIfName(interfaceType, pArpStaticConfigLocalMessage->getFortyGigabitValue());
                break;
            
            case CONFIG_ARP_VLAN:
                pArpStaticConfigLocalManagedObject->setVlanValue(pArpStaticConfigLocalMessage->getVlanValue());
        		pArpStaticConfigLocalManagedObject->setInterfaceType(pArpStaticConfigLocalMessage->getInterfaceType());
                break;
        }
		if (slot_id != (int32_t)WAVE_INVALID_SLOT) {
			pArpStaticConfigLocalManagedObject->associateWithSlot(slot_id);
		}

        if (pResults) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectsForGarbageCollection(*pResults);
            delete pResults;
        }
        if (createFlag) {
            pPrismSynchronousLinearSequencerContext->addManagedObjectForGarbageCollection (pArpStaticConfigLocalManagedObject);
        }

        pArpStaticConfigLocalMessage->setCompletionStatus(status);
        return pArpStaticConfigLocalMessage->getCompletionStatus();
    }
    
    void  ArpConfigLocalWorker::ArpConfigLocalMessageHandler( ArpConfigLocalMessage *pArpConfigLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        { 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::updateArpConfigStep), 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerStartTransactionStep), 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::updateArpConfigMOStep), 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep), 
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep) 
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pArpConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    ResourceId 
    ArpConfigLocalWorker::sendDebugToArpDaemon(PrismSynchronousLinearSequencerContext *ctx_p)
    {
        ArpDebugLocalMessage *msg_p = dynamic_cast<ArpDebugLocalMessage *>
                                      (ctx_p->getPPrismMessage ());
        ArpDebugClientMessage *cl_msg_p = NULL;
        ResourceId      status;

        trace(TRACE_LEVEL_DEBUG, "Entering ArpConfigLocalWorker::sendDebugToArpDaemon");

        if (! msg_p)
            return WAVE_MESSAGE_ERROR;

        cl_msg_p = new ArpDebugClientMessage(msg_p->getIpAddress());
		cl_msg_p->setNoFlag(msg_p->getNoFlag());

        status = sendSynchronouslyToWaveClient("arp", cl_msg_p);

        if (status == WAVE_MESSAGE_SUCCESS) {
            if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS) {
                trace(TRACE_LEVEL_ERROR, "Message send to Arpd Failed.");
                msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
                return msg_p->getCompletionStatus();
            }
        } else if (status != WAVE_MESSAGE_SUCCESS) {
            trace(TRACE_LEVEL_ERROR, "Message send to Arpd Failed.");
        }

        delete cl_msg_p;
        msg_p->setCompletionStatus(status);
        return msg_p->getCompletionStatus();
    }

    void  ArpConfigLocalWorker::ArpDebugLocalMessageHandler( ArpDebugLocalMessage *pArpDebugLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::sendDebugToArpDaemon),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pArpDebugLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ArpConfigLocalWorker::ArpResetLocalMessageHandler( ArpResetLocalMessage *pArpResetLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pArpResetLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    void  ArpConfigLocalWorker::ArpShowLocalMessageHandler( ArpShowLocalMessage *pArpShowLocalMessage)
    {
        if (pArpShowLocalMessage->getisDistributionRequired()) 
        {
            PrismLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismLinearSequencerStep> (&ArpConfigLocalWorker::sendArpShowToCommandToCluster),
                reinterpret_cast<PrismLinearSequencerStep> (&ArpConfigLocalWorker::prismLinearSequencerSucceededStep),
                reinterpret_cast<PrismLinearSequencerStep> (&ArpConfigLocalWorker::prismLinearSequencerFailedStep)
            };

            PrismLinearSequencerContext *pPrismLinearSequencerContext = new PrismLinearSequencerContext(pArpShowLocalMessage, this, sequencerSteps, sizeof(sequencerSteps) / sizeof(sequencerSteps[0]));
            pPrismLinearSequencerContext->start ();
            return;
        } else {
            PrismSynchronousLinearSequencerStep sequencerSteps[] =
            {
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ArpConfigLocalWorker::sendArpShowCommandToArpDaemon),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
                reinterpret_cast<PrismSynchronousLinearSequencerStep> (&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep)
            };

            PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext= new PrismSynchronousLinearSequencerContext(pArpShowLocalMessage , this, sequencerSteps,
                sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
            pPrismSynchronousLinearSequencerContext->execute ();

        }
    }

    void  ArpConfigLocalWorker::ArpStaticConfigLocalMessageHandler( ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::validateArpStaticConfigStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::updateArpStaticConfigMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&ArpConfigLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pArpStaticConfigLocalMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }

    /**
     * Validate whether the ArpStaticConfigLocalMessage contains valid data.
     */
    ResourceId ArpConfigLocalWorker::validateArpStaticConfigStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        trace(TRACE_LEVEL_DEBUG, "ArpConfigLocalWorker::validateArpStaticConfigStep called.");

        ResourceId status = WAVE_MESSAGE_SUCCESS;

        ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage = dynamic_cast<ArpStaticConfigLocalMessage*> (pPrismSynchronousLinearSequencerContext->getPPrismMessage ());

        // Check for pre-existence
        WaveManagedObjectSynchronousQueryContext syncQueryCtxt(ArpStaticConfigLocalManagedObject::getClassName());

        IpV4Address IpAddress = pArpStaticConfigLocalMessage->getIpAddress();
        syncQueryCtxt.addAndAttribute (new AttributeIpV4Address(&IpAddress,"IpAddress"));
        syncQueryCtxt.setPartitionFilter(pArpStaticConfigLocalMessage->getPartitionName(), FrameworkToolKit::getThisLocationId ());

        UI32 count;
        status = querySynchronouslyForCount(&syncQueryCtxt, count);

        // If the query failed, return immediately with the error status.
        if (status != WAVE_MESSAGE_SUCCESS) {
            pArpStaticConfigLocalMessage->setCompletionStatus(status);
            return status;
        }

        bool opCode = pArpStaticConfigLocalMessage->getOpCode();
        if (opCode == true) {
            // Delete operation
            if (count == 0) {
                // Nothing to delete.
                trace (TRACE_LEVEL_INFO, "ArpConfigLocalWorker::validateArpStaticConfigStep: Attempted delete of non-existing arp.");

                // TODO: We should jump to success step directly from here.
            }
            else if (count > 1) {
                // We should never reach here.
                trace (TRACE_LEVEL_ERROR, "ArpConfigLocalWorker::validateArpStaticConfigStep: more than one similar arp objects existing.");
                status = WAVE_MESSAGE_ERROR;
            }
        }
        else if (opCode == false) {
            // Create operation
            if (count == 1) {
                trace (TRACE_LEVEL_DEBUG, "ArpConfigLocalWorker::validateArpStaticConfigStep: arp already existing.");
                status = WRC_ARP_STATIC_EXISTS;
            }
            else if (count > 1) {
                // We should never reach here.
                trace (TRACE_LEVEL_ERROR, "ArpConfigLocalWorker::validateArpStaticConfigStep: more than one similar arp objects pre-existing.");
                status = WAVE_MESSAGE_ERROR;
            }
        }

        // Validate correctness of interface, if the operation is CREATE.
        if (opCode == false) {
            validateIfName(pArpStaticConfigLocalMessage);
        }

        pArpStaticConfigLocalMessage->setCompletionStatus(status);
        return status;
    }

    /**
     * Checks whether the interfaces set in the passed message is valid
     *
     * return "WAVE_MESSAGE_SUCCESS" if interface found. Otherwise returns an error code.
     */
    ResourceId ArpConfigLocalWorker::validateIfName(ArpStaticConfigLocalMessage *pArpStaticConfigLocalMessage) {
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        UI32 interfaceType = pArpStaticConfigLocalMessage->getInterfaceType();

        if (interfaceType == CONFIG_ARP_GIGABIT) {
            if (! NsmUtils::isValidInterface(pArpStaticConfigLocalMessage->getGigabitValue(), IF_TYPE_GI) ) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_ONEGIGE;
            }
        } else if (interfaceType == CONFIG_ARP_TENGIGABIT) {
            if (! NsmUtils::isValidInterface(pArpStaticConfigLocalMessage->getTenGigabitValue(), IF_TYPE_TE) ) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_TENGIGE;
            }
        } else if (interfaceType == CONFIG_ARP_FORTYGIGABIT) {
            if (! NsmUtils::isValidInterface(pArpStaticConfigLocalMessage->getFortyGigabitValue(), IF_TYPE_FO)) {
                status = WRC_NSM_ERR_PHYINTF_INVALID_FORTYGIGE;
            }
        } else if (interfaceType == CONFIG_ARP_VLAN) {
            // For vlan no validity check is required.
            // TOD0: should we check whether the passed number falls with possible range for vlan.
            status = WAVE_MESSAGE_SUCCESS;
        } else {
            // We should never reach here.
            trace(TRACE_LEVEL_ERROR, "ArpConfigLocalWorker::validateIfName: invalid interface = " + interfaceType);
            status = WRC_ARP_INVALID_IF_TYPE_ERROR;
        }

        return status;
    }

    void ArpConfigLocalWorker::sendArpShowToCommandToCluster(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");

        ArpShowLocalMessage *msg_p = dynamic_cast<ArpShowLocalMessage*> (seq_ctx_p->getPPrismMessage ()); 
        PLUG_ASSERT_PTR(msg_p);

        ArpShowLocalMessage *loc_msg_p = new ArpShowLocalMessage();
        PLUG_ASSERT_PTR(loc_msg_p);
        loc_msg_p->setisDistributionRequired(false);
        loc_msg_p->setPartitionName(msg_p->getPartitionName());

        WaveSendToClusterContext *send_ctx_p = new WaveSendToClusterContext(this, reinterpret_cast<PrismAsynchronousCallback> (&ArpConfigLocalWorker::ArpShowCmdClusterCallback), seq_ctx_p);
        PLUG_ASSERT_PTR(send_ctx_p);

        send_ctx_p->setPPrismMessageForPhase1(loc_msg_p);
        if(msg_p->m_locs.size() > 0) {
            PLUG_DBG_S(string("setting locationids"));      
            send_ctx_p->setLocationsToSendToForPhase1(msg_p->m_locs);                 
        }
        sendToWaveCluster(send_ctx_p);
        return;
    }

    ResourceId ArpConfigLocalWorker::sendArpShowCommandToArpDaemon(PrismLinearSequencerContext *seq_ctx_p)
    {
        PLUG_DBG("Entering ...");
        ArpShowLocalMessage *msg_p = dynamic_cast<ArpShowLocalMessage*> (seq_ctx_p->getPPrismMessage ());    
        ArpShowMessage  *m = new ArpShowMessage(msg_p);
        m->setPartitionName(msg_p->getPartitionName());

        PLUG_INFO_S(string("vrf name = ") + m->getPartitionName());
        PLUG_INFO_S(string("if Name = ") + m->getIfName() + "cmd code = " + m->getCmdCode());
        ResourceId status = sendSynchronouslyToWaveClient ("arp",m);

        if(WAVE_MESSAGE_SUCCESS != status)
        {
            PLUG_DBG_S(string("sendSynchronouslyToWaveClient Failed") + status);
            delete m;
            return status;
        }
        msg_p->copyAllBuffers(*m);
        //bpatel: To avoid memory leak
        if(m){
            delete m;
        }
        return status;
    }

    void ArpConfigLocalWorker::ArpShowCmdClusterCallback(WaveSendToClusterContext *ctx_p)
    {
        PLUG_DBG("Entering...");
        PrismLinearSequencerContext            *seq_ctx_p = NULL;
        ArpShowLocalMessage *req_msg_p = NULL;
        ArpShowLocalMessage *resp_msg_p = NULL;
        ArpShowLocalMessage *sw_resp_p = NULL;


        seq_ctx_p = reinterpret_cast<PrismLinearSequencerContext *> (ctx_p->getPCallerContext());

        PLUG_ASSERT_PTR(seq_ctx_p);

        req_msg_p = dynamic_cast<ArpShowLocalMessage*> (seq_ctx_p->getPPrismMessage ());

        resp_msg_p =  dynamic_cast<ArpShowLocalMessage*> (ctx_p->getPPrismMessageForPhase1());

        PLUG_ASSERT_PTR(req_msg_p);
        PLUG_ASSERT_PTR(resp_msg_p);
        //req_msg_p->copyBuffersFrom(*resp_msg_p);

        vector<LocationId> locations =  ctx_p->getLocationsToSendToForPhase1();
        UI32    i = 0;

        PLUG_DBG_S(string("Response processing: Num resp:") + locations.size());

        
        for (i = 0; i < locations.size(); i++)
        {
            sw_resp_p = dynamic_cast<ArpShowLocalMessage*> (ctx_p->getResultingMessageForPhase1(locations[i]));
            PLUG_ASSERT_PTR(sw_resp_p);
            if (WAVE_MESSAGE_SUCCESS == sw_resp_p->getCompletionStatus())
            {
                req_msg_p->copyAllBuffers(*sw_resp_p);
            } 
            else 
            {
                PLUG_DBG_S((string("Response processing: The location: ") + locations[i] ) );
                PLUG_DBG_S (string("returned error: ")  + FrameworkToolKit::localize(sw_resp_p->getCompletionStatus()));
            }
        }
        seq_ctx_p->executeNextStep(WAVE_MESSAGE_SUCCESS);

        delete resp_msg_p;
        delete ctx_p;
        return;

   }

}
