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

#include "Framework/Types/MacAddress.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

MacAddress::MacAddress ()
    : m_separator (':')
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);
}

MacAddress::MacAddress (const UI8 name[6])
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

MacAddress::MacAddress (const string &macAddressInStringFormat)
    : m_separator (':')
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    fromString (macAddressInStringFormat);
}

MacAddress::MacAddress (const MacAddress &macAddress)
{
    m_pName = new UI8[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = macAddress[i];
    }

    m_separator = macAddress.m_separator;
}

MacAddress::~MacAddress ()
{
    if (NULL != m_pName)
    {
        delete[] m_pName;
        m_pName = NULL;
    }
}

string MacAddress::toString () const
{
    string macAddressInStringFormat;
    char   tempBuffer[4] = {0};
    UI32   i = 0;

    for (i = 0; i < (m_nameLength - 1); i++)
    {
        snprintf (tempBuffer, 4, "%02X%c", m_pName[i], m_separator);

        macAddressInStringFormat += tempBuffer;
    }

    snprintf (tempBuffer, 4, "%02X", m_pName[m_nameLength -1]);

    macAddressInStringFormat += tempBuffer;

    return (macAddressInStringFormat);
}

string MacAddress::toString2 () const
{
    string macAddressInStringFormat;
    char   tempBuffer[16] = {0};

    snprintf (tempBuffer, 16, "%02X%02X.%02X%02X.%02X%02X", m_pName[0], m_pName[1], m_pName[2], m_pName[3], m_pName[4], m_pName[5]);

    macAddressInStringFormat = tempBuffer;

    return (macAddressInStringFormat);
}

void MacAddress::fromString (const string &macAddressInStringFormat)
{
          UI32  givenStringLength    = macAddressInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = macAddressInStringFormat.c_str ();
          char tempChar;
          UI32 tempValue             = 0;

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_FATAL, "MacAddress::fromString : Invalid MacAddress String : " + macAddressInStringFormat);
        prismAssert (false, __FILE__, __LINE__);
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_FATAL, "MacAddress::fromString : Invalid MacAddress in String Format : " + macAddressInStringFormat);
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
            trace (TRACE_LEVEL_FATAL, "MacAddress::fromString : Invalid MacAddress in String Format : " + macAddressInStringFormat);
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

ResourceId MacAddress::loadFromPlainString (const string &macAddressInStringFormat)
{
          UI32  givenStringLength    = macAddressInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = macAddressInStringFormat.c_str ();
          char tempChar;
          UI32 tempValue             = 0;
          ResourceId status          = WAVE_MESSAGE_ERROR;

    if(NULL == m_pName)
    {
        return status;
    }

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, "MacAddress::loadFromPlainString : Invalid MacAddress String : " + macAddressInStringFormat);
        return status;
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, "MacAddress::loadFromPlainString : Invalid MacAddress in String Format : " + macAddressInStringFormat);
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
            trace (TRACE_LEVEL_FATAL, "MacAddress::loadFromPlainString : Invalid MacAddress in String Format : " + macAddressInStringFormat);
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

bool MacAddress::isValidMacAddressString (const string &macAddressInStringFormat)
{
          UI32  givenStringLength    = macAddressInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 3 - 1;
          UI32  i                    = 0;
    const char *pWwnCString          = macAddressInStringFormat.c_str ();
          char tempChar;

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, string("MacAddress::isValidMacAddressString : Invalid MacAddress string length. Expected: ") + expectedStringLength + string(" Got:") + givenStringLength + string(" for arg:") + macAddressInStringFormat);
        return (false);
    }

    for (i = 0; i < m_nameLength; i++)
    {
        tempChar  = *(pWwnCString + (3  * i));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, string("MacAddress::isValidMacAddressString : Invalid char:") + tempChar + string(" at index ") + (3*i) + string(" for MacAddress arg: ") + macAddressInStringFormat);
            return (false);
        }

        tempChar  = *(pWwnCString + ((3  * i) + 1));

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, string("MacAddress::isValidMacAddressString : Invalid char:") + tempChar + string(" at index ") + (3*i+1) + string(" for MacAddress arg: ") + macAddressInStringFormat);
            return (false);
        }
    }

    return (true);
}

void MacAddress::setSeparator (const char &separator)
{
    m_separator = separator;
}

bool MacAddress::isAValidHexaDecimalCharacter (const char &ch)
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

UI8 &MacAddress::operator [] (const UI32 &index) const
{
    if (index >= m_nameLength)
    {
        trace (TRACE_LEVEL_FATAL, string ("MacAddress::operator []: Invalid Index : ") + index + string (", Maximum allowed is : ") + (m_nameLength -1));
        prismAssert (false, __FILE__, __LINE__);
    }

    return (m_pName[index]);
}


bool MacAddress::operator == (const MacAddress &macAddress) const
{
    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        if (m_pName[i] != macAddress[i])
        {
            return (false);
        }
    }

    return (true);
}

bool MacAddress::operator != (const MacAddress &macAddress) const
{
    return (! (macAddress == (*this)));
}

MacAddress &MacAddress::operator = (const MacAddress &macAddress)
{
    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = macAddress[i];
    }

    m_separator = macAddress.m_separator;

    return (*this);
}

char MacAddress::getSeparator () const
{
    return (m_separator);
}

}
