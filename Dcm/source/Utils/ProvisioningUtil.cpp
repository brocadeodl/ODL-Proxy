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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Author : Ganesh Sindigi
 *
 *   Definitions for provisioning utilities
 ***************************************************************************/
#include "ProvisioningUtil.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveManagedObjectToolKit.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Cluster/Local/WaveNode.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

#include <string>
#include <unistd.h>

namespace DcmNs {

    PrismMutex ProvisioningUtil::provisioningMutex;
    set<UI32> ProvisioningUtil::provisionedVlans;

    UI32 ProvisioningUtil::isGvlanEnabled() {
		UI32 gVlanState = GVLAN_STATE_DISABLE;
#if 0
		Attribute *readConfigAttr = new AttributeUI32(&gVlanState, NSM_GVLAN_PERSISTENCE_MO_NAME);
		bool foundConfig = false;
		string configName(NSM_GVLAN_PERSISTENCE_MO_NAME);
		WaveObjectManagerToolKit::getWaveConfigEntry(configName, readConfigAttr, foundConfig);

		if(foundConfig) {
			string configValue;
			readConfigAttr->getPlainString(configValue);
			gVlanState = atoi(configValue.c_str());
			tracePrintf (TRACE_LEVEL_INFO, "ProvisioningUtil::isGvlanEnabled:Current config value of GVlan is:%d.", gVlanState);
		} else {
			tracePrintf (TRACE_LEVEL_FATAL, "ProvisioningUtil::isGvlanEnabled:Current GVlan config is not found !");
		}

#endif
		return gVlanState;
    }
    bool ProvisioningUtil::isVlanProvisioned(const UI32 vlanId) {
        if (ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE) {
            return true;
        }

        bool provisioned = false;
        provisioningMutex.lock();
        set<UI32>::iterator it = provisionedVlans.find(vlanId);
        provisioned = (it != provisionedVlans.end());
        provisioningMutex.unlock();
        return provisioned;
    }

    void ProvisioningUtil::addProvisionedVlan(const UI32 vlanId) {
        provisioningMutex.lock();
        provisionedVlans.insert(vlanId);
        provisioningMutex.unlock();
    }

    void ProvisioningUtil::removeUnProvisionedVlan(const UI32 vlanId) {
        provisioningMutex.lock();
        provisionedVlans.erase(vlanId);
        provisioningMutex.unlock();
    }


    void ProvisioningUtil::addProvisionedVlan(const UI32Range &vlanIdRange) {
        provisioningMutex.lock();
        vector<UI32> vlans;
        vlanIdRange.getUI32RangeVector(vlans);
        provisionedVlans.insert(vlans.begin(), vlans.end());
        provisioningMutex.unlock();
    }

    void ProvisioningUtil::removeUnProvisionedVlan(const UI32Range &vlanIdRange) {
        provisioningMutex.lock();
        vector<UI32> vlans;
        vlanIdRange.getUI32RangeVector(vlans);
        for(vector<UI32>::iterator it=vlans.begin(); it != vlans.end(); ++it) {
            provisionedVlans.erase(*it);
        }
        provisioningMutex.unlock();
    }

    void ProvisioningUtil::printProvisionedVlans() {
        provisioningMutex.lock();
        const vector<UI32> vlans(provisionedVlans.begin(), provisionedVlans.end());
        UI32Range range(vlans);
        tracePrintf(TRACE_LEVEL_INFO,
                "ProvisioningUtil::printProvisionedVlans : "
                "provisionedVlanRange = %s",
                range.toString().c_str());
        provisioningMutex.unlock();
    }

    bool ProvisioningUtil::isMOProvisioned(const ObjectId &moObjectId,
                const LocationId locationId) {
        return false;
    }

    bool ProvisioningUtil::isMOProvisioned (
            WaveManagedObjectSynchronousQueryContext* syncQueryCtxt, 
            const LocationId locationId) {
        return true;
    }


    ResourceId ProvisioningUtil::populateProvisionedMOIds(
            vector<ObjectId> &provisionedObjectIds,
            const string &moClassName, const LocationId locationId) {
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId ProvisioningUtil::populateProvisionedMOs(
            vector<WaveManagedObject *> &provisionedMOs,
            const string &moClassName, const LocationId locationId) {
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId ProvisioningUtil::populateProvisionedLocationIdsForMO (
            vector<LocationId> &locationIds,
            WaveManagedObjectSynchronousQueryContext* syncQueryCtxt) {

        DcmToolKit::getFullyConnectedLocationsSortedOnDomainId(locationIds, true);
        //ResourceId status = WAVE_MESSAGE_SUCCESS;

        /*syncQueryCtxt->addSelectField("provisionedOnLocations");
        vector<WaveManagedObject *> *pResults = WaveObjectManagerToolKit::querySynchronously (syncQueryCtxt);
        vector<ObjectId> listOfObjIds;

        if (NULL != pResults) {
            UI32 numberOfResults = pResults->size ();
            if (0 == numberOfResults) {
                status = WAVE_MESSAGE_ERROR;
                //TODO LOGGING
            } else if ( 1 == numberOfResults) {
                WaveGlobalProvisionedManagedObjectBase *pWaveManagedObject  
                    = dynamic_cast<WaveGlobalProvisionedManagedObjectBase*>((*pResults)[0]);
                if(pWaveManagedObject != NULL) {
                    listOfObjIds = pWaveManagedObject->getProvisionedOnLocations();
                }
            } else {
                status = WAVE_MESSAGE_ERROR;
                //TODO LOGGING
            }
        }
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);


        //Get the list of WaveNode and get the vector of location ids.
        WaveManagedObjectSynchronousQueryContext waveNodeSyncCtxt(WaveNode::getClassName());
        waveNodeSyncCtxt.addObjectIds(listOfObjIds);
        waveNodeSyncCtxt.addSelectField("locationId");
        vector<WaveManagedObject *> *waveNodeMOs = WaveObjectManagerToolKit::querySynchronously(&waveNodeSyncCtxt);
        vector<LocationId> toBeReturned;

        if(NULL != waveNodeMOs) {
            UI32 numberOfProvisionedNodes = waveNodeMOs->size();
            if (0 == numberOfProvisionedNodes) {
            } else {
                for(UI32 waveNodeCount = 0; waveNodeCount < numberOfProvisionedNodes; waveNodeCount++) {
                    WaveNode *pWaveManagedObject
                        = dynamic_cast<WaveNode*>((*pResults)[waveNodeCount]);
                    if(pWaveManagedObject != NULL) {
                        toBeReturned.push_back(pWaveManagedObject->getLocationId());
                    }
                }
            }
        }
        WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pResults);*/

        return WAVE_MESSAGE_SUCCESS; //TODO : return status here.

    }

    UI32Range ProvisioningUtil::getProviosionedMORange() {
        vector<UI32> provisionedMOVector(provisionedVlans.begin(),provisionedVlans.end());
        UI32Range range(provisionedMOVector);
        return range;
    }


}
