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
 *   Author : skbalasu.                                                     *
 **************************************************************************/

#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/NsmGlobalCreatePoIntfMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalCreatePoIntfMessage::NsmGlobalCreatePoIntfMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEPOINTF),
		m_refPoIntfManagedObject (NULL)
    {
		m_locationId = 0;
		m_poIfIndex = 0;
		m_poLacpSystemIdDomainId = 0;
		m_poMinLinks = 1;
		m_poAllowedSpeed = (LineSpeed)2;
		m_poLoadBalanceMode = 7;
		m_poMode = 0;
		m_poType = 0;
		m_send = true;
    }

    NsmGlobalCreatePoIntfMessage::NsmGlobalCreatePoIntfMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc,const UI32 &poMode,const UI32 &poType, const UI32 &poMinLinks, const LineSpeed &poAllowedSpeed,const UI32 &poLoadBalanceMode, const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId )
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALCREATEPOINTF),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_desc    (desc),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
		m_poAllowedSpeed (poAllowedSpeed),
        m_poLoadBalanceMode    (poLoadBalanceMode),
        m_poLacpSystemId    (poLacpSystemId),
        m_poLacpSystemIdDomainId    (poLacpSystemIdDomainId),
		m_refPoIntfManagedObject (NULL)
    {
		m_locationId = 0; 
		m_send = true;
    }

    NsmGlobalCreatePoIntfMessage::~NsmGlobalCreatePoIntfMessage ()
    {
    }

    void  NsmGlobalCreatePoIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_desc,"descr"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
        addSerializableAttribute (new AttributeUI32(&m_poMinLinks,"poMinLinks"));
        addSerializableAttribute (new AttributeEnum((UI32*)(&m_poAllowedSpeed),"poAllowedSpeed"));
		addSerializableAttribute (new AttributeUI32(&m_poLoadBalanceMode,"poLoadBalanceMode"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));

        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeLocationId(&m_locationId, "locationId"));
        addSerializableAttribute (new AttributeBool(&m_send, "send"));
    }

    void  NsmGlobalCreatePoIntfMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmGlobalCreatePoIntfMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmGlobalCreatePoIntfMessage::getPoName() const
    {
        return (m_poName);
    }

    void  NsmGlobalCreatePoIntfMessage::setLocationId(const LocationId &locationId)
    {
        m_locationId  =  locationId;
    }

    LocationId  NsmGlobalCreatePoIntfMessage::getLocationId() const
    {
        return (m_locationId);
    }

    void  NsmGlobalCreatePoIntfMessage::setSendToLocal(const bool &send)
    {
        m_send  =  send;
    }

    bool  NsmGlobalCreatePoIntfMessage::getSendToLocal() const
    {
        return (m_send);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmGlobalCreatePoIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalCreatePoIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

	void  NsmGlobalCreatePoIntfMessage::setPoAllowedSpeed(const LineSpeed &poAllowedSpeed)
	{
		m_poAllowedSpeed  =  poAllowedSpeed;
	}

	LineSpeed NsmGlobalCreatePoIntfMessage::getPoAllowedSpeed() const
	{
		return (m_poAllowedSpeed);
	}

	void  NsmGlobalCreatePoIntfMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
	{
		m_poLoadBalanceMode  =  poLoadBalanceMode;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmGlobalCreatePoIntfMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmGlobalCreatePoIntfMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmGlobalCreatePoIntfMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    PoIntfManagedObject *NsmGlobalCreatePoIntfMessage::getPoRef()
    {
        return m_refPoIntfManagedObject;
    }

    void NsmGlobalCreatePoIntfMessage::setPoRef(PoIntfManagedObject *refPoIntfManagedObject)
    {
        m_refPoIntfManagedObject = refPoIntfManagedObject;
    }
    
}
