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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#ifndef LOCKMANAGEMENTMESSAGINGCONTEXT_h
#define LOCKMANAGEMENTMESSAGINGCONTEXT_h

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "LockManagement/LockManagedObject.h"

namespace WaveNs
{

class LockManagementMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                    LockManagementMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                   ~LockManagementMessagingContext ();
       LocationId                   getLocationId () const;
       const string&                getServiceString () const;
       LockManagedObject*           getPLockManagedObject () const;
       void                         setLocationId (const LocationId locationId);
       void                         setServiceString (const string &serviceString);
       void                         setPLockManagedObject (LockManagedObject* lockManagedObject);


    // Now the data members

    private :
        LocationId                  m_locationId;
        string                      m_serviceString;
        LockManagedObject*          m_lockManagedObject;

    protected :
    public :
};

}

#endif // LOCKMANAGEMENTMESSAGINGCONTEXT_h

