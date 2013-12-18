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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef WAVEDEBUGINFORMATIONCONTEXT_H
#define WAVEDEBUGINFORMATIONCONTEXT_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismLinearSequencerContext.h"

namespace WaveNs
{

class WaveDebugInformationContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                            WaveDebugInformationContext             (PrismMessage* pPrismMessage, PrismElement* pPrismElement, PrismLinearSequencerStep* pSteps, UI32 numberOfSteps);
        virtual                            ~WaveDebugInformationContext             ();

                void                        setDebugInformation                     (const string &debugInformation);  
                void                        appendDebugInformation                  (const string &debugInformation);  
                const   string &            getDebugInformation                     () const;  

        // Now the data members

    private :
                        string              m_debugInformation;

    protected :
    public :
};

}
#endif // WAVEDEBUGINFORMATIONCONTEXT_H

