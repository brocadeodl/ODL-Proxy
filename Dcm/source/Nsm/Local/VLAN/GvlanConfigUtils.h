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
 *   Declarations for GVLAN configuration/provisioning utilities
 ***************************************************************************/

#ifndef GVLAN_CONFIG_UTILS_H
#define GVLAN_CONFIG_UTILS_H

#define PROVISION_VLAN(context, vlanId) \
    { \
        GvlanConfigUtils util(context); \
        ResourceId provisionStatus = util.provision(vlanId);\
        if (WAVE_MESSAGE_SUCCESS != provisionStatus) {\
            tracePrintf(TRACE_LEVEL_ERROR,\
                    "Failed to provision Vlan %d status=%u",\
                    vlanId, provisionStatus);\
            return provisionStatus;\
        }\
    }

#define PROVISION_ALL_VLANS(context) \
    { \
        GvlanConfigUtils util(context); \
        ResourceId provisionStatus = util.provisionAllVlans();\
        if (WAVE_MESSAGE_SUCCESS != provisionStatus) {\
            tracePrintf(TRACE_LEVEL_ERROR,\
                    "Failed to provision All Vlans status=%u",\
                    provisionStatus);\
            return provisionStatus;\
        }\
    }

#define PROVISION_VLAN_RANGE(context, vlanRange, provisionCode) \
    { \
        GvlanConfigUtils util(context); \
        ResourceId provisionStatus = util.provision( \
                vlanRange, provisionCode);\
        if (WAVE_MESSAGE_SUCCESS != provisionStatus) {\
            tracePrintf(TRACE_LEVEL_ERROR,\
                    "Failed to provision Vlan %s status=%u",\
                    vlanRange.toString().c_str(), provisionStatus);\
            return provisionStatus;\
        }\
    }

#define HANDLE_PROVISION_RESPONSE(msg) \
    { \
        auto_ptr<UI32Range> unProvisionedVlans = msg->getVlanRangeForStatus(NSM_VLAN_UNPROVISIONED_NOW);\
        if (!unProvisionedVlans->isEmpty()) {\
            ProvisioningUtil::removeUnProvisionedVlan(*unProvisionedVlans);\
        }\
        \
        /* Replay the configuration for VLANs which are provisioned now */ \
        auto_ptr<UI32Range> provisionedVlans = msg->getVlanRangeForStatus(NSM_VLAN_PROVISIONED_NOW);\
        if (!provisionedVlans->isEmpty()) {\
            ProvisioningUtil::addProvisionedVlan(*provisionedVlans);\
            ProvisioningUtil::printProvisionedVlans();\
        } \
        /* How to deal with failed-ivid allocations? */ \
    }

#include <string>
#include <stack>

#include "vcs.h"

#include "ClientInterface/Nsm/nsmdVlanIntf.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/ObjectModel/WaveObjectManagerToolKit.h"
#include "Framework/Types/UI32Range.h"
#include "Utils/DceContext.h"
#include "Utils/ProvisioningUtil.h"

using namespace std;
using namespace WaveNs;

namespace DcmNs {


/**
 * Utilities to deal with GVLAN configuration/provisioning.
 */

class GvlanConfigUtils : public WaveObjectManagerToolKit {

public:

private:
    ResourceId sendToBackend (const char *daemon,
                DcmManagementInterfaceMessage *pMessage);

    ResourceId configReplay(const UI32Range &vlanRange,
            stack<UI32Range> &order);

    /**
     * Replays the configuration of the specified VLAN range.
     */
    ResourceId configReplay(UI32Range &vlanRange);

    /**
     * Populates the given VLANs' container with private VLAN ranges which the
     * input VLAN range is dependent on.
     */
    ResourceId populateDependentPrivateVlans(const UI32Range &vlanRange,
            stack<UI32Range> &vlans);

public:
    GvlanConfigUtils(DceContext *context);
    ~GvlanConfigUtils();

    /**
     * Provisions the specified VLAN range.
     */
    ResourceId provision(const UI32Range &vlanRange,
            const vlan_provision_code provisionCode,
            bool needsExplicitUnprovision = false) ;


    /**
     * Provisions the specified VLAN, if it is not provisioned already.
     */
    ResourceId provision(const UI32 vlanId) ;

    /**
     * Provisions all VLANs.
     */
    ResourceId provisionAllVlans();

    /**
     * Triggers un-provision for eligible VLANs.
     */
    ResourceId triggerUnprovisionEligibleVlans();

private:
    DceContext *dceContext;

};

}

#endif //GVLAN_CONFIG_UTILS_H
