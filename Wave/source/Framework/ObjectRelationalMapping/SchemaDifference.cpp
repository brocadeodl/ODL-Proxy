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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mayur Mahajan                                                *
 ***************************************************************************/

#include "Framework/ObjectRelationalMapping/SchemaDifference.h"
#include "Framework/ObjectRelationalMapping/RelationshipInfoFromSchemaDifference.h"
#include "Framework/ObjectModel/PrismElement.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

/**
 * Name:  SchemaDifference
 * Description: Constructor
 * @param
 */
template <class _Tdiff>
SchemaDifference<_Tdiff>::SchemaDifference ()
{
}

/**
 * Name: ~SchemaDifference
 * Description: Destructor
 */
template <class _Tdiff>
SchemaDifference<_Tdiff>::~SchemaDifference ()
{
}

template <class _Tdiff>
bool SchemaDifference<_Tdiff>::hasDifferences () const
{
    return ((m_additionsVec.size () > 0) ||
            (m_deletionsVec.size () > 0) ||
            (m_changesVec.size () > 0));
}

template <class _Tdiff>
const vector<_Tdiff>& SchemaDifference<_Tdiff>::getAdditions () const
{
    return (m_additionsVec);
}

template <class _Tdiff>
const vector<_Tdiff>& SchemaDifference<_Tdiff>::getDeletions () const
{
    return (m_deletionsVec);
}

template <class _Tdiff>
const vector<_Tdiff>& SchemaDifference<_Tdiff>::getChanges () const
{
    return (m_changesVec);	
}

template <class _Tdiff>
void SchemaDifference<_Tdiff>::insertAddition (const _Tdiff &difference)
{
    m_additionsVec.push_back (difference);
}

template <class _Tdiff>
void SchemaDifference<_Tdiff>::insertDeletion (const _Tdiff &difference)
{
    m_deletionsVec.push_back (difference);
}

template <class _Tdiff>
void SchemaDifference<_Tdiff>::insertChange (const _Tdiff &difference)
{
    m_changesVec.push_back (difference);
}

class ManagedObjectSchemaInfo;
template class SchemaDifference<ManagedObjectSchemaInfo *>;
template class SchemaDifference<pair<string, string> >;
template class SchemaDifference<RelationshipInfoFromSchemaDifference>;

}
