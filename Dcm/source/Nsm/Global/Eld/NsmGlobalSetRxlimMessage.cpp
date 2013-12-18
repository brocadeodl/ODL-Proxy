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
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "Nsm/Global/PoIntfManagedObject.h"
#include "Nsm/Global/Eld/NsmGlobalSetRxlimMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"
//#include "Nsm/Global/Eld/EldGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalSetRxlimMessage::NsmGlobalSetRxlimMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALELDUPDATECONFIG),
		m_refPoIntfManagedObject (NULL)
    {
		m_locationId = 0;
		m_poIfIndex = 0;
		m_poLacpSystemIdDomainId = 0;
		m_poMinLinks = 1;
		m_poLoadBalanceMode = 7;
		m_poMode = 0;
		m_poType = 0;
		m_send = true;
        m_cmdCode = 0;
		m_rxlim = 0;
		m_hello_int = 0;
		m_shutdn_time = 0;
    }

    NsmGlobalSetRxlimMessage::NsmGlobalSetRxlimMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc,const UI32 &poMode,const UI32 &poType, const UI32 &poMinLinks, const UI32 &poLoadBalanceMode, const string &poLacpSystemId, const UI32 &poLacpSystemIdDomainId )
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALELDUPDATECONFIG),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_desc    (desc),
        m_poMode    (poMode),
        m_poType    (poType),
        m_poMinLinks    (poMinLinks),
        m_poLoadBalanceMode    (poLoadBalanceMode),
        m_poLacpSystemId    (poLacpSystemId),
        m_poLacpSystemIdDomainId    (poLacpSystemIdDomainId),
		m_refPoIntfManagedObject (NULL)
    {
		m_locationId = 0; 
		m_send = true;
        m_cmdCode = 0;
		m_rxlim = 0;
		m_hello_int = 0;
		m_shutdn_time = 0;
    }

    NsmGlobalSetRxlimMessage::~NsmGlobalSetRxlimMessage ()
    {
    }

    void  NsmGlobalSetRxlimMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_rxlim,"rxlim"));
		addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
		addSerializableAttribute (new AttributeUI32(&m_hello_int,"helloint"));
		addSerializableAttribute (new AttributeUI32(&m_shutdn_time,"shutdntime"));
		addSerializableAttribute (new AttributeString(&m_desc,"descr"));
        addSerializableAttribute (new AttributeUI32(&m_poMode,"poMode"));
        addSerializableAttribute (new AttributeUI32(&m_poType,"poType"));
        addSerializableAttribute (new AttributeUI32(&m_poMinLinks,"poMinLinks"));
        addSerializableAttribute (new AttributeUI32(&m_poLoadBalanceMode,"poLoadBalanceMode"));
        addSerializableAttribute (new AttributeString(&m_poLacpSystemId,"poLacpSystemId"));

        addSerializableAttribute (new AttributeUI32(&m_poLacpSystemIdDomainId,"poLacpSystemIdDomainId"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
        addSerializableAttribute (new AttributeBool(&m_send, "send")); 
        addSerializableAttribute (new AttributeLocationId(&m_locationId, "locationId"));
    }

	void  NsmGlobalSetRxlimMessage::setRxlim(const UI32 &rxlim)
	{
		m_rxlim  =  rxlim;
	}

	UI32  NsmGlobalSetRxlimMessage::getRxlim() const
	{
		return (m_rxlim);
	}

	void  NsmGlobalSetRxlimMessage::setCmdCode(const UI32 &cmdCode)
	{
		m_cmdCode  =  cmdCode;
	}

	UI32  NsmGlobalSetRxlimMessage::getCmdCode() const
	{
		return (m_cmdCode);
	}

    void  NsmGlobalSetRxlimMessage::setHelloInt(const UI32 &hello_int)
    {
        m_hello_int  =  hello_int;
    }

    UI32  NsmGlobalSetRxlimMessage::getHelloInt() const
    {
        return (m_hello_int);
    }

    void  NsmGlobalSetRxlimMessage::setShutdnTime(const UI32 &shutdn_time)
    {
        m_shutdn_time  =  shutdn_time;
    }

    UI32  NsmGlobalSetRxlimMessage::getShutdnTime() const
    {
        return (m_shutdn_time);
    }
    
	void  NsmGlobalSetRxlimMessage::setLocationId(const LocationId &locationId)
    {
        m_locationId  =  locationId;
    }

    LocationId  NsmGlobalSetRxlimMessage::getLocationId() const
    {
        return (m_locationId);
    }
    
	void  NsmGlobalSetRxlimMessage::setPoId(const string &poId)
    {
        m_poId  =  poId;
    }

    string  NsmGlobalSetRxlimMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmGlobalSetRxlimMessage::setPoName(const string &poName)
    {
        m_poName  =  poName;
    }

    string  NsmGlobalSetRxlimMessage::getPoName() const
    {
        return (m_poName);
    }


    void  NsmGlobalSetRxlimMessage::setSendToLocal(const bool &send)
    {
        m_send  =  send;
    }

    bool  NsmGlobalSetRxlimMessage::getSendToLocal() const
    {
        return (m_send);
    }

    void  NsmGlobalSetRxlimMessage::setPoIfIndex(const UI32 &poIfIndex)
    {
        m_poIfIndex  =  poIfIndex;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoIfIndex() const
    {
        return (m_poIfIndex);
    }

    void  NsmGlobalSetRxlimMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  NsmGlobalSetRxlimMessage::getDesc() const
    {
        return (m_desc);
    }

    void  NsmGlobalSetRxlimMessage::setPoMode(const UI32 &poMode)
    {
        m_poMode  =  poMode;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoMode() const
    {
        return (m_poMode);
    }

    void  NsmGlobalSetRxlimMessage::setPoType(const UI32 &poType)
    {
        m_poType  =  poType;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoType() const
    {
        return (m_poType);
    }

    void  NsmGlobalSetRxlimMessage::setPoMinLinks(const UI32 &poMinLinks)
    {
        m_poMinLinks  =  poMinLinks;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoMinLinks() const
    {
        return (m_poMinLinks);
    }

   void  NsmGlobalSetRxlimMessage::setPoLoadBalanceMode(const UI32 &poLoadBalanceMode)
    {
        m_poLoadBalanceMode  =  poLoadBalanceMode;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoLoadBalanceMode() const
    {
        return (m_poLoadBalanceMode);
    }

    void  NsmGlobalSetRxlimMessage::setPoLacpSystemId(const string &poLacpSystemId)
    {
        m_poLacpSystemId  =  poLacpSystemId;
    }

    string  NsmGlobalSetRxlimMessage::getPoLacpSystemId() const
    {
        return (m_poLacpSystemId);
    }

    void  NsmGlobalSetRxlimMessage::setPoLacpSystemIdDomainId(const UI32 &poLacpSystemIdDomainId)
    {
        m_poLacpSystemIdDomainId  =  poLacpSystemIdDomainId;
    }

    UI32  NsmGlobalSetRxlimMessage::getPoLacpSystemIdDomainId() const
    {
        return (m_poLacpSystemIdDomainId);
    }

    PoIntfManagedObject *NsmGlobalSetRxlimMessage::getPoRef()
    {
        return m_refPoIntfManagedObject;
    }

    void NsmGlobalSetRxlimMessage::setPoRef(PoIntfManagedObject *refPoIntfManagedObject)
    {
        m_refPoIntfManagedObject = refPoIntfManagedObject;
    }
    
}
