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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#include "Framework/Types/StringVectorUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

StringVectorUC::StringVectorUC ()
{
    m_pStringVector.clear ();
    m_pIsUserConfigured = false;
}

StringVectorUC::StringVectorUC (const vector<string> &stringVector)
{
    m_pStringVector     = stringVector;
    m_pIsUserConfigured = false;
}

StringVectorUC::StringVectorUC (const vector<string> &stringVector, const bool &isUserConfigured)
{
    m_pStringVector     = stringVector;
    m_pIsUserConfigured = isUserConfigured;
}

StringVectorUC::StringVectorUC (const StringVectorUC &stringVectorUC)
{
    m_pStringVector     = stringVectorUC.getStringVector ();
    m_pIsUserConfigured = stringVectorUC.m_pIsUserConfigured;
}

StringVectorUC::~StringVectorUC ()
{
}

void StringVectorUC::setStringVector(const vector<string> &stringVector)
{
    m_pStringVector = stringVector;
}

vector<string> StringVectorUC::getStringVector() const
{
    return (m_pStringVector);
}

void StringVectorUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool StringVectorUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

void StringVectorUC::toString (string &valueString)
{
    vector<string>::iterator element = m_pStringVector.begin ();
    vector<string>::iterator end     = m_pStringVector.end ();

    valueString = "";

    while (end != element)
    {
        valueString = valueString + (*element).size () + "#" + (*element);
        element++;
    }

    valueString = (m_pIsUserConfigured ? "T" : "F") + valueString;
}

void StringVectorUC::fromString (const string &stringVectorUCInStringFormat)
{
    string tempString = stringVectorUCInStringFormat;

    UI32 tempIsUserConfigured;

    if ('T' == tempString [0])
    {
        tempIsUserConfigured = 1;
        tempString.erase (0, 1);
    }
    else if ('F' == tempString [0])
    {
        tempIsUserConfigured = 0;
        tempString.erase (0, 1);
    }
    else
    {
        tempIsUserConfigured = 1;
    }

    vector<string>::iterator begin = m_pStringVector.begin ();
    vector<string>::iterator end   = m_pStringVector.end ();

    m_pStringVector.erase (begin, end);

    ULI  startFromIndex = 0;
    ULI  firstIndex     = 0;
    UI32 stringSize     = 0;

    while (string::npos != (firstIndex = tempString.find ("#", startFromIndex)))
    {
        stringSize = atoi (tempString.c_str () + startFromIndex);

        m_pStringVector.push_back (tempString.substr (firstIndex + 1, stringSize));

        startFromIndex = firstIndex + stringSize + 1;
    }

    m_pIsUserConfigured  =  tempIsUserConfigured ? true : false;    
}

bool StringVectorUC::operator == (const StringVectorUC &stringVectorUC) const
{
    if ((m_pStringVector != stringVectorUC.getStringVector ()) || (m_pIsUserConfigured != stringVectorUC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool StringVectorUC::operator != (const StringVectorUC &stringVectorUC) const
{
    return (! (operator == (stringVectorUC)));
}

StringVectorUC &StringVectorUC::operator = (const StringVectorUC &stringVectorUC)
{
    m_pStringVector     = stringVectorUC.getStringVector ();
    m_pIsUserConfigured = stringVectorUC.m_pIsUserConfigured;

    return (*this);
}

string StringVectorUC::getPlainString () const
{
    vector<string>::const_iterator itrElement   = m_pStringVector.begin ();
    vector<string>::const_iterator endElement   = m_pStringVector.end ();

    string valueString;

    bool isSeparatorRequired = false;

    for (; endElement != itrElement; itrElement++)
    {
        if (false == isSeparatorRequired)
        {
            isSeparatorRequired = true;
        }
        else
        {
            valueString += " ";
        }

        valueString = valueString + (*itrElement);
    }

    return valueString;
}

}
