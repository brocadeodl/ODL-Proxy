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
 *   Author : vsatyana                                                     *
 **************************************************************************/

#include "Ssm/Local/SsmLocalObjectManager.h"
#include "Ssm/Local/SsmTypes.h"
#include "ClientInterface/Ssm/SsmApp.h"
#include "Ssm/Local/BpratelimitLocalManagedObject.h"
#include "vcs.h"

namespace DcmNs
{

    BpratelimitLocalManagedObject::BpratelimitLocalManagedObject (SsmLocalObjectManager *pSsmLocalObjectManager)
        : PrismElement  (pSsmLocalObjectManager),
        PrismPersistableObject (BpratelimitLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSsmLocalObjectManager),
        DcmManagedObject (pSsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSsmLocalObjectManager)
    {
    }
    BpratelimitLocalManagedObject::BpratelimitLocalManagedObject (SsmLocalObjectManager *pSsmLocalObjectManager,const UI32  &slot)
        : PrismElement  (pSsmLocalObjectManager),
        PrismPersistableObject (BpratelimitLocalManagedObject::getClassName (), DcmLocalManagedObject::getClassName ()),
        WaveManagedObject (pSsmLocalObjectManager),
        DcmManagedObject (pSsmLocalObjectManager),
        WaveLocalManagedObjectBase (this),
        DcmLocalManagedObjectBase (this),
        DcmLocalManagedObject (pSsmLocalObjectManager),
	    m_slot	   (slot)
    {
    }

    BpratelimitLocalManagedObject::~BpratelimitLocalManagedObject ()
    {
    }


    string  BpratelimitLocalManagedObject::getClassName()
    {
        return ("BpratelimitLocalManagedObject");
    }

    void  BpratelimitLocalManagedObject::setupAttributesForPersistence()
    {

        DcmLocalManagedObject::setupAttributesForPersistence ();
        addPersistableAttribute (new AttributeUI32(&m_slot,"slotId"));
        setUserTagForAttribute ("slotId",bprate_limit_bp_rate_limit_slot_num);
    }

    void  BpratelimitLocalManagedObject::setupAttributesForCreate()
    {
        DcmLocalManagedObject::setupAttributesForCreate ();
        addPersistableAttributeForCreate  (new  AttributeUI32(&m_slot,"slotId"));
        vector<string> keyName;
        keyName.push_back ("slotId");
        setUserDefinedKeyCombination (keyName);
    }



    void  BpratelimitLocalManagedObject::setSlot(const UI32 &slotId)
    { 
            m_slot  = slotId;
    }

    UI32  BpratelimitLocalManagedObject::getSlot()
    {
       return  ( m_slot);
    }

}

