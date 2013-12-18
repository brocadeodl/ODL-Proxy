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

#include "Fcoe/Global/FcoeGlobalFcoeMapMessage.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

namespace DcmNs
{

    FcoeGlobalFcoeMapMessage::FcoeGlobalFcoeMapMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_MAP)
    {
    }

    FcoeGlobalFcoeMapMessage::FcoeGlobalFcoeMapMessage (const string &name, 
                                                              const uint32_t &oper_bmp, 
                                                              const string &cee_map_name, 
                                                              const string &fabric_map_name)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_MAP),
        m_fcoe_map_name (name),
        m_oper_bmp (oper_bmp),
        m_cee_map_name (cee_map_name),
        m_fabric_map_name (fabric_map_name)
    {
    }

    FcoeGlobalFcoeMapMessage::~FcoeGlobalFcoeMapMessage ()
    {
    }

    void  FcoeGlobalFcoeMapMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_fcoe_map_name,"fcoe_map_name"));
        addSerializableAttribute (new AttributeString(&m_fabric_map_name, "fabric_map_name"));
        addSerializableAttribute (new AttributeString(&m_cee_map_name, "cee_map_name"));
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp, "oper_bmp"));
    }

    void  FcoeGlobalFcoeMapMessage::setName(const string &name)
    {
        m_fcoe_map_name  =  name;
    }

    string  FcoeGlobalFcoeMapMessage::getName() const
    {
        return (m_fcoe_map_name);
    }

    void FcoeGlobalFcoeMapMessage::setOperBmp(const uint32_t &oper_bmp)
    {
        m_oper_bmp = oper_bmp;
    }
    
    uint32_t FcoeGlobalFcoeMapMessage::getOperBmp() const
    {
        return m_oper_bmp;
    }
    
    void FcoeGlobalFcoeMapMessage::setFabricMapName(const string &fabric_map_name)
    {
        m_fabric_map_name = fabric_map_name;
    }
    
    string FcoeGlobalFcoeMapMessage::getFabricMapName() const
    {
        return m_fabric_map_name;
    }
    
    void FcoeGlobalFcoeMapMessage::setCeeMapName(const string &cee_map_name)
    {
        m_cee_map_name = cee_map_name;
    }
    
    string FcoeGlobalFcoeMapMessage::getCeeMapName() const
    {
        return m_cee_map_name;
    }
}
