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

#include "Rtm/Local/NhStaticRouteLocalManagedObject.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"
#include "vcs.h"

namespace DcmNs
{

    NhStaticRouteLocalManagedObject::NhStaticRouteLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (NhStaticRouteLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager)
    {
    }

    NhStaticRouteLocalManagedObject::~NhStaticRouteLocalManagedObject ()
    {
    }

    string  NhStaticRouteLocalManagedObject::getClassName()
    {
        return ("NhStaticRouteLocalManagedObject");
    }

    void NhStaticRouteLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("dest");
        keyName.push_back("nhIp");
        setUserDefinedKeyCombination (keyName);
    }

    void  NhStaticRouteLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_dest));
        addPersistableAttribute (new AttributeIpV4Address(&m_nhIp,"nhIp",rtm_static_route_nh));
        addPersistableAttribute (new AttributeUI32UC(&m_cost,false,false,0,"cost",rtm_metric));
        addPersistableAttribute (new AttributeUI32UC(&m_distance,false,false,0,"distance",rtm_distance));
        addPersistableAttribute (new AttributeUI32UC(&m_tag,false,false,0,"tag",rtm_tag));
    }

    void  NhStaticRouteLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_dest));
        addPersistableAttributeForCreate  (new AttributeIpV4Address(&m_nhIp,"nhIp",rtm_static_route_nh));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_cost,false,false,0,"cost",rtm_metric));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_distance,false,false,0,"distance",rtm_distance));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_tag,false,false,0,"tag",rtm_tag));
    }

    void  NhStaticRouteLocalManagedObject::setDest(const IpV4AddressNetworkMask &dest)
    {
        m_dest  =  dest;
    }

    IpV4AddressNetworkMask  NhStaticRouteLocalManagedObject::getDest() const
    {
        return (m_dest);
    }

    void  NhStaticRouteLocalManagedObject::setNhIp(const IpV4Address &nhIp)
    {
        m_nhIp  =  nhIp;
    }

    IpV4Address  NhStaticRouteLocalManagedObject::getNhIp() const
    {
        return (m_nhIp);
    }

    void  NhStaticRouteLocalManagedObject::setCost(const UI32 &cost, bool userConfig)
    {
		m_cost.setIsUserConfigured(userConfig);
		m_cost.setUI32Value(cost);
    }

    UI32UC  NhStaticRouteLocalManagedObject::getCost() const
    {
        return (m_cost);
    }

    void  NhStaticRouteLocalManagedObject::setDistance(const UI32 &distance, bool userConfig)
    {
        m_distance.setIsUserConfigured(userConfig);
        m_distance.setUI32Value(distance);
    }

    UI32UC  NhStaticRouteLocalManagedObject::getDistance() const
    {
        return (m_distance);
    }

    void  NhStaticRouteLocalManagedObject::setTag(const UI32 &tag, bool userConfig)
    {
        m_tag.setIsUserConfigured(userConfig);
        m_tag.setUI32Value(tag);
    }

    UI32UC  NhStaticRouteLocalManagedObject::getTag() const
    {
        return (m_tag);
    }

}
