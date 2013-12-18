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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVETESTMANAGEDOBJECT6_H
#define WAVETESTMANAGEDOBJECT6_H

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Core/Test/PrismTestManagedObject1.h"

namespace WaveNs
{

class WaveTestManagedObject6 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                   WaveTestManagedObject6   (WaveObjectManager *pWaveObjectManager);
        virtual                                                   ~WaveTestManagedObject6   ();

        static  string                                             getClassName             ();

        virtual void                                               setupAttributesForCreate ();

                void                                               setInteger6              (const UI32 &integer6);
                void                                               setMessage6              (const string &message6);
                void                                               setObjectId6             (const ObjectId &objectId6);
                void                                               setComposition6          (const WaveManagedObjectPointer<PrismTestManagedObject1> &composition6);

                UI32                                               getInteger6              () const;
                string                                             getMessage6              () const;
                ObjectId                                           getObjectId6             () const;
                WaveManagedObjectPointer<PrismTestManagedObject1>  getComposition6          () const;

        virtual bool                                               operator ==              (const WaveTestManagedObject6 &rhs) const;
        virtual bool                                               operator !=              (const WaveTestManagedObject6 &rhs) const;

    // Now the data members

    private :
        UI32                                                       m_integer6;
        string                                                     m_message6;
        ObjectId                                                   m_objectId6;

        WaveManagedObjectPointer<PrismTestManagedObject1>          m_composition6;

    protected :
    public :
};

}

#endif // WAVETESTMANAGEDOBJECT6_H
