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
 *   Copyright (C) 2005-20l1 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amitayu Das                                                  *
 **************************************************************************/

#include "ClientInterface/Eld/EldClientUpdateRxlimConfigMessage.h"
#include "Nsm/Global/Eld/EldGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"

namespace DcmNs
{

    EldClientUpdateRxlimConfigMessage::EldClientUpdateRxlimConfigMessage ()
        : DcmManagementInterfaceMessage (MSG_ELD_SCALARS_GLOBAL_CONF),
        m_opCode            (0),
		m_rxlim             (0),
		m_hello_int          (0),
		m_shutdn_time        (0)
    {
    }

    EldClientUpdateRxlimConfigMessage::EldClientUpdateRxlimConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation)
        : DcmManagementInterfaceMessage (MSG_ELD_SCALARS_GLOBAL_CONF),
		m_opCode    (opCode)
		{
			m_rxlim = 0;
			m_hello_int = 0;
			m_shutdn_time = 0;
    	}

    EldClientUpdateRxlimConfigMessage::~EldClientUpdateRxlimConfigMessage ()
    {
    }

    void  EldClientUpdateRxlimConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_rxlim,"rxlim"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI32(&m_hello_int,"helloint"));
        addSerializableAttribute (new AttributeUI32(&m_shutdn_time,"shutdntime"));
    }
    
	void  EldClientUpdateRxlimConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  EldClientUpdateRxlimConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }
	
	void  EldClientUpdateRxlimConfigMessage::setRxlim(const UI32 &rxlim)
    {
        m_rxlim  =  rxlim;
    }

    UI32  EldClientUpdateRxlimConfigMessage::getRxlim() const
    {
        return (m_rxlim);
    }
	
	void  EldClientUpdateRxlimConfigMessage::setHelloInt(const UI32 &hello_int)
    {
        m_hello_int  =  hello_int;
    }

    UI32  EldClientUpdateRxlimConfigMessage::getHelloInt() const
    {
        return (m_hello_int);
    }
	
	void  EldClientUpdateRxlimConfigMessage::setShutdnTime(const UI32 &shutdn_time)
    {
        m_shutdn_time  =  shutdn_time;
    }

    UI32  EldClientUpdateRxlimConfigMessage::getShutdnTime() const
    {
        return (m_shutdn_time);
    }
	
	const void *EldClientUpdateRxlimConfigMessage::getCStructureForInputs ()
    {
        eld_mgmt_conf_req_t *eld_global_config = (eld_mgmt_conf_req_t *)malloc (sizeof (eld_mgmt_conf_req_t));

        eld_global_config->entry.global_conf.cmd_code          = m_opCode;
		trace(TRACE_LEVEL_DEBUG, string("Op_code is ") + eld_global_config->entry.global_conf.cmd_code);

		if(IS_CMDCODE_SET(m_opCode, ELD_GLOBAL_CONF_VCS_RX_LIMIT)) 
		{
        	eld_global_config->entry.global_conf.vcs_rx_limit = m_rxlim;
			trace(TRACE_LEVEL_DEBUG, string("Rxlim is ") + eld_global_config->entry.global_conf.vcs_rx_limit);
		}
		if(IS_CMDCODE_SET(m_opCode, ELD_GLOBAL_CONF_HELLO_TIME)) 
		{
        	eld_global_config->entry.global_conf.hello_interval = m_hello_int;
			trace(TRACE_LEVEL_DEBUG, string("Hello-time is ") +  eld_global_config->entry.global_conf.hello_interval);
		}
		if(IS_CMDCODE_SET(m_opCode, ELD_GLOBAL_CONF_SHUTDOWN_TIME))
		{
        	eld_global_config->entry.global_conf.shutdown_time = m_shutdn_time;
			trace(TRACE_LEVEL_DEBUG, string("Shutdown-time is ") + eld_global_config->entry.global_conf.shutdown_time);
		}

		return (eld_global_config);
	} 
	
	void EldClientUpdateRxlimConfigMessage::loadOutputsFromCStructure (const void *pOutputCStructure)
	{
	}
}
