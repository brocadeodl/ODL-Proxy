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

#include "Framework/Types/SnmpObjectId.h"
#include "Framework/Utils/StringUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

void SnmpObjectId::copy (const SnmpObjectId &snmpObjectId)
{
    SI32 i;

    for (i = 0; i < snmpObjectId.m_Length; i++)
    {
        m_Oid[i] = snmpObjectId.m_Oid[i];
    }

    m_Length = snmpObjectId.m_Length;
}

void SnmpObjectId::parseSnmpObjectId   (string &objectIdentifier)
{
    SI32            count = 0;
    SI32            i;
    vector<string>  objectIdToken;

    tokenize (objectIdentifier, objectIdToken, '.');
    
    count  = objectIdToken.size ();

    for (i = 0; i < count; i++)
    {
        m_Oid[i]   = strtoul (objectIdToken[i].c_str (), NULL, 10);
    }

    m_Length = count;
}

SnmpObjectId::SnmpObjectId ()
{
    m_Oid[0] = 0;
    m_Length = 1; 
}

SnmpObjectId::SnmpObjectId (const string &objectIdentifier)
{
    string tempString = objectIdentifier;

    parseSnmpObjectId (tempString);
}

SnmpObjectId::SnmpObjectId (const SnmpObjectId &objectIdentifier)
{
    copy (objectIdentifier);
}

SnmpObjectId::~SnmpObjectId ()
{
}

string SnmpObjectId::toString () const
{
    char   tempBuffer[1024] = {0};
    SI32   i = 0;
    string tempString;

    for (i = 0; i < m_Length - 1; i++)
    {
        snprintf (tempBuffer, 1024, "%u.", m_Oid[i]);
        tempString.append (tempBuffer);
    }

    snprintf (tempBuffer, 1024, "%u", m_Oid[m_Length - 1]);
    tempString.append (tempBuffer);

    return tempString;
}

void SnmpObjectId::fromString (const string &objectIdentifier)
{
    string tempString = objectIdentifier;

    parseSnmpObjectId (tempString);
}


ResourceId SnmpObjectId::loadFromPlainString (const string &objectIdentifier)
{
    ResourceId      status        = WAVE_MESSAGE_SUCCESS;
    string          tempString    = objectIdentifier;
    SI32            count         = 0;
    SI32            i;
    vector<string>  objectIdToken;

    tokenize (tempString, objectIdToken, '.');
    
    count  = objectIdToken.size ();

    if(count == 0)
    {
        status = WAVE_MESSAGE_SUCCESS;
    }

    for (i = 0; i < count; i++)
    {
        char*         firstInvalidCharacterPosition = NULL;
        m_Oid[i]   = strtoul (objectIdToken[i].c_str (), &(firstInvalidCharacterPosition), 10);

        if (*firstInvalidCharacterPosition != '\0')
        {
            status = WAVE_MESSAGE_ERROR;
            break;
        }
    }
    
    if (WAVE_MESSAGE_ERROR == status)
    {
        trace (TRACE_LEVEL_ERROR, string("SnmpObjectId::loadFromPlainString : Improper format of SnmpObjectId string"));
    }

    m_Length = count;

    return status;
}


void SnmpObjectId::getOid (UI32 *oid) const
{
    SI32 i;

    for (i = 0; i < m_Length; i++)
    {
        oid[i] = m_Oid[i];
    }
}

SI32 SnmpObjectId::getLength () const
{
    return (m_Length);
}

bool SnmpObjectId::operator == (const SnmpObjectId &snmpObjectId) const
{
    SI32 i;

    if (m_Length != snmpObjectId.m_Length)
    {
        return (false);
    }

    for (i = 0; i < m_Length; i++)
    {
        if (m_Oid[i] != snmpObjectId.m_Oid[i])
        {
            return (false);
        }
    }

    return (true);
}


bool SnmpObjectId::operator != (const SnmpObjectId &snmpObjectId) const
{
    return (! (operator == (snmpObjectId)));
}

SnmpObjectId &SnmpObjectId::operator = (const SnmpObjectId &snmpObjectId)
{
    copy (snmpObjectId);

    return (*this);
}

}


