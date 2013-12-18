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

#ifndef LOCKMANAGEDOBJECT_H
#define LOCKMANAGEDOBJECT_H

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectModel/WaveManagedObject.h"
#include "Framework/Types/DateTime.h"

#include <vector>

using namespace WaveNs;

namespace WaveNs
{

class LockManagedObject : public WaveManagedObject
{
    private :
        virtual void             setupAttributesForPersistence ();
        virtual void             setupAttributesForCreate      ();

    protected :
    public :
                                 LockManagedObject   (WaveObjectManager *pWaveObjectManager);
                                 LockManagedObject   (WaveObjectManager *pWaveObjectManager, const LocationId &locationId, const string &serviceString, const PrismServiceId &serviceId);
        virtual                 ~LockManagedObject   ();

        static string            getClassName               ();

               LocationId        getLocationId              () const;
        const  string&           getServiceString           () const;
               PrismServiceId    getServiceId               () const;
               

               void              setLocationId              (const LocationId locationId);
               void              setServiceString           (const string &serviceString);
               void              setServiceId               (const PrismServiceId serviceId);

               void              resetLockManagedObject     ();
               
            LockManagedObject   &operator=                  (const LockManagedObject &lockManagedObject);             

    // Now the data members

    private :
               LocationId        m_locationId;
               string            m_serviceString;
               PrismServiceId    m_serviceId;

    protected :
    public :
};

}

#endif //LOCKMANAGEDOBJECT_H
