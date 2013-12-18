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
#include "Nsm/Local/NsmQosPfcConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmQosPfcConfigSPMessage::NsmQosPfcConfigSPMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSPFCCONFIGSP)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
		m_pfcFlowControlCos = 0;
		m_pfcFlowControlTx = 0;
		m_pfcFlowControlRx = 0;
		m_poId = 0;
    }

    NsmQosPfcConfigSPMessage::NsmQosPfcConfigSPMessage (const SI32 &pfcFlowControlCos,const SI32 &pfcFlowControlTx,const SI32 &pfcFlowControlRx)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMQOSPFCCONFIGSP),
        m_pfcFlowControlCos    (pfcFlowControlCos),
        m_pfcFlowControlTx    (pfcFlowControlTx),
        m_pfcFlowControlRx    (pfcFlowControlRx)
    {
		m_poId = 0;
		m_OpCode = 0;
		m_cmdCode = 0;
    }

    NsmQosPfcConfigSPMessage::~NsmQosPfcConfigSPMessage ()
    {
    }

    void  NsmQosPfcConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
        addSerializableAttribute (new AttributeSI32(&m_pfcFlowControlCos,"pfcFlowControlCos"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlTx,"pfcFlowControlTx"));
        addSerializableAttribute (new AttributeUI32(&m_pfcFlowControlRx,"pfcFlowControlRx"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
    }

    void  NsmQosPfcConfigSPMessage::setPfcFlowControlCos(const SI32 &pfcFlowControlCos)
    {
        m_pfcFlowControlCos  =  pfcFlowControlCos;
    }

    SI32  NsmQosPfcConfigSPMessage::getPfcFlowControlCos() const
    {
        return (m_pfcFlowControlCos);
    }

    void  NsmQosPfcConfigSPMessage::setPfcFlowControlTx(const UI32 &pfcFlowControlTx)
    {
        m_pfcFlowControlTx  =  pfcFlowControlTx;
    }

    UI32  NsmQosPfcConfigSPMessage::getPfcFlowControlTx() const
    {
        return (m_pfcFlowControlTx);
    }

    void  NsmQosPfcConfigSPMessage::setPfcFlowControlRx(const UI32 &pfcFlowControlRx)
    {
        m_pfcFlowControlRx  =  pfcFlowControlRx;
    }

    UI32  NsmQosPfcConfigSPMessage::getPfcFlowControlRx() const
    {
        return (m_pfcFlowControlRx);
    }

    void  NsmQosPfcConfigSPMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmQosPfcConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmQosPfcConfigSPMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  NsmQosPfcConfigSPMessage::getOpCode() const
    {
        return (m_OpCode);
    }

	 void  NsmQosPfcConfigSPMessage::setPoId(const UI32 &poId)
    {
        m_poId =  poId;
    }

    UI32  NsmQosPfcConfigSPMessage::getPoId() const
    {
        return (m_poId);
    }


}
