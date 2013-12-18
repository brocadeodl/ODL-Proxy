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

#include "ClientInterface/Fcoe/FcoeClientFipMessage.h"
#include "ClientInterface/DceWaveClientMessage.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeClientFipMessage::FcoeClientFipMessage ()
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FIP_CONFIG)
    {
    }

    FcoeClientFipMessage::FcoeClientFipMessage (const UI32 &oper_bmp,const UI32 &fka_intvl,const bool &timeout)
        : DcmManagementInterfaceMessage (FCOE_CLIENT_FIP_CONFIG),
        m_oper_bmp    (oper_bmp),
        m_fka_intvl    (fka_intvl),
        m_timeout    (timeout)
    {
    }

    FcoeClientFipMessage::~FcoeClientFipMessage ()
    {
    }

    void  FcoeClientFipMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_fka_intvl,"fka_intvl"));
        addSerializableAttribute (new AttributeBool(&m_timeout,"timeout"));
    }

    void  FcoeClientFipMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeClientFipMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeClientFipMessage::setFka_intvl(const UI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    UI32  FcoeClientFipMessage::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeClientFipMessage::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeClientFipMessage::getTimeout() const
    {
        return (m_timeout);
    }
    const void * FcoeClientFipMessage::getCStructureForInputs()
    {
        fcoe_fip_msg_t *msg_p = new fcoe_fip_msg_t;

        msg_p->oper_bmp = m_oper_bmp;
        msg_p->fka_intvl = m_fka_intvl;
        msg_p->timeout = m_timeout;
        
        return msg_p;
    }
}
