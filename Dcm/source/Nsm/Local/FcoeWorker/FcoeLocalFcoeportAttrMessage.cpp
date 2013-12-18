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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/FcoeWorker/FcoeLocalFcoeportAttrMessage.h"
#include "Nsm/Local/FcoeWorker/NsmFcoeWorkerTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeportAttrMessage::FcoeLocalFcoeportAttrMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),
                        NSM_FCOE_WORKER_FCOEPORT_ATTR_LOCAL)
    {
		m_oper_bmp = 0;
    }

    FcoeLocalFcoeportAttrMessage::FcoeLocalFcoeportAttrMessage (const UI32 &oper_bmp,const string &if_id, const string &map_name)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSM_FCOE_WORKER_FCOEPORT_ATTR_LOCAL),
        m_oper_bmp    (oper_bmp),
        m_if_id       (if_id),
        m_map_name    (map_name)
    {
    }
    FcoeLocalFcoeportAttrMessage::FcoeLocalFcoeportAttrMessage (const UI32 &oper_bmp,const string &if_id, const string &map_name, const UI32 prov_context)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSM_FCOE_WORKER_FCOEPORT_ATTR_LOCAL),
        m_oper_bmp    (oper_bmp),
        m_if_id       (if_id),
        m_map_name    (map_name),
		m_prov_context (prov_context)
    {
    }

    FcoeLocalFcoeportAttrMessage::~FcoeLocalFcoeportAttrMessage ()
    {
    }

    void  FcoeLocalFcoeportAttrMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_if_id, "intf_name"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "ifindex"));
        addSerializableAttribute (new AttributeUI32(&m_prov_context, "prov_context"));
    }

    void  FcoeLocalFcoeportAttrMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalFcoeportAttrMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }
    UI32  FcoeLocalFcoeportAttrMessage::get_prov_context() const
    {
        return (m_prov_context);
    }

    void  FcoeLocalFcoeportAttrMessage::setMap_name(const string &map_name)
    {
        m_map_name = map_name;
    }

    string  FcoeLocalFcoeportAttrMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeLocalFcoeportAttrMessage::setIfId(const string &if_id)
    {
        m_if_id = if_id;
    }

    string FcoeLocalFcoeportAttrMessage::getIfId() const
    {
        return m_if_id;
    }

    void FcoeLocalFcoeportAttrMessage::setIfindex(const UI64 &ifindex)
    {
        m_if_index = ifindex;
    }

    UI64 FcoeLocalFcoeportAttrMessage::getIfindex() const
    {
        return m_if_index;
    }
}
