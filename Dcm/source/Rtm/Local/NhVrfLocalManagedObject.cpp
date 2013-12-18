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
 *   Author : hzhu                                                     *
 **************************************************************************/

#include "Rtm/Local/NhVrfLocalManagedObject.h"
#include "Vrf/Local/VrfLocalManagedObject.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    NhVrfLocalManagedObject::NhVrfLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (NhVrfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager)
    {
    }

    NhVrfLocalManagedObject::~NhVrfLocalManagedObject ()
    {
    }

    string  NhVrfLocalManagedObject::getClassName()
    {
        return ("NhVrfLocalManagedObject");
    }

    void NhVrfLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("dest");
        keyName.push_back("vrf");
        keyName.push_back("nhIp");
        setUserDefinedKeyCombination (keyName);
    }

    void  NhVrfLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_next_vrf_dest));
        addPersistableAttribute (new AttributeObjectIdAssociation(&m_nextHopVrf,"vrf", 
            VrfLocalManagedObject::getClassName (),true, rtm_next_hop_vrf)); 
        addPersistableAttribute (new AttributeIpV4Address(&m_nhIp,"nhIp",rtm_static_route_next_hop));
        addPersistableAttribute (new AttributeString(&m_currVrf,"currVRf"));
    }

    void  NhVrfLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_next_vrf_dest));
        addPersistableAttributeForCreate  (new AttributeObjectIdAssociation(&m_nextHopVrf,"vrf", 
            VrfLocalManagedObject::getClassName (),true, rtm_next_hop_vrf)); 
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_nhIp,"nhIp",rtm_static_route_next_hop));
        addPersistableAttributeForCreate (new AttributeString(&m_currVrf,"currVRf"));
    }

    void  NhVrfLocalManagedObject::setDest(const IpV4AddressNetworkMask &dest)
    {
        m_dest  =  dest;
    }

    IpV4AddressNetworkMask  NhVrfLocalManagedObject::getDest() const
    {
        return (m_dest);
    }

    void  NhVrfLocalManagedObject::setNhIp(const IpV4Address &nhIp)
    {
        m_nhIp  =  nhIp;
    }

    IpV4Address  NhVrfLocalManagedObject::getNhIp() const
    {
        return (m_nhIp);
    }

    void NhVrfLocalManagedObject::setNextHopVrf(ObjectId& vrf)
    {
        m_nextHopVrf = vrf;
    }

    ObjectId NhVrfLocalManagedObject::getNextHopVrf() const
    {
        return m_nextHopVrf;
    }
}
