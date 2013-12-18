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
#include "Mstp/Global/MstpGlobalObjectManager.h"
#include "Mstp/Global/MstpGlobalTypes.h"
#include "Framework/Utils/TraceUtils.h"
#include "ClientInterface/Mstp/MstpToolKit.h"

namespace DcmNs
{

    MstpGlobalUpdateBridgeConfigMessage::MstpGlobalUpdateBridgeConfigMessage ()
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEBRIDGECONFIG)
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
    }

    MstpGlobalUpdateBridgeConfigMessage::MstpGlobalUpdateBridgeConfigMessage
(const UI32 &priority,const UI32 &forwardDelay,const UI32 &helloTime,const UI32
&maxAge,const UI32 &maxHop,const string &desc,const UI32 &errDisableInterval,const UI32 &txHoldCount,const UI16 &revision,const string &region,const UI32 &cmdCode, const UI32 &mode)
        : ManagementInterfaceMessage (MstpGlobalObjectManager::getClassName (),MSTPGLOBALUPDATEBRIDGECONFIG),
        m_priority    (priority),
        m_forwardDelay    (forwardDelay),
        m_helloTime    (helloTime),
        m_maxAge    (maxAge),
        m_maxHop    (maxHop),
        m_desc    (desc),
        m_errDisableInterval    (errDisableInterval),
        m_txHoldCount    (txHoldCount),
        m_revision    (revision),
        m_region    (region),
        m_cmdCode    (cmdCode),
        m_mode    (mode)
    {
    }

    MstpGlobalUpdateBridgeConfigMessage::~MstpGlobalUpdateBridgeConfigMessage ()
    {
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
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
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setPriority(const UI32 &priority)
    {
        m_priority  =  priority;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getPriority() const
    {
        return (m_priority);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setForwardDelay(const UI32 &forwardDelay)
    {
        m_forwardDelay  =  forwardDelay;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getForwardDelay() const
    {
        return (m_forwardDelay);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setHelloTime(const UI32 &helloTime)
    {
        m_helloTime  =  helloTime;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getHelloTime() const
    {
        return (m_helloTime);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setMaxAge(const UI32 &maxAge)
    {
        m_maxAge  =  maxAge;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getMaxAge() const
    {
        return (m_maxAge);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setMaxHop(const UI32 &maxHop)
    {
        m_maxHop  =  maxHop;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getMaxHop() const
    {
        return (m_maxHop);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  MstpGlobalUpdateBridgeConfigMessage::getDesc() const
    {
        return (m_desc);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setErrDisableInterval(const UI32 &errDisableInterval)
    {
        m_errDisableInterval  =  errDisableInterval;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getErrDisableInterval() const
    {
        return (m_errDisableInterval);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setTxHoldCount(const UI32 &txHoldCount)
    {
        m_txHoldCount  =  txHoldCount;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getTxHoldCount() const
    {
        return (m_txHoldCount);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setRevision(const UI16 &revision)
    {
        m_revision  =  revision;
    }

    UI16  MstpGlobalUpdateBridgeConfigMessage::getRevision() const
    {
        return (m_revision);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setRegion(const string &region)
    {
        m_region  =  region;
    }

    string  MstpGlobalUpdateBridgeConfigMessage::getRegion() const
    {
        return (m_region);
    }


    void  MstpGlobalUpdateBridgeConfigMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::setMode(const UI32 &mode)
    {
        m_mode  =  mode;
    }

    UI32  MstpGlobalUpdateBridgeConfigMessage::getMode() const
    {
        return (m_mode);
    }

    void  MstpGlobalUpdateBridgeConfigMessage::printMsg()
    {
		trace(TRACE_LEVEL_DEBUG, string("MstpGlobalUpdateBridgeConfigMessage::printMsg()"));
		trace(TRACE_LEVEL_DEBUG, string("m_mode:			")+toString(m_mode));
		trace(TRACE_LEVEL_DEBUG, string("m_cmdCode:		")+toString(m_cmdCode));
		trace(TRACE_LEVEL_DEBUG, string("m_priority:		")+toString(m_priority));
		trace(TRACE_LEVEL_DEBUG, string("m_forwardDelay:	")+toString(m_forwardDelay));
		trace(TRACE_LEVEL_DEBUG, string("m_helloTime:	")+toString(m_helloTime));
		trace(TRACE_LEVEL_DEBUG, string("m_maxAge:		")+toString(m_maxAge));
		trace(TRACE_LEVEL_DEBUG, string("m_maxHop:		")+toString(m_maxHop));
		trace(TRACE_LEVEL_DEBUG, string("m_revision:	")+toString(UI32(m_revision)));
		trace(TRACE_LEVEL_DEBUG, string("m_region:			")+m_region);
		trace(TRACE_LEVEL_DEBUG, string("m_desc:			")+m_desc);
		trace(TRACE_LEVEL_DEBUG, string("m_txHoldCount:	")+toString(m_txHoldCount));
		trace(TRACE_LEVEL_DEBUG, string("m_errDisableInterval: ")+toString(m_errDisableInterval));
#if 0
		cout<< endl<<"MstpGlobalUpdateBridgeConfigMessage::printMsg()"<<endl;
		cout<< "		m_mode = "<<m_mode <<endl;
		cout<< "		m_cmdCode = "<<m_cmdCode <<endl;
		cout<< "		m_priority = "<<m_priority <<endl;
		cout<< "		m_forwardDelay = "<<m_forwardDelay <<endl;
		cout<< "		m_helloTime = "<<m_helloTime <<endl;
		cout<< "		m_maxAge = "<<m_maxAge <<endl;
		cout<< "		m_des = "<<m_desc <<endl;
		cout<< "		m_errDisableInterval = "<<m_errDisableInterval <<endl;
		cout<< "		m_txHoldCount = "<<m_txHoldCount <<endl;
#endif
    }

}
