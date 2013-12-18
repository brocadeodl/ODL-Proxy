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
 *   Author : krao                                                     *
 **************************************************************************/

#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/PhyIntfLocalManagedObject.h"
#include "Nsm/Local/Layer3/NsmL3ReadyManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPLocalConfWorker.h"
#include "Nsm/Local/VRRPWorker/NsmUpdateVRRPLocalConfigMessage.h"
#include "Nsm/Local/VRRPWorker/NsmUpdateVRRPLocalConfigPhyIntfMessage.h"
#include "ClientInterface/VRRP/VRRPShowMessage.h"
#include "Nsm/Local/VRRPWorker/VRRPLocalShowMessage.h"
#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPSessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPESessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPVcsSessionConfigLocalManagedObject.h"
#include "Nsm/Local/NsmTypes.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"
#include "ClientInterface/Nsm/nsmdIpIntf.h"
#include "ClientInterface/L2sys/L2sysUtils.h"

namespace DcmNs
{

    VRRPLocalConfWorker::VRRPLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager)
        : WaveWorker  (pNsmLocalObjectManager)
    {
        VRRPVirtualIPLocalManagedObject    VRRPVirtualIPLocalManagedObject    (pNsmLocalObjectManager);
        VRRPTrackPortLocalManagedObject    VRRPTrackPortLocalManagedObject    (pNsmLocalObjectManager);
        VRRPSessionConfigLocalManagedObject    VRRPSessionConfigLocalManagedObject    (pNsmLocalObjectManager);
        VRRPESessionConfigLocalManagedObject    VRRPESessionConfigLocalManagedObject    (pNsmLocalObjectManager);
        VRRPVcsSessionConfigLocalManagedObject    VRRPVcsSessionConfigLocalManagedObject    (pNsmLocalObjectManager);
        VRRPVirtualIPLocalManagedObject.setupOrm ();
        addManagedClass (VRRPVirtualIPLocalManagedObject::getClassName (), this);
        VRRPTrackPortLocalManagedObject.setupOrm ();
        addManagedClass (VRRPTrackPortLocalManagedObject::getClassName (), this);
        VRRPSessionConfigLocalManagedObject.setupOrm ();
        addManagedClass (VRRPSessionConfigLocalManagedObject::getClassName (), this);
        VRRPESessionConfigLocalManagedObject.setupOrm ();
        addManagedClass (VRRPESessionConfigLocalManagedObject::getClassName (), this);
        VRRPVcsSessionConfigLocalManagedObject.setupOrm ();
        addManagedClass (VRRPVcsSessionConfigLocalManagedObject::getClassName (), this);
        addOperationMap (NSMUPDATEVRRPLOCALCONFIG, reinterpret_cast<PrismMessageHandler> (&VRRPLocalConfWorker::NsmUpdateVRRPLocalConfigMessageHandler));
        addOperationMap (NSMUPDATEVRRPLOCALCONFIGPHYINTF, reinterpret_cast<PrismMessageHandler> (&VRRPLocalConfWorker::NsmUpdateVRRPLocalConfigPhyIntfMessageHandler));
        addOperationMap (VRRP_LOCAL_SHOW_MSG, reinterpret_cast<PrismMessageHandler> (&VRRPLocalConfWorker::VRRPLocalShowMessageHandler));
    }

    VRRPLocalConfWorker::~VRRPLocalConfWorker ()
    {
    }

    PrismMessage  *VRRPLocalConfWorker::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            case NSMUPDATEVRRPLOCALCONFIG :
                pPrismMessage = new NsmUpdateVRRPLocalConfigMessage ();
                break;
            case NSMUPDATEVRRPLOCALCONFIGPHYINTF :
                pPrismMessage = new NsmUpdateVRRPLocalConfigPhyIntfMessage ();
                break;
            case VRRP_LOCAL_SHOW_MSG :
                pPrismMessage = new VRRPLocalShowMessage ();
                break;
            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPLocalConfWorker::createManagedObjectInstance(const string &managedClassName)
    {
        WaveManagedObject *pWaveManagedObject = NULL;

        if ((VRRPVirtualIPLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPVirtualIPLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VRRPTrackPortLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPTrackPortLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VRRPSessionConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPSessionConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VRRPESessionConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPESessionConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else if ((VRRPVcsSessionConfigLocalManagedObject::getClassName ()) == managedClassName)
        {
            pWaveManagedObject = new VRRPVcsSessionConfigLocalManagedObject(dynamic_cast<NsmLocalObjectManager *>(getPWaveObjectManager()));
        }
        else
        {
            trace (TRACE_LEVEL_FATAL, "VRRPLocalConfWorker::createManagedObjectInstance : Unknown Managed Class Name : " + managedClassName);
        }
        return (pWaveManagedObject);
    }

    void  VRRPLocalConfWorker::NsmUpdateVRRPLocalConfigMessageHandler( NsmUpdateVRRPLocalConfigMessage *pNsmUpdateVRRPLocalConfigMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmUpdateVRRPLocalConfigMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceSynchronousLinearSeqContext->execute ();
    }

	int VRRPLocalConfWorker::mapBackEndErrorsToDcmdErrors(int err)
	{
		switch(err)
		{
			case 0: return WAVE_MESSAGE_SUCCESS;
			case -1: return WAVE_MESSAGE_ERROR;
			default:
				return err;
		}
	}

    ResourceId VRRPLocalConfWorker::validateVRRPLocalConfigCommon (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext,
        NsmL3ReadyManagedObject *&pL3ReadyMo, UI32 &choice, UI32 &op_type)
    {
        string ifName;
        InterfaceType ifType;
        SI32 mapId = DcmToolKit::getLocalMappedId();

        trace (TRACE_LEVEL_DEBUG, string ("VRRPLocalConfWorker: Entering validateVRRPLocalConfigStep"));
        
        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

 		pL3ReadyMo = NULL;
        ifName = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfName();
        ifType = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType();

		if ((ifType == IF_TYPE_GI) || (ifType == IF_TYPE_TE) || (ifType == IF_TYPE_FO)) {
            PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject
                = NsmL3Util::PhyIfNameGetPhyIfLocalMO(ifName, pDceSynchronousLinearSeqContext);
			bool isISLPort = (DcmToolKit::isVcsEnabled () == WAVE_PERSISTENCE_CLUSTER_ENABLED)
					&& (pPhyIntfLocalManagedObject->getPortrole());
			pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pPhyIntfLocalManagedObject);
			if(true == isISLPort) {
				trace(TRACE_LEVEL_DEBUG, string("VRRPLocalConfWorker::validateVRRPLocalConfigStep:")+ifName+" is ISL Port. "
						"No L3 Config permitted");
				return WRC_NSM_API_ISL_INTERFACE_COMMAND_ERROR;
			}

			pL3ReadyMo = (WaveManagedObjectPointer<NsmL3ReadyManagedObject>(pPhyIntfLocalManagedObject->getL3Ready())).operator->();
			pDceSynchronousLinearSeqContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo);
		} else if (ifType == IF_TYPE_VL) {
			UI32 vlan_id = strtoul(ifName.c_str(),NULL,0);
			pL3ReadyMo = NsmL3Util::VlanIdGetL3ReadyMO(vlan_id, mapId, pDceSynchronousLinearSeqContext, false);
			pDceSynchronousLinearSeqContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo, true);
		} else {
			UI32 po_id = strtoul(ifName.c_str(),NULL,0);
			pL3ReadyMo = NsmL3Util::PoIdGetL3ReadyMO(po_id, mapId, pDceSynchronousLinearSeqContext, false);
			pDceSynchronousLinearSeqContext->getCache()->put(NsmL3ReadyManagedObject::getClassName(), pL3ReadyMo, true);
		}

		if (pL3ReadyMo == NULL) {
			trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: L3 Ready MO does not exist");
			return WRC_VRRP_API_SET_ERR_L3_NOT_READY;
		} 

        choice = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice();
        op_type = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getOp_type();

        return WAVE_MESSAGE_SUCCESS;
    }

    template <class VRRPSessionConfigLocalManagedObjectClass,
              WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObjectClass> (NsmL3ReadyManagedObject::*pGetVRRPIntfSessionFunc)(UI8 vrid)>
    ResourceId VRRPLocalConfWorker::validateVRRPLocalConfigStepTemplate (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
 		NsmL3ReadyManagedObject *pL3ReadyMo = NULL;
        VRRPSessionConfigLocalManagedObjectClass *pVRRPSessionConfigLocalManagedObject = NULL;

        trace (TRACE_LEVEL_DEBUG, string ("VRRPLocalConfWorker: Entering validateVRRPLocalConfigStep"));
        
        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        UI32 choice, op_type;
        ResourceId status = validateVRRPLocalConfigCommon (pDceSynchronousLinearSeqContext, pL3ReadyMo, choice, op_type);

        if (WAVE_MESSAGE_SUCCESS != status)
            return status;

        /* Get a pointer to the VRRP MO if it exists */
        pVRRPSessionConfigLocalManagedObject = (WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObjectClass>((pL3ReadyMo->*pGetVRRPIntfSessionFunc)(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid()))).operator->();
        if (NULL == pVRRPSessionConfigLocalManagedObject) {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::validateVRRPLocalConfigStep:") + string(" VRRP MO does not exist"));

            if (VRRP_OP_DELETE == op_type) {
                trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Delete for nonexistent object");
                // To prevent a request to delete a nonexistent config from causing any issues (which is needed for
                // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
            }
            else if(VRRP_SET_VRID != choice)
                status = WRC_VRRP_API_SET_ERR_VRRP_NOT_CONFIGURED;
        } else {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::validateVRRPLocalConfigStep:") + string(" VRRP MO exists"));

            if (VRRP_SET_VRID == choice) {
                trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Create for existing object");
                // To prevent a request to create an already-existing config from causing any issues (which is needed for
                // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                return WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
            }

            prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
            switch (choice) {
                case NO_VRRP_ENABLE:
                {
                    trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker::validateVRRPLocalConfigStep case NO_VRRP_ENABLE");
                    if (!pVRRPSessionConfigLocalManagedObject->getEnable()) {
                        trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Delete for nonexistent object");
                        // To suppress a backend warning when disabling an already-disabled group (which results from
                        // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                        status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                    }
                }
                break;

                case VRRP_VIP:
                case NO_VRRP_VIP:
                {
                    trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker::validateVRRPLocalConfigStep case VRRP_VIP/NO_VRRP_VIP");
                    VRRPVirtualIPLocalManagedObject *pVirtualIPMO = (WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject>(pVRRPSessionConfigLocalManagedObject->searchVirtualIPAddr(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip()))).operator->();

                    if (pVirtualIPMO == NULL) {
                        trace(TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::validateVRRPLocalConfigStep virtual IP does not exist");

                        if (NO_VRRP_VIP == choice) {
                            trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Delete for nonexistent VIP object");
                            // To prevent a request to delete a nonexistent config from causing any issues (which is needed for
                            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                        }
                    } else {
                        trace(TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::validateVRRPLocalConfigStep virtual IP already exists");

                        if (VRRP_VIP == choice) {
                            trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Create for existing VIP object");
                            // To prevent a request to create an already-existing config from causing any issues (which is needed for
                            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                        }
                    }
                }
                break;

                case VRRP_TRACK:
                case NO_VRRP_TRACK:
                {
                    trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker::validateVRRPLocalConfigStep case VRRP_TRACK/NO_VRRP_TRACK");

                    VRRPTrackPortLocalManagedObject *pTrackPortMO = 
                            (WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject>(pVRRPSessionConfigLocalManagedObject->searchTrackPort(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName()))).operator->();

                    if (pTrackPortMO == NULL) {
                        trace(TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::validateVRRPLocalConfigStep TrackPortMO does not exist");

                        if (NO_VRRP_TRACK == choice) {
                            trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Delete for nonexistent track object");
                            // To prevent a request to delete a nonexistent config from causing any issues (which is needed for
                            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                        }
                    } else {
                        trace(TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::validateVRRPLocalConfigStep TrackPortMO already exists");

                        if ((VRRP_TRACK == choice) && (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio() == pTrackPortMO->getTrack_prio())) {
                            trace(TRACE_LEVEL_INFO, "VRRPLocalConfWorker::validateVRRPLocalConfigStep: Create for existing track object");
                            // To prevent a request to create an already-existing config from causing any issues (which is needed for
                            // Brocade WyserEa read optimizations), we skip the rest of the sequence and cause it to return success.
                            status = WAVE_SEQUENCER_SKIP_TO_SUCCESS_STEP;
                        }
                    }
                }
                break;
            }
        }

        return status;
    }

    ResourceId VRRPLocalConfWorker::validateVRRPLocalConfigStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        return validateVRRPLocalConfigStepTemplate<VRRPSessionConfigLocalManagedObject, &NsmL3ReadyManagedObject::getVRRPIntfSession> (pDceSynchronousLinearSeqContext);
    }

    ResourceId VRRPLocalConfWorker::validateVRRPELocalConfigStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        return validateVRRPLocalConfigStepTemplate<VRRPESessionConfigLocalManagedObject, &NsmL3ReadyManagedObject::getVRRPEIntfSession> (pDceSynchronousLinearSeqContext);
    }

    ResourceId VRRPLocalConfWorker::validateVRRPLocalConfigIntfStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        ResourceId  status   = WAVE_MESSAGE_SUCCESS;

        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::validateVRRPLocalConfigIntfStep: Entered....");

        UI32 choice = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice();
        if ((VRRP_GLOBAL_CMD_ENABLE != choice) &&
            (VRRP_GLOBAL_CMD_DISABLE != choice) &&
			(VRRPE_GLOBAL_CMD_ENABLE != choice) &&
            (VRRPE_GLOBAL_CMD_DISABLE != choice) &&
			(VRRP_ALL_GLOBAL_CMD_ENABLE != choice) &&
            (VRRP_ALL_GLOBAL_CMD_DISABLE != choice) )
        {
        	UI8 vrrp_type = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type();
        	switch (vrrp_type) {
            	case VRRP_TYPE_STANDARD:
                	status = validateVRRPLocalConfigStep(pDceSynchronousLinearSeqContext);
                	break;
            	case VRRP_TYPE_EXTENDED:
                	status = validateVRRPELocalConfigStep(pDceSynchronousLinearSeqContext);
                	break;
            	case VRRP_TYPE_CLUSTER:
//                  	status = VRRPVCSLocalValidateStep(pDceSynchronousLinearSeqContext);
                	break;
			}
        }

        return status;
    }

    ResourceId VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        string ifName;
        //InterfaceType ifType;
        VRRPSessionConfigLocalManagedObject *pVRRPMO = NULL;
		bool owner = false;
		UI32 match;
		IpV4Address vip;

        // Check for Layer3 license before sending to backend
        LAYER_3_LICENSE_CHECK();
    
        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfStep: Entered....");

        if ((VRRP_GLOBAL_CMD_ENABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
            (VRRP_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
			(VRRPE_GLOBAL_CMD_ENABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
            (VRRPE_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
			(VRRP_ALL_GLOBAL_CMD_ENABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
            (VRRP_ALL_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) )
        {   
            trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfStep: global command....");
            return WAVE_MESSAGE_SUCCESS;
        } 

		ifName = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfName();
		//ifType = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType();
		 pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType();

		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pDceSynchronousLinearSeqContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));

		if (pL3ReadyMo == NULL) {
			trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfStep: L3 Ready MO does not exist");
			return WRC_VRRP_API_SET_ERR_L3_NOT_READY;
		} 

		if (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice() == VRRP_VIP) {
			vip = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip();
			NsmIpAddrConfigManagedObject *addrCfg = ((WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>)pL3ReadyMo->getIpAddrConfig()).operator->();
			if (addrCfg == NULL) {
				return WRC_VRRP_API_SET_ERR_NO_INTF_IP;
			}

			match = addrCfg->matchIPSubnet(vip.toString());
        	trace (TRACE_LEVEL_INFO, 
				string("VRRPLocalConfWorker:createVRRPLocalConfigPhyIntfStep: VIP: ") 
				+ vip.toString() + string( " match: ") + match 
				+ string (" VRRP Type: ") 
				+ pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type());

			/* If the first Virtual IP is being configured */
			if (match == NSM_IP_ADDR_NO_MATCH) 
			{
				return WRC_VRRP_API_SET_ERR_PREFIX_NOT_SAME;
			}

			/* For VRRPE, if the VIP exactly matches the Intf IP, then return */
			if ((match == NSM_IP_ADDR_EXACT_MATCH) && (VRRP_TYPE_EXTENDED == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type()))
			{
				return WRC_VRRP_API_SET_ERR_SAME_REALIP_VIP;
			}

			if (VRRP_TYPE_STANDARD == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type()) {
				pVRRPMO = (WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject>(pL3ReadyMo->getVRRPIntfSession(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid()))).operator->();

				if (pVRRPMO) {
					if (pVRRPMO->m_vip.size() == 0)
					{
						owner = (match == NSM_IP_ADDR_EXACT_MATCH) ? true: false;
					}
					else 
					{
						owner = pVRRPMO->m_owner;
						trace (TRACE_LEVEL_INFO, string("m_owner: ") + pVRRPMO->m_owner);
						trace (TRACE_LEVEL_INFO, string("owner: ") + owner);
						if ((owner == true) && (match == NSM_IP_ADDR_PREFIX_MATCH)) {
							return WRC_VRRP_API_SET_ERR_OWNER_NOT_REAL_VIP;
						}

						if ((owner == false) && (match == NSM_IP_ADDR_EXACT_MATCH)) {
							return WRC_VRRP_API_SET_ERR_NON_OWNER_REAL_VIP;
						}
					}
				} else {
					trace (TRACE_LEVEL_ERROR, 
						string("VRRPLocalConfWorker:::createVRRPLocalConfigPhyIntfStep:") +
						string(" VRRP MO does not exist"));
					return WAVE_MESSAGE_ERROR;
				}
			}
		}
		if (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice() == VRRP_TRACK) {
			InterfaceType trackIfType =  pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifType();
			string trackIfName =  pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName();
			UI32 count = 0;
			switch (trackIfType)
			{
				case IF_TYPE_GI:
				case IF_TYPE_TE:
				case IF_TYPE_FO:
				{
        			WaveManagedObjectSynchronousQueryContext trackPhyQueryCtxt(PhyIntfLocalManagedObject::getClassName());
					trackPhyQueryCtxt.addAndAttribute(new AttributeString(trackIfName, "id"));
					trackPhyQueryCtxt.addAndAttribute(new AttributeEnum((UI32 *)&trackIfType, "phyType"));
					ResourceId queryStatus = WaveObjectManagerToolKit::querySynchronouslyForCount(&trackPhyQueryCtxt, count);
					if (WAVE_MESSAGE_SUCCESS == queryStatus) {
						trace(TRACE_LEVEL_INFO, string("count = ") + count);
						if(count != 1)
							return WRC_NSM_ERR_PHYINTF_INVALID; 
					} else {
						trace(TRACE_LEVEL_INFO, string("Query Failed : ") + FrameworkToolKit::localize(queryStatus));
						return queryStatus;
					}
					int mapid=DcmToolKit::getLocalMappedId();
					int myrbrid=DcmToolKit::getDomainIdFromMappedId(mapid);
					if ((L2sysUtils::tell_rbrid((char *)trackIfName.c_str())) == myrbrid) {
						trace(TRACE_LEVEL_INFO, string("Same Rbridge:") + myrbrid);
					}
					else {
						trace(TRACE_LEVEL_INFO, string("Different Rbridge:") + myrbrid);
					}
					break;
				}
				case IF_TYPE_PO:
					break;
				default:
					return WRC_VRRP_API_SET_ERR_NO_SUCH_INTERFACE; 
			}
		}
        trace (TRACE_LEVEL_INFO, 
				string("VRRPLocalConfWorker:createVRRPLocalConfigPhyIntfStep: VIP: ") 
				+ vip.toString()); 

        int status;
        VRRPClientIntfConfigSPMessage *m = new VRRPClientIntfConfigSPMessage();

        m->setIfName(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfName());
        m->setIfType(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType());
        m->setVrid(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid());
        m->setVirtual_ip(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip());
        m->setTrack_ifType(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifType());
        m->setTrack_ifName(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName());
        m->setTrack_prio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio());
        m->setAdvt_int(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getAdvt_int());
        m->setPrio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getPrio());
        m->setEnable(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getEnable());
        m->setPreempt(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getPreempt());
        m->setHold_time(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getHold_time());
        m->setDescription(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getDescription());
		m->setOwner(owner);
        m->setAdvt_backup(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getAdvt_backup());
        m->setGarp_timer(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getGarp_timer());
        m->setBackup_advt_int(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getBackup_advt_int());
        m->setSpf(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getSpf());
        m->setRevPrio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getRevPrio());
        m->setVrrp_type(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type());
        m->setChoice(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice());

        status = sendSynchronouslyToWaveClient ("vrrp", m);
		pNsmUpdateVRRPLocalConfigPhyIntfMessage->setCompletionStatus(status);

		if(WAVE_MESSAGE_SUCCESS == status)
		{
			if(WAVE_MESSAGE_SUCCESS != pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus())
			{       
				status = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus();
			}       
			else if(WAVE_MESSAGE_SUCCESS != VRRPLocalConfWorker::mapBackEndErrorsToDcmdErrors(m->getClientStatus()))
			{       
				status =  VRRPLocalConfWorker::mapBackEndErrorsToDcmdErrors(m->getClientStatus());
				pNsmUpdateVRRPLocalConfigPhyIntfMessage->setCompletionStatus(status);
				trace(TRACE_LEVEL_ERROR, string("VRRPLocalConfWorker::errorCode: Error : ")+status);
				trace(TRACE_LEVEL_ERROR, string("VRRPLocalConfWorker::errorCode: Status : ") +FrameworkToolKit::localize(status));
			}
		}

		delete(m);
        return status;

    }

    ResourceId VRRPLocalConfWorker::VRRPLocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        UI32 defaultAdvt_int = VRRP_DEFAULT_ADVT_INT;
        UI8 defaultPrio = VRRP_DEFAULT_PRIO;
        bool defaultEnable = false;
        bool defaultPreempt = true;
        UI32 defaultHold_time = VRRP_DEFAULT_HOLD_TIME;
        string defaultDescription = "\0";
        string ifName;
        InterfaceType ifType;
        VRRPSessionConfigLocalManagedObject *pVRRPSessionConfigLocalManagedObject = NULL;
        int createFlag = 0;

        trace (TRACE_LEVEL_DEBUG, string ("VRRPLocalConfWorker: Entering VRRPLocalMOStep"));
        
        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        ifName = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfName();
        ifType = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType();

		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pDceSynchronousLinearSeqContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));
		if (pL3ReadyMo == NULL) {
			trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker:: MO Step - L3 Ready MO does not exist");
			return WAVE_MESSAGE_ERROR;
		}

        /* Get a pointer to the VRRP MO if it exists */
        pVRRPSessionConfigLocalManagedObject = (WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject>(pL3ReadyMo->getVRRPIntfSession(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid()))).operator->();
        if (NULL == pVRRPSessionConfigLocalManagedObject) {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::VRRPLocalMOStep:") +
                string(" VRRP MO does not exist"));
            if(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice() != VRRP_SET_VRID)
                return WAVE_MESSAGE_ERROR;

            /* Create a new MO since we didnt find one */
            pVRRPSessionConfigLocalManagedObject = new VRRPSessionConfigLocalManagedObject(
                      dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
            VRRP_PLUG_ASSERT_PTR(pVRRPSessionConfigLocalManagedObject);
            createFlag = 1;
                
            /* Create the link between VRRPMO and L3 ready MO */
			pVRRPSessionConfigLocalManagedObject->setOwnerManagedObjectId(pL3ReadyMo->getObjectId());
			pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("ownerManagedObjectId");
        } else {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::VRRPLocalMOStep:") +
                string(" VRRP MO exists"));
        }
        pVRRPSessionConfigLocalManagedObject->setifName(ifName);
        pVRRPSessionConfigLocalManagedObject->setifType(ifType);
		pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("ifname");
		pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("iftype");
        
        switch(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) {
            case VRRP_SET_VRID:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_SET_VRID ");
                pVRRPSessionConfigLocalManagedObject->setVrid(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("vrid");
                if (createFlag) {
                    pVRRPSessionConfigLocalManagedObject->setAdvt_int(defaultAdvt_int);
                    pVRRPSessionConfigLocalManagedObject->setPrio(defaultPrio);
                    pVRRPSessionConfigLocalManagedObject->setEnable(defaultEnable);
                    pVRRPSessionConfigLocalManagedObject->setPreempt(defaultPreempt);
                    pVRRPSessionConfigLocalManagedObject->setHold_time(defaultHold_time);
                    pVRRPSessionConfigLocalManagedObject->setDescription(defaultDescription);
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("owner");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
                }
				break;

            case VRRP_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_PRIORITY");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPSessionConfigLocalManagedObject->setPrio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getPrio());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
				break;
            case NO_VRRP_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_PRIORITY");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setPrio(defaultPrio);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
				break;

            case VRRP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_ADVT_INT");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setAdvt_int (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getAdvt_int ());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
				break;
            case NO_VRRP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_ADVT_INT");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setAdvt_int (defaultAdvt_int);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
				break;

            case VRRP_ENABLE:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_ENABLE");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setEnable(true);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				break;
            case NO_VRRP_ENABLE:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_ENABLE");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setEnable(false);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				break;

            case VRRP_PREEMPT:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_PREEMPT");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setPreempt(true);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
				break;
            case NO_VRRP_PREEMPT:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_PREEMPT");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setPreempt(false);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
				break;

            case VRRP_HOLD_TIME:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_HOLD_TIME");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setHold_time(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getHold_time ());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
				break;
            case NO_VRRP_HOLD_TIME:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_HOLD_TIME");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setHold_time(defaultHold_time);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
				break;

            case VRRP_DESC:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_DESC");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setDescription(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getDescription ());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
				break;
            case NO_VRRP_DESC:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_DESC");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
                pVRRPSessionConfigLocalManagedObject->setDescription(defaultDescription);
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
				break;

			case NO_VRRP_SET_VRID:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_SET_VRID");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPSessionConfigLocalManagedObject->deleteVirtualIPAll();
				pVRRPSessionConfigLocalManagedObject->deleteTrackPortAll();
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
				// deleteVirtualIPAll sets m_owner to false, so update owner too.
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("owner");
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("track");

                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_SET_VRID, deleted Vip and Track, deleting parent");
			    deleteFromComposition (NsmL3ReadyManagedObject::getClassName(), VRRPSessionConfigLocalManagedObject::getClassName(),
				                       "vrrpIntfConfig", pVRRPSessionConfigLocalManagedObject->getOwnerManagedObjectId(),
									   pVRRPSessionConfigLocalManagedObject->getObjectId());
				return pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus ();
				break;

            case VRRP_VIP:
			{
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_VIP");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				VRRPVirtualIPLocalManagedObject *pVirtualIPMO = (WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject>(pVRRPSessionConfigLocalManagedObject->searchVirtualIPAddr(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip()))).operator->();
				if (pVirtualIPMO == NULL) {
					pVirtualIPMO = new VRRPVirtualIPLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()),
							pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip());
					if (pVRRPSessionConfigLocalManagedObject->addVirtualIPMO(pVirtualIPMO) == false)
					{
						trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker virtual IP already exist");
						delete (pVirtualIPMO);
						return WAVE_MESSAGE_ERROR;
					}
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("owner");
					pVRRPSessionConfigLocalManagedObject->setEnable(true);
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
					updateWaveManagedObject(pVirtualIPMO);

					NsmIpAddrConfigManagedObject *addrCfg = ((WaveManagedObjectPointer<NsmIpAddrConfigManagedObject>)pL3ReadyMo->getIpAddrConfig()).operator->();
					IpV4Address vip = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip();
					UI32 match = addrCfg->matchIPSubnet(vip.toString());
					bool owner = (match == NSM_IP_ADDR_EXACT_MATCH) ? true: false;
					pVRRPSessionConfigLocalManagedObject->setOwner(owner);
				} else {
					trace(TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker virtual IP already exist");
				}
			}
				break;

            case NO_VRRP_VIP:
			{
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_VIP");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPSessionConfigLocalManagedObject->deleteVirtualIPMO(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip());
				if (pVRRPSessionConfigLocalManagedObject->getVirtualIPListSize() == 0) {
					pVRRPSessionConfigLocalManagedObject->setEnable(false);
					pVRRPSessionConfigLocalManagedObject->setOwner(false);
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				}
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("owner");
			}
				break;

            case VRRP_TRACK:
			{
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_TRACK");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);

				VRRPTrackPortLocalManagedObject *pTrackPortMO = 
						(WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject>(pVRRPSessionConfigLocalManagedObject->searchTrackPort(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName()))).operator->();
				if (pTrackPortMO == NULL) {
					pTrackPortMO = new VRRPTrackPortLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()),
								pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifType(), pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName(),
								pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio());
					if (pVRRPSessionConfigLocalManagedObject->addTrackPortMO(pTrackPortMO) == false)
					{
						trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker:: MO Step - TrackPortMO already exist");
						delete pTrackPortMO;
						return WAVE_MESSAGE_ERROR;
					}
					pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("track");
					updateWaveManagedObject(pTrackPortMO);
				} else {
					pTrackPortMO->setTrack_prio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio());
					pTrackPortMO->addAttributeToBeUpdated("track_prio");
					updateWaveManagedObject(pTrackPortMO);
				}
			}	
				break;
            case NO_VRRP_TRACK:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_TRACK");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPSessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPSessionConfigLocalManagedObject->deleteTrackPortMO(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName());
				pVRRPSessionConfigLocalManagedObject->addAttributeToBeUpdated("track");
				break;
        }

        trace (TRACE_LEVEL_DEVEL, string("vrid: ") + pVRRPSessionConfigLocalManagedObject->getVrid());
        trace (TRACE_LEVEL_DEVEL, string("prio: ") + pVRRPSessionConfigLocalManagedObject->getPrio());
        trace (TRACE_LEVEL_DEVEL, string("hold-time: ") + pVRRPSessionConfigLocalManagedObject->getHold_time());
        trace (TRACE_LEVEL_DEVEL, string("desc: ") + pVRRPSessionConfigLocalManagedObject->getDescription());
        trace (TRACE_LEVEL_DEVEL, string("advt-int: ") + pVRRPSessionConfigLocalManagedObject->getAdvt_int());
        trace (TRACE_LEVEL_DEVEL, string("prempt: ") + pVRRPSessionConfigLocalManagedObject->getPreempt());
        trace (TRACE_LEVEL_DEVEL, string("enable: ") + pVRRPSessionConfigLocalManagedObject->getEnable());

		if(createFlag)
		{
			addToComposition(NsmL3ReadyManagedObject::getClassName(), VRRPSessionConfigLocalManagedObject::getClassName(),
				"vrrpIntfConfig", pVRRPSessionConfigLocalManagedObject->getOwnerManagedObjectId(),
				pVRRPSessionConfigLocalManagedObject->getObjectId());
			pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pVRRPSessionConfigLocalManagedObject);
		}
		else
		{
			updateWaveManagedObject (pVRRPSessionConfigLocalManagedObject);
		}

        return pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus ();
    }

    ResourceId VRRPLocalConfWorker::VRRPELocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        UI32 defaultAdvt_int = VRRP_DEFAULT_ADVT_INT;
        UI32 defaultBackup_advt_int = VRRP_DEFAULT_BACKUP_ADVT_INT;
        UI8 defaultPrio = VRRP_DEFAULT_PRIO;
        bool defaultEnable = false;
        bool defaultPreempt = false;
        bool defaultAdvt_backup = 0;
        bool defaultSpf = false;
		UI8 defaultRevPrio = 0;
        UI32 defaultHold_time = VRRP_DEFAULT_HOLD_TIME;
        UI32 defaultGarp_timer = VRRPE_DEFAULT_GARP_TIMER;
        string defaultDescription = "\0";
        VRRPESessionConfigLocalManagedObject *pVRRPESessionConfigLocalManagedObject = NULL;
        InterfaceType ifType;
		string ifName;
		bool createFlag = false;

        trace (TRACE_LEVEL_DEBUG, string ("VRRPLocalConfWorker: Entering VRRPELocalMOStep"));

        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        ifName = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfName();
        ifType = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getIfType();

		NsmL3ReadyManagedObject *pL3ReadyMo = dynamic_cast<NsmL3ReadyManagedObject *> (pDceSynchronousLinearSeqContext->getCache()->getWaveManagedObject(NsmL3ReadyManagedObject::getClassName()));

		if (pL3ReadyMo == NULL) {
			trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker:: MO Step - L3 Ready MO does not exist");
			return WAVE_MESSAGE_ERROR;
		} 

        /* Get a pointer to the VRRP MO if it exists */
        pVRRPESessionConfigLocalManagedObject = (WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject>(pL3ReadyMo->getVRRPEIntfSession(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid()))).operator->();
        if (NULL == pVRRPESessionConfigLocalManagedObject) {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::VRRPELocalMOStep:") +
                string(" VRRP MO does not exist"));

            /* Create a new MO since we didnt find one */
            pVRRPESessionConfigLocalManagedObject = new VRRPESessionConfigLocalManagedObject(
                      dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()));
            VRRP_PLUG_ASSERT_PTR(pVRRPESessionConfigLocalManagedObject);
            createFlag = 1;
                
            /* Create the link between VRRPEMO and L3 ready MO */
			pVRRPESessionConfigLocalManagedObject->setOwnerManagedObjectId(pL3ReadyMo->getObjectId());
			pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("ownerManagedObjectId");
        } else {
            trace (TRACE_LEVEL_INFO, 
                string("VRRPLocalConfWorker::VRRPELocalMOStep:") +
                string(" VRRP MO exists"));
        }

        VRRPVirtualIPLocalManagedObject *pVirtualIPMO = NULL;

        pVRRPESessionConfigLocalManagedObject->setifName(ifName);
        pVRRPESessionConfigLocalManagedObject->setifType(ifType);
		pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("ifname");
		pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("iftype");

        switch(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) {
            case VRRP_SET_VRID:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_SET_VRID ");
                pVRRPESessionConfigLocalManagedObject->setVrid(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid());
                pVRRPESessionConfigLocalManagedObject->setAdvt_int(defaultAdvt_int);
                pVRRPESessionConfigLocalManagedObject->setPrio(defaultPrio);
                pVRRPESessionConfigLocalManagedObject->setEnable(defaultEnable);
                pVRRPESessionConfigLocalManagedObject->setPreempt(defaultPreempt);
                pVRRPESessionConfigLocalManagedObject->setHold_time(defaultHold_time);
                pVRRPESessionConfigLocalManagedObject->setDescription(defaultDescription);
                pVRRPESessionConfigLocalManagedObject->setAdvt_backup(defaultAdvt_backup);
                pVRRPESessionConfigLocalManagedObject->setBackup_advt_int(defaultBackup_advt_int);
                pVRRPESessionConfigLocalManagedObject->setSpf(defaultSpf);
                pVRRPESessionConfigLocalManagedObject->setRevPrio(defaultRevPrio);
                pVRRPESessionConfigLocalManagedObject->setGarp_timer(defaultGarp_timer);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("vrid");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_backup");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("backup_advt_int");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("spf");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("garp_timer");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("rev_prio");
				break;

            case VRRP_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_PRIORITY");
                pVRRPESessionConfigLocalManagedObject->setPrio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getPrio ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
				break;
            case NO_VRRP_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_PRIORITY");
                pVRRPESessionConfigLocalManagedObject->setPrio(defaultPrio);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("prio");
				break;

            case VRRP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_ADVT_INT");
                pVRRPESessionConfigLocalManagedObject->setAdvt_int (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getAdvt_int ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
				break;
            case NO_VRRP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_ADVT_INT");
                pVRRPESessionConfigLocalManagedObject->setAdvt_int (defaultAdvt_int);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_int");
				break;

            case VRRP_ENABLE:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_ENABLE");
                pVRRPESessionConfigLocalManagedObject->setEnable(true);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				break;
            case NO_VRRP_ENABLE:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_ENABLE");
                pVRRPESessionConfigLocalManagedObject->setEnable(false);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				break;

            case VRRP_PREEMPT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_PREEMPT");
                pVRRPESessionConfigLocalManagedObject->setPreempt(true);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
				break;
            case NO_VRRP_PREEMPT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_PREEMPT");
                pVRRPESessionConfigLocalManagedObject->setPreempt(false);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("preempt");
				break;

            case VRRP_HOLD_TIME:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_HOLD_TIME");
                pVRRPESessionConfigLocalManagedObject->setHold_time(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getHold_time ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
				break;
            case NO_VRRP_HOLD_TIME:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_HOLD_TIME");
                pVRRPESessionConfigLocalManagedObject->setHold_time(defaultHold_time);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("hold_time");
				break;

            case VRRP_DESC:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_DESC");
                pVRRPESessionConfigLocalManagedObject->setDescription(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getDescription ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
				break;
            case NO_VRRP_DESC:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_DESC");
                pVRRPESessionConfigLocalManagedObject->setDescription("");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("description");
				break;

            case VRRP_ADVT_BACKUP:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_ADVT_BACKUP");
                pVRRPESessionConfigLocalManagedObject->setAdvt_backup(true);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_backup");
				break;
            case NO_VRRP_ADVT_BACKUP:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_ADVT_BACKUP");
                pVRRPESessionConfigLocalManagedObject->setAdvt_backup(false);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("advt_backup");
				break;

            case VRRP_BACKUP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_BACKUP_ADVT_INT");
                pVRRPESessionConfigLocalManagedObject->setBackup_advt_int (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getBackup_advt_int ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("backup_advt_int");
				break;
            case NO_VRRP_BACKUP_ADVT_INT:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_BACKUP_ADVT_INT");
                pVRRPESessionConfigLocalManagedObject->setBackup_advt_int (defaultBackup_advt_int);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("backup_advt_int");
				break;

            case VRRP_SPF:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_SPF");
                pVRRPESessionConfigLocalManagedObject->setSpf(true);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("spf");
				break;
            case NO_VRRP_SPF:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_SPF");
                pVRRPESessionConfigLocalManagedObject->setSpf(false);
                pVRRPESessionConfigLocalManagedObject->setRevPrio(defaultRevPrio);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("spf");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("rev_prio");
				break;

            case VRRP_GARP_TIMER:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_GARP_TIMER");
                pVRRPESessionConfigLocalManagedObject->setGarp_timer (pNsmUpdateVRRPLocalConfigPhyIntfMessage->getGarp_timer ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("garp_timer");
				break;
            case NO_VRRP_GARP_TIMER:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_GARP_TIMER");
                pVRRPESessionConfigLocalManagedObject->setGarp_timer(defaultGarp_timer);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("garp_timer");
				break;

            case VRRP_REVERT_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case VRRP_REVERT_PRIORITY");
                pVRRPESessionConfigLocalManagedObject->setSpf(true);
                pVRRPESessionConfigLocalManagedObject->setRevPrio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getRevPrio ());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("spf");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("rev_prio");
				break;
            case NO_VRRP_REVERT_PRIORITY:
                trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_REVERT_PRIORITY");
                pVRRPESessionConfigLocalManagedObject->setRevPrio(defaultRevPrio);
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("rev_prio");
				break;

			case NO_VRRP_SET_VRID:
			{
				trace (TRACE_LEVEL_DEVEL, "VRRPELocalConfWorker case NO_VRRP_SET_VRID");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPESessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPESessionConfigLocalManagedObject->deleteVirtualIPAll();
				pVRRPESessionConfigLocalManagedObject->deleteTrackPortAll();
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
				// deleteVirtualIPAll sets m_owner to false, so update owner too.
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("owner");
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("track");

			    deleteFromComposition (NsmL3ReadyManagedObject::getClassName(), VRRPESessionConfigLocalManagedObject::getClassName(),
				                       "vrrpeIntfConfig", pVRRPESessionConfigLocalManagedObject->getOwnerManagedObjectId(),
									   pVRRPESessionConfigLocalManagedObject->getObjectId());
				return pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus ();
				break;
			}
            case VRRP_VIP:
			{
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_VIP");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPESessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVirtualIPMO = (WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject>(pVRRPESessionConfigLocalManagedObject->searchVirtualIPAddr(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip()))).operator->();
				if (pVirtualIPMO == NULL) {
					pVirtualIPMO = new VRRPVirtualIPLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()),
							pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip());
					if (pVRRPESessionConfigLocalManagedObject->addVirtualIPMO(pVirtualIPMO) == false)
					{
						delete (pVirtualIPMO);
						return WAVE_MESSAGE_ERROR;
					}
					pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
					pVRRPESessionConfigLocalManagedObject->setEnable(true);
					pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
					updateWaveManagedObject(pVirtualIPMO);
				} else {
						trace(TRACE_LEVEL_DEBUG, "VRRPELocalConfWorker virtual IP already exist");
				}
			}
				break;
            case NO_VRRP_VIP:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_VIP");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPESessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPESessionConfigLocalManagedObject->deleteVirtualIPMO(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVirtual_ip());
				if (pVRRPESessionConfigLocalManagedObject->getVirtualIPListSize() == 0) {
					pVRRPESessionConfigLocalManagedObject->setEnable(false);
					pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("enable");
				}
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("vip");
				break;

            case VRRP_TRACK:
			{
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case VRRP_TRACK");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPESessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);

				VRRPTrackPortLocalManagedObject *pTrackPortMO = 
						(WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject>(pVRRPESessionConfigLocalManagedObject->searchTrackPort(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName()))).operator->();
				if (pTrackPortMO == NULL) {
					pTrackPortMO = new VRRPTrackPortLocalManagedObject(dynamic_cast<NsmLocalObjectManager*>(getPWaveObjectManager()),
								pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifType(), pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName(),
								pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio());
					if (pVRRPESessionConfigLocalManagedObject->addTrackPortMO(pTrackPortMO) == false)
					{
						trace(TRACE_LEVEL_FATAL, "VRRPLocalConfWorker:: MO Step - TrackPortMO already exist");
						delete pTrackPortMO;
						return WAVE_MESSAGE_ERROR;
					}
					pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("track");
					updateWaveManagedObject(pTrackPortMO);
				} else {
					pTrackPortMO->setTrack_prio(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_prio());
					pTrackPortMO->addAttributeToBeUpdated("track_prio");
					updateWaveManagedObject(pTrackPortMO);
				}
			}	
				break;
            case NO_VRRP_TRACK:
                trace (TRACE_LEVEL_DEVEL, "VRRPLocalConfWorker case NO_VRRP_TRACK");
				prismAssert(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrid() == pVRRPESessionConfigLocalManagedObject->getVrid(), __FILE__, __LINE__);
				pVRRPESessionConfigLocalManagedObject->deleteTrackPortMO(pNsmUpdateVRRPLocalConfigPhyIntfMessage->getTrack_ifName());
				pVRRPESessionConfigLocalManagedObject->addAttributeToBeUpdated("track");
				break;
        
        }

        trace (TRACE_LEVEL_DEVEL, string("vrid: ") + pVRRPESessionConfigLocalManagedObject->getVrid());
        trace (TRACE_LEVEL_DEVEL, string("prio: ") + pVRRPESessionConfigLocalManagedObject->getPrio());
        trace (TRACE_LEVEL_DEVEL, string("hold-time: ") + pVRRPESessionConfigLocalManagedObject->getHold_time());
        trace (TRACE_LEVEL_DEVEL, string("desc: ") + pVRRPESessionConfigLocalManagedObject->getDescription());
        trace (TRACE_LEVEL_DEVEL, string("advt-int: ") + pVRRPESessionConfigLocalManagedObject->getAdvt_int());
        trace (TRACE_LEVEL_DEVEL, string("prempt: ") + pVRRPESessionConfigLocalManagedObject->getPreempt());
        trace (TRACE_LEVEL_DEVEL, string("enable: ") + pVRRPESessionConfigLocalManagedObject->getEnable());
        trace (TRACE_LEVEL_DEVEL, string("advt-backup: ") + pVRRPESessionConfigLocalManagedObject->getAdvt_backup());
        trace (TRACE_LEVEL_DEVEL, string("backup-advt-int: ") + pVRRPESessionConfigLocalManagedObject->getBackup_advt_int());
        trace (TRACE_LEVEL_DEVEL, string("spf: ") + pVRRPESessionConfigLocalManagedObject->getSpf());
        trace (TRACE_LEVEL_DEVEL, string("garp-timer: ") + pVRRPESessionConfigLocalManagedObject->getGarp_timer());
        trace (TRACE_LEVEL_DEVEL, string("rev_prio: ") + pVRRPESessionConfigLocalManagedObject->getRevPrio());

		if(createFlag)
		{
			addToComposition(NsmL3ReadyManagedObject::getClassName(), VRRPESessionConfigLocalManagedObject::getClassName(),
				"vrrpeIntfConfig", pVRRPESessionConfigLocalManagedObject->getOwnerManagedObjectId(),
				pVRRPESessionConfigLocalManagedObject->getObjectId());
			pDceSynchronousLinearSeqContext->addManagedObjectForGarbageCollection(pVRRPESessionConfigLocalManagedObject);
		}
		else
		{
			updateWaveManagedObject (pVRRPESessionConfigLocalManagedObject);
		}

        return pNsmUpdateVRRPLocalConfigPhyIntfMessage->getCompletionStatus ();
    }

    ResourceId VRRPLocalConfWorker::VRRPVCSLocalMOStep (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;
        trace (TRACE_LEVEL_DEBUG, string ("VRRPLocalConfWorker: Entering VRRPELocalMOStep"));

        return status;
    }

    ResourceId VRRPLocalConfWorker::updateVRRPLocalConfigSessionMO (DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext, UI32 choice)
    {
		VRRPSessionConfigLocalManagedObject *pVRRPMO = NULL;
		VRRPESessionConfigLocalManagedObject *pVRRPEMO = NULL;
        UI32 sessionMOCount = 0;
		ObjectId sessionObjectId;
		vector<WaveManagedObject * >* pVRRPResults;
		vector<WaveManagedObject * >* pVRRPEResults;
		LocationId locationId = FrameworkToolKit::getThisLocationId();

		if (choice == VRRP_GLOBAL_CMD_DISABLE || choice == VRRP_ALL_GLOBAL_CMD_DISABLE) {
			/* Get all the VRRPSessionMOs for the current node */
			WaveManagedObjectSynchronousQueryContext vrrpSessionsyncQueryCtxt (VRRPSessionConfigLocalManagedObject::getClassName());
			vrrpSessionsyncQueryCtxt.setLocationIdFilter(locationId);
			pVRRPResults = querySynchronously(&vrrpSessionsyncQueryCtxt);
			sessionMOCount = pVRRPResults->size ();
			trace (TRACE_LEVEL_DEBUG, string("VRRPMO sessionMOCount: ") + sessionMOCount);

			for (UI32 i = 0; i < sessionMOCount; i++)
			{
				sessionObjectId = ((*pVRRPResults)[i])->getOwnerManagedObjectId();
				pVRRPMO = dynamic_cast<VRRPSessionConfigLocalManagedObject *>((*pVRRPResults)[i]);
				deleteFromComposition (NsmL3ReadyManagedObject::getClassName(), VRRPSessionConfigLocalManagedObject::getClassName(), "vrrpIntfConfig", pVRRPMO->getOwnerManagedObjectId(), pVRRPMO->getObjectId());
			}

			if(pDceSynchronousLinearSeqContext)
				pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pVRRPResults);
			if (pVRRPResults)
				delete (pVRRPResults);
		}

		if (choice == VRRPE_GLOBAL_CMD_DISABLE || choice == VRRP_ALL_GLOBAL_CMD_DISABLE) {
			/* Get all the VRRPESessionMOs for the current node */
			WaveManagedObjectSynchronousQueryContext vrrpeSessionsyncQueryCtxt (VRRPESessionConfigLocalManagedObject::getClassName());
			vrrpeSessionsyncQueryCtxt.setLocationIdFilter(locationId);
			pVRRPEResults = querySynchronously(&vrrpeSessionsyncQueryCtxt);
			sessionMOCount = pVRRPEResults->size ();
			trace (TRACE_LEVEL_DEBUG, string("VRRPEMO sessionMOCount: ") + sessionMOCount);

			for (UI32 i = 0; i < sessionMOCount; i++)
			{
				sessionObjectId = ((*pVRRPEResults)[i])->getOwnerManagedObjectId();
				pVRRPEMO = dynamic_cast<VRRPESessionConfigLocalManagedObject *>((*pVRRPEResults)[i]);
				deleteFromComposition (NsmL3ReadyManagedObject::getClassName(), VRRPESessionConfigLocalManagedObject::getClassName(), "vrrpeIntfConfig", pVRRPEMO->getOwnerManagedObjectId(), pVRRPEMO->getObjectId());
			}

			if(pDceSynchronousLinearSeqContext)
				pDceSynchronousLinearSeqContext->addManagedObjectsForGarbageCollection(*pVRRPEResults);
			if (pVRRPEResults)
				delete (pVRRPEResults);
		}

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfMOStep(DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext)
    {
        ResourceId  status   = WAVE_MESSAGE_ERROR;

        NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage = dynamic_cast<NsmUpdateVRRPLocalConfigPhyIntfMessage*> (pDceSynchronousLinearSeqContext->getPPrismMessage ());

        trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfMOStep: Entered....");

        if ((VRRP_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
            (VRRPE_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()) ||
            (VRRP_ALL_GLOBAL_CMD_DISABLE == pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice()))
        {
            trace (TRACE_LEVEL_DEBUG, "VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfMOStep: global command update....");
            status = updateVRRPLocalConfigSessionMO(pDceSynchronousLinearSeqContext, pNsmUpdateVRRPLocalConfigPhyIntfMessage->getChoice());
        } else {
        	UI8 vrrp_type = pNsmUpdateVRRPLocalConfigPhyIntfMessage->getVrrp_type();
        	switch (vrrp_type) {
            	case VRRP_TYPE_STANDARD:
                	status = VRRPLocalMOStep(pDceSynchronousLinearSeqContext);
                	break;
            	case VRRP_TYPE_EXTENDED:
                	status = VRRPELocalMOStep(pDceSynchronousLinearSeqContext);
                	break;
            	case VRRP_TYPE_CLUSTER:
                	status = VRRPVCSLocalMOStep(pDceSynchronousLinearSeqContext);
                	break;
			}
        }

        return status;
    }

    void  VRRPLocalConfWorker::NsmUpdateVRRPLocalConfigPhyIntfMessageHandler( NsmUpdateVRRPLocalConfigPhyIntfMessage *pNsmUpdateVRRPLocalConfigPhyIntfMessage)
    {
// Validations and Queries should go here

        PrismSynchronousLinearSequencerStep sequencerSteps[] =
        {
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::validateVRRPLocalConfigIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerStartTransactionStep),
// Your configuration change code goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::createVRRPLocalConfigPhyIntfMOStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerCommitTransactionStep),
// Programming Protocol Daemons goes here

            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
            reinterpret_cast<PrismSynchronousLinearSequencerStep>(&VRRPLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
        };

        DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext (pNsmUpdateVRRPLocalConfigPhyIntfMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));

        pDceSynchronousLinearSeqContext->execute ();
    }

	ResourceId VRRPLocalConfWorker::sendShowToVRRPDaemon (DceSynchronousLinearSeqContext *ctx_p)
	{
		VRRPShowMessage *cl_msg_p = NULL;
		ResourceId status = WAVE_MESSAGE_ERROR;
		VRRPLocalShowMessage *msg_p = NULL;
		SI32 mappedId = DcmToolKit::getLocalMappedId();
		if (mappedId == (-1)) {
			mappedId = 0;
		}

		if(NULL==ctx_p->getPPrismMessage())
		{
			return status;
		}
		msg_p = dynamic_cast<VRRPLocalShowMessage *> (ctx_p->getPPrismMessage ());
		cl_msg_p = new VRRPShowMessage(msg_p->getVrid(), msg_p->getIfName(), msg_p->getCmdCode(), msg_p->getShowType(), msg_p->getLastIndex());
		cl_msg_p->setMappedId(mappedId);
		if (cl_msg_p->getCmdCode() == VRRP_SHOW_CMD_NSM_DB || cl_msg_p->getCmdCode() == VRRP_SHOW_CMD_NSM_PENDING_LIST) {
			status = sendSynchronouslyToWaveClient("nsm", cl_msg_p);
		} else {
			status = sendSynchronouslyToWaveClient ("vrrp", cl_msg_p);
		}
		if (status == WAVE_MESSAGE_SUCCESS)
		{
			if (cl_msg_p->getCompletionStatus() != WAVE_MESSAGE_SUCCESS)
			{
				trace(TRACE_LEVEL_DEBUG, string("Resp: Client Completion Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getCompletionStatus()));
				msg_p->setCompletionStatus(cl_msg_p->getCompletionStatus());
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}
			if (cl_msg_p->getClientStatus() != WAVE_MESSAGE_SUCCESS)
			{ 
				trace(TRACE_LEVEL_DEBUG, string("Resp:  Client Error MsgId: ") + cl_msg_p->getMessageId() + FrameworkToolKit::localize(cl_msg_p->getClientStatus())); 
				msg_p->setCompletionStatus(cl_msg_p->getClientStatus());
				status = msg_p->getCompletionStatus();
				delete cl_msg_p;
				return status;
			}
		}
		if (status != WAVE_MESSAGE_SUCCESS)
		{
			trace(TRACE_LEVEL_DEBUG, string("Client Message sending failed for MsgId: ") + cl_msg_p->getMessageId() + string("reason:") +  FrameworkToolKit::localize(status));
			delete cl_msg_p;
			return status;
		}
		trace(TRACE_LEVEL_DEBUG, string("Client Message processing successful for MsgId: ") + cl_msg_p->getMessageId());
		msg_p->copyAllBuffers(*cl_msg_p);
		status = cl_msg_p->getCompletionStatus();
		delete cl_msg_p;
		return status;
	}

	void  VRRPLocalConfWorker::VRRPLocalShowMessageHandler(VRRPLocalShowMessage *pVRRPLocalShowMessage)
	{
		PrismSynchronousLinearSequencerStep sequencerSteps[] =
		{
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VRRPLocalConfWorker::sendShowToVRRPDaemon),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VRRPLocalConfWorker::prismSynchronousLinearSequencerSucceededStep),
			reinterpret_cast<PrismSynchronousLinearSequencerStep> (&VRRPLocalConfWorker::prismSynchronousLinearSequencerFailedStep)
		};
		DceSynchronousLinearSeqContext *pDceSynchronousLinearSeqContext = new DceSynchronousLinearSeqContext(pVRRPLocalShowMessage, this, sequencerSteps, sizeof (sequencerSteps) /sizeof (sequencerSteps[0]));
       	pDceSynchronousLinearSeqContext->execute ();
	}
}
