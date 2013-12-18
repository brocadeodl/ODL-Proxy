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
 *   Author : rjain                                                     *
 **************************************************************************/

#include "Pim/Local/MultRtrCfgLocalManagedObject.h"
#include "Pim/Local/PimLocalObjectManager.h"
#include "Pim/Local/PimTypes.h"
#include "vcs.h"

namespace DcmNs
{

    MultRtrCfgLocalManagedObject::MultRtrCfgLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (MultRtrCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager)
    {
    }

    MultRtrCfgLocalManagedObject::MultRtrCfgLocalManagedObject (PimLocalObjectManager *pPimLocalObjectManager,
        const bool &presenceFlag,const bool &enable)
        : PrismElement  (pPimLocalObjectManager),
        PrismPersistableObject (MultRtrCfgLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pPimLocalObjectManager),
        DcmManagedObject (pPimLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pPimLocalObjectManager),
        m_presenceFlag    (presenceFlag),
        m_enable    (enable)
    {
    }

    MultRtrCfgLocalManagedObject::~MultRtrCfgLocalManagedObject ()
    {
    }

    string  MultRtrCfgLocalManagedObject::getClassName()
    {
        return ("MultRtrCfgLocalManagedObject");
    }

    void  MultRtrCfgLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeBool(&m_presenceFlag,"presenceFlag"));
        addPersistableAttribute (new AttributeBool(&m_enable,"enable"));

        /* Adding the XML tag mappings */
       /* setUserTagForAttribute("presenceFlag", pim_mult_rtr_cont);
        setUserTagForAttribute("enable", pim_multicast_routing);*/
    }

    void  MultRtrCfgLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeBool(&m_presenceFlag,"presenceFlag"));
        addPersistableAttributeForCreate  (new AttributeBool(&m_enable,"enable"));
    }

    void  MultRtrCfgLocalManagedObject::setPresenceFlag(const bool &presenceFlag)
    {
        m_presenceFlag  =  presenceFlag;
    }

    bool  MultRtrCfgLocalManagedObject::getPresenceFlag() const
    {
        return (m_presenceFlag);
    }

    void  MultRtrCfgLocalManagedObject::setEnable(const bool &enable)
    {
        m_enable  =  enable;
    }

    bool  MultRtrCfgLocalManagedObject::getEnable() const
    {
        return (m_enable);
    }

}
