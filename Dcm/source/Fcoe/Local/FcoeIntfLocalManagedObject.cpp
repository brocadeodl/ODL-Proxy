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
 *   Author : vdharwad                                                     *
 **************************************************************************/
#include "Fcoe/Local/FcoeIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

#include "Framework/Attributes/AttributeEnum.h"

#include "vcs.h"

namespace DcmNs
{

    FcoeIntfLocalManagedObject::FcoeIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager)
        : PrismElement  (pFcoeLocalObjectManager),
        PrismPersistableObject (FcoeIntfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFcoeLocalObjectManager),
        DcmManagedObject (pFcoeLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFcoeLocalObjectManager)
    {
    }

    FcoeIntfLocalManagedObject::FcoeIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager,const string &name, 
                                                            const UI64 &ifindex, const UI32 &fc_uport_num,
                                                            const UI32 &mapped_id, const UI32 &vn_num, const UI32 &port_num,
                                                            const bool &status, const string &mode,const string &binding,
                                                            const UI32 &binding_type)
        : PrismElement  (pFcoeLocalObjectManager),
        PrismPersistableObject (FcoeIntfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFcoeLocalObjectManager),
        DcmManagedObject (pFcoeLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFcoeLocalObjectManager),
        m_name (name),
        m_ifindex (ifindex),
        m_fc_uport_num (fc_uport_num),
        m_mapped_id (mapped_id),
        m_vn_num (vn_num),
        m_port_num (port_num),
        m_status (status),
        m_mode    (mode),
        m_binding    (binding),
        m_binding_type (binding_type)
    {
    }

    FcoeIntfLocalManagedObject::~FcoeIntfLocalManagedObject ()
    {
    }

    string  FcoeIntfLocalManagedObject::getClassName()
    {
        return ("FcoeIntfLocalManagedObject");
    }

    void  FcoeIntfLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_name, "fcoe_if_name"));
        addPersistableAttribute (new AttributeString(&m_fc_port_name, "fc_port_name"));
        addPersistableAttribute (new AttributeUI64(&m_ifindex, "ifindex"));
        addPersistableAttribute (new AttributeUI32(&m_fc_uport_num, "fc_uport_num"));
        addPersistableAttribute (new AttributeUI32(&m_mapped_id, "mapped_id"));
        addPersistableAttribute (new AttributeUI32(&m_vn_num, "vn_num"));
        addPersistableAttribute (new AttributeUI32(&m_port_num, "port_num"));
        addPersistableAttribute (new AttributeBool(&m_status, "status"));
        addPersistableAttribute (new AttributeString(&m_mode,"mode"));
        addPersistableAttribute (new AttributeString(&m_binding,"binding"));
        addPersistableAttribute (new AttributeEnum((UI32 *)&m_binding_type, 0, "binding_type", fcoe_fcoe_interface_bind_type));

        setUserTagForAttribute("status", fcoe_fcoe_interface_shutdown);
        setUserTagForAttribute("name", fcoe_fcoe_interface_name);
        setUserTagForAttribute("binding", fcoe_fcoe_interface_bind_name);
    }

    void  FcoeIntfLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_name, "fcoe_if_name"));
        addPersistableAttributeForCreate  (new AttributeString(&m_fc_port_name, "fc_port_name"));
        addPersistableAttributeForCreate  (new AttributeUI64(&m_ifindex, "ifindex"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_fc_uport_num, "fc_uport_num"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_mapped_id, "mapped_id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vn_num, "vn_num"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_port_num, "port_num"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_status, "status"));
        addPersistableAttributeForCreate  (new AttributeString(&m_mode,"mode"));
        addPersistableAttributeForCreate  (new AttributeString(&m_binding,"binding"));
        addPersistableAttributeForCreate  (new AttributeEnum((UI32 *)&m_binding_type, 0, "binding_type", fcoe_fcoe_interface_bind_type));

        vector<string > keyName;
        keyName.push_back ("fcoe_if_name");
        setUserDefinedKeyCombination (keyName);
    }

    void FcoeIntfLocalManagedObject::setName(const string &name)
    {
        m_name = name;
    }

    string FcoeIntfLocalManagedObject::getName() const
    {
        return m_name;
    }

    void FcoeIntfLocalManagedObject::setIfindex(const UI64 &ifindex)
    {
        m_ifindex = ifindex;
    }

    UI64 FcoeIntfLocalManagedObject::getIfindex() const
    {
        return m_ifindex;
    }

    void FcoeIntfLocalManagedObject::setStatus(const bool &status)
    {
        m_status = status;
    }

    bool FcoeIntfLocalManagedObject::getStatus() const
    {
        return m_status;
    }

    void  FcoeIntfLocalManagedObject::setMode(const string &mode)
    {
        m_mode  =  mode;
    }

    string  FcoeIntfLocalManagedObject::getMode() const
    {
        return (m_mode);
    }

    void  FcoeIntfLocalManagedObject::setBinding(const string &binding)
    {
        m_binding  =  binding;
    }

    string  FcoeIntfLocalManagedObject::getBinding() const
    {
        return (m_binding);
    }

    void FcoeIntfLocalManagedObject::setFcUportNum(const UI32 &fc_uport_num)
    {
        m_fc_uport_num = fc_uport_num;
    }

    UI32 FcoeIntfLocalManagedObject::getFcUportNum() const
    {
        return m_fc_uport_num;
    }

    void FcoeIntfLocalManagedObject::setMappedId(const UI32 &mapped_id)
    {
        m_mapped_id = mapped_id;
    }

    UI32 FcoeIntfLocalManagedObject::getMappedId() const
    {
        return m_mapped_id;
    }

    void FcoeIntfLocalManagedObject::setVnNum(const UI32 &vn_num)
    {
        m_vn_num = vn_num;
    }

    UI32 FcoeIntfLocalManagedObject::getVnNum() const
    {
        return m_vn_num;
    }

    void FcoeIntfLocalManagedObject::setPortNum(const UI32 &port_num)
    {
        m_port_num = port_num;
    }

    UI32 FcoeIntfLocalManagedObject::getPortNum() const
    {
        return m_port_num;
    }

    void FcoeIntfLocalManagedObject::setBindingType(const UI32 &binding_type)
    {
        m_binding_type = binding_type;
    }

    UI32 FcoeIntfLocalManagedObject::getBindingType() const
    {
        return m_binding_type;
    }
}
