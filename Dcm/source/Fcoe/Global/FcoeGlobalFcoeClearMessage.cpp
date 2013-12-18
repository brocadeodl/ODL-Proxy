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

#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalFcoeClearMessage.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

namespace DcmNs
{

    FcoeGlobalFcoeClearMessage::FcoeGlobalFcoeClearMessage ()
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (),
                                      FCOE_GLOBAL_FCOE_CLEAR)
    {
    }

    FcoeGlobalFcoeClearMessage::FcoeGlobalFcoeClearMessage (const UI32 &cmdCode)
        : ManagementInterfaceMessage (FcoeGlobalObjectManager::getClassName (), 
                                      FCOE_GLOBAL_FCOE_CLEAR),
        m_cmdCode (cmdCode),
        m_vlan (0),
        m_vfid (0),
        m_fc_uport (0),
        m_intf_name (""),
        m_wwn (""),
        m_slotid (0),
        m_po (0)        
    {
    }

    FcoeGlobalFcoeClearMessage::~FcoeGlobalFcoeClearMessage ()
    {
    }

    void  FcoeGlobalFcoeClearMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_cmdCode,"cmd_code"));
        addSerializableAttribute (new AttributeUI32(&m_vlan, "Vlan"));
        addSerializableAttribute (new AttributeUI32(&m_vfid, "VFID"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport, "FC_Uport"));
        addSerializableAttribute (new AttributeString(&m_intf_name, "intf_name"));
        addSerializableAttribute (new AttributeString(&m_wwn, "WWN"));
        addSerializableAttribute (new AttributeUI32(&m_slotid, "Slotid"));
        addSerializableAttribute (new AttributeUI32(&m_po, "Portchannel"));
    }

    void  FcoeGlobalFcoeClearMessage::setCmdCode(const UI32 &cmdCode)
    {
        m_cmdCode  =  cmdCode;
    }

    UI32  FcoeGlobalFcoeClearMessage::getCmdCode() const
    {
        return (m_cmdCode);
    }

    UI32 
    FcoeGlobalFcoeClearMessage::getVlan() const
    {
        return m_vlan;
    }
    
    void 
    FcoeGlobalFcoeClearMessage::setVlan(const UI32 &vlan)
    {
        m_vlan = vlan;
    }
    
    UI32 
    FcoeGlobalFcoeClearMessage::getVfid() const
    {
        return m_vfid;
    }
    
    void 
    FcoeGlobalFcoeClearMessage::setVfid(const UI32 &vfid)
    {
        m_vfid = vfid;
    }
    
    UI32 
    FcoeGlobalFcoeClearMessage::getFcUport() const
    {
        return m_fc_uport;
    }
    
    void 
    FcoeGlobalFcoeClearMessage::setFcUport(const UI32 &fc_uport)
    {
        m_fc_uport = fc_uport;
    }
    
    string 
    FcoeGlobalFcoeClearMessage::getIfName() const
    {
        return m_intf_name;
    }
    
    void 
    FcoeGlobalFcoeClearMessage::setIfName(const string &if_name)
    {
        m_intf_name = if_name;
    }

    void 
    FcoeGlobalFcoeClearMessage::setWWN(const string &wwn)
    {
        m_wwn = wwn;
    }
    
    string
    FcoeGlobalFcoeClearMessage::getWWN() const
    {
        return (m_wwn);
    }

    UI32
    FcoeGlobalFcoeClearMessage::getSlotid() const
    {   
        return m_slotid;
    }

    void
    FcoeGlobalFcoeClearMessage::setSlotid(const UI32 &slotid)
    {   
        m_slotid = slotid;
    }

    UI32
    FcoeGlobalFcoeClearMessage::getPo() const
    {   
        return m_po;
    }

    void
    FcoeGlobalFcoeClearMessage::setPo(const UI32 &po)
    {   
        m_po = po;
    }

}
