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

#ifndef ZEROIZEFORFIPSLINEARSEQUENCERCONTEXT_H
#define ZEROIZEFORFIPSLINEARSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class ZeroizeForFIPSLinearSequencerContext : public PrismLinearSequencerContext
{
    protected:
    public:
        ZeroizeForFIPSLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        ~ZeroizeForFIPSLinearSequencerContext (); 

        
    // Now the data members

    protected:
    public:
};

}

#endif // ZEROIZEFORFIPSLINEARSEQUENCERCONTEXT_H