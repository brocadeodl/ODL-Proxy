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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMVnmLocalWorker.h"
#include "APPM/Local/APPMVnmLocalCdpNotifyMessage.h"
#include "APPM/Local/APPMLocalTypes.h"
#include "Nsm/Global/NsmGlobalUpdatePoIntfMessage.h"
#include "Nsm/Local/NsmUpdatePhyIntfMessage.h"
#include "Nsm/Local/Layer3/NsmLocalPhyIpMessage.h"
#include "ClientInterface/APPM/appm_cdp_def.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "WyserEaGateway/Virtual/sq.h"
#include "DcmCStatus.h"

#include <raslog/raslog.h>
#include <raslog/raslog_php.h>

namespace DcmNs
{

    APPMVnmLocalWorker::APPMVnmLocalWorker (APPMLocalObjectManager *pAPPMLocalObjectManager)
        : WaveWorker  (pAPPMLocalObjectManager)
    {
        addOperationMap (APPMLOCALVNMCDPNOTIFY, reinterpret_cast<PrismMessageHandler> (&APPMVnmLocalWorker::APPMVnmLocalCdpNotifyMessageHandler));
    }

    APPMVnmLocalWorker::~APPMVnmLocalWorker ()
    {
    }

    PrismMessage  *APPMVnmLocalWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case APPMLOCALVNMCDPNOTIFY :
                pPrismMessage = new APPMVnmLocalCdpNotifyMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *APPMVnmLocalWorker::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    /* Put Port-channel into port-profile-port mode */
    ResourceId APPMVnmLocalWorker::putPoIntfToPortProfilePort(string poIfName)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;

        /* Check if port is already profiled */
        status = NsmUtils::getIntfPortProfiled(poIfName, PO_INTF_TYPE);
        if (status == WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Port is already in profiled mode"));
            return WAVE_MESSAGE_SUCCESS; 
        }
        
        /* WRC_NSM_ERR_DCM_APPM_PROFILED_PORT indicates that port can be put
         * into port-profile-port. Throw error for anything other than this.
         */
        if (status != WRC_NSM_ERR_DCM_APPM_PROFILED_PORT)
            return WAVE_MESSAGE_ERROR; 
        
        NsmGlobalUpdatePoIntfMessage *pMessage = new NsmGlobalUpdatePoIntfMessage();
        pMessage->setPoId(poIfName);
        pMessage->setPoName(poIfName);
        pMessage->setCmdCode(NSMUPDATE_PO_PORT_PROFILED);
        pMessage->setIsPortProfiled(true);
        
        status = sendSynchronously (pMessage);
        
