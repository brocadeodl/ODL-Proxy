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

#include "PrismMutex.h"
#include "errno.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

PrismMutex::PrismMutex ()
{
    pthread_mutex_init (&m_mutex, NULL);
}

PrismMutex::~PrismMutex ()
{
    pthread_mutex_destroy (&m_mutex);
}

PrismMutexStatus PrismMutex::lock ()
{
    int status = pthread_mutex_lock (&m_mutex);

    if (0 == status)
    {
        return (WAVE_MUTEX_SUCCESS);
    }
    else
    {
        trace (TRACE_LEVEL_FATAL, string ("Could not acquire lock : ") + status);
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MUTEX_COULD_NOT_LOCK);
    }
}

PrismMutexStatus PrismMutex::unlock()
{
    int status = pthread_mutex_unlock (&m_mutex);

    if (0 == status)
    {
        return (WAVE_MUTEX_SUCCESS);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MUTEX_COULD_NOT_UNLOCK);
    }
}

PrismMutexStatus PrismMutex::tryLock ()
{
    int status = pthread_mutex_trylock (&m_mutex);

    if (EBUSY == status)
    {
        return (WAVE_MUTEX_ALREADY_LOCKED);
    }
    else if (0 == status)
    {
        return (WAVE_MUTEX_SUCCESS);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_MUTEX_COULD_NOT_LOCK);
    }
}

}
