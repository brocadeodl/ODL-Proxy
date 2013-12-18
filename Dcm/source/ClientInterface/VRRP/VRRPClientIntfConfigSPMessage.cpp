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

#include "ClientInterface/VRRP/VRRPClientIntfConfigSPMessage.h"
#include "ClientInterface/VRRP/VRRPMessageDef.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/Nsm/NsmUtils.h"

namespace DcmNs
{

    VRRPClientIntfConfigSPMessage::VRRPClientIntfConfigSPMessage ()
             : DcmManagementInterfaceMessage (VRRP_INTF_CONFIG_CLIENT_MSG)
    {
		m_intf_ip_addr = 0;
		m_intf_ip_mask = 0;
		m_vipList.clear();
		m_trackList_ifType.clear();
		m_trackList_ifName.clear();
		m_trackList_prio.clear();
    }

    VRRPClientIntfConfigSPMessage::VRRPClientIntfConfigSPMessage (const string &ifName,const UI8 &ifType, const UI8 &vrid,const IpV4Address &virtual_ip,const UI8 &track_ifType,const string &track_ifName,const UI8 &track_prio,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &description,const bool &advt_backup,const UI32 &garp_timer,const UI32 &backup_advt_int,const bool &spf,const bool &owner,const UI8 &rev_prio,const UI8 &vrrp_type,const UI32 &choice)
        : DcmManagementInterfaceMessage (VRRP_INTF_CONFIG_CLIENT_MSG),
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
		m_owner    (owner),
        m_rev_prio    (rev_prio),
        m_vrrp_type    (vrrp_type),
        m_choice    (choice)
    {
		m_intf_ip_addr = 0;
		m_intf_ip_mask = 0;
		m_vipList.clear();
		m_trackList_ifType.clear();
		m_trackList_ifName.clear();
		m_trackList_prio.clear();
    }

    VRRPClientIntfConfigSPMessage::~VRRPClientIntfConfigSPMessage ()
    {
    }

    void  VRRPClientIntfConfigSPMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI8(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeUI8(&m_vrid,"vrid"));
        addSerializableAttribute (new AttributeIpV4Address(&m_virtual_ip,"virtual_ip"));
        addSerializableAttribute (new AttributeUI8(&m_track_ifType,"track_ifType"));
        addSerializableAttribute (new AttributeString(&m_track_ifName,"track_ifName"));
        addSerializableAttribute (new AttributeUI8(&m_track_prio,"track_prio"));
        addSerializableAttribute (new AttributeUI32(&m_advt_int,"advt_int"));
        addSerializableAttribute (new AttributeUI8(&m_prio,"prio"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_preempt,"preempt"));
        addSerializableAttribute (new AttributeUI32(&m_hold_time,"hold_time"));
        addSerializableAttribute (new AttributeString(&m_description,"description"));
        addSerializableAttribute (new AttributeBool(&m_owner,"owner"));
        addSerializableAttribute (new AttributeBool(&m_advt_backup,"advt_backup"));
        addSerializableAttribute (new AttributeUI32(&m_garp_timer,"garp_timer"));
        addSerializableAttribute (new AttributeUI32(&m_backup_advt_int,"backup_advt_int"));
		addSerializableAttribute (new AttributeBool(&m_spf,"spf"));
        addSerializableAttribute (new AttributeUI32(&m_intf_ip_addr,"intf_ip_addr"));
        addSerializableAttribute (new AttributeUI32(&m_intf_ip_mask,"intf_ip_mask"));
        addSerializableAttribute (new AttributeUI8(&m_rev_prio,"rev_prio"));
        addSerializableAttribute (new AttributeUI8(&m_vrrp_type,"vrrp_type"));
        addSerializableAttribute (new AttributeUI32(&m_choice,"choice"));

