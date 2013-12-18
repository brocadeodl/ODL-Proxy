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

#include "Mathematics/Z2/MultiByteWord.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

MultiByteWord::MultiByteWord (const UI32 &polynomialDegree)
{
    construct (polynomialDegree);
}

MultiByteWord::MultiByteWord (const MultiByteWord &rhs)
{
    construct (rhs.getPolynomialDegree ());
    memcpy (m_pData, rhs.m_pData, m_numberOfWords * sizeof (UI32));
}

MultiByteWord::~MultiByteWord ()
{
    destroy ();
}

void MultiByteWord::construct (const UI32 &polynomialDegree)
{
    m_polynomialDegree = polynomialDegree;
    m_numberOfWords    = (m_polynomialDegree + 31) >> 5;
    m_pData            = new UI32[m_numberOfWords];
}

void MultiByteWord::destroy ()
{
    if (NULL != m_pData)
    {
        delete[] m_pData;
        m_pData = NULL;
    }

    m_polynomialDegree = 0;
    m_numberOfWords    = 0;
}

UI32 MultiByteWord::getNumberOfWords () const
{
    return (m_numberOfWords);
}

UI32 MultiByteWord::getPolynomialDegree () const
{
    return (m_polynomialDegree);
}

void MultiByteWord::setPolynomialDegree (const UI32 &polynomialDegree)
{
    destroy ();

    construct (polynomialDegree);
}

void MultiByteWord::setBit (const UI32 &index)
{
    prismAssert (index < m_polynomialDegree, __FILE__, __LINE__);

    UI32 temp      = index + 1;
    UI32 temp1     = temp >> 5;
    UI32 temp2     = temp - (temp1 << 5);
    UI32 wordIndex = (temp2 > 0) ? temp1 : (temp1 - 1);
    UI32 bitMask   = (temp2 > 0) ? 0x1 << (temp2 - 1) : 0x80000000;

    m_pData[wordIndex] |= bitMask;
}

void MultiByteWord::unsetBit (const UI32 &index)
{
    prismAssert (index < m_polynomialDegree, __FILE__, __LINE__);

    UI32 temp      = index + 1;
    UI32 temp1     = temp >> 5;
    UI32 temp2     = temp - (temp1 << 5);
    UI32 wordIndex = (temp2 > 0) ? temp1 : (temp1 - 1);
    UI32 bitMask   = ~((temp2 > 0) ? 0x1 << (temp2 - 1) : 0x80000000);

    m_pData[wordIndex] &= bitMask;
}

bool MultiByteWord::isBitSet (const UI32 &index)
{
    prismAssert (index < m_polynomialDegree, __FILE__, __LINE__);

    UI32 temp      = index + 1;
    UI32 temp1     = temp >> 5;
    UI32 temp2     = temp - (temp1 << 5);
    UI32 wordIndex = (temp2 > 0) ? temp1 : (temp1 - 1);
    UI32 bitMask   = (temp2 > 0) ? 0x1 << (temp2 - 1) : 0x80000000;
    bool isSet     = ((m_pData[wordIndex] & bitMask) > 0) ? true : false;

    return (isSet);
}

MultiByteWord &MultiByteWord::i8Mod (const MultiByteWord &polynomial)
{
    prismAssert (m_numberOfWords == polynomial.m_numberOfWords, __FILE__, __LINE__);

    UI32 i    = 0;
    bool temp = false;

    for (i = 0; i < 8; i++)
    {
        temp = isBitSet (m_polynomialDegree - 1);

        (*this) <<= 1;

        if (true == temp)
        {
            (*this) ^= polynomial;
        }
    }

    return (*this);
}

MultiByteWord &MultiByteWord::i1Mod (const MultiByteWord &polynomial)
{
    prismAssert (m_numberOfWords == polynomial.m_numberOfWords, __FILE__, __LINE__);

    bool temp = isBitSet (m_polynomialDegree - 1);

    (*this) <<= 1;

    if (true == temp)
    {
        (*this) ^= polynomial;
    }

    return (*this);
}

MultiByteWord &MultiByteWord::operator= (const MultiByteWord& rhs)
{
    destroy ();

    construct (rhs.getPolynomialDegree ());
    memcpy (m_pData, rhs.m_pData, m_numberOfWords * sizeof (UI32));

    return (*this);
}

MultiByteWord &MultiByteWord::operator= (const UI32 &rhs)
{
    UI32 i = 0;

    for (i = m_numberOfWords - 1; i > 0; i--)
    {
        m_pData[i] = 0;
    }

    m_pData[0] = rhs;

    return (*this);
}

UI32 &MultiByteWord::operator[] (const UI32 &index)
{
    WaveNs::prismAssert (index <= (m_numberOfWords - 1), __FILE__, __LINE__);

    return (m_pData[index]);
}

