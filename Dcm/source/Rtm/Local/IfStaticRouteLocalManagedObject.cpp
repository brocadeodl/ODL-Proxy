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

#include "Rtm/Local/IfStaticRouteLocalManagedObject.h"
#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "vcs.h"

namespace DcmNs
{

    IfStaticRouteLocalManagedObject::IfStaticRouteLocalManagedObject (RtmLocalObjectManager *pRtmLocalObjectManager)
        : PrismElement  (pRtmLocalObjectManager),
        PrismPersistableObject (IfStaticRouteLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pRtmLocalObjectManager),
        DcmManagedObject (pRtmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pRtmLocalObjectManager)
    {
    }

    IfStaticRouteLocalManagedObject::~IfStaticRouteLocalManagedObject ()
    {
    }

    string  IfStaticRouteLocalManagedObject::getClassName()
    {
        return ("IfStaticRouteLocalManagedObject");
    }

    void  IfStaticRouteLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_dest));
        addPersistableAttribute (new AttributeEnum(&m_ifType,"ifType",rtm_static_route_oif_type));
        addPersistableAttribute (new AttributeString(&m_ifName,"ifName",rtm_static_route_oif_name));
        addPersistableAttribute (new AttributeUI32UC(&m_cost,false,false,0,"cost",rtm_metric));
        addPersistableAttribute (new AttributeUI32UC(&m_distance,false,false,0,"distance",rtm_distance));
        addPersistableAttribute (new AttributeUI32UC(&m_tag,false,false,0,"tag",rtm_tag));
    }

    void  IfStaticRouteLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeIpV4AddressNetworkMask(&m_dest,"dest",rtm_static_route_dest));
        addPersistableAttributeForCreate  (new AttributeEnum(&m_ifType,"ifType",rtm_static_route_oif_type));
        addPersistableAttributeForCreate  (new AttributeString(&m_ifName,"ifName",rtm_static_route_oif_name));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_cost,false,false,0,"cost",rtm_metric));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_distance,false,false,0,"distance",rtm_distance));
        addPersistableAttributeForCreate  (new AttributeUI32UC(&m_tag,false,false,0,"tag",rtm_tag));
    }

    void  IfStaticRouteLocalManagedObject::setupKeys()
    {
        vector<string> keyName;
        keyName.push_back("dest");
        keyName.push_back("ifType");
        keyName.push_back("ifName");
        setUserDefinedKeyCombination (keyName);
    }

    void  IfStaticRouteLocalManagedObject::setDest(const IpV4AddressNetworkMask &dest)
    {
        m_dest = dest;
    }

    IpV4AddressNetworkMask  IfStaticRouteLocalManagedObject::getDest() const
    {
        return (m_dest);
    }

    void  IfStaticRouteLocalManagedObject::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  IfStaticRouteLocalManagedObject::getIfType() const
    {
        return (m_ifType);
    }

    void  IfStaticRouteLocalManagedObject::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  IfStaticRouteLocalManagedObject::getIfName() const
    {
        return (m_ifName);
    }

    void  IfStaticRouteLocalManagedObject::setCost(const UI32 &cost, bool userConfig)
    {
        m_cost.setIsUserConfigured(userConfig);
        m_cost.setUI32Value(cost);
    }

    UI32UC  IfStaticRouteLocalManagedObject::getCost() const
    {
        return (m_cost);
    }

    void  IfStaticRouteLocalManagedObject::setDistance(const UI32 &distance, bool userConfig)
    {
        m_distance.setIsUserConfigured(userConfig);
        m_distance.setUI32Value(distance);
    }

    UI32UC  IfStaticRouteLocalManagedObject::getDistance() const
    {
        return (m_distance);
    }

    void  IfStaticRouteLocalManagedObject::setTag(const UI32 &tag, bool userConfig)
    {
        m_tag.setIsUserConfigured(userConfig);
        m_tag.setUI32Value(tag);
    }

    UI32UC  IfStaticRouteLocalManagedObject::getTag() const
    {
        return (m_tag);
    }

}
