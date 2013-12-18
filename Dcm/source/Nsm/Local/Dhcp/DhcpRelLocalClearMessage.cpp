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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/
#include "ClientInterface/DceWaveClientMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalObjectManager.h"
#include "Nsm/Local/Dhcp/DhcpRelLocalClearMessage.h"
#include "Nsm/Local/Dhcp/DhcpRelInterfaceLocalManagedObject.h"
#include "ClientInterface/Dhcp/DhcpMessageDef.h"

namespace DcmNs
{

    DhcpRelLocalClearMessage::DhcpRelLocalClearMessage()
        : PrismMessage (DhcpRelLocalObjectManager::getPrismServiceId (), IPHELPCLEAR),
        m_cmdCode       (0),
        m_serverAddress (""),
        m_isDistributionReqd(false)
    {
        m_locationIds.clear ();
    }

    DhcpRelLocalClearMessage::DhcpRelLocalClearMessage(const UI32 &cmdCode)
        : PrismMessage (DhcpRelLocalObjectManager::getPrismServiceId (), IPHELPCLEAR),
        m_cmdCode       (cmdCode),
        m_serverAddress (""),
        m_isDistributionReqd(false)
    {
        m_locationIds.clear ();
    }

    DhcpRelLocalClearMessage::~DhcpRelLocalClearMessage ()
    {
    }

    void  DhcpRelLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
	    addSerializableAttribute (new AttributeIpV4Address(&m_serverAddress,"serverAddress"));
        addSerializableAttribute (new AttributeBool(&m_isDistributionReqd,"isDistributionRequired"));
        addSerializableAttribute (new AttributeUI32Vector(&m_locationIds, "locations"));
    }


    void  DhcpRelLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  DhcpRelLocalClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

	void  DhcpRelLocalClearMessage::setServerAddress(const IpV4Address &serverAddress)
	{
    	m_serverAddress  =  serverAddress;
	}

	IpV4Address  DhcpRelLocalClearMessage::getServerAddress() const
	{
    	return (m_serverAddress);
	}

    void DhcpRelLocalClearMessage::setIsDistributionReqd (const bool &isDistributionReqd)
    {
        m_isDistributionReqd = isDistributionReqd;
    }

    bool DhcpRelLocalClearMessage::getIsDistributionReqd () const
    {
        return m_isDistributionReqd;
    }

    void DhcpRelLocalClearMessage::setLocationIds (const vector<LocationId> &locationIds)
    {
        m_locationIds = locationIds;
    }

    vector<LocationId> DhcpRelLocalClearMessage::getLocationIds () const
    {
        return m_locationIds;
    }

    void DhcpRelLocalClearMessage::getOutputBuffer (UI32 bufferNum, UI32 &size, void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void DhcpRelLocalClearMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {   
        this->copyBuffersFrom(prismMessage);
    }
}
