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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/Types/Decimal64.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

Decimal64::Decimal64 ()
{
    m_val               = 0;
    m_fractionDigits    = 0;
}

Decimal64::Decimal64 (const string &decimal64InStringFormat)
{
    fromString (decimal64InStringFormat);
}

Decimal64::Decimal64 (const Decimal64 &decimal64)
{
    m_val             = decimal64.m_val;
    m_fractionDigits  = decimal64.m_fractionDigits;
}

Decimal64::~Decimal64 ()
{
}

string Decimal64::toString () const
{
    string          decimal64InStringFormat;
    int             insertPos;
    char            tempBuf[32] = {0};

    // convert the entire value in string
    snprintf(tempBuf, 32, "%lld", m_val);

    decimal64InStringFormat = tempBuf;

    // insert 0s if m_val length is lesser than where '.' is to be inserted
    while (decimal64InStringFormat.length() < m_fractionDigits)
    {   
        // insert at location 0, a '0'
        decimal64InStringFormat.insert(0, 1, '0');
        //cout << "decimal64InStringFormat length was less than m_fractionDigits. decimal64InStringFormat.length=>" << decimal64InStringFormat.length() << endl;
        //cout << "decimal64InStringFormat after adding 0 =>" << decimal64InStringFormat << endl;
    }
    
    // insert . based on fraction digits value
    insertPos = decimal64InStringFormat.length() - m_fractionDigits;
    decimal64InStringFormat.insert(insertPos, 1, '.');
    //cout << "after insert, decimal64InStringFormat=>" << decimal64InStringFormat << endl;

    return (decimal64InStringFormat);
}

void Decimal64::fromString(const string &decimal64InStringFormat)
{
    int             decimalPosFromStart;
    string          tmpStr;
    vector<string>  decimal64StringVector;

    // tokenize input to get strings before & after '.'
    tokenize (decimal64InStringFormat, decimal64StringVector, '.');

    decimalPosFromStart = decimal64InStringFormat.find('.');

    // input is either not having a '.' OR it starts with a '.'
    if (decimal64StringVector.size() == 1)
    {
        if (decimalPosFromStart == 0)
        {
            // First char is '.'
            m_fractionDigits = decimal64StringVector[0].size();
            m_val = strtoul (decimal64StringVector[0].c_str(), NULL, 10);
        }
        else
        {
            // no '.' in input
            m_fractionDigits = 0;
            m_val = strtoul (decimal64StringVector[0].c_str(), NULL, 10);
        }
    }   
    else if (decimal64StringVector.size() == 2)
    {
        m_fractionDigits = decimal64StringVector[1].size();

        // Just converting first index of vector won't give correct val. We need both
        tmpStr = decimal64InStringFormat.substr(0, decimalPosFromStart);
        tmpStr += decimal64InStringFormat.substr(decimalPosFromStart+1, decimal64InStringFormat.length());
        m_val = strtoul (tmpStr.c_str(), NULL, 10);
    }   
    else
    {
        trace (TRACE_LEVEL_ERROR, string("Decimal64::fromString : Improper format of decimal64 in string"));
        return;
    }   

    //cout << "m_val=>" << m_val << endl;
    //cout << "m_fractionDigits=>" << m_fractionDigits << endl;
}

ResourceId Decimal64::loadFromPlainString(const string &decimal64InStringFormat)
{
    int             decimalPosFromStart;
    string          tmpStr;
    vector<string>  decimal64StringVector;
    ResourceId      status = WAVE_MESSAGE_ERROR;

    // tokenize input to get strings before & after '.'
    tokenize (decimal64InStringFormat, decimal64StringVector, '.');

    decimalPosFromStart = decimal64InStringFormat.find('.');

    // input is either not having a '.' OR it starts with a '.'
    if (decimal64StringVector.size() == 1)
    {
        if (decimalPosFromStart == 0)
        {
            // First char is '.'
            char* firstInvalidCharacterPosition = NULL;

            m_fractionDigits = decimal64StringVector[0].size();

            m_val = strtoul (decimal64StringVector[0].c_str(), &(firstInvalidCharacterPosition), 10);

            if ( *firstInvalidCharacterPosition != '\0')
            {
                trace (TRACE_LEVEL_ERROR, string("Decimal64::fromString : Improper format of decimal64 in string"));
            }
            else
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
        else
        {
            // no '.' in input
            char* firstInvalidCharacterPosition = NULL;

            m_fractionDigits = 0;
            m_val = strtoul (decimal64StringVector[0].c_str(), &(firstInvalidCharacterPosition), 10);
            
            if ( *firstInvalidCharacterPosition != '\0')
            {
                trace (TRACE_LEVEL_ERROR, string("Decimal64::fromString : Improper format of decimal64 in string"));
            }
            else
            {
                status = WAVE_MESSAGE_SUCCESS;
            }
        }
    }   
    else if (decimal64StringVector.size() == 2)
    {
        char* firstInvalidCharacterPosition = NULL;
              m_fractionDigits              = decimal64StringVector[1].size();

        // Just converting first index of vector won't give correct val. We need both
        tmpStr = decimal64InStringFormat.substr(0, decimalPosFromStart);
        tmpStr += decimal64InStringFormat.substr(decimalPosFromStart+1, decimal64InStringFormat.length());
        m_val = strtoul (tmpStr.c_str(), &(firstInvalidCharacterPosition), 10);
        
        if ( *firstInvalidCharacterPosition != '\0')
        {
            trace (TRACE_LEVEL_ERROR, string("Decimal64::fromString : Improper format of decimal64 in string"));
        }
        else
        {
            status = WAVE_MESSAGE_SUCCESS;
        }
    }   
    else
    {
        trace (TRACE_LEVEL_ERROR, string("Decimal64::fromString : Improper format of decimal64 in string"));
    }   

    //cout << "m_val=>" << m_val << endl;
    //cout << "m_fractionDigits=>" << m_fractionDigits << endl;

    return status;
}

bool Decimal64::operator == (const Decimal64 &decimal64) const
{
    if ((m_val != decimal64.m_val) || (m_fractionDigits != decimal64.m_fractionDigits))
    {
        return (false);
    }
    return (true);
}

bool Decimal64::operator != (const Decimal64 &decimal64) const
{
    return (! (decimal64 == (*this)));
}

Decimal64 &Decimal64::operator = (const Decimal64 &decimal64)
{
    m_val = decimal64.m_val;
    m_fractionDigits = decimal64.m_fractionDigits;

    return (*this);
}

SI64 Decimal64::getVal() const
{
    return(m_val);
}

UI8 Decimal64::getFractionDigits() const
{
    return(m_fractionDigits);
}

void Decimal64::setAllValues(SI64 val, UI8 fractiondigits)
{
    m_val = val;
    m_fractionDigits = fractiondigits;
}

}
