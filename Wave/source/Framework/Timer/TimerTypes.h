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
 *   Author : Amr Sabaa                                                    *
 ***************************************************************************/

#ifndef TIMERTYPES_H
#define TIMERTYPES_H

namespace WaveNs
{

#define             MAX_USEC                     1000000

typedef enum
{
    TIMER_ADD_TIMER         = 0,
    TIMER_DELETE_TIMER,
    TIMER_DELETE_ALL_TIMERS_FOR_SERVICE
} TimerOpCodes;

}

#endif //TIMERTYPES_H
