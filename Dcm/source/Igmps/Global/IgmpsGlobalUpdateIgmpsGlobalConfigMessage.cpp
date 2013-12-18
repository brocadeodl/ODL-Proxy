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

#include "Igmps/Global/IgmpsGlobalUpdateIgmpsGlobalConfigMessage.h"
#include "Igmps/Global/IgmpsGlobalObjectManager.h"
#include "Igmps/Global/IgmpsGlobalTypes.h"

namespace DcmNs
{

    IgmpsGlobalUpdateIgmpsGlobalConfigMessage::IgmpsGlobalUpdateIgmpsGlobalConfigMessage ()
        : ManagementInterfaceMessage (IgmpsGlobalObjectManager::getClassName (),IGMPSGLOBALUPDATEIGMPSGLOBALCONFIG),
        m_enable    (false), m_restrictUnknownMcast (false)
    {
    }

    IgmpsGlobalUpdateIgmpsGlobalConfigMessage::IgmpsGlobalUpdateIgmpsGlobalConfigMessage (const bool &enable, const bool &restrictUnknownMcast)
        : ManagementInterfaceMessage (IgmpsGlobalObjectManager::getClassName (),IGMPSGLOBALUPDATEIGMPSGLOBALCONFIG),
        m_enable    (enable), m_restrictUnknownMcast (false)
    {
    }

    IgmpsGlobalUpdateIgmpsGlobalConfigMessage::~IgmpsGlobalUpdateIgmpsGlobalConfigMessage ()
    {
    }

    void  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_restrictUnknownMcast,"restrictUnknownMcast"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opcode"));

    }

    void  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void IgmpsGlobalUpdateIgmpsGlobalConfigMessage::setRestrictUnknownMcast(const bool &restrictUnknownMcast)
    {
        m_restrictUnknownMcast  = restrictUnknownMcast;
    }

    bool  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::getRestrictUnknownMcast() const
    {
        return (m_restrictUnknownMcast);
    }

    void  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::setOpCode(const UI32 &opCode)
        {
                m_opCode  =  opCode;
        }

        UI32  IgmpsGlobalUpdateIgmpsGlobalConfigMessage::getOpCode() const
        {
                return (m_opCode);
        }



}
