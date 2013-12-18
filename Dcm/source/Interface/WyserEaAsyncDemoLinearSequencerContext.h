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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef WYSEREAASYNCDEMOLINEARSEQUENCERCONTEXT_H
#define WYSEREAASYNCDEMOLINEARSEQUENCERCONTEXT_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{

class WyserEaAsyncDemoLinearSequencerContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                    WyserEaAsyncDemoLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                   ~WyserEaAsyncDemoLinearSequencerContext ();

    // Now the data members

    public :
        UI32                        m_numberOfExtraRepliesRemaining;
        UI32                        m_delayBetweenReplies;

    protected :
    public :
};

}

#endif // WYSEREAASYNCDEMOLINEARSEQUENCERCONTEXT_H
