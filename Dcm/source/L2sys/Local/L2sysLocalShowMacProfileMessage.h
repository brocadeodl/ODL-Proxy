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
 *   Author : sriramr                                                     *
 **************************************************************************/

#ifndef L2sysLocalShowMacProfileMessage_H
#define L2sysLocalShowMacProfileMessage_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/L2sys/L2sysShowMessage.h"
using namespace WaveNs;

namespace DcmNs
{

    class L2sysLocalShowMacProfileMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            L2sysLocalShowMacProfileMessage ();
             L2sysLocalShowMacProfileMessage (const UI32 &rbridgeid,const string &Mac,const UI32 &interface_type,const string &ifname,const UI32 &ifindex,const UI32 &vlan,const UI32 &vlanid,const UI32 &cmdcode,const string &Profiled,const UI32 &type,bool m_isGwRequest);
		virtual    ~L2sysLocalShowMacProfileMessage ();
            void setRbridgeid(const UI32 &rbridgeid);
            UI32 getRbridgeid()  const;
            void setMac(const string &Mac);
            string getMac()  const;
            void setInterface_type(const UI32 &interface_type);
            UI32 getInterface_type()  const;
            void setIfname(const string &ifname);
            string getIfname()  const;
            void setIfindex(const UI32 &ifindex);
            UI32 getIfindex()  const;
            void setVlan(const UI32 &vlan);
            UI32 getVlan()  const;
            void setVlanId(const UI32 &vlanid);
            UI32 getVlanId()  const;
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
	    void setProfiled(const string &m_Profiled);
	    string getProfiled() const;
	    void setType(const UI32 &type);
	    UI32 getType() const;
	    void setIsGwRequest(bool isGwRequest);
            bool getIsGwRequest() const;
            void getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer);
	    void transferOutputBuffer(L2sysShowMessage *&pMsg);
	    void copyAllBuffers (const PrismMessage &prismMessage);
        void setLastMacRecord(const l2sys_show_mac_output_msg_t *lastMacRecord);
        void getLastMacRecord(l2sys_show_mac_output_msg_t *lastMacRecord);
        void setIsFirstRequest(const UI32 &isFirstRequest);
        UI32 getIsFirstRequest() const;
	    // Now the data members

        private:
        protected:
        public:
            UI32  m_rbridgeid;
            string  m_Mac;
            UI32  m_interface_type;
            string  m_ifname;
            UI32  m_ifindex;
            UI32  m_vlan;
            UI32  m_vlanid;
            UI32  m_cmdcode;
	    string m_Profiled;
	    UI32 m_type;
	    bool m_isGwRequest;
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
#endif                                            //LSYSUPDATELSYSGLOBALSTATICMACSPMESSAGE_H
