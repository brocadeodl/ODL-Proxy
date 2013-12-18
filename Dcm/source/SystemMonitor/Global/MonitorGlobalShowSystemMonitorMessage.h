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

#ifndef MONITORGLOBALSHOWSYSTEMMONITORMESSAGE_H
#define MONITORGLOBALSHOWSYSTEMMONITORMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class MonitorGlobalShowSystemMonitorMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MonitorGlobalShowSystemMonitorMessage ();
            MonitorGlobalShowSystemMonitorMessage (const bool &destAllFlag,
                const UI32 &mapId, const vector<UI32> &vMapId,
                const vector<string> &vSwitchName,
                const vector<IpVxAddress> &vIp,
                const vector<UI32> &vSwitchStatus,
                const vector<string>  &vSwitchStatusReason,
                const vector<DateTime>  &vDate,
                const vector<UI32>  &vNumComponent,
                const vector<UI32>  &vNumPort,
                const vector<UI32>  &vComponentMapId,
                const vector<string>  &vComponent,
                const vector<UI32>  &vCompStatus,
                const vector<UI32>  &vNumBad,
                const vector<UI32>  &vNumAbsent,
                const vector<UI32>  &vPortMapId,
                const vector<UI32>  &vPortArea,
                const vector<string>  &vPortName,
                const vector<UI32>  &vPortStatus);

            virtual    ~MonitorGlobalShowSystemMonitorMessage ();

            void setDestinationAllFlag(const bool &destAll);
            bool getDestinationAllFlag() const;
            void setDestinationMapId(const UI32 &mapId);
            UI32 getDestinationMapId() const;

            void setSwitchStatus(const UI32 &mapId, const string &switchName,
                 const IpVxAddress &switchIpAddr, const UI32 &switchStatus,
                 const string &switchStatusReason, const DateTime &date,
                 const UI32 &numComponent, const UI32 &numPort);
            UI32 getNumSwitch() const;
            bool getSwitchStatus(const UI32 &switchIndex, UI32 &mapId,
                 string &switchName, IpVxAddress &switchIpAddr,
                 UI32 &switchStatus, string &switchStatusReason,
                 DateTime &date, UI32 &numComponent, UI32 &numPort)  const;
            void clearSwitchStatus();

            void setComponent(const UI32 &mapId, const string &component,
                 const UI32 &compStatus, const UI32 &numBad,
                 const UI32 &numAbsent);
            UI32 getNumComponent(const UI32 &mapId) const;
            bool getComponent(const UI32 &mapId, const UI32 &compIndex,
                 string &component, UI32 &compStatus,
                 UI32 &numBad, UI32 &numAbsent)  const;
            void clearComponents();

            void setPort(const UI32 &mapId, const UI32 &portArea,
                 const string &portName, const UI32 &portStatus);
            UI32 getNumPorts(const UI32 &mapId)  const;
            bool getPort(const UI32 &mapId, const UI32 &portIndex,
                 UI32 &portArea, string &portName, UI32 &portStatus)  const;
            void clearPorts();

           // Now the data members

            private:
            protected:
            public:
                bool m_destinationAllFlag;
                UI32 m_destinationMapId;
                vector<UI32> m_vMapId;
                vector<string>  m_vSwitchName;
                vector<IpVxAddress>  m_vSwitchIpAddr;
                vector<UI32>  m_vSwitchStatus;
                vector<string>  m_vSwitchStatusReason;
                vector<DateTime>  m_vDate;
                vector<UI32>  m_vNumComponent;
                vector<UI32>  m_vNumPort;

                vector<UI32>  m_vComponentMapId;
                vector<string>  m_vComponent;
                vector<UI32>  m_vCompStatus;
                vector<UI32>  m_vNumBad;
                vector<UI32>  m_vNumAbsent;

                vector<UI32>  m_vPortMapId;
                vector<UI32>  m_vPortArea;
                vector<string>  m_vPortName;
                vector<UI32>  m_vPortStatus;
        };
}
#endif                                            //MONITORGLOBALSHOWSYSTEMMONITORMESSAGE_H
