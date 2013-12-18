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
#include "Pim/Local/PimRpCandidateCfgMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimRpCandidateCfgMessage::PimRpCandidateCfgMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMRPCANDIDATECFG)
    {
    }

    PimRpCandidateCfgMessage::PimRpCandidateCfgMessage (const UI8 &msgType,const UI8 &intfType,const string &intfName,const string &prefixList)
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMRPCANDIDATECFG),
        m_msgType    (msgType),
        m_intfType    (intfType),
        m_intfName    (intfName),
        m_prefixList    (prefixList)
    {
    }

    PimRpCandidateCfgMessage::~PimRpCandidateCfgMessage ()
    {
    }

    void  PimRpCandidateCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeString(&m_prefixList,"prefixList"));
    }

    void  PimRpCandidateCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimRpCandidateCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimRpCandidateCfgMessage::setIntfType(const UI8 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI8  PimRpCandidateCfgMessage::getIntfType() const
    {
        return (m_intfType);
    }

    void  PimRpCandidateCfgMessage::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  PimRpCandidateCfgMessage::getIntfName() const
    {
        return (m_intfName);
    }

    void  PimRpCandidateCfgMessage::setPrefixList(const string &prefixList)
    {
        m_prefixList  =  prefixList;
    }

    string  PimRpCandidateCfgMessage::getPrefixList() const
    {
        return (m_prefixList);
    }

}