        if (WAVE_MESSAGE_SUCCESS == status) {
            status = pMessage->getCompletionStatus();
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                    string("Completion Status of port-profile-port: ") + 
                    FrameworkToolKit::localize(status));
        }
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                FrameworkToolKit::localize(status));

        delete pMessage;
        return (status);
    }
    
    /* Put Physical interface into port-profile-port mode */
    ResourceId APPMVnmLocalWorker::putPhyIntfToPortProfilePort(string ifName, InterfaceType type)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        /* Check if port is already profiled */
        status = NsmUtils::getIntfPortProfiled(ifName, PHY_INTF_TYPE);
        if (status == WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Port is already in profiled mode"));
            return WAVE_MESSAGE_SUCCESS; 
        }
        
        /* WRC_NSM_ERR_DCM_APPM_PROFILED_PORT indicates that port can be put into port-profile-port
         * Throw error for anything other than this.
         */
        if (status != WRC_NSM_ERR_DCM_APPM_PROFILED_PORT)
            return WAVE_MESSAGE_ERROR; 
        
        NsmUpdatePhyIntfMessage *pMessage = new NsmUpdatePhyIntfMessage();
        pMessage->setIfName(ifName);
        pMessage->setIfType(type);
        pMessage->setIsPortProfiled(true);
        pMessage->setCmdCode(NSMUPDATE_PHY_PORT_PROFILED);
        
        NsmLocalPhyIpMessage *pPhyIpMsg = new NsmLocalPhyIpMessage();
        pPhyIpMsg->setIfName(ifName);
        pPhyIpMsg->setIfType(type);
        pPhyIpMsg->setCmdCode(NSM_MSG_CMD_L3READY_MO);
        pPhyIpMsg->setOpCode(NSM_MSG_OP_DELETE);
        
        status = sendSynchronously (pMessage);

        if (WAVE_MESSAGE_SUCCESS == status) {
            status = pMessage->getCompletionStatus();
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Completion Status of port-profile-port: ") + 
                    FrameworkToolKit::localize(status));
            if (status == WAVE_MESSAGE_SUCCESS) {
                ResourceId statusIpMsg;
                statusIpMsg = sendSynchronously(pPhyIpMsg);
                if (statusIpMsg == WAVE_MESSAGE_SUCCESS) {
                    statusIpMsg = pPhyIpMsg->getCompletionStatus();
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Completion Status Ip Message: ") + 
                            FrameworkToolKit::localize(status));
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Failed"));
                }
            }
        }
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + FrameworkToolKit::localize(status));
        delete pMessage;
        delete pPhyIpMsg;
        return (status);
    }
    
    ResourceId APPMVnmLocalWorker::putIntfToPortProfilePort(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string ifName = pAPPMVnmLocalCdpNotifyMessage->getIfName();
        string hostMor = pAPPMVnmLocalCdpNotifyMessage->getHostMor();
        string portId = pAPPMVnmLocalCdpNotifyMessage->getPortId();
        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
        string rasIfName;
        string name(ifName);
        name = name.substr(3);
        
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("name: ") + name);
        
        /* Check if the interface is part of port-channel */
        status = NsmUtils::getPhyMoByPhyName(name, pPhyIntfLocalManagedObject);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (pPhyIntfLocalManagedObject != NULL) {
                if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId) {
                    /* interface is part of port-channel */
                    string poIfName = NsmUtils::getPoIntfNameByObjectId(
                            pPhyIntfLocalManagedObject->getPoId());
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("poIfName: ") + poIfName);
                    status = putPoIntfToPortProfilePort(poIfName);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                            FrameworkToolKit::localize(status));
                    rasIfName = "Po " + poIfName;
                } else {
                    /* Interface is not part of port-channel */
                    InterfaceType type = NsmUtils::getIntfTypeFromName(ifName);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("type: ") + type);
                    status = putPhyIntfToPortProfilePort(name, type);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                            FrameworkToolKit::localize(status));
                    rasIfName = ifName;
                }
                
                if (status != WAVE_MESSAGE_SUCCESS) {
                    int retry = 0;
                    int ret = SQ_init();
                    do {
                        retry++;
                        if (retry == 3) {
                            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                                    string("Error on SQLite DB update"));
                            status = WAVE_MESSAGE_ERROR;
                            break;
                        }
                        ret = SQ_updateHostRecord((char *)hostMor.c_str(), 
                                (char *)portId.c_str(), (char *)"", 0);
                    }  while (!ret);
                    
                    if (ret) {
                        string rasMsg("port-profile-port configuration failed due to "
                                "conflicting configuration on interface " + rasIfName);
                        raslog_ext(__FILE__, __FUNCTION__, __LINE__, RASLOG_OID_INVALID, 
                                RASLOG_NOFLAGS, PHP_1003, rasMsg.c_str());
                    }
                    SQ_closedb();
                }
                delete pPhyIntfLocalManagedObject;
            } else {
                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                        string("Counld not find the pPhyIntfLocalManagedObject"));
                status = WAVE_MESSAGE_ERROR;
            }
        } else {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                    string("PhyIntfLocalManagedObject is NULL"));
            status = WAVE_MESSAGE_ERROR;
        }
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                FrameworkToolKit::localize(status));
        return (status);
    }
    
    /* Make SQL query and check if interface is receiving CDP frames */
    ResourceId APPMVnmLocalWorker::isIntfReceiveCdp(string ifName, bool &isMemberHasCdp)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        char *table = (char *) "vmnics";
        char *cols = (char *)"iface";
        char *where = (char *)"iface IS NOT NULL AND iface != ''";
        char *items[1] = {NULL};
        char *name = NULL;
        int ret = 0;
        vector<string> ifNameInDb;
        isMemberHasCdp = false;
        
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Looking for intf: ") + ifName);
        
        ret = SQ_init();
        if (!ret) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Init Failed Ret: ") + ret);
            return WAVE_MESSAGE_ERROR;
        }
        
        /* Get ALL interface names which are receiving CDP frames */
        do {
            ret = SQ_getRow(table, cols, where, items, 1, NO_INDEX_IS_SET);
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("ret: ") + ret);
            if (ret) {
                if (items[0] != NULL) {
                    name = items[0] + 3;
                    ifNameInDb.push_back(name);
                    free(items[0]);
                    items[0] = NULL;
                } else {
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("ifName: null"));
                }
            }
        } while(ret);

        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("ifNames in DB, Count: ") + ifNameInDb.size());

        /* Check if the input ifName is one among the CDP receiving intf */
        for (UI32 i = 0; i != ifNameInDb.size(); i++) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("ifNames in DB: ") + ifNameInDb[i]);
            if (ifNameInDb[i] == ifName) {
                APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Match found for: ") + ifName);
                isMemberHasCdp = true;
                break;
            }
        }
        
        SQ_closedb();
        return (status);
    }
   
    /* Remove Port-channel from port-profile-port */
    ResourceId APPMVnmLocalWorker::removePoIntfFromPortProfilePort(string poIfName)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("poIfName: ") + poIfName);
        
       /* Return if profiled is already not profiled */
        status = NsmUtils::getIntfPortProfiled(poIfName, PO_INTF_TYPE);
        if (status == WRC_NSM_ERR_DCM_APPM_PROFILED_PORT) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Port is already not in profiled mode"));
            return WAVE_MESSAGE_SUCCESS; 
        }

        /* WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED indicates that port is in 
         * port-profile-port. Throw error for anything other than this.
         */
        if (status != WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED)
            return WAVE_MESSAGE_ERROR; 

        /* Check if another port, which is a member, is receiving cdp frame */
        ObjectId poOid;
        bool isMemberHasCdp = false;
        NsmUtils::getPoIntfObjectIdByPoIfName(poIfName, poOid, false);
        vector<PhyIntfLocalManagedObject *> vpPhyIntfLocalManagedObject;
        status = NsmUtils::GetPhyMoByPoOid(poOid, vpPhyIntfLocalManagedObject);
        if (status != WAVE_MESSAGE_SUCCESS) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                                        FrameworkToolKit::localize(status));
            return status;
        }
        
        for (unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++) {
            string ifName = vpPhyIntfLocalManagedObject[i]->getIfName();
            status = isIntfReceiveCdp(ifName, isMemberHasCdp);
            if ((status != WAVE_MESSAGE_SUCCESS) || isMemberHasCdp)
                break;
        }	
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string(" CAlling is IntfShut "));

		bool shut = NsmUtils::isIntfShut(PO_INTF_TYPE,poIfName);
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string(" CAlling is IntfShut ")+shut);

 
        /* Free memory */
        for(unsigned int i = 0;i< vpPhyIntfLocalManagedObject.size(); i++) {
            delete vpPhyIntfLocalManagedObject[i];
        }	
        
        if (status != WAVE_MESSAGE_SUCCESS) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + 
                                        FrameworkToolKit::localize(status));
            return status;
        }

        /* Avoiding removing port-profile-port config from the PO interface,
         * if member port is receiving CDP
         */
        if (isMemberHasCdp) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("CDP on member port")); 
            return WAVE_MESSAGE_SUCCESS;
        }
        
        NsmGlobalUpdatePoIntfMessage *pMessage = new NsmGlobalUpdatePoIntfMessage();
        pMessage->setPoId(poIfName);
        pMessage->setPoName(poIfName);
        pMessage->setCmdCode(NSMUPDATE_PO_PORT_PROFILED);
        pMessage->setIsPortProfiled(false);
        
        /* Put intf into "no shut" as previous config made it to "shut" */
        NsmGlobalUpdatePoIntfMessage *m = new NsmGlobalUpdatePoIntfMessage();
        m->setPoId(poIfName);
        m->setPoName(poIfName);
        m->setFlag(false);
        m->setCmdCode(NSMUPDATE_PO_DELSTATUS);
        
        status = sendSynchronously (pMessage);
        
        if (WAVE_MESSAGE_SUCCESS == status) { 
            status = pMessage->getCompletionStatus();
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                    string("Completion Status of no port-profile-port: ") +
                    FrameworkToolKit::localize(status));
            if (status == WAVE_MESSAGE_SUCCESS) {
				if (shut == false)
                	status = sendSynchronously(m);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    status = m->getCompletionStatus();
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                            string("Completion Status no shut Message: ") +
                            FrameworkToolKit::localize(status));
                }
            }
        }

        delete pMessage;
        delete m;
        return (status);
    }
    
    /* Remove Physical Interface from port-profile-port */
    ResourceId APPMVnmLocalWorker::removePhyIntfFromPortProfilePort(string ifName, InterfaceType type)
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;

       /* Return if profiled is already not profiled */
        status = NsmUtils::getIntfPortProfiled(ifName, PHY_INTF_TYPE);
        if (status == WRC_NSM_ERR_DCM_APPM_PROFILED_PORT) {
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("Port is already not in profiled mode"));
            return WAVE_MESSAGE_SUCCESS;
        }

        /* WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED indicates that port is in 
         * port-profile-port. Throw error for anything other than this.
         */
        if (status != WRC_NSM_ERR_DCM_APPM_PORT_IS_PROFILED)
            return WAVE_MESSAGE_ERROR;

        NsmUpdatePhyIntfMessage *pMessage = new NsmUpdatePhyIntfMessage();
        pMessage->setIfName(ifName);
        pMessage->setIfType(type);
        pMessage->setIsPortProfiled(false);
        pMessage->setCmdCode(NSMUPDATE_PHY_PORT_PROFILED);
                
        NsmLocalPhyIpMessage *pPhyIpMsg = new NsmLocalPhyIpMessage();
        pPhyIpMsg->setIfName(ifName);
        pPhyIpMsg->setIfType(type);
        pPhyIpMsg->setCmdCode(NSM_MSG_CMD_L3READY_MO);
        pPhyIpMsg->setOpCode(NSM_MSG_OP_CREATE);
        
        /* Put intf into "no shut" as previous config made it to "shut" */
        NsmUpdatePhyIntfMessage *m = new NsmUpdatePhyIntfMessage();
        m->setIfName(ifName);
        m->setFlag(false);
        m->setCmdCode(NSMUPDATE_PHY_DELSTATUS);
        /* By-Pass the tracking of Configuration Change */
        m->setIsConfigurationChange (false);
       
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string(" CAlling is IntfShut "));

		bool shut = NsmUtils::isIntfShut(PHY_INTF_TYPE, ifName);
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string(" CAlling is IntfShut ")+shut);
        status = sendSynchronously (pMessage);
        
        if (WAVE_MESSAGE_SUCCESS == status) {   
            status = pMessage->getCompletionStatus();
            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                    string("Completion Status of no port-profile-port: ") +
                    FrameworkToolKit::localize(status));
            if (status == WAVE_MESSAGE_SUCCESS) {
                status = sendSynchronously(pPhyIpMsg);
                if (status == WAVE_MESSAGE_SUCCESS) {
                    status = pPhyIpMsg->getCompletionStatus();
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                            string("Completion Status Ip Message: ") +
                            FrameworkToolKit::localize(status));
                    if (status == WAVE_MESSAGE_SUCCESS) {
						if (shut == false)
                        	status = sendSynchronously (m);
                        if (WAVE_MESSAGE_SUCCESS == status) {   
                            status = m->getCompletionStatus();
                            APPM_PLUG_TRACE(TRACE_LEVEL_INFO, 
                                string("Completion Status of no shutdown: ") +
                                FrameworkToolKit::localize(status));
                        }
                    }
                }
            }
        }

        delete pMessage;
        delete pPhyIpMsg;
        delete m;
        return (status);
    }
    
    ResourceId APPMVnmLocalWorker::removeIntfFromPortProfilePort(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage)
    {
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        string ifName = pAPPMVnmLocalCdpNotifyMessage->getIfName();
        string hostMor = pAPPMVnmLocalCdpNotifyMessage->getHostMor();
        string portId = pAPPMVnmLocalCdpNotifyMessage->getPortId();
        PhyIntfLocalManagedObject* pPhyIntfLocalManagedObject = NULL;
        string name(ifName);
        name = name.substr(3);
        
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("name: ") + name);
        
        /* Check if the interface is part of port-channel */
        status = NsmUtils::getPhyMoByPhyName(name, pPhyIntfLocalManagedObject);
        if (status == WAVE_MESSAGE_SUCCESS) {
            if (pPhyIntfLocalManagedObject != NULL) {
                if (pPhyIntfLocalManagedObject->getPoId() != ObjectId::NullObjectId) {
                    /* interface is part of port-channel */
                    string poIfName = NsmUtils::getPoIntfNameByObjectId(
                            pPhyIntfLocalManagedObject->getPoId());
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("poIfName: ") + poIfName);
                    status = removePoIntfFromPortProfilePort(poIfName);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + FrameworkToolKit::localize(status));
                } else {
                    /* Interface is not part of port-channel */
                    InterfaceType type = NsmUtils::getIntfTypeFromName(ifName);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("type: ") + type);
                    status = removePhyIntfFromPortProfilePort(name, type);
                    APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + FrameworkToolKit::localize(status));
                }
                delete pPhyIntfLocalManagedObject;
            }
        } else {
            status = WAVE_MESSAGE_ERROR;
        }
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("status: ") + FrameworkToolKit::localize(status));
        return (status);
    }
    
    ResourceId APPMVnmLocalWorker::handleCdpNotification(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext)
    {
        ResourceId status = WAVE_MESSAGE_SUCCESS;
        APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage =
            dynamic_cast<APPMVnmLocalCdpNotifyMessage *> (pPrismSynchronousLinearSequencerContext->getPPrismMessage());
        
        APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);

        UI32 cmdCode = pAPPMVnmLocalCdpNotifyMessage->getCmdCode();
        string ifName = pAPPMVnmLocalCdpNotifyMessage->getIfName();
        string hostMor = pAPPMVnmLocalCdpNotifyMessage->getHostMor();
        string portId = pAPPMVnmLocalCdpNotifyMessage->getPortId();
        
        APPM_PLUG_TRACE(TRACE_LEVEL_INFO, string("cmdCode: ") + cmdCode + 
                string(" ifName: ") + ifName + string(" hostMor: ") + hostMor + 
                string(" portId: ") + portId);
        
        if (cmdCode == VNM_CDP_RECEIVED) {
            status = putIntfToPortProfilePort(pAPPMVnmLocalCdpNotifyMessage);
        } else if (cmdCode == VNM_CDP_STOPPED) {
            status = removeIntfFromPortProfilePort(pAPPMVnmLocalCdpNotifyMessage);
        } else {
            status = WAVE_MESSAGE_ERROR;
        }

		APPM_PLUG_TRACE_FUNC_EXIT(TRACE_LEVEL_INFO);
        return (status);
    }

    void  APPMVnmLocalWorker::APPMVnmLocalCdpNotifyMessageHandler(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage)
    {
		APPM_PLUG_TRACE_FUNC_ENTRY(TRACE_LEVEL_INFO);
        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
			reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMVnmLocalWorker::handleCdpNotification),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMVnmLocalWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&APPMVnmLocalWorker::prismSynchronousLinearSequencerFailedStep)
        };

        PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext = new PrismSynchronousLinearSequencerContext (pAPPMVnmLocalCdpNotifyMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pPrismSynchronousLinearSequencerContext->execute ();
    }


}
