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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef EARLYCONFIGREPLAYMESSAGINGCONTEXT_H
#define EARLYCONFIGREPLAYMESSAGINGCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

using namespace WaveNs;

namespace DcmNs
{

class EarlyConfigReplayMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
            EarlyConfigReplayMessagingContext                    (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);

           ~EarlyConfigReplayMessagingContext                    ();
		   vector<string>   getConfigVectorPointer               () const;
		   void 		    setConfigVectorPointer               (const vector<string> &config);
           bool             getParsingIncompleteOnFwdlFlag       ();
           void             setParsingIncompleteOnFwdlFlag       (const bool parsingIncompleteOnFwdl);
           string           getConfigFileForParsing              ()const;
           void             setConfigFileForParsing              (const string& configFile);

    // Now the data members

    private :
        vector<string>      m_config;
        bool                m_parsingIncompletOnFwdl;
        string              m_configFileForParsing;

    protected :
    public :
};

}

#endif // EARLYCONFIGREPLAYMESSAGINGCONTEXT_H

