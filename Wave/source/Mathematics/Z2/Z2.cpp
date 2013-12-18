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

#include "Mathematics/Z2/Z2.h"
#include "Framework/Utils/AssertUtils.h"
#include <stdlib.h>

namespace WaveNs
{

Z2::Z2 (const UI32 &polynomialDegree, const UI32 slidingWindowSize)
    : m_polynomialDegree  (polynomialDegree),
      m_slidingWindowSize (slidingWindowSize)
{
    prismAssert (0 == (slidingWindowSize % 8),         __FILE__, __LINE__);
    prismAssert ((slidingWindowSize << 3) >= polynomialDegree, __FILE__, __LINE__);

    setRandomIrreduciblePolynomial ();
}

Z2::~Z2 ()
{
}

MultiByteWord Z2::getCurrentPolynomial () const
{
    return (m_currentIrreduciblePolynomial);
}

void Z2::setRandomIrreduciblePolynomial ()
{
    UI32 i = 0;
    UI32 r = 1 + (UI32) (((m_polynomialDegree - 1) / 2) * (rand () / (RAND_MAX + 1.0)));
    UI32 x = 0;

    r = 2 * r + 1;

    m_currentIrreduciblePolynomial.setPolynomialDegree (m_polynomialDegree);

    m_currentIrreduciblePolynomial    = 0;
    m_currentIrreduciblePolynomial[0] = 0x1;

    for (i = 0; i < r; i ++)
    {
        x = 1 + (UI32) ((m_polynomialDegree - 1) * (rand () / (RAND_MAX + 1.0)));

        while (true == (m_currentIrreduciblePolynomial.isBitSet (x)))
        {
            x = 1 + (UI32) ((m_polynomialDegree - 1) * (rand () / (RAND_MAX + 1.0)));
        }

        m_currentIrreduciblePolynomial.setBit (x);
    }

    if (0 == (m_polynomialDegree % 32))
    {
        m_numberOfI1ModsForSlidingWindow = 0;
    }
    else
    {
        m_numberOfI1ModsForSlidingWindow = (((m_polynomialDegree >> 5) + 1) << 5) - m_polynomialDegree;
    }

    if (m_polynomialDegree <= 64)
    {
        if (32 < m_polynomialDegree)
        {
            m_currentIrreduciblePolynomialOptimizedFor64 = (((UI64) m_currentIrreduciblePolynomial[1]) << 32) | m_currentIrreduciblePolynomial[0];
        }
        else
        {
            m_currentIrreduciblePolynomialOptimizedFor64 = m_currentIrreduciblePolynomial[0];
        }

        m_msbMaskOptimizedFor64 = 0x1ULL << (m_polynomialDegree - 1);

        m_validBitMaskOptimizedFor64 = 0;

        for (i = 0; i < 64 - m_polynomialDegree; i++)
        {
            m_validBitMaskOptimizedFor64 >>= 1;
            m_validBitMaskOptimizedFor64  |= 0x8000000000000000ULL;
        }

        m_validBitMaskOptimizedFor64 = ~m_validBitMaskOptimizedFor64;
    }

    preComputeTables ();
}

void Z2::setCurrentIrreduciblePolynomial (const MultiByteWord &irreduciblePolynomial)
{
    UI32 i = 0;

    m_currentIrreduciblePolynomial = irreduciblePolynomial;
    m_polynomialDegree             = irreduciblePolynomial.getPolynomialDegree ();

    if (0 == (m_polynomialDegree % 32))
    {
        m_numberOfI1ModsForSlidingWindow = 0;
    }
    else
    {
        m_numberOfI1ModsForSlidingWindow = (((m_polynomialDegree >> 5) + 1) << 5) - m_polynomialDegree;
    }

    if (m_polynomialDegree <= 64)
    {
        if (32 < m_polynomialDegree)
        {
            m_currentIrreduciblePolynomialOptimizedFor64 = (((UI64) m_currentIrreduciblePolynomial[1]) << 32) | m_currentIrreduciblePolynomial[0];
        }
        else
        {
            m_currentIrreduciblePolynomialOptimizedFor64 = m_currentIrreduciblePolynomial[0];
        }

        m_msbMaskOptimizedFor64 = 0x1ULL << (m_polynomialDegree - 1);

        m_validBitMaskOptimizedFor64 = 0;

        for (i = 0; i < 64 - m_polynomialDegree; i++)
        {
            m_validBitMaskOptimizedFor64 >>= 1;
            m_validBitMaskOptimizedFor64  |= 0x8000000000000000ULL;
        }

        m_validBitMaskOptimizedFor64 = ~m_validBitMaskOptimizedFor64;
    }

    preComputeTables ();
}

void Z2::preComputeTables ()
{
    UI32 i                               = 0;
    UI32 j                               = 0;
    UI32 numberOfI8ModsForSlidingWindow  = ((m_slidingWindowSize << 3) - m_polynomialDegree) >> 3;
    UI32 numberOfI8ModsForSlidingWindow1 = 0;

    for (i = 0; i < 256; i++)
    {
        m_ta[i].setPolynomialDegree (m_polynomialDegree);
        m_tb[i].setPolynomialDegree (m_polynomialDegree);
        m_tc[i].setPolynomialDegree (m_polynomialDegree);
        m_td[i].setPolynomialDegree (m_polynomialDegree);
        m_ts[i].setPolynomialDegree (m_polynomialDegree);
    }

    for (i = 0; i < 256; i++)
    {
        m_td[i] = i;
        m_td[i] <<= (m_polynomialDegree - 8);
        //cout << m_td[i].toHexString () << ", ";
        m_td[i].i8Mod (m_currentIrreduciblePolynomial);
        //cout << m_td[i].toHexString () << ", ";

        m_tc[i] = m_td[i];
        m_tc[i].i8Mod (m_currentIrreduciblePolynomial);
        //cout << m_tc[i].toHexString () << ", ";

        m_tb[i] = m_tc[i];
        m_tb[i].i8Mod (m_currentIrreduciblePolynomial);
        //cout << m_tb[i].toHexString () << ", ";

        m_ta[i] = m_tb[i];
        m_ta[i].i8Mod (m_currentIrreduciblePolynomial);
        //cout << m_ta[i].toHexString () << ", ";

        //cout << "Number Of I8 Mods : " << numberOfI8ModsForSlidingWindow << endl;

        if (numberOfI8ModsForSlidingWindow <= 4)
        {
            switch (numberOfI8ModsForSlidingWindow)
            {
                case 0:
                    break;

                case 1:
                    m_ts[i] = m_td[i];
                    break;

                case 2:
                    m_ts[i] = m_tc[i];
                    break;

                case 3:
                    m_ts[i] = m_tb[i];
                    break;

                case 4:
                    m_ts[i] = m_ta[i];
                    break;

                default:
                    prismAssert (false, __FILE__, __LINE__);
            }
        }
        else
        {
            m_ts[i] = m_ta[i];

            numberOfI8ModsForSlidingWindow1 = numberOfI8ModsForSlidingWindow - 4;

            //cout << "Number Of effective I8 Mods : " << numberOfI8ModsForSlidingWindow1 << endl;

            for (j = 0; j < numberOfI8ModsForSlidingWindow1; j++)
            {
                m_ts[i].i8Mod (m_currentIrreduciblePolynomial);
            }
        }

        for (j = 0; j < m_numberOfI1ModsForSlidingWindow; j++)
        {
            m_ts[i].i1Mod (m_currentIrreduciblePolynomial);
        }

        //cout << m_ts[i].toHexString () << endl;
    }

    // Setup the optimzed tables for 64 bits if required.

    if (64 >= m_polynomialDegree)
    {
        if (32 < m_polynomialDegree)
        {
            for (i = 0; i < 256; i++)
            {
                m_ta64[i] = (((UI64) m_ta[i][1]) << 32) | m_ta[i][0];
                m_tb64[i] = (((UI64) m_tb[i][1]) << 32) | m_tb[i][0];
                m_tc64[i] = (((UI64) m_tc[i][1]) << 32) | m_tc[i][0];
                m_td64[i] = (((UI64) m_td[i][1]) << 32) | m_td[i][0];
                m_ts64[i] = (((UI64) m_ts[i][1]) << 32) | m_ts[i][0];
            }
        }
        else
        {
            for (i = 0; i < 256; i++)
            {
                m_ta64[i] = m_ta[i][0];
                m_tb64[i] = m_tb[i][0];
                m_tc64[i] = m_tc[i][0];
                m_td64[i] = m_td[i][0];
                m_ts64[i] = m_ts[i][0];
            }
        }
    }
}

UI32 Z2::getPolynomialDegree () const
{
    return (m_polynomialDegree);
}

UI32 Z2::getSlidingWindowSize () const
{
    return (m_slidingWindowSize);
}

void Z2::computeFingerPrint (MultiByteWord &fingerPrint, const UI32 &newData)
{
    UI32          numberOfWords   = fingerPrint.getNumberOfWords ();
    UI32          i               = 0;
    UI32          finalIndex      = numberOfWords - 1;
    MultiByteWord tempFingerPrint = fingerPrint << m_numberOfI1ModsForSlidingWindow;
    UI32          f1              = (tempFingerPrint[finalIndex] >> 24) & 0xFF;
    UI32          f2              = (tempFingerPrint[finalIndex] >> 16) & 0xFF;
    UI32          f3              = (tempFingerPrint[finalIndex] >> 8)  & 0xFF;
    UI32          f4              = (tempFingerPrint[finalIndex])       & 0xFF;

    for (i = finalIndex; i > 0; i--)
    {
        fingerPrint[i] = fingerPrint[i - 1] ^ m_ta[f1][i] ^ m_tb[f2][i] ^ m_tc[f3][i] ^ m_td[f4][i];
    }

    fingerPrint[0] = newData ^ m_ta[f1][0] ^ m_tb[f2][0] ^ m_tc[f3][0] ^ m_td[f4][0];
}

void Z2::computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const UI32 &newData)
{
    UI32 tempFingerPrint = (fingerPrint << m_numberOfI1ModsForSlidingWindow) >> 32;
    UI32 f4              = (tempFingerPrint)       & 0xFF;
    UI32 f3              = (tempFingerPrint >> 8)  & 0xFF;
    UI32 f2              = (tempFingerPrint >> 16) & 0xFF;
    UI32 f1              = (tempFingerPrint >> 24) & 0xFF;

    fingerPrint = ((fingerPrint << 32) ^ m_ta64[f1] ^ m_tb64[f2] ^ m_tc64[f3] ^ m_td64[f4] ^ ((UI64) newData)) & m_validBitMaskOptimizedFor64;
}

