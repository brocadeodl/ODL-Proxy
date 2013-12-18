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
 *   Author : abhaskar                                                     *
 **************************************************************************/

#include "Nsm/Global/Igmps/NsmGlobalUpdateIgmpsVlanConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdateIgmpsVlanConfigMessage::NsmGlobalUpdateIgmpsVlanConfigMessage ()
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEIGMPSVLANCONFIG)
    {
		m_enable = false;
        m_fastLeave = false;
        m_lmqi = 0;
        m_mrouterTimeout = 0;
        m_negation = false;
        m_opCode = 0;
        m_qmrt = 0;
        m_querierEnable = false;
        m_queryInterval = 0;
        m_vlanId = 1;
        m_vlanIfIndex = 0;
        m_restrictUnknownMcast = false;
    }

    NsmGlobalUpdateIgmpsVlanConfigMessage::NsmGlobalUpdateIgmpsVlanConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation)
        : ManagementInterfaceMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEIGMPSVLANCONFIG),
        m_lmqi    (lmqi),
        m_qmrt    (qmrt),
        m_enable    (enable),
        m_fastLeave    (fastLeave),
        m_queryInterval    (queryInterval),
        m_querierEnable    (querierEnable),
        m_mrouterTimeout    (mrouterTimeout),
        m_vlanId    (vlanId),
        m_vlanIfName    (vlanIfName),
        m_vlanIfIndex    (vlanIfIndex),
        m_opCode    (opCode),
        m_negation    (negation)
    {
        m_restrictUnknownMcast = false;
    }

    NsmGlobalUpdateIgmpsVlanConfigMessage::~NsmGlobalUpdateIgmpsVlanConfigMessage ()
    {
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_lmqi,"lmqi"));
        addSerializableAttribute (new AttributeUI32(&m_qmrt,"qmrt"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
        addSerializableAttribute (new AttributeBool(&m_fastLeave,"fastLeave"));
        addSerializableAttribute (new AttributeUI32(&m_queryInterval,"queryInterval"));
        addSerializableAttribute (new AttributeBool(&m_querierEnable,"querierEnable"));
        addSerializableAttribute (new AttributeUI32(&m_mrouterTimeout,"mrouterTimeout"));
        addSerializableAttribute (new AttributeUI32(&m_vlanId,"vlanId"));
        addSerializableAttribute (new AttributeString(&m_vlanIfName,"vlanIfName"));
        addSerializableAttribute (new AttributeUI64(&m_vlanIfIndex,"vlanIfIndex"));
        addSerializableAttribute (new AttributeUI32(&m_opCode,"opCode"));
        addSerializableAttribute (new AttributeBool(&m_negation,"negation"));
        addSerializableAttribute (new AttributeBool(&m_restrictUnknownMcast,"restrictUnknownMcast"));
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setLmqi(const UI32 &lmqi)
    {
        m_lmqi  =  lmqi;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getLmqi() const
    {
        return (m_lmqi);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setQmrt(const UI32 &qmrt)
    {
        m_qmrt  =  qmrt;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getQmrt() const
    {
        return (m_qmrt);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  NsmGlobalUpdateIgmpsVlanConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setFastLeave(const bool &fastLeave)
    {
        m_fastLeave  =  fastLeave;
    }

    bool  NsmGlobalUpdateIgmpsVlanConfigMessage::getFastLeave() const
    {
        return (m_fastLeave);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setQueryInterval(const UI32 &queryInterval)
    {
        m_queryInterval  =  queryInterval;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getQueryInterval() const
    {
        return (m_queryInterval);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setQuerierEnable(const bool &querierEnable)
    {
        m_querierEnable  =  querierEnable;
    }

    bool  NsmGlobalUpdateIgmpsVlanConfigMessage::getQuerierEnable() const
    {
        return (m_querierEnable);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setMrouterTimeout(const UI32 &mrouterTimeout)
    {
        m_mrouterTimeout  =  mrouterTimeout;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getMrouterTimeout() const
    {
        return (m_mrouterTimeout);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  NsmGlobalUpdateIgmpsVlanConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  NsmGlobalUpdateIgmpsVlanConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmGlobalUpdateIgmpsVlanConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmGlobalUpdateIgmpsVlanConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  NsmGlobalUpdateIgmpsVlanConfigMessage::getNegation() const
    {
        return (m_negation);
    }

    void NsmGlobalUpdateIgmpsVlanConfigMessage::setRestrictUnknownMcast(const bool &restrictUnknownMcast)
    {
        m_restrictUnknownMcast  = restrictUnknownMcast;
    }

    bool  NsmGlobalUpdateIgmpsVlanConfigMessage::getRestrictUnknownMcast() const
    {
        return (m_restrictUnknownMcast);
    }
}
