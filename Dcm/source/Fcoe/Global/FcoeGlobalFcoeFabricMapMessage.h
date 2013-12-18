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

#ifndef FCOE_GLOBAL_FCOE_FABRIC_MAPMESSAGE_H
#define FCOE_GLOBAL_FCOE_FABRIC_MAPMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeGlobalFcoeFabricMapMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeGlobalFcoeFabricMapMessage ();
            FcoeGlobalFcoeFabricMapMessage (const string &name, const uint32_t &oper_bmp, const uint32_t &vlan, const uint32_t &vfid, const uint32_t &priority, const string &fcmap, const uint32_t &adv_intvl, const uint32_t &timeout, const uint32_t &mtu, const uint32_t &max_enodes, const string &enodes_cfg_mode);
            virtual    ~FcoeGlobalFcoeFabricMapMessage ();
            void setName(const string &name);
            string getName()  const;
            void setOperBmp(const uint32_t &oper_bmp);
            uint32_t getOperBmp() const;
            void setVlan(const uint32_t &vlan);
            uint32_t getVlan() const;
            void setVfid(const uint32_t &vfid);
            uint32_t getVfid() const;
            void setPriority(const uint32_t &priority);
            uint32_t getPriority() const;
            void setFcmap(const string &fcmap);
            string getFcmap() const;
            void setAdvIntvl(const uint32_t &adv_intvl);
            uint32_t getAdvIntvl() const;
            void setTimeout(const bool &timeout);
            bool getTimeout() const;
            uint8_t* getFCFMacForLocation(const UI32 &loc_id);
            void setMtu(const uint32_t &mtu);
            uint32_t getMtu() const;
            void setMaxEnodes(const uint32_t &max_enodes);
            uint32_t getMaxEnodes() const;
            void setEnodesCfgMode(const string &enodes_cfg_mode);
            string getEnodesCfgMode()  const;
        // Now the data members

        private:
        protected:
        public:
            string      m_fabric_map_name;
            uint32_t    m_oper_bmp;
            uint32_t    m_vlan;
            uint32_t    m_vfid;
            uint32_t    m_priority;
            string      m_fcmap;
            uint32_t    m_adv_intvl;
            bool        m_timeout;
            uint32_t    m_mtu;
            uint32_t    m_max_enodes;
            string      m_enodes_cfg_mode;
    };
}
#endif                                            //FCOE_GLOBAL_FCOE_FABRIC_MAPMESSAGE_H
