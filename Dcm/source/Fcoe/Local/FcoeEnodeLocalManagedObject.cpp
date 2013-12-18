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
 *   Author : MMREDDY                                                      *
 **************************************************************************/

#include "Fcoe/Local/FcoeEnodeLocalManagedObject.h"
#include "Fcoe/Local/FcoeLocalObjectManager.h"
#include "Fcoe/Local/FcoeLocalTypes.h"
#include "Fcoe/Common/FcoePluginTypes.h"
#include "Fcoe/Common/FcoePluginUtils.h"

#include "vcs.h"

namespace DcmNs
{

    FcoeEnodeLocalManagedObject::FcoeEnodeLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager)
        : PrismElement  (pFcoeLocalObjectManager),
        PrismPersistableObject (FcoeEnodeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFcoeLocalObjectManager),
        DcmManagedObject (pFcoeLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFcoeLocalObjectManager)
    {
    }

    FcoeEnodeLocalManagedObject::FcoeEnodeLocalManagedObject (FcoeLocalObjectManager *pFcoeLocalObjectManager, string fcoe_enodes_fab_map, uint32_t fcoe_enodes, bool upgrade)
        : PrismElement  (pFcoeLocalObjectManager),
        PrismPersistableObject (FcoeEnodeLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFcoeLocalObjectManager),
        DcmManagedObject (pFcoeLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFcoeLocalObjectManager),
        m_fcoe_enodes_fab_map(fcoe_enodes_fab_map),
        m_fcoe_enodes (fcoe_enodes),
        m_upgrade (upgrade)
    {
    }

    FcoeEnodeLocalManagedObject::~FcoeEnodeLocalManagedObject ()
    {
    }

    string  FcoeEnodeLocalManagedObject::getClassName()
    {
        return ("FcoeEnodeLocalManagedObject");
    }

    void  FcoeEnodeLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeString(&m_fcoe_enodes_fab_map, "fcoe_enodes_fab_map", fcoe_fcoe_enode_fabric_map_name));
        addPersistableAttribute (new AttributeUI32(&m_fcoe_enodes, "fcoe_enodes", fcoe_fcoe_max_enode));
        addPersistableAttribute (new AttributeBool(&m_upgrade, "upgrade"));
    }

    void  FcoeEnodeLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeString(&m_fcoe_enodes_fab_map, "fcoe_enodes_fab_map", fcoe_fcoe_enode_fabric_map_name));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_fcoe_enodes, "fcoe_enodes", fcoe_fcoe_max_enode));
        addPersistableAttributeForCreate  (new AttributeBool(&m_upgrade, "upgrade"));
       
    }
    void  FcoeEnodeLocalManagedObject::setupKeys()
    {
        vector<string > keyName;
        keyName.push_back ("fcoe_enodes_fab_map");
        setUserDefinedKeyCombination (keyName);
    }

    void  FcoeEnodeLocalManagedObject::setFcoeEnodes(const UI32 &fcoe_enodes)
    {
        m_fcoe_enodes  =  fcoe_enodes;
    }

    UI32  FcoeEnodeLocalManagedObject::getFcoeEnodes() const
    {
        return (m_fcoe_enodes);
    }
    void  FcoeEnodeLocalManagedObject::setFcoeEnodesUpgradeFlag(const bool &upgrade)
    {
        m_upgrade  =  upgrade;
    }

    bool  FcoeEnodeLocalManagedObject::getFcoeEnodesUpgradeFlag() const
    {
        return (m_upgrade);
    }
    void  FcoeEnodeLocalManagedObject::setFcoeEnodesFabricMap(const string &fcoe_enodes_fab_map)
    {
        m_fcoe_enodes_fab_map  =  fcoe_enodes_fab_map;
    }

    string  FcoeEnodeLocalManagedObject::getFcoeEnodesFabricMap() const
    {
        return (m_fcoe_enodes_fab_map);
    } 
}

