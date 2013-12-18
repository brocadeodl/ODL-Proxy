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

#include "ClientInterface/Fcoe/FcoeClientDebugMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientDebugMessage::FcoeClientDebugMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_DEBUG)
    {
    }

    FcoeClientDebugMessage::FcoeClientDebugMessage (const UI32 &oper_bmp,
													const UI32 &send_type,
													const UI32 &service_type)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_DEBUG),
        m_oper_bmp    (oper_bmp),
		m_send_type   (send_type),
		m_service_type(service_type)
    {
    }

    FcoeClientDebugMessage::~FcoeClientDebugMessage ()
    {
    }

    void  FcoeClientDebugMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_send_type,"send_type"));
        addSerializableAttribute (new AttributeUI32(&m_service_type,"service_type"));
    }

    void  FcoeClientDebugMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientDebugMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientDebugMessage::setSend_type(const UI32 &send_type)
    {
        m_send_type  =  send_type;
    }

    UI32  FcoeClientDebugMessage::getSend_type() const
    {
        return (m_send_type);
    }

    void  FcoeClientDebugMessage::setServ_type(const UI32 &service_type)
    {
        m_service_type  =  service_type;
    }

    UI32  FcoeClientDebugMessage::getServ_type() const
    {
        return (m_service_type);
    }

    const void * FcoeClientDebugMessage::getCStructureForInputs()
    {
        fcoe_debug_msg_t *msg_p = new fcoe_debug_msg_t;

        msg_p->oper_bmp = m_oper_bmp;
		msg_p->send_type = m_send_type;
		msg_p->service_type = m_service_type;
        
        return msg_p;
    }
}
