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

/**********************************************************
 *  Copyright (C) 2011 Brocade Communications Systems, Inc.
 *  All rights reserved.
 *
 *  Created on: Dec 20, 2011
 *      Author: statpatt
 *
 *
 **********************************************************/


#include "Nsm/Local/NsmLocalProvisioningInfoMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs {

    NsmLocalProvisioningInfoMessage::NsmLocalProvisioningInfoMessage()
        : PrismMessage(NsmLocalObjectManager::getPrismServiceId (), PROVISIONING_INFO) {
        }

    NsmLocalProvisioningInfoMessage::~NsmLocalProvisioningInfoMessage() {
    }

    void NsmLocalProvisioningInfoMessage::setProvisionedMORange(const UI32Range provisionedMORange) {
        m_provisionedMORange = provisionedMORange;
    }

    UI32Range NsmLocalProvisioningInfoMessage::getProvisionedMORange() const {
        return m_provisionedMORange;
    }

    void NsmLocalProvisioningInfoMessage::setIsGwRequest(bool isGwRequest) {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalProvisioningInfoMessage::getIsGwRequest() const {
        return m_isGwRequest;
    }

    void NsmLocalProvisioningInfoMessage::setupAttributesForSerialization() {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32Range(&m_provisionedMORange,"provisionedMORange"));
        addSerializableAttribute(new AttributeBool(&m_isGwRequest, "isGwRequest"));
    }
}
