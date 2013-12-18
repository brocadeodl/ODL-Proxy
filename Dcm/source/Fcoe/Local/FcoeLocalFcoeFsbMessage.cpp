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
 *   Copyright (C) 2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jaideep Singh Saluja                                         *
 **************************************************************************/

#include "Fcoe/Local/FcoeLocalFcoeFsbMessage.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{   

    FcoeLocalFcoeFsbMessage::FcoeLocalFcoeFsbMessage ()
		: PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FSB)
    {
    }

    FcoeLocalFcoeFsbMessage::FcoeLocalFcoeFsbMessage (const bool &enable)
		: PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),FCOE_LOCAL_FCOE_FSB),
        m_enable (enable)
    {
    }

    FcoeLocalFcoeFsbMessage::~FcoeLocalFcoeFsbMessage ()
    {
    }

    void  FcoeLocalFcoeFsbMessage::setupAttributesForSerialization()
    {   
		PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable, "enable"));
    }

    void  FcoeLocalFcoeFsbMessage::setMode(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  FcoeLocalFcoeFsbMessage::getMode() const
    {
        return (m_enable);
    }
}