		addSerializableAttribute ( new AttributeIpV4AddressVector(&m_vipList, "vipList"));
		addSerializableAttribute ( new AttributeUI32Vector(&m_trackList_ifType, "trackList_ifType"));
		addSerializableAttribute ( new AttributeStringVector(&m_trackList_ifName, "trackList_ifName"));
		addSerializableAttribute ( new AttributeUI32Vector(&m_trackList_prio, "trackList_prio"));
		addSerializableAttribute ( new AttributeUI8(&m_num_vips, "num_vips"));
		addSerializableAttribute ( new AttributeUI8(&m_num_track_ports, "num_track_ports"));
    }

    void  VRRPClientIntfConfigSPMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  VRRPClientIntfConfigSPMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  VRRPClientIntfConfigSPMessage::setIfType(const UI8 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI8  VRRPClientIntfConfigSPMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  VRRPClientIntfConfigSPMessage::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  VRRPClientIntfConfigSPMessage::getVrid() const
    {
        return (m_vrid);
    }

    void  VRRPClientIntfConfigSPMessage::setVirtual_ip(const IpV4Address &virtual_ip)
    {
        m_virtual_ip  =  virtual_ip;
    }

    IpV4Address  VRRPClientIntfConfigSPMessage::getVirtual_ip() const
    {
        return (m_virtual_ip);
    }

    void  VRRPClientIntfConfigSPMessage::setTrack_ifType(const UI8 &track_ifType)
    {
        m_track_ifType  =  track_ifType;
    }

    UI8 VRRPClientIntfConfigSPMessage::getTrack_ifType() const
    {
        return (m_track_ifType);
    }

    void  VRRPClientIntfConfigSPMessage::setTrack_ifName(const string &track_ifName)
    {
        m_track_ifName  =  track_ifName;
    }

    string  VRRPClientIntfConfigSPMessage::getTrack_ifName() const
    {
        return (m_track_ifName);
    }

    void  VRRPClientIntfConfigSPMessage::setTrack_prio(const UI8 &track_prio)
    {
        m_track_prio  =  track_prio;
    }

    UI8  VRRPClientIntfConfigSPMessage::getTrack_prio() const
    {
        return (m_track_prio);
    }

    void  VRRPClientIntfConfigSPMessage::setAdvt_int(const UI32 &advt_int)
    {
        m_advt_int  =  advt_int;
    }

    UI32  VRRPClientIntfConfigSPMessage::getAdvt_int() const
    {
        return (m_advt_int);
    }

    void  VRRPClientIntfConfigSPMessage::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  VRRPClientIntfConfigSPMessage::getPrio() const
    {
        return (m_prio);
    }

    void  VRRPClientIntfConfigSPMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  VRRPClientIntfConfigSPMessage::getEnable() const
    {
        return (m_enable);
    }

    void  VRRPClientIntfConfigSPMessage::setPreempt(const bool &preempt)
    {
        m_preempt  =  preempt;
    }

    bool  VRRPClientIntfConfigSPMessage::getPreempt() const
    {
        return (m_preempt);
    }

    void  VRRPClientIntfConfigSPMessage::setHold_time(const UI32 &hold_time)
    {
        m_hold_time  =  hold_time;
    }

    UI32  VRRPClientIntfConfigSPMessage::getHold_time() const
    {
        return (m_hold_time);
    }

    void  VRRPClientIntfConfigSPMessage::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  VRRPClientIntfConfigSPMessage::getDescription() const
    {
        return (m_description);
    }

    void  VRRPClientIntfConfigSPMessage::setAdvt_backup(const bool &advt_backup)
    {
        m_advt_backup  =  advt_backup;
    }

    bool  VRRPClientIntfConfigSPMessage::getAdvt_backup() const
    {
        return (m_advt_backup);
    }

    void  VRRPClientIntfConfigSPMessage::setOwner(const bool &owner)
    {
        m_owner  =  owner;
    }

    bool  VRRPClientIntfConfigSPMessage::getOwner() const
    {
        return (m_owner);
    }

    void  VRRPClientIntfConfigSPMessage::setGarp_timer(const UI32 &garp_timer)
    {
        m_garp_timer  =  garp_timer;
    }

    UI32  VRRPClientIntfConfigSPMessage::getGarp_timer() const
    {
        return (m_garp_timer);
    }

    void  VRRPClientIntfConfigSPMessage::setBackup_advt_int(const UI32 &backup_advt_int)
    {
        m_backup_advt_int  =  backup_advt_int;
    }

    UI32  VRRPClientIntfConfigSPMessage::getBackup_advt_int() const
    {
        return (m_backup_advt_int);
    }

    void  VRRPClientIntfConfigSPMessage::setSpf(const bool &spf)
    {
        m_spf  =  spf;
    }

    bool  VRRPClientIntfConfigSPMessage::getSpf() const
    {
        return (m_spf);
    }

    void  VRRPClientIntfConfigSPMessage::setIntf_ip_addr(const UI32 &intf_ip_addr)
    {   
        m_intf_ip_addr  =  intf_ip_addr;
    }

    UI32  VRRPClientIntfConfigSPMessage::getIntf_ip_addr() const
    {   
        return (m_intf_ip_addr);
    }

    void  VRRPClientIntfConfigSPMessage::setIntf_ip_mask(const UI32 &intf_ip_mask)
    {   
        m_intf_ip_mask  =  intf_ip_mask;
    }

    UI32  VRRPClientIntfConfigSPMessage::getIntf_ip_mask() const
    {   
        return (m_intf_ip_mask);
    }

    void  VRRPClientIntfConfigSPMessage::setRevPrio(const UI8 &rev_prio)
    {
        m_rev_prio  =  rev_prio;
    }

    UI8  VRRPClientIntfConfigSPMessage::getRevPrio() const
    {
        return (m_rev_prio);
    }

    void  VRRPClientIntfConfigSPMessage::setVrrp_type(const UI8 &vrrp_type)
    {
        m_vrrp_type  =  vrrp_type;
    }

    UI8 VRRPClientIntfConfigSPMessage::getVrrp_type() const
    {
        return (m_vrrp_type);
    }

    void  VRRPClientIntfConfigSPMessage::setChoice(const UI32 &choice)
    {
        m_choice  =  choice;
    }

    UI32  VRRPClientIntfConfigSPMessage::getChoice() const
    {
        return (m_choice);
    }

    const void *VRRPClientIntfConfigSPMessage::getCStructureForInputs ()
    {
        string str_ipaddr;
        vrrp_local_config_msg_t *pInput = NULL;
        string twoTupleName = "";

		if (m_choice == VRRP_INTF_CONFIG_REPLAY) {
			pInput = (vrrp_local_config_msg_t *)malloc (sizeof (struct vrrp_local_config_msg_) + sizeof (vrrp_config_replay_info_t));
		} else {
			pInput = (vrrp_local_config_msg_t *)malloc (sizeof (struct vrrp_local_config_msg_));
		}

		if (NsmUtils::isValidThreeTuple(m_ifName)) {
            NsmUtils::getTwoTupleIfName(m_ifName, twoTupleName);
        } else {
            twoTupleName = m_ifName;
        }

        strncpy (pInput->ifName, twoTupleName.c_str (), VRRP_IFNAME_LEN);
        pInput->ifType = m_ifType;
        pInput->vrid = m_vrid;
        /* Virtual IP */
        str_ipaddr = m_virtual_ip.toString();
        strncpy (pInput->virtual_ip, str_ipaddr.c_str (), IPV4_ADDR_STR_SIZE);
        pInput->virtual_ip[15] = '\0';
        pInput->track_ifType = m_track_ifType;

		twoTupleName = "";
		if (NsmUtils::isValidThreeTuple(m_track_ifName)) {
			NsmUtils::getTwoTupleIfName(m_track_ifName, twoTupleName);
		} else {
			twoTupleName = m_track_ifName;
		}
        strncpy (pInput->track_ifName, twoTupleName.c_str (), VRRP_IFNAME_LEN);

        pInput->track_prio = m_track_prio;
        pInput->advt_int = m_advt_int;
        pInput->prio = m_prio;
        pInput->enable = m_enable;
        pInput->preempt = m_preempt;
        pInput->hold_time = m_hold_time;
        strncpy (pInput->description, m_description.c_str (), VRRP_DESCRIPTION_MAX_LEN-1);
		pInput->description[VRRP_DESCRIPTION_MAX_LEN-1] = '\0';
        pInput->owner = m_owner;
        pInput->advt_backup = m_advt_backup;
        pInput->garp_timer = m_garp_timer;
        pInput->backup_advt_int = m_backup_advt_int;
		pInput->spf = m_spf;
        pInput->intf_ip_addr = m_intf_ip_addr;
        pInput->intf_ip_mask = m_intf_ip_mask;
        pInput->rev_prio = m_rev_prio;
        pInput->vrrp_type = m_vrrp_type;
        pInput->choice = m_choice;

		if (m_choice == VRRP_INTF_CONFIG_REPLAY) {
			vrrp_config_replay_info_t	*config = (vrrp_config_replay_info_t*)pInput->config_replay;
			unsigned int i = 0;

			if (m_vipList.size() > MAX_VRRP_VIRTUAL_IPS) {
				config->num_virtual_ip = MAX_VRRP_VIRTUAL_IPS;
			} else {
				config->num_virtual_ip = m_vipList.size();
			}

			if (m_trackList_ifType.size() > MAX_TRACKPORT_INTERFACES) {
				config->num_track_port = MAX_TRACKPORT_INTERFACES;
			} else {
				config->num_track_port = m_trackList_ifType.size();
			}

			for (i = 0; i < config->num_virtual_ip; i++) {
				str_ipaddr = m_vipList[i].toString();
				strncpy (config->vip_list[i].virtual_ip, str_ipaddr.c_str (), IPV4_ADDR_STR_SIZE);
				config->vip_list[i].virtual_ip[15] = '\0';
			}

			for (i = 0; i < config->num_track_port; i++) {
				twoTupleName = "";
				if (NsmUtils::isValidThreeTuple(m_trackList_ifName[i])) {
					NsmUtils::getTwoTupleIfName(m_trackList_ifName[i], twoTupleName);
				} else {
					twoTupleName = m_trackList_ifName[i];
				}
				strncpy (config->track_list[i].track_ifName, twoTupleName.c_str (), VRRP_IFNAME_LEN);
				config->track_list[i].track_ifType = (UI8)m_trackList_ifType[i];
				config->track_list[i].track_prio = (UI8)m_trackList_prio[i];
			}
		}

        return (pInput);
    }

    void VRRPClientIntfConfigSPMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
    {
    }


}
