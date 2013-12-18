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
#include "SystemMonitor/Global/MonitorGlobalShowSystemMonitorMessage.h"
#include "SystemMonitor/Global/SystemMonitorGlobalObjectManager.h"
#include "SystemMonitor/Global/SystemMonitorGlobalTypes.h"

namespace DcmNs
{

    MonitorGlobalShowSystemMonitorMessage::MonitorGlobalShowSystemMonitorMessage ()
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), MONITORGLOBALSHOWSYSTEMMONITOR)
    {
    }

    MonitorGlobalShowSystemMonitorMessage::MonitorGlobalShowSystemMonitorMessage (const bool &destAllFlag, const UI32 &mapId, const vector<UI32> &vMapId,
        const vector<string> &vSwitchName, const vector<IpVxAddress> &vIp,
        const vector<UI32> &vSwitchStatus,
        const vector<string>  &vSwitchStatusReason,
        const vector<DateTime>  &vDate, const vector<UI32>  &vNumComponent,
        const vector<UI32>  &vNumPort, const vector<UI32>  &vComponentMapId,
        const vector<string>  &vComponent, const vector<UI32>  &vCompStatus,
        const vector<UI32>  &vNumBad, const vector<UI32>  &vNumAbsent,
        const vector<UI32>  &vPortMapId, const vector<UI32>  &vPortArea,
        const vector<string>  &vPortName, const vector<UI32>  &vPortStatus)
        : PrismMessage (SystemMonitorGlobalObjectManager::getPrismServiceId (), MONITORGLOBALSHOWSYSTEMMONITOR),
        m_destinationAllFlag (destAllFlag),
        m_destinationMapId (mapId),
        m_vMapId (vMapId),
        m_vSwitchName (vSwitchName),
        m_vSwitchIpAddr (vIp),
        m_vSwitchStatus (vSwitchStatus),
        m_vSwitchStatusReason (vSwitchStatusReason),
        m_vDate (vDate),
        m_vNumComponent (vNumComponent),
        m_vNumPort (vNumPort),
        m_vComponentMapId (vComponentMapId),
        m_vComponent (vComponent),
        m_vCompStatus (vCompStatus),
        m_vNumBad (vNumBad),
        m_vNumAbsent (vNumAbsent),
        m_vPortMapId (vPortMapId),
        m_vPortArea (vPortArea),
        m_vPortName (vPortName),
        m_vPortStatus (vPortStatus)
    {
    }

    MonitorGlobalShowSystemMonitorMessage::~MonitorGlobalShowSystemMonitorMessage ()
    {
    }

    void  MonitorGlobalShowSystemMonitorMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_destinationAllFlag,"vDestinationAllFlag"));
        addSerializableAttribute (new AttributeUI32(&m_destinationMapId,"vDestinationMapId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vMapId,"vMapId"));
        addSerializableAttribute (new AttributeStringVector(&m_vSwitchName,"vSwitchName"));
        addSerializableAttribute (new AttributeIpVxAddressVector(&m_vSwitchIpAddr,"vSwitchIpAddr"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vSwitchStatus,"vSwitchStatus"));
        addSerializableAttribute (new AttributeStringVector(&m_vSwitchStatusReason,"vSwitchStatusReason"));
        addSerializableAttribute (new AttributeDateTimeVector(&m_vDate,"vDate"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumComponent,"vNumComponent"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumPort,"vNumPort"));

        addSerializableAttribute (new AttributeUI32Vector(&m_vComponentMapId,"vComponentMapId"));
        addSerializableAttribute (new AttributeStringVector(&m_vComponent,"vComponent"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vCompStatus,"vCompStatus"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumBad,"vNumBad"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vNumAbsent,"vNumAbsent"));

        addSerializableAttribute (new AttributeUI32Vector(&m_vPortMapId,"vPortMapId"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vPortArea,"vPortArea"));
        addSerializableAttribute (new AttributeStringVector(&m_vPortName,"vPortName"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vPortStatus,"vPortStatus"));
    }

    void MonitorGlobalShowSystemMonitorMessage::setDestinationAllFlag(const bool &destAll)
    {
        m_destinationAllFlag = destAll;
    }

    bool MonitorGlobalShowSystemMonitorMessage::getDestinationAllFlag() const
    {
        return m_destinationAllFlag;
    }

    void MonitorGlobalShowSystemMonitorMessage::setDestinationMapId(const UI32 &mapId)
    {
        m_destinationMapId = mapId;
    }

    UI32 MonitorGlobalShowSystemMonitorMessage::getDestinationMapId() const
    {
        return m_destinationMapId;
    }

    void  MonitorGlobalShowSystemMonitorMessage::setSwitchStatus(
          const UI32 &mapId, const string &switchName,
          const IpVxAddress &switchIpAddr, const UI32 &switchStatus,
          const string &switchStatusReason, const DateTime &date,
          const UI32 &numComponent, const UI32 &numPort)
    {
        m_vMapId.push_back(mapId);
        m_vSwitchName.push_back(switchName);
        m_vSwitchIpAddr.push_back(switchIpAddr);
        m_vSwitchStatus.push_back(switchStatus);
        m_vSwitchStatusReason.push_back(switchStatusReason);
        m_vDate.push_back(date);
        m_vNumComponent.push_back(numComponent);
        m_vNumPort.push_back(numPort);
    }

    UI32  MonitorGlobalShowSystemMonitorMessage::getNumSwitch() const
    {
        return (m_vMapId.size());
    }

    bool  MonitorGlobalShowSystemMonitorMessage::getSwitchStatus(
        const UI32 &switchIndex, UI32 &mapId, string &switchName,
        IpVxAddress &switchIpAddr, UI32 &switchStatus,
        string &switchStatusReason, DateTime &date,
        UI32 &numComponent, UI32 &numPort)
        const
    {
        if (switchIndex >= m_vMapId.size())
        {
            return false;
        }

        mapId = m_vMapId[switchIndex];
        switchName = m_vSwitchName[switchIndex];
        switchIpAddr = m_vSwitchIpAddr[switchIndex];
        switchStatus = m_vSwitchStatus[switchIndex];
        switchStatusReason = m_vSwitchStatusReason[switchIndex];
        date = m_vDate[switchIndex];
        numComponent = m_vNumComponent[switchIndex];
        numPort = m_vNumPort[switchIndex];

        return true;
    }

    void MonitorGlobalShowSystemMonitorMessage::clearSwitchStatus()
    {
        m_vMapId.clear();
        m_vSwitchName.clear();
        m_vSwitchIpAddr.clear();
        m_vSwitchStatus.clear();
        m_vSwitchStatusReason.clear();
        m_vDate.clear();
        m_vNumComponent.clear();
        m_vNumPort.clear();
    }

    void  MonitorGlobalShowSystemMonitorMessage::setComponent(const UI32 &mapId,
        const string &component, const UI32 &compStatus, const UI32 &numBad,
        const UI32 &numAbsent)
    {
        m_vComponentMapId.push_back(mapId);
        m_vComponent.push_back(component);
        m_vCompStatus.push_back(compStatus);
        m_vNumBad.push_back(numBad);
        m_vNumAbsent.push_back(numAbsent);
    }

    UI32  MonitorGlobalShowSystemMonitorMessage::getNumComponent(const UI32 &mapId) const
    {
        for (UI32 i = 0; i < m_vMapId.size(); i++)
        {
            if (m_vMapId[i] == mapId)
            {
                return (m_vNumComponent[i]);
            }
        }

        return (0);
    }

    bool  MonitorGlobalShowSystemMonitorMessage::getComponent(
        const UI32 &mapId, const UI32 &compIndex, string &component,
        UI32 &compStatus, UI32 &numBad, UI32 &numAbsent) const
    {
        UI32 i, maxIndex = m_vComponentMapId.size();
        for (i = 0; i < maxIndex; i++)
            if (mapId == m_vComponentMapId[i])
                break;

        if ((i + compIndex) >= maxIndex)
            return false;

        if (m_vComponentMapId[i + compIndex] != mapId)
            return false;

        component = m_vComponent[i + compIndex];
        compStatus = m_vCompStatus[i + compIndex];
        numBad = m_vNumBad[i + compIndex];
        numAbsent = m_vNumAbsent[i + compIndex];

        return true;
    }

    void  MonitorGlobalShowSystemMonitorMessage::clearComponents()
    {
        m_vComponentMapId.clear();
        m_vComponent.clear();
        m_vCompStatus.clear();
        m_vNumBad.clear();
        m_vNumAbsent.clear();
    }


    void  MonitorGlobalShowSystemMonitorMessage::setPort(const UI32 &mapId,
        const UI32 &portArea, const string &portName, const UI32 &portStatus)
    {
        m_vPortMapId.push_back(mapId);
        m_vPortArea.push_back(portArea);
        m_vPortName.push_back(portName);
        m_vPortStatus.push_back(portStatus);
    }

    UI32  MonitorGlobalShowSystemMonitorMessage::getNumPorts(const UI32 &mapId) const
    {
        for (UI32 i = 0; i < m_vMapId.size(); i++)
        {
            if (m_vMapId[i] == mapId)
            {
                return (m_vNumPort[i]);
            }
        }

        return (0);
    }

    bool  MonitorGlobalShowSystemMonitorMessage::getPort(
        const UI32 &mapId, const UI32 &portIndex, UI32 &portArea,
        string &portName, UI32 &portStatus) const
    {
        UI32 i, maxIndex = m_vPortMapId.size();
        for (i = 0; i < maxIndex; i++)
            if (mapId == m_vPortMapId[i])
                break;

        if ((i + portIndex) >= maxIndex)
            return false;

        if (m_vPortMapId[i + portIndex] != mapId)
            return false;

        portArea = m_vPortArea[i + portIndex];
        portName = m_vPortName[i + portIndex];
        portStatus = m_vPortStatus[i + portIndex];

        return true;
    }

    void  MonitorGlobalShowSystemMonitorMessage::clearPorts()
    {
        m_vPortMapId.clear();
        m_vPortArea.clear();
        m_vPortName.clear();
        m_vPortStatus.clear();
    }

}
