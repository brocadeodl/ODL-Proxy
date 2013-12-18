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

#ifndef DCELINEARSEQCONTEXT_H
#define DCELINEARSEQCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Utils/DceContext.h"

using namespace WaveNs;

namespace DcmNs
{

class DceLinearSeqContext : public PrismLinearSequencerContext, public DceContext
{
    private :
    protected :
    public :
    DceLinearSeqContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
    DceLinearSeqContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
    ~DceLinearSeqContext ();

    WaveManagedObject *getPWaveManagedObject() const;
    void setPWaveManagedObject(WaveManagedObject * const pWaveMAnagedObject);

    void addManagedObjectsForGarbageCollection (const vector<WaveManagedObject *> &mos);
    void addManagedObjectForGarbageCollection  (WaveManagedObject *mo);

    // Now the data members

    private :
        WaveManagedObject *m_pWaveManagedObject;

    protected :
    public :
};

}

#endif // DCELINEARSEQCONTEXT_H
