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
 *   Author : jsaluja                                                      *
 **************************************************************************/

#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalFcoeInternalMessage.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

namespace DcmNs
{   

    FcoeGlobalFcoeInternalMessage::FcoeGlobalFcoeInternalMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),
                                      FCOE_GLOBAL_FCOE_INTERNAL)
    {
    }

    FcoeGlobalFcoeInternalMessage::FcoeGlobalFcoeInternalMessage (const UI32 &cmdCode)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),
                                      FCOE_GLOBAL_FCOE_INTERNAL),
        m_cmdCode (cmdCode)
    {
    }

    FcoeGlobalFcoeInternalMessage::~FcoeGlobalFcoeInternalMessage ()
    {
    }

    void  FcoeGlobalFcoeInternalMessage::setupAttributesForSerialization()
    {   
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "if_index"));
    }

    void  FcoeGlobalFcoeInternalMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeGlobalFcoeInternalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32
    FcoeGlobalFcoeInternalMessage::getVlan() const
    {
        return m_vlan;
    }

    void
    FcoeGlobalFcoeInternalMessage::setVlan(const UI32 &vlan)
    {   
        m_vlan = vlan;
    }

    UI32
    FcoeGlobalFcoeInternalMessage::getVfid() const
    {   
        return m_vfid;
    }

    void
    FcoeGlobalFcoeInternalMessage::setVfid(const UI32 &vfid)
    {   
        m_vfid = vfid;
    }

    UI32
    FcoeGlobalFcoeInternalMessage::getFcUport() const
    {
        return m_fc_uport;
    }

    void
    FcoeGlobalFcoeInternalMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }

    string
    FcoeGlobalFcoeInternalMessage::getIfName() const
    {   
        return m_intf_name;
    }

    void
    FcoeGlobalFcoeInternalMessage::setIfName(const string &if_name)
    {   
        m_intf_name = if_name;
    }

    UI64
    FcoeGlobalFcoeInternalMessage::getIfindex() const
    {
        return m_if_index;
    }

    void
    FcoeGlobalFcoeInternalMessage::setIfindex(const UI64 &if_index)
    {
        m_if_index = if_index;
    }

    void FcoeGlobalFcoeInternalMessage::getOutputBufferRef (UI32 bufferNum,
                                                       UI32 &size,
                                                       void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }
}

