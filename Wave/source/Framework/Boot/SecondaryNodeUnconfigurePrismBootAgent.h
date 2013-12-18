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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef SECONDARYNODEUNCONFIGUREPRISMBOOTAGENT_H
#define SECONDARYNODEUNCONFIGUREPRISMBOOTAGENT_H

#include "Framework/Boot/PrismBootAgent.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"

namespace WaveNs
{

class SecondaryNodeUnconfigurePrismBootAgent : public PrismBootAgent
{
    private :
        virtual bool            isAPersistentBoot                           ();
        virtual bool            isToBeExcludedForEnableAndBoot              (const PrismServiceId &prismServiceId);
        virtual bool            isToBeExcludedFromInitializeDuringPrePhase  (const PrismServiceId &prismServiceId);
        virtual bool            isToBeExcludedFromInitializePhase           (const PrismServiceId &prismServiceId);
        virtual bool            willBeAPrimaryLocation                      ();
        virtual WaveBootReason  getReason                                   () const;

    protected :
    public :
                                SecondaryNodeUnconfigurePrismBootAgent      (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual                ~SecondaryNodeUnconfigurePrismBootAgent      ();
        virtual ResourceId      execute                                     (const WaveBootPhase &waveBootPhase);

    //Now the data members

    private :
    protected :
    public :
};

}

#endif // SECONDARYNODEUNCONFIGUREPRISMBOOTAGENT_H
