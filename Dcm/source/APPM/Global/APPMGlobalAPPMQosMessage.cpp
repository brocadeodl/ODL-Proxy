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
 *   Author : kverma                                                     *
 **************************************************************************/

#include "APPM/Global/APPMGlobalAPPMQosMessage.h"
#include "APPM/Global/APPMGlobalObjectManager.h"
#include "APPM/Global/APPMGlobalTypes.h"

namespace DcmNs
{

    APPMGlobalAPPMQosMessage::APPMGlobalAPPMQosMessage ()
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMQOS)
    {
    }

    APPMGlobalAPPMQosMessage::APPMGlobalAPPMQosMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const SI32 &defaultCosValue,const bool &qosTrust,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &pfcValue,const UI32 &txValue,const UI32 &rxValue,const string &ceeMapName,const string &cosToCosMutationMap,const string &cosToTrafficClassMap)
        : ManagementInterfaceMessage (APPMGlobalObjectManager::getClassName (),APPMGLOBALAPPMQOS),
        m_opCode    (opCode),
        m_cmdCode    (cmdCode),
        m_profileName    (profileName),
        m_profileId    (profileId),
        m_defaultCosValue    (defaultCosValue),
        m_qosTrust    (qosTrust),
        m_flowControlTxEnable    (flowControlTxEnable),
        m_flowControlRxEnable    (flowControlRxEnable),
        m_pfcValue    (pfcValue),
        m_txValue    (txValue),
        m_rxValue    (rxValue),
        m_ceeMapName    (ceeMapName),
        m_cosToCosMutationMap    (cosToCosMutationMap),
        m_cosToTrafficClassMap    (cosToTrafficClassMap)
    {
    }

    APPMGlobalAPPMQosMessage::~APPMGlobalAPPMQosMessage ()
    {
    }

    void  APPMGlobalAPPMQosMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_profileName,"profileName"));
        addSerializableAttribute (new AttributeUI32(&m_profileId,"profileId"));
        addSerializableAttribute (new AttributeSI32(&m_defaultCosValue,"defaultCosValue"));
        addSerializableAttribute (new AttributeBool(&m_qosTrust,"qosTrust"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlTxEnable,"flowControlTxEnable"));
        addSerializableAttribute (new AttributeSI32(&m_flowControlRxEnable,"flowControlRxEnable"));
        addSerializableAttribute (new AttributeSI32(&m_pfcValue,"pfcValue"));
        addSerializableAttribute (new AttributeUI32(&m_txValue,"txValue"));
        addSerializableAttribute (new AttributeUI32(&m_rxValue,"rxValue"));
        addSerializableAttribute (new AttributeString(&m_ceeMapName,"ceeMapName"));
        addSerializableAttribute (new AttributeString(&m_cosToCosMutationMap,"cosToCosMutationMap"));
        addSerializableAttribute (new AttributeString(&m_cosToTrafficClassMap,"cosToTrafficClassMap"));
    }

    void  APPMGlobalAPPMQosMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMGlobalAPPMQosMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMGlobalAPPMQosMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMGlobalAPPMQosMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMGlobalAPPMQosMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMGlobalAPPMQosMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMGlobalAPPMQosMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMGlobalAPPMQosMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMGlobalAPPMQosMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  APPMGlobalAPPMQosMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  APPMGlobalAPPMQosMessage::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  APPMGlobalAPPMQosMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  APPMGlobalAPPMQosMessage::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  APPMGlobalAPPMQosMessage::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  APPMGlobalAPPMQosMessage::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  APPMGlobalAPPMQosMessage::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  APPMGlobalAPPMQosMessage::setPfcValue(const SI32 &pfcValue)
    {
        m_pfcValue  =  pfcValue;
    }

    SI32  APPMGlobalAPPMQosMessage::getPfcValue() const
    {
        return (m_pfcValue);
    }

    void  APPMGlobalAPPMQosMessage::setTxValue(const UI32 &txValue)
    {
        m_txValue  =  txValue;
    }

    UI32  APPMGlobalAPPMQosMessage::getTxValue() const
    {
        return (m_txValue);
    }

    void  APPMGlobalAPPMQosMessage::setRxValue(const UI32 &rxValue)
    {
        m_rxValue  =  rxValue;
    }

    UI32  APPMGlobalAPPMQosMessage::getRxValue() const
    {
        return (m_rxValue);
    }

    void  APPMGlobalAPPMQosMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  APPMGlobalAPPMQosMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  APPMGlobalAPPMQosMessage::setCosToCosMutationMap(const string &cosToCosMutationMap)
    {
        m_cosToCosMutationMap  =  cosToCosMutationMap;
    }

    string  APPMGlobalAPPMQosMessage::getCosToCosMutationMap() const
    {
        return (m_cosToCosMutationMap);
    }

    void  APPMGlobalAPPMQosMessage::setCosToTrafficClassMap(const string &cosToTrafficClassMap)
    {
        m_cosToTrafficClassMap  =  cosToTrafficClassMap;
    }

    string  APPMGlobalAPPMQosMessage::getCosToTrafficClassMap() const
    {
        return (m_cosToTrafficClassMap);
    }

}
