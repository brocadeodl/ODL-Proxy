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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "ClientInterface/Mstp/MstpBridgeConfigMessage.h"
#include "ClientInterface/Mstp/MstpMessageDef.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"

namespace DcmNs
{

    MstpBridgeConfigMessage::MstpBridgeConfigMessage ()
        : DcmManagementInterfaceMessage (MSTPBRIDGECONFIG)
        , m_priority ( 0 )
        , m_forwardDelay ( 0 )
        , m_helloTime ( 0 )
        , m_maxAge ( 0 )
        , m_maxHop ( 0 )
        , m_desc ( "" )
        , m_errDisableInterval ( 0 )
        , m_txHoldCount ( 0 )
        , m_revision ( 0 )
        , m_region ( "" )
        , m_mode ( 0 )
        , m_agingTime ( 0 )
        , m_ciscoInterop(false)
        , m_macReduction(false)
        , m_stpDisable(false)
        , m_errDisableEnable(false)
        , m_poPathcost(MSTP_DCM_DEFAULT_PATHCOST) 
        , m_cmdCode ( 0 )
        , m_vcsBridgeMac ((UI8*) "\0\0\0\0\0\0")
    {
    }

   MstpBridgeConfigMessage::MstpBridgeConfigMessage(MstpUpdateBridgeConfigMessage &msg)
        : DcmManagementInterfaceMessage (MSTPBRIDGECONFIG)
    {
        m_priority = msg.m_priority;
        m_forwardDelay = msg.m_forwardDelay;
        m_helloTime = msg.m_helloTime;
        m_maxAge = msg.m_maxAge;
        m_maxHop = msg.m_maxHop;
        m_desc = msg.m_desc;
        m_errDisableInterval = msg.m_errDisableInterval;
        m_txHoldCount = msg.m_txHoldCount;
        m_revision = msg.m_revision;
        m_region = msg.m_region;
        m_mode = msg.m_mode;
        m_agingTime = 0;
        m_ciscoInterop  = false;
        m_macReduction  = false;
        m_stpDisable    = false;
        m_errDisableEnable  = false;
        m_poPathcost    = MSTP_DCM_DEFAULT_PATHCOST;
        m_cmdCode = msg.m_cmdCode;
        m_vcsBridgeMac = msg.m_vcsBridgeMac;
    }


    MstpBridgeConfigMessage::~MstpBridgeConfigMessage ()
    {
    }

