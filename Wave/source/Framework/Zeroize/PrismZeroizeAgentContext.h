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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vadiraj C S                                                  *
 ***************************************************************************/

#ifndef PRISMZEROIZEAGENTCONTEXT_H
#define PRISMZEROIZEAGENTCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include <vector>

using namespace std;

namespace WaveNs
{

class PrismZeroizeAgentContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
            PrismZeroizeAgentContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
            virtual                        ~PrismZeroizeAgentContext ();

            vector<PrismServiceId> &getZeroizableServices        ();

    // Now the data members

    private :
        vector<PrismServiceId> m_zeroizableServices;

    protected :
    public :
};

}

#endif // PRISMZEROIZEAGENTCONTEXT_H
