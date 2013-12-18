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
 *   Author : vbagavat                                                     *
 **************************************************************************/

#ifndef PORTCFGFCPORTINTERFACEINFOMESSAGE_H
#define PORTCFGFCPORTINTERFACEINFOMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFCPortInterfaceInfoMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFCPortInterfaceInfoMessage ();
            PortCfgFCPortInterfaceInfoMessage (const string &portId,const string &portType,const string &portWWN,const string &remotePortWWN,const string &remoteNodeWWN,const string &state,const string &status,const string &statusMessage,const string &health,const bool &trunked,const UI32 &trunkMaster,const UI32 &actualDistance,const UI32 &desiredCredit,const UI32 &bufferAllocated,const UI32 &switchPort,const bool &licensed,const string &portaddr);
            virtual    ~PortCfgFCPortInterfaceInfoMessage ();
            void setPortId(const string &portId);
            string getPortId()  const;
            void setPortType(const string &portType);
            string getPortType()  const;
            void setPortWWN(const string &portWWN);
            string getPortWWN()  const;
            void setRemotePortWWN(const string &remotePortWWN);
            string getRemotePortWWN()  const;
            void setRemoteNodeWWN(const string &remoteNodeWWN);
            string getRemoteNodeWWN()  const;
            void setState(const string &state);
            string getState()  const;
            void setStatus(const string &status);
            string getStatus()  const;
            void setStatusMessage(const string &statusMessage);
            string getStatusMessage()  const;
            void setHealth(const string &health);
            string getHealth()  const;
            void setTrunked(const bool &trunked);
            bool getTrunked()  const;
            void setTrunkMaster(const UI32 &trunkMaster);
            UI32 getTrunkMaster()  const;
            void setActualDistance(const UI32 &actualDistance);
            UI32 getActualDistance()  const;
            void setDesiredCredit(const UI32 &desiredCredit);
            UI32 getDesiredCredit()  const;
            void setBufferAllocated(const UI32 &bufferAllocated);
            UI32 getBufferAllocated()  const;
            void setSwitchPort(const UI32 &switchPort);
            UI32 getSwitchPort()  const;
            void setLicensed(const bool &licensed);
            bool getLicensed()  const;
            void setPortaddr(const string &portaddr);
            string getPortaddr()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_portId;
            string  m_portType;
            string  m_portWWN;
            string  m_remotePortWWN;
            string  m_remoteNodeWWN;
            string  m_state;
            string  m_status;
            string  m_statusMessage;
            string  m_health;
            bool  m_trunked;
            UI32  m_trunkMaster;
            UI32  m_actualDistance;
            UI32  m_desiredCredit;
            UI32  m_bufferAllocated;
            UI32  m_switchPort;
            bool  m_licensed;
            string  m_portaddr;
    };
}
#endif                                            //PORTCFGFCPORTINTERFACEINFOMESSAGE_H
