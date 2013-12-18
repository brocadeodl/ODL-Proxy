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

#ifndef WAVEASYNCHRONOUSCONTEXTFORPOSTBOOTPHASE_H
#define WAVEASYNCHRONOUSCONTEXTFORPOSTBOOTPHASE_H

#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WavePostBootEvent.h"

namespace WaveNs
{

class WaveAsynchronousContextForPostbootPhase : public WaveAsynchronousContextForBootPhases
{
    private :
    protected :
    public :
                                WaveAsynchronousContextForPostbootPhase (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                ~WaveAsynchronousContextForPostbootPhase ();

                SI8             getPassNum                           () const;
                void            setPassNum                           (SI8 passNum);
                string          getPassName                          () const;
                void            setPassName                          (string passName);
                UI32            getSlotNum                           () const;
                void            setSlotNum                           (UI32 slotNum);
                UI32            getRecoveryType                      () const;
                void            setRecoveryType                      (UI32 recoveryType);
    // Now the data members

    private :
                SI8             m_passNum; // indicates postboot pass#
                string          m_passName;
                UI32            m_slotNum;
                UI32            m_recoveryType;
    protected :
    public :
};

}

#endif // WAVEASYNCHRONOUSCONTEXTFORPOSTBOOTPHASE_H
