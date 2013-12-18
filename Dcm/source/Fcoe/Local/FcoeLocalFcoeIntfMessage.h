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

#ifndef FCOE_LOCAL_UPD_FCOE_INTFMESSAGE_H
#define FCOE_LOCAL_UPD_FCOE_INTFMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"
using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalFcoeIntfMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalFcoeIntfMessage ();
            FcoeLocalFcoeIntfMessage (const UI32 &oper_bmp,
                                      const string &ifName = FCOE_INTF_DEFAULT_NAME,
                                      const bool &status = FCOE_INTF_STATUS_DEFAULT,
                                      const UI64 &ifIndex = FCOE_INTF_IFINDEX_INVALID,
                                      const UI32 &fc_uport_num = FCOE_INTF_UPORT_NUM_INVALID,
                                      const UI32 &vn_num = FCOE_INTF_VN_NUM_INVALID,
                                      const UI32 &port_num = FCOE_INTF_PORT_NUM_INVALID,
                                      const string &desc = FCOE_INTF_DEFAULT_DESCRIPTION,
                                      const string &mode = FCOE_INTF_DEFAULT_MODE,
                                      const string &binding = FCOE_INTF_DEFAULT_BINDING,
                                      const UI32 &bind_type = FCOE_INTERFACE_BIND_VAL_TYPE_NONE);

            virtual    ~FcoeLocalFcoeIntfMessage ();
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
            void setBindingType(const UI32 &binding_type);
            UI32 getBindingType()  const;
            void setFcUportNum(const UI32 &fc_uport_num);
            UI32 getFcUportNum() const;
            void setPortNum(const UI32 &port_num);
            UI32 getPortNum() const;
            void setVnNum(const UI32 &vn_num);
            UI32 getVnNum() const;

// Now the data members

        private:
        protected:
        public:
            UI32    m_oper_bmp;
            string  m_ifName;
            bool    m_status;
            UI64    m_ifIndex;
            UI32    m_fc_uport_num;
            UI32    m_vn_num;
            UI32    m_port_num;
            string  m_desc;
            string  m_mode;
            string  m_binding;
            UI32    m_binding_type;
    };
}
#endif                                            //FCOE_LOCAL_UPD_FCOE_INTFMESSAGE_H
