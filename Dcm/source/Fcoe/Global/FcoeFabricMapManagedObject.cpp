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

#include "Fcoe/Global/FcoeFabricMapManagedObject.h"
#include "Fcoe/Global/FcoeGlobalObjectManager.h"
#include "Fcoe/Global/FcoeGlobalTypes.h"

#include "vcs.h"

namespace DcmNs
{

    FcoeFabricMapManagedObject::FcoeFabricMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeFabricMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager)
    {
    }

    FcoeFabricMapManagedObject::FcoeFabricMapManagedObject (FcoeGlobalObjectManager *pFcoeGlobalObjectManager,const string &fabric_map_name,const UI32 &vlan,const SI32 &priority,const string &fcmap,const SI32 &vfid,const SI32 &fka_intvl,const bool &timeout, const SI32 &mtu, const UI32 &max_enodes, const string &enodes_cfg_mode)
        : PrismElement  (pFcoeGlobalObjectManager),
        PrismPersistableObject (FcoeFabricMapManagedObject::getClassName (), DcmManagedObject::getClassName ()),
        WaveManagedObject (pFcoeGlobalObjectManager),
        DcmManagedObject (pFcoeGlobalObjectManager),
        m_fabric_map_name    (fabric_map_name),
        m_vlan    (vlan),
        m_priority    (priority),
        m_fcmap    (fcmap),
        m_vfid    (vfid),
        m_fka_intvl    (fka_intvl),
        m_timeout    (timeout),
        m_mtu     (mtu),
        m_max_enodes (max_enodes),
        m_enodes_cfg_mode(enodes_cfg_mode)
    {
    }

    FcoeFabricMapManagedObject::~FcoeFabricMapManagedObject ()
    {
    }

    string  FcoeFabricMapManagedObject::getClassName()
    {
        return ("FcoeFabricMapManagedObject");
    }

    void  FcoeFabricMapManagedObject::setupAttributesForPersistence()
    {
        DcmManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addPersistableAttribute (new AttributeUI32(&m_vlan,"vlan"));
        addPersistableAttribute (new AttributeSI32(&m_priority,"priority"));
        addPersistableAttribute (new AttributeString(&m_fcmap,"fcmap"));
        addPersistableAttribute (new AttributeSI32(&m_vfid,"vfid"));
        addPersistableAttribute (new AttributeSI32(&m_fka_intvl,"fka_intvl"));
        addPersistableAttribute (new AttributeBool(&m_timeout,"timeout"));
        addPersistableAttribute (new AttributeSI32(&m_mtu,"mtu"));
        addPersistableAttribute (new AttributeUI32(&m_max_enodes, "max_enodes"));
        addPersistableAttribute (new AttributeString(&m_enodes_cfg_mode, "enodes_cfg_mode"));

        setUserTagForAttribute("fabric_map_name", fcoe_fcoe_fabric_map_name);
        setUserTagForAttribute("vlan", fcoe_fcoe_fabric_map_vlan);
        setUserTagForAttribute("priority", fcoe_fcoe_fabric_map_priority);
        setUserTagForAttribute("fcmap", fcoe_fcoe_fabric_map_fcmap);
        setUserTagForAttribute("vfid", fcoe_fcoe_fabric_map_virtual_fabric);
        setUserTagForAttribute("fka_intvl", fcoe_fcoe_fip_advertisement_interval);
        setUserTagForAttribute("timeout", fcoe_fcoe_fip_keep_alive_timeout);
        // setUserTagForAttribute("mtu", fcoe_fcoe_fabric_map_mtu);
        setUserTagForAttribute("max_enodes", fcoe_fcoe_fabric_map_max_enodes);
        setUserTagForAttribute("enodes_cfg_mode", fcoe_fcoe_fabric_map_enodes_cfg);
    }

    void  FcoeFabricMapManagedObject::setupAttributesForCreate()
    {
        DcmManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_fabric_map_name,"fabric_map_name"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_vlan,"vlan"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_priority,"priority"));
        addPersistableAttributeForCreate  (new AttributeString(&m_fcmap,"fcmap"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_vfid,"vfid"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_fka_intvl,"fka_intvl"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_timeout,"timeout"));
        addPersistableAttributeForCreate  (new AttributeSI32(&m_mtu,"mtu"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_max_enodes, "max_enodes"));
        addPersistableAttributeForCreate  (new AttributeString(&m_enodes_cfg_mode, "enodes_cfg_mode"));

        vector<string > keyName;
        keyName.push_back ("fabric_map_name");
        setUserDefinedKeyCombination (keyName);
    }

    void  FcoeFabricMapManagedObject::setName(const string &fabric_map_name)
    {
        m_fabric_map_name  =  fabric_map_name;
    }

    string  FcoeFabricMapManagedObject::getName() const
    {
        return (m_fabric_map_name);
    }

    void  FcoeFabricMapManagedObject::setVlan(const UI32 &vlan)
    {
        m_vlan  =  vlan;
    }

    UI32  FcoeFabricMapManagedObject::getVlan() const
    {
        return (m_vlan);
    }

    void  FcoeFabricMapManagedObject::setPriority(const SI32 &priority)
    {
        m_priority  =  priority;
    }

    SI32  FcoeFabricMapManagedObject::getPriority() const
    {
        return (m_priority);
    }

    void  FcoeFabricMapManagedObject::setFcmap(const string &fcmap)
    {
        m_fcmap  =  fcmap;
    }

    string  FcoeFabricMapManagedObject::getFcmap() const
    {
        return (m_fcmap);
    }

    void  FcoeFabricMapManagedObject::setVfid(const SI32 &vfid)
    {
        m_vfid  =  vfid;
    }

    SI32  FcoeFabricMapManagedObject::getVfid() const
    {
        return (m_vfid);
    }

    void  FcoeFabricMapManagedObject::setFka_intvl(const SI32 &fka_intvl)
    {
        m_fka_intvl  =  fka_intvl;
    }

    SI32  FcoeFabricMapManagedObject::getFka_intvl() const
    {
        return (m_fka_intvl);
    }

    void  FcoeFabricMapManagedObject::setTimeout(const bool &timeout)
    {
        m_timeout  =  timeout;
    }

    bool  FcoeFabricMapManagedObject::getTimeout() const
    {
        return (m_timeout);
    }

    void  FcoeFabricMapManagedObject::setMtu(const SI32 &mtu)
    {
        m_mtu  =  mtu;
    }

    SI32  FcoeFabricMapManagedObject::getMtu() const
    {
        return (m_mtu);
    }

    void  FcoeFabricMapManagedObject::setMaxEnodes(const UI32 &max_enodes)
    {
        m_max_enodes  =  max_enodes;
    }

    UI32  FcoeFabricMapManagedObject::getMaxEnodes() const
    {
        return (m_max_enodes);
    }

   void FcoeFabricMapManagedObject::setEnodesCfgMode(const string &enodes_cfg_mode)
   {

     m_enodes_cfg_mode = enodes_cfg_mode;
    
   }
   string FcoeFabricMapManagedObject::getEnodesCfgMode() const
   {

     return (m_enodes_cfg_mode); 
    
    }
}
