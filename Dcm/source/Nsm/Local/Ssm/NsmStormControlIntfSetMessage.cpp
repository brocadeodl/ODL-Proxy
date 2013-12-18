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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          		*
 *   All rights reserved.                                                   *
 *   .Author : Ojasvita                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Ssm/NsmStormControlIntfSetMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ssmBum_public.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmStormControlIntfSetMessage::NsmStormControlIntfSetMessage ()
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMSTORMCONTROLINTFSET)
    {
    }

    NsmStormControlIntfSetMessage::NsmStormControlIntfSetMessage (const UI32 &opCode, 
        const string &ifName, const UI32 &ifType, const UI32 &protocolType, const UI32 &rateFormat,
        const UI64 &bps, const UI32 &percentage, const UI32 &action)
        : ManagementInterfaceMessage (NsmLocalObjectManager::getClassName (),NSMSTORMCONTROLINTFSET),
        m_opCode	  		(opCode),
        m_ifName  	  		(ifName),
        m_ifType	  		(ifType),
        m_protocolType    	(protocolType),
        m_rateFormat    	(rateFormat),
        m_bps			    (bps),
        m_percentage	    (percentage),
		m_action			(action)
    {
    }

    NsmStormControlIntfSetMessage::~NsmStormControlIntfSetMessage ()
    {
    }

    void  NsmStormControlIntfSetMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32((UI32*)&m_protocolType, "protocolType"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_rateFormat, "rateFormat"));
        addSerializableAttribute (new AttributeUI64(&m_bps, "bps"));
        addSerializableAttribute (new AttributeUI32(&m_percentage, "percentage"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_action, "action"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void  NsmStormControlIntfSetMessage::setProtocolType(const UI32 &protocolType)
	{
		m_protocolType = protocolType;
	}

    UI32  NsmStormControlIntfSetMessage::getProtocolType() const
	{
		return (m_protocolType);
	}

    void  NsmStormControlIntfSetMessage::setRateFormat(const UI32 &rateFormat)
	{
		m_rateFormat = rateFormat;
	}
		
	UI32  NsmStormControlIntfSetMessage::getRateFormat() const
	{
		return (m_rateFormat);
	}

    void  NsmStormControlIntfSetMessage::setBps(const UI64 &bps)
	{
		m_bps = bps;
	}
	
	UI64  NsmStormControlIntfSetMessage::getBps() const
	{
		return (m_bps);
	}

    void  NsmStormControlIntfSetMessage::setPercentage(const UI32 &percentage)
	{
		m_percentage = percentage;
	}

    UI32  NsmStormControlIntfSetMessage::getPercentage() const
	{
		return (m_percentage);
	}

    void  NsmStormControlIntfSetMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmStormControlIntfSetMessage::getIfType() const
    {
        return (m_ifType);
    }

	void  NsmStormControlIntfSetMessage::setAction(const UI32 &action)
	{
		m_action = action;
	}

	UI32	NsmStormControlIntfSetMessage::getAction() const
	{
		return (m_action);
	}

    void  NsmStormControlIntfSetMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmStormControlIntfSetMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmStormControlIntfSetMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmStormControlIntfSetMessage::getIfName() const
    {
        return (m_ifName);
    }
}
