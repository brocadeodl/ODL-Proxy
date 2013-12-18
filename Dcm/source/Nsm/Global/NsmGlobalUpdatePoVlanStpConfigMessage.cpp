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
 *   Author : nsong                                                     *
 **************************************************************************/

#include "Nsm/Global/NsmGlobalUpdatePoVlanStpConfigMessage.h"
#include "Nsm/Global/NsmGlobalObjectManager.h"
#include "Nsm/Global/NsmGlobalTypes.h"

namespace DcmNs
{

    NsmGlobalUpdatePoVlanStpConfigMessage::NsmGlobalUpdatePoVlanStpConfigMessage ()
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName (),NSMGLOBALUPDATEPOVLANSTPCONFIG)
        , m_vid ( 1 )
    {
    }

    NsmGlobalUpdatePoVlanStpConfigMessage::NsmGlobalUpdatePoVlanStpConfigMessage (const string &ifName,const UI32 &vid,const UI32 &priority,const UI32 &cost,const UI32 &cmdCode)
        : NsmPortStpConfigMessage (NsmGlobalObjectManager::getClassName (), NSMGLOBALUPDATEPOVLANSTPCONFIG, cmdCode, IF_TYPE_PO, ifName, cost, priority)
        , m_vid ( vid )
    {
    }

    NsmGlobalUpdatePoVlanStpConfigMessage::~NsmGlobalUpdatePoVlanStpConfigMessage ()
    {
    }

    void  NsmGlobalUpdatePoVlanStpConfigMessage::setupAttributesForSerialization()
    {
        NsmPortStpConfigMessage::setupAttributesForSerialization();
        addSerializableAttribute (new AttributeUI32(&m_vid,"vid"));
    }

    void  NsmGlobalUpdatePoVlanStpConfigMessage::setIfName(const string &ifName)
    {
        setInterfaceInfo(IF_TYPE_PO, ifName);
    }

    void  NsmGlobalUpdatePoVlanStpConfigMessage::setVid(const UI32 &vid)
    {
        m_vid  =  vid;
    }

    UI32  NsmGlobalUpdatePoVlanStpConfigMessage::getVid() const
    {
        return (m_vid);
    }

}
