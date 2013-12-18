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
 *   Author : Pritee Kadu                                                  *
 ***************************************************************************/

#include "Framework/Types/MacAddress2.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

MacAddress2::MacAddress2 ()
    : m_separator ('.')
{
    m_pName = new UI16[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);
}

MacAddress2::MacAddress2 (const UI16 name[3])
    : m_separator ('.')
{
    m_pName = new UI16[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = name[i];
    }
}

MacAddress2::MacAddress2 (const string &macAddressInStringFormat)
    : m_separator ('.')
{
    m_pName = new UI16[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    fromString (macAddressInStringFormat);
}

MacAddress2::MacAddress2 (const MacAddress2 &macAddress)
{
    m_pName = new UI16[m_nameLength];

    prismAssert (NULL != m_pName, __FILE__, __LINE__);

    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = macAddress[i];
    }

    m_separator = macAddress.m_separator;
}

MacAddress2::~MacAddress2 ()
{
    if (NULL != m_pName)
    {
        delete[] m_pName;
        m_pName = NULL;
    }
}

string MacAddress2::toString () const
{
    string macAddressInStringFormat;
    char   tempBuffer[5] = {0};
    UI32   i = 0;

    for (i = 0; i < (m_nameLength - 1); i++)
    { 
        snprintf (tempBuffer, 5, "%04X", m_pName[i]);

        macAddressInStringFormat += tempBuffer;
        macAddressInStringFormat += m_separator;
    }
    snprintf (tempBuffer, 5, "%04X", m_pName[m_nameLength -1]);

    macAddressInStringFormat += tempBuffer;

    return (macAddressInStringFormat);
}

void MacAddress2::fromString (const string &macAddressInStringFormat)
{
          UI32  givenStringLength    = macAddressInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 5 - 1;
          UI32  i                    = 0;
          UI32  pos                  = 0;
    const char *pWwnCString          = macAddressInStringFormat.c_str ();
          char tempChar;
          UI16 tempValue             = 0;

    prismAssert (NULL != m_pName, __FILE__, __LINE__);
    
    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_FATAL, "MacAddress2::fromString : Invalid MacAddress2 String : " + macAddressInStringFormat);
        prismAssert (false, __FILE__, __LINE__);
    }

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = 0;
        for (pos = 0; pos < m_maxCharInAUnit; pos++)
        {
            tempChar  = *(pWwnCString + (i * 5) + pos);
            

            if (false == (isAValidHexaDecimalCharacter (tempChar)))
            {
                trace (TRACE_LEVEL_FATAL, "MacAddress2::fromString : Invalid MacAddress2 in String Format : " + macAddressInStringFormat);
                prismAssert (false, __FILE__, __LINE__);
            }
            tempValue = getValueFromChar (tempChar); 

            tempValue = tempValue << ((m_maxCharInAUnit*4) - ((pos+1)*4));
            
            m_pName[i] |= tempValue;
        }
    }
}

ResourceId MacAddress2::loadFromPlainString (const string &macAddressInStringFormat)
{
          UI32       givenStringLength    = macAddressInStringFormat.size ();
    const UI32       expectedStringLength = m_nameLength * 5 - 1;
          UI32       i                    = 0;
          UI32       pos                  = 0;
    const char      *pWwnCString          = macAddressInStringFormat.c_str ();
          char       tempChar;
          UI16       tempValue            = 0;
          ResourceId status               = WAVE_MESSAGE_ERROR;

    prismAssert (NULL != m_pName, __FILE__, __LINE__);
    
    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, "MacAddress2::loadFromPlainString : Invalid MacAddress2 String : " + macAddressInStringFormat);
        return status;
    }

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = 0;
        for (pos = 0; pos < m_maxCharInAUnit; pos++)
        {
            tempChar  = *(pWwnCString + (i * 5) + pos);
            

            if (false == (isAValidHexaDecimalCharacter (tempChar)))
            {
                trace (TRACE_LEVEL_ERROR, "MacAddress2::loadFromPlainString : Invalid MacAddress2 in String Format : " + macAddressInStringFormat);
                return status;
            }
            tempValue = getValueFromChar (tempChar); 

            tempValue = tempValue << ((m_maxCharInAUnit*4) - ((pos+1)*4));
            
            m_pName[i] |= tempValue;
        }
    }

    status = WAVE_MESSAGE_SUCCESS;

    return status;
}

UI16 MacAddress2::getValueFromChar (char ch)
{
    UI32 value = 0;
    
    ch = toupper (ch);
    if ((ch >= '0') && (ch <= '9'))
    {
        value = ch - '0';
    }
    else if ((ch >= 'A') && (ch <= 'F'))
    {
        value = ch - 'A' + 10;
    }
    return value;
}

bool MacAddress2::isValidMacAddress2String (const string &macAddressInStringFormat)
{
          UI32  givenStringLength    = macAddressInStringFormat.size ();
    const UI32  expectedStringLength = m_nameLength * 5 - 1;                
          UI32  i                    = 0;
    const char *pWwnCString          = macAddressInStringFormat.c_str ();
          char tempChar;

    if (expectedStringLength != givenStringLength)
    {
        trace (TRACE_LEVEL_ERROR, string("MacAddress2::isValidMacAddress2String : Invalid MacAddress2 string length. Expected: ") + expectedStringLength + string(" Got:") + givenStringLength + string(" for arg:") + macAddressInStringFormat);
        return (false);
    }

    for (i = 0; i < expectedStringLength; i++)
    {
        tempChar  = *(pWwnCString + i);
        if (!((i+1) % 5))
        {
            if ('.' != tempChar)
            {
                trace (TRACE_LEVEL_ERROR, string("MacAddress2::isValidMacAddress2String : Invalid seperator :") + tempChar + string("  expecting '.' as seperator for MacAddress2 arg: ") + macAddressInStringFormat);
                return (false);
            }
            continue;
        }

        if (false == (isAValidHexaDecimalCharacter (tempChar)))
        {
            trace (TRACE_LEVEL_ERROR, string("MacAddress2::isValidMacAddress2String : Invalid char:") + tempChar + string(" at index ") + (i/5) + string(" for MacAddress2 arg: ") + macAddressInStringFormat);
            return (false);
        }
    }
    return (true);
}

void MacAddress2::setSeparator (const char &separator)
{
    m_separator = separator;
}

bool MacAddress2::isAValidHexaDecimalCharacter (const char &ch)
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

UI16 &MacAddress2::operator [] (const UI32 &index) const
{
    if (index >= m_nameLength)
    {
        trace (TRACE_LEVEL_FATAL, string ("MacAddress2::operator []: Invalid Index : ") + index + string (", Maximum allowed is : ") + (m_nameLength -1));
        prismAssert (false, __FILE__, __LINE__);
    }

    return (m_pName[index]);
}


bool MacAddress2::operator == (const MacAddress2 &macAddress) const
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

bool MacAddress2::operator != (const MacAddress2 &macAddress) const
{
    return (! (macAddress == (*this)));
}

MacAddress2 &MacAddress2::operator = (const MacAddress2 &macAddress)
{
    UI32 i = 0;

    for (i = 0; i < m_nameLength; i++)
    {
        m_pName[i] = macAddress[i];
    }

    m_separator = macAddress.m_separator;

    return (*this);
}

char MacAddress2::getSeparator () const
{
    return (m_separator);
}

}
