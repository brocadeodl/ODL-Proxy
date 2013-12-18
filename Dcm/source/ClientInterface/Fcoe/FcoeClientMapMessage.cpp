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

#include "ClientInterface/Fcoe/FcoeClientMapMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientMapMessage::FcoeClientMapMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_MAP_CONFIG)
    {
    }

    FcoeClientMapMessage::FcoeClientMapMessage (const UI32 &oper_bmp,const string &map_name,const string &fabric_map_name,const string &cee_map_name)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_MAP_CONFIG),
        m_oper_bmp    (oper_bmp),
        m_map_name    (map_name),
        m_fabric_map_name    (fabric_map_name),
        m_cee_map_name    (cee_map_name)
    {
    }

    FcoeClientMapMessage::~FcoeClientMapMessage ()
    {
    }

    void  FcoeClientMapMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_map_name,"map_name"));
        addSerializableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addSerializableAttribute (new AttributeString(&m_cee_map_name,"cee_map_name"));
    }

    void  FcoeClientMapMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientMapMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientMapMessage::setMap_name(const string &map_name)
    {
        m_map_name  =  map_name;
    }

    string  FcoeClientMapMessage::getMap_name() const
    {
        return (m_map_name);
    }

    void  FcoeClientMapMessage::setFabric_map_name(const string &fabric_map_name)
    {
        m_fabric_map_name  =  fabric_map_name;
    }

    string  FcoeClientMapMessage::getFabric_map_name() const
    {
        return (m_fabric_map_name);
    }

    void  FcoeClientMapMessage::setCee_map_name(const string &cee_map_name)
    {
        m_cee_map_name  =  cee_map_name;
    }

    string  FcoeClientMapMessage::getCee_map_name() const
    {
        return (m_cee_map_name);
    }
    const void * FcoeClientMapMessage::getCStructureForInputs()
    {
        fcoe_map_msg_t *msg_p = new fcoe_map_msg_t;

        msg_p->oper_bmp = m_oper_bmp;
        strncpy((char *)msg_p->map_name, m_map_name.c_str(),
                FCOE_MAP_NAME_LEN_MAX);

        strncpy((char *)msg_p->fab_map_name, m_fabric_map_name.c_str(),
                FCOE_FAB_MAP_NAME_LEN_MAX);

        strncpy((char *)msg_p->cee_map_name, m_cee_map_name.c_str(),
                FCOE_CEE_MAP_NAME_LEN_MAX);

        return msg_p;
    }
}
