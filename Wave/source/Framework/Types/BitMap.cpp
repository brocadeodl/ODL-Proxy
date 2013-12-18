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

 /**************************************************************************
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Types/BitMap.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include <sstream>

namespace WaveNs
{

void BitMap::setAllValues (const UI64 &val)
{
    construct(64);
    m_pBitBlocks[0]     = val;
}

void BitMap::construct (const UI32 &numberOfBits)
{
    m_numberOfBits      = numberOfBits;    
    m_numberOfBitBlocks = ((m_numberOfBits + 63)/ 64) ;
    m_pBitBlocks        = new UI64[m_numberOfBitBlocks];
}

void BitMap::destroy ()
{
    if (NULL != m_pBitBlocks)
    {
        delete[] m_pBitBlocks;
    }

    m_numberOfBits      = 0;
    m_numberOfBitBlocks = 0;
}

void BitMap::resizewithoutcopy (const UI32 &numberOfBits)
{
    destroy ();
    construct (numberOfBits);
}

void BitMap::copy (const BitMap &bitMap)
{
    prismAssert (m_numberOfBits == bitMap.m_numberOfBits, __FILE__, __LINE__);

    memcpy(m_pBitBlocks, bitMap.m_pBitBlocks, (m_numberOfBitBlocks * 8)); 
}

BitMap::BitMap ()
{
    construct (256);
}

BitMap::BitMap (const UI32 &numberOfBits)
{
    construct (numberOfBits);
}

BitMap::BitMap (const BitMap &bitMap)
{
    construct (bitMap.m_numberOfBits);
    copy (bitMap);    
}

BitMap::~BitMap ()
{
    destroy ();
}

void BitMap::resize (const UI32 &numberOfBits)
{
    UI64 *pBitBlocks;
    UI32 numberOfBitBlocks;

    numberOfBitBlocks = ((numberOfBits + 63)/ 64) ;
    pBitBlocks        = new UI64[numberOfBitBlocks];
    memset (pBitBlocks, 0x0, numberOfBitBlocks * 8);

    if (numberOfBitBlocks >= m_numberOfBitBlocks)
    {
        memcpy(pBitBlocks, m_pBitBlocks, (m_numberOfBitBlocks * 8));
    }
    else
    {
        memcpy(pBitBlocks, m_pBitBlocks, (numberOfBitBlocks * 8));
    }

    delete[] m_pBitBlocks;
    m_pBitBlocks = pBitBlocks;
    m_numberOfBitBlocks = numberOfBitBlocks;
    m_numberOfBits = numberOfBits;
}

string BitMap::toString () const
{
    string bitBlockInStringFormat;
    char   buffer[128] = {0};
    SI32   i = 0;
    string tempString;
    UI32   extraStringLength = 0;
    UI32   totalStringLength = 0;

    for (i = m_numberOfBitBlocks - 1; i  >= 0 ; i--)
    {
        snprintf(buffer, 128, "%016llx", m_pBitBlocks[i]);
        bitBlockInStringFormat.append(buffer);
    }

    totalStringLength = (m_numberOfBits + 3)/4;
    extraStringLength = ((m_numberOfBitBlocks * 64) - m_numberOfBits)/4;
    tempString = bitBlockInStringFormat.substr (extraStringLength, totalStringLength);

    return (tempString);
}

void BitMap::getPlainString (string &valueString) const 
{
    valueString = toString ();
}

void BitMap::fromString (const string &bitMapInStringFormat, const UI32 &noOfBits)
{
    UI32          length            = bitMapInStringFormat.length();
    UI32          numberOfBits      = length * 4;
    UI32          numberOfBitBlocks = (numberOfBits + 63)/ 64;
    UI64         *pBitBlocks        = new UI64[numberOfBitBlocks];
    UI32          BlockSize         = 16;
    UI32          countBitBlocks    = 0;
    string        bitMapString      = bitMapInStringFormat;
    stringstream  converStringToHex[128];
    UI64          convertedHex = 0;
    
    memset(m_pBitBlocks, 0x0, (m_numberOfBitBlocks * 8));

    while (bitMapString.length() > BlockSize)
    {
        converStringToHex[countBitBlocks] << hex << bitMapString.substr((bitMapString.length() - BlockSize), BlockSize);
        converStringToHex[countBitBlocks] >> convertedHex;
        pBitBlocks[countBitBlocks] = convertedHex;
        bitMapString.resize(bitMapString.length() - BlockSize);
        countBitBlocks++;
    }

    if (bitMapString.length() > 0)
    {
        converStringToHex[countBitBlocks] << hex << bitMapString.c_str();
        converStringToHex[countBitBlocks] >> convertedHex;
        pBitBlocks[countBitBlocks] = convertedHex;
        countBitBlocks++;
    }

    prismAssert (countBitBlocks == numberOfBitBlocks, __FILE__, __LINE__);

    if (noOfBits != 0)
    {
        UI32 differenceNumberOfBts = numberOfBits - noOfBits;
        prismAssert ((0 <= differenceNumberOfBts) && (differenceNumberOfBts < 4), __FILE__, __LINE__);
        m_numberOfBits = noOfBits;
    }
    else
    {
        m_numberOfBits = numberOfBits;
    }

    delete[] m_pBitBlocks;

    m_pBitBlocks        = pBitBlocks;
    m_numberOfBitBlocks = numberOfBitBlocks;
}

ResourceId BitMap::loadFromPlainString (const string &bitMapInStringFormat, const UI32 &noOfBits)
{
    UI32          len               = bitMapInStringFormat.length();
    UI32          numberOfBits      = len * 4;
    UI32          numberOfBitBlocks = (numberOfBits + 63)/ 64;
    UI64         *pBitBlocks        = new UI64[numberOfBitBlocks];
    UI32          BlockSize         = 16;
    UI32          countBitBlocks    = 0;
    string        bitMapString      = bitMapInStringFormat;
    UI64          convertedHex      = 0;
    ResourceId    status            = WAVE_MESSAGE_ERROR; 
    stringstream  converStringToHex[128];
    
    memset(m_pBitBlocks, 0x0, (m_numberOfBitBlocks * 8));

    while (bitMapString.length() > BlockSize)
    {
        converStringToHex[countBitBlocks] << hex << bitMapString.substr((bitMapString.length() - BlockSize), BlockSize);
        converStringToHex[countBitBlocks] >> convertedHex;
        pBitBlocks[countBitBlocks] = convertedHex;
        bitMapString.resize(bitMapString.length() - BlockSize);
        countBitBlocks++;
    }

    if (bitMapString.length() > 0)
    {
        converStringToHex[countBitBlocks] << hex << bitMapString.c_str();
        converStringToHex[countBitBlocks] >> convertedHex;
        pBitBlocks[countBitBlocks] = convertedHex;
        countBitBlocks++;
    }

    if(countBitBlocks == numberOfBitBlocks)
    {
        if (noOfBits != 0)
        {
            UI32 differenceNumberOfBts = numberOfBits - noOfBits;
            if ((0 <= differenceNumberOfBts) && (differenceNumberOfBts < 4))
            {
                m_numberOfBits = noOfBits;
                delete[] m_pBitBlocks;
        
                m_pBitBlocks        = pBitBlocks;
                m_numberOfBitBlocks = numberOfBitBlocks;

                status = WAVE_MESSAGE_SUCCESS;
            }

        }
        else
        {
            m_numberOfBits = numberOfBits;
            delete[] m_pBitBlocks;
        
            m_pBitBlocks        = pBitBlocks;
            m_numberOfBitBlocks = numberOfBitBlocks;

            status = WAVE_MESSAGE_SUCCESS;
        }
        
    }

    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("Bitmap::loadFromPlainString : Improper format of decimal64 in string"));
    }

    return status;  
}

UI8 BitMap::getValue (const UI32 &index)
{
    prismAssert (index < m_numberOfBits, __FILE__, __LINE__);
    
    UI8  userBitInBlock = 0;
    UI32 bitBlockNumber = index/64;         /* Block Number Starts From 0 */
    UI64 bitMask = 0;

    userBitInBlock = index % 64;
    bitMask = (UI64) 1 << userBitInBlock;
 
    if (!(bitMask & m_pBitBlocks[bitBlockNumber]))
    {
        return ((UI8) 0);
    }
    else
    {
        return ((UI8) 1);
    }
}

