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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : krao                                                     *
 **************************************************************************/

#include "Nsm/Local/VRRPWorker/VRRPESessionConfigLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"
#include <arpa/inet.h>
#include <stdlib.h>

namespace DcmNs
{

    VRRPESessionConfigLocalManagedObject::VRRPESessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPESessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_vip.clear();
		m_track.clear();
    }

    VRRPESessionConfigLocalManagedObject::VRRPESessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI8 &vrid,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &description,const bool &advt_backup,const UI32 &garp_timer,const UI32 &backup_advt_int,const bool &spf,const UI8 &rev_prio,const InterfaceType &iftype,const string &ifname)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPESessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_vrid    (vrid),
//        m_vip    (vip),
//        m_track    (track),
        m_advt_int    (advt_int),
        m_prio    (prio),
        m_enable    (enable),
        m_preempt    (preempt),
        m_hold_time    (hold_time),
        m_description      (description),
        m_advt_backup    (advt_backup),
        m_garp_timer    (garp_timer),
        m_backup_advt_int    (backup_advt_int),
        m_spf    (spf),
        m_rev_prio    (rev_prio),
		m_iftype    (iftype),
		m_ifname    (ifname)
    {
		m_vip.clear();
		m_track.clear();
    }

    VRRPESessionConfigLocalManagedObject::~VRRPESessionConfigLocalManagedObject ()
    {
    }

    string  VRRPESessionConfigLocalManagedObject::getClassName()
    {
        return ("VRRPESessionConfigLocalManagedObject");
    }

    void  VRRPESessionConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeUI32(&m_advt_int,"advt_int", vrrp_advertisement_interval));
        addPersistableAttribute (new AttributeUI8(&m_prio,"prio", vrrp_priority));
        addPersistableAttribute (new AttributeBool(&m_enable,"enable", vrrp_enable));
        addPersistableAttribute (new AttributeBool(&m_preempt,"preempt", vrrp_preempt_mode));
        addPersistableAttribute (new AttributeUI32(&m_hold_time,0,"hold_time", vrrp_hold_time));
        addPersistableAttribute (new AttributeString(&m_description,true,"","description",vrrp_description));
        addPersistableAttribute (new AttributeBool(&m_advt_backup,"advt_backup", vrrp_advertise_backup));
        addPersistableAttribute (new AttributeUI32(&m_garp_timer,"garp_timer", vrrp_vrrpe_garp_timer));
        addPersistableAttribute (new AttributeUI32(&m_backup_advt_int,"backup_advt_int", vrrp_backup_advertisement_interval));
        addPersistableAttribute (new AttributeBool(&m_spf,"spf", vrrp_basic));
        addPersistableAttribute (new AttributeUI8(&m_rev_prio,0,"rev_prio", vrrp_revert_priority));
		addPersistableAttribute (new AttributeString(&m_ifname,true,"","ifname"));
		addPersistableAttribute (new AttributeEnum((UI32 *)&m_iftype,"iftype"));
    }

    void  VRRPESessionConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_advt_int,"advt_int", vrrp_advertisement_interval));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_prio,"prio", vrrp_priority));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable", vrrp_enable));
        addPersistableAttributeForCreate  (new AttributeBool(&m_preempt,"preempt", vrrp_preempt_mode));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_hold_time,0,"hold_time", vrrp_hold_time));
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description", vrrp_description));
        addPersistableAttributeForCreate  (new AttributeBool(&m_advt_backup,"advt_backup", vrrp_advertise_backup));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_garp_timer,"garp_timer", vrrp_vrrpe_garp_timer));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_backup_advt_int,"backup_advt_int", vrrp_backup_advertisement_interval));
        addPersistableAttributeForCreate  (new AttributeBool(&m_spf,"spf", vrrp_basic));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_rev_prio,0,"rev_prio", vrrp_revert_priority));
		addPersistableAttributeForCreate  (new AttributeString(&m_ifname,"ifname"));
		addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_iftype,"iftype"));

        vector<string> keyName;
        keyName.push_back ("vrid");
        setUserDefinedKeyCombination (keyName);
    }

    void  VRRPESessionConfigLocalManagedObject::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  VRRPESessionConfigLocalManagedObject::getVrid() const
    {
        return (m_vrid);
    }

    void  VRRPESessionConfigLocalManagedObject::setVip(const vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > &vip)
    {
        m_vip  =  vip;
    }

    vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> >  VRRPESessionConfigLocalManagedObject::getVip() const
    {
        return (m_vip);
    }

	WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> VRRPESessionConfigLocalManagedObject::searchVirtualIPAddr(IpV4Address ipaddr)
	{
		unsigned int i = 0;

		for (; i < m_vip.size(); i++)
		{
			if(m_vip[i]->m_virtual_ip == ipaddr)
			{
				return m_vip[i];
			}
		}
		return NULL;
	}

	bool VRRPESessionConfigLocalManagedObject::addVirtualIPMO(VRRPVirtualIPLocalManagedObject *pMO)
	{
		WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> ipaddr(pMO);
		unsigned int i = 0;
		bool found = false;

		for (; i < m_vip.size(); i++)
		{
			if ((m_vip[i])->m_virtual_ip == pMO->m_virtual_ip)
			{
				found = true;
				break;
			}
		}

		if (found == false)
		{
			m_vip.push_back(ipaddr);
			return true;
		}

		return false;
	}

	bool VRRPESessionConfigLocalManagedObject::deleteVirtualIPMO(IpV4Address ipaddr)
	{
		unsigned int i = 0, index = 0;
		bool found = false;

		for (; i < m_vip.size(); i++)
		{
			if ((m_vip[i])->m_virtual_ip == ipaddr)
			{
				found = true;
				index = i;
				break;
			}
		}

		if (found == true)
		{
			m_vip.erase(m_vip.begin() + index);
			return true;
		}

		return false;
	}

	bool VRRPESessionConfigLocalManagedObject::deleteVirtualIPMO(const string& ipaddr)
	{
    	size_t sep = ipaddr.find('/');
    	if (sep != string::npos) {
    		string ip_str(ipaddr, 0, sep);
    		string len_str(ipaddr, sep+1);
    		UI32 ip = ntohl(inet_addr(ip_str.c_str()));
    		UI32 prefix_len = atoi(len_str.c_str());

    		unsigned int i = 0;
    		bool found = false;

    		for (; i < m_vip.size(); i++)
    		{
    			UI32 tmp_ip = ntohl(inet_addr((m_vip[i])->m_virtual_ip.toString().c_str()));
    			if (ip >> (32 - prefix_len) == tmp_ip  >> (32- prefix_len))
    			{
					m_vip.erase(m_vip.begin() + i);
    				found = true;
					i --;
    			}
    		}

    		if (found == true)
    		{
    			return true;
    		}
    	}
    	return false;
	}

	bool VRRPESessionConfigLocalManagedObject::deleteVirtualIPAll()
	{
		unsigned int i = 0;
		unsigned size = m_vip.size();

		for (; i < size; i++)
		{
			m_vip.erase(m_vip.begin() + i);
		}

		if (size) return true;
		return false;
	}

	UI32 VRRPESessionConfigLocalManagedObject::getVirtualIPListSize()
	{
		return m_vip.size();
	}

    void  VRRPESessionConfigLocalManagedObject::setTrack(const vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > &track)
    {
        m_track  =  track;
    }

    vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> >  VRRPESessionConfigLocalManagedObject::getTrack() const
    {
        return (m_track);
    }

	WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> VRRPESessionConfigLocalManagedObject::searchTrackPort(string track_ifname)
	{
		unsigned int i = 0;

		for (; i < m_track.size(); i++)
		{
			if ((m_track[i])->m_track_ifname == track_ifname)
				return m_track[i];
		}

		return NULL;
	}

	bool VRRPESessionConfigLocalManagedObject::addTrackPortMO(VRRPTrackPortLocalManagedObject *pMO)
	{
		unsigned int i= 0;
		bool found = false;
		WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> trackPort(pMO);

		for (; i < m_track.size(); i++)
		{
			if ((m_track[i])->m_track_ifname == pMO->m_track_ifname)
			{
				found = true;
				break;
			}
		}

		if (found == false)
		{
			m_track.push_back(trackPort);
			return true;
		}

		return false;
	}

	bool VRRPESessionConfigLocalManagedObject::deleteTrackPortMO(string track_ifname)
	{
		unsigned int i = 0, index = 0;
		bool found = false;

		for (; i < m_track.size(); i++)
		{
			if ((m_track[i])->m_track_ifname == track_ifname)
			{
				found = true;
				index = i;
				break;
			}
		}

		if (found == true)
		{
			m_track.erase(m_track.begin() + index);
			return true;
		}

		return false;
	}

	void VRRPESessionConfigLocalManagedObject::deleteTrackPortAll()
	{
		unsigned int i = 0;
		unsigned int size = m_track.size();

		for (; i < size; i ++)
		{
			m_track.erase(m_track.begin() + i);
		}
	}

    void  VRRPESessionConfigLocalManagedObject::setAdvt_int(const UI32 &advt_int)
    {
        m_advt_int  =  advt_int;
    }

    UI32  VRRPESessionConfigLocalManagedObject::getAdvt_int() const
    {
        return (m_advt_int);
    }

    void  VRRPESessionConfigLocalManagedObject::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  VRRPESessionConfigLocalManagedObject::getPrio() const
    {
        return (m_prio);
    }

    void  VRRPESessionConfigLocalManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  VRRPESessionConfigLocalManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  VRRPESessionConfigLocalManagedObject::setPreempt(const bool &preempt)
    {
        m_preempt  =  preempt;
    }

    bool  VRRPESessionConfigLocalManagedObject::getPreempt() const
    {
        return (m_preempt);
    }

    void  VRRPESessionConfigLocalManagedObject::setHold_time(const UI32 &hold_time)
    {
        m_hold_time  =  hold_time;
    }

    UI32  VRRPESessionConfigLocalManagedObject::getHold_time() const
    {
        return (m_hold_time);
    }

    void  VRRPESessionConfigLocalManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  VRRPESessionConfigLocalManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  VRRPESessionConfigLocalManagedObject::setAdvt_backup(const bool &advt_backup)
    {
        m_advt_backup  =  advt_backup;
    }

    bool  VRRPESessionConfigLocalManagedObject::getAdvt_backup() const
    {
        return (m_advt_backup);
    }

    void  VRRPESessionConfigLocalManagedObject::setGarp_timer(const UI32 &garp_timer)
    {
        m_garp_timer  =  garp_timer;
    }

    UI32  VRRPESessionConfigLocalManagedObject::getGarp_timer() const
    {
        return (m_garp_timer);
    }

    void  VRRPESessionConfigLocalManagedObject::setBackup_advt_int(const UI32 &backup_advt_int)
    {
        m_backup_advt_int  =  backup_advt_int;
    }

    UI32  VRRPESessionConfigLocalManagedObject::getBackup_advt_int() const
    {
        return (m_backup_advt_int);
    }

    void  VRRPESessionConfigLocalManagedObject::setSpf(const bool &spf)
    {
        m_spf  =  spf;
    }

    bool  VRRPESessionConfigLocalManagedObject::getSpf() const
    {
        return (m_spf);
    }

    void  VRRPESessionConfigLocalManagedObject::setRevPrio(const UI8 &rev_prio)
    {
        m_rev_prio  =  rev_prio;
    }

    UI8  VRRPESessionConfigLocalManagedObject::getRevPrio() const
    {
        return (m_rev_prio);
    }

    void  VRRPESessionConfigLocalManagedObject::setifType(const InterfaceType &iftype)
    {
        m_iftype  =  iftype;
    }

    InterfaceType  VRRPESessionConfigLocalManagedObject::getifType() const
    {
        return (m_iftype);
    }

    void  VRRPESessionConfigLocalManagedObject::setifName(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  VRRPESessionConfigLocalManagedObject::getifName() const
    {
        return (m_ifname);
    }

}
