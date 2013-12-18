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
 *   Author : krao                                                     *
 **************************************************************************/

#include "ClientInterface/VRRP/VRRPGlobalConfigSPMessage.h"
#include "VRRP/Local/VRRPGlobalConfigManagedObject.h"
#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "VRRP/Local/VRRPLocalConfigWorker.h"
#include "VRRP/Local/VRRPTypes.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Local/Layer3/NsmL3Util.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"
#include "Nsm/Local/VRRPWorker/VRRPSessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPESessionConfigLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "L3Node/Global/L3NodeSpecificGlobalObjectManager.h"
#include "DcmCore/DcmToolKit.h"
#include "Nsm/Local/Layer3/NsmIntfSviManagedObject.h"
#include "Nsm/Local/Layer3/NsmNodePoManagedObject.h"
#include "ClientInterface/Nsm/NsmUtils.h"
#include "Utils/Layer3LicenseUtil.h"
#include "DcmResourceIds.h"

namespace DcmNs
{

    VRRPLocalObjectManager::VRRPLocalObjectManager ()
        : MultiPartitionCleanup  (getClassName (),getObjectToDelete())
    {
	    associateWithVirtualWaveObjectManager (L3NodeSpecificGlobalObjectManager::getInstance());
        m_pVRRPLocalConfigWorker = new VRRPLocalConfigWorker (this);
        prismAssert (NULL != m_pVRRPLocalConfigWorker, __FILE__, __LINE__);
    }

    VRRPLocalObjectManager::~VRRPLocalObjectManager ()
    {
    }

    VRRPLocalObjectManager  *VRRPLocalObjectManager::getInstance()
    {
        static VRRPLocalObjectManager *pVRRPLocalObjectManager = new VRRPLocalObjectManager ();

        WaveNs::prismAssert (NULL != pVRRPLocalObjectManager, __FILE__, __LINE__);

        return (pVRRPLocalObjectManager);
    }

    string  VRRPLocalObjectManager::getClassName()
    {
		return ("VRRP");
    }

    PrismServiceId  VRRPLocalObjectManager::getPrismServiceId()
    {
        return ((getInstance ())->getServiceId ());
    }

    PrismMessage  *VRRPLocalObjectManager::createMessageInstance(const UI32 &operationCode)
    {
        PrismMessage *pPrismMessage = NULL;

        switch (operationCode)
        {

            default :
                pPrismMessage = NULL;
        }

        return (pPrismMessage);
    }

    WaveManagedObject  *VRRPLocalObjectManager::createManagedObjectInstance(const string &managedClassName)
    {
        return NULL;
    }

