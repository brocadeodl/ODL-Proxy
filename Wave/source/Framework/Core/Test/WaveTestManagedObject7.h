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

#ifndef WAVETESTMANAGEDOBJECT7_H
#define WAVETESTMANAGEDOBJECT7_H

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Core/Test/PrismTestManagedObject1.h"

namespace WaveNs
{

class WaveTestManagedObject7 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                            WaveTestManagedObject7   (WaveObjectManager *pWaveObjectManager);
        virtual                                                            ~WaveTestManagedObject7   ();

        static  string                                                      getClassName             ();

        virtual void                                                        setupAttributesForCreate ();

                void                                                        setInteger7              (const UI32 &integer7);
                void                                                        setMessage7              (const string &message7);
                void                                                        setObjectId7             (const ObjectId &objectId7);
                void                                                        setComposition7          (const vector<WaveManagedObjectPointer<PrismTestManagedObject1> > &composition7);

                UI32                                                        getInteger7              () const;
                string                                                      getMessage7              () const;
                ObjectId                                                    getObjectId7             () const;
                vector<WaveManagedObjectPointer<PrismTestManagedObject1> >  getComposition7          () const;

        virtual bool                                                        operator ==              (const WaveTestManagedObject7 &rhs) const;
        virtual bool                                                        operator !=              (const WaveTestManagedObject7 &rhs) const;

    // Now the data members

    private :
        UI32                                                       m_integer7;
        string                                                     m_message7;
        ObjectId                                                   m_objectId7;

        vector<WaveManagedObjectPointer<PrismTestManagedObject1> > m_composition7;

    protected :
    public :
};

}

#endif // WAVETESTMANAGEDOBJECT7_H
