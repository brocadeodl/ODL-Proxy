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
#include "Pim/Local/PimMultRtrCfgMessage.h"
#include "Pim/Local/PimTypes.h"

namespace DcmNs
{

    PimMultRtrCfgMessage::PimMultRtrCfgMessage ()
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMMULTRTRCFG)
    {
    }

    PimMultRtrCfgMessage::PimMultRtrCfgMessage (const UI8 &msgType,const UI8 &cmdCode,const bool &enable)
        : PrismMessage (PimLocalObjectManager::getPrismServiceId (), PIMMULTRTRCFG),
        m_msgType    (msgType),
        m_cmdCode    (cmdCode),
        m_enable    (enable)
    {
    }

    PimMultRtrCfgMessage::~PimMultRtrCfgMessage ()
    {
    }

    void  PimMultRtrCfgMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI8(&m_msgType,"msgType"));
        addSerializableAttribute (new AttributeUI8(&m_cmdCode,"cmdCode"));
        addSerializableAttribute (new AttributeBool(&m_enable,"enable"));
    }

    void  PimMultRtrCfgMessage::setMsgType(const UI8 &msgType)
    {
        m_msgType  =  msgType;
    }

    UI8  PimMultRtrCfgMessage::getMsgType() const
    {
        return (m_msgType);
    }

    void  PimMultRtrCfgMessage::setCmdCode(const UI8 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI8  PimMultRtrCfgMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    void  PimMultRtrCfgMessage::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  PimMultRtrCfgMessage::getEnable() const
    {
        return (m_enable);
    }

}
