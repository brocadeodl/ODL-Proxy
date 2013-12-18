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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Ojasvita                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "ClientInterface/StormControl/NsmStormControlClientIntfConfSetMsg.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/Qos/qos_bum_dcm_common.h"
#include "Framework/Attributes/AttributeEnum.h"

namespace DcmNs
{

    NsmStormControlClientIntfConfSetMsg::NsmStormControlClientIntfConfSetMsg ()
        : DcmManagementInterfaceMessage (MSG_STORMCONTROL_PHY_INTF_CONF)
    {
    }

    NsmStormControlClientIntfConfSetMsg::NsmStormControlClientIntfConfSetMsg (const UI32 &opCode, 
        const string &ifName, const UI32 &protocolType, const UI32 &rateFormat,
        const UI64 &bps, const UI32 &percentage, const UI32 &action)
        : DcmManagementInterfaceMessage (MSG_STORMCONTROL_PHY_INTF_CONF),
        m_opCode	  		(opCode),
        m_ifName  	  		(ifName),
        m_protocolType    	(protocolType),
        m_rateFormat    	(rateFormat),
        m_bps			    (bps),
        m_percentage	    (percentage),
		m_action			(action)
    {
    }

    NsmStormControlClientIntfConfSetMsg::~NsmStormControlClientIntfConfSetMsg ()
    {
    }

    void  NsmStormControlClientIntfConfSetMsg::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32((UI32*)&m_protocolType, "protocolType"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_rateFormat, "rateFormat"));
        addSerializableAttribute (new AttributeUI64(&m_bps, "bps"));
        addSerializableAttribute (new AttributeUI32(&m_percentage, "percentage"));
        addSerializableAttribute (new AttributeUI32((UI32*)&m_action, "action"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
    }

    void  NsmStormControlClientIntfConfSetMsg::setProtocolType(const UI32 &protocolType)
	{
		m_protocolType = protocolType;
	}

    UI32  NsmStormControlClientIntfConfSetMsg::getProtocolType() const
	{
		return (m_protocolType);
	}

    void  NsmStormControlClientIntfConfSetMsg::setRateFormat(const UI32 &rateFormat)
	{
		m_rateFormat = rateFormat;
	}
		
	UI32  NsmStormControlClientIntfConfSetMsg::getRateFormat() const
	{
		return (m_rateFormat);
	}

    void  NsmStormControlClientIntfConfSetMsg::setBps(const UI64 &bps)
	{
		m_bps = bps;
	}
	
	UI64  NsmStormControlClientIntfConfSetMsg::getBps() const
	{
		return (m_bps);
	}

    void  NsmStormControlClientIntfConfSetMsg::setPercentage(const UI32 &percentage)
	{
		m_percentage = percentage;
	}

    UI32  NsmStormControlClientIntfConfSetMsg::getPercentage() const
	{
		return (m_percentage);
	}

	void  NsmStormControlClientIntfConfSetMsg::setAction(const UI32 &action)
	{
		m_action = action;
	}

	UI32	NsmStormControlClientIntfConfSetMsg::getAction() const
	{
		return (m_action);
	}

    void  NsmStormControlClientIntfConfSetMsg::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmStormControlClientIntfConfSetMsg::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmStormControlClientIntfConfSetMsg::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmStormControlClientIntfConfSetMsg::getIfName() const
    {
        return (m_ifName);
    }

	const void *NsmStormControlClientIntfConfSetMsg::getCStructureForInputs ()
	{

		qosBumDcmdConfigMsg_t *req_msg = (qosBumDcmdConfigMsg_t*)calloc(1, sizeof(qosBumDcmdConfigMsg_t));

		req_msg->opcode = (qosBumDcmdOpcode_t) m_opCode;
		strcpy (req_msg->if_name, m_ifName.c_str());
		req_msg->protocol = (qosBumProtocol_t) m_protocolType;
		req_msg->action = (qosBumAction_t) m_action;
		req_msg->rate_format = (qosBumRateFormat_t) m_rateFormat;
		if (m_rateFormat == QOS_BUM_RATE_FORMAT_BPS_E)
			req_msg->rate = m_bps;
		else if (m_rateFormat == QOS_BUM_RATE_FORMAT_PRECENTAGE_E)
			req_msg->rate = m_percentage;

		return (req_msg);
	}
	    
	void NsmStormControlClientIntfConfSetMsg::loadOutputsFromCStructure ( const void *pOutputCStructure)
    {
    }		
}
