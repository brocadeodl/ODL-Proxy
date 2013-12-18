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
 *   Definitions for GVLAN configuration/provisioning utilities
 ***************************************************************************/

#include <vector>

#include "GvlanConfigUtils.h"

#include "ClientInterface/Ssm/SsmApp.h"
#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Nsm/Global/VlanIntfManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Utils/mo_iterator.h"

namespace DcmNs {

    GvlanConfigUtils::GvlanConfigUtils(DceContext *context): dceContext(context){}
    GvlanConfigUtils::~GvlanConfigUtils() {}

    ResourceId GvlanConfigUtils::configReplay(const UI32Range &vlanRange,
            stack<UI32Range> &order) {
        // when there is no order specified, we just need to configure vlanRange
        if (order.empty()) {
            UI32Range curRange = vlanRange;
            return configReplay(curRange);
        }

        ResourceId status = WAVE_MESSAGE_SUCCESS;
        // Pop range from 'order' stack, take an intersection with 'vlanRange'
        // do the config replay
        while (WAVE_MESSAGE_SUCCESS == status && !order.empty()) {
            UI32Range curRange = order.top();
            UI32Range provisionedVlans = curRange.intersection(vlanRange);
            order.pop();
            status = configReplay(provisionedVlans);
        }

        return status;
    }

