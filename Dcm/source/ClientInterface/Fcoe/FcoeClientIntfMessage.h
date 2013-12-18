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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#ifndef FCOE_CLIENT_INTF_CONFIG_MESSAGE_H
#define FCOE_CLIENT_INTF_CONFIG_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeClientIntfMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientIntfMessage ();
            virtual    ~FcoeClientIntfMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setStatus(const bool &status);
            bool getStatus()  const;
            void setMode(const string &mode);
            string getMode()  const;
            void setBinding(const string &binding);
            string getBinding()  const;
            void setFcPortNum(const UI32 &uport_num);
            UI32 getFcPortNum () const;
            void setPortNum(const UI32 &port_num);
            UI32 getPortNum() const;
            void setVnNum(const UI32 &vn_num);
            UI32 getVnNum() const;
            void setBindingType(const UI32 &binding_type);
            UI32 getBindingType() const;
            void setConfigReplay(const UI32 &config_replay);
            UI32 getConfigReplay() const;
            virtual const void *getCStructureForInputs    ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            UI32    m_oper_bmp;
            string  m_ifName;
            UI64    m_ifIndex;
            string  m_desc;
            bool    m_status;
            string  m_mode;
            string  m_binding;
            UI32    m_fc_port_num;
            UI32    m_port_num;
            UI32    m_vn_num;
            UI32    m_binding_type;
            UI32    m_config_replay;
    };
}
#endif
