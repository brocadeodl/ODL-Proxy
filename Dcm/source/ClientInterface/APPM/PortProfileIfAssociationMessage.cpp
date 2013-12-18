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

/****************************************************************************
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.				*
 *   All rights reserved.													*
 *   Author : statpatt														*
 ****************************************************************************/

#include "ClientInterface/APPM/PortProfileIfAssociationMessage.h"
#include "ClientInterface/APPM/APPMMessageDef.h"
#include "ClientInterface/Nsm/nsmdVlanIntf.h"

namespace DcmNs {

    PortProfileInterfaceAssociationMessage::PortProfileInterfaceAssociationMessage ()
        : DcmManagementInterfaceMessage (PORT_PROFILE_INTERFACE_ASSOCIATION) {
        }

    PortProfileInterfaceAssociationMessage::~PortProfileInterfaceAssociationMessage () {
    }

    void PortProfileInterfaceAssociationMessage::setupAttributesForSerialization() {

        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
        addSerializableAttribute (new AttributeStringVector(&m_associatedPortProfiles, "associatedPortProfiles"));
        addSerializableAttribute (new AttributeString(&m_associatedPortProfileDomain, "associatedPortProfileDomain"));
    }

    void PortProfileInterfaceAssociationMessage::setCmdCode(const UI32 &cmdCode) {
        m_cmdCode = cmdCode;
    }
    UI32 PortProfileInterfaceAssociationMessage::getCmdCode() const {
        return m_cmdCode;
    }
    void PortProfileInterfaceAssociationMessage::setIfType(const UI32 &ifType) {
        m_ifType = ifType;
    }
    UI32 PortProfileInterfaceAssociationMessage::getIfType() const {
        return m_ifType;
    }
    void PortProfileInterfaceAssociationMessage::setIfName(const string &ifName) {
        m_ifName = ifName;
    }
    string PortProfileInterfaceAssociationMessage::getIfName() const {
        return m_ifName;
    }
    void PortProfileInterfaceAssociationMessage::setAssociatedPortProfiles(const vector<string> &associatedPP) {
        m_associatedPortProfiles = associatedPP;
    }
    vector<string> PortProfileInterfaceAssociationMessage::getAssociatedPortProfiles() const {
        return m_associatedPortProfiles;
    }
    void PortProfileInterfaceAssociationMessage::setAssociatedPortProfileDomain(const string &associatedPPDomain) {
        m_associatedPortProfileDomain = associatedPPDomain;
    }
    string PortProfileInterfaceAssociationMessage::getAssociatedPortProfileDomain() const {
        return m_associatedPortProfileDomain;
    }

    const void *PortProfileInterfaceAssociationMessage::getCStructureForInputs () {

        interfaceToPortProfileAssociation_t *pInput = NULL;
        unsigned int numRecords = m_associatedPortProfiles.size();

        pInput = (interfaceToPortProfileAssociation_t *)calloc(1, (sizeof(interfaceToPortProfileAssociation_t) 
                    + (numRecords - 1) * sizeof(portProfile_t)));
        pInput->cmdCode = m_cmdCode;
        pInput->ifType  = m_ifType;
        snprintf(pInput->ifName, (MAX_INTERFACE_NAME_LEN + 1), "%s", m_ifName.c_str()); 
        snprintf(pInput->portProfileDomain.portProfileDomainName, 
                (MAX_WYSEREA_PROFILE_NAME_LEN + 1), "%s", m_associatedPortProfileDomain.c_str());

        pInput->numberOfPortProfiles = numRecords;
        unsigned int iterator = 0;

        for(iterator = 0 ; iterator < numRecords ; iterator++) {
            snprintf(pInput->portProfiles[iterator].portProfileName, (MAX_WYSEREA_PROFILE_NAME_LEN + 1),
                    "%s", m_associatedPortProfiles[iterator].c_str());
        }

        return pInput;
    }

    void PortProfileInterfaceAssociationMessage::loadOutputsFromCStructure (const void *pOutputCStructure) {

    }

}

