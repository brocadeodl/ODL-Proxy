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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef WAVEASYNCHRONOUSCONTEXTFOREXTERNALSTATESYNCHRONIZATION_H
#define WAVEASYNCHRONOUSCONTEXTFOREXTERNALSTATESYNCHRONIZATION_H

#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class WaveAsynchronousContextForExternalStateSynchronization: public PrismAsynchronousContext
{
    private :
    protected :
    public :
                                WaveAsynchronousContextForExternalStateSynchronization (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                ~WaveAsynchronousContextForExternalStateSynchronization ();

                UI32            getFssStageNumber                                      () const;
                void            setFssStageNumber                                      (const UI32 fssStageNumber);
                
                ResourceId      getServiceType                                         () const;
                void            setServiceType                                         (const ResourceId serviceType);

    // Now the data members

    private :
        UI32        m_fssStageNumber;
        ResourceId  m_serviceType;

    protected :
    public :
};

}

#endif // WAVEASYNCHRONOUSCONTEXTFOREXTERNALSTATESYNCHRONIZATION_H
