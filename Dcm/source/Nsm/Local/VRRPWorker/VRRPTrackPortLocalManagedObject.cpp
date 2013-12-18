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

#include "Nsm/Local/VRRPWorker/VRRPTrackPortLocalManagedObject.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    VRRPTrackPortLocalManagedObject::VRRPTrackPortLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPTrackPortLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager)
    {
    }

    VRRPTrackPortLocalManagedObject::VRRPTrackPortLocalManagedObject (NsmLocalObjectManager *pNsmLocalObjectManager,const InterfaceType &track_iftype,const string &track_ifname,const UI8 &track_prio)
        : PrismElement  (pNsmLocalObjectManager),
        PrismPersistableObject (VRRPTrackPortLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pNsmLocalObjectManager),
        DcmManagedObject (pNsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pNsmLocalObjectManager),
        m_track_iftype    (track_iftype),
        m_track_ifname    (track_ifname),
        m_track_prio    (track_prio)
    {
    }

    VRRPTrackPortLocalManagedObject::~VRRPTrackPortLocalManagedObject ()
    {
    }

    string  VRRPTrackPortLocalManagedObject::getClassName()
    {
        return ("VRRPTrackPortLocalManagedObject");
    }

    void  VRRPTrackPortLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeEnum((UI32*)&m_track_iftype,0,"track_iftype", vrrp_interface_type));
        addPersistableAttribute (new AttributeString(&m_track_ifname,"track_ifname", vrrp_interface_name));
        addPersistableAttribute (new AttributeUI8(&m_track_prio,"track_prio", vrrp_track_priority));
    }

    void  VRRPTrackPortLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeEnum((UI32*)&m_track_iftype,0,"track_iftype", vrrp_interface_type));
        addPersistableAttributeForCreate  (new AttributeString(&m_track_ifname,"track_ifname", vrrp_interface_name));
        addPersistableAttributeForCreate  (new AttributeUI8(&m_track_prio,"track_prio", vrrp_track_priority));
    }

	void VRRPTrackPortLocalManagedObject::setupKeys()
	{
        vector<string> keyName;
		keyName.push_back ("track_iftype");
		keyName.push_back ("track_ifname");
		setUserDefinedKeyCombination (keyName);
	}

    void  VRRPTrackPortLocalManagedObject::setTrack_iftype(const InterfaceType &track_iftype)
    {
        m_track_iftype  =  track_iftype;
    }

    InterfaceType VRRPTrackPortLocalManagedObject::getTrack_iftype() const
    {
        return (m_track_iftype);
    }

    void  VRRPTrackPortLocalManagedObject::setTrack_ifname(const string &track_ifname)
    {
        m_track_ifname  =  track_ifname;
    }

    string  VRRPTrackPortLocalManagedObject::getTrack_ifname() const
    {
        return (m_track_ifname);
    }

    void  VRRPTrackPortLocalManagedObject::setTrack_prio(const UI8 &track_prio)
    {
        m_track_prio  =  track_prio;
    }

    UI8  VRRPTrackPortLocalManagedObject::getTrack_prio() const
    {
        return (m_track_prio);
    }

}
