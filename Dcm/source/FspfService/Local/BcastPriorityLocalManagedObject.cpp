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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : krangara                                                     *
 **************************************************************************/

#include "FspfService/Local/BcastPriorityLocalManagedObject.h"
#include "FspfService/Local/FspfServiceLocalObjectManager.h"
//#include "FspfService/Local/FspfServiceTypes.h"
#include "vcs.h"
//#include "brocade-fabric-service.h"

namespace DcmNs
{

    BcastPriorityLocalManagedObject::BcastPriorityLocalManagedObject (FspfServiceLocalObjectManager *pFspfServiceLocalObjectManager)
        : PrismElement  (pFspfServiceLocalObjectManager),
        PrismPersistableObject (BcastPriorityLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFspfServiceLocalObjectManager),
        DcmManagedObject (pFspfServiceLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFspfServiceLocalObjectManager)
    {
    }

    BcastPriorityLocalManagedObject::BcastPriorityLocalManagedObject (FspfServiceLocalObjectManager *pFspfServiceLocalObjectManager,const UI32 &switchId,const UI32 &bcastPriority)
        : PrismElement  (pFspfServiceLocalObjectManager),
        PrismPersistableObject (BcastPriorityLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pFspfServiceLocalObjectManager),
        DcmManagedObject (pFspfServiceLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pFspfServiceLocalObjectManager),
        m_switchId    (switchId),    
        m_bcastPriority    (MCAST_PRIORITY_DEFAULT)
    {
    }

    BcastPriorityLocalManagedObject::~BcastPriorityLocalManagedObject ()
    {
    }

    string  BcastPriorityLocalManagedObject::getClassName()
    {
        return ("BcastPriorityLocalManagedObject");
    }

    void  BcastPriorityLocalManagedObject::setupAttributesForPersistence()
    {
        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_switchId,"switchId"));
        addPersistableAttribute (new AttributeUI32(&m_bcastPriority,"bcastPriority"));

        setUserTagForAttribute ("switchId", fabric_service_rbridge_id);
        setUserTagForAttribute ("bcastPriority", fabric_service_priority);
    }

    void  BcastPriorityLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new AttributeUI32(&m_switchId,"switchId"));
        addPersistableAttributeForCreate  (new AttributeUI32(&m_bcastPriority,"bcastPriority"));

        vector<string> keyName;
        keyName.push_back ("switchId");
        setUserDefinedKeyCombination (keyName);
    }

    void  BcastPriorityLocalManagedObject::setSwitchId(const UI32 &switchId)
    {
        m_switchId  =  switchId;
    }

    UI32  BcastPriorityLocalManagedObject::getSwitchId() const
    {
        return (m_switchId);
    }

    void  BcastPriorityLocalManagedObject::setBcastPriority(const UI32 &bcastPriority)
    {
        m_bcastPriority  =  bcastPriority;
    }

    UI32  BcastPriorityLocalManagedObject::getBcastPriority() const
    {
        return (m_bcastPriority);
    }

}
