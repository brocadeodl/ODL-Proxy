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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalETIntfMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalETIntfMessage::FcoeLocalETIntfMessage ()
        : ManagementInterfaceMessage (FcoeLocalObjectManager::getClassName (),FCOE_LOCAL_ET_INTF)
    {
    }

    FcoeLocalETIntfMessage::~FcoeLocalETIntfMessage ()
    {
    }

    void  FcoeLocalETIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport_num, "fc_uport_num"));
        addSerializableAttribute (new AttributeBool(&m_status,"status"));
        addSerializableAttribute (new AttributeBool(&m_trunk_status,"trunk_status"));
    }

    void  FcoeLocalETIntfMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalETIntfMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeLocalETIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  FcoeLocalETIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  FcoeLocalETIntfMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  FcoeLocalETIntfMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  FcoeLocalETIntfMessage::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool  FcoeLocalETIntfMessage::getStatus() const
    {
        return (m_status);
    }

    void FcoeLocalETIntfMessage::setFcUportNum (const UI32 &fc_uport_num)
    {
        m_fc_uport_num = fc_uport_num;
    }
    
    UI32 FcoeLocalETIntfMessage::getFcUportNum() const
    {
        return m_fc_uport_num;
    }

    void  FcoeLocalETIntfMessage::setTrunkStatus(const bool &status)
    {
        m_trunk_status  =  status;
    }

    bool  FcoeLocalETIntfMessage::getTrunkStatus() const
    {
        return (m_trunk_status);
    }

}
