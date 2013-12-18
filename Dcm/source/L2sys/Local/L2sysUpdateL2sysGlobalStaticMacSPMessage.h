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
 *   Author : aantony                                                     *
 **************************************************************************/

#ifndef LSYSUPDATELSYSGLOBALSTATICMACSPMESSAGE_H
#define LSYSUPDATELSYSGLOBALSTATICMACSPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysUpdateL2sysGlobalStaticMacSPMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysUpdateL2sysGlobalStaticMacSPMessage ();
            L2sysUpdateL2sysGlobalStaticMacSPMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &forward,const UI32 &interface_type,const string &ifname,const UI32 &ifindex,const UI32 &vlan,const UI32 &vlanid,const UI32 &opcode, const UI32 &port_channel_id);
            virtual    ~L2sysUpdateL2sysGlobalStaticMacSPMessage ();
            void setRbridgeid(const UI32 &rbridgeid);
            UI32 getRbridgeid()  const;
            void setMac(const string &Mac);
            string getMac()  const;
            void setForward(const UI32 &forward);
            UI32 getForward()  const;
            void setInterface_type(const UI32 &interface_type);
            UI32 getInterface_type()  const;
            void setIfname(const string &ifname);
            string getIfname()  const;
            void setIfindex(const UI32 &ifindex);
            UI32 getIfindex()  const;
            void setVlan(const UI32 &vlan);
            UI32 getVlan()  const;
            void setVlanid(const UI32 &vlanid);
            UI32 getVlanid()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;
            void setPortChannelId(const UI32 &port_channel_id);
            UI32 getPortChannelId()  const;
// Now the data members

        private:
        protected:
        public:
            UI32  m_rbridgeid;
            string  m_Mac;
            UI32  m_forward;
            UI32  m_interface_type;
            string  m_ifname;
            UI32  m_ifindex;
            UI32  m_vlan;
            UI32  m_vlanid;
            UI32  m_opcode;
            UI32  m_port_channel_id;
    };
}
#endif                                            //LSYSUPDATELSYSGLOBALSTATICMACSPMESSAGE_H
