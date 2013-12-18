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

#ifndef MIGRATIONEVENTSMESSGE_H_
#define MIGRATIONEVENTSMESSGE_H_

using namespace std;
#include <string.h>
#include "Framework/Types/Types.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include <string>
#include <vector>
using namespace WaveNs;
namespace DcmNs {

    class MigrationEventsMessage: public ManagementInterfaceMessage {
        protected:
            virtual void setupAttributesForSerialization();
        public:
            MigrationEventsMessage();
            virtual ~MigrationEventsMessage();
            MigrationEventsMessage(UI32 &m_eventID, UI32 &m_eventChainID,
                    UI32 &m_eventType, string &m_vCenterIP, string &m_hostMOR,
                    string &m_destHostMOR, string &m_vmMOR,

                    vector<vector<string> > &m_vmmacaddrList,

                    string &m_srcConsoleIP,
                    vector<vector<string> > &m_srckernelIPList,

                    string &m_destConsoleIP,
                    vector<vector<string> >&m_destKernelIPList
                    );

            string getCenterIp() const;
            string getDestConsoleIp() const;
            string getDestHostMor() const;
            vector<vector<string> >getDestKernelIpList() const;
            UI32 getEventChainId() const;
            UI32 getEventId() const;
            UI32 getEventType() const;
            string getHostMor() const;
            string getSrcConsoleIp() const;
            vector<vector<string> >getSrckernelIpList() const;
            string getVmMor() const;

            vector<vector<string> > getVmmacaddrList() const;
            void setCenterIp(string &centerIp);
            void setDestConsoleIp(string &destConsoleIp);
            void setDestHostMor(string &destHostMor);
            void setDestKernelIpList(vector<vector<string> > &destKernelIpList);
            void setEventChainId(UI32 &eventChainId);
            void setEventId(UI32 &eventId);
            void setEventType(UI32 &eventType);
            void setHostMor(string &hostMor);
            void setSrcConsoleIp(string &srcConsoleIp);
            void setSrckernelIpList(vector<vector<string> > &srckernelIpList);
            void setVmMor(string &vmMor);

            void setVmmacaddrList(vector<vector<string> > &vmmacaddrList);
            UI32 eventID;
            UI32 eventChainID;
            UI32 eventType;
            string vCenterIP;
            string hostMOR;
            string destHostMOR;
            string vmMOR;
            vector<vector<string> > vmmacaddrList; // {{M1,IP1,IP2),{M2,IP3,IP4}}  mac and IP list
            string srcConsoleIP;
            vector<vector<string> > srckernelIPList; //{IP1,N1},{IP2,N2}} IP and Nictype
            string destConsoleIP;
            vector<vector<string> > destKernelIPList; //{IP1,N1},{IP2,N2}} IP and Nictype

    };

} /* namespace DcmNs */
#endif /* MIGRATIONEVENTSMESSGE_H_ */