void Z2::computeFingerPrint (MultiByteWord &fingerPrint, const unsigned char &newData)
{
    UI32 i          = 0;
    bool needXor    = false;
    UI32 newData1   = newData & 0xFF;
    UI32 currentBit = 0;

    for (i = 0; i < 8; i++)
    {
        currentBit   = (newData1 & 0x80) ? 1 : 0;
        newData1   <<= 1;
        needXor      = fingerPrint.isBitSet (m_polynomialDegree - 1);

        fingerPrint    <<= 1;
        fingerPrint[0]  |= currentBit;

        if (true == needXor)
        {
            fingerPrint ^= m_currentIrreduciblePolynomial;
        }
    }

    //cout << fingerPrint.toHexString () << endl;
}

void Z2::computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const unsigned char &newData)
{
    UI32 i          = 0;
    bool needXor    = false;
    UI32 newData1   = newData & 0xFF;
    UI64 currentBit = 0;

    for (i = 0; i < 8; i++)
    {
        currentBit   = (newData1 & 0x80) ? 1 : 0;
        newData1   <<= 1;
        needXor      = (fingerPrint & m_msbMaskOptimizedFor64) ? true : false;

        fingerPrint <<= 1;
        fingerPrint  |= currentBit;

        if (true == needXor)
        {
            fingerPrint ^= m_currentIrreduciblePolynomialOptimizedFor64;
        }
    }

    fingerPrint &= m_validBitMaskOptimizedFor64;

    //cout << fingerPrint.toHexString () << endl;
}

