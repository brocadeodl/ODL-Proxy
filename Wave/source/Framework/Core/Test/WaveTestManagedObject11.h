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

#ifndef WAVETESTMANAGEDOBJECT11_H
#define WAVETESTMANAGEDOBJECT11_H

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Core/Test/WaveTestManagedObject7.h"

namespace WaveNs
{

class WaveTestManagedObject11 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                            WaveTestManagedObject11   (WaveObjectManager *pWaveObjectManager);
        virtual                                                            ~WaveTestManagedObject11   ();

        static  string                                                      getClassName              ();

        virtual void                                                        setupAttributesForCreate  ();

                void                                                        setInteger11              (const UI32 &integer11);
                void                                                        setMessage11              (const string &message11);
                void                                                        setComposition11          (const vector<WaveManagedObjectPointer<WaveTestManagedObject7> > &composition11);

                UI32                                                        getInteger11              () const;
                string                                                      getMessage11              () const;
                vector<WaveManagedObjectPointer<WaveTestManagedObject7> >   getComposition11          () const;

        virtual bool                                                        operator ==               (const WaveTestManagedObject11 &rhs) const;
        virtual bool                                                        operator !=               (const WaveTestManagedObject11 &rhs) const;

    // Now the data members

    private :
        UI32                                                       m_integer11;
        string                                                     m_message11;

        vector<WaveManagedObjectPointer<WaveTestManagedObject7> >  m_composition11;

    protected :
    public :
};

}

#endif // WAVETESTMANAGEDOBJECT7_H
