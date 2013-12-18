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
 ***************************************************************************/

#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Messaging/Local/PrismMessage.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/ObjectModel/WaveManagedObject.h"

namespace WaveNs
{

PrismAsynchronousContext::PrismAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : m_pCaller (pCaller),
      m_pCallback (pCallback),
      m_pCallerContext (pCallerContext)
{
    m_completionStatus = WAVE_MESSAGE_ERROR;
}

PrismAsynchronousContext::PrismAsynchronousContext (const PrismAsynchronousContext &prismAsynchronousContext)
    : m_pCaller (NULL),
      m_pCallback (NULL),
      m_pCallerContext (NULL)
{
    trace (TRACE_LEVEL_FATAL, "PrismAsynchronousContext::PrismAsynchronousContext : Copy constructing PrismAsynchronousContext does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);
}

PrismAsynchronousContext::~PrismAsynchronousContext ()
{
    garbageCollect ();
}

PrismAsynchronousContext &PrismAsynchronousContext::operator = (const PrismAsynchronousContext &prismAsynchronousContext)
{
    trace (TRACE_LEVEL_FATAL, "PrismAsynchronousContext::operator = : Assigning to a PrismAsynchronousContext does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);

    return (*this);
}

void PrismAsynchronousContext::setCompletionStatus (const ResourceId &status)
{
    m_completionStatus = status;
}

ResourceId PrismAsynchronousContext::getCompletionStatus ()
{
    return (m_completionStatus);
}

void *PrismAsynchronousContext::getPCallerContext ()
{
    return (m_pCallerContext);
}

void PrismAsynchronousContext::callback ()
{
    if ((NULL != m_pCaller) && (((PrismAsynchronousCallback) NULL) != m_pCallback))
    {
        (m_pCaller->*m_pCallback) (this);
    }
}

void PrismAsynchronousContext::addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &managedObjectsForGarbageCollection)
{
    m_managedObjectsForGarbageCollection.insert (m_managedObjectsForGarbageCollection.end (), managedObjectsForGarbageCollection.begin (), managedObjectsForGarbageCollection.end ());
}

void PrismAsynchronousContext::addManagedObjectForGarbageCollection (WaveManagedObject *pWaveManagedObjectForGarbageCollection)
{
    prismAssert (NULL != pWaveManagedObjectForGarbageCollection, __FILE__, __LINE__);

    m_managedObjectsForGarbageCollection.push_back (pWaveManagedObjectForGarbageCollection);
}

void PrismAsynchronousContext::garbageCollect ()
{
    UI32 numberOfManagedObjectsForGarbageCollection = m_managedObjectsForGarbageCollection.size ();
    UI32 i                                          = 0;

    for (i = 0; i < numberOfManagedObjectsForGarbageCollection; i++)
    {
        prismAssert (NULL != m_managedObjectsForGarbageCollection[i], __FILE__, __LINE__);

        delete (m_managedObjectsForGarbageCollection[i]);

        m_managedObjectsForGarbageCollection[i] = NULL;
    }

    m_managedObjectsForGarbageCollection.clear ();
}

}