UI64 BitMap::getElemValue (void) const
{
    if (m_numberOfBitBlocks != 1)
    {
        trace (TRACE_LEVEL_ERROR, string("BitMap::getElemValue: m_numberOfBitBlocks is:") + m_numberOfBitBlocks + string(" but expected value is 1"));
        return(0);
    }
    
    return (m_pBitBlocks[0]);
}

void BitMap::setValue (const UI32 &index, const UI8 &Value)
{
    prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

    UI8  userBitInBlock = 0;
    UI32 bitBlockNumber = index/64;
    UI64 bitMask = 0, bitMaskCompliment = 0;

    userBitInBlock = index % 64;
    if (1 == Value)
    {
        bitMask = (UI64) Value << userBitInBlock;
        m_pBitBlocks[bitBlockNumber] = m_pBitBlocks[bitBlockNumber] | bitMask;
    }
    else if (0 == Value)
    {
        bitMaskCompliment = (UI64) 1 << userBitInBlock;
        bitMask = ~bitMaskCompliment;
        m_pBitBlocks[bitBlockNumber] = m_pBitBlocks[bitBlockNumber] & bitMask;
    }
}

void BitMap::set (const UI32 &index)
{
    prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

    setValue (index, 1);
}

void BitMap::set ()
{
    UI64 extraBit = 0;

    memset(m_pBitBlocks, 0xff, (m_numberOfBitBlocks * 8));

    extraBit = (m_numberOfBitBlocks * 64) - m_numberOfBits;

    m_pBitBlocks[m_numberOfBitBlocks - 1] = m_pBitBlocks[m_numberOfBitBlocks - 1] >> extraBit;
}

void BitMap::reset (const UI32 &index)
{
    prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

    setValue (index, 0);
}

void BitMap::reset ()
{
    memset(m_pBitBlocks, 0x0, (m_numberOfBitBlocks * 8));
}

UI32 BitMap::getNumberOfBits () const
{
    return (m_numberOfBits);
}

UI8 BitMap::operator [] (const UI32 &index)
{
    prismAssert (index < m_numberOfBits, __FILE__, __LINE__);

    return (getValue (index));
}

bool BitMap::operator == (const BitMap &bitMap)
{
    UI32 i = 0;

    if (m_numberOfBits != bitMap.m_numberOfBits)
    {
        return (false);
    }
    else if (m_numberOfBitBlocks != bitMap.m_numberOfBitBlocks)
    {
        return (false);
    }
    else
    {
        for (i = 0; i < m_numberOfBitBlocks; i++)
        {
            if (m_pBitBlocks[i] != bitMap.m_pBitBlocks[i])
            {
                return (false);
            }
        }
    }

    return (true);
}

bool BitMap::operator != (const BitMap &bitMap)
{
    return (! (operator == (bitMap)));
}

BitMap &BitMap::operator = (const BitMap &bitMap)
{
    resizewithoutcopy (bitMap.m_numberOfBits);
    copy (bitMap);

    return (*this);
}

}

