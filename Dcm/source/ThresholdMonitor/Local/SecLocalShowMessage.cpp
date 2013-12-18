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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Ayush Jaiswal                                                *
 **************************************************************************/

#include "ThresholdMonitor/Local/SecLocalShowMessage.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalObjectManager.h"
#include "ThresholdMonitor/Local/ThresholdMonitorLocalTypes.h"

namespace DcmNs
{

    SecLocalShowMessage::SecLocalShowMessage ()
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECURITYLOCALSHOW)
    {
    }

    SecLocalShowMessage::SecLocalShowMessage (const UI32 areaName)
        : PrismMessage (ThresholdMonitorLocalObjectManager::getPrismServiceId (), SECURITYLOCALSHOW),
		m_areaName (areaName)
    {
    }

    SecLocalShowMessage::~SecLocalShowMessage ()
    {
    }

    void  SecLocalShowMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_areaName,"areaName"));
    }

    void SecLocalShowMessage::setAreaName(const UI32 &areaName)
    {
        m_areaName = areaName;
    }

    UI32 SecLocalShowMessage::getAreaName() const
    {
        return m_areaName;
    }
}
