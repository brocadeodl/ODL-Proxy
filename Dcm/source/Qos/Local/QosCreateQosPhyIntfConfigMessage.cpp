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

#include "Qos/Local/QosLocalObjectManager.h"
#include "Qos/Local/QosCreateQosPhyIntfConfigMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosCreateQosPhyIntfConfigMessage::QosCreateQosPhyIntfConfigMessage ()
        : ManagementInterfaceMessage (QosLocalObjectManager::getClassName (),QOSCREATEQOSPHYINTFCONFIG)
    {
    }

    QosCreateQosPhyIntfConfigMessage::QosCreateQosPhyIntfConfigMessage (const string &ifName,const UI64 &ifIndex,const string &id,const string &cosToCosMutationMapName,const string &CosToTrafficClassMapName,const string &ceeMapName,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &flowControlPfcCosValue,const SI32 &defaultCosValue,const string &qosTrust,const SI32 &opCode)
        : ManagementInterfaceMessage (QosLocalObjectManager::getClassName (),QOSCREATEQOSPHYINTFCONFIG),
        m_ifName    (ifName),
        m_ifIndex    (ifIndex),
        m_id    (id),
        m_cosToCosMutationMapName    (cosToCosMutationMapName),
        m_CosToTrafficClassMapName    (CosToTrafficClassMapName),
        m_ceeMapName    (ceeMapName),
        m_flowControlTxEnable    (flowControlTxEnable),
        m_flowControlRxEnable    (flowControlRxEnable),
        m_flowControlPfcCosValue    (flowControlPfcCosValue),
        m_defaultCosValue    (defaultCosValue),
        m_qosTrust    (qosTrust),
        m_opCode    (opCode)
    {
    }

    QosCreateQosPhyIntfConfigMessage::~QosCreateQosPhyIntfConfigMessage ()
    {
    }

    void  QosCreateQosPhyIntfConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeString(&m_id,"id"));
        addSerializableAttribute (new AttributeString(&m_cosToCosMutationMapName,"cosToCosMutationMapName"));
        addSerializableAttribute (new AttributeString(&m_CosToTrafficClassMapName,"CosToTrafficClassMapName"));
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlTxEnable,"flowControlTxEnable"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlRxEnable,"flowControlRxEnable"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlPfcCosValue,"flowControlPfcCosValue"));
        addSerializableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addSerializableAttribute (new AttributeString(&m_qosTrust,"qosTrust"));
        addSerializableAttribute (new AttributeSI32(&m_opCode,"opCode"));
    }

    void  QosCreateQosPhyIntfConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosCreateQosPhyIntfConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  QosCreateQosPhyIntfConfigMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  QosCreateQosPhyIntfConfigMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  QosCreateQosPhyIntfConfigMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  QosCreateQosPhyIntfConfigMessage::getId() const
    {
        return (m_id);
    }

    void  QosCreateQosPhyIntfConfigMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  QosCreateQosPhyIntfConfigMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  QosCreateQosPhyIntfConfigMessage::setCosToTrafficClassMapName(const string &CosToTrafficClassMapName)
    {
        m_CosToTrafficClassMapName  =  CosToTrafficClassMapName;
    }

    string  QosCreateQosPhyIntfConfigMessage::getCosToTrafficClassMapName() const
    {
        return (m_CosToTrafficClassMapName);
    }

    void  QosCreateQosPhyIntfConfigMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosCreateQosPhyIntfConfigMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosCreateQosPhyIntfConfigMessage::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  QosCreateQosPhyIntfConfigMessage::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  QosCreateQosPhyIntfConfigMessage::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  QosCreateQosPhyIntfConfigMessage::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  QosCreateQosPhyIntfConfigMessage::setFlowControlPfcCosValue(const SI32 &flowControlPfcCosValue)
    {
        m_flowControlPfcCosValue  =  flowControlPfcCosValue;
    }

    SI32  QosCreateQosPhyIntfConfigMessage::getFlowControlPfcCosValue() const
    {
        return (m_flowControlPfcCosValue);
    }

    void  QosCreateQosPhyIntfConfigMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  QosCreateQosPhyIntfConfigMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  QosCreateQosPhyIntfConfigMessage::setQosTrust(const string &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    string  QosCreateQosPhyIntfConfigMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  QosCreateQosPhyIntfConfigMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosCreateQosPhyIntfConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
