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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Amitayu
 **************************************************************************/

#ifndef ELDSHOWMESSSAGE_H
#define ELDSHOWMESSSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Nsm/Local/Eld/EldLocalShowMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class EldShowMessage: public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			EldShowMessage ();
			EldShowMessage (const string &interface, const string &ifName,
				const string &Mac, const UI32 &vlan_id, const UI32 &type,
				const UI32 &cmdcode, const UI32 &bridge, const UI32 &ifindex,
				const UI32 &interfaceType, const UI32 &portChannelId,const UI32 &mappedid);
			EldShowMessage(EldLocalShowMessage *pMsg);
			virtual ~EldShowMessage ();
			void setIfName(const string &ifName);
			string getIfName() const;
			void setInterface(const string &interface);
			string getInterface() const;
			void setMac(const string &Mac);
			string getMac() const;
			void setVlanid(const UI32 &vlan_id);
			UI32 getVlanid() const;
			void setCmdCode(const UI32 &cmdcode);
			UI32 getCmdCode() const;
			void setRbridge(const UI32 &rbridge_id);
			UI32 getRbridge() const;
			void setType(const UI32 &type);
			UI32 getType() const;
			void setBridge(const UI32 &bridgeId);
			UI32 getBridge() const;
			void setMappedId(const UI32 &mappedid);
			UI32 getMappedId() const;
			void setIfindex(const UI32 &ifindex);
			UI32 getIfindex() const;
			void setInterfaceType(const UI32 &ifType);
			UI32 getInterfaceType() const;
			void setPortChannelId(const UI32 &poId);
			UI32 getPortChannelId() const;

			virtual const void *getCStructureForInputs ();
			virtual void loadOutputsFromCStructure (const void 	*pOutputCStructure);

            void getDuplicateInterfaceOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            string m_ifName;
            string m_interface;
            string m_Mac;
            UI32 m_vlanid;
            UI32 m_type;
            UI32 m_cmdcode;
            UI32 m_bridge;
			UI32 m_ifindex;
			UI32 m_interface_type;
			UI32 m_port_channel_id;
			UI32 m_mappedid;
			UI32 m_rbridge;
    };
}
#endif  //ELDSHOWMESSSAGE_H
