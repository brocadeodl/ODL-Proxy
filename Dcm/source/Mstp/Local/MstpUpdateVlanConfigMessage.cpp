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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Mstp/Local/MstpUpdateVlanConfigMessage.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpTypes.h"

namespace DcmNs
{

    MstpUpdateVlanConfigMessage::MstpUpdateVlanConfigMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEVLANCONFIG)
    {
      m_id=0;
      m_cmdCode=0;
    }

    MstpUpdateVlanConfigMessage::MstpUpdateVlanConfigMessage (const UI32 &cmdCode, const UI32 &id)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEVLANCONFIG),
        m_cmdCode(cmdCode), m_id(id)
    {
    }

    MstpUpdateVlanConfigMessage::~MstpUpdateVlanConfigMessage ()
    {
    }

    void  MstpUpdateVlanConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_id,"id"));
    }

    void  MstpUpdateVlanConfigMessage::setId(const UI32 &vid)
    {
        m_id  =  vid;
    }

    UI32  MstpUpdateVlanConfigMessage::getId() const
    {
        return (m_id);
    }

    void  MstpUpdateVlanConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpUpdateVlanConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

}
