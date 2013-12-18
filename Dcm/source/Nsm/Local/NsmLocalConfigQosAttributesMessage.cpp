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
#include "Nsm/Local/NsmLocalConfigQosAttributesMessage.h"
#include "Nsm/Local/NsmTypes.h"
#include "Framework/Attributes/AttributeEnum.h"
#include "ClientInterface/InterfaceCommon.h"

namespace DcmNs
{

    NsmLocalConfigQosAttributesMessage::NsmLocalConfigQosAttributesMessage() :
	  PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMLOCALCONFIGQOSATTRIBUTES)
    {
		m_OpCode = 0;
		m_cmdCode = 0;
		m_defaultCosValue = 0;
		m_flowControlTx = 0;
		m_flowControlRx = 0;
		m_qosTrust = false;
		m_ifType = IF_TYPE_INVALID;
		m_distance = LONG_DISTANCE_200;
		m_qosDscpTrust = false;
    }

    NsmLocalConfigQosAttributesMessage::NsmLocalConfigQosAttributesMessage(const string &ifName, const SI32 &flowControlTx,const SI32 &flowControlRx,
		const bool &qosTrust, const bool &qosDscpTrust, const SI32 &defaultCosValue,const SI32 &cmdCode,
		const SI32 &OpCode) : PrismMessage (NsmLocalObjectManager::getPrismServiceId(),NSMLOCALCONFIGQOSATTRIBUTES),
        m_ifName    (ifName),
        m_flowControlTx    (flowControlTx),
        m_flowControlRx    (flowControlRx),
        m_qosTrust    (qosTrust),
		m_qosDscpTrust (qosDscpTrust),
        m_defaultCosValue    (defaultCosValue),
        m_cmdCode    (cmdCode),
        m_OpCode    (OpCode)
    {
		m_ifType = IF_TYPE_INVALID;
		m_distance = LONG_DISTANCE_200;
    }

    NsmLocalConfigQosAttributesMessage::~NsmLocalConfigQosAttributesMessage()
    {
    }

    void  NsmLocalConfigQosAttributesMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlTx,"flowControlTx"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlRx,"flowControlRx"));
        addSerializableAttribute (new AttributeBool(&m_qosTrust,"qosTrust"));
		addSerializableAttribute (new AttributeBool(&m_qosTrust,"qosDscpTrust"));
        addSerializableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeSI32(&m_OpCode,"OpCode"));
		addSerializableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_cosToTrafficClassMapName,"cosToTrafficClassMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToDscpMutationMapName,"dscpToDscpMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_dscpToTrafficClassMapName,"dscpToTrafficClassMapName"));
		addSerializableAttribute (new AttributeString(&m_dscpToCosMapName,"dscpToCosMapName"));
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeUI32(&m_ifType, "ifType"));
		addSerializableAttribute (new AttributeEnum((UI32 *)(&m_distance), "Distance"));
    }

    void  NsmLocalConfigQosAttributesMessage::setIfType(const UI32 &ifType)
    {
        m_ifType =  ifType;
    }

    UI32  NsmLocalConfigQosAttributesMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  NsmLocalConfigQosAttributesMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  NsmLocalConfigQosAttributesMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  NsmLocalConfigQosAttributesMessage::setFlowControlTx(const SI32 &flowControlTx)
    {
        m_flowControlTx  =  flowControlTx;
    }

    SI32  NsmLocalConfigQosAttributesMessage::getFlowControlTx() const
    {
        return (m_flowControlTx);
    }

    void  NsmLocalConfigQosAttributesMessage::setFlowControlRx(const SI32 &flowControlRx)
    {
        m_flowControlRx  =  flowControlRx;
    }

    SI32  NsmLocalConfigQosAttributesMessage::getFlowControlRx() const
    {
        return (m_flowControlRx);
    }

    void  NsmLocalConfigQosAttributesMessage::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  NsmLocalConfigQosAttributesMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  NsmLocalConfigQosAttributesMessage::setQosDscpTrust(const bool &qosDscpTrust)
    {
        m_qosDscpTrust  =  qosDscpTrust;
    }

    bool  NsmLocalConfigQosAttributesMessage::getQosDscpTrust() const
    {
        return (m_qosDscpTrust);
    }

    void  NsmLocalConfigQosAttributesMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  NsmLocalConfigQosAttributesMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  NsmLocalConfigQosAttributesMessage::setCmdCode(const SI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    SI32  NsmLocalConfigQosAttributesMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  NsmLocalConfigQosAttributesMessage::setOpCode(const SI32 &OpCode)
    {
        m_OpCode  =  OpCode;
    }

    SI32  NsmLocalConfigQosAttributesMessage::getOpCode() const
    {
        return (m_OpCode);
    }
	void  NsmLocalConfigQosAttributesMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  NsmLocalConfigQosAttributesMessage::setCosToTrafficClassMapName(const string &cosToTrafficClassMapName)
    {
        m_cosToTrafficClassMapName  =  cosToTrafficClassMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getCosToTrafficClassMapName() const
    {
        return (m_cosToTrafficClassMapName);
    }

	 void  NsmLocalConfigQosAttributesMessage::setDscpToDscpMutationMapName(const string &dscpToDscpMutationMapName)
    {
        m_dscpToDscpMutationMapName  =  dscpToDscpMutationMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getDscpToDscpMutationMapName() const
    {
        return (m_dscpToDscpMutationMapName);
    }

    void  NsmLocalConfigQosAttributesMessage::setDscpToTrafficClassMapName(const string &dscpToTrafficClassMapName)
    {
        m_dscpToTrafficClassMapName  =  dscpToTrafficClassMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getDscpToTrafficClassMapName() const
    {
        return (m_dscpToTrafficClassMapName);
    }

	void  NsmLocalConfigQosAttributesMessage::setDscpToCosMapName(const string &dscpToCosMapName)
    {
        m_dscpToCosMapName  =  dscpToCosMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getDscpToCosMapName() const
    {
        return (m_dscpToCosMapName);
    }

    void  NsmLocalConfigQosAttributesMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  NsmLocalConfigQosAttributesMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

	void NsmLocalConfigQosAttributesMessage::setDistance(const LongDistance &distance)
	{
		m_distance = distance;
	}

	LongDistance NsmLocalConfigQosAttributesMessage::getDistance() const
	{
		return (m_distance);
	}
}
