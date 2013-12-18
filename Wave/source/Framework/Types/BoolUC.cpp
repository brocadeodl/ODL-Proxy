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

#include "Framework/Types/BoolUC.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

BoolUC::BoolUC ()
{
    m_pBool             = false;
    m_pIsUserConfigured = false;
}

BoolUC::BoolUC (const bool &pBool)
{
    m_pBool             = pBool;
    m_pIsUserConfigured = false;
}

BoolUC::BoolUC (const bool &pBool, const bool &isUserConfigured)
{
    m_pBool             = pBool;
    m_pIsUserConfigured = isUserConfigured;
}

BoolUC::BoolUC (const BoolUC &boolUC)
{
    m_pBool             = boolUC.m_pBool;
    m_pIsUserConfigured = boolUC.m_pIsUserConfigured;
}

void BoolUC::setBoolValue (const bool &pBool)
{
    m_pBool = pBool;
}

bool BoolUC::getBoolValue () const
{
    return (m_pBool);
}

void BoolUC::setIsUserConfigured (const bool &isUserConfigured)
{
    m_pIsUserConfigured = isUserConfigured;
}

bool BoolUC::getIsUserConfigured () const
{
    return (m_pIsUserConfigured);
}

string  BoolUC::toString ()
{
    char buffer[256] = {0}; 
    string tempString;

    snprintf (buffer, 256, "%u:%u", (m_pBool ? 1 : 0), (m_pIsUserConfigured ? 1 : 0));
    
    tempString = buffer;

    return (tempString);
}

void BoolUC::getPlainString (string &valueString) const
{
    if (true == m_pBool)
    {
        valueString = "true";
    }
    else
    {
        valueString = "false";
    }
}

void BoolUC::fromString (const string &pBoolUCInStringFormat)
{
    string tempString = pBoolUCInStringFormat;
    vector<string>  token;
    UI32 numberOfToken = 0;
    UI32 tempBool;
    UI32 tempIsUserConfigured;
    
    tokenize (tempString, token, ':');

    numberOfToken = token.size ();

    prismAssert (2 >= numberOfToken, __FILE__, __LINE__);

    if (numberOfToken > 1)
    {
        tempIsUserConfigured = strtoul (token[1].c_str (), NULL, 10);
        m_pIsUserConfigured  = tempIsUserConfigured ? true : false;
    }

    tempBool = strtoul (token[0].c_str (), NULL, 10);

    m_pBool  = tempBool ? true : false;
}


ResourceId BoolUC::loadFromPlainString (const string &pBoolUCInPlainStringFormat)
{
    ResourceId status    = WAVE_MESSAGE_ERROR;
    m_pIsUserConfigured  = true ;

    status =  WAVE_MESSAGE_ERROR;
    
    if (("true" == pBoolUCInPlainStringFormat) || ("t" == pBoolUCInPlainStringFormat))
    {   
       m_pBool = true;
       status = WAVE_MESSAGE_SUCCESS;
    }
    else if (("false" == pBoolUCInPlainStringFormat) || ("f" == pBoolUCInPlainStringFormat))
    {   
       m_pBool =  false;
       status = WAVE_MESSAGE_SUCCESS;
    }
    else
    {   
        trace (TRACE_LEVEL_ERROR, string("BoolUC::loadFromPlainString : Improper format of BoolUC in string"));
    }

    return status;
}

bool BoolUC::operator == (const BoolUC &pBoolUC) const
{
    if ((m_pBool != pBoolUC.m_pBool) || (m_pIsUserConfigured != pBoolUC.m_pIsUserConfigured))
    {
        return (false);
    }

    return (true);
}

bool BoolUC::operator != (const BoolUC &pBoolUC) const
{
    return (! (operator == (pBoolUC)));
}

BoolUC &BoolUC::operator = (const BoolUC &pBoolUC)
{
    m_pBool             = pBoolUC.m_pBool;
    m_pIsUserConfigured = pBoolUC.m_pIsUserConfigured;

    return (*this);
}

}