    void  MstpBridgeConfigMessage::setupAttributesForSerialization()
    {
        DcmManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_priority,"priority"));
        addSerializableAttribute (new AttributeUI32(&m_forwardDelay,"forwardDelay"));
        addSerializableAttribute (new AttributeUI32(&m_helloTime,"helloTime"));
        addSerializableAttribute (new AttributeUI32(&m_maxAge,"maxAge"));
        addSerializableAttribute (new AttributeUI32(&m_maxHop,"maxHop"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_errDisableInterval,"errDisableInterval"));
        addSerializableAttribute (new AttributeUI32(&m_txHoldCount,"txHoldCount"));
        addSerializableAttribute (new AttributeUI16(&m_revision,"revision"));
        addSerializableAttribute (new AttributeString(&m_region,"region"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeUI32(&m_agingTime,"agingTime"));
        addSerializableAttribute (new AttributeBool(&m_ciscoInterop,"ciscoInterop"));
        addSerializableAttribute (new AttributeBool(&m_macReduction,"macReduction"));
        addSerializableAttribute (new AttributeBool(&m_stpDisable,"stpDisable"));
        addSerializableAttribute (new AttributeBool(&m_errDisableEnable,"errDisableEnable"));
        addSerializableAttribute (new AttributeBool(&m_poPathcost,"poPathcost"));
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeMacAddress(&m_vcsBridgeMac,"vcsBridgeMac"));
    }

    void  MstpBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpBridgeConfigMessage::setMaxHop(const UI32 &maxHop)
    {
        m_maxHop  =  maxHop;
    }

    UI32  MstpBridgeConfigMessage::getMaxHop() const
    {
        return (m_maxHop);
    }

    void  MstpBridgeConfigMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  MstpBridgeConfigMessage::getDesc() const
    {
        return (m_desc);
    }

    void  MstpBridgeConfigMessage::setErrDisableInterval(const UI32 &errDisableInterval)
    {
        m_errDisableInterval  =  errDisableInterval;
    }

    UI32  MstpBridgeConfigMessage::getErrDisableInterval() const
    {
        return (m_errDisableInterval);
    }

    void  MstpBridgeConfigMessage::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  MstpBridgeConfigMessage::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  MstpBridgeConfigMessage::setRevision(const UI16 &revision)
    {
        m_revision  =  revision;
    }

    UI16  MstpBridgeConfigMessage::getRevision() const
    {
        return (m_revision);
    }

    void  MstpBridgeConfigMessage::setRegion(const string &region)
    {
        m_region  =  region;
    }

    string  MstpBridgeConfigMessage::getRegion() const
    {
        return (m_region);
    }

    void  MstpBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  MstpBridgeConfigMessage::setAgingTime(const UI32 &agingTime)
    {
        m_agingTime  =  agingTime;
    }

    UI32  MstpBridgeConfigMessage::getAgingTime() const
    {
        return (m_agingTime);
    }

	void MstpBridgeConfigMessage::setCiscoInterop(const bool &ciscoInterop)
	{
		m_ciscoInterop = ciscoInterop;
	}

	bool MstpBridgeConfigMessage::getCiscoInterop()  const
	{
		return (m_ciscoInterop);
	}

	void MstpBridgeConfigMessage::setMacReduction(const bool &macReduction)
	{
		m_macReduction = macReduction;
	}

	bool MstpBridgeConfigMessage::getMacReduction()  const
	{
		return (m_macReduction);
	}

	void MstpBridgeConfigMessage::setStpDisable(const bool &stpDisable)
	{
		m_stpDisable = stpDisable;
	}

	bool MstpBridgeConfigMessage::getStpDisable()  const
	{
		return (m_stpDisable);
	}

	void MstpBridgeConfigMessage::setErrDisableEnable(const bool &errDisableEnable)
	{
		m_errDisableEnable = errDisableEnable;
	}

	bool MstpBridgeConfigMessage::getErrDisableEnable()  const
	{
		return (m_errDisableEnable);
	}

	void MstpBridgeConfigMessage::setPoPathcost(const bool &poPathcost)
	{
		m_poPathcost = poPathcost;
	}

	bool MstpBridgeConfigMessage::getPoPathcost()  const
	{
		return (m_poPathcost);
	}

	void MstpBridgeConfigMessage::setVcsBridgeMac(const MacAddress &mac)
	{
	    m_vcsBridgeMac = mac;
	}

	const MacAddress &MstpBridgeConfigMessage::getVcsBridgeMac() const
	{
	    return m_vcsBridgeMac;
	}


    const void * MstpBridgeConfigMessage::getCStructureForInputs() 
    {
		mstp_msg_br_config *br_config = (mstp_msg_br_config*) calloc(1, sizeof(mstp_msg_br_config));
		if (br_config == NULL) return NULL;
		br_config->cmd = m_cmdCode;
		br_config->mode = m_mode;
		br_config->priority = m_priority;
		br_config->fwd_delay = m_forwardDelay;
		br_config->hello_time = m_helloTime;
		br_config->max_age = m_maxAge;
		br_config->revision = m_revision;
		br_config->max_hop = m_maxHop;
		br_config->err_timer_interval = m_errDisableInterval;
		br_config->hold_count = m_txHoldCount;
		br_config->aging_time = m_agingTime;
		br_config->cisco_interop = m_ciscoInterop;
		br_config->mac_reduction = m_macReduction;
		br_config->stp_enable = !m_stpDisable;
		br_config->err_disable = m_errDisableEnable;
		br_config->po_cost_standard = m_poPathcost;
		strncpy(br_config->desc, m_desc.data(), MSTP_DCM_MAX_STR);
		strncpy(br_config->region, m_region.data(), MSTP_DCM_MAX_STR);

		for (UI32 i = 0; i < 6; ++i) br_config->br_mac[i] = m_vcsBridgeMac[i];

        return (br_config);
    }

}
