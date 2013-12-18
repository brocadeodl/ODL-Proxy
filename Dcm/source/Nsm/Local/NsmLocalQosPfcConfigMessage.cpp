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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/NsmLocalQosPfcConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmLocalQosPfcConfigMessage::NsmLocalQosPfcConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSPFCCONFIG)
    {
		m_OpCode = 0;
		m_pfcFlowControlCos = 0;
		m_cmdCode = 0;
		m_pfcFlowControlTx = 0;
		m_pfcFlowControlRx = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmLocalQosPfcConfigMessage::NsmLocalQosPfcConfigMessage (const SI32 &pfcFlowControlCos,const SI32 &pfcFlowControlTx,const SI32 &pfcFlowControlRx)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSPFCCONFIG),
        m_pfcFlowControlCos    (pfcFlowControlCos),
        m_pfcFlowControlTx    (pfcFlowControlTx),
        m_pfcFlowControlRx    (pfcFlowControlRx)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
		m_ifType = IF_TYPE_INVALID;
    }

    NsmLocalQosPfcConfigMessage::~NsmLocalQosPfcConfigMessage ()
    {
    }

    void  NsmLocalQosPfcConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_pfcFlowControlCos,"pfcFlowControlCos"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlTx,"pfcFlowControlTx"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlRx,"pfcFlowControlRx"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
    }

    void NsmLocalQosPfcConfigMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmLocalQosPfcConfigMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmLocalQosPfcConfigMessage::setPfcFlowControlCos(const SI32 &pfcFlowControlCos)
    {
        m_pfcFlowControlCos  =  pfcFlowControlCos;
    }

    SI32  NsmLocalQosPfcConfigMessage::getPfcFlowControlCos() const
    {
        return (m_pfcFlowControlCos);
    }

    void  NsmLocalQosPfcConfigMessage::setPfcFlowControlTx(const UI32 &pfcFlowControlTx)
    {
        m_pfcFlowControlTx  =  pfcFlowControlTx;
    }

    UI32  NsmLocalQosPfcConfigMessage::getPfcFlowControlTx() const
    {
        return (m_pfcFlowControlTx);
    }

    void  NsmLocalQosPfcConfigMessage::setPfcFlowControlRx(const UI32 &pfcFlowControlRx)
    {
        m_pfcFlowControlRx  =  pfcFlowControlRx;
    }

    UI32  NsmLocalQosPfcConfigMessage::getPfcFlowControlRx() const
    {
        return (m_pfcFlowControlRx);
    }

    void  NsmLocalQosPfcConfigMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmLocalQosPfcConfigMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmLocalQosPfcConfigMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  NsmLocalQosPfcConfigMessage::getOpCode() const
    {
        return (m_OpCode);
    }

    void  NsmLocalQosPfcConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLocalQosPfcConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

}
