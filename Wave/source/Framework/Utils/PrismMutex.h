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

#ifndef PRISMMUTEX_H
#define PRISMMUTEX_H

#include "pthread.h"

namespace WaveNs
{

class PrismCondition;

typedef enum
{
    WAVE_MUTEX_SUCCESS = 0,
    WAVE_MUTEX_COULD_NOT_LOCK,
    WAVE_MUTEX_COULD_NOT_UNLOCK,
    WAVE_MUTEX_ALREADY_LOCKED
} PrismMutexStatus;

class PrismMutex
{
    private :
    protected :
    public :
                       PrismMutex ();
                       ~PrismMutex ();
        PrismMutexStatus lock     ();
        PrismMutexStatus unlock   ();
        PrismMutexStatus tryLock  ();

    // Now the data members

    private :
        pthread_mutex_t m_mutex;

    protected :
    public :

    friend class PrismCondition;
};

}

#endif //PRISMMUTEX_H
