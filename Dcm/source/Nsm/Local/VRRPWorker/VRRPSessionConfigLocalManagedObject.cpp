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

#include "Nsm/Local/VRRPWorker/VRRPSessionConfigLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/VRRPWorker/VRRPVirtualIPLocalManagedObject.h"
#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "Framework/Attributes/AttributeManagedObjectVectorComposition.cpp"
#include "Framework/Attributes/AttributeEnum.h"
#include "Nsm/Local/NsmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    VRRPSessionConfigLocalManagedObject::VRRPSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPSessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
		m_owner = false;
		m_vip.clear();
		m_track.clear();
    }

    VRRPSessionConfigLocalManagedObject::VRRPSessionConfigLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const UI8 &vrid,const UI32 &advt_int,const UI8 &prio,const bool &enable,const bool &preempt,const UI32 &hold_time,const string &description,const InterfaceType &iftype,const string &ifname,const bool &owner)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPSessionConfigLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_vrid    (vrid),
        m_advt_int    (advt_int),
        m_prio    (prio),
        m_enable    (enable),
        m_preempt    (preempt),
        m_hold_time    (hold_time),
        m_description    (description),
        m_iftype    (iftype),
        m_ifname    (ifname),
        m_owner     (owner)
    {
		m_vip.clear();
		m_track.clear();
    }

    VRRPSessionConfigLocalManagedObject::~VRRPSessionConfigLocalManagedObject ()
    {
    }

    string  VRRPSessionConfigLocalManagedObject::getClassName()
    {
        return ("VRRPSessionConfigLocalManagedObject");
    }

    void  VRRPSessionConfigLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));

        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttribute (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));

        addPersistableAttribute (new AttributeUI32(&m_advt_int,"advt_int", vrrp_advertisement_interval));
        addPersistableAttribute (new AttributeUI8(&m_prio,"prio", vrrp_priority));
//      addPersistableAttribute (new AttributeBool(&m_enablePresence,"enable", vrrp_enable));
//      addPersistableAttribute (new AttributeBool(&m_enable,"enableflag", vrrp_enable_flag));
        addPersistableAttribute (new AttributeBool(&m_enable,"enable", vrrp_enable));
        addPersistableAttribute (new AttributeBool(&m_preempt,"preempt", vrrp_preempt_mode));
        addPersistableAttribute (new AttributeUI32(&m_hold_time,0,"hold_time", vrrp_hold_time));
        addPersistableAttribute (new AttributeString(&m_description,true,"","description",vrrp_description));
        addPersistableAttribute (new AttributeString(&m_ifname,true,"","ifname"));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_iftype,"iftype"));
        addPersistableAttribute (new AttributeBool(&m_owner,"owner"));
    }

    void  VRRPSessionConfigLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI8(&m_vrid,"vrid", vrrp_vrid));

        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPVirtualIPLocalManagedObject>(&m_vip,"vip", getClassName (), getObjectId (), VRRPVirtualIPLocalManagedObject::getClassName ()));
        addPersistableAttributeForCreate  (new AttributeManagedObjectVectorComposition<VRRPTrackPortLocalManagedObject>(&m_track,"track", getClassName (), getObjectId (), VRRPTrackPortLocalManagedObject::getClassName ()));

        addPersistableAttributeForCreate  (new AttributeUI32(&m_advt_int,"advt_int", vrrp_advertisement_interval));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_prio,"prio", vrrp_priority));
