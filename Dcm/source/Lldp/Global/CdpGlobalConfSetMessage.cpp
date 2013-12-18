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
 *   Author : mmohan                                                       *
 **************************************************************************/

#include "Lldp/Global/CdpGlobalConfSetMessage.h"
#include "Lldp/Global/LldpGlobalObjectManager.h"
#include "Lldp/Global/LldpGlobalTypes.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Utils/TraceUtils.h"

namespace DcmNs
{

    CdpGlobalConfSetMessage::CdpGlobalConfSetMessage ()
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),CDPGLOBALCONFSET)
    {
        m_mgmtFlags = 0;
        m_cmdcode = 0;
        m_msgType = 0;
        m_protoEnable = false;

        trace (TRACE_LEVEL_INFO,"CdpGlobalConfSetMessage::Constructor"
                                          "Entering..");
    }

    CdpGlobalConfSetMessage::CdpGlobalConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const bool &protoEnable)
        : ManagementInterfaceMessage (LldpGlobalObjectManager::getClassName (),CDPGLOBALCONFSET),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_protoEnable    (protoEnable)
    {	
        m_mgmtFlags = 0;
        m_cmdcode = 0;
        m_msgType = 0;
        m_protoEnable = false;

		trace (TRACE_LEVEL_INFO,"CdpGlobalConfSetMessage::Constructor"
			                                          "Entering..");
    }

    CdpGlobalConfSetMessage::~CdpGlobalConfSetMessage ()
    {
		trace (TRACE_LEVEL_INFO,"CdpGlobalConfSetMessage::Destructor"
                                          "Entering..");
    }

    void  CdpGlobalConfSetMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeBool(&m_protoEnable,"protoEnable"));
    }

    void  CdpGlobalConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  CdpGlobalConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  CdpGlobalConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  CdpGlobalConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  CdpGlobalConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  CdpGlobalConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  CdpGlobalConfSetMessage::setProtoEnable(const bool &protoEnable)
    {
        m_protoEnable  =  protoEnable;
    }

    bool  CdpGlobalConfSetMessage::getProtoEnable() const
    {
        return (m_protoEnable);
    }

}
