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

#ifndef PORTCFGFIBRECHANNELCLEARMESSAGE_H
#define PORTCFGFIBRECHANNELCLEARMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class PortCfgFibreChannelClearMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            PortCfgFibreChannelClearMessage ();
            PortCfgFibreChannelClearMessage (const UI32 &cmdCode,const SI32 &switchPort,const UI32 &slotId,const string &portId,const UI32 &rbridgeId);
            virtual    ~PortCfgFibreChannelClearMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;
            void setSwitchPort(const SI32 &switchPort);
            SI32 getSwitchPort()  const;
            void setSlotId(const UI32 &slotId);
            UI32 getSlotId()  const;
            void setPortId(const string &portId);
            string getPortId()  const;
            void setRbridgeId(const UI32 &rbridgeId);
            UI32 getRbridgeId()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            SI32  m_switchPort;
            UI32  m_slotId;
            string  m_portId;
            UI32  m_rbridgeId;
    };
}
#endif                                            //PORTCFGFIBRECHANNELCLEARMESSAGE_H