    ResourceId GvlanConfigUtils::configReplay(UI32Range &vlanRange) {

        if (vlanRange.isEmpty()) {
            return WAVE_MESSAGE_SUCCESS;
        }

        NsmLocalObjectManager *nsmLocalObjMgr =
                NsmLocalObjectManager::getInstance();

        tracePrintf(TRACE_LEVEL_INFO,
                "GvlanConfigUtils::configReplay : Config replay for  %s ",
                vlanRange.toString().c_str());

        mo_criteria vlanCriteria(VlanIntfManagedObject::getClassName());
        vlanCriteria.getQueryContext()->setAttributeRangeForQuery(
                new AttributeUI32Range(vlanRange, "id"));

        vector<WaveManagedObject *> *pVlans = vlanCriteria.query();

        nsmLocalObjMgr->postBootPass1Configuration(pVlans, false);

        //MstpLocalObjectManager::configReplayVlanBridgeConfig(curRange);
        SsmLocalObjectManager::configReplayMacAclGlobalIntf(VLAN_INTF_TYPE,
                pVlans);

        nsmLocalObjMgr->configReplayIgmpsIntfConfig(pVlans);

        if (NULL != dceContext) {
            dceContext->addManagedObjectsForGarbageCollection(*pVlans);
            pVlans->clear();
            delete pVlans;
        } else {
            WaveManagedObjectToolKit::releaseMemoryOfWaveMOVector(pVlans);
        }

        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId GvlanConfigUtils::populateDependentPrivateVlans(
            const UI32Range &vlanRange, stack<UI32Range> &vlans) {

        if (vlanRange.isEmpty()) {
            return WAVE_MESSAGE_SUCCESS;
        }
        UI32Range reqdRange(vlanRange);
        do {
            vlans.push(reqdRange);
            // VLAN is dependent on other vlans which are private - vlan
            mo_iterator<VlanIntfManagedObject> iter(dceContext);
            iter.criteria().getQueryContext()->setAttributeRangeForQuery(
                    new AttributeUI32Range(reqdRange, "id"));
            /*iter.criteria().And(
             new AttributeUI32Range(UI32Range(), "pVlanSecAssocRange"),
             WAVE_ATTRIBUTE_CONDITION_OPERATOR_NOT_EQUAL);*/
            iter.criteria().select("pVlanSecAssocRange");

            UI32Range allRange, curRange;
            while (iter.hasNext()) {
                curRange = iter.next()->getPvlanSecAssocRange();
                if (!curRange.isEmpty()) {
                    allRange = allRange + curRange;
                }
            }
            reqdRange = allRange;
        } while (!reqdRange.isEmpty());

        return WAVE_MESSAGE_SUCCESS;

    }

    ResourceId GvlanConfigUtils::provision(const UI32 vlanId) {
        if (ProvisioningUtil::isVlanProvisioned(vlanId)) {
            return WAVE_MESSAGE_SUCCESS;
        }
        UI32Range vlanRange(vlanId, vlanId);
        return provision(vlanRange, PROVISION_SPECIFIED);

    }

    /**
     * Send provision/unprovision trigger to nsm. Updates the provisioned vlan
     * inventory if some of the vlans are actually provisioned or unprovisioned
     * as part of this request. If the vlans are provisioned the "vlan postboot"
     * will also be run.
     *
     * Parameters:
     *  (1) vlanRange       Range of vlans to be provisioned/unprovisioned.
     *  (2) provisionCode   Request code.
     *  (3) needsExplicitUnprovision
     *                      Boolean flag indicating if the vlans being
     *                      provisioned expect explict unprovision request. All
     *                      configurations which need provisioning but are not
     *                      tracked by nsm need to set this flag to true.
     *                      Defaults to false.. Value is ignored for unprovision
     *                      requests.
     *
     * Returns WAVE_MESSAGE_SUCCESS if everything goes smooth, otherwise returns
     * a ResourceId for the error..
     */
    ResourceId GvlanConfigUtils::provision(const UI32Range &vlanRange,
            const vlan_provision_code provisionCode,
            bool needsExplicitUnprovision) {
        const char* func = __FUNCTION__;
        if (ProvisioningUtil::isGvlanEnabled() == GVLAN_STATE_DISABLE) {
            tracePrintf(TRACE_LEVEL_INFO,
                    "GvlanConfigUtils::provision : Gvlan is not enabled... ");
            return WAVE_MESSAGE_SUCCESS;
        }

        if (((provisionCode == PROVISION_SPECIFIED) ||
                provisionCode == PROVISION_EXCEPT ||
                PROVISION_NOT_REQUIRED == provisionCode ) && vlanRange.isEmpty()) {
            tracePrintf(TRACE_LEVEL_WARN,
                    "GvlanConfigUtils::provision : Empty range is being triggered.");
            return WAVE_MESSAGE_SUCCESS;
        }

        // Get the dependent vlans for the given range
        stack<UI32Range> dependentVlans;
        UI32Range combinedRange;
        if (PROVISION_SPECIFIED == provisionCode) {
            populateDependentPrivateVlans(vlanRange, dependentVlans);

            // Make a copy of dependent vlans so that it can be used for making a
            // combined range
            stack<UI32Range> combinedVlans(dependentVlans);
            while (!combinedVlans.empty()) {
                UI32Range curRange = combinedVlans.top();
                combinedVlans.pop();
                if (!curRange.isEmpty()) {
                    combinedRange = combinedRange + curRange;
                }
            }
        } else {
            combinedRange = vlanRange;
        }

        NsmVlanProvisionMessage *m = new NsmVlanProvisionMessage();
        m->setProvisionVlanRange(combinedRange);
        m->setProvisionCode(provisionCode);
        m->setNeedsExplicitUnprovision(needsExplicitUnprovision);

        tracePrintf(TRACE_LEVEL_INFO,
                "GvlanConfigUtils::%s : provisionCode = %d, vlanRange = %s",
                func, provisionCode, combinedRange.toString().c_str());
        ResourceId status = sendToBackend(NSM_DAEMON, m);

        if (status == WAVE_MESSAGE_SUCCESS) {
            HANDLE_PROVISION_RESPONSE(m);
            /* Replay the configuration for VLANs which are provisioned now */
            auto_ptr<UI32Range> provisionedVlans = m->getVlanRangeForStatus(NSM_VLAN_PROVISIONED_NOW);
            if (!provisionedVlans->isEmpty()) {
                status = configReplay(*provisionedVlans, dependentVlans);
            }
        }
        delete m;

        return status;
    }

    ResourceId GvlanConfigUtils::provisionAllVlans() {
        UI32Range vlanRange;
        return GvlanConfigUtils::provision(vlanRange, PROVISION_ALL);
    }

    ResourceId GvlanConfigUtils::sendToBackend (const char *daemon,
            DcmManagementInterfaceMessage *pMessage)
    {
        const char* func = __FUNCTION__;

        UI32 opCode = pMessage->getOperationCode();
        tracePrintf(TRACE_LEVEL_DEBUG,
                "GvlanConfigUtils::%s : Sending Message-<%u> to %s...",
                func, opCode, daemon);

        ResourceId status =
                WaveObjectManagerToolKit::sendSynchronouslyToWaveClient(daemon, pMessage);

        if (status != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR,
                    "GvlanConfigUtils::%s : Cannot send Message-<%u> to %s... err=%u ( %s )",
                    func, opCode, daemon, status, FrameworkToolKit::localize(status).c_str());
            return status;
        }

        status = pMessage->getCompletionStatus();
        if (status != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR,
                    "GvlanConfigUtils::%s : Message-<%u> processing failed in %s... err=%u ( %s )",
                    func, opCode, daemon, status, FrameworkToolKit::localize(status).c_str());
            return status;
        }

        status = pMessage->getClientStatus();
        if (status != 0 && status != WAVE_MESSAGE_SUCCESS) {
            tracePrintf(TRACE_LEVEL_ERROR,
                    "GvlanConfigUtils::%s : For message-<%u>, %s returned %u. ( %s )",
                    func, opCode, daemon, status,
                    FrameworkToolKit::localize(status).c_str());
            return status;
        }

        tracePrintf(TRACE_LEVEL_DEBUG,
                "GvlanConfigUtils::%s : Sending Message-<%u> to %s was "
                "successful. Return status=%d.",
                func, opCode, daemon, status);
        return WAVE_MESSAGE_SUCCESS;
    }

    ResourceId GvlanConfigUtils::triggerUnprovisionEligibleVlans() {
        tracePrintf(TRACE_LEVEL_INFO,
                "GvlanConfigUtils::triggerUnprovisionEligibleVlans : "
                "Triggering unprovision code= %d....",
                UNPROVISION_ELIGIBLE_VLANS);
        UI32Range vlanRange;
        return GvlanConfigUtils::provision(vlanRange, UNPROVISION_ELIGIBLE_VLANS);
    }
}