void Z2::computeFingerPrint (MultiByteWord &fingerPrint, const string &message)
{
    UI32 messageLength          = message.length ();
    UI32 numberOfWordsInMessage = messageLength / 4;
    UI32 i                      = 0;
    UI32 j                      = 0;
    UI32 newData                = 0;
    UI32 temp1                  = 0;
    UI32 temp2                  = 0;
    UI32 temp3                  = 0;
    UI32 temp4                  = 0;

    fingerPrint.setPolynomialDegree (m_polynomialDegree);
    fingerPrint = 0;

    for (i = 0, j = 0; i < numberOfWordsInMessage; i++, j += 4)
    {
        temp1    = message[j];
        temp1   &= 0xFF;
        temp2    = message[j + 1];
        temp2   &= 0xFF;
        temp3    = message[j + 2];
        temp3   &= 0xFF;
        temp4    = message[j + 3];
        temp4   &= 0xFF;
        newData  = 0;
        newData |= (temp1 << 24);
        newData |= (temp2 << 16);
        newData |= (temp3 << 8);
        newData |= temp4;

        computeFingerPrint (fingerPrint, newData);
    }

    for (; j < messageLength; j++)
    {
        computeFingerPrint (fingerPrint, (const unsigned char) message[j]);
    }
}

void Z2::computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const string &message)
{
    UI32 messageLength          = message.length ();
    UI32 numberOfWordsInMessage = messageLength / 4;
    UI32 i                      = 0;
    UI32 j                      = 0;
    UI32 newData                = 0;
    UI32 temp1                  = 0;
    UI32 temp2                  = 0;
    UI32 temp3                  = 0;
    UI32 temp4                  = 0;

    fingerPrint = 0;

    for (i = 0, j = 0; i < numberOfWordsInMessage; i++, j += 4)
    {
        temp1    = message[j];
        temp1   &= 0xFF;
        temp2    = message[j + 1];
        temp2   &= 0xFF;
        temp3    = message[j + 2];
        temp3   &= 0xFF;
        temp4    = message[j + 3];
        temp4   &= 0xFF;
        newData  = 0;
        newData |= (temp1 << 24);
        newData |= (temp2 << 16);
        newData |= (temp3 << 8);
        newData |= temp4;

        computeFingerPrintOptimizedFor64 (fingerPrint, newData);
    }

    for (; j < messageLength; j++)
    {
        computeFingerPrintOptimizedFor64 (fingerPrint, (const unsigned char) message[j]);
    }
}

