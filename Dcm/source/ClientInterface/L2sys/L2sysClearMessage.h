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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Adarsh
 **************************************************************************/

#ifndef L2SYSCLEARMESSAGE_H
#define L2SYSCLEARMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysClearMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysClearMessage ();
            L2sysClearMessage (const string &ifName, const string &Mac, const UI32 &vlan_id,const UI32 &type, const UI32 &cmdCode, const UI32 &bridge, const UI32 &ifindex, const UI32 &interface_type, const UI32 &port_channel_id);
            virtual ~L2sysClearMessage ();
            void setIfName(const string &ifName);
            string getIfName() const;
            void setMac(const string &Mac);
            string getMac() const;
            void setVlanid(const UI32 &vlan_id);
            UI32 getVlanid() const;
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode() const;
            void setType(const UI32 &type);
            UI32 getType() const;
            void setBridge(const UI32 &bridgeId);
            UI32 getBridge() const;
            void setIfindex(const UI32 &ifindex);
            UI32 getIfindex() const;
            void setInterfaceType(const UI32 &interface_type);
            UI32 getInterfaceType() const;
            void setPortChannelId(const UI32 &port_channel_id);
            UI32 getPortChannelId() const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, 
												UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_ifName;
            string m_Mac;
            UI32 m_vlanid;
            UI32 m_type;
            UI32 m_cmdCode;
            UI32 m_bridge;
            UI32 m_ifindex;
            UI32 m_interface_type;
            UI32 m_port_channel_id;
    };
}
#endif  //L2sysClearMessage_H
