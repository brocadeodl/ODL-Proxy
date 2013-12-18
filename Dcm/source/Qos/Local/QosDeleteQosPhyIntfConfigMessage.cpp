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
#include "Qos/Local/QosDeleteQosPhyIntfConfigMessage.h"
#include "Qos/Local/QosTypes.h"

namespace DcmNs
{

    QosDeleteQosPhyIntfConfigMessage::QosDeleteQosPhyIntfConfigMessage ()
        : ManagementInterfaceMessage (QosLocalObjectManager::getClassName (),QOSDELETEQOSPHYINTFCONFIG)
    {
    }

    QosDeleteQosPhyIntfConfigMessage::QosDeleteQosPhyIntfConfigMessage (const string &ifName,const UI64 &ifIndex,const string &id,const string &cosToCosMutationMapName,const string &CosToTrafficClassMapName,const string &ceeMapName,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &flowControlPfcCosValue,const SI32 &defaultCosValue,const string &qosTrust,const SI32 &opCode)
        : ManagementInterfaceMessage (QosLocalObjectManager::getClassName (),QOSDELETEQOSPHYINTFCONFIG),
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

    QosDeleteQosPhyIntfConfigMessage::~QosDeleteQosPhyIntfConfigMessage ()
    {
    }

    void  QosDeleteQosPhyIntfConfigMessage::setupAttributesForSerialization()
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

    void  QosDeleteQosPhyIntfConfigMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  QosDeleteQosPhyIntfConfigMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setId(const string &id)
    {
        m_id  =  id;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getId() const
    {
        return (m_id);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setCosToCosMutationMapName(const string &cosToCosMutationMapName)
    {
        m_cosToCosMutationMapName  =  cosToCosMutationMapName;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getCosToCosMutationMapName() const
    {
        return (m_cosToCosMutationMapName);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setCosToTrafficClassMapName(const string &CosToTrafficClassMapName)
    {
        m_CosToTrafficClassMapName  =  CosToTrafficClassMapName;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getCosToTrafficClassMapName() const
    {
        return (m_CosToTrafficClassMapName);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  QosDeleteQosPhyIntfConfigMessage::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  QosDeleteQosPhyIntfConfigMessage::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setFlowControlPfcCosValue(const SI32 &flowControlPfcCosValue)
    {
        m_flowControlPfcCosValue  =  flowControlPfcCosValue;
    }

    SI32  QosDeleteQosPhyIntfConfigMessage::getFlowControlPfcCosValue() const
    {
        return (m_flowControlPfcCosValue);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  QosDeleteQosPhyIntfConfigMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setQosTrust(const string &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    string  QosDeleteQosPhyIntfConfigMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  QosDeleteQosPhyIntfConfigMessage::setOpCode(const SI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    SI32  QosDeleteQosPhyIntfConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

}
