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
#include "Fcoe/Local/FcoeLocalFcoeMapMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeMapMessage::FcoeLocalFcoeMapMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_MAP)
    {
    }

    FcoeLocalFcoeMapMessage::FcoeLocalFcoeMapMessage (const UI32 &oper_bmp,const string &map_name,const string &fabric_map_name,const string &cee_map_name)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_MAP),
        m_oper_bmp    (oper_bmp),
        m_map_name    (map_name),
        m_fabric_map_name    (fabric_map_name),
        m_cee_map_name    (cee_map_name)
    {
    }

    FcoeLocalFcoeMapMessage::~FcoeLocalFcoeMapMessage ()
    {
    }

    void  FcoeLocalFcoeMapMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addSerializableAttribute (new AttributeString(&m_cee_map_name,"cee_map_name"));
    }

    void  FcoeLocalFcoeMapMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalFcoeMapMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeLocalFcoeMapMessage::setMap_name(const string &map_name)
    {
        m_map_name  =  map_name;
    }

    string  FcoeLocalFcoeMapMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeLocalFcoeMapMessage::setFabric_map_name(const string &fabric_map_name)
    {
        m_fabric_map_name  =  fabric_map_name;
    }

    string  FcoeLocalFcoeMapMessage::getFabric_map_name() const
    {
        return (m_fabric_map_name);
    }

    void  FcoeLocalFcoeMapMessage::setCee_map_name(const string &cee_map_name)
    {
        m_cee_map_name  =  cee_map_name;
    }

    string  FcoeLocalFcoeMapMessage::getCee_map_name() const
    {
        return (m_cee_map_name);
    }

}
