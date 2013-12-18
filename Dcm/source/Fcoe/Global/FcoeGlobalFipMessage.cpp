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

#include "Fcoe/Global/FcoeGlobalFipMessage.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

namespace DcmNs
{

    FcoeGlobalFipMessage::FcoeGlobalFipMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_FIP)
    {
    }

    FcoeGlobalFipMessage::FcoeGlobalFipMessage (const uint32_t &oper_bmp, const SI32 &fka_intvl, const bool &timeout)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),FCOE_GLOBAL_FCOE_FIP),
        m_oper_bmp (oper_bmp),
        m_fka_intvl    (fka_intvl),
        m_timeout (timeout)
    {
    }

    FcoeGlobalFipMessage::~FcoeGlobalFipMessage ()
    {
    }

    void  FcoeGlobalFipMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeSI32(&m_fka_intvl,"fka_intvl"));
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp, "oper_bmp"));
        addSerializableAttribute (new AttributeBool(&m_timeout, "timeout"));
    }

    void  FcoeGlobalFipMessage::setFka_intvl(const SI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    SI32  FcoeGlobalFipMessage::getFka_intvl() const
    {
        return (m_fka_intvl);
    }
    
    void FcoeGlobalFipMessage::setOperBmp(const uint32_t &oper_bmp)
    {
        m_oper_bmp = oper_bmp;
    }
    uint32_t FcoeGlobalFipMessage::getOperBmp() const
    {
        return m_oper_bmp;
    }
    void FcoeGlobalFipMessage::setTimeout(const bool &timeout)
    {
        m_timeout = timeout;
    }
    bool FcoeGlobalFipMessage::getTimeout() const
    {
        return m_timeout;
    }
}
