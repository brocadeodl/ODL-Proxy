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
 *   Author : Mandar Datar                                                 *
 *   Note   : Copied from Framework/Types/UI32UC.cpp                       *
 *            Changed one strtoul to strtoull                              *
 ***************************************************************************/

#include "Framework/Types/UI64UC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UI64UC::UI64UC ()
{
    m_pUI64             = 0;
    m_pIsUserConfigured = false;
}

UI64UC::UI64UC (const UI64 &ui64)
{
    m_pUI64             = ui64;
    m_pIsUserConfigured = false;
}

UI64UC::UI64UC (const UI64 &ui64, const bool &isUserConfigured)
{
    m_pUI64             = ui64;
    m_pIsUserConfigured = isUserConfigured;
}

UI64UC::UI64UC (const UI64UC &ui64UC)
{
    m_pUI64             = ui64UC.m_pUI64;
    m_pIsUserConfigured = ui64UC.m_pIsUserConfigured;
}

void UI64UC::setUI64Value (const UI64 &ui64)
{
    m_pUI64 = ui64;
}

UI64 UI64UC::getUI64Value () const
{
    return (m_pUI64);
}

void UI64UC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool UI64UC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

string  UI64UC::toString ()
{
    char buffer[256] = {0}; 
    string tempString;

    snprintf (buffer, 256, "%llu:%u", (long long unsigned int) m_pUI64, m_pIsUserConfigured ? 1 : 0);
    
    tempString = buffer;

    return (tempString);
}


void UI64UC::getPlainString (string &plainString) const
{
    char buffer[256] = {0};

    snprintf (buffer, 256, "%llu", (long long unsigned int) m_pUI64);
    
    plainString = buffer;
}


void UI64UC::fromString (const string &ui64UCInStringFormat)
{
    string tempString = ui64UCInStringFormat;
    vector<string>  token;
    UI64 numberOfToken = 0;
    UI32 tempIsUserConfigured;

    tokenize (tempString, token, ':');

    numberOfToken = token.size ();

    prismAssert (2 >= numberOfToken, __FILE__, __LINE__);

    if (numberOfToken > 1)
    {
        tempIsUserConfigured =  strtoull (token[1].c_str (), NULL, 10);
        m_pIsUserConfigured  =  tempIsUserConfigured ? true : false;
    }

    m_pUI64 = strtoull (token[0].c_str (), NULL, 10);
}

ResourceId UI64UC::loadFromPlainString (const string &pUI64UCInplainStringFormat)
{
    char*         firstInvalidCharacterPosition = NULL;
    ResourceId    status                        = WAVE_MESSAGE_ERROR;
                  m_pIsUserConfigured           = true;
    
    m_pUI64 = strtoull (pUI64UCInplainStringFormat.c_str(), &(firstInvalidCharacterPosition),10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        status = WAVE_MESSAGE_SUCCESS;
    }

    ///// print error 
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("UI64UC::loadFromPlainString : Improper format of UI64UC in string"));
    }

    return status;
}

bool UI64UC::operator == (const UI64UC &ui64UC) const
{
    if ((m_pUI64 != ui64UC.m_pUI64) || (m_pIsUserConfigured != ui64UC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool UI64UC::operator != (const UI64UC &ui64UC) const
{
    return (! (operator == (ui64UC)));
}

UI64UC &UI64UC::operator = (const UI64UC &ui64UC)
{
    m_pUI64             = ui64UC.m_pUI64;
    m_pIsUserConfigured = ui64UC.m_pIsUserConfigured;

    return (*this);
}

}
