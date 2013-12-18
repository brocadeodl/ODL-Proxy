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

#include "ClientInterface/Igmps/IgmpsClientUpdateIgmpsConfigMessage.h"
#include "ClientInterface/Igmps/IgmpsMessageDef.h"

namespace DcmNs
{

    IgmpsClientUpdateIgmpsConfigMessage::IgmpsClientUpdateIgmpsConfigMessage ()
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEIGMPSCONFIG),
        m_enable    (false)
    {
    }

    IgmpsClientUpdateIgmpsConfigMessage::IgmpsClientUpdateIgmpsConfigMessage (const bool &enable)
        : DcmManagementInterfaceMessage (IGMPSCLIENTUPDATEIGMPSCONFIG),
        m_enable    (enable)
    {
    }

    IgmpsClientUpdateIgmpsConfigMessage::~IgmpsClientUpdateIgmpsConfigMessage ()
    {
    }

    void  IgmpsClientUpdateIgmpsConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_restrictUnknownMcast,"restrictUnknownMcast"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opcode"));
    }

    void  IgmpsClientUpdateIgmpsConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  IgmpsClientUpdateIgmpsConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void IgmpsClientUpdateIgmpsConfigMessage::setRestrictUnknownMcast(const bool &restrictUnknownMcast)
    {
        m_restrictUnknownMcast  = restrictUnknownMcast;
    }

    bool  IgmpsClientUpdateIgmpsConfigMessage::getRestrictUnknownMcast() const
    {
        return (m_restrictUnknownMcast);
    }

    void  IgmpsClientUpdateIgmpsConfigMessage::setOpCode(const UI32 &opCode)
        {
                m_opCode  =  opCode;
        }

        UI32  IgmpsClientUpdateIgmpsConfigMessage::getOpCode() const
        {
                return (m_opCode);
        }
	const void *IgmpsClientUpdateIgmpsConfigMessage::getCStructureForInputs ()
	{
		igmps_global_config_msg_t *pInput = (igmps_global_config_msg_t *)malloc (sizeof (struct igmps_global_config_msg_));

		pInput->enable = m_enable;
                pInput->opcode = m_opCode;
                pInput->restrict_unknown_mcast = m_restrictUnknownMcast;
		return (pInput);
	}

	void IgmpsClientUpdateIgmpsConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
