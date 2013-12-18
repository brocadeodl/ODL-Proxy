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
 *   Author : Jayraman Iyer                                                *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForMultiPartitionCleanup.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveAsynchronousContextForMultiPartitionCleanup::WaveAsynchronousContextForMultiPartitionCleanup (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext  (pCaller, pCallback, pCallerContext),
      m_isPartialCleanup        (false),
      m_partialCleanupTag       (FRAMEWORK_ERROR)
{
}

WaveAsynchronousContextForMultiPartitionCleanup::~WaveAsynchronousContextForMultiPartitionCleanup ()
{
}

string WaveAsynchronousContextForMultiPartitionCleanup::getPartitionName () const
{
    return (m_partitionName);
}

void WaveAsynchronousContextForMultiPartitionCleanup::setPartitionName (const string &partitionName)
{
    m_partitionName = partitionName;
}

ObjectId WaveAsynchronousContextForMultiPartitionCleanup::getOwnerPartitionManagedObjectId() const
{
    return (m_ownerPartitionManagedObjectId);
}

void WaveAsynchronousContextForMultiPartitionCleanup::setOwnerPartitionManagedObjectId (const ObjectId &objId)
{
    m_ownerPartitionManagedObjectId = objId;
}

bool WaveAsynchronousContextForMultiPartitionCleanup::getIsPartialCleanup () const
{
    return m_isPartialCleanup;
}

void WaveAsynchronousContextForMultiPartitionCleanup::setPartialCleanupTag (const ResourceId &partialCleanupTag)
{
    m_isPartialCleanup = true;

    m_partialCleanupTag = partialCleanupTag;
}

ResourceId WaveAsynchronousContextForMultiPartitionCleanup::getPartialCleanupTag ()
{
    prismAssert (true == m_isPartialCleanup, __FILE__, __LINE__);

    return m_partialCleanupTag;
}

}

