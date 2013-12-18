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
 *   Author : MMREDDY                                                      *
 **************************************************************************/
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalEnodeMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{

    FcoeLocalEnodeMessage::FcoeLocalEnodeMessage ()
        : ManagementInterfaceMessage (FcoeLocalObjectManager::getClassName (),FCOE_LOCAL_FCOE_ENODE)
    {
    }

    FcoeLocalEnodeMessage::FcoeLocalEnodeMessage (const UI32 &oper_bmp, const uint32_t &fcoe_enodes)
        : ManagementInterfaceMessage (FcoeLocalObjectManager::getClassName (),FCOE_LOCAL_FCOE_ENODE),
        m_oper_bmp (oper_bmp),
        m_fcoe_enodes (fcoe_enodes)
    {
    }

    FcoeLocalEnodeMessage::~FcoeLocalEnodeMessage ()
    {
    }

    void  FcoeLocalEnodeMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp, "oper_bmp"));
        addSerializableAttribute (new AttributeUI32(&m_fcoe_enodes, "fcoe_enodes"));
    }


    void FcoeLocalEnodeMessage::setFcoeEnodes(const uint32_t &fcoe_enodes)
    {
        m_fcoe_enodes = fcoe_enodes;
    }

    uint32_t FcoeLocalEnodeMessage::getFcoeEnodes() const
    {
        return m_fcoe_enodes;
    }

    UI32 FcoeLocalEnodeMessage::getFcoeEnodesOperBmp()
    {
       return m_oper_bmp;
    }
        
    void FcoeLocalEnodeMessage::setFcoeEnodesOperBmp(const UI32 &oper_bmp)
    {
        m_oper_bmp = oper_bmp;
    }
}

