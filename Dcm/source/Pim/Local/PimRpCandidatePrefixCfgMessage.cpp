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
 *   Author : Chirag Taunk                                                 *
 **************************************************************************/

#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimRpCandidatePrefixCfgMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimRpCandidatePrefixCfgMessage::PimRpCandidatePrefixCfgMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), 
		PIMRPCANDIDATEPREFIXCFG)
    {
    }

    PimRpCandidatePrefixCfgMessage::PimRpCandidatePrefixCfgMessage (
		const UI8 &msgType, const UI8 &prefixAction,const IpV4Address 
		&prefixIP,const UI16 &prefixLen)
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), 
		PIMSTATICRPCFG),
        m_msgType    (msgType),
		m_prefixAction	(prefixAction),
        m_prefixIP    (prefixIP),
        m_prefixLen    (prefixLen)
    {
    }

    PimRpCandidatePrefixCfgMessage::~PimRpCandidatePrefixCfgMessage ()
    {
    }

    void  PimRpCandidatePrefixCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
		addSerializableAttribute (new AttributeUI8(&m_prefixAction,
			"prefixAction"));
        addSerializableAttribute (new AttributeIpV4Address(&m_prefixIP,
			"prefixIP"));
        addSerializableAttribute (new AttributeUI16(&m_prefixLen,"prefixLen"));
    }

    void  PimRpCandidatePrefixCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimRpCandidatePrefixCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimRpCandidatePrefixCfgMessage::setAction(const UI8 &prefixAction)
    {
        m_prefixAction  =  prefixAction;
    }

    UI8  PimRpCandidatePrefixCfgMessage::getAction() const
    {
        return (m_prefixAction);
    }

    void  PimRpCandidatePrefixCfgMessage::setPrefixIP(const IpV4Address 
		&prefixIP)
    {
        m_prefixIP  =  prefixIP;
    }

    IpV4Address  PimRpCandidatePrefixCfgMessage::getPrefixIP() const
    {
        return (m_prefixIP);
    }

    void  PimRpCandidatePrefixCfgMessage::setPrefixLen(const UI16 &prefixLen)
    {
        m_prefixLen  =  prefixLen;
    }

    UI16  PimRpCandidatePrefixCfgMessage::getPrefixLen() const
    {
        return (m_prefixLen);
    }

}
