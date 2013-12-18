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
 *   Author : rgandhi                                                     *
 **************************************************************************/

#include "Nsm/Local/NSMLocalReserevedVlanMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmLocalReservedVlanMessage::NsmLocalReservedVlanMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALRESERVEDVLAN)
    {
		m_cmdcode = 0;
		m_start = 0;
		m_end = 0;
    }
    NsmLocalReservedVlanMessage::NsmLocalReservedVlanMessage (const UI32 &cmdcode, 
                              const UI32 &start, 
                              const UI32 &end)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (), NSMLOCALRESERVEDVLAN),
        m_cmdcode    (cmdcode),
        m_start    (start),
        m_end    (end)
    {
    }

    NsmLocalReservedVlanMessage::~NsmLocalReservedVlanMessage ()
    {
    }

    void  NsmLocalReservedVlanMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_start,"start"));
        addSerializableAttribute (new AttributeUI32(&m_end,"end"));
    }

    void  NsmLocalReservedVlanMessage::setStart(const UI32 &start)
    {
        m_start =  start;
    }

    UI32  NsmLocalReservedVlanMessage::getStart() const
    {
        return (m_start);
    }

    void  NsmLocalReservedVlanMessage::setEnd(const UI32 &end)
    {
        m_end =  end;
    }

    UI32  NsmLocalReservedVlanMessage::getEnd() const
    {
        return (m_end);
    }

    void  NsmLocalReservedVlanMessage::setCmdCode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  NsmLocalReservedVlanMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }

}
