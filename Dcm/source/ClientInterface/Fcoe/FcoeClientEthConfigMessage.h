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

#ifndef FCOE_CLIENT_ETH_CONFIG_MESSAGE_H
#define FCOE_CLIENT_ETH_CONFIG_MESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeClientEthConfigMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeClientEthConfigMessage ();
            virtual    ~FcoeClientEthConfigMessage ();
            void setOper_bmp(const UI32 &oper_bmp);
            UI32 getOper_bmp()  const;
            void setMap_name(const string &map_name);
            string getMap_name() const;
            void setCeeMapName(const string &cee_map_name);
            string getCeeMapName() const;
            void addFcoeVlan(const UI32 &vlan_id);
            void setIfId(const string &if_id);
            string getIfId() const;
            vector<UI32> getFcoeVlanVec() const;
            void setFcoeVlanVec(const vector<UI32> &vlan_vec);
            void setFcoeMbrIfindexVec(const vector<UI64> &ifindex_vec);
            UI64 getIfindex() const;
            void setIfindex(const UI64 &ifindex);
            void addMbrIfindex(const UI64 &ifindex);
            virtual const void *getCStructureForInputs    ();
            virtual       void  loadOutputsFromCStructure (const void *pOutputCStructure);

        // Now the data members

        private:
        protected:
        public:
            UI32            m_oper_bmp;
            string          m_map_name;
            string          m_cee_map_name;
            string          m_if_id;
            UI64            m_ifindex;
            vector<UI32>    m_fcoe_vlan_vec;
            vector<UI64>    m_mbr_ifindex_list;
    };
}
#endif