    void VRRPLocalObjectManager::ConfigReplayVRRPGlobalConfig()
    {
        trace (TRACE_LEVEL_DEBUG, "Entering VRRPLocalObjectManager::ConfigReplayVRRPGlobalConfig");
        VRRPGlobalConfigManagedObject *pVRRPGlobalConfigManagedObject = NULL;
        LocationId locationId = FrameworkToolKit::getThisLocationId ();
        vector<WaveManagedObject * >* pResults = querySynchronouslyLocalManagedObjectsForLocationId (locationId, VRRPGlobalConfigManagedObject::getClassName());

        if ((NULL != pResults) && (pResults -> size() == 1))
        {
            VRRPGlobalConfigSPMessage *msg = new VRRPGlobalConfigSPMessage();
            pVRRPGlobalConfigManagedObject = dynamic_cast<VRRPGlobalConfigManagedObject *>((*pResults)[0]);
            msg->setConfigDisable(false);
            msg->setVrrpEnable(pVRRPGlobalConfigManagedObject->getVrrpEnable());
            msg->setVrrpeEnable(pVRRPGlobalConfigManagedObject->getVrrpeEnable());
            sendSynchronouslyToWaveClient ("vrrp", msg);
            delete msg;
        }

        if(pResults)
        {
			WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);
        }
    }

	void VRRPLocalObjectManager::postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase)
	{
		string passName  = pWaveAsynchronousContextForPostBootPhase->getPassName();
		LocationId locId = FrameworkToolKit::getThisLocationId();

		trace (TRACE_LEVEL_INFO, string("VRRPLocalObjectManager::postboot Entered with pass:") + passName);

        // Check for Layer3 license before sending to backend
        if (WRC_LAYER_3_LICENSE_NOT_PRESENT_ERROR == Layer3LicenseUtil::checkLayer3License()) {
            trace (TRACE_LEVEL_DEBUG, "VRRPLocalObjectManager::postboot. LAYER_3_LICENSE not present");
            pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
            pWaveAsynchronousContextForPostBootPhase->callback ();
            return;
        }

		if (passName.compare("DCM_POSTBOOT_GLOBAL_STAGE5") == 0) {
			trace (TRACE_LEVEL_DEVEL, "ConfigReplayVRRPGlobalConfig:: Replaying global config");
			ConfigReplayVRRPGlobalConfig();
			ConfigReplayVRRPVlanIntfConfig(locId);

			if (WAVE_PERSISTENCE_CLUSTER_ENABLED !=
				DcmToolKit::isVcsEnabled()) {
				ConfigReplayVRRPPoIntfConfig(locId);
			}
		}

		pWaveAsynchronousContextForPostBootPhase->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
		pWaveAsynchronousContextForPostBootPhase->callback ();
   }

	void VRRPLocalObjectManager::ConfigReplayVRRPVlanIntfConfig(LocationId locId)
	{
		NsmIntfSviManagedObject *pIntfSviManagedObject = NULL;
		vector<NsmIntfSviManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmL3Util::NodeVlanIfGetMOs(results, locId);
		for(unsigned int i = 0; i < results.size(); i++) {
			pIntfSviManagedObject = results[i];
			pL3ReadyMO = pIntfSviManagedObject->getL3ReadyMO();

			if (pL3ReadyMO) {
				vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > vrrpSessionConfigVector = pL3ReadyMO->getVRRPIntfConfig();
				if (vrrpSessionConfigVector.size()) {
					ConfigReplayVRRPInterfaceConfig(vrrpSessionConfigVector);
				}

				vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > vrrpeSessionConfigVector = pL3ReadyMO->getVRRPEIntfConfig();
				if (vrrpeSessionConfigVector.size()) {
					ConfigReplayVRRPEInterfaceConfig(vrrpeSessionConfigVector);
				}
			}
			delete pIntfSviManagedObject;
		}
		return ;
	}

	void VRRPLocalObjectManager::ConfigReplayVRRPPoIntfConfig(LocationId locId)
	{
		SI32 mapId = 0;
		NsmNodePoManagedObject *pNodePoManagedObject = NULL;
		vector<NsmNodePoManagedObject *> results;
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		NsmL3Util::NodePoIfGetMOs(results, mapId);

		for(unsigned int i = 0; i < results.size(); i++) {
			pNodePoManagedObject = results[i];
			pL3ReadyMO = pNodePoManagedObject->getL3ReadyMO();

			if (pL3ReadyMO) {
				vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > vrrpSessionConfigVector = pL3ReadyMO->getVRRPIntfConfig();
				if (vrrpSessionConfigVector.size()) {
					ConfigReplayVRRPInterfaceConfig(vrrpSessionConfigVector);
				}

				vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > vrrpeSessionConfigVector = pL3ReadyMO->getVRRPEIntfConfig();
				if (vrrpeSessionConfigVector.size()) {
					ConfigReplayVRRPEInterfaceConfig(vrrpeSessionConfigVector);
				}
			}
			delete pNodePoManagedObject;
		}
		return ;
	}

	void VRRPLocalObjectManager::configReplayVRRPPhyIntf(PhyIntfLocalManagedObject *pPhyIntfLocalManagedObject)
	{
		const NsmL3ReadyManagedObject *pL3ReadyMO;

		pL3ReadyMO = pPhyIntfLocalManagedObject->getL3ReadyMO();
		if (pL3ReadyMO) {
			vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > vrrpSessionConfigVector = pL3ReadyMO->getVRRPIntfConfig();
			if (vrrpSessionConfigVector.size()) {
				VRRPLocalObjectManager::ConfigReplayVRRPInterfaceConfig(vrrpSessionConfigVector);
			}

			vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > vrrpeSessionConfigVector = pL3ReadyMO->getVRRPEIntfConfig();
			if (vrrpeSessionConfigVector.size()) {
				VRRPLocalObjectManager::ConfigReplayVRRPEInterfaceConfig(vrrpeSessionConfigVector);
			}
		}
	}

	void VRRPLocalObjectManager::ConfigReplayVRRPInterfaceConfig(vector<WaveManagedObjectPointer<VRRPSessionConfigLocalManagedObject> > &vrrpSessionConfigVector)
	{
		unsigned int idx = 0;
		unsigned int ii = 0;
		VRRPClientIntfConfigSPMessage *msg = NULL;
		trace (TRACE_LEVEL_INFO, "Entering VRRPLocalObjectManager::ConfigReplayVRRPInterfaceConfig");

		for (idx = 0; idx < vrrpSessionConfigVector.size(); idx++)
		{
			UI8 vrid = vrrpSessionConfigVector[idx]->getVrid();
			UI32 advt_int = vrrpSessionConfigVector[idx]->getAdvt_int();
			UI8 prio = vrrpSessionConfigVector[idx]->getPrio();
			bool vrrp_en = vrrpSessionConfigVector[idx]->getEnable();
			bool preempt = vrrpSessionConfigVector[idx]->getPreempt();
			UI32 hold_time = vrrpSessionConfigVector[idx]->getHold_time();
			string desc = vrrpSessionConfigVector[idx]->getDescription();
			UI32 choice = VRRP_INTF_CONFIG_REPLAY;
			string ifname = vrrpSessionConfigVector[idx]->getifName();
			UI8 iftype = vrrpSessionConfigVector[idx]->getifType();
			bool owner = vrrpSessionConfigVector[idx]->getOwner();
			string track_ifname = "";
			const IpV4Address vip;

			msg = new VRRPClientIntfConfigSPMessage(ifname, iftype, vrid, vip, 0, track_ifname, 0, advt_int, prio, vrrp_en, preempt, hold_time, desc, 0, 0, 0, 0, owner, 0, VRRP_TYPE_STANDARD, choice);

			vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > virtualIPList = vrrpSessionConfigVector[idx]->getVip();
			vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > trackPortList = vrrpSessionConfigVector[idx]->gettrack();
			msg->m_num_vips = virtualIPList.size();
			msg->m_num_track_ports = trackPortList.size();

			for (ii = 0; ii < msg->m_num_vips; ii ++) {
				msg->m_vipList.push_back(virtualIPList[ii]->m_virtual_ip);
			}

			for (ii = 0; ii < msg->m_num_track_ports; ii ++) {
				msg->m_trackList_ifName.push_back(trackPortList[ii]->m_track_ifname);
				msg->m_trackList_ifType.push_back(trackPortList[ii]->m_track_iftype);
				msg->m_trackList_prio.push_back(trackPortList[ii]->m_track_prio);
			}

			trace (TRACE_LEVEL_DEVEL, "sending synchronously to vrrp client");
			sendSynchronouslyToWaveClient("vrrp", msg);
			delete msg;
		}
	}

	void VRRPLocalObjectManager::ConfigReplayVRRPEInterfaceConfig(vector<WaveManagedObjectPointer<VRRPESessionConfigLocalManagedObject> > &vrrpeSessionConfigVector)
	{
		unsigned int idx = 0;
		unsigned int ii = 0;
		VRRPClientIntfConfigSPMessage *msg = NULL;
		trace (TRACE_LEVEL_INFO, "Entering VRRPLocalObjectManager::ConfigReplayVRRPEInterfaceConfig");

		for (idx = 0; idx < vrrpeSessionConfigVector.size(); idx ++)
		{
			UI8 vrid = vrrpeSessionConfigVector[idx]->getVrid();
			UI32 advt_int = vrrpeSessionConfigVector[idx]->getAdvt_int();
			UI8 prio = vrrpeSessionConfigVector[idx]->getPrio();
			bool vrrp_en= vrrpeSessionConfigVector[idx]->getEnable();
			bool preempt = vrrpeSessionConfigVector[idx]->getPreempt();
			UI32 hold_time = vrrpeSessionConfigVector[idx]->getHold_time();
			string desc = vrrpeSessionConfigVector[idx]->getDescription();
			UI32 choice = VRRP_INTF_CONFIG_REPLAY;
			string ifname = vrrpeSessionConfigVector[idx]->getifName();
			UI8 iftype = vrrpeSessionConfigVector[idx]->getifType();
			bool advt_backup = vrrpeSessionConfigVector[idx]->getAdvt_backup();
			UI32 garp_timer = vrrpeSessionConfigVector[idx]->getGarp_timer();
			UI32 backup_advt_int = vrrpeSessionConfigVector[idx]->getBackup_advt_int();
			bool spf = vrrpeSessionConfigVector[idx]->getSpf();
			UI8 rev_prio = vrrpeSessionConfigVector[idx]->getRevPrio();
			string track_ifname = "";
			const IpV4Address vip;

			msg = new VRRPClientIntfConfigSPMessage(ifname, iftype, vrid, vip, 0, track_ifname, 0, advt_int, prio, vrrp_en, preempt, hold_time, desc, advt_backup, garp_timer, backup_advt_int, spf, 0, rev_prio, VRRP_TYPE_EXTENDED, choice);

			vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > virtualIPList = vrrpeSessionConfigVector[idx]->getVip();
			vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > trackPortList = vrrpeSessionConfigVector[idx]->getTrack();
			msg->m_num_vips = virtualIPList.size();
			msg->m_num_track_ports = trackPortList.size();

			for (ii = 0; ii < msg->m_num_vips; ii ++) {
				msg->m_vipList.push_back(virtualIPList[ii]->m_virtual_ip);
			}

			for (ii = 0; ii < msg->m_num_track_ports; ii ++) {
				msg->m_trackList_ifName.push_back(trackPortList[ii]->m_track_ifname);
				msg->m_trackList_ifType.push_back(trackPortList[ii]->m_track_iftype);
				msg->m_trackList_prio.push_back(trackPortList[ii]->m_track_prio);
			}

			trace (TRACE_LEVEL_DEVEL, "sending synchronously to vrrp client");
			sendSynchronouslyToWaveClient("vrrp", msg);
			delete msg;
		}
	}

	vector<string> VRRPLocalObjectManager::getObjectToDelete()
	{   
        vector<string> managedClass;
        managedClass.push_back(VRRPGlobalConfigManagedObject::getClassName());
        return managedClass;
	}
}
