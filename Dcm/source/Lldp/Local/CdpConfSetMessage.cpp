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
 *   Author : mmohan                                                     *
 **************************************************************************/

#include "Lldp/Local/CdpConfSetMessage.h"
#include "Lldp/Local/LldpLocalObjectManager.h"
#include "Lldp/Local/LldpTypes.h"
#include "Utils/DcmMemUtil.h"
#include "Framework/Utils/TraceUtils.h"
namespace DcmNs
{

    CdpConfSetMessage::CdpConfSetMessage ()
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),CDPCONFSET)
    {
        m_mgmtFlags = 0;
        m_cmdcode = 0;
        m_msgType = 0;
        m_protoEnable = false;
        
        trace (TRACE_LEVEL_INFO,"CdpConfSetMessage::Constructor"
                                          "Entering..");
    }

    CdpConfSetMessage::CdpConfSetMessage (const UI32 &mgmtFlags,const UI32 &cmdcode,const UI32 &msgType,const bool &protoEnable)
        : PrismMessage (LldpLocalObjectManager::getPrismServiceId (),CDPCONFSET),
        m_mgmtFlags    (mgmtFlags),
        m_cmdcode    (cmdcode),
        m_msgType    (msgType),
        m_protoEnable    (protoEnable)
    {	
        m_mgmtFlags = 0;
        m_cmdcode = 0;
        m_msgType = 0;
        m_protoEnable = false;

		trace (TRACE_LEVEL_INFO,"CdpConfSetMessage::Constructor"
			                                          "Entering..");
    }

    CdpConfSetMessage::~CdpConfSetMessage ()
    {
		trace (TRACE_LEVEL_INFO,"CdpConfSetMessage::Destructor"
                                          "Entering..");
    }

    void  CdpConfSetMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_mgmtFlags,"mgmtFlags"));
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeBool(&m_protoEnable,"protoEnable"));
    }

    void  CdpConfSetMessage::setMgmtFlags(const UI32 &mgmtFlags)
    {
        m_mgmtFlags  =  mgmtFlags;
    }

    UI32  CdpConfSetMessage::getMgmtFlags() const
    {
        return (m_mgmtFlags);
    }

    void  CdpConfSetMessage::setCmdcode(const UI32 &cmdcode)
    {
        m_cmdcode  =  cmdcode;
    }

    UI32  CdpConfSetMessage::getCmdcode() const
    {
        return (m_cmdcode);
    }

    void  CdpConfSetMessage::setMsgType(const UI32 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI32  CdpConfSetMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  CdpConfSetMessage::setProtoEnable(const bool &protoEnable)
    {
        m_protoEnable  =  protoEnable;
    }

    bool  CdpConfSetMessage::getProtoEnable() const
    {
        return (m_protoEnable);
    }

}
