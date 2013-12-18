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
 *   Definitions for VLAN provisioning message.
 ***************************************************************************/

#include <map>
#include <vector>

#include "ClientInterface/Nsm/NsmVlanProvisionMessage.h"
#include "Framework/Attributes/AttributeUI32Range.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "Framework/Attributes/Attributes.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs {

    void NsmVlanProvisionMessage::setupAttributesForSerialization() {
        DcmManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(
                new AttributeEnum((UI32*) &m_provisionCode, "provisionCode"));
        addSerializableAttribute(
                new AttributeUI32Range(&m_provisionVlanRange,
                        "provisionVlanRange"));
        addSerializableAttribute(
                new AttributeBool(&m_needsExplicitUnprovision, "needsUnprov"));
    }

    NsmVlanProvisionMessage::NsmVlanProvisionMessage(const UI32 opCode) :
            DcmManagementInterfaceMessage(opCode), m_provisionVlanRange(
                    UI32Range()), buffersRead(false) {
        m_needsExplicitUnprovision = false;
    }

    NsmVlanProvisionMessage::NsmVlanProvisionMessage() :
            DcmManagementInterfaceMessage(NSM_VLAN_PROVISION), m_provisionVlanRange(
                    UI32Range()), buffersRead(false) {
        m_needsExplicitUnprovision = false;
    }

    NsmVlanProvisionMessage::~NsmVlanProvisionMessage() {
        for (map<vlan_provision_status, vector<UI32> >::iterator it =
                mapResponse.begin(); it != mapResponse.end(); it++) {
            it->second.clear();
        }
        mapResponse.clear();
    }

    void NsmVlanProvisionMessage::interpretBuffers() {
        UI32 size;
        nsm_vlan_provision_response_t *response =
                (nsm_vlan_provision_response_t*) findBuffer(NSM_VLAN_PROVISION,
                        size);
        if (response == NULL) {
            tracePrintf(TRACE_LEVEL_INFO,
                    "NsmVlanProvisionMessage::interpretBuffers: No Buffers! ");
            return;
        }

        map<vlan_provision_status, vector<UI32> >::iterator itResponse;
        UI32 vlanId;
        vlan_provision_status provisionStatus;
        for (UI32 i = 0; i < response->number_of_records; i++) {
            vlanId = response->status[i].vlan_id;
            provisionStatus = response->status[i].provision_status;
            tracePrintf(TRACE_LEVEL_INFO,
                    "NsmVlanProvisionMessage::interpretBuffers:VLAN=%d "
                            "ProvisionStatus = %d", vlanId, provisionStatus);
            mapResponse[provisionStatus].push_back(vlanId);

        }
    }

    void NsmVlanProvisionMessage::setProvisionCode(
            const vlan_provision_code provisionCode) {
        m_provisionCode = provisionCode;
    }

    vlan_provision_code NsmVlanProvisionMessage::getProvisionCode() const {
        return m_provisionCode;
    }

    bool NsmVlanProvisionMessage::getNeedsExplicitUnprovision () const {
        return m_needsExplicitUnprovision;
    }

    void NsmVlanProvisionMessage::setNeedsExplicitUnprovision (bool flag) {
        m_needsExplicitUnprovision = flag;
    }

    UI32Range NsmVlanProvisionMessage::getProvisionVlanRange() const {
        return m_provisionVlanRange;
    }

    void NsmVlanProvisionMessage::setProvisionVlanRange(
            const UI32Range &vlanRange) {
        m_provisionVlanRange = vlanRange;
        tracePrintf(TRACE_LEVEL_DEVEL,
                "NsmVlanProvisionMessage::setProvisionVlanRange:VLANRange=%s ",
                m_provisionVlanRange.toString().c_str());
    }

    auto_ptr<UI32Range> NsmVlanProvisionMessage::getVlanRangeForStatus(
            const vlan_provision_status provisionStatus) {
        if (!buffersRead) {
            interpretBuffers();
            buffersRead = true;
        }
        map<vlan_provision_status, vector<UI32> >::const_iterator it =
                mapResponse.find(provisionStatus);
        UI32Range *range;
        if (it == mapResponse.end()) {
            tracePrintf(
                    TRACE_LEVEL_DEVEL,
                    "NsmVlanProvisionMessage::getProvisionVlanRange: Didn't find  "
                            "vlans for provisionStatus=%d", provisionStatus);
            range = new UI32Range();
        } else {
            vector<UI32> v = it->second;
            range = new UI32Range(UI32Range::getUI32RangeStringFromVector(v));
        }
        tracePrintf(TRACE_LEVEL_DEVEL, true, false,
                "NsmVlanProvisionMessage::getProvisionVlanRange: Range for "
                        "provisionStatus %d is \"%s\"", provisionStatus,
                range->toString().c_str());
        auto_ptr<UI32Range> apRange(range);
        return apRange;
    }

    const void* NsmVlanProvisionMessage::getCStructureForInputs() {
        string range = m_provisionVlanRange.toString();
        UI32 rangeLength = range.length();
        size_t request_size = sizeof(nsm_vlan_provision_request_t);

        nsm_vlan_provision_request_t *request =
                (nsm_vlan_provision_request_t*) malloc(
                        request_size + rangeLength + 1);
        request->vlan_range = ((char*) request) + request_size;

        request->provision_code = m_provisionCode;
        request->need_explicit_unprovision = m_needsExplicitUnprovision;
        tracePrintf(TRACE_LEVEL_DEVEL,
                "getCStructureForInputs:VLANRange=%s Provision = %d",
                range.c_str(), request->provision_code);

        strncpy(request->vlan_range, range.c_str(), rangeLength);
        request->vlan_range[rangeLength] = '\0';
        tracePrintf(TRACE_LEVEL_DEVEL,
                "getCStructureForInputs:Request: VLANRange=%s Provision = %d",
                request->vlan_range, request->provision_code);
        return request;
    }

    void NsmVlanProvisionMessage::loadOutputsFromCStructure(
            const void* pOutputCStructure) {
        nsm_vlan_provision_response_t *response =
                (nsm_vlan_provision_response_t*) pOutputCStructure;

        if (response == NULL) {
            tracePrintf(TRACE_LEVEL_DEBUG,
                    "NsmVlanProvisionMessage::loadOutputsFromCStructure: "
                            "Did not receive status from backend daemon!");
            return;
        }
        if (0 == response->number_of_records) {
            tracePrintf(TRACE_LEVEL_DEBUG,
                    "NsmVlanProvisionMessage::loadOutputsFromCStructure: "
                            "No VLANs are provisioned!");
            return;
        }
        UI32 vlanId;
        vlan_provision_status provisionStatus;
        for (UI32 i = 0; i < response->number_of_records; i++) {
            vlanId = response->status[i].vlan_id;
            provisionStatus = response->status[i].provision_status;
            tracePrintf(TRACE_LEVEL_DEBUG,
                    "loadOutputsFromCStructure:VLAN=%d ProvisionStatus =  %d",
                    vlanId, provisionStatus);
        }
        // TODO:when should response be cleared?
        UI32 number_of_records = response->number_of_records;
        addBuffer(
                NSM_VLAN_PROVISION,
                (sizeof(nsm_vlan_provision_response_t)
                        + ((number_of_records - 1)
                                * sizeof(vlan_provision_status_record_t))),
                pOutputCStructure, false);

    }
}
