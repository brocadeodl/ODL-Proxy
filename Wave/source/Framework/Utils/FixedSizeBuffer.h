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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FIXEDSIZEBUFFER_H
#define FIXEDSIZEBUFFER_H

#include "Framework/Types/Types.h"
#include <string>

using namespace std;

namespace WaveNs
{

class FixedSizeBuffer
{
    private :
    protected :
    public :
                    FixedSizeBuffer      (const UI32 maximumSize);
                    FixedSizeBuffer      (const UI32 maximumSize, void *pBuffer, bool isBufferUsed = true);
                   ~FixedSizeBuffer      ();
              UI8  *getPRawBuffer        ();
              UI8  *getPRawBuffer        () const;
              UI8  *getPCurrentRawBuffer ();
        const UI32  getMaximumSize       () const;
        const UI32  getCurrentSize       () const;
        const UI32  getRemainingSize     () const;
              void  incrementCurrentSize (const UI32 incrementSize);
              void  reset                ();
              bool  isFull               ();
        const void  toString             (string &resultString);
              UI8  *transferBufferToUser (UI32 &currentSize, UI32 &maximumSize);

    // Now the data members

    private :
        UI32  m_maximumSize;
        UI8  *m_pRawBuffer;
        UI32  m_currentSize;
        bool  m_isBufferOwnedByMe;

    protected :
    public :
};

}

#endif // FIXEDSIZEBUFFER_H
