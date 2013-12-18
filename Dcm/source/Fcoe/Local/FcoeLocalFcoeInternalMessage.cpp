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

#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalFcoeInternalMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

#include "Fcoe/Common/FcoePluginTypes.h"

namespace DcmNs
{   

    FcoeLocalFcoeInternalMessage::FcoeLocalFcoeInternalMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),
                                      FCOE_LOCAL_FCOE_INTERNAL)
    {
    }

    FcoeLocalFcoeInternalMessage::FcoeLocalFcoeInternalMessage (const UI32 &cmdCode)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),
                        FCOE_LOCAL_FCOE_INTERNAL),
        m_cmdCode (cmdCode),
        m_vlan (0),
        m_vfid (0),
        m_fc_uport (FCOE_INTF_UPORT_NUM_INVALID),
        m_intf_name (""),
        m_if_index(0),
        m_locationId(0)
    {
    }

    FcoeLocalFcoeInternalMessage::~FcoeLocalFcoeInternalMessage ()
    {
    }

    void  FcoeLocalFcoeInternalMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "if_index"));
        addSerializableAttribute (new AttributeUI32(&m_locationId, "loc_id"));
    }

    void  FcoeLocalFcoeInternalMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeLocalFcoeInternalMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32
    FcoeLocalFcoeInternalMessage::getVlan() const
    {   
        return m_vlan;
    }

    void
    FcoeLocalFcoeInternalMessage::setVlan(const UI32 &vlan)
    {   
        m_vlan = vlan;
    }

    UI32
    FcoeLocalFcoeInternalMessage::getVfid() const
    {   
        return m_vfid;
    }

    void
    FcoeLocalFcoeInternalMessage::setVfid(const UI32 &vfid)
    {   
        m_vfid = vfid;
    }

    UI32
    FcoeLocalFcoeInternalMessage::getFcUport() const
    {
        return m_fc_uport;
    }

    void
    FcoeLocalFcoeInternalMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }

    string
    FcoeLocalFcoeInternalMessage::getIfName() const
    {   
        return m_intf_name;
    }

    void
    FcoeLocalFcoeInternalMessage::setIfName(const string &if_name)
    {
        m_intf_name = if_name;
    }

    UI64
    FcoeLocalFcoeInternalMessage::getIfindex() const
    {
        return (m_if_index);
    }

    void
    FcoeLocalFcoeInternalMessage::setIfindex(const UI64 &if_index)
    {
        m_if_index = if_index;
    }

    void FcoeLocalFcoeInternalMessage::getOutputBufferRef (UI32 bufferNum,
                                                       UI32 &size,
                                                       void *&pBuffer)
    {
        pBuffer = findBuffer(bufferNum, size);
    }

    UI32
    FcoeLocalFcoeInternalMessage::getDestLocationId() const
    {
        return (m_locationId);
    }

    void
    FcoeLocalFcoeInternalMessage::setDestLocationId(const UI32 &loc_id)
    {
        m_locationId = loc_id;
    }
}

