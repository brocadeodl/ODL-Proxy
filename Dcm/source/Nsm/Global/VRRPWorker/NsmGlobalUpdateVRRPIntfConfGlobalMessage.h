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
 *   Author : krao                                                     *
 **************************************************************************/

#ifndef NSMGLOBALUPDATEVRRPINTFCONFGLOBALMESSAGE_H
#define NSMGLOBALUPDATEVRRPINTFCONFGLOBALMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalUpdateVRRPIntfConfGlobalMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            NsmGlobalUpdateVRRPIntfConfGlobalMessage ();
            NsmGlobalUpdateVRRPIntfConfGlobalMessage (const string &ifName,const InterfaceType &ifType,const UI8 &vrid,const IpV4Address &virtual_ip,const InterfaceType &track_ifType,const string &track_ifName,const UI8 &track_prio,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &desc,const bool &advt_backup,const UI32 &garp_timer,const UI32 &backup_advt_int,const bool &spf,const UI8 &rev_prio,const UI8 &vrrp_type,const UI32 &choice);
            virtual    ~NsmGlobalUpdateVRRPIntfConfGlobalMessage ();
            void setIfName(const string &ifName);
            string getIfName()  const;
            void setIfType(const InterfaceType &ifType);
            InterfaceType getIfType()  const;
            void setVrid(const UI8 &vrid);
            UI8 getVrid()  const;
            void setVirtual_ip(const IpV4Address &virtual_ip);
            IpV4Address getVirtual_ip()  const;
            void setTrack_ifType(const InterfaceType &track_ifType);
            InterfaceType getTrack_ifType()  const;
            void setTrack_ifName(const string &track_ifName);
            string getTrack_ifName()  const;
            void setTrack_prio(const UI8 &track_prio);
            UI8 getTrack_prio()  const;
            void setAdvt_int(const UI32 &advt_int);
            UI32 getAdvt_int()  const;
            void setPrio(const UI8 &prio);
            UI8 getPrio()  const;
            void setEnable(const bool &enable);
            bool getEnable()  const;
            void setPreempt(const bool &preempt);
            bool getPreempt()  const;
            void setHold_time(const UI32 &hold_time);
            UI32 getHold_time()  const;
            void setDesc(const string &desc);
            string getDesc()  const;
            void setAdvt_backup(const bool &advt_backup);
            bool getAdvt_backup()  const;
            void setGarp_timer(const UI32 &garp_timer);
            UI32 getGarp_timer()  const;
            void setBackup_advt_int(const UI32 &backup_advt_int);
            UI32 getBackup_advt_int()  const;
			void setSpf(const bool &spf);
			bool getSpf()  const;
            void setRevPrio(const UI8 &rev_prio);
            UI8 getRevPrio()  const;
            void setVrrp_type(const UI8 &vrrp_type);
            UI8 getVrrp_type()  const;
            void setChoice(const UI32 &choice);
            UI32 getChoice()  const;

// Now the data members

        private:
        protected:
        public:
            string  m_ifName;
            InterfaceType m_ifType;
            UI8  m_vrid;
            IpV4Address  m_virtual_ip;
            InterfaceType m_track_ifType;
            string m_track_ifName;
            UI8  m_track_prio;
            UI32  m_advt_int;
            UI8  m_prio;
            bool  m_enable;
            bool  m_preempt;
            UI32  m_hold_time;
            string  m_desc;
            bool  m_advt_backup;
            UI32  m_garp_timer;
            UI32  m_backup_advt_int;
			bool  m_spf;
            UI8  m_rev_prio;
            UI8 m_vrrp_type;
            UI32  m_choice;
    };
}
#endif                                            //NSMGLOBALUPDATEVRRPINTFCONFGLOBALMESSAGE_H
