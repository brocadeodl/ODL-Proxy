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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : cgangwar                                                     *
 ***************************************************************************/

#ifndef PRISMLINEARSEQUENCERCONTEXTFORSHUTDOWNPHASE_H
#define PRISMLINEARSEQUENCERCONTEXTFORSHUTDOWNPHASE_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include <map>

namespace WaveNs
{
class UnifiedClientBackendDetails;

class PrismLinearSequencerContextForShutdownPhase : public PrismLinearSequencerContext
{
    private:
    protected:
    public:
                      PrismLinearSequencerContextForShutdownPhase(PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual      ~PrismLinearSequencerContextForShutdownPhase();

                void  setBackendMap(const map<string,UnifiedClientBackendDetails*>  &backendMap);  
                map<string,UnifiedClientBackendDetails*>  getBackendMap() const;

                //data members
    private:
        map<string,UnifiedClientBackendDetails*> m_backendMap;
    protected:
    public:        
};

}

#endif
