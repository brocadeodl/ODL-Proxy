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

#include "Nsm/Local/NsmLocalObjectManager.h"
#include "Nsm/Local/Igmps/NsmUpdateIgmpsVlanLocalConfigMessage.h"
#include "Nsm/Local/NsmTypes.h"

namespace DcmNs
{

    NsmUpdateIgmpsVlanLocalConfigMessage::NsmUpdateIgmpsVlanLocalConfigMessage ()
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEIGMPSVLANLOCALCONFIG)
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
	}

    NsmUpdateIgmpsVlanLocalConfigMessage::NsmUpdateIgmpsVlanLocalConfigMessage (const UI32 &lmqi,const UI32 &qmrt,const bool &enable,const bool &fastLeave,const UI32 &queryInterval,const bool &querierEnable,const UI32 &mrouterTimeout,const UI32 &vlanId,const string &vlanIfName,const UI64 &vlanIfIndex,const UI32 &opCode,const bool &negation)
        : PrismMessage (NsmLocalObjectManager::getPrismServiceId (),NSMUPDATEIGMPSVLANLOCALCONFIG),
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
    }

    NsmUpdateIgmpsVlanLocalConfigMessage::~NsmUpdateIgmpsVlanLocalConfigMessage ()
    {
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
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
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setLmqi(const UI32 &lmqi)
    {
        m_lmqi  =  lmqi;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getLmqi() const
    {
        return (m_lmqi);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setQmrt(const UI32 &qmrt)
    {
        m_qmrt  =  qmrt;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getQmrt() const
    {
        return (m_qmrt);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  NsmUpdateIgmpsVlanLocalConfigMessage::getEnable() const
    {
        return (m_enable);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setFastLeave(const bool &fastLeave)
    {
        m_fastLeave  =  fastLeave;
    }

    bool  NsmUpdateIgmpsVlanLocalConfigMessage::getFastLeave() const
    {
        return (m_fastLeave);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setQueryInterval(const UI32 &queryInterval)
    {
        m_queryInterval  =  queryInterval;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getQueryInterval() const
    {
        return (m_queryInterval);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setQuerierEnable(const bool &querierEnable)
    {
        m_querierEnable  =  querierEnable;
    }

    bool  NsmUpdateIgmpsVlanLocalConfigMessage::getQuerierEnable() const
    {
        return (m_querierEnable);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setMrouterTimeout(const UI32 &mrouterTimeout)
    {
        m_mrouterTimeout  =  mrouterTimeout;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getMrouterTimeout() const
    {
        return (m_mrouterTimeout);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setVlanId(const UI32 &vlanId)
    {
        m_vlanId  =  vlanId;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getVlanId() const
    {
        return (m_vlanId);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setVlanIfName(const string &vlanIfName)
    {
        m_vlanIfName  =  vlanIfName;
    }

    string  NsmUpdateIgmpsVlanLocalConfigMessage::getVlanIfName() const
    {
        return (m_vlanIfName);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setVlanIfIndex(const UI64 &vlanIfIndex)
    {
        m_vlanIfIndex  =  vlanIfIndex;
    }

    UI64  NsmUpdateIgmpsVlanLocalConfigMessage::getVlanIfIndex() const
    {
        return (m_vlanIfIndex);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setOpCode(const UI32 &opCode)
    {
        m_opCode  =  opCode;
    }

    UI32  NsmUpdateIgmpsVlanLocalConfigMessage::getOpCode() const
    {
        return (m_opCode);
    }

    void  NsmUpdateIgmpsVlanLocalConfigMessage::setNegation(const bool &negation)
    {
        m_negation  =  negation;
    }

    bool  NsmUpdateIgmpsVlanLocalConfigMessage::getNegation() const
    {
        return (m_negation);
    }

}
