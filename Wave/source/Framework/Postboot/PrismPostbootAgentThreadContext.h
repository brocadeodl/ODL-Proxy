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
 *   Author Jitendra Singh                                                   *
 ***************************************************************************/

#ifndef PRISMPOSTBOOTAGENTTHREADCONTEXT_H
#define PRISMPOSTBOOTAGENTTHREADCONTEXT_H

#include "Framework/Postboot/PrismPostPersistentBootMessages.h"

using namespace std;

namespace WaveNs
{
class PrismMutex;
class PrismCondition;
class PrismPostbootAgentThreadContext 
{
    public :
        		                        PrismPostbootAgentThreadContext ();
        virtual                        ~PrismPostbootAgentThreadContext ();
    	void 							setPostbootMessagePointer (PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage);
		PrismPostPersistenceBootMessage *getPostbootMessagePointer ();
                void                    setPostbootMutex                        (PrismMutex *pPostbootMutex);
                PrismMutex             *getPostbootMutex                        () const;
                void                    setPostbootSynchronizingCondition       (PrismCondition *pPostbootSynchronizingCondition);
                PrismCondition         *getPostbootSynchronizingCondition       () const;


    // Now the data members

    private :
		PrismPostPersistenceBootMessage *m_pPrismPostPersistenceBootMessage;
        PrismMutex                      *m_pPostbootMutex;
        PrismCondition                  *m_pPostbootSynchronizingCondition;

};

}

#endif // PRISMPOSTBOOTAGENTTHREADCONTEXT_H
