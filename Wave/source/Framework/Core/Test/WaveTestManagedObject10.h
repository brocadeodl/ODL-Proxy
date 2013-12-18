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

#ifndef WAVETESTMANAGEDOBJECT10_H
#define WAVETESTMANAGEDOBJECT10_H

#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Core/Test/WaveTestManagedObject6.h"

namespace WaveNs
{

class WaveTestManagedObject10 : public WaveManagedObject
{
    private :
    protected :
        virtual void setupAttributesForPersistence ();

    public :
                                                                   WaveTestManagedObject10   (WaveObjectManager *pWaveObjectManager);
        virtual                                                   ~WaveTestManagedObject10   ();

        static  string                                             getClassName             ();

        virtual void                                               setupAttributesForCreate ();

                void                                               setInteger10              (const UI32 &integer10);
                void                                               setMessage10              (const string &message10);
                void                                               setComposition10          (const WaveManagedObjectPointer<WaveTestManagedObject6> &composition10);

                UI32                                               getInteger10              () const;
                string                                             getMessage10              () const;
                WaveManagedObjectPointer<WaveTestManagedObject6>   getComposition10          () const;

        virtual bool                                               operator ==              (const WaveTestManagedObject10 &rhs) const;
        virtual bool                                               operator !=              (const WaveTestManagedObject10 &rhs) const;

    // Now the data members

    private :
        UI32                                                       m_integer10;
        string                                                     m_message10;

        WaveManagedObjectPointer<WaveTestManagedObject6>           m_composition10;

    protected :
    public :
};

}

#endif // WAVETESTMANAGEDOBJECT10_H
