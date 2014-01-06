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
#include "Nsm/Local/NsmQosPOConfigSPMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmQosPOConfigSPMessage::NsmQosPOConfigSPMessage()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMQOSPOCONFIGSP)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
		m_defaultCosValue =0;
		m_flowControlTx = 0;
		m_flowControlRx = 0;
		m_poId = 0;
		m_qosTrust = false;
		m_qosDscpTrust = false;
    }

    NsmQosPOConfigSPMessage::NsmQosPOConfigSPMessage(const UI32 &poId,const UI32 &flowControlTx,const UI32 &flowControlRx,const bool &qosTrust,const bool &qosDscpTrust, const SI32 &defaultCosValue,const SI32 &cmdCode,const SI32 &OpCode, const string &qosPolicyMapName)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMQOSPOCONFIGSP),
        m_poId    (poId),
        m_flowControlTx    (flowControlTx),
        m_flowControlRx    (flowControlRx),
        m_qosTrust    (qosTrust),
		m_qosDscpTrust 	(qosDscpTrust),
        m_defaultCosValue    (defaultCosValue),
        m_cmdCode    (cmdCode),
        m_OpCode    (OpCode),
        m_qosPolicyMapName	(qosPolicyMapName)
    {
    }

    NsmQosPOConfigSPMessage::~NsmQosPOConfigSPMessage()
    {
    }

    void  NsmQosPOConfigSPMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_poId,"poId"));
        addSerializableAttribute (new AttributeUI32(&m_flowControlTx,"flowControlTx"));
        addSerializableAttribute (new AttributeUI32(&m_flowControlRx,"flowControlRx"));
        addSerializableAttribute (new AttributeBool(&m_qosTrust,"qosTrust"));
        addSerializableAttribute (new AttributeBool(&m_qosDscpTrust, "qosDscpTrust"));
		addSerializableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
		addSerializableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
   		addSerializableAttribute (new AttributeString(&m_dscpToDscpMutationMapName,"dscpToDscpMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToCosMapName,"dscpToCosMapName"));     
		addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
		addSerializableAttribute (new AttributeString(&m_qosPolicyMapName,"qosPolicyMapName"));

    }

    void  NsmQosPOConfigSPMessage::setPoId(const UI32 &poId)
    {
        m_poId  =  poId;
    }

    UI32  NsmQosPOConfigSPMessage::getPoId() const
    {
        return (m_poId);
    }

    void  NsmQosPOConfigSPMessage::setFlowControlTx(const UI32 &flowControlTx)
    {
        m_flowControlTx  =  flowControlTx;
    }

    UI32  NsmQosPOConfigSPMessage::getFlowControlTx() const
    {
        return (m_flowControlTx);
    }

    void  NsmQosPOConfigSPMessage::setFlowControlRx(const UI32 &flowControlRx)
    {
        m_flowControlRx  =  flowControlRx;
    }

    UI32  NsmQosPOConfigSPMessage::getFlowControlRx() const
    {
        return (m_flowControlRx);
    }

    void  NsmQosPOConfigSPMessage::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  NsmQosPOConfigSPMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

	void  NsmQosPOConfigSPMessage::setQosDscpTrust(const bool &qosDscpTrust)
    {
        m_qosDscpTrust  =  qosDscpTrust;
    }

    bool  NsmQosPOConfigSPMessage::getQosDscpTrust() const
    {
        return (m_qosDscpTrust);
    }

    void  NsmQosPOConfigSPMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  NsmQosPOConfigSPMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  NsmQosPOConfigSPMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmQosPOConfigSPMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmQosPOConfigSPMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  NsmQosPOConfigSPMessage::getOpCode() const
    {
        return (m_OpCode);
    }

	 void  NsmQosPOConfigSPMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  NsmQosPOConfigSPMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  NsmQosPOConfigSPMessage::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  NsmQosPOConfigSPMessage::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }
	
	void  NsmQosPOConfigSPMessage::setDscpToDscpMutationMapName(const string &dscpToDscpMutationMapName)
    {
        m_dscpToDscpMutationMapName  =  dscpToDscpMutationMapName;
    }

    string  NsmQosPOConfigSPMessage::getDscpToDscpMutationMapName() const
    {
        return (m_dscpToDscpMutationMapName);
    }

    void  NsmQosPOConfigSPMessage::setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  NsmQosPOConfigSPMessage::getDscpToTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

    void  NsmQosPOConfigSPMessage::setDscpToCosMapName(const string &dscpToCosMapName)
    {
        m_dscpToCosMapName  =  dscpToCosMapName;
    }

    string  NsmQosPOConfigSPMessage::getDscpToCosMapName() const
    {
        return (m_dscpToCosMapName);
    }

    void  NsmQosPOConfigSPMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  NsmQosPOConfigSPMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

	void NsmQosPOConfigSPMessage::setQosPolicyMapName(const string &qosPolicyMapName) {
		m_qosPolicyMapName = qosPolicyMapName;
	}
	string NsmQosPOConfigSPMessage::getQosPolicyMapName() const {
		return m_qosPolicyMapName;
	}
}