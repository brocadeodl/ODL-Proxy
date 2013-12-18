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
 **************************************************************************/

#include "ClientInterface/APPM/APPMPortProfileDomainConfigMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    APPMPortProfileDomainConfigMessage::APPMPortProfileDomainConfigMessage ()
        : DcmManagementInterfaceMessage (APPMPORTPROFILEDOMAIN) {
    }

    APPMPortProfileDomainConfigMessage::~APPMPortProfileDomainConfigMessage () {
    }

    void  APPMPortProfileDomainConfigMessage::setupAttributesForSerialization() {
		DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_portProfileName,"profileName"));
        addSerializableAttribute (new AttributeString(&m_portProfileDomainName, "portProfileDomainName"));
	}

    void  APPMPortProfileDomainConfigMessage::setOpCode(const UI8 &opCode) {
        m_opCode  =  opCode;
    }

    UI8  APPMPortProfileDomainConfigMessage::getOpCode() const {
        return (m_opCode);
    }

    void  APPMPortProfileDomainConfigMessage::setCmdCode(const UI8 &cmdCode) {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMPortProfileDomainConfigMessage::getCmdCode() const {
        return (m_cmdCode);
    }

    void  APPMPortProfileDomainConfigMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName  =  portProfileName;
    }

    string  APPMPortProfileDomainConfigMessage::getPortProfileName() const {
        return (m_portProfileName);
    }
    void APPMPortProfileDomainConfigMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName = portProfileDomainName;
    }

    string APPMPortProfileDomainConfigMessage::getPortProfileDomainName() const {
        return m_portProfileDomainName;
    }

    const void *APPMPortProfileDomainConfigMessage::getCStructureForInputs () {

        portProfileDomainMessage_t *pInput = new portProfileDomainMessage_t;

        pInput->opCode = m_opCode; 
        pInput->cmdCode = m_cmdCode; 
        snprintf(pInput->portProfileDomainName, (MAX_WYSEREA_PROFILE_NAME_LEN + 1), "%s"
                ,m_portProfileDomainName.c_str());
        snprintf(pInput->portProfileName, (MAX_WYSEREA_PROFILE_NAME_LEN + 1), "%s"
                ,m_portProfileName.c_str());

        //trace (TRACE_LEVEL_INFO, string("APPMPortProfileDomainConfigMessage::getCStructureForInputs: Opcode<") + pInput->opCode );
		//trace (TRACE_LEVEL_INFO, string(">CmdCode<") + pInput->cmdCode );
		//trace (TRACE_LEVEL_INFO, string(">PortProfileDomainName<") + pInput->portProfileDomainName);
        //trace (TRACE_LEVEL_INFO, string(">PortProfileName<") + pInput->portProfileName + ">\n");

        return (pInput);
    }

	void APPMPortProfileDomainConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
