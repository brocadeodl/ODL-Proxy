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
#include "Rtm/Local/RtmUpdateRtmConfigMessage.h"
#include "Rtm/Local/RtmTypes.h"

namespace DcmNs
{

    RtmUpdateRtmConfigMessage::RtmUpdateRtmConfigMessage ()
        : ManagementInterfaceMessage (RtmLocalObjectManager::getClassName (),RTMUPDATERTMCONFIG)
    {
    }

/*    RtmUpdateRtmConfigMessage::RtmUpdateRtmConfigMessage (const UI8 &nhProto,const bool &nhDefault,const UI32 &nhRecurr,const UI32 &loadSharing,const IpV4Address &routerID,const UI32 &opCode)
        : ManagementInterfaceMessage (RtmLocalObjectManager::getClassName (),RTMUPDATERTMCONFIG),
        m_nhProto    (nhProto),
        m_nhDefault    (nhDefault),
        m_nhRecurr    (nhRecurr),
        m_loadSharing    (loadSharing),
        m_routerId    (routerID),
        m_opCode    (opCode)
    {
    }
*/
    RtmUpdateRtmConfigMessage::~RtmUpdateRtmConfigMessage ()
    {
    }

    void  RtmUpdateRtmConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_nhProto,"nhProto"));
        addSerializableAttribute (new AttributeBool(&m_nhDefault,"nhDefault"));
        addSerializableAttribute (new AttributeUI32(&m_nhRecur,"nhRecurr"));
        addSerializableAttribute (new AttributeUI32(&m_loadSharing,"loadSharing"));
        addSerializableAttribute (new AttributeIpV4Address(&m_routerID,"routerID"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

    void  RtmUpdateRtmConfigMessage::setNhProto(const UI32 &nhProto)
    {
        m_nhProto  =  nhProto;
    }

    UI32  RtmUpdateRtmConfigMessage::getNhProto() const
    {
        return (m_nhProto);
    }

    void  RtmUpdateRtmConfigMessage::setNhDefault(const bool &nhDefault)
    {
        m_nhDefault  =  nhDefault;
    }

    bool  RtmUpdateRtmConfigMessage::getNhDefault() const
    {
        return (m_nhDefault);
    }

    void  RtmUpdateRtmConfigMessage::setNhRecur(const UI32 &nhRecur)
    {
        m_nhRecur  =  nhRecur;
    }

    UI32  RtmUpdateRtmConfigMessage::getNhRecur() const
    {
        return (m_nhRecur);
    }

    void  RtmUpdateRtmConfigMessage::setLoadSharing(const UI32 &loadSharing)
    {
        m_loadSharing  =  loadSharing;
    }

    UI32  RtmUpdateRtmConfigMessage::getLoadSharing() const
    {
        return (m_loadSharing);
    }

    void  RtmUpdateRtmConfigMessage::setRouterId(const IpV4Address &routerID)
    {
        m_routerID  =  routerID;
    }

    IpV4Address  RtmUpdateRtmConfigMessage::getRouterId() const
    {
        return (m_routerID);
    }

    void  RtmUpdateRtmConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  RtmUpdateRtmConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
