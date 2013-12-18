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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#ifndef PRISMPOSTBOOTAGENTTHREAD_H
#define PRISMPOSTBOOTAGENTTHREAD_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Postboot/PrismPostbootAgentThreadContext.h"
#include "Framework/MultiThreading/PrismPosixThread.h"

namespace WaveNs
{

class PrismPostbootAgentContext;
class PrismPostbootAgentThread : protected WaveWorker, public PrismPosixThread
{
    private :
    protected :
    public :
							PrismPostbootAgentThread		  (WaveObjectManager *pWaveObjectManager, const UI32 &event, const UI32 &parameter, const ResourceId &recoveryType, PrismPostbootAgentThreadContext *pPrismPostbootAgentThreadContext);
        virtual            ~PrismPostbootAgentThread            ();

        WaveThreadStatus    start               ();

    // Now the data members

    private :
                UI32        m_eventId;
                UI32        m_parameter;
                ResourceId  m_recoveryType;
				PrismPostbootAgentThreadContext *m_pPrismPostbootAgentThreadContext;
        static  UI32        m_parallelPostBootAgentThreads;
        static  PrismMutex  m_parallelPostBootAgentThreadsMutex;
    protected :
    public :
};

}

#endif // PRISMPOSTBOOTAGENTTHREAD_H
