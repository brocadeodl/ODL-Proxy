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
 *   Author : Raymond Lai		                                           *
 ***************************************************************************/

#include "Framework/Types/LargeObject.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include <sstream>

namespace WaveNs
{

void LargeObject::construct (const UI32 &numberOfBytes)
{
    m_numberOfBytes      = numberOfBytes;    
    m_pByteBlocks        = new UI8[m_numberOfBytes];
}

void LargeObject::destroy ()
{
    if (0 != m_pByteBlocks)
    {
        delete[] m_pByteBlocks;
    }

    m_numberOfBytes      = 0;
}

void LargeObject::resizewithoutcopy (const UI32 &numberOfBytes)
{
    destroy ();
    construct (numberOfBytes);
}

void LargeObject::copy (const LargeObject &largeObject)
{
    prismAssert (m_numberOfBytes == largeObject.m_numberOfBytes, __FILE__, __LINE__);

    memcpy(m_pByteBlocks, largeObject.m_pByteBlocks, m_numberOfBytes); 
}

LargeObject::LargeObject ()
{
	construct (1024);
}

LargeObject::LargeObject (const UI32 &numberOfBytes)
{
	construct (numberOfBytes);
}

LargeObject::LargeObject (const LargeObject &largeObject)
{
	construct (largeObject.m_numberOfBytes);
    copy (largeObject);
}

LargeObject::~LargeObject ()
{
	destroy ();
}

void LargeObject::resize (const UI32 &numberOfBytes)
{
    UI8		*pByteBlocks;

    pByteBlocks        = new UI8[numberOfBytes];
    memset (pByteBlocks, 0x0, numberOfBytes);

    if (numberOfBytes >= m_numberOfBytes)
    {
        memcpy(pByteBlocks, m_pByteBlocks, m_numberOfBytes);
    }
    else
    {
        memcpy(pByteBlocks, m_pByteBlocks, numberOfBytes);
    }

    delete[] m_pByteBlocks;
    m_pByteBlocks = pByteBlocks;
	m_numberOfBytes = numberOfBytes;
}

string LargeObject::toString () const
{
    string byteBlockInStringFormat;
    char   buffer[4] = {0};
    SI32   i = 0;

    for (i = m_numberOfBytes - 1; i  >= 0 ; i--)
    {
        snprintf(buffer, 4, "%02x", m_pByteBlocks[i]);
        byteBlockInStringFormat.append(buffer);
    }

    return (byteBlockInStringFormat);
}

void LargeObject::fromString (const string &largeObjectInStringFormat, const UI32 &noOfBytes)
{
    UI32          length            = largeObjectInStringFormat.length(); // # of nibbles
    UI32          numberOfBytes      = (length + 1) / 2; // 2 nibbles are one byte
    UI8	         *pByteBlocks        = new UI8[numberOfBytes];
    UI32          BlockSize          = 2; // 2 nibbles are one byte
    UI32          countByteBlocks    = 0;
    string        largeObjectString      = largeObjectInStringFormat;
    stringstream  converStringToHex[128];
    UI32          convertedHex = 0;
    
    memset(m_pByteBlocks, 0x0, m_numberOfBytes);

    while (largeObjectString.length() > BlockSize)
    {
        converStringToHex[countByteBlocks] << hex << largeObjectString.substr((largeObjectString.length() - BlockSize), BlockSize);
        converStringToHex[countByteBlocks] >> convertedHex;
        pByteBlocks[countByteBlocks] = convertedHex;
        largeObjectString.resize(largeObjectString.length() - BlockSize);
        countByteBlocks++;
    }

    if (largeObjectString.length() > 0)
    {
        converStringToHex[countByteBlocks] << hex << largeObjectString.c_str();
        converStringToHex[countByteBlocks] >> convertedHex;
        pByteBlocks[countByteBlocks] = convertedHex;
        countByteBlocks++;
    }

    if ((noOfBytes > 0) && (noOfBytes < numberOfBytes))
    {
        m_numberOfBytes = noOfBytes;
    }
    else
    {
        m_numberOfBytes = numberOfBytes;
    }

    delete[] m_pByteBlocks;

    m_pByteBlocks        = pByteBlocks;
}

UI32 LargeObject::getNumberOfBytes () const
{
    return (m_numberOfBytes);
}

UI8 LargeObject::operator [] (const UI32 &index)
{
    prismAssert (index < m_numberOfBytes, __FILE__, __LINE__);

    return (m_pByteBlocks[index]);
}

bool LargeObject::operator == (const LargeObject &largeObject) const
{
    UI32 i;

    if (m_numberOfBytes != largeObject.m_numberOfBytes)
    {
        return (false);
    }
	else 
	{
		for (i = 0; i < m_numberOfBytes; i++)
		{
			if (m_pByteBlocks[i] != largeObject.m_pByteBlocks[i])
			{
				return (false);
			}
		}
	}

    return (true);
}

bool LargeObject::operator != (const LargeObject &largeObject) const
{
    return (! (operator == (largeObject)));
}

LargeObject &LargeObject::operator = (const LargeObject &largeObject)
{
	resizewithoutcopy (largeObject.m_numberOfBytes);
    copy (largeObject);

    return (*this);
}

}
