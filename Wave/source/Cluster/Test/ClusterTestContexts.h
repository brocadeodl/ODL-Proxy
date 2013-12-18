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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef CLUSTERTESTCONTEXTS_H
#define CLUSTERTESTCONTEXTS_H

#include "Framework/Utils/PrismLinearSequencerContext.h"


namespace WaveNs
{

class ClusterTestContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
            ClusterTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
           ~ClusterTestContext ();

        UI32  getNumberOfFailures                ();
        void  setNumberOfFailures                (UI32 numberOfFailures);
        void  incrementNumberOfFailures          ();
        void  incrementNumberOfFailures          (UI32 numberOfFailures);

    // Now the data members

    private :
        UI32 m_numberOfFailures;

    protected :
    public :
};

}

#endif // CLUSTERTESTCONTEXTS_H
