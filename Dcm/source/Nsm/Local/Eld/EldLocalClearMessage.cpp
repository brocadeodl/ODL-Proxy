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

/****************************************************************************
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Brunda Rajagopala
 **************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Nsm/Local/NsmLocalObjectManager.h"
#include "ClientInterface/Eld/EldClearMessage.h"
#include "Nsm/Local/Eld/EldLocalClearMessage.h"
#include "ClientInterface/Eld/eld_dcm_def.h"
#include "ClientInterface/DceWaveClientMessage.h"

namespace DcmNs
{

    EldLocalClearMessage::EldLocalClearMessage ()
        :PrismMessage (NsmLocalObjectManager::getPrismServiceId (),ELDLOCALCLEAR)
    {
                m_cmdcode = 0;
                m_rbridgeId = 0;
                m_is_distribution_required =false;
    }
    EldLocalClearMessage::EldLocalClearMessage (EldLocalClearMessage *pMsg)
        :PrismMessage (NsmLocalObjectManager::getPrismServiceId (),ELDLOCALCLEAR)
    {
                m_cmdcode = pMsg->getCmdCode();
                m_rbridgeId = pMsg->getRbridgeId();
                m_is_distribution_required = pMsg->getisDistributionRequired();
                m_locs = pMsg->getLocationIds();
                m_ifName = pMsg->getIfName();
		m_ifType = pMsg->getIfType();
    }

    EldLocalClearMessage::EldLocalClearMessage (const UI32 &cmdcode,const UI32& rbridgeId,const bool &disreq, const string &ifName, const UI32 &ifType)
        :PrismMessage (NsmLocalObjectManager::getPrismServiceId (),ELDLOCALCLEAR)
    {
                m_cmdcode = cmdcode;
                m_rbridgeId = rbridgeId;
                m_is_distribution_required = disreq;
                m_ifName = ifName;
                m_ifType = ifType;
    }


    EldLocalClearMessage::~EldLocalClearMessage ()
    {
    }

    void  EldLocalClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdcode,"cmdcode"));
        addSerializableAttribute (new AttributeUI32(&m_rbridgeId,"rbridgeId"));
        addSerializableAttribute (new AttributeBool(&m_is_distribution_required,"isDistributionRequired"));
        addSerializableAttribute (new AttributeUI32Vector(&m_locs, "locs"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifname"));
        addSerializableAttribute (new AttributeUI32(&m_ifType,"iftype"));


    }

    void  EldLocalClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdcode  =  cmdCode;
    }

    UI32  EldLocalClearMessage::getCmdCode() const
    {
        return (m_cmdcode);
    }


    bool EldLocalClearMessage::getisDistributionRequired() const 
    {
        return m_is_distribution_required;
    }

    void EldLocalClearMessage::setisDistributionRequired(const bool f) 
    { 
        m_is_distribution_required = f;
    }

    void  EldLocalClearMessage::setRbridgeId(const UI32 &rbridgeId)
    {
        m_rbridgeId  =  rbridgeId;
    }

    UI32  EldLocalClearMessage::getRbridgeId() const
    {
        return (m_rbridgeId);
    }

    void  EldLocalClearMessage::setIfType(const UI32 &ifType)
    {
        m_ifType = ifType;
    }

    UI32  EldLocalClearMessage::getIfType() const
    {
        return (m_ifType);
    }

    void  EldLocalClearMessage::setIfName(const string &ifName)
    {
        m_ifName = ifName;
    }

    string  EldLocalClearMessage::getIfName() const
    {
        return (m_ifName);
    }
}

