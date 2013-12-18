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
#include "Pim/Local/PimStaticRpCfgMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimStaticRpCfgMessage::PimStaticRpCfgMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMSTATICRPCFG)
    {
    }

    PimStaticRpCfgMessage::PimStaticRpCfgMessage (const UI8 &msgType,const IpV4Address &rpAddr,const string &prefixList)
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMSTATICRPCFG),
        m_msgType    (msgType),
        m_rpAddr    (rpAddr),
        m_prefixList    (prefixList)
    {
    }

    PimStaticRpCfgMessage::~PimStaticRpCfgMessage ()
    {
    }

    void  PimStaticRpCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeIpV4Address(&m_rpAddr,"rpAddr"));
        addSerializableAttribute (new AttributeString(&m_prefixList,"prefixList"));
    }

    void  PimStaticRpCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimStaticRpCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimStaticRpCfgMessage::setRpAddr(const IpV4Address &rpAddr)
    {
        m_rpAddr  =  rpAddr;
    }

    IpV4Address  PimStaticRpCfgMessage::getRpAddr() const
    {
        return (m_rpAddr);
    }

    void  PimStaticRpCfgMessage::setPrefixList(const string &prefixList)
    {
        m_prefixList  =  prefixList;
    }

    string  PimStaticRpCfgMessage::getPrefixList() const
    {
        return (m_prefixList);
    }

}
