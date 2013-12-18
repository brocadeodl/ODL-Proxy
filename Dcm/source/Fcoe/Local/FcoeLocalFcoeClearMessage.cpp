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
 *   Author : vdharwad                                                     *
 **************************************************************************/

#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalFcoeClearMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeClearMessage::FcoeLocalFcoeClearMessage ()
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (),
                                      FCOE_LOCAL_FCOE_CLEAR)
    {
    }

    FcoeLocalFcoeClearMessage::FcoeLocalFcoeClearMessage (const UI32 &cmdCode)
        : PrismMessage (FcoeLocalObjectManager::getPrismServiceId (), 
                        FCOE_LOCAL_FCOE_CLEAR),
        m_cmdCode (cmdCode),
        m_vlan (0),
        m_vfid (0),
        m_fc_uport (0),
        m_intf_name (""),
        m_wwn (""),
        m_if_index(0),
        m_locationId(0),
		m_po (0)
    {
    }

    FcoeLocalFcoeClearMessage::~FcoeLocalFcoeClearMessage ()
    {
    }

    void  FcoeLocalFcoeClearMessage::setupAttributesForSerialization()
    {
        PrismMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeString(&m_wwn, "WWN"));
        addSerializableAttribute (new AttributeUI64(&m_if_index, "if_index"));
        addSerializableAttribute (new AttributeUI32(&m_locationId, "loc_id"));
        addSerializableAttribute (new AttributeUI32(&m_po, "Portchannel"));

    }

    void  FcoeLocalFcoeClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeLocalFcoeClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32 
    FcoeLocalFcoeClearMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeLocalFcoeClearMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeLocalFcoeClearMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeLocalFcoeClearMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeLocalFcoeClearMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeLocalFcoeClearMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeLocalFcoeClearMessage::getIfName() const
    {
        return m_intf_name;
    }
    
    void 
    FcoeLocalFcoeClearMessage::setIfName(const string &if_name)
    {
        m_intf_name = if_name;
    }

    void 
    FcoeLocalFcoeClearMessage::setWWN(const string &wwn)
    {
        m_wwn = wwn;
    }
    
    string
    FcoeLocalFcoeClearMessage::getWWN() const
    {
        return (m_wwn);
    }

    UI64
    FcoeLocalFcoeClearMessage::getIfindex() const
    {
        return (m_if_index);
    }

    void
    FcoeLocalFcoeClearMessage::setIfindex(const UI64 &if_index)
    {
        m_if_index = if_index;
    }

    UI32
    FcoeLocalFcoeClearMessage::getDestLocationId() const
    {
        return (m_locationId);
    }

    void
    FcoeLocalFcoeClearMessage::setDestLocationId(const UI32 &loc_id)
    {
        m_locationId = loc_id;
    }

    UI32
    FcoeLocalFcoeClearMessage::getPo() const
    {
        return m_po;
    }

    void
    FcoeLocalFcoeClearMessage::setPo(const UI32 &po)
    {
        m_po = po;
    }
}
