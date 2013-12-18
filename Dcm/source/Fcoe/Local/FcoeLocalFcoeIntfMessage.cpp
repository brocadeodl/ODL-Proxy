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
#include "Fcoe/Local/FcoeLocalFcoeIntfMessage.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

namespace DcmNs
{

    FcoeLocalFcoeIntfMessage::FcoeLocalFcoeIntfMessage ()
        : ManagementInterfaceMessage (FcoeLocalObjectManager::getClassName (),FCOE_LOCAL_UPD_FCOE_INTF)
    {
    }

    FcoeLocalFcoeIntfMessage::FcoeLocalFcoeIntfMessage (const UI32 &oper_bmp,
                                                        const string &ifName,
                                                        const bool &status,
                                                        const UI64 &ifIndex,
                                                        const UI32 &fc_uport_num,
                                                        const UI32 &vn_num,
                                                        const UI32 &port_num,
                                                        const string &desc,
                                                        const string &mode,
                                                        const string &binding,
                                                        const UI32 &binding_type)
        : ManagementInterfaceMessage (FcoeLocalObjectManager::getClassName (),
                                      FCOE_LOCAL_UPD_FCOE_INTF),
        m_oper_bmp    (oper_bmp),
        m_ifName    (ifName),
        m_status    (status),
        m_ifIndex    (ifIndex),
        m_fc_uport_num (fc_uport_num),
        m_vn_num (vn_num),
        m_port_num (port_num),
        m_desc    (desc),
        m_mode    (mode),
        m_binding    (binding),
        m_binding_type (binding_type)
    {
    }

    FcoeLocalFcoeIntfMessage::~FcoeLocalFcoeIntfMessage ()
    {
    }

    void  FcoeLocalFcoeIntfMessage::setupAttributesForSerialization()
    {
        ManagementInterfaceMessage::setupAttributesForSerialization ();
        addSerializableAttribute (new AttributeUI32(&m_oper_bmp,"oper_bmp"));
        addSerializableAttribute (new AttributeString(&m_ifName,"ifName"));
        addSerializableAttribute (new AttributeUI64(&m_ifIndex,"ifIndex"));
        addSerializableAttribute (new AttributeString(&m_desc,"desc"));
        addSerializableAttribute (new AttributeUI32(&m_fc_uport_num, "fc_uport_num"));
        addSerializableAttribute (new AttributeUI32(&m_vn_num, "VN_NUM"));
        addSerializableAttribute (new AttributeUI32(&m_port_num, "Port_Num"));
        addSerializableAttribute (new AttributeBool(&m_status,"status"));
        addSerializableAttribute (new AttributeString(&m_mode,"mode"));
        addSerializableAttribute (new AttributeString(&m_binding,"binding"));
        addSerializableAttribute (new AttributeUI32(&m_binding_type,"binding_type"));
    }

    void  FcoeLocalFcoeIntfMessage::setOper_bmp(const UI32 &oper_bmp)
    {
        m_oper_bmp  =  oper_bmp;
    }

    UI32  FcoeLocalFcoeIntfMessage::getOper_bmp() const
    {
        return (m_oper_bmp);
    }

    void  FcoeLocalFcoeIntfMessage::setIfName(const string &ifName)
    {
        m_ifName  =  ifName;
    }

    string  FcoeLocalFcoeIntfMessage::getIfName() const
    {
        return (m_ifName);
    }

    void  FcoeLocalFcoeIntfMessage::setIfIndex(const UI64 &ifIndex)
    {
        m_ifIndex  =  ifIndex;
    }

    UI64  FcoeLocalFcoeIntfMessage::getIfIndex() const
    {
        return (m_ifIndex);
    }

    void  FcoeLocalFcoeIntfMessage::setDesc(const string &desc)
    {
        m_desc  =  desc;
    }

    string  FcoeLocalFcoeIntfMessage::getDesc() const
    {
        return (m_desc);
    }

    void  FcoeLocalFcoeIntfMessage::setStatus(const bool &status)
    {
        m_status  =  status;
    }

    bool  FcoeLocalFcoeIntfMessage::getStatus() const
    {
        return (m_status);
    }

    void  FcoeLocalFcoeIntfMessage::setMode(const string &mode)
    {
        m_mode  =  mode;
    }

    string  FcoeLocalFcoeIntfMessage::getMode() const
    {
        return (m_mode);
    }

    void  FcoeLocalFcoeIntfMessage::setBinding(const string &binding)
    {
        m_binding  =  binding;
    }

    string  FcoeLocalFcoeIntfMessage::getBinding() const
    {
        return (m_binding);
    }

    void  FcoeLocalFcoeIntfMessage::setBindingType(const UI32 &binding_type)
    {
        m_binding_type  =  binding_type;
    }

    UI32  FcoeLocalFcoeIntfMessage::getBindingType() const
    {
        return (m_binding_type);
    }
    
    void FcoeLocalFcoeIntfMessage::setFcUportNum (const UI32 &fc_uport_num)
    {
        m_fc_uport_num = fc_uport_num;
    }
    
    UI32 FcoeLocalFcoeIntfMessage::getFcUportNum() const
    {
        return m_fc_uport_num;
    }

    void FcoeLocalFcoeIntfMessage::setVnNum (const UI32 &vn_num)
    {
        m_vn_num = vn_num;
    }

    UI32 FcoeLocalFcoeIntfMessage::getVnNum () const
    {
        return (m_vn_num);
    }

    void FcoeLocalFcoeIntfMessage::setPortNum (const UI32 &port_num)
    {
        m_port_num = port_num;
    }

    UI32 FcoeLocalFcoeIntfMessage::getPortNum () const
    {
        return m_port_num;
    }
}
