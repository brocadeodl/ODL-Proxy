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

#include "stdlib.h"
#include <cerrno>
#include "Uuid.h"
#include "Framework/Utils/StringUtils.h"
#include "Mathematics/Z2/Z2.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

Uuid::Uuid()
{
    uuid_generate (m_uuidInBinary);
}

Uuid::Uuid (const string &uuidInStringFormat)
{
	clear();
    fromString (uuidInStringFormat);
}

Uuid::Uuid (const Uuid &uuid)
{
	uuid_copy(m_uuidInBinary, uuid.m_uuidInBinary);
}

Uuid::~Uuid ()
{
    clear();
}

string Uuid::toString () const
{
	char                   uuidInStringFormat[64];

    uuid_unparse (m_uuidInBinary, uuidInStringFormat);
	return ( string(uuidInStringFormat) );
}

bool Uuid::fromString(const string &uuidInStringFormat)
{
	char *                 in;
    int                    success = -1;

    in = new char [uuidInStringFormat.size() + 1];
    WaveNs::prismAssert (NULL != in, __FILE__, __LINE__);
	strcpy(in, uuidInStringFormat.c_str());
	success = uuid_parse(in, m_uuidInBinary);
	delete [] in;

	return (success == 0);
}

bool Uuid::operator == (const Uuid &uuid) const
{
	 int rst = uuid_compare(m_uuidInBinary, uuid.m_uuidInBinary);
	 return (rst == 0);
}

bool Uuid::operator != (const Uuid &uuid) const
{
	int rst = uuid_compare(m_uuidInBinary, uuid.m_uuidInBinary);
	return (rst != 0);
}

Uuid &Uuid::operator = (const Uuid &uuid)
{
	if(this == &uuid)
		return (*this);

	uuid_copy(m_uuidInBinary, uuid.m_uuidInBinary);

    return (*this);
}

void Uuid::getUuid (uuid_t out) const
{
	uuid_copy(out, m_uuidInBinary);
}

string Uuid::getUuidString () const
{
    return toString();
}

UI64   Uuid::getUuidHash () const
{
	MultiByteWord tempPolynomial(64);
    MultiByteWord fingerPrintValue(64);
    Z2 z2;

    errno = 0;
    tempPolynomial[0] = 0xD4F51B49;
    tempPolynomial[1] = 0xA17870F5;
    string uuidInStringFormat = toString();
    z2.setCurrentIrreduciblePolynomial (tempPolynomial);
    z2.computeFingerPrint (fingerPrintValue, uuidInStringFormat);

    string uuidHexString = fingerPrintValue.toHexString();
    UI64 uniqueHash = strtoull (uuidHexString.c_str(), NULL, 16);

    trace (TRACE_LEVEL_DEBUG, string("Uuid::getUuidHash: Uuid ") + uuidInStringFormat);
    trace (TRACE_LEVEL_DEBUG, string("Uuid::getUuidHash: UuidHexString ") + uuidHexString);
    tracePrintf (TRACE_LEVEL_DEBUG, "Uuid::getUuidHash: UuidHash %016llX %llu", uniqueHash, uniqueHash);

    if (errno == EINVAL)
    {
        // not a valid number
    	tracePrintf (TRACE_LEVEL_FATAL, "Uuid::getUuidHash : hash value returned is an invalid number: %016llX", uniqueHash);
    }
    else if (errno == ERANGE)
    {
        tracePrintf (TRACE_LEVEL_FATAL, "Uuid::getUuidHash : hash value returned is greater than required range or strtoull has failed : %016llX", uniqueHash);
        prismAssert (false, __FILE__, __LINE__);
    }

    return uniqueHash;
}

void   Uuid::clear ()
{
	for(int i=0; i < (int)(sizeof (uuid_t)); i++)
       *(((char*)m_uuidInBinary)+i) = 0;
}

}
