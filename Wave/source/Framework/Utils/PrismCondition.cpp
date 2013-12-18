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

#include "PrismCondition.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

PrismCondition::PrismCondition (PrismMutex *pAssociatedPrismMutex)
{
    prismAssert (NULL != pAssociatedPrismMutex, __FILE__, __LINE__);

    pthread_cond_init (&m_condition, NULL);
    m_pAssociatedPrismMutex = pAssociatedPrismMutex;
}

PrismCondition::PrismCondition (const PrismCondition &prismCondition)
{
    trace (TRACE_LEVEL_FATAL, "PrismCondition::PrismCondition : Copy constructing PrismCondition does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);
}

PrismCondition::~PrismCondition ()
{
    pthread_cond_destroy (&m_condition);
}

PrismCondition &PrismCondition::operator = (const PrismCondition &prismCondition)
{
    trace (TRACE_LEVEL_FATAL, "PrismCondition::operator = : Assigning to a PrismCondition does not make sense and hence not allowed.");
    prismAssert (false, __FILE__, __LINE__);

    return (*this);
}

PrismConditionStatus PrismCondition::wait ()
{
    int status = pthread_cond_wait (&m_condition, &(m_pAssociatedPrismMutex->m_mutex));

    if (0 == status)
    {
        return (WAVE_CONDITION_SUCCESS);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_CONDITION_ERROR);
    }
}

PrismConditionStatus PrismCondition::resume ()
{
    int status = pthread_cond_signal (&m_condition);

    if (0 == status)
    {
        return (WAVE_CONDITION_SUCCESS);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_CONDITION_ERROR);
    }
}

PrismConditionStatus PrismCondition::resumeAll ()
{
    int status = pthread_cond_broadcast (&m_condition);

    if (0 == status)
    {
        return (WAVE_CONDITION_SUCCESS);
    }
    else
    {
        prismAssert (false, __FILE__, __LINE__);
        return (WAVE_CONDITION_ERROR);
    }
}

}
