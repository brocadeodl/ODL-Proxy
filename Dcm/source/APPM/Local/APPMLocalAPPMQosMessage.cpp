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

#include "APPM/Local/APPMLocalObjectManager.h"
#include "APPM/Local/APPMLocalAPPMQosMessage.h"
#include "APPM/Local/APPMLocalTypes.h"

namespace DcmNs
{

    APPMLocalAPPMQosMessage::APPMLocalAPPMQosMessage ()
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMQOS)
    {
    }

    APPMLocalAPPMQosMessage::APPMLocalAPPMQosMessage (const UI8 &opCode,const UI8 &cmdCode,const string &profileName,const UI32 &profileId,const SI32 &defaultCosValue,const bool &qosTrust,const SI32 &flowControlTxEnable,const SI32 &flowControlRxEnable,const SI32 &pfcValue,const UI32 &txValue,const UI32 &rxValue,const string &ceeMapName,const string &cosToCosMutationMap,const string &cosToTrafficClassMap)
        : PrismMessage (APPMLocalObjectManager::getPrismServiceId (),APPMLOCALAPPMQOS),
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

    APPMLocalAPPMQosMessage::~APPMLocalAPPMQosMessage ()
    {
    }

    void  APPMLocalAPPMQosMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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

    void  APPMLocalAPPMQosMessage::setOpCode(const UI8 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI8  APPMLocalAPPMQosMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  APPMLocalAPPMQosMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  APPMLocalAPPMQosMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  APPMLocalAPPMQosMessage::setProfileName(const string &profileName)
    {
        m_profileName  =  profileName;
    }

    string  APPMLocalAPPMQosMessage::getProfileName() const
    {
        return (m_profileName);
    }

    void  APPMLocalAPPMQosMessage::setProfileId(const UI32 &profileId)
    {
        m_profileId  =  profileId;
    }

    UI32  APPMLocalAPPMQosMessage::getProfileId() const
    {
        return (m_profileId);
    }

    void  APPMLocalAPPMQosMessage::setDefaultCosValue(const SI32 &defaultCosValue)
    {
        m_defaultCosValue  =  defaultCosValue;
    }

    SI32  APPMLocalAPPMQosMessage::getDefaultCosValue() const
    {
        return (m_defaultCosValue);
    }

    void  APPMLocalAPPMQosMessage::setQosTrust(const bool &qosTrust)
    {
        m_qosTrust  =  qosTrust;
    }

    bool  APPMLocalAPPMQosMessage::getQosTrust() const
    {
        return (m_qosTrust);
    }

    void  APPMLocalAPPMQosMessage::setFlowControlTxEnable(const SI32 &flowControlTxEnable)
    {
        m_flowControlTxEnable  =  flowControlTxEnable;
    }

    SI32  APPMLocalAPPMQosMessage::getFlowControlTxEnable() const
    {
        return (m_flowControlTxEnable);
    }

    void  APPMLocalAPPMQosMessage::setFlowControlRxEnable(const SI32 &flowControlRxEnable)
    {
        m_flowControlRxEnable  =  flowControlRxEnable;
    }

    SI32  APPMLocalAPPMQosMessage::getFlowControlRxEnable() const
    {
        return (m_flowControlRxEnable);
    }

    void  APPMLocalAPPMQosMessage::setPfcValue(const SI32 &pfcValue)
    {
        m_pfcValue  =  pfcValue;
    }

    SI32  APPMLocalAPPMQosMessage::getPfcValue() const
    {
        return (m_pfcValue);
    }

    void  APPMLocalAPPMQosMessage::setTxValue(const UI32 &txValue)
    {
        m_txValue  =  txValue;
    }

    UI32  APPMLocalAPPMQosMessage::getTxValue() const
    {
        return (m_txValue);
    }

    void  APPMLocalAPPMQosMessage::setRxValue(const UI32 &rxValue)
    {
        m_rxValue  =  rxValue;
    }

    UI32  APPMLocalAPPMQosMessage::getRxValue() const
    {
        return (m_rxValue);
    }

    void  APPMLocalAPPMQosMessage::setCeeMapName(const string &ceeMapName)
    {
        m_ceeMapName  =  ceeMapName;
    }

    string  APPMLocalAPPMQosMessage::getCeeMapName() const
    {
        return (m_ceeMapName);
    }

    void  APPMLocalAPPMQosMessage::setCosToCosMutationMap(const string &cosToCosMutationMap)
    {
        m_cosToCosMutationMap  =  cosToCosMutationMap;
    }

    string  APPMLocalAPPMQosMessage::getCosToCosMutationMap() const
    {
        return (m_cosToCosMutationMap);
    }

    void  APPMLocalAPPMQosMessage::setCosToTrafficClassMap(const string &cosToTrafficClassMap)
    {
        m_cosToTrafficClassMap  =  cosToTrafficClassMap;
    }

    string  APPMLocalAPPMQosMessage::getCosToTrafficClassMap() const
    {
        return (m_cosToTrafficClassMap);
    }

}
