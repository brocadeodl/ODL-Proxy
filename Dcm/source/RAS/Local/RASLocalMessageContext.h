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

#ifndef RASLOCALMESSAGINGCONTEXT_h
#define RASLOCALMESSAGINGCONTEXT_h

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "RAS/Local/LoggingLocalManagedObject.h"

namespace DcmNs
{

class RASLocalMessagingContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
    RASLocalMessagingContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
    ~RASLocalMessagingContext ();
    LoggingLocalManagedObject *getPLoggingLocalManagedObject () const;
    void setLoggingLocalManagedObject (LoggingLocalManagedObject *loggingLocalManagedObject);
    // Now the data members

    private :
    LoggingLocalManagedObject   *m_pLoggingLocalManagedObject;

    protected :
    public :
};

}

#endif // RASLOCALMESSAGINGCONTEXT_h