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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimRtrInstMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimRtrInstMessage::PimRtrInstMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMRTRINST)
    {
    }

    PimRtrInstMessage::~PimRtrInstMessage ()
    {
    }

    void  PimRtrInstMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeString(&m_vrfId,"vrfId"));
        addSerializableAttribute (new AttributeUI32(&m_maxMcache,"maxMcache"));
        addSerializableAttribute (new AttributeSI16(&m_helloTimer,"helloTimer"));
        addSerializableAttribute (new AttributeSI16(&m_NbrTimeout,"NbrTimeout"));
        addSerializableAttribute (new AttributeSI16(&m_inactivityTimer,"inactivityTimer"));
        addSerializableAttribute (new AttributeUI32(&m_msgInterval,"msgInterval"));
        addSerializableAttribute (new AttributeUI32(&m_sptThreshold,"sptThreshold"));
    }

    void  PimRtrInstMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimRtrInstMessage::getMsgType() const
    {
        return (m_msgType);
    }
    
    void  PimRtrInstMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  PimRtrInstMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PimRtrInstMessage::setVrfId(const string &vrfId)
    {
        m_vrfId  =  vrfId;
    }

    string  PimRtrInstMessage::getVrfId() const
    {
        return (m_vrfId);
    }

    void  PimRtrInstMessage::setMaxMcache(const UI32 &maxMcache)
    {
        m_maxMcache  =  maxMcache;
    }

    UI32  PimRtrInstMessage::getMaxMcache() const
    {
        return (m_maxMcache);
    }

    void  PimRtrInstMessage::setHelloTimer(const SI16 &helloTimer)
    {
        m_helloTimer  =  helloTimer;
    }

    SI16  PimRtrInstMessage::getHelloTimer() const
    {
        return (m_helloTimer);
    }

    void  PimRtrInstMessage::setNbrTimeout(const SI16 &NbrTimeout)
    {
        m_NbrTimeout  =  NbrTimeout;
    }

    SI16  PimRtrInstMessage::getNbrTimeout() const
    {
        return (m_NbrTimeout);
    }

    void  PimRtrInstMessage::setInactivityTimer(const SI16 &inactivityTimer)
    {
        m_inactivityTimer  =  inactivityTimer;
    }

    SI16  PimRtrInstMessage::getInactivityTimer() const
    {
        return (m_inactivityTimer);
    }

    void  PimRtrInstMessage::setMsgInterval(const UI32 &msgInterval)
    {
        m_msgInterval  =  msgInterval;
    }

    UI32  PimRtrInstMessage::getMsgInterval() const
    {
        return (m_msgInterval);
    }

    void  PimRtrInstMessage::setSptThreshold(const UI32 &sptThreshold)
    {
        m_sptThreshold  =  sptThreshold;
    }

    UI32  PimRtrInstMessage::getSptThreshold() const
    {
        return (m_sptThreshold);
    }

}
