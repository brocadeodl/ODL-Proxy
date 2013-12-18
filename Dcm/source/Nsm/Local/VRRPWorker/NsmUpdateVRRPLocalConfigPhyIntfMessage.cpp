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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/VRRPWorker/NsmUpdateVRRPLocalConfigPhyIntfMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmUpdateVRRPLocalConfigPhyIntfMessage::NsmUpdateVRRPLocalConfigPhyIntfMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVRRPLOCALCONFIGPHYINTF)
    {
    }

    NsmUpdateVRRPLocalConfigPhyIntfMessage::NsmUpdateVRRPLocalConfigPhyIntfMessage (const string &ifName,const InterfaceType &ifType,const UI8 &vrid,const IpV4Address &virtual_ip,const InterfaceType &track_ifType,const string &track_ifName,const UI8 &track_prio,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &description,const bool &advt_backup,const UI32 &garp_timer,const UI32 &backup_advt_int,const bool &spf,const UI8 &rev_prio,const UI8 &vrrp_type,const UI32 &choice,const UI32 &op_type)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEVRRPLOCALCONFIGPHYINTF),
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
        m_description    (description),
        m_advt_backup    (advt_backup),
        m_garp_timer    (garp_timer),
        m_backup_advt_int    (backup_advt_int),
		m_spf    (spf),
        m_rev_prio    (rev_prio),
        m_vrrp_type    (vrrp_type),
        m_choice    (choice),
        m_op_type    (op_type)
    {
    }

    NsmUpdateVRRPLocalConfigPhyIntfMessage::~NsmUpdateVRRPLocalConfigPhyIntfMessage ()
    {
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeBool(&m_advt_backup,"advt_backup"));
        addSerializableAttribute (new AttributeUI32(&m_garp_timer,"garp_timer"));
        addSerializableAttribute (new AttributeUI32(&m_backup_advt_int,"backup_advt_int"));
        addSerializableAttribute (new AttributeBool(&m_spf,"spf"));
        addSerializableAttribute (new AttributeUI8(&m_rev_prio,"rev_prio"));
        addSerializableAttribute (new AttributeUI8(&m_vrrp_type,"vrrp_type"));
        addSerializableAttribute (new AttributeUI32(&m_choice,"choice"));
        addSerializableAttribute (new AttributeUI32(&m_op_type,"op_type"));
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmUpdateVRRPLocalConfigPhyIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setIfType(const InterfaceType &ifType)
    {
        m_ifType  =  ifType;
    }

    InterfaceType  NsmUpdateVRRPLocalConfigPhyIntfMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  NsmUpdateVRRPLocalConfigPhyIntfMessage::getVrid() const
    {
        return (m_vrid);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setVirtual_ip(const IpV4Address &virtual_ip)
    {
        m_virtual_ip  =  virtual_ip;
    }

    IpV4Address  NsmUpdateVRRPLocalConfigPhyIntfMessage::getVirtual_ip() const
    {
        return (m_virtual_ip);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setTrack_ifType(const InterfaceType &track_ifType)
    {
        m_track_ifType  =  track_ifType;
    }

    InterfaceType NsmUpdateVRRPLocalConfigPhyIntfMessage::getTrack_ifType() const
    {
        return (m_track_ifType);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setTrack_ifName(const string &track_ifName)
    {
        m_track_ifName  =  track_ifName;
    }

    string  NsmUpdateVRRPLocalConfigPhyIntfMessage::getTrack_ifName() const
    {
        return (m_track_ifName);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setTrack_prio(const UI8 &track_prio)
    {
        m_track_prio  =  track_prio;
    }

    UI8  NsmUpdateVRRPLocalConfigPhyIntfMessage::getTrack_prio() const
    {
        return (m_track_prio);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setAdvt_int(const UI32 &advt_int)
    {
        m_advt_int  =  advt_int;
    }

    UI32  NsmUpdateVRRPLocalConfigPhyIntfMessage::getAdvt_int() const
    {
        return (m_advt_int);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  NsmUpdateVRRPLocalConfigPhyIntfMessage::getPrio() const
    {
        return (m_prio);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  NsmUpdateVRRPLocalConfigPhyIntfMessage::getEnable() const
    {
        return (m_enable);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setPreempt(const bool &preempt)
    {
        m_preempt  =  preempt;
    }

    bool  NsmUpdateVRRPLocalConfigPhyIntfMessage::getPreempt() const
    {
        return (m_preempt);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setHold_time(const UI32 &hold_time)
    {
        m_hold_time  =  hold_time;
    }

    UI32  NsmUpdateVRRPLocalConfigPhyIntfMessage::getHold_time() const
    {
        return (m_hold_time);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  NsmUpdateVRRPLocalConfigPhyIntfMessage::getDescription() const
    {
        return (m_description);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setAdvt_backup(const bool &advt_backup)
    {
        m_advt_backup  =  advt_backup;
    }

    bool  NsmUpdateVRRPLocalConfigPhyIntfMessage::getAdvt_backup() const
    {
        return (m_advt_backup);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setGarp_timer(const UI32 &garp_timer)
    {
        m_garp_timer  =  garp_timer;
    }

    UI32  NsmUpdateVRRPLocalConfigPhyIntfMessage::getGarp_timer() const
    {
        return (m_garp_timer);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setBackup_advt_int(const UI32 &backup_advt_int)
    {
        m_backup_advt_int  =  backup_advt_int;
    }

    UI32  NsmUpdateVRRPLocalConfigPhyIntfMessage::getBackup_advt_int() const
    {
        return (m_backup_advt_int);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setSpf(const bool &spf)
    {
        m_spf  =  spf;
    }

    bool  NsmUpdateVRRPLocalConfigPhyIntfMessage::getSpf() const
    {
        return (m_spf);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setRevPrio(const UI8 &rev_prio)
    {
        m_rev_prio  =  rev_prio;
    }

    UI8  NsmUpdateVRRPLocalConfigPhyIntfMessage::getRevPrio() const
    {
        return (m_rev_prio);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setVrrp_type(const UI8 &vrrp_type)
    {
        m_vrrp_type  =  vrrp_type;
    }

    UI8 NsmUpdateVRRPLocalConfigPhyIntfMessage::getVrrp_type() const
    {
        return (m_vrrp_type);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setChoice(const UI32 &choice)
    {
        m_choice  =  choice;
    }

    UI32  NsmUpdateVRRPLocalConfigPhyIntfMessage::getChoice() const
    {
        return (m_choice);
    }

    void  NsmUpdateVRRPLocalConfigPhyIntfMessage::setOp_type(const UI32 &op_type)
    {
        m_op_type  =  op_type;
    }

    UI32 NsmUpdateVRRPLocalConfigPhyIntfMessage::getOp_type() const
    {
        return (m_op_type);
    }

}
