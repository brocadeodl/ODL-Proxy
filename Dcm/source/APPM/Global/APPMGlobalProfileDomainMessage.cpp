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

#include "APPM/Global/APPMGlobalProfileDomainMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalProfileDomainMessage::APPMGlobalProfileDomainMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALPORTPROFILEDOMAIN) {
	}
    APPMGlobalProfileDomainMessage::~APPMGlobalProfileDomainMessage() {
    }

    void  APPMGlobalProfileDomainMessage::setupAttributesForSerialization() {

        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_portProfileName,"portProfileName"));
        addSerializableAttribute (new AttributeString(&m_portProfileDomainName,"portProfileDomainName"));
    }

    void  APPMGlobalProfileDomainMessage::setOpCode(const UI8 &opCode) {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalProfileDomainMessage::getOpCode() const {
        return (m_opCode);
    }

    void  APPMGlobalProfileDomainMessage::setCmdCode(const UI8 &cmdCode) {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalProfileDomainMessage::getCmdCode() const {
        return (m_cmdCode);
    }

    void  APPMGlobalProfileDomainMessage::setPortProfileName(const string &portProfileName) {
        m_portProfileName  =  portProfileName;
    }

    string  APPMGlobalProfileDomainMessage::getPortProfileName() const {
        return (m_portProfileName);
    }

    void  APPMGlobalProfileDomainMessage::setPortProfileDomainName(const string &portProfileDomainName) {
        m_portProfileDomainName  =  portProfileDomainName;
    }

    string  APPMGlobalProfileDomainMessage::getPortProfileDomainName() const
    {
        return (m_portProfileDomainName);
    }


}
