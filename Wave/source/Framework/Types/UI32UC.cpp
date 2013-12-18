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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Types/UI32UC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

UI32UC::UI32UC ()
{
    m_pUI32             = 0;
    m_pIsUserConfigured = false;
}

UI32UC::UI32UC (const UI32 &ui32)
{
    m_pUI32             = ui32;
    m_pIsUserConfigured = false;
}

UI32UC::UI32UC (const UI32 &ui32, const bool &isUserConfigured)
{
    m_pUI32             = ui32;
    m_pIsUserConfigured = isUserConfigured;
}

UI32UC::UI32UC (const UI32UC &ui32UC)
{
    m_pUI32             = ui32UC.m_pUI32;
    m_pIsUserConfigured = ui32UC.m_pIsUserConfigured;
}

void UI32UC::setUI32Value (const UI32 &ui32)
{
    m_pUI32 = ui32;
}

UI32 UI32UC::getUI32Value () const
{
    return (m_pUI32);
}

void UI32UC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool UI32UC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

string  UI32UC::toString ()
{
    char buffer[256] = {0}; 
    string tempString;

    snprintf (buffer, 256, "%u:%u", m_pUI32, m_pIsUserConfigured ? 1 : 0);
    
    tempString = buffer;

    return (tempString);
}

void UI32UC::getPlainString (string &plainString) const
{
    char buffer[256] = {0};

    snprintf (buffer, 256, "%u", m_pUI32);
    
    plainString = buffer;
}


void UI32UC::fromString (const string &ui32UCInStringFormat)
{
    string tempString = ui32UCInStringFormat;
    vector<string>  token;
    UI32 numberOfToken = 0;
    UI32 tempIsUserConfigured;

    tokenize (tempString, token, ':');

    numberOfToken = token.size ();

    prismAssert (2 >= numberOfToken, __FILE__, __LINE__);

    if (numberOfToken > 1)
    {
        tempIsUserConfigured =  strtoul (token[1].c_str (), NULL, 10);
        m_pIsUserConfigured  =  tempIsUserConfigured ? true : false;
    }
    else
    {
        m_pIsUserConfigured = true;
    }

    m_pUI32 = strtoul (token[0].c_str (), NULL, 10);
}

ResourceId UI32UC::loadFromPlainString (const string &pUI32UCInplainStringFormat)
{
    char*         firstInvalidCharacterPosition = NULL;
    ResourceId    status                        = WAVE_MESSAGE_ERROR;
                  m_pIsUserConfigured           = true;

    m_pUI32 = strtoul (pUI32UCInplainStringFormat.c_str(), &(firstInvalidCharacterPosition), 10);

    // Check if the string has invalid character
    if ( *firstInvalidCharacterPosition == '\0' )
    {   
        status =  WAVE_MESSAGE_SUCCESS;
    }

    ///// print error 
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("UI32UC::loadFromPlainString : Improper format of UI32UC in string"));
    }

    return status;
}

bool UI32UC::operator == (const UI32UC &ui32UC) const
{
    if ((m_pUI32 != ui32UC.m_pUI32) || (m_pIsUserConfigured != ui32UC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool UI32UC::operator != (const UI32UC &ui32UC) const
{
    return (! (operator == (ui32UC)));
}

UI32UC &UI32UC::operator = (const UI32UC &ui32UC)
{
    m_pUI32             = ui32UC.m_pUI32;
    m_pIsUserConfigured = ui32UC.m_pIsUserConfigured;

    return (*this);
}

}
