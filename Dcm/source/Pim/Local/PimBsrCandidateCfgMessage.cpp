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
#include "Pim/Local/PimBsrCandidateCfgMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimBsrCandidateCfgMessage::PimBsrCandidateCfgMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMBSRCANDIDATECFG),
        m_intfType (PIM_INTF_TYPE_UNKWN),
        m_mask     (0),
        m_priority (0)
    {
    }

    PimBsrCandidateCfgMessage::PimBsrCandidateCfgMessage (const UI8 &msgType,const UI8 &intfType,const string &intfName,const UI16 &mask,const UI16 &priority)
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMBSRCANDIDATECFG),
        m_msgType    (msgType),
        m_intfType    (intfType),
        m_intfName    (intfName),
        m_mask    (mask),
        m_priority    (priority)
    {
    }

    PimBsrCandidateCfgMessage::~PimBsrCandidateCfgMessage ()
    {
    }

    void  PimBsrCandidateCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_intfType,"intfType"));
        addSerializableAttribute (new AttributeString(&m_intfName,"intfName"));
        addSerializableAttribute (new AttributeUI16(&m_mask,"mask"));
        addSerializableAttribute (new AttributeUI16(&m_priority,"priority"));
    }

    void  PimBsrCandidateCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimBsrCandidateCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimBsrCandidateCfgMessage::setIntfType(const UI8 &intfType)
    {
        m_intfType  =  intfType;
    }

    UI8  PimBsrCandidateCfgMessage::getIntfType() const
    {
        return (m_intfType);
    }

    void  PimBsrCandidateCfgMessage::setIntfName(const string &intfName)
    {
        m_intfName  =  intfName;
    }

    string  PimBsrCandidateCfgMessage::getIntfName() const
    {
        return (m_intfName);
    }

    void  PimBsrCandidateCfgMessage::setMask(const UI16 &mask)
    {
        m_mask  =  mask;
    }

    UI16  PimBsrCandidateCfgMessage::getMask() const
    {
        return (m_mask);
    }

    void  PimBsrCandidateCfgMessage::setPriority(const UI16 &priority)
    {
        m_priority  =  priority;
    }

    UI16  PimBsrCandidateCfgMessage::getPriority() const
    {
        return (m_priority);
    }

}
