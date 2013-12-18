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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeIpVxAddress.h"

#include "SystemMonitor/Local/SystemMonitorLocalObjectManager.h"
#include "SystemMonitor/Local/MonitorLocalShowSystemMonitorMessage.h"
#include "SystemMonitor/Local/SystemMonitorLocalTypes.h"

namespace DcmNs
{

    MonitorLocalShowSystemMonitorMessage::MonitorLocalShowSystemMonitorMessage ()
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), MONITORLOCALSHOWSYSTEMMONITOR)
    {
    }

    MonitorLocalShowSystemMonitorMessage::MonitorLocalShowSystemMonitorMessage (const string &switchName,
        const IpVxAddress &switchIpAddr, const UI32  &switchStatus,
        const string  &switchStatusReason, const DateTime &datetime,
        const vector<string> &vComponent, const vector<UI32> &vCompStatus,
        const vector<UI32> &vNumBad, const vector<UI32> &vNumAbsent,
        const vector<UI32> &vPortArea, const vector<string> &vPortName,
        const vector<UI32> &vPortState)
        : PrismMessage (SystemMonitorLocalObjectManager::getPrismServiceId (), MONITORLOCALSHOWSYSTEMMONITOR),
        m_switchName (switchName),
        m_switchIpAddr (switchIpAddr),
        m_switchStatus (switchStatus),
        m_switchStatusReason (switchStatusReason),
        m_date (datetime),
        m_vComponent (vComponent),
        m_vCompStatus (vCompStatus),
        m_vNumBad (vNumBad),
        m_vNumAbsent (vNumAbsent),
        m_vPortArea (vPortArea),
        m_vPortName (vPortName),
        m_vPortState (vPortState) 
    {
    }

    MonitorLocalShowSystemMonitorMessage::~MonitorLocalShowSystemMonitorMessage ()
    {
    }

    void  MonitorLocalShowSystemMonitorMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_switchName,"switchName"));
        addSerializableAttribute (new AttributeIpVxAddress(&m_switchIpAddr,"switchIpAddr"));
        addSerializableAttribute (new AttributeUI32(&m_switchStatus,"switchStatus"));
        addSerializableAttribute (new AttributeString(&m_switchStatusReason,"switchStatusReason"));
        addSerializableAttribute (new AttributeDateTime(&m_date,"date"));
        addSerializableAttribute (new AttributeStringVector(&m_vComponent,"vComponent"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vCompStatus,"vCompStatus"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumBad,"vNumBad"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumAbsent,"vNumAbsent"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vPortArea,"vPortArea"));
        addSerializableAttribute (new AttributeStringVector(&m_vPortName,"vPortName"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vPortState,"vPortState"));
    }

    void  MonitorLocalShowSystemMonitorMessage::setSwitchName(const string &switchName)
    {
        m_switchName  =  switchName;
    }

    string  MonitorLocalShowSystemMonitorMessage::getSwitchName() const
    {
        return (m_switchName);
    }

    void  MonitorLocalShowSystemMonitorMessage::setSwitchIpAddr(const IpVxAddress &switchIpAddr)
    {
        m_switchIpAddr  =  switchIpAddr;
    }

    IpVxAddress  MonitorLocalShowSystemMonitorMessage::getSwitchIpAddr() const
    {
        return (m_switchIpAddr);
    }

    void  MonitorLocalShowSystemMonitorMessage::setSwitchStatus(const UI32 &switchStatus)
    {
        m_switchStatus  =  switchStatus;
    }

    UI32  MonitorLocalShowSystemMonitorMessage::getSwitchStatus() const
    {
        return (m_switchStatus);
    }

    void  MonitorLocalShowSystemMonitorMessage::setSwitchStatusReason(const string &switchStatusReason)
    {
        m_switchStatusReason  =  switchStatusReason;
    }

    string  MonitorLocalShowSystemMonitorMessage::getSwitchStatusReason() const
    {
        return (m_switchStatusReason);
    }

    void  MonitorLocalShowSystemMonitorMessage::setDate(const DateTime &date)
    {
        m_date  =  date;
    }

    DateTime  MonitorLocalShowSystemMonitorMessage::getDate() const
    {
        return (m_date);
    }

    void  MonitorLocalShowSystemMonitorMessage::setComponent(const string &component, const UI32 &compStatus, const UI32 &numBad, const UI32 &numAbsent)
    {
        m_vComponent.push_back(component);
        m_vCompStatus.push_back(compStatus);
        m_vNumBad.push_back(numBad);
        m_vNumAbsent.push_back(numAbsent);
    }

    UI32 MonitorLocalShowSystemMonitorMessage::getNumComponent() const
    {
        return (m_vComponent.size());
    }

    bool  MonitorLocalShowSystemMonitorMessage::getComponent(const UI32
          &compIndex, string &component, UI32 &compStatus,
          UI32 &numBad, UI32 &numAbsent) const
    {
        if (compIndex >= m_vComponent.size())
        {
            return false;
        }

        component = m_vComponent[compIndex];
        compStatus = m_vCompStatus[compIndex];
        numBad = m_vNumBad[compIndex];
        numAbsent = m_vNumAbsent[compIndex];

        return true;
    }

    void MonitorLocalShowSystemMonitorMessage::clearComponents()
    {
        m_vComponent.clear();
        m_vCompStatus.clear();
        m_vNumBad.clear();
        m_vNumAbsent.clear();
    }

    void  MonitorLocalShowSystemMonitorMessage::setPort(const UI32 &portArea,
          const string &portName, const UI32 &portState)
    {
        m_vPortArea.push_back(portArea);
        m_vPortName.push_back(portName);
        m_vPortState.push_back(portState);
    }

    UI32  MonitorLocalShowSystemMonitorMessage::getNumPorts() const
    {
        return (m_vPortName.size());
    }

    bool  MonitorLocalShowSystemMonitorMessage::getPort(const UI32 &portIndex,
          UI32 &portArea, string &portName, UI32 &portState) const
    {
        if (portIndex >= m_vPortName.size())
        {
            return false;
        }

        portArea = m_vPortArea[portIndex];
        portName = m_vPortName[portIndex];
        portState = m_vPortState[portIndex];

        return true;
    }

    void MonitorLocalShowSystemMonitorMessage::clearPorts()
    {
        m_vPortArea.clear();
        m_vPortName.clear();
        m_vPortState.clear();
    }
}