//        addPersistableAttributeForCreate  (new AttributeBool(&m_enablePresence,"enable", vrrp_enable));
//        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enableflag", vrrp_enable_flag));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable", vrrp_enable));
        addPersistableAttributeForCreate  (new AttributeBool(&m_preempt,"preempt", vrrp_preempt_mode));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_hold_time,0,"hold_time", vrrp_hold_time));
        addPersistableAttributeForCreate  (new AttributeString(&m_description,"description", vrrp_description));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifname,"ifname"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_iftype,"iftype"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_owner,"owner"));

        vector<string> keyName;
        keyName.push_back ("vrid");
        setUserDefinedKeyCombination (keyName);
    }

    void  VRRPSessionConfigLocalManagedObject::setVrid(const UI8 &vrid)
    {
        m_vrid  =  vrid;
    }

    UI8  VRRPSessionConfigLocalManagedObject::getVrid() const
    {
        return (m_vrid);
    }

    void  VRRPSessionConfigLocalManagedObject::setVip(const vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> > &vip)
    {
        m_vip  =  vip;
    }

    vector<WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> >  VRRPSessionConfigLocalManagedObject::getVip() const
    {
        return (m_vip);
    }

	WaveManagedObjectPointer<VRRPVirtualIPLocalManagedObject> VRRPSessionConfigLocalManagedObject::searchVirtualIPAddr(IpV4Address ipaddr)
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

	bool VRRPSessionConfigLocalManagedObject::addVirtualIPMO(VRRPVirtualIPLocalManagedObject *pMO)
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

	bool VRRPSessionConfigLocalManagedObject::deleteVirtualIPMO(IpV4Address ipaddr)
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

	bool VRRPSessionConfigLocalManagedObject::deleteVirtualIPMO(const string& ipaddr)
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
    			if (ip >> (32 - prefix_len) == tmp_ip >> (32 - prefix_len))
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

	bool VRRPSessionConfigLocalManagedObject::deleteVirtualIPAll()
	{
		unsigned int i = 0;
		unsigned int size = m_vip.size();

		for (; i < size; i++)
		{
			m_vip.erase(m_vip.begin() + i);
		}
		m_owner = false;
		if (size) return true;
		return false;
	}

	UI32 VRRPSessionConfigLocalManagedObject::getVirtualIPListSize()
	{
		return m_vip.size();
	}

    void  VRRPSessionConfigLocalManagedObject::settrack(const vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> > &track)
    {
        m_track  =  track;
    }

    vector<WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> >  VRRPSessionConfigLocalManagedObject::gettrack() const
    {
        return (m_track);
    }

	WaveManagedObjectPointer<VRRPTrackPortLocalManagedObject> VRRPSessionConfigLocalManagedObject::searchTrackPort(string track_ifname)
	{
		unsigned int i = 0;

		for (; i < m_track.size(); i++)
		{
			if ((m_track[i])->m_track_ifname == track_ifname)
				return m_track[i];
		}

		return NULL;
	}

	bool VRRPSessionConfigLocalManagedObject::addTrackPortMO(VRRPTrackPortLocalManagedObject *pMO)
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

	bool VRRPSessionConfigLocalManagedObject::deleteTrackPortMO(string track_ifname)
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

	void VRRPSessionConfigLocalManagedObject::deleteTrackPortAll()
	{
		unsigned int i = 0;
		unsigned size = m_track.size();

		for (; i < size; i++)
		{
			m_track.erase(m_track.begin() + i);
		}
	}

    void  VRRPSessionConfigLocalManagedObject::setAdvt_int(const UI32 &advt_int)
    {
        m_advt_int  =  advt_int;
    }

    UI32  VRRPSessionConfigLocalManagedObject::getAdvt_int() const
    {
        return (m_advt_int);
    }

    void  VRRPSessionConfigLocalManagedObject::setPrio(const UI8 &prio)
    {
        m_prio  =  prio;
    }

    UI8  VRRPSessionConfigLocalManagedObject::getPrio() const
    {
        return (m_prio);
    }

    void  VRRPSessionConfigLocalManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
/*	
		if (m_enable)
		    m_enablePresence = false;
		else
		    m_enablePresence = true;
*/			
    }

    bool  VRRPSessionConfigLocalManagedObject::getEnable() const
    {
        return (m_enable);
    }

    void  VRRPSessionConfigLocalManagedObject::setPreempt(const bool &preempt)
    {
        m_preempt  =  preempt;
    }

    bool  VRRPSessionConfigLocalManagedObject::getPreempt() const
    {
        return (m_preempt);
    }

    void  VRRPSessionConfigLocalManagedObject::setHold_time(const UI32 &hold_time)
    {
        m_hold_time  =  hold_time;
    }

    UI32  VRRPSessionConfigLocalManagedObject::getHold_time() const
    {
        return (m_hold_time);
    }

    void  VRRPSessionConfigLocalManagedObject::setDescription(const string &description)
    {
        m_description  =  description;
    }

    string  VRRPSessionConfigLocalManagedObject::getDescription() const
    {
        return (m_description);
    }

    void  VRRPSessionConfigLocalManagedObject::setifType(const InterfaceType &iftype)
    {
        m_iftype  =  iftype;
    }

    InterfaceType VRRPSessionConfigLocalManagedObject::getifType() const
    {
        return (m_iftype);
    }

    void  VRRPSessionConfigLocalManagedObject::setifName(const string &ifname)
    {
        m_ifname  =  ifname;
    }

    string  VRRPSessionConfigLocalManagedObject::getifName() const
    {
        return (m_ifname);
    }

    void  VRRPSessionConfigLocalManagedObject::setOwner(const bool &owner)
    {
        m_owner  =  owner;
    }

    bool  VRRPSessionConfigLocalManagedObject::getOwner() const
    {
        return (m_owner);
    }

}
