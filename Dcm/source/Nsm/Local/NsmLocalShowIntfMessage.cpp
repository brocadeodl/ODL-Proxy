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
 *   Author : skbalasu                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalShowIntfMessage.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/DceWaveClientMessage.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmLocalShowIntfMessage::NsmLocalShowIntfMessage ()
        : PoPaginationLocalMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWINTERFACE)
    {
		m_cmdCode = 0;
		m_isGwRequest = true;
		m_locationId = -1;
		m_reqNoRecords = INTF_PER_PAGE;
		m_reqStartIndex = "";
		m_reqEndIndex = "";
		m_reqIfType = IF_TYPE_INVALID;
		m_reqRequestType = 0;
    }

    NsmLocalShowIntfMessage::NsmLocalShowIntfMessage (const string &ifName,const UI32 &cmdCode,const bool isGwRequest, const SI32 locationId)
        : PoPaginationLocalMessage (NsmLocalObjectManager::getPrismServiceId (),NSMLOCALSHOWINTERFACE,isGwRequest),
        m_ifName    (ifName),
        m_cmdCode   (cmdCode),
		m_locationId(locationId)
    {
		m_reqNoRecords = INTF_PER_PAGE;
		m_reqStartIndex = "";
		m_reqEndIndex = "";
		m_reqIfType = IF_TYPE_INVALID;
		m_reqRequestType = 0;
    }

    NsmLocalShowIntfMessage::~NsmLocalShowIntfMessage ()
    {
    }

    void  NsmLocalShowIntfMessage::setupAttributesForSerialization()
    {
    	PoPaginationLocalMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
		//addSerializableAttribute (new AttributeBool(&m_isGwRequest,"isGwRequest"));
		addSerializableAttribute (new AttributeSI32(&m_locationId,"locationId"));
        addSerializableAttribute (new AttributeUI32(&m_reqNoRecords,"reqNoRecords"));
        addSerializableAttribute (new AttributeString(&m_reqStartIndex,"reqStartIndex"));
        addSerializableAttribute (new AttributeString(&m_reqEndIndex,"reqEndIndex"));
        addSerializableAttribute (new AttributeEnum((UI32 *)(&m_reqIfType),"reqIfType"));
        addSerializableAttribute (new AttributeUI32(&m_reqRequestType,"reqRequestType"));
    }

    void  NsmLocalShowIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLocalShowIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmLocalShowIntfMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  NsmLocalShowIntfMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    /*void NsmLocalShowIntfMessage::setIsGwRequest (bool isGwRequest)
    {
        m_isGwRequest = isGwRequest;
    }

    bool NsmLocalShowIntfMessage::getIsGwRequest () const
    {
        return (m_isGwRequest);
    }*/

	void NsmLocalShowIntfMessage::setReqNoRecords(const UI32 &reqNoRecords)
	{
		m_reqNoRecords = reqNoRecords;
	}

	UI32 NsmLocalShowIntfMessage::getReqNoRecords()  const
	{
		return m_reqNoRecords;
	}

	void NsmLocalShowIntfMessage::setReqStartIndex(const string &reqStartIndex)
	{
		m_reqStartIndex = reqStartIndex;
	}

	string NsmLocalShowIntfMessage::getReqStartIndex()  const
	{
		return m_reqStartIndex;
	}

	void NsmLocalShowIntfMessage::setReqEndIndex(const string &reqEndIndex)
	{
		m_reqEndIndex = reqEndIndex;
	}

	string NsmLocalShowIntfMessage::getReqEndIndex()  const
	{
		return m_reqEndIndex;
	}

	void NsmLocalShowIntfMessage::setReqIfType(const InterfaceType &reqIfType)
	{
		m_reqIfType = reqIfType;
	}

	InterfaceType NsmLocalShowIntfMessage::getReqIfType()  const
	{
		return m_reqIfType;
	}

	void NsmLocalShowIntfMessage::setReqRequestType(const UI32 &reqRequestType)
	{
		m_reqRequestType = reqRequestType;
	}

	UI32 NsmLocalShowIntfMessage::getReqRequestType()  const
	{
		return m_reqRequestType;
	}

    void NsmLocalShowIntfMessage::getOutputBufferRef (UI32 bufferNum, UI32 &size,void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    void NsmLocalShowIntfMessage::copyAllBuffers (const PrismMessage &prismMessage)
    {
        this->copyBuffersFrom(prismMessage);
    }


	void NsmLocalShowIntfMessage::setLocationId(const SI32 &locationId)
	{
		m_locationId = locationId;

	}	
	SI32 NsmLocalShowIntfMessage::getLocationId()  const
	{
		return m_locationId;
	}	

}
