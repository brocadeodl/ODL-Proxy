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

#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/FcoeWorker/FcoeGlobalFcoeportAttrMessage.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    FcoeGlobalFcoeportAttrMessage::FcoeGlobalFcoeportAttrMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),
                                      NSM_FCOE_WORKER_FCOEPORT_ATTR)
    {
		m_oper_bmp = 0;
    }

    FcoeGlobalFcoeportAttrMessage::FcoeGlobalFcoeportAttrMessage (const UI32 &oper_bmp,const string &if_name, const string &map_name)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSM_FCOE_WORKER_FCOEPORT_ATTR),
        m_oper_bmp    (oper_bmp),
        m_if_id       (if_name),
        m_map_name    (map_name)
    {
    }

    FcoeGlobalFcoeportAttrMessage::FcoeGlobalFcoeportAttrMessage (const UI32 &oper_bmp,const string &if_name, const string &map_name, const UI32 prov_context)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSM_FCOE_WORKER_FCOEPORT_ATTR),
        m_oper_bmp    (oper_bmp),
        m_if_id       (if_name),
        m_map_name    (map_name),
		m_prov_context(prov_context)
    {
    }
    FcoeGlobalFcoeportAttrMessage::~FcoeGlobalFcoeportAttrMessage ()
    {
    }

    void  FcoeGlobalFcoeportAttrMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeString(&m_if_id,"if_id"));
        addSerializableAttribute (new AttributeUI32(&m_prov_context,"prov_context"));
    }

    void  FcoeGlobalFcoeportAttrMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeGlobalFcoeportAttrMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    UI32  FcoeGlobalFcoeportAttrMessage::get_prov_context() const
    {
        return (m_prov_context);
    }
    void  FcoeGlobalFcoeportAttrMessage::setMap_name(const string &map_name)
    {
        m_map_name = map_name;
    }

    string  FcoeGlobalFcoeportAttrMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeGlobalFcoeportAttrMessage::setIfId(const string &if_id)
    {
        m_if_id = if_id;
    }

    string FcoeGlobalFcoeportAttrMessage::getIfId() const
    {
        return m_if_id;
    }
}
