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
 *   Copyright (C) 2005-2008 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef MULTIBYTEWORD_H
#define MULTIBYTEWORD_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class MultiByteWord
{
    private :
        void construct (const UI32 &polynomialDegree);
        void destroy   ();

    protected :
    public :
                             MultiByteWord       (const UI32 &polynomialDegree = 32);
                             MultiByteWord       (const MultiByteWord &rhs);
                            ~MultiByteWord       ();

              UI32           getNumberOfWords    () const;
              UI32           getPolynomialDegree () const;
              void           setPolynomialDegree (const UI32 &polynomialDegree);
              void           setBit              (const UI32 &index);
              void           unsetBit            (const UI32 &index);
              bool           isBitSet            (const UI32 &index);

              MultiByteWord &operator=           (const MultiByteWord &rhs);
              MultiByteWord &operator=           (const UI32 &rhs);
              UI32          &operator[]          (const UI32 &index);
              MultiByteWord &operator<<=         (const UI32 &numberOfBitsToShift);
              MultiByteWord  operator<<          (const UI32 &numberOfBitsToShift);
              MultiByteWord &operator^=          (const MultiByteWord &rhs);
              MultiByteWord  operator^           (const MultiByteWord &rhs);
              MultiByteWord &operator|=          (const MultiByteWord &rhs);
              MultiByteWord  operator|           (const MultiByteWord &rhs);
              bool           operator==          (const MultiByteWord &rhs);

              MultiByteWord &i8Mod               (const MultiByteWord &polynomial);
              MultiByteWord &i1Mod               (const MultiByteWord &polynomial);

              string         toHexString         () const;
              UI32           getNumberOfBitsSet  () const;

        const UI32           getMsbBitMask       () const;

    // Now the data members

    private :
        UI32  m_polynomialDegree;
        UI32  m_numberOfWords;
        UI32 *m_pData;

    protected :
    public :
};

}

#endif // MULTIBYTEWORD_H