void Z2::computeFingerPrint (MultiByteWord &fingerPrint, const char *pBuffer, const UI32 &bufferSize)
{
    UI32 messageLength          = bufferSize;
    UI32 numberOfWordsInMessage = messageLength / 4;
    UI32 i                      = 0;
    UI32 j                      = 0;
    UI32 newData                = 0;
    UI32 temp1                  = 0;
    UI32 temp2                  = 0;
    UI32 temp3                  = 0;
    UI32 temp4                  = 0;

    fingerPrint.setPolynomialDegree (m_polynomialDegree);
    fingerPrint = 0;

    for (i = 0, j = 0; i < numberOfWordsInMessage; i++, j += 4)
    {
        temp1    = pBuffer[j];
        temp1   &= 0xFF;
        temp2    = pBuffer[j + 1];
        temp2   &= 0xFF;
        temp3    = pBuffer[j + 2];
        temp3   &= 0xFF;
        temp4    = pBuffer[j + 3];
        temp4   &= 0xFF;
        newData  = 0;
        newData |= (temp1 << 24);
        newData |= (temp2 << 16);
        newData |= (temp3 << 8);
        newData |= temp4;

        computeFingerPrint (fingerPrint, newData);
    }

    for (; j < messageLength; j++)
    {
        computeFingerPrint (fingerPrint, (const unsigned char) pBuffer[j]);
    }
}

void Z2::computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const char *pBuffer, const UI32 &bufferSize)
{
    UI32 messageLength          = bufferSize;
    UI32 numberOfWordsInMessage = messageLength / 4;
    UI32 i                      = 0;
    UI32 j                      = 0;
    UI32 newData                = 0;
    UI32 temp1                  = 0;
    UI32 temp2                  = 0;
    UI32 temp3                  = 0;
    UI32 temp4                  = 0;

    fingerPrint = 0;

    for (i = 0, j = 0; i < numberOfWordsInMessage; i++, j += 4)
    {
        temp1    = pBuffer[j];
        temp1   &= 0xFF;
        temp2    = pBuffer[j + 1];
        temp2   &= 0xFF;
        temp3    = pBuffer[j + 2];
        temp3   &= 0xFF;
        temp4    = pBuffer[j + 3];
        temp4   &= 0xFF;
        newData  = 0;
        newData |= (temp1 << 24);
        newData |= (temp2 << 16);
        newData |= (temp3 << 8);
        newData |= temp4;

        computeFingerPrintOptimizedFor64 (fingerPrint, newData);
    }

    for (; j < messageLength; j++)
    {
        computeFingerPrintOptimizedFor64 (fingerPrint, (const unsigned char) pBuffer[j]);
    }
}

void Z2::computeFingerPrint (MultiByteWord &fingerPrint, const char &oldestData, const unsigned char &newData)
{
    fingerPrint ^= m_ts[(UI32) oldestData];

    computeFingerPrint (fingerPrint, newData);
}

void Z2::computeFingerPrintOptimizedFor64 (UI64 &fingerPrint, const char &oldestData, const unsigned char &newData)
{
    fingerPrint ^= m_ts64[(UI32) oldestData];

    computeFingerPrintOptimizedFor64 (fingerPrint, newData);
}

}
