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
 *   Author : sbalodia                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLocalPolicyMapMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmLocalPolicyMapMessage::NsmLocalPolicyMapMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMLOCALPOLICYMAP)
    {
		m_opCode = 0;
		m_cmdCode = 0;
	}

    NsmLocalPolicyMapMessage::NsmLocalPolicyMapMessage (const string &ifName, const string &mapName, const UI32 &cmdCode, const UI32 &opCode)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(), NSMLOCALPOLICYMAP),
        m_ifName (ifName),
		m_mapName    (mapName),
		 m_cmdCode (cmdCode),
		m_opCode (opCode)
    {
    }

    NsmLocalPolicyMapMessage::~NsmLocalPolicyMapMessage ()
    {
    }

    void  NsmLocalPolicyMapMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeString(&m_ifName, "ifName"));
		addSerializableAttribute (new AttributeString(&m_mapName, "mapName"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
    }

	void NsmLocalPolicyMapMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string NsmLocalPolicyMapMessage::getIfName() const
    {
        return (m_ifName);
    }

	void NsmLocalPolicyMapMessage::setMapName(const string &mapName)
	{
		m_mapName = mapName;
	}

	string NsmLocalPolicyMapMessage::getMapName() const
	{
		return (m_mapName);
	}

    void  NsmLocalPolicyMapMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalPolicyMapMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmLocalPolicyMapMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmLocalPolicyMapMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
