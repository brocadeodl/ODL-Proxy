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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skrastog					                                   *
 ***************************************************************************/

#ifndef PORTSECURITYCONTEXT_H
#define PORTSECURITYCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"

using namespace std;

namespace WaveNs
{

class PortSecurityContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                PortSecurityContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                PortSecurityContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
        		virtual ~PortSecurityContext ();

				void addBuffer (void *data);
				void *getBuffer() const;

				void setBufferSize(const UI32 &size);
				UI32 getBufferSize() const;

    // Now the data members

    private :
				void *m_buffer;
				UI32 m_bufSize;

    protected :
    public :
};

}

#endif // PORTSECURITYCONTEXT_H