MultiByteWord &MultiByteWord::operator<<= (const UI32 &numberOfBitsToShift)
{
    prismAssert (numberOfBitsToShift <= m_polynomialDegree, __FILE__, __LINE__);

    if (0 == numberOfBitsToShift)
    {
        return (*this);
    }

    UI32 numberOfTotalBits     = getNumberOfWords () * 32;
    UI32 numberOfResultingBits = numberOfTotalBits - numberOfBitsToShift;
    UI32 numberOfBitsToShift1  = numberOfBitsToShift % 32;
    UI32 numberWordsToIgnore   = numberOfBitsToShift / 32;
    UI32 numberOfWordsRequired = (numberOfResultingBits + 31) / 32;

    UI32 i = 0;

    for (i = (m_numberOfWords - 1); i > (m_numberOfWords - numberOfWordsRequired); i--)
    {
        //cout << "1 ... " << (m_pData[i - numberWordsToIgnore] << numberOfBitsToShift1) << endl;
        //cout << "2 ... " << (m_pData[i - numberWordsToIgnore - 1] >> (32 - numberOfBitsToShift1)) << endl;

        m_pData[i] = (m_pData[i - numberWordsToIgnore] << numberOfBitsToShift1) | (m_pData[i - numberWordsToIgnore - 1] >> (32 - numberOfBitsToShift1));
    }

    m_pData[i] = m_pData[0] << numberOfBitsToShift1;

    if (i > 0)
    {
        for (--i; i > 0; i--)
        {
            m_pData[i] = 0;
        }

        m_pData[i] = 0;
    }

    return (*this);
}

MultiByteWord MultiByteWord::operator<< (const UI32 &numberOfBitsToShift)
{
    MultiByteWord tempMultiByteWord = *this;

    tempMultiByteWord <<= numberOfBitsToShift;

    return (tempMultiByteWord);
}

MultiByteWord &MultiByteWord::operator^= (const MultiByteWord &rhs)
{
    prismAssert (m_numberOfWords == rhs.m_numberOfWords, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_numberOfWords; i++)
    {
        m_pData[i] ^= rhs.m_pData[i];
    }

    return (*this);
}

MultiByteWord MultiByteWord::operator^ (const MultiByteWord &rhs)
{
    MultiByteWord tempMultiByteWord = *this;

    tempMultiByteWord ^= rhs;

    return (tempMultiByteWord);
}

MultiByteWord &MultiByteWord::operator|= (const MultiByteWord &rhs)
{
    prismAssert (m_numberOfWords == rhs.m_numberOfWords, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_numberOfWords; i++)
    {
        m_pData[i] |= rhs.m_pData[i];
    }

    return (*this);
}

MultiByteWord MultiByteWord::operator| (const MultiByteWord &rhs)
{
    MultiByteWord tempMultiByteWord = *this;

    tempMultiByteWord |= rhs;

    return (tempMultiByteWord);
}

bool MultiByteWord::operator== (const MultiByteWord &rhs)
{
    UI32 i          = 0;
    UI32 msbBitMask = getMsbBitMask ();

    if (m_polynomialDegree != rhs.m_polynomialDegree)
    {
        return (false);
    }

    for (i = 0; i < m_numberOfWords - 1; i++)
    {
        if (m_pData[i] != rhs.m_pData[i])
        {
            return (false);
        }
    }

    if (((m_pData[i]) & msbBitMask) != ((rhs.m_pData[i]) & msbBitMask))
    {
        return (false);
    }

    return (true);
}

const UI32 MultiByteWord::getMsbBitMask () const
{
    UI32 i                         = 0;
    UI32 numberOfMsbBitsToBeMasked = (m_numberOfWords << 5) - m_polynomialDegree;
    UI32 msbBitMask                = 0;

    prismAssert (numberOfMsbBitsToBeMasked < 32, __FILE__, __LINE__);

    for (i = 0; i < numberOfMsbBitsToBeMasked; i++)
    {
        msbBitMask |= (0x1 << (31 - i));
    }

    msbBitMask = ~msbBitMask;

    return (msbBitMask);
}

string MultiByteWord::toHexString () const
{
    UI32    numberOfWords = getNumberOfWords ();
    UI32    stringSize    = (numberOfWords << 3) + 1;
    char    *pTemp        = new char[stringSize];
    string  tempString;
    UI32    i             = 0;
    UI32    msbBitMask    = getMsbBitMask ();

    WaveNs::prismAssert (NULL != pTemp, __FILE__, __LINE__);

    snprintf (pTemp, stringSize, "%08X", m_pData[numberOfWords - 1] & msbBitMask);

    for (i = 1; i < numberOfWords; i++)
    {
        snprintf (pTemp + 8 * i, stringSize , "%08X", m_pData[numberOfWords - 1 - i]);
    }

    tempString = string (pTemp);

    delete[] pTemp;

    return (tempString);
}

UI32 MultiByteWord::getNumberOfBitsSet () const
{
    UI32 count = 0;
    UI32 i     = 0;
    UI32 j     = 0;

    for (i = 0; i < m_numberOfWords; i++)
    {
        for (j = 0; j < 32; j++)
        {
            if (0 != (m_pData[i] & (1 << j)))
            {
                count++;
            }
        }
    }

    return (count);
}

}
