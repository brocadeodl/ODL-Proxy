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

#include "Rtm/Local/RtmLocalObjectManager.h"
#include "Rtm/Local/RtmUpdateStaticRouteMessage.h"
#include "Rtm/Local/RtmTypes.h"

namespace DcmNs
{

    RtmUpdateStaticRouteMessage::RtmUpdateStaticRouteMessage ()
        : ManagementInterfaceMessage (RtmLocalObjectManager::getClassName (),RTMUPDATESTATICROUTE)
    {
		m_ifType = 0;
        m_vrf = "";
    }

/*    RtmUpdateStaticRouteMessage::RtmUpdateStaticRouteMessage (const IpV4AddressNetworkMask &dest,const IpV4Address &nhIp,const UI32 &ifType,const string &ifName,const UI32UC &cost,const UI32UC &distance,const UI32UC &tag,const UI32 &opCode)
        : ManagementInterfaceMessage (RtmLocalObjectManager::getClassName (),RTMUPDATESTATICROUTE),
        m_dest    (dest),
        m_nhIp    (nhIp),
        m_ifType    (ifType),
        m_ifName    (ifName),
        m_cost    (cost),
        m_distance    (distance),
        m_tag    (tag),
        m_opCode    (opCode)
    {
    }*/

    RtmUpdateStaticRouteMessage::~RtmUpdateStaticRouteMessage ()
    {
    }

    void  RtmUpdateStaticRouteMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpV4AddressNetworkMask(&m_dest,"dest"));
        addSerializableAttribute (new AttributeIpV4Address(&m_nhIp,"nhIp"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"ifType"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32UC(&m_cost,     (const bool&)false, (const bool&)false,(const WaveNs::UI32&)0, (const std::string&)"cost"));
        addSerializableAttribute (new AttributeUI32UC(&m_distance, (const bool&)false, (const bool&)false,(const WaveNs::UI32&)0, (const std::string&)"distance"));
        addSerializableAttribute (new AttributeUI32UC(&m_tag,      (const bool&)false, (const bool&)false,(const WaveNs::UI32&)0, (const std::string&)"tag"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_vrf,"vrf"));
    }

    void RtmUpdateStaticRouteMessage::setNextHopVrf(string& vrf)
    {
         m_vrf = vrf;
    }

    string RtmUpdateStaticRouteMessage::getNextHopVrf() const 
    {
        return m_vrf;
    }

    void  RtmUpdateStaticRouteMessage::setDest(const IpV4AddressNetworkMask &dest)
    {
        m_dest  =  dest;
    }

    IpV4AddressNetworkMask  RtmUpdateStaticRouteMessage::getDest() const
    {
        return (m_dest);
    }

    void  RtmUpdateStaticRouteMessage::setNhIp(const IpV4Address &nhIp)
    {
        m_nhIp  =  nhIp;
    }

    IpV4Address  RtmUpdateStaticRouteMessage::getNhIp() const
    {
        return (m_nhIp);
    }

    void  RtmUpdateStaticRouteMessage::setIfType(const UI32 &ifType)
    {
        m_ifType  =  ifType;
    }

    UI32  RtmUpdateStaticRouteMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  RtmUpdateStaticRouteMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  RtmUpdateStaticRouteMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  RtmUpdateStaticRouteMessage::setCost(const UI32 &cost)
    {
		m_cost.setIsUserConfigured(true);
        m_cost.setUI32Value(cost);
    }

    UI32UC  RtmUpdateStaticRouteMessage::getCost() const
    {
        return (m_cost);
    }

    void  RtmUpdateStaticRouteMessage::setDistance(const UI32 &distance)
    {
		m_distance.setIsUserConfigured(true);
        m_distance.setUI32Value(distance);
    }

    UI32UC  RtmUpdateStaticRouteMessage::getDistance() const
    {
        return (m_distance);
    }

    void  RtmUpdateStaticRouteMessage::setTag(const UI32 &tag)
    {
		m_tag.setIsUserConfigured(true);
        m_tag.setUI32Value(tag);
    }

    UI32UC  RtmUpdateStaticRouteMessage::getTag() const
    {
        return (m_tag);
    }

    void  RtmUpdateStaticRouteMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  RtmUpdateStaticRouteMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
