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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Eld/NsmSetRxlimMessage.h"
#include "Nsm/Local/NsmTypes.h"
//#include "Nsm/Local/Eld/EldLocalTypes.h"

namespace DcmNs
{

    NsmSetRxlimMessage::NsmSetRxlimMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),ELDGLOBALSETRXLIM)
		{
			m_poIfIndex = 0;
			m_opCode = 0;
			m_rxlim = 1;
			m_hello_int = 1000;
			m_shutdn_time = 0;
    }

    NsmSetRxlimMessage::NsmSetRxlimMessage (const string &poId,const string &poName,const UI32 &poIfIndex,const string &desc)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId  (),ELDGLOBALSETRXLIM),
        m_poId    (poId),
        m_poName    (poName),
        m_poIfIndex    (poIfIndex),
        m_desc    (desc)
    {
		m_opCode = 0;
		m_rxlim = 1;
		m_hello_int = 1000;
		m_shutdn_time = 0;
    }

    NsmSetRxlimMessage::~NsmSetRxlimMessage ()
    {
    }

    void  NsmSetRxlimMessage::setupAttributesForSerialization()
	{
		PrismMessage::setupAttributesForSerialization ();
		addSerializableAttribute (new AttributeUI32(&m_rxlim,"rxlim"));
		addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
		addSerializableAttribute (new AttributeUI32(&m_hello_int,"helloint"));
		addSerializableAttribute (new AttributeUI32(&m_shutdn_time,"shutdntime"));
		addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeString(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_poIfIndex,"poIfIndex"));
        addSerializableAttribute (new AttributeString(&m_poName,"poName"));
    }
    
	void  NsmSetRxlimMessage::setRxlim(const UI32 &rxlim)
	{
		m_rxlim  =  rxlim;
	}

	UI32  NsmSetRxlimMessage::getRxlim() const
	{
		return (m_rxlim);
	}
	void  NsmSetRxlimMessage::setOpCode(const UI32 &opCode)
	{
		m_opCode  =  opCode;
	}   

	UI32  NsmSetRxlimMessage::getOpCode() const
	{   
		return (m_opCode); 
	}   

	void  NsmSetRxlimMessage::setHelloInt(const UI32 &hello_int)
    {
        m_hello_int  =  hello_int;
    }

    UI32  NsmSetRxlimMessage::getHelloInt() const
    {
        return (m_hello_int);
    }

    void  NsmSetRxlimMessage::setShutdnTime(const UI32 &shutdn_time)
    {
        m_shutdn_time  =  shutdn_time;
    }

    UI32  NsmSetRxlimMessage::getShutdnTime() const
    {
        return (m_shutdn_time);
    }
}
