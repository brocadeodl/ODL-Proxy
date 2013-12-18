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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForPostbootPhase.h"

namespace WaveNs
{

WaveAsynchronousContextForPostbootPhase::WaveAsynchronousContextForPostbootPhase (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : WaveAsynchronousContextForBootPhases (pCaller, pCallback, pCallerContext),
      m_passNum                (0)
{
}

WaveAsynchronousContextForPostbootPhase::~WaveAsynchronousContextForPostbootPhase ()
{
}

SI8 WaveAsynchronousContextForPostbootPhase::getPassNum () const
{
    return (m_passNum);
}

void WaveAsynchronousContextForPostbootPhase::setPassNum (SI8 passNum)
{
    m_passNum = passNum;
}

string WaveAsynchronousContextForPostbootPhase::getPassName () const
{
    return (m_passName);
}

void WaveAsynchronousContextForPostbootPhase::setPassName (string passName)
{
    m_passName = passName;
}

UI32 WaveAsynchronousContextForPostbootPhase::getSlotNum () const
{
    return (m_slotNum);
}

void WaveAsynchronousContextForPostbootPhase::setSlotNum (UI32 slotNum)
{
    m_slotNum = slotNum;
}

UI32 WaveAsynchronousContextForPostbootPhase::getRecoveryType () const
{
    return (m_recoveryType);
}

void WaveAsynchronousContextForPostbootPhase::setRecoveryType (UI32 recoveryType)
{
    m_recoveryType = recoveryType;
}

}
