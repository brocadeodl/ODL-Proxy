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

/* **
 * MstpLocalClearMessage.cpp
 *
 * Dec 26, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#include "Mstp/Local/MstpLocalClearMessage.h"
#include "Mstp/Local/MstpLocalObjectManager.h"
#include "Mstp/Local/MstpTypes.h"
#include "ClientInterface/Mstp/MstpClearMessage.h"
#include "Framework/Attributes/AttributeEnum.h"


namespace DcmNs
{

    MstpLocalClearMessage::MstpLocalClearMessage ()
        : PrismMessage (MstpLocalObjectManager::getPrismServiceId(), MSTPLOCALCLEARSTP)
    {
        m_distribute    = false;
        m_cmdCode       = 0;
        m_ifType        = IF_TYPE_INVALID;
    }

    MstpLocalClearMessage::~MstpLocalClearMessage ()
    {
    }

    void MstpLocalClearMessage::copyFrom (MstpLocalClearMessage *other)
    {
        // Only command attributes are copied.. Distribute flag is omitted
        m_cmdCode = other->m_cmdCode;
        m_ifType  = other->m_ifType;
        m_ifName  = other->m_ifName;
    }

    void MstpLocalClearMessage::copyTo (MstpClearMessage *pClearMessage)
    {
        pClearMessage->setCmdCode(m_cmdCode);
        pClearMessage->setIfType(m_ifType);
        pClearMessage->setIfName(m_ifName);
    }


    void  MstpLocalClearMessage::setupAttributesForSerialization ()
    {
        PrismMessage::setupAttributesForSerialization();
        addSerializableAttribute(new AttributeBool(&m_distribute, "distribute"));
        addSerializableAttribute(new AttributeUI32(&m_cmdCode, "cmdCode"));
        addSerializableAttribute(new AttributeEnum((UI32*)(&m_ifType), "ifType"));
        addSerializableAttribute(new AttributeString(&m_ifName, "ifName"));
    }

    void  MstpLocalClearMessage::setInterfaceInfo (InterfaceType ifType, const string &ifName)
    {
        m_ifType = ifType;
        m_ifName= ifName;
    }

    InterfaceType MstpLocalClearMessage::getIfType () const
    {
        return m_ifType;
    }

    string  MstpLocalClearMessage::getIfName () const
    {
        return m_ifName;
    }

    void  MstpLocalClearMessage::setCmdCode (UI32 cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  MstpLocalClearMessage::getCmdCode () const
    {
        return m_cmdCode;
    }

    void MstpLocalClearMessage::setDistribute (bool distribute)
    {
        m_distribute = distribute;
    }

    bool MstpLocalClearMessage::isDistribute () const
    {
        return m_distribute;
    }

}
