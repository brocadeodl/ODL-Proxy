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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

ObjectId ObjectId::NullObjectId;

ObjectId::ObjectId (UI32 classId, UI64 instanceId)
    : m_classId (classId),
      m_instanceId (instanceId)
{
}

ObjectId::ObjectId (const ObjectId &rhs)
    : m_classId (rhs.m_classId),
      m_instanceId (rhs.m_instanceId)
{
}

ObjectId::ObjectId ()
    : m_classId (0),
      m_instanceId (0)
{
}

ObjectId &ObjectId::operator = (const ObjectId &rhs)
{
    m_classId    = rhs.m_classId;
    m_instanceId = rhs.m_instanceId;

    return (*this);
}

bool ObjectId::operator == (const ObjectId &rhs) const
{
    if ((m_classId == rhs.m_classId) && (m_instanceId == rhs.m_instanceId))
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

bool ObjectId::operator != (const ObjectId &rhs) const
{
    return (!(operator == (rhs)));
}

bool ObjectId::operator < (const ObjectId &rhs) const
{
    if (rhs.m_classId > m_classId)
    {
        return (true);
    }
    else if (rhs.m_classId < m_classId)
    {
        return (false);
    }
    else
    {
        if (rhs.m_instanceId > m_instanceId)
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
}

bool ObjectId::operator > (const ObjectId &rhs) const
{
    if (rhs.m_classId < m_classId)
    {
        return (true);
    }
    else if (rhs.m_classId > m_classId)
    {
        return (false);
    }
    else
    {
        if (rhs.m_instanceId < m_instanceId)
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
}

UI32 ObjectId::getClassId () const
{
    return (m_classId);
}

void ObjectId::setClassId (const UI32 &classId)
{
    m_classId = classId;
}

void ObjectId::setInstanceId (const UI64 &instanceId)
{
    m_instanceId = instanceId;
}

UI64 ObjectId::getInstanceId () const
{
    return (m_instanceId);
}

string ObjectId::toString () const
{
    return (string ("") + m_classId + string (":") + m_instanceId);
}

}
