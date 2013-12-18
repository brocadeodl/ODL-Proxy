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
#include "Fcoe/Local/FcoeLocalFcoeFipMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeFipMessage::FcoeLocalFcoeFipMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FIP)
    {
    }

    FcoeLocalFcoeFipMessage::FcoeLocalFcoeFipMessage (const UI32 &oper_bmp,const UI32 &fka_intvl,const bool &timeout)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FIP),
        m_oper_bmp    (oper_bmp),
        m_fka_intvl    (fka_intvl),
        m_timeout    (timeout)
    {
    }

    FcoeLocalFcoeFipMessage::~FcoeLocalFcoeFipMessage ()
    {
    }

    void  FcoeLocalFcoeFipMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_fka_intvl,"fka_intvl"));
        addSerializableAttribute (new AttributeBool(&m_timeout,"timeout"));
    }

    void  FcoeLocalFcoeFipMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalFcoeFipMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeLocalFcoeFipMessage::setFka_intvl(const UI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    UI32  FcoeLocalFcoeFipMessage::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeLocalFcoeFipMessage::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeLocalFcoeFipMessage::getTimeout() const
    {
        return (m_timeout);
    }

}
