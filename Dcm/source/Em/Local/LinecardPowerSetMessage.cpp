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
 *   Author : hungn                                                     *
 **************************************************************************/

#include "Em/Local/LinecardLocalObjectManager.h"
#include "Em/Local/LinecardPowerSetMessage.h"
#include "Em/Local/LinecardTypes.h"

namespace DcmNs
{

    LinecardPowerSetMessage::LinecardPowerSetMessage ()
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), LINECARDPOWERSET)
    {
    }

    LinecardPowerSetMessage::LinecardPowerSetMessage (const UI32 &slotnum,const UI32 &mode,const SI32 &result)
        : PrismMessage (LinecardLocalObjectManager::getPrismServiceId (), LINECARDPOWERSET),
        m_slotnum    (slotnum),
        m_mode    (mode),
        m_result    (result)
    {
		m_slotnum = slotnum;
		m_mode = mode;
		m_result = result;
    }

    LinecardPowerSetMessage::~LinecardPowerSetMessage ()
    {
    }

    void  LinecardPowerSetMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_slotnum,"slotnum"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeSI32(&m_result,"result"));
    }

    void  LinecardPowerSetMessage::setSlotnum(const UI32 &slotnum)
    {
        m_slotnum  =  slotnum;
    }

    UI32  LinecardPowerSetMessage::getSlotnum() const
    {
        return (m_slotnum);
    }

    void  LinecardPowerSetMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  LinecardPowerSetMessage::getMode() const
    {
        return (m_mode);
    }

    void  LinecardPowerSetMessage::setResult(const SI32 &result)
    {
        m_result  =  result;
    }

    SI32  LinecardPowerSetMessage::getResult() const
    {
        return (m_result);
    }

}
