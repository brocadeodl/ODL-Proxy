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

#ifndef NSMLOCALFCOEVLANMESSAGE_H
#define NSMLOCALFCOEVLANMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalFcoeVlanMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLocalFcoeVlanMessage ();
            NsmLocalFcoeVlanMessage (const UI32 &cmd_code, const UI32 &old_vlan, const UI32 &new_vlan, const string &vlanName, const UI32 &vlanIfIndex, const string &m_desc);
            virtual    ~NsmLocalFcoeVlanMessage ();
            void setCmdCode(const UI32 &cmd_code);
            UI32 getCmdCode() const;
            void setOldVlan(const UI32 &old_vlan);
            UI32 getOldVlan()  const;
            void setNewVlan(const UI32 &new_vlan);
            UI32 getNewVlan()  const;
            void setVlanName(const string &vlanName);
            string getVlanName()  const;
            void setVlanIfIndex(const UI32 &vlanIfIndex);
            UI32 getVlanIfIndex()  const;
            void setDesc(const string &desc);
            string getDesc()  const;

        // Now the data members

        private:
        protected:
        public:
            UI32  m_cmdCode;
            UI32  m_old_vlan;
            UI32  m_new_vlan;
            string  m_vlanName;
            UI32  m_vlanIfIndex;
            string  m_desc;
    };
}
#endif                                            //NSMLOCALFCOEVLANINTFMESSAGE_H

