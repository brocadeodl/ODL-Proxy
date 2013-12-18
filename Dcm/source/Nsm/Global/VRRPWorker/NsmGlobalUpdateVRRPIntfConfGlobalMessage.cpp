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

#include "Nsm/Global/VRRPWorker/NsmGlobalUpdateVRRPIntfConfGlobalMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdateVRRPIntfConfGlobalMessage::NsmGlobalUpdateVRRPIntfConfGlobalMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVRRPINTFCONFGLOBAL)
    {
    }

    NsmGlobalUpdateVRRPIntfConfGlobalMessage::NsmGlobalUpdateVRRPIntfConfGlobalMessage (const string &ifName,const InterfaceType &ifType,const UI8 &vrid,const IpV4Address &virtual_ip,const InterfaceType &track_ifType,const string &track_ifName,const UI8 &track_prio,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &desc,const bool &advt_backup,const UI32 &garp_timer,const UI32 &backup_advt_int,const bool &spf,const UI8 &rev_prio,const UI8 &vrrp_type,const UI32 &choice)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEVRRPINTFCONFGLOBAL),
        m_ifName    (ifName),
        m_ifType    (ifType),
        m_vrid    (vrid),
        m_virtual_ip    (virtual_ip),
        m_track_ifType    (track_ifType),
        m_track_ifName    (track_ifName),
        m_track_prio    (track_prio),
        m_advt_int    (advt_int),
        m_prio    (prio),
        m_enable    (enable),
        m_preempt    (preempt),
        m_hold_time    (hold_time),
        m_desc    (desc),
        m_advt_backup    (advt_backup),
        m_garp_timer    (garp_timer),
        m_backup_advt_int    (backup_advt_int),
		m_spf    (spf),
        m_rev_prio    (rev_prio),
        m_vrrp_type    (vrrp_type),
        m_choice    (choice)
    {
    }

    NsmGlobalUpdateVRRPIntfConfGlobalMessage::~NsmGlobalUpdateVRRPIntfConfGlobalMessage ()
    {
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI8(&m_vrid,"vrid"));
        addSerializableAttribute (new AttributeIpV4Address(&m_virtual_ip,"virtual_ip"));
        addSerializableAttribute (new AttributeEnum((UI32*)&m_track_ifType,"track_ifType"));
        addSerializableAttribute (new AttributeString(&m_track_ifName,"track_ifName"));
        addSerializableAttribute (new AttributeUI8(&m_track_prio,"track_prio"));
        addSerializableAttribute (new AttributeUI32(&m_advt_int,"advt_int"));
        addSerializableAttribute (new AttributeUI8(&m_prio,"prio"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_preempt,"preempt"));
        addSerializableAttribute (new AttributeUI32(&m_hold_time,"hold_time"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeBool(&m_advt_backup,"advt_backup"));
        addSerializableAttribute (new AttributeUI32(&m_garp_timer,"garp_timer"));
        addSerializableAttribute (new AttributeUI32(&m_backup_advt_int,"backup_advt_int"));
        addSerializableAttribute (new AttributeBool(&m_spf,"spf"));
        addSerializableAttribute (new AttributeUI8(&m_rev_prio,"rev_prio"));
        addSerializableAttribute (new AttributeUI8(&m_vrrp_type,"vrrp_type"));
        addSerializableAttribute (new AttributeUI32(&m_choice,"choice"));
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setIfType(const InterfaceType &ifType)
    {
        m_ifType  =  ifType;
    }

    InterfaceType NsmGlobalUpdateVRRPIntfConfGlobalMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getVrid() const
    {
        return (m_vrid);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setVirtual_ip(const IpV4Address &virtual_ip)
    {
        m_virtual_ip  =  virtual_ip;
    }

    IpV4Address  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getVirtual_ip() const
    {
        return (m_virtual_ip);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setTrack_ifType(const InterfaceType &track_ifType)
    {
        m_track_ifType  =  track_ifType;
    }

    InterfaceType NsmGlobalUpdateVRRPIntfConfGlobalMessage::getTrack_ifType() const
    {
        return (m_track_ifType);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setTrack_ifName(const string &track_ifName)
    {
        m_track_ifName  =  track_ifName;
    }

    string  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getTrack_ifName() const
    {
        return (m_track_ifName);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setTrack_prio(const UI8 &track_prio)
    {
        m_track_prio  =  track_prio;
    }

    UI8  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getTrack_prio() const
    {
        return (m_track_prio);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setAdvt_int(const UI32 &advt_int)
    {
        m_advt_int  =  advt_int;
    }

    UI32  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getAdvt_int() const
    {
        return (m_advt_int);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getPrio() const
    {
        return (m_prio);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getEnable() const
    {
        return (m_enable);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setPreempt(const bool &preempt)
    {
        m_preempt  =  preempt;
    }

    bool  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getPreempt() const
    {
        return (m_preempt);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setHold_time(const UI32 &hold_time)
    {
        m_hold_time  =  hold_time;
    }

    UI32  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getHold_time() const
    {
        return (m_hold_time);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setAdvt_backup(const bool &advt_backup)
    {
        m_advt_backup  =  advt_backup;
    }

    bool  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getAdvt_backup() const
    {
        return (m_advt_backup);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setGarp_timer(const UI32 &garp_timer)
    {
        m_garp_timer  =  garp_timer;
    }

    UI32  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getGarp_timer() const
    {
        return (m_garp_timer);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setBackup_advt_int(const UI32 &backup_advt_int)
    {
        m_backup_advt_int  =  backup_advt_int;
    }

    UI32  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getBackup_advt_int() const
    {
        return (m_backup_advt_int);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setSpf(const bool &spf)
    {
        m_spf  =  spf;
    }

    bool  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getSpf() const
    {
        return (m_spf);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setRevPrio(const UI8 &rev_prio)
    {
        m_rev_prio  =  rev_prio;
    }

    UI8  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getRevPrio() const
    {
        return (m_rev_prio);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setVrrp_type(const UI8 &vrrp_type)
    {
        m_vrrp_type  =  vrrp_type;
    }

    UI8 NsmGlobalUpdateVRRPIntfConfGlobalMessage::getVrrp_type() const
    {
        return (m_vrrp_type);
    }

    void  NsmGlobalUpdateVRRPIntfConfGlobalMessage::setChoice(const UI32 &choice)
    {
        m_choice  =  choice;
    }

    UI32  NsmGlobalUpdateVRRPIntfConfGlobalMessage::getChoice() const
    {
        return (m_choice);
    }

}
