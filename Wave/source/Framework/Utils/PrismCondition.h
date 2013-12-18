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

#ifndef PRISMCONDITION_H
#define PRISMCONDITION_H

#include <pthread.h>
#include "PrismMutex.h"

namespace WaveNs
{

typedef enum
{
    WAVE_CONDITION_SUCCESS = 0,
    WAVE_CONDITION_ERROR
} PrismConditionStatus;

class PrismCondition
{
    private :
    protected :
    public :
                              PrismCondition (PrismMutex *pAssociatedPrismMutex);
                              PrismCondition (const PrismCondition &prismCondition);
                             ~PrismCondition ();
        PrismCondition       &operator = (const PrismCondition &prismCondition);
        PrismConditionStatus  wait           ();
        PrismConditionStatus  resume         ();
        PrismConditionStatus  resumeAll      ();

    // Now the member variables

    private :
        pthread_cond_t  m_condition;
        PrismMutex     *m_pAssociatedPrismMutex;

    protected :
    public :

};

}

#endif //PRISMCONDITION_H
