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
#include "Fcoe/Local/FcoeETIntfLocalManagedObject.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    FcoeETIntfLocalManagedObject::FcoeETIntfLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager)
        : PrismElement  (pFcoeLocalObjectManager),
        PrismPersistableObject (FcoeETIntfLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFcoeLocalObjectManager),
        DcmManagedObject (pFcoeLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFcoeLocalObjectManager)
    {
    }

    FcoeETIntfLocalManagedObject::~FcoeETIntfLocalManagedObject ()
    {
    }

    string  FcoeETIntfLocalManagedObject::getClassName()
    {
        return ("FcoeETIntfLocalManagedObject");
    }

    void  FcoeETIntfLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_name, "et_if_name"));
        addPersistableAttribute (new AttributeUI64(&m_ifindex, "ifindex"));
        addPersistableAttribute (new AttributeSI32(&m_fc_uport_num, "fc_uport_num"));
        addPersistableAttribute (new AttributeBool(&m_status, "status"));
        addPersistableAttribute (new AttributeBool(&m_trunk_status, "trunk_status"));
        addPersistableAttribute (new AttributeUI32(&m_rb_id, "rb_id"));
        addPersistableAttribute (new AttributeUI32(&m_bind_type, "bind_type"));
        addPersistableAttribute (new AttributeBool(&m_breakout, "breakout"));

        setUserTagForAttribute("status", fcoe_fabric_isl_enable);
        setUserTagForAttribute("trunk_status", fcoe_fabric_trunk_enable);
        setUserTagForAttribute("et_if_name", fcoe_fcoe_interface_name);
    }

    void  FcoeETIntfLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_name, "et_if_name"));
        addPersistableAttributeForCreate  (new AttributeUI64(&m_ifindex, "ifindex"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_fc_uport_num, "fc_uport_num"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_status, "status"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_trunk_status, "trunk_status"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_rb_id, "rb_id"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_bind_type, "bind_type"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_breakout, "breakout"));

        vector<string > keyName;
        keyName.push_back ("et_if_name");
        setUserDefinedKeyCombination (keyName);
    }

    void FcoeETIntfLocalManagedObject::setName(const string &name)
    {
        m_name = name;
    }

    string FcoeETIntfLocalManagedObject::getName() const
    {
        return m_name;
    }

    void FcoeETIntfLocalManagedObject::setIfindex(const UI64 &ifindex)
    {
        m_ifindex = ifindex;
    }

    UI64 FcoeETIntfLocalManagedObject::getIfindex() const
    {
        return m_ifindex;
    }

    void FcoeETIntfLocalManagedObject::setStatus(const bool &status)
    {
        m_status = status;
    }

    bool FcoeETIntfLocalManagedObject::getStatus() const
    {
        return m_status;
    }

    void FcoeETIntfLocalManagedObject::setFcUportNum(const SI32 &fc_uport_num)
    {
        m_fc_uport_num = fc_uport_num;
    }

    SI32 FcoeETIntfLocalManagedObject::getFcUportNum() const
    {
        return m_fc_uport_num;
    }

    void FcoeETIntfLocalManagedObject::setRbId(const UI32 &rb_id)
    {
        m_rb_id = rb_id;
    }

    UI32 FcoeETIntfLocalManagedObject::getRbId() const
    {
        return m_rb_id;
    }

    void FcoeETIntfLocalManagedObject::setBindType(const UI32 &bind_type)
    {
        m_bind_type = bind_type;
    }

    UI32 FcoeETIntfLocalManagedObject::getBindType() const
    {
        return m_bind_type;
    }

    void FcoeETIntfLocalManagedObject::setTrunkStatus(const bool &status)
    {
        m_trunk_status = status;
    }

    bool FcoeETIntfLocalManagedObject::getTrunkStatus() const
    {
        return m_trunk_status;
    }

    void FcoeETIntfLocalManagedObject::setAsBreakoutPort(const bool &breakout)
    {   
        m_breakout = breakout;
    }

    bool FcoeETIntfLocalManagedObject::isBreakoutPort() const
    {   
        return m_breakout;
    }

}
