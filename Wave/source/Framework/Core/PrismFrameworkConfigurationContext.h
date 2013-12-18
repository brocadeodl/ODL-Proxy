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

#ifndef PRISMFRAMEWORKCONFIGURATIONCONTEXT_H
#define PRISMFRAMEWORKCONFIGURATIONCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Core/PrismFrameworkConfiguration.h"
#include <string>

using namespace std;

namespace WaveNs
{

class PrismFrameworkConfigurationContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                                     PrismFrameworkConfigurationContext     (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const string &prismFrameworkConfigurationFileName);
                                    ~PrismFrameworkConfigurationContext     ();

        string                      &getPrismFrameworkConfigurationFileName ();
        PrismFrameworkConfiguration &getPrismFrameworkConfigurationObject   ();
        bool                         getSyncToStandby                       ();
        void                         setSyncToStandby                       (const bool &syncToStandby);

    // Now the data members

    private :
        string                      m_prismFrameworkConfigurationFileName;
        PrismFrameworkConfiguration m_prismFrameworkConfigurationObject;
        bool                        m_syncToStandby;

    protected :
    public :
};

}

#endif // PRISMFRAMEWORKCONFIGURATIONCONTEXT_H
