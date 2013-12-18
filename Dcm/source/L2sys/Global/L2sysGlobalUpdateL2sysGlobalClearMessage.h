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
 *   Author :                                                      *
 **************************************************************************/

#ifndef LSYSGLOBALUPDATELSYSGLOBALCLEARMESSAGE_H
#define LSYSGLOBALUPDATELSYSGLOBALCLEARMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysGlobalUpdateL2sysGlobalClearMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysGlobalUpdateL2sysGlobalClearMessage ();
            L2sysGlobalUpdateL2sysGlobalClearMessage (const UI32 &bridgeid, const string &ifname,const string &Mac,const UI32 &vlanid,const UI32 &type,const UI32 &opcode,const UI32 &interface_type,const UI32 &port_channel_id);
            virtual    ~L2sysGlobalUpdateL2sysGlobalClearMessage ();
            void setRbridgeid(const UI32 &bridgeid);
            UI32 getRbridgeid()  const;
            void setIfname(const string &ifname);
            string getIfname()  const;
            void setMac(const string &Mac);
            string getMac()  const;
            void setVlanid(const UI32 &vlanid);
            UI32 getVlanid()  const;
            void setType(const UI32 &type);
            UI32 getType()  const;
            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;
            void setInterfaceType(const UI32 &interface_type);
            UI32 getInterfaceType()  const;
            void setPortChannelId(const UI32 &port_channel_id);
            UI32 getPortChannelId()  const;

// Now the data members

        private:
        protected:
        public:
            UI32 m_rbridgeid;
            string m_ifname;
            string m_Mac;
            UI32 m_vlanid;
            UI32 m_type;
            UI32 m_opcode;
            UI32 m_interface_type;
            UI32 m_port_channel_id;
    };
}
#endif                                            //LSYSGLOBALUPDATELSYSGLOBALCLEARMESSAGE_H
