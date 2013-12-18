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

#ifndef L2SYSSHOWMESSAGE_H
#define L2SYSSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/L2sys/L2sysMessageDef.h"

using namespace WaveNs;

namespace DcmNs
{

    class L2sysShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
			L2sysShowMessage ();
			L2sysShowMessage (const string &interface, const string &ifName,
				const string &Mac, const UI32 &vlan_id, const UI32 &type,
				const UI32 &cmdCode, const UI32 &bridge, const UI32 &ifindex,
				const UI32 &interfaceType, const UI32 &portChannelId,const UI32 &mappedid );
			virtual ~L2sysShowMessage ();
			void setIfName(const string &ifName);
			string getIfName() const;
			void setInterface(const string &interface);
			string getInterface() const;
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
			void setMappedId(const UI32 &mappedid);
			UI32 getMappedId() const;
			void setIfindex(const UI32 &ifindex);
			UI32 getIfindex() const;
			void setInterfaceType(const UI32 &ifType);
			UI32 getInterfaceType() const;
			void setPortChannelId(const UI32 &poId);
			UI32 getPortChannelId() const;
			void setPortChannelType(const UI32 &poType);
			UI32 getPortChannelType() const;
			void setPageIdx(const SI32 &pageIdx);
			SI32  getPageIdx() const;
			void setLastMacRecord(const l2sys_show_mac_output_msg_t *lastMacRecord);
			void setIsFirstRequest(const UI32 &isFirstRequest);
			UI32 getIsFirstRequest() const;

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
            UI32 m_cmdCode;
            UI32 m_bridge;
			UI32 m_ifindex;
			UI32 m_interface_type;
			UI32 m_port_channel_id;
			UI32 m_poType;
			UI32 m_mappedid;
			SI32 m_pageIdx;
			//These members will be used to convey the information of lastRecord to the backend
			UI32 m_lastRbridge_id;
			string m_lastIf_name;
			string m_lastMac;
			string m_lastType;
			string m_lastState;
			UI32 m_lastVlan_id;
			UI32 m_lastAppmFlag;
			UI32 m_lastIs_last;
			string m_lastIsprofiled;
			UI32 m_isFirstRequest;
    };
}
#endif  //L2sysShowMessage_H
