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

#include "discover/Framework/MigrationEventsMessage.h"
#include "discover/Framework/EventEngineTypes.h"
#include "discover/Framework/EventEngineObjectManager.h"
#include "Framework/Attributes/AttributeStringVectorVector.h"

namespace DcmNs {

    MigrationEventsMessage::MigrationEventsMessage() :
            ManagementInterfaceMessage(EventEngineObjectManager::getClassName(),
                    EVENTENGINE_VCENTER_EVENTS_NOTIFY) {
    }

    MigrationEventsMessage::MigrationEventsMessage(UI32 &m_eventID,
            UI32 &m_eventChainID, UI32 &m_eventType, string &m_vCenterIP,
            string &m_hostMOR, string &m_destHostMOR, string &m_vmMOR,

            vector<vector<string> >  &m_vmmacaddrList,

            string &m_srcConsoleIP, vector<vector<string> > &m_srckernelIPList,


            string &m_destConsoleIP,vector<vector<string> > &m_destKernelIPList
            ) :
            ManagementInterfaceMessage(EventEngineObjectManager::getClassName(),
                    EVENTENGINE_VCENTER_EVENTS_NOTIFY), eventID(m_eventID), eventChainID(
                    m_eventChainID), eventType(m_eventType), vCenterIP(
                    m_vCenterIP), hostMOR(m_hostMOR), destHostMOR(
                    m_destHostMOR), vmMOR(m_vmMOR),  vmmacaddrList(m_vmmacaddrList),
                    srcConsoleIP(
                    m_srcConsoleIP), srckernelIPList(m_srckernelIPList),
                    destConsoleIP(
                    m_destConsoleIP), destKernelIPList(m_destKernelIPList)
    {
    }

    MigrationEventsMessage::~MigrationEventsMessage() {
    }

    void MigrationEventsMessage::setupAttributesForSerialization() {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeUI32(&eventID, "eventID"));
        addSerializableAttribute(
                new AttributeUI32(&eventChainID, "eventChainID"));
        addSerializableAttribute(new AttributeUI32(&eventType, "eventType"));
        addSerializableAttribute(new AttributeString(&vCenterIP, "vCenterIP"));
        addSerializableAttribute(new AttributeString(&hostMOR, "hostMOR"));
        addSerializableAttribute(
                new AttributeString(&destHostMOR, "destHostMOR"));
        addSerializableAttribute(new AttributeString(&vmMOR, "vmMOR"));
        addSerializableAttribute(
                new AttributeString(&srcConsoleIP, "srcConsoleIP"));
        addSerializableAttribute(
                new AttributeString(&destConsoleIP, "destConsole"));
       /* addSerializableAttribute(
                new AttributeStringVector(&vmipaddrList, "vmipaddrList"));*/
        addSerializableAttribute(
                new AttributeStringVectorVector(&vmmacaddrList, "vmmacaddrList",0,false));
        addSerializableAttribute(
                new AttributeStringVectorVector(&srckernelIPList, "srckernelIPList"));
        addSerializableAttribute(
                new AttributeStringVectorVector(&destKernelIPList,
                        "destKernelIPList"));


    }

    string MigrationEventsMessage::getCenterIp() const {
        return vCenterIP;
    }

    string MigrationEventsMessage::getDestConsoleIp() const {
        return destConsoleIP;
    }

    string MigrationEventsMessage::getDestHostMor() const {
        return destHostMOR;
    }

    vector<vector<string> > MigrationEventsMessage::getDestKernelIpList() const {
        return destKernelIPList;
    }

    UI32 MigrationEventsMessage::getEventChainId() const {
        return eventChainID;
    }

    UI32 MigrationEventsMessage::getEventId() const {
        return eventID;
    }

    UI32 MigrationEventsMessage::getEventType() const {
        return eventType;
    }

    string MigrationEventsMessage::getHostMor() const {
        return hostMOR;
    }

    string MigrationEventsMessage::getSrcConsoleIp() const {
        return srcConsoleIP;
    }

    vector<vector<string> > MigrationEventsMessage::getSrckernelIpList() const {
        return srckernelIPList;
    }

    string MigrationEventsMessage::getVmMor() const {
        return vmMOR;
    }



    vector<vector<string> > MigrationEventsMessage::getVmmacaddrList() const {
        return vmmacaddrList;
    }

    void MigrationEventsMessage::setCenterIp(string &centerIp) {
        vCenterIP = centerIp;
    }

    void MigrationEventsMessage::setDestConsoleIp(string &destConsoleIp) {
        destConsoleIP = destConsoleIp;
    }

    void MigrationEventsMessage::setDestHostMor(string &destHostMor) {
        destHostMOR = destHostMor;
    }

    void MigrationEventsMessage::setDestKernelIpList(
            vector<vector<string> > &destKernelIpList) {
        destKernelIPList = destKernelIpList;
    }

    void MigrationEventsMessage::setEventChainId(UI32 &eventChainId) {
        eventChainID = eventChainId;
    }

    void MigrationEventsMessage::setEventId(UI32 &eventId) {
        eventID = eventId;
    }

    void MigrationEventsMessage::setEventType(UI32 &eventtype) {
        eventType = eventtype;
    }

    void MigrationEventsMessage::setHostMor(string &hostMor) {
        hostMOR = hostMor;
    }

    void MigrationEventsMessage::setSrcConsoleIp(string &srcConsoleIp) {
        srcConsoleIP = srcConsoleIp;
    }

    void MigrationEventsMessage::setSrckernelIpList(
            vector<vector<string> > &srckernelIpList) {
        srckernelIPList = srckernelIpList;
    }

    void MigrationEventsMessage::setVmMor(string &vmMor) {
        vmMOR = vmMor;
    }



    void MigrationEventsMessage::setVmmacaddrList(
            vector<vector<string> >&m_vmmacaddrList) {
        vmmacaddrList = m_vmmacaddrList;

    }

} /* namespace DcmNs */
