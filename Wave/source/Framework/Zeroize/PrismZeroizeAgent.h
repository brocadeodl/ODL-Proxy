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
 *   Author : Vadiraj C S
 ***************************************************************************/

#ifndef PRISMZEROIZEAGENT_H
#define PRISMZEROIZEAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Core/FrameworkSequenceGenerator.h"
#include "Framework/Zeroize/PrismZeroizeAgentContext.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

class PrismZeroizeAgentContext;

class PrismZeroizeAgent : protected WaveWorker
{
    private :
        bool                isZeroizeAtBoot             ();
        bool                needsZeroize                ( const PrismServiceId &prismServiceId );
    protected :
    public :
                            PrismZeroizeAgent           (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
                            PrismZeroizeAgent           (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator, bool zeroizeAtBoot);
        virtual            ~PrismZeroizeAgent           ();
        virtual ResourceId  execute                     ();
        ResourceId          shutdownServicesStep        (PrismZeroizeAgentContext *pContext);
        ResourceId          executeZeroizeStep          (PrismZeroizeAgentContext *pContext);
        ResourceId          completeFIPSZeroizeStep     (PrismZeroizeAgentContext *pContext);
        ResourceId          prepareFIPSZeroizeStep      (PrismZeroizeAgentContext *pContext);


    // Now the data members

    private :
        FrameworkSequenceGenerator  &m_currentFrameworkSequenceGenerator;
        WaveObjectManager           *m_pWaveObjectManager;
        bool                        m_zeroizeAtBoot;

    protected :
    public :
};

}

#endif // PRISMZEROIZEAGENT_H
