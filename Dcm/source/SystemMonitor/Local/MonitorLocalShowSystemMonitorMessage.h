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

#ifndef MONITORLOCALSHOWSYSTEMMONITORMESSAGE_H
#define MONITORLOCALSHOWSYSTEMMONITORMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Attributes/AttributeDateTime.h"
#include "Framework/Types/IpVxAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class MonitorLocalShowSystemMonitorMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MonitorLocalShowSystemMonitorMessage ();
            MonitorLocalShowSystemMonitorMessage (const string &switchName,
                const IpVxAddress &switchIpAddr, const UI32  &switchStatus,
                const string  &switchStatusReason, const DateTime &datetime,
                const vector<string> &vComponent,
                const vector<UI32> &vCompStatus, const vector<UI32> &vNumBad,
                const vector<UI32> &vNumAbsent, const vector<UI32> &vPortArea,
                const vector<string> &vPortName,
                const vector<UI32> &vPortState);
            virtual    ~MonitorLocalShowSystemMonitorMessage ();

            void setSwitchName(const string &switchName);
            string getSwitchName()  const;
            void setSwitchIpAddr(const IpVxAddress &switchIpAddr);
            IpVxAddress getSwitchIpAddr()  const;
            void setSwitchStatus(const UI32 &switchStatus);
            UI32 getSwitchStatus()  const;
            void setSwitchStatusReason(const string &switchStatusReason);
            string getSwitchStatusReason()  const;
            void setDate(const DateTime &date);
            DateTime getDate()  const;

            void setComponent(const string &component,
                 const UI32 &compStatus, const UI32 &numBad,
                 const UI32 &numAbsent);
            UI32 getNumComponent()  const;
            bool getComponent(const UI32 &compIndex, string &component,
                 UI32 &compStatus, UI32 &numBad, UI32 &numAbsent)  const;
            void clearComponents();

            void setPort(const UI32 &portArea, const string &portName,
                 const UI32 &portState);
            UI32 getNumPorts()  const;
            bool getPort(const UI32 &portIndex, UI32 &portArea,
                 string &portName, UI32 &portState)  const;
            void clearPorts();

// Now the data members

        private:
        protected:
        public:
            string  m_switchName;
            IpVxAddress  m_switchIpAddr;
            UI32  m_switchStatus;
            string  m_switchStatusReason;
            DateTime  m_date;
            vector<string>  m_vComponent;
            vector<UI32>  m_vCompStatus;
            vector<UI32>  m_vNumBad;
            vector<UI32>  m_vNumAbsent;
            vector<UI32>  m_vPortArea;
            vector<string>  m_vPortName;
            vector<UI32>  m_vPortState;
    };
}
#endif                                            //MONITORLOCALSHOWSYSTEMMONITORMESSAGE_H
