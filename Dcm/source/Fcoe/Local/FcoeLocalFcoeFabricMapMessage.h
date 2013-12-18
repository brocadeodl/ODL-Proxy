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

#ifndef FCOE_LOCAL_FCOE_FABRIC_MAPMESSAGE_H
#define FCOE_LOCAL_FCOE_FABRIC_MAPMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalFcoeFabricMapMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalFcoeFabricMapMessage ();
            FcoeLocalFcoeFabricMapMessage (const UI32 &oper_bmp,const string &fabric_map_name,const UI32 &vlan,const UI32 &vfid,const UI32 &priority,const string &fcmap,const UI32 &fka_intvl,const bool &timeout, const UI32 &mtu, const UI32 &max_enodes, const string &enodes_cfg_mode);
            virtual    ~FcoeLocalFcoeFabricMapMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setFabric_map_name(const string &fabric_map_name);
            string getFabric_map_name()  const;
            void setVlan(const UI32 &vlan);
            UI32 getVlan()  const;
            void setVfid(const UI32 &vfid);
            UI32 getVfid()  const;
            void setPriority(const UI32 &priority);
            UI32 getPriority()  const;
            void setFcmap(const string &fcmap);
            string getFcmap()  const;
            void setFka_intvl(const UI32 &fka_intvl);
            UI32 getFka_intvl()  const;
            void setTimeout(const bool &timeout);
            bool getTimeout()  const;
            void setMtu(const UI32 &mtu);
            UI32 getMtu()  const;
            void setMaxEnodes(const UI32 &mtu);
            UI32 getMaxEnodes()  const;
            void setEnodesCfgMode(const string &enodes_cfg_mode);
            string getEnodesCfgMode()  const;

// Now the data members

        private:
        protected:
        public:
            UI32  m_oper_bmp;
            string  m_fabric_map_name;
            UI32  m_vlan;
            UI32  m_vfid;
            UI32  m_priority;
            string  m_fcmap;
            UI32  m_fka_intvl;
            bool  m_timeout;
            UI32  m_mtu;
            UI32  m_max_enodes;
            string  m_enodes_cfg_mode;
    };
}
#endif                                            //FCOE_LOCAL_FCOE_FABRIC_MAPMESSAGE_H
