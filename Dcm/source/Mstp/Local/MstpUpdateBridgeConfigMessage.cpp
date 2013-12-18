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

#include "Mstp/Global/MstpGlobalUpdateBridgeConfigMessage.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpUpdateBridgeConfigMessage.h"
#include "Mstp/Local/MstpTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    MstpUpdateBridgeConfigMessage::MstpUpdateBridgeConfigMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEBRIDGECONFIG)
        , m_vcsBridgeMac ((UI8*) "\0\0\0\0\0\0")
    {
		m_priority = 0;
		m_forwardDelay = 0;
		m_helloTime = 0;
		m_maxAge = 0;
		m_maxHop = 0;
		m_desc = "";
		m_errDisableInterval = 0;
		m_txHoldCount = 0;
		m_revision = 0;
		m_region = "";
		m_cmdCode = 0;
		m_mode = 0;
		m_ciscoInterop=MSTP_DCM_DEFAULT_CISCO_INTEROP ;
		m_poPathcost=MSTP_DCM_DEFAULT_PATHCOST;
		m_errDisableEnable=MSTP_DCM_DEFAULT_ERR_TIMEOUT;
		m_stpDisable=MSTP_DCM_DEFUALT_PROTO_STATUS;
    }

    MstpUpdateBridgeConfigMessage::MstpUpdateBridgeConfigMessage (MstpGlobalUpdateBridgeConfigMessage & global)
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPUPDATEBRIDGECONFIG)
        , m_vcsBridgeMac ((UI8*) "\0\0\0\0\0\0")
    {
		m_priority = global.m_priority;
		m_forwardDelay = global.m_forwardDelay;
		m_helloTime = global.m_helloTime;
		m_maxAge = global.m_maxAge;
		m_maxHop = global.m_maxHop;
		m_desc = global.m_desc;
		m_errDisableInterval = global.m_errDisableInterval;
		m_txHoldCount = global.m_txHoldCount;
		m_revision = global.m_revision;
		m_region = global.m_region;
		m_cmdCode = global.m_cmdCode;
		m_mode = global.m_mode;
                m_errDisableEnable  = MSTP_DCM_DEFAULT_ERR_TIMEOUT;
                m_poPathcost    = MSTP_DCM_DEFAULT_PATHCOST;
                m_stpDisable    = MSTP_DCM_DEFUALT_PROTO_STATUS;
                m_ciscoInterop  = MSTP_DCM_DEFAULT_CISCO_INTEROP;
    }

    MstpUpdateBridgeConfigMessage::~MstpUpdateBridgeConfigMessage ()
    {
    }

    void  MstpUpdateBridgeConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization();
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
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeUI32(&m_mode,"mode"));
        addSerializableAttribute (new AttributeMacAddress(&m_vcsBridgeMac, "vcsBridgeMac"));
    }

    void  MstpUpdateBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpUpdateBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpUpdateBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpUpdateBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpUpdateBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpUpdateBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpUpdateBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpUpdateBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpUpdateBridgeConfigMessage::setMaxHop(const UI32 &maxHop)
    {
        m_maxHop  =  maxHop;
    }

    UI32  MstpUpdateBridgeConfigMessage::getMaxHop() const
    {
        return (m_maxHop);
    }

    void  MstpUpdateBridgeConfigMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  MstpUpdateBridgeConfigMessage::getDesc() const
    {
        return (m_desc);
    }

    void  MstpUpdateBridgeConfigMessage::setErrDisableInterval(const UI32 &errDisableInterval)
    {
        m_errDisableInterval  =  errDisableInterval;
    }

    UI32  MstpUpdateBridgeConfigMessage::getErrDisableInterval() const
    {
        return (m_errDisableInterval);
    }

    void  MstpUpdateBridgeConfigMessage::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  MstpUpdateBridgeConfigMessage::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  MstpUpdateBridgeConfigMessage::setRevision(const UI16 &revision)
    {
        m_revision  =  revision;
    }

    UI16  MstpUpdateBridgeConfigMessage::getRevision() const
    {
        return (m_revision);
    }

    void  MstpUpdateBridgeConfigMessage::setRegion(const string &region)
    {
        m_region  =  region;
    }

    string  MstpUpdateBridgeConfigMessage::getRegion() const
    {
        return (m_region);
    }

    void  MstpUpdateBridgeConfigMessage::setCiscoInterop(const MstpCiscoInterOp &ciscoInterop)
    {
        m_ciscoInterop  =  ciscoInterop;
    }

    MstpCiscoInterOp  MstpUpdateBridgeConfigMessage::getCiscoInterop() const
    {
        return (m_ciscoInterop);
    }


    void  MstpUpdateBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpUpdateBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpUpdateBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpUpdateBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  MstpUpdateBridgeConfigMessage::setErrDisableEnable(const bool &errDisableEnable)
    {
        m_errDisableEnable  =  errDisableEnable;
    }

    bool  MstpUpdateBridgeConfigMessage::getErrDisableEnable() const
    {
        return (m_errDisableEnable);
    }


    void  MstpUpdateBridgeConfigMessage::setPoPathcost(const MstpPoPathcost &poPathcost)
    {
        m_poPathcost  =  poPathcost;
    }

    MstpPoPathcost  MstpUpdateBridgeConfigMessage::getPoPathcost() const
    {
        return (m_poPathcost);
    }

    void  MstpUpdateBridgeConfigMessage::setStpDisable(const bool &stpDisable)
    {
        m_stpDisable  =  stpDisable;
    }

    bool  MstpUpdateBridgeConfigMessage::getStpDisable() const
    {
        return (m_stpDisable);
    }

    void MstpUpdateBridgeConfigMessage::setVcsBridgeMac(const MacAddress &mac)
    {
        m_vcsBridgeMac = mac;
    }

    const MacAddress &MstpUpdateBridgeConfigMessage::getVcsBridgeMac() const
    {
        return m_vcsBridgeMac;
    }

    void  MstpUpdateBridgeConfigMessage::printMsg()
    {
		tracePrintf(TRACE_LEVEL_DEBUG, "MstpUpdateBridgeConfigMessage::printMsg()");
		tracePrintf(TRACE_LEVEL_DEBUG, " m_mode         : %u", m_mode);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_cmdCode      : %u", m_cmdCode);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_priority     : %u", m_priority);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_forwardDelay : %u", m_forwardDelay);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_helloTime    : %u", m_helloTime);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_maxAge       : %u", m_maxAge);
        tracePrintf(TRACE_LEVEL_DEBUG, " m_maxHop       : %u", m_maxHop);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_revision     : %u", (UI32) m_revision);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_region       : %s", m_region.c_str());
		tracePrintf(TRACE_LEVEL_DEBUG, " m_desc         : %s", m_desc.c_str());
		tracePrintf(TRACE_LEVEL_DEBUG, " m_txHoldCount  : %u", m_txHoldCount);
        tracePrintf(TRACE_LEVEL_DEBUG, " m_stpDisable   : %d", m_stpDisable);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_errDisableInterval: %d", m_errDisableInterval);
		tracePrintf(TRACE_LEVEL_DEBUG, " m_vcsBridgeMac : %s", m_vcsBridgeMac.toString2().c_str());
    }


}
