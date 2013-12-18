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
 *   Author : Balachandar Mani                                             *
 ***************************************************************************/

#ifndef DCESYNCHRONOUSLINEARSEQCONTEXT_H
#define DCESYNCHRONOUSLINEARSEQCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Utils/DceContext.h"

using namespace WaveNs;

namespace DcmNs
{

class DceSynchronousLinearSeqContext : public PrismSynchronousLinearSequencerContext, public DceContext
{
    private :
    protected :
    public :
						   DceSynchronousLinearSeqContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
						   DceSynchronousLinearSeqContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                          ~DceSynchronousLinearSeqContext ();

        WaveManagedObject *getPWaveManagedObject                 () const;
        void               setPWaveManagedObject                 (WaveManagedObject * const pWaveMAnagedObject);

        void addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &mos);
        void addManagedObjectForGarbageCollection  (WaveManagedObject *mo);

    // Now the data members

    private :
        WaveManagedObject *m_pWaveManagedObject;

    protected :
    public :
};

}

#endif // DCESYNCHRONOUSLINEARSEQCONTEXT_H
