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

#include "Framework/Types/WorldWideName.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

WorldWideName::WorldWideName ()
    : m_separator (':')
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);
}

WorldWideName::WorldWideName (const UI8 name[8])
    : m_separator (':')
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = name[i];
    }
}

WorldWideName::WorldWideName (const string &worldWideNameInStringFormat)
    : m_separator (':')
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    fromString (worldWideNameInStringFormat);
}

WorldWideName::WorldWideName (const WorldWideName &worldWideName)
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = worldWideName[i];
    }

    m_separator = worldWideName.m_separator;
}

WorldWideName::~WorldWideName ()
{
    if (NULL != m_pName)
    {
        delete[] m_pName;
        m_pName = NULL;
    }
}

string WorldWideName::toString () const
{
    string worldWideNameInStringFormat;
    char   tempBuffer[4] = {0};
    UI32   i   = 0;

    for (i = 0; i < (m_nameLength - 1); i++)
    {
        snprintf (tempBuffer, 4, "%02X%c", m_pName[i], m_separator);

        worldWideNameInStringFormat += tempBuffer;
    }

    snprintf (tempBuffer, 4, "%02X", m_pName[m_nameLength -1]);

    worldWideNameInStringFormat += tempBuffer;

    return (worldWideNameInStringFormat);
}

void WorldWideName::fromString (const string &worldWideNameInStringFormat)
{
          UI32  givenStringLength    = worldWideNameInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = worldWideNameInStringFormat.c_str ();
          char tempChar;
          UI32 tempValue             = 0;

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_FATAL, string("WorldWideName::fromString : Invalid World Wide Name String : ") + worldWideNameInStringFormat + string(" expectedStringLength=") + expectedStringLength + string(" givenStringLength=") + givenStringLength);
        prismAssert (false, __FILE__, __LINE__);
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_FATAL, "WorldWideName::fromString : Invalid World Wide Name in String Format : " + worldWideNameInStringFormat);
            prismAssert (false, __FILE__, __LINE__);
        }

        tempChar  = toupper (tempChar);

        if ((tempChar >= '0') && (tempChar <= '9'))
        {
            tempValue = tempChar - '0';
        }
        else if ((tempChar >= 'A') && (tempChar <= 'F'))
        {
            tempValue = tempChar - 'A' + 10;
        }

        m_pName[i] = tempValue << 4;

        tempChar  = *(pWwnCString + ((3  * i) + 1));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_FATAL, "WorldWideName::fromString : Invalid World Wide Name in String Format : " + worldWideNameInStringFormat);
            prismAssert (false, __FILE__, __LINE__);
        }

        tempChar  = toupper (tempChar);

         if ((tempChar >= '0') && (tempChar <= '9'))
         {
            tempValue = tempChar - '0';
         }
         else if ((tempChar >= 'A') && (tempChar <= 'F'))
         {
            tempValue = tempChar - 'A' + 10;
         }

        m_pName[i] |= tempValue;
    }
}

ResourceId WorldWideName::loadFromPlainString (const string &worldWideNameInPlainStringFormat)
{
          UI32  givenStringLength    = worldWideNameInPlainStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = worldWideNameInPlainStringFormat.c_str ();
          char tempChar;
          UI32 tempValue             = 0;
          ResourceId status          = WAVE_MESSAGE_ERROR;  

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, string("WorldWideName::loadFromPlainString : Invalid World Wide Name String : ") + worldWideNameInPlainStringFormat + string(" expectedStringLength=") + expectedStringLength + string(" givenStringLength=") + givenStringLength);
        return status;          
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, "WorldWideName::loadFromPlainString : Invalid World Wide Name in String Format : " + worldWideNameInPlainStringFormat);
            return status;          
        }

        tempChar  = toupper (tempChar);

        if ((tempChar >= '0') && (tempChar <= '9'))
        {
            tempValue = tempChar - '0';
        }
        else if ((tempChar >= 'A') && (tempChar <= 'F'))
        {
            tempValue = tempChar - 'A' + 10;
        }

        m_pName[i] = tempValue << 4;

        tempChar  = *(pWwnCString + ((3  * i) + 1));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, "WorldWideName::loadFromPlainString : Invalid World Wide Name in String Format : " + worldWideNameInPlainStringFormat);
            return status;          
        }

        tempChar  = toupper (tempChar);

         if ((tempChar >= '0') && (tempChar <= '9'))
         {
            tempValue = tempChar - '0';
         }
         else if ((tempChar >= 'A') && (tempChar <= 'F'))
         {
            tempValue = tempChar - 'A' + 10;
         }

        m_pName[i] |= tempValue;
    }

    status = WAVE_MESSAGE_SUCCESS;
    return status;
}

bool WorldWideName::isValidWorldWideNameString (const string &worldWideNameInStringFormat)
{
          UI32  givenStringLength    = worldWideNameInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = worldWideNameInStringFormat.c_str ();
          char tempChar;

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, string("WorldWideName::isValidWorldWideNameString : Invalid WorldWide Name string length. Expected: ") + expectedStringLength + string(" Got:") + givenStringLength + string(" for arg:") + worldWideNameInStringFormat);
        return (false);
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, string("WorldWideName::isValidWorldWideNameString : Invalid char:") + tempChar + string(" at index:") + (3*i) + string(" for WorldWide arg:") + worldWideNameInStringFormat);
            return (false);
        }

        tempChar  = *(pWwnCString + ((3  * i) + 1));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, string("WorldWideName::isValidWorldWideNameString : Invalid char:") + tempChar + string(" at index:") + (3*i+1) + string(" for WorldWide arg:") + worldWideNameInStringFormat);
            return (false);
        }
    }

    return (true);
}

void WorldWideName::setSeparator (const char &separator)
{
    m_separator = separator;
}

bool WorldWideName::isAValidHexaDecimalCharacter (const char &ch)
{
    char tempChar = toupper (ch);
    bool isValid  = false;

    if (((tempChar >= '0') && (tempChar <= '9')) || ((tempChar >= 'A') && (tempChar <= 'F')))
    {
        isValid = true;
    }
    else
    {
        isValid = false;
    }

    return (isValid);
}

UI8 &WorldWideName::operator [] (const UI32 &index) const
{
    if (index >= m_nameLength)
    {
        trace (TRACE_LEVEL_FATAL, string ("WorldWideName::operator []: Invalid Index : ") + index + string (", Maximum allowed is : ") + (m_nameLength -1));
        prismAssert (false, __FILE__, __LINE__);
    }

    return (m_pName[index]);
}


bool WorldWideName::operator == (const WorldWideName &worldWideName) const
{
    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        if (m_pName[i] != worldWideName[i])
        {
            return (false);
        }
    }

    return (true);
}

bool WorldWideName::operator != (const WorldWideName &worldWideName) const
{
    return (! (worldWideName == (*this)));
}

WorldWideName &WorldWideName::operator = (const WorldWideName &worldWideName)
{
    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = worldWideName[i];
    }

    m_separator = worldWideName.m_separator;

    return (*this);
}

char WorldWideName::getSeparator () const
{
    return (m_separator);
}

}
