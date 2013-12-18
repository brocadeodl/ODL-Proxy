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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Aashish Akhouri                                    *
 ***************************************************************************/

#ifndef SECONDARYNODEREJOINPRISMBOOTAGENT_H
#define SECONDARYNODEREJOINPRISMBOOTAGENT_H

#include "Framework/Boot/PrismBootAgent.h"

namespace WaveNs
{
///Class:
///SecondaryNodeRejoinPrismBootAgent
/// 
///Description:
/// This Class serves as the boot agent for rebooting of services
/// during processing of phase1 and phase2 rejoin messages on the 
/// secondary.

class SecondaryNodeRejoinPrismBootAgent : public PrismBootAgent
{
    private :
	//Rejoin Boot is always persistent
        virtual bool           isAPersistentBoot                       ();
	//QUESTION: SAGAR
        virtual bool           isToBeExcludedForEnableAndBoot          (const PrismServiceId& prismServiceId);

        virtual bool           willBeAPrimaryLocation                  ();

        virtual bool           isToBeExcludedFromInitializeDuringPrePhase   (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromInitializePhase            (const PrismServiceId &prismServiceId);
        virtual bool           isToBeExcludedFromCurrentBootPhase           (PrismServiceId prismServiceId);
	//Reason for boot
        virtual WaveBootReason getReason                               () const;

    protected :
    public :
                            SecondaryNodeRejoinPrismBootAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        virtual            ~SecondaryNodeRejoinPrismBootAgent ();

	//This method implements the pre and the post boot phase
	//It calls the steps alreday implemented in the base class
        virtual ResourceId  execute                              (const WaveBootPhase &waveBootPhase);

    private :
                            WaveBootPhase m_waveBootPhase;
    protected :
    public :
};

}

#endif // SECONDARYNODEREJOINPRISMBOOTAGENT_H
