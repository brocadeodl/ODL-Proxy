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

#include "Framework/Types/StringUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

StringUC::StringUC ()
{
    m_pString           = "";
    m_pIsUserConfigured = false;
}

StringUC::StringUC (const string &pString)
{
    m_pString             = pString;
    m_pIsUserConfigured = false;
}

StringUC::StringUC (const string &pString, const bool &isUserConfigured)
{
    m_pString           = pString;
    m_pIsUserConfigured = isUserConfigured;
}

StringUC::StringUC (const StringUC &stringUC)
{
    m_pString           = stringUC.m_pString;
    m_pIsUserConfigured = stringUC.m_pIsUserConfigured;
}

void StringUC::setStringValue (const string &pString)
{
    m_pString = pString;
}

string StringUC::getStringValue () const
{
    return (m_pString);
}

void StringUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool StringUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

string  StringUC::toString ()
{
    char   buffer [256] = {0}; 
    string tempString;
    UI32   stringLength = m_pString.size ();

    snprintf (buffer, 256, "%u#%s:%u", stringLength, m_pString.c_str (), (UI32) m_pIsUserConfigured);
    
    tempString = buffer;

    return (tempString);
}

void StringUC::getPlainString (string &plainString) const
{
    plainString = m_pString;
}

void StringUC::fromString (const string &pStringUCInStringFormat)
{
    UI32            stringLength;
    char            buffer[256];
    string          tempString;
    UI32            tempIsUserConfigured;

    sscanf (pStringUCInStringFormat.c_str (), "%u#%s", &stringLength, buffer);

    // If a string (of type stringUC) is stored in a database with multiple words with spaces in between e.g. "hello there".
    // then with sscanf above, buffer only gets first word i.e. hello. Moreover, tempStringForIsConfigured (below) will be 0 tempString is 
    // truncated. So, the string will not be shown as userConfigured flag is false.
    // With below changes, tempString will be complete irrespective of the spaces in between. 

    size_t position = pStringUCInStringFormat.find ("#");
    if (position == string::npos)
    {
        trace (TRACE_LEVEL_FATAL, "StringUC::fromString : pStringUCInStringFormat must have #");
        prismAssert (false, __FILE__, __LINE__);
    }
    
    tempString = pStringUCInStringFormat.substr (position + 1);

    if (stringLength < tempString.size ())
    {
        string tempStringForIsConfigured = tempString.substr ((stringLength + 1));

        prismAssert (1 == tempStringForIsConfigured.size (), __FILE__, __LINE__);

        tempIsUserConfigured = strtoul(tempStringForIsConfigured.c_str (), NULL, 10);
        m_pIsUserConfigured  = tempIsUserConfigured ? true : false;
    }

    m_pString = tempString.substr (0, stringLength);
}

ResourceId StringUC::loadFromPlainString (const string &pStringUCInplainStringFormat)
{
    m_pIsUserConfigured  = true;
    m_pString            = pStringUCInplainStringFormat;

    return WAVE_MESSAGE_SUCCESS;
}

bool StringUC::operator == (const StringUC &pStringUC) const
{
    if ((m_pString != pStringUC.m_pString) || (m_pIsUserConfigured != pStringUC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool StringUC::operator != (const StringUC &pStringUC) const
{
    return (! (operator == (pStringUC)));
}

StringUC &StringUC::operator = (const StringUC &pStringUC)
{
    m_pString           = pStringUC.m_pString;
    m_pIsUserConfigured = pStringUC.m_pIsUserConfigured;

    return (*this);
}

}
