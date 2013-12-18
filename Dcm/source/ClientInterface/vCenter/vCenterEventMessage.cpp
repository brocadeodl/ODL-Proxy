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
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#include "ClientInterface/vCenter/vCenterEventMessage.h"
#include "ClientInterface/vCenter/vCenterMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/vCenter/vCenterClientParseUtility.h"

namespace DcmNs {

    vCenterEventMessage::vCenterEventMessage() :
            DcmManagementInterfaceMessage(VCENTEREVENTS) {

    }

    vCenterEventMessage::vCenterEventMessage(const UI8 &opCode,
            const UI8 &cmdCode) :
            DcmManagementInterfaceMessage(VCENTEREVENTS), m_opCode(opCode), m_cmdCode(
                    cmdCode)

    {
    }

    vCenterEventMessage::~vCenterEventMessage() {
    }

    void vCenterEventMessage::setupAttributesForSerialization() {
        DcmManagementInterfaceMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI8(&m_opCode, "opCode"));
        addSerializableAttribute(new AttributeUI8(&m_cmdCode, "cmdCode"));

    }

    const void * vCenterEventMessage::getCStructureForInputs() {
        UI32 size;
        vnm_resp_t *temp_resp = (vnm_resp_t *) findBuffer(NOTIFICATION_PLD_TAG,
                size);
        //buffer would get cleaned up by framework
        //TODO validate the same

        ///Allocate memory to hold the response to be consumed by B.E
        vnm_resp_t *resp = (vnm_resp_t *) malloc(size);
        memcpy(resp, temp_resp, size);

        return (resp);
    }

    void vCenterEventMessage::loadOutputsFromCStructure(
            const void *pOutputCStructure) {
    }

    UI8 vCenterEventMessage::getOpCode() const {
        return m_opCode;
    }

    void vCenterEventMessage::setOpCode(UI8 opCode) {
        m_opCode = opCode;
    }


}

