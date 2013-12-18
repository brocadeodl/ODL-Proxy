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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "LockManagement/LockManagementMessagingContext.h"

namespace WaveNs
{

LockManagementMessagingContext::LockManagementMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps) 
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
        m_locationId (0),
        m_lockManagedObject (NULL)
{
}

LockManagementMessagingContext::~LockManagementMessagingContext ()
{
    if (m_lockManagedObject)
    {
        delete (m_lockManagedObject);
        m_lockManagedObject = NULL;
    }
}

LocationId LockManagementMessagingContext::getLocationId () const
{
    return (m_locationId);
}

const string& LockManagementMessagingContext::getServiceString () const
{
    return (m_serviceString);
}

LockManagedObject* LockManagementMessagingContext::getPLockManagedObject () const
{
    return (m_lockManagedObject);
}

void LockManagementMessagingContext::setLocationId (const LocationId locationId)
{
    m_locationId = locationId;
}

void LockManagementMessagingContext::setServiceString (const string &serviceString)
{
    m_serviceString = serviceString;
}

void LockManagementMessagingContext::setPLockManagedObject (LockManagedObject *lockManagedObject)
{
    m_lockManagedObject = lockManagedObject;
}

}


