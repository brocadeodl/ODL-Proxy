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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Igmps/Local/IgmpsLocalObjectManager.h"
#include "Igmps/Local/IgmpsUpdateIgmpsConfigMessage.h"
#include "Igmps/Local/IgmpsTypes.h"

namespace DcmNs
{

    IgmpsUpdateIgmpsConfigMessage::IgmpsUpdateIgmpsConfigMessage ()
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (),IGMPSUPDATEIGMPSCONFIG),
        m_enable    (false),
        m_restrictUnknownMcast (false),
	m_opCode ( 0)
    {
    }

    IgmpsUpdateIgmpsConfigMessage::IgmpsUpdateIgmpsConfigMessage (const bool &enable)
        : PrismMessage (IgmpsLocalObjectManager::getPrismServiceId (),IGMPSUPDATEIGMPSCONFIG),
        m_enable    (enable),
        m_restrictUnknownMcast (false),
	m_opCode (0)
    {
    }

    IgmpsUpdateIgmpsConfigMessage::~IgmpsUpdateIgmpsConfigMessage ()
    {
    }

    void  IgmpsUpdateIgmpsConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_restrictUnknownMcast,"restrict-unknown-mcast"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opcode"));

    }

    void  IgmpsUpdateIgmpsConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsUpdateIgmpsConfigMessage::getEnable() const
    {
        return (m_enable);
    }
     void  IgmpsUpdateIgmpsConfigMessage::setRestrictUnknownMcast(const bool &restrictUnknownMcast)
    {
        m_restrictUnknownMcast  = restrictUnknownMcast;
    }

    bool  IgmpsUpdateIgmpsConfigMessage::getRestrictUnknownMcast() const
    {
        return (m_restrictUnknownMcast);
    }

    void  IgmpsUpdateIgmpsConfigMessage::setOpCode(const UI32 &opCode)
        {
                m_opCode  =  opCode;
        }

        UI32  IgmpsUpdateIgmpsConfigMessage::getOpCode() const
        {
                return (m_opCode);
        }

}
