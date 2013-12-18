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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayaraman Iyer                                               *
 ***************************************************************************/

#ifndef VRFLOCALSEQUENCERCONTEXT_H
#define VRFLOCALSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Vrf/Local/VrfLocalManagedObject.h"

namespace DcmNs
{

class VrfLocalSynchronousLinearSequencerContext : public PrismSynchronousLinearSequencerContext
{
    private:
    protected:
    public:
                                        VrfLocalSynchronousLinearSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                        ~VrfLocalSynchronousLinearSequencerContext (void);

                void                    setVrfLocalManagedObjectPointer           (VrfLocalManagedObject *pObject);
                VrfLocalManagedObject  *getVrfLocalManagedObjectPointer           (void);

                void                    setReceivedWavePartitionManagedObjectId   (const ObjectId &receivedWavePartitionManagedObjectId); 
                ObjectId                getReceivedWavePartitionManagedObjectId   (void) const ;

    // Now the data members
    private:
             VrfLocalManagedObject   *m_pVrfLocalManagedObject;
             ObjectId                 m_receivedWavePartitionManagedObjectId;

    protected:
    public:
};

}

#endif // VRFLOCALSEQUENCERCONTEXT_H
