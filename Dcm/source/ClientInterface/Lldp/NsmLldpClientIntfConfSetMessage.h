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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef NSMLLDPCLIENTINTFCONFSETMESSAGE_H
#define NSMLLDPCLIENTINTFCONFSETMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "ClientInterface/Lldp/lldp_dcm_def.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLldpClientIntfConfSetMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmLldpClientIntfConfSetMessage ();
            NsmLldpClientIntfConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const string &id,const string &ifName,const UI64 &ifIndex,const UI8 &dcbxVersion,const bool &disable,const string &profile,const UI32 &iscsiPriority);
            virtual    ~NsmLldpClientIntfConfSetMessage ();
            void setMgmtFlags(const UI32 &mgmtFlags);
            UI32 getMgmtFlags()  const;
            void setCmdcode(const UI32 &cmdcode);
            UI32 getCmdcode()  const;
            void setMsgType(const UI32 &msgType);
            UI32 getMsgType()  const;
            void setId(const string &id);
            string getId()  const;
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfIndex(const UI64 &ifIndex);
            UI64 getIfIndex()  const;
            void setDcbxVersion(const UI8 &dcbxVersion);
            UI8 getDcbxVersion()  const;
            void setDisable(const bool &disable);
            bool getDisable()  const;
            void setProfile(const string &profile);
            string getProfile()  const;
            void setIscsiPriority(const UI32 &iscsiPriority);
            UI32 getIscsiPriority()  const;
            void setDescription(const string &description);
            string getDescription() const;
            void setIfType(const UI32 &ifType);
            UI32 getIfType()  const;
			void setLldpCeeStatus(const lldp_cee_on_off_t &lldpCeeType);
			lldp_cee_on_off_t getLldpCeeStatus() const;
			
			virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void 
													*pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
            UI32  m_mgmtFlags;
            UI32  m_cmdcode;
            UI32  m_msgType;
            string  m_id;
            string  m_ifName;
            UI64  m_ifIndex;
            UI8  m_dcbxVersion;
            bool  m_disable;
            string  m_profile;
            UI32  m_iscsiPriority;
            string  m_description;
            UI32 m_ifType;
			lldp_cee_on_off_t m_lldpCeeType;
    };
}
#endif                                            //NSMLLDPCLIENTINTFCONFSETMESSAGE_H
