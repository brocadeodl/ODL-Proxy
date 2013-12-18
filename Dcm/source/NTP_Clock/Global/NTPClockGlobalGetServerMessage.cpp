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
 *   Author : noku                                                     *
 **************************************************************************/

#include "Framework/Attributes/AttributeIpVxAddress.h"
#include "NTP_Clock/Global/NTPClockGlobalGetServerMessage.h"
#include "NTP_Clock/Global/NTPClockGlobalObjectManager.h"
#include "NTP_Clock/Global/NTPClockGlobalTypes.h"

namespace DcmNs
{

    NTPClockGlobalGetServerMessage::NTPClockGlobalGetServerMessage ()
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETSERVER)
    {
    }

    NTPClockGlobalGetServerMessage::NTPClockGlobalGetServerMessage (const IpVxAddress &ActiveServer,const UI32 &MappedId)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETSERVER),
        m_ActiveServer    (ActiveServer),
        m_MappedId    (MappedId)
    {
		m_NumVectors = 1;
    }

    NTPClockGlobalGetServerMessage::NTPClockGlobalGetServerMessage (const vector<IpVxAddress> &ActiveServer,const vector<UI32> &MappedId)
        : PrismMessage (NTPClockGlobalObjectManager::getPrismServiceId (), NTPCLOCKGLOBALGETSERVER),
        m_vActiveServer    (ActiveServer),
        m_vMappedId    (MappedId)
    {
		m_NumVectors = 0;
    }

    NTPClockGlobalGetServerMessage::~NTPClockGlobalGetServerMessage ()
    {
    }

    void  NTPClockGlobalGetServerMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeIpVxAddress(&m_ActiveServer,"ActiveServer"));
        addSerializableAttribute (new AttributeUI32(&m_MappedId,"MappedId"));
        addSerializableAttribute (new AttributeUI32(&m_NumVectors,"NumVectors"));
        addSerializableAttribute (new AttributeIpVxAddressVector(&m_vActiveServer,"vActiveServer"));
        addSerializableAttribute (new AttributeUI32Vector(&m_vMappedId,"vMappedId"));
    }

    void  NTPClockGlobalGetServerMessage::setActiveServer(const IpVxAddress &ActiveServer)
    {
        m_ActiveServer  =  ActiveServer;

		m_vActiveServer.push_back(ActiveServer);
		m_NumVectors = 1;
    }

    IpVxAddress  NTPClockGlobalGetServerMessage::getActiveServer() const
    {
        return (m_ActiveServer);
    }

    void  NTPClockGlobalGetServerMessage::setMappedId(const UI32 &MappedId)
    {
        m_MappedId  =  MappedId;

		m_vMappedId.push_back(MappedId);
		m_NumVectors = 1;
    }

    UI32  NTPClockGlobalGetServerMessage::getMappedId() const
    {
        return (m_MappedId);
    }

    UI32  NTPClockGlobalGetServerMessage::getNumVectors() const
    {
        return (m_NumVectors);
    }

	void NTPClockGlobalGetServerMessage::clearNumVectors()
	{
		m_NumVectors = 0;
        m_vActiveServer.clear();
        m_vMappedId.clear();
	}

    void  NTPClockGlobalGetServerMessage::setVectorActiveServer(const IpVxAddress &ActiveServer, const UI32 &MappedId)
    {
        m_vActiveServer.push_back(ActiveServer);
        m_vMappedId.push_back(MappedId);
        m_NumVectors++;
    }

    bool NTPClockGlobalGetServerMessage::getVectorActiveServer(const UI32 &index, IpVxAddress &ActiveServer, UI32 &MappedId)
    {
        if (index >= m_NumVectors)
        {
            return false;
        }

        ActiveServer = m_vActiveServer[index];
        MappedId = m_vMappedId[index];

        return true;
    }


}
