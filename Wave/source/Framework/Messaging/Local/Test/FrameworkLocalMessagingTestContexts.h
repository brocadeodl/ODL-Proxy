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

#ifndef FRAMEWORKLOCALMESSAGINGTESTCONTEXTS_H
#define FRAMEWORKLOCALMESSAGINGTESTCONTEXTS_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

namespace WaveNs
{

class FrameworkLocalMessagingTestContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                    FrameworkLocalMessagingTestContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                   ~FrameworkLocalMessagingTestContext ();

        LocationId  getRemoteLocationId                ();
        void        setRemoteLocationId                (const LocationId &remoteLocationId);

        void        setNumberOfMessagesToSend          (const UI32 &numberOfMessagesToSend);
        UI32        getNumberOfMessagesToSend          ();

        void        incrementNumberOfMessagesSend      ();
        void        resetNumberOfMessagesSend          ();
        UI32        getNumberOfMessagesSend            ();

        bool        areAllMessagesSent                 ();
    // Now the data members

    private :
        LocationId m_remoteLocationId;
        UI32       m_numberOfMessagesToSend;
        UI32       m_numberOfMessagesSend;

    protected :
    public :
};

}

#endif // FRAMEWORKLOCALMESSAGINGTESTCONTEXTS_H
