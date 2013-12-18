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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTPOINTER_H
#define WAVEMANAGEDOBJECTPOINTER_H

#include "Framework/Types/Types.h"

#include <iostream>

using namespace std;

namespace WaveNs
{

template<class T> class WaveManagedObjectPointer
{
    private :

        template<class TX> class WaveManagedObjectCountedPointer
        {
            private :
            protected :
            public :
                      WaveManagedObjectCountedPointer (TX *pTX);
                     ~WaveManagedObjectCountedPointer ();

                void  operator ++                     (int);
                void  operator --                     (int);

                UI32  getReferenceCount               () const;

                TX   *operator ->                     ();

                bool  operator ==                     (const WaveManagedObjectCountedPointer<TX> &rhs) const;

            // Now the data members

            private :
                TX   *m_pTX;
                UI32  m_referenceCount;

            protected :
            public :
        };

        void destroy ();

    protected :
    public :
                                     WaveManagedObjectPointer ();
                                     WaveManagedObjectPointer (const WaveManagedObjectPointer<T> &rhs);
                                     WaveManagedObjectPointer (T *pT);
                                    ~WaveManagedObjectPointer ();

        T                           *operator ->              ();
        WaveManagedObjectPointer<T> &operator =               (const WaveManagedObjectPointer<T> &rhs);
        bool                         operator ==              (const WaveManagedObjectPointer<T> &rhs) const;
        bool                         operator !=              (const WaveManagedObjectPointer<T> &rhs) const;

    // Now the data members

    private :
        WaveManagedObjectCountedPointer<T> *m_pWaveManagedObjectCountedPointer;

    protected :
    public :
};

}

#endif // WAVEMANAGEDOBJECTPOINTER_H
