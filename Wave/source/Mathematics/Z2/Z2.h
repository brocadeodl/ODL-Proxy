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

#ifndef Z2_H
#define Z2_H

#include "Mathematics/Z2/MultiByteWord.h"

namespace WaveNs
{

class Z2
{
    private :
    protected :
    public :
                       Z2                               (const UI32 &polynomialDegree = 64, const UI32 slidingWindowSize = 48);
                      ~Z2                               ();

        MultiByteWord  getCurrentPolynomial             () const;
        void           setRandomIrreduciblePolynomial   ();
        void           setCurrentIrreduciblePolynomial  (const MultiByteWord &polynomial);
        void           preComputeTables                 ();

        UI32           getPolynomialDegree              () const;
        UI32           getSlidingWindowSize             () const;

        void           computeFingerPrint               (MultiByteWord &fingerPrint, const UI32 &newData);
        void           computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const UI32 &newData);
        void           computeFingerPrint               (MultiByteWord &fingerPrint, const unsigned char &newData);
        void           computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const unsigned char &newData);
        void           computeFingerPrint               (MultiByteWord &fingerPrint, const string &message);
        void           computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const string &message);
        void           computeFingerPrint               (MultiByteWord &fingerPrint, const char *pBuffer, const UI32 &bufferSize);
        void           computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const char *pBuffer, const UI32 &bufferSize);
        void           computeFingerPrint               (MultiByteWord &fingerPrint, const char &oldestData, const unsigned char &newData);
        void           computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const char &oldestData, const unsigned char &newData);

    // Now the Data members

    private :
        UI32          m_polynomialDegree;
        UI32          m_slidingWindowSize;
        MultiByteWord m_currentIrreduciblePolynomial;
        UI32          m_numberOfI1ModsForSlidingWindow;

        UI64          m_currentIrreduciblePolynomialOptimizedFor64;
        UI64          m_msbMaskOptimizedFor64;
        UI64          m_validBitMaskOptimizedFor64;

        MultiByteWord m_ta[256];
        MultiByteWord m_tb[256];
        MultiByteWord m_tc[256];
        MultiByteWord m_td[256];
        MultiByteWord m_ts[256];

        UI64          m_ta64[256];
        UI64          m_tb64[256];
        UI64          m_tc64[256];
        UI64          m_td64[256];
        UI64          m_ts64[256];

    protected :
    public :
};

}

#endif // Z2_H
