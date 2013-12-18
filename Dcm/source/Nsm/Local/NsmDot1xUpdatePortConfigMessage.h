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

#ifndef NSMDOTXUPDATEPORTCONFIGMESSAGE_H
#define NSMDOTXUPDATEPORTCONFIGMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmDot1xUpdatePortConfigMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmDot1xUpdatePortConfigMessage ();
            NsmDot1xUpdatePortConfigMessage (const string & ifName, const bool &enabled,const UI32 &port_ctrl_type,const UI32 &proto_version,const UI32 &quiet_interval,const UI32 &max_reauth_attempt,const bool &enable_auth,const UI32 &reauth_timeout_interval,const UI32 &server_timeout_interval,const UI32 &supp_timeout_interval,const UI32 &tx_timeout_interval,const UI32 &opcode,const UI32 &cmdcode);
            virtual    ~NsmDot1xUpdatePortConfigMessage ();
            void setEnabled(const bool &enabled);
            bool getEnabled()  const;
            void setPort_ctrl_type(const UI32 &port_ctrl_type);
            UI32 getPort_ctrl_type()  const;
            void setProto_version(const UI32 &proto_version);
            UI32 getProto_version()  const;
            void setQuiet_interval(const UI32 &quiet_interval);
            UI32 getQuiet_interval()  const;
            void setMax_reauth_attempt(const UI32 &max_reauth_attempt);
            UI32 getMax_reauth_attempt()  const;
            void setEnable_auth(const bool &enable_auth);
            bool getEnable_auth()  const;
            void setReauth_timeout_interval(const UI32 &reauth_timeout_interval);
            UI32 getReauth_timeout_interval()  const;
            void setServer_timeout_interval(const UI32 &server_timeout_interval);
            UI32 getServer_timeout_interval()  const;
            void setSupp_timeout_interval(const UI32 &supp_timeout_interval);
            UI32 getSupp_timeout_interval()  const;
            void setTx_timeout_interval(const UI32 &tx_timeout_interval);
            UI32 getTx_timeout_interval()  const;

            void setOpcode(const UI32 &opcode);
            UI32 getOpcode()  const;
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
            void setIfname(string  &ifName);
            string getIfname()  const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;

// Now the data members

        private:
        protected:
        public:
			string m_ifName;
            bool  m_enabled;
            UI32  m_port_ctrl_type;
            UI32  m_proto_version;
            UI32  m_quiet_interval;
            UI32  m_max_reauth_attempt;
            bool  m_enable_auth;
            UI32  m_reauth_timeout_interval;
            UI32  m_server_timeout_interval;
            UI32  m_supp_timeout_interval;
            UI32  m_tx_timeout_interval;
            UI32  m_opcode;
            UI32  m_cmdcode;
            UI32  m_ifType;
    };
}
#endif                                            //NSMDOTXUPDATEPORTCONFIGMESSAGE_H
