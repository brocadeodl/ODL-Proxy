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
 *   Author : krao                                                     *
 **************************************************************************/

#include "VRRP/Local/VRRPGlobalConfigManagedObject.h"
#include "VRRP/Local/VRRPLocalObjectManager.h"
#include "vcs.h"

namespace DcmNs
{

    VRRPGlobalConfigManagedObject::VRRPGlobalConfigManagedObject (VRRPLocalObjectManager *pVRRPLocalObjectManager)
        : PrismElement  (pVRRPLocalObjectManager),
        PrismPersistableObject (VRRPGlobalConfigManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pVRRPLocalObjectManager),
        DcmManagedObject (pVRRPLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pVRRPLocalObjectManager)

    {
		m_vrrp_enable = 0;
		m_vrrpe_enable = 0;
    }

    VRRPGlobalConfigManagedObject::VRRPGlobalConfigManagedObject (VRRPLocalObjectManager *pVRRPLocalObjectManager,const bool &vrrp_config,const bool &vrrpe_config)
        : PrismElement  (pVRRPLocalObjectManager),
        PrismPersistableObject (VRRPGlobalConfigManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pVRRPLocalObjectManager),
        DcmManagedObject (pVRRPLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pVRRPLocalObjectManager),
        m_vrrp_enable    (vrrp_config),
        m_vrrpe_enable    (vrrpe_config)
    {
    }

    VRRPGlobalConfigManagedObject::~VRRPGlobalConfigManagedObject ()
    {
    }

    string  VRRPGlobalConfigManagedObject::getClassName()
    {
        return ("VRRPGlobalConfigManagedObject");
    }

    void  VRRPGlobalConfigManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_vrrp_enable,"vrrp_enable", vrrp_vrrp));
        addPersistableAttribute (new AttributeBool(&m_vrrpe_enable,"vrrpe_enable", vrrp_vrrp_extended));
    }

    void  VRRPGlobalConfigManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_vrrp_enable,"vrrp_enable", vrrp_vrrp));
        addPersistableAttributeForCreate  (new AttributeBool(&m_vrrpe_enable,"vrrpe_enable", vrrp_vrrp_extended));
    }

    void  VRRPGlobalConfigManagedObject::setVrrpEnable(const bool &enable)
    {
        m_vrrp_enable  =  enable;
    }

    bool  VRRPGlobalConfigManagedObject::getVrrpEnable() const
    {
        return (m_vrrp_enable);
    }

    void  VRRPGlobalConfigManagedObject::setVrrpeEnable(const bool &enable)
    {
        m_vrrpe_enable  =  enable;
    }

    bool  VRRPGlobalConfigManagedObject::getVrrpeEnable() const
    {
        return (m_vrrpe_enable);
    }

}
