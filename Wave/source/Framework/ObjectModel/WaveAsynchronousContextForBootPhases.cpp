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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"

namespace WaveNs
{

WaveAsynchronousContextForBootPhases::WaveAsynchronousContextForBootPhases (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext),
      m_bootReason             (WAVE_BOOT_FIRST_TIME_BOOT),
      m_rollBackFlag           (false)
{
}

WaveAsynchronousContextForBootPhases::~WaveAsynchronousContextForBootPhases ()
{
}

WaveBootReason WaveAsynchronousContextForBootPhases::getBootReason () const
{
    return (m_bootReason);
}

void WaveAsynchronousContextForBootPhases::setBootReason (const WaveBootReason &bootReason)
{
    m_bootReason = bootReason;
}

bool WaveAsynchronousContextForBootPhases::getRollBackFlag () const
{
    return (m_rollBackFlag);
}

void WaveAsynchronousContextForBootPhases::setRollBackFlag (const bool &rollBackFlag)
{
    m_rollBackFlag = rollBackFlag;
}

}
