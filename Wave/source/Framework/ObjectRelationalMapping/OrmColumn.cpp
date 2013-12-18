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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectRelationalMapping/OrmColumn.h"

namespace WaveNs
{

OrmColumn::OrmColumn (const string &name, const string &sqlType, const string &typeForUpgradeMO, const bool &isPrimary)
    :   OrmEntity           (name),
        m_sqlType           (sqlType),
        m_typeForUpgradeMO  (typeForUpgradeMO),
        m_isPrimary         (isPrimary)
{
}

OrmColumn::~OrmColumn ()
{
}

string OrmColumn::getSqlType () const
{
    return (m_sqlType);
}

string OrmColumn::getTypeForUpgradeMO () const
{
    return (m_typeForUpgradeMO);
}

string OrmColumn::getSqlForCreate (const string &schema) const
{
    return ("    " + getName () + " " + m_sqlType + (m_isPrimary ? " PRIMARY KEY" : ""));
}

bool OrmColumn::getIsPrimary () const
{
    return (m_isPrimary);
}

void OrmColumn::setIsPrimary (const bool &isPrimary)
{
    m_isPrimary = isPrimary;
}

}

