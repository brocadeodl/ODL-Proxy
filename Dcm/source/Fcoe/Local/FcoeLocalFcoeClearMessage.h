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

#ifndef FCOE_LOCAL_FCOE_CLEAR_MESSAGE_H
#define FCOE_LOCAL_FCOE_CLEAR_MESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class FcoeLocalFcoeClearMessage : public PrismMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            FcoeLocalFcoeClearMessage ();
            FcoeLocalFcoeClearMessage (const UI32 &cmd_code);
            virtual    ~FcoeLocalFcoeClearMessage ();
            void setCmdCode(const UI32 &cmdCode);
            UI32 getCmdCode()  const;

            UI32 getVlan() const;
            void setVlan(const UI32 &vlan);
            UI32 getVfid() const;
            void setVfid(const UI32 &vfid);
            UI32 getFcUport() const;
            void setFcUport(const UI32 &fc_uport);
            string getIfName() const;
            void setIfName(const string &if_name);
            void setWWN(const string &wwn);
            string getWWN() const;
            UI64 getIfindex() const;
            void setIfindex(const UI64 &if_index);
            UI32 getDestLocationId() const;
            void setDestLocationId(const UI32 &loc_id);
            UI32 getPo() const;
            void setPo(const UI32 &po);

// Now the data members

        private:
        protected:
        public:
            UI32    m_cmdCode;
            UI32    m_vlan;
            UI32    m_vfid;
            UI32    m_fc_uport;
            string  m_intf_name;
            string  m_wwn;
            UI64    m_if_index;
            UI32    m_locationId;
			UI32    m_po;
    };
}
#endif                                            //FCOE_LOCAL_FCOE_CLEAR_MESSAGE_H
