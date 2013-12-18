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
 *   Author : Awanish Kumar Trivedi                                        *
 ***************************************************************************/

#include "Framework/ObjectModel/CommonManagedObjectBase.h"
#include "Framework/ObjectModel/PrismPersistableObject.h"
#include "Framework/Utils/TraceUtils.h"

using namespace std;

namespace WaveNs
{

CommonManagedObjectBase::CommonManagedObjectBase (PrismPersistableObject *pPrismPersistableObject)
    : m_pPrismPersistableObject (pPrismPersistableObject)
{
    // Do not request for tracking if the Wave Managed Object is being instantiated because of query
#if 0
    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManager::getPrismServiceId ()))->getId ())
    {
        pWaveObjectManager->trackObjectCreatedDuringCurrentTransaction (this);

        ObjectTracker::addToObjectTracker (this);
    }

    m_lastModifiedTimeStamp = m_createdTimeStamp;
#endif
}

CommonManagedObjectBase::~CommonManagedObjectBase ()
{
    // Request Object Manager to track this object if necessary.
    // Do not request for tracking if the Wave Managed Object is being instantiated because of query
#if 0
    if ((PrismThread::getSelf ()) != (PrismThread::getPrismThreadForServiceId (DatabaseObjectManager::getPrismServiceId ()))->getId ())
    {
        m_pCurrentOwnerWaveObjectManager->trackObjectDeletedDuringCurrentTransaction (this);
    }

    ObjectTracker::deleteFromObjectTracker (this);
#endif
}

void CommonManagedObjectBase::setupAttributesForPersistence ()
{
}

//#if 0
void CommonManagedObjectBase::addPersistableAttributeForCommonBase(Attribute *pAttribute)
{
    m_pPrismPersistableObject->addPersistableAttribute(pAttribute);
}

void CommonManagedObjectBase::addPersistableAttributeForCommonBaseCreate(Attribute *pAttribute)
{
    m_pPrismPersistableObject->addPersistableAttributeForCreate(pAttribute);
}
//#endif

void CommonManagedObjectBase::setupAttributesForCreate ()
{
}


}
