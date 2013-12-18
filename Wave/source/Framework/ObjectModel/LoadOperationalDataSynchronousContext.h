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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef LOADOPERATIONALDATASYNCHRONOUSCONTEXT_H
#define LOADOPERATIONALDATASYNCHRONOUSCONTEXT_H

#include "Framework/Utils/PrismSynchronousLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/ObjectId.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class LoadOperationalDataSynchronousContext : public PrismSynchronousLinearSequencerContext
{
    private :
    protected :
    public :
                           LoadOperationalDataSynchronousContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                           LoadOperationalDataSynchronousContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps);
                          ~LoadOperationalDataSynchronousContext ();

        ObjectId           getWaveManagedObjectId                () const;
        void               setWaveManagedObjectId                (const ObjectId &waveManagedObjectId);

        vector<string>     getOperationalDataFields              () const;
        void               setOperationalDataFields              (const vector<string> &operationalDataFields);

        WaveManagedObject *getPWaveManagedObject                 () const;
        void               setPWaveManagedObject                 (WaveManagedObject * const pWaveMAnagedObject);

    // Now the data members

    private :
        ObjectId           m_waveManagedObjctId;
        vector<string>     m_operationalDataFields;
        WaveManagedObject *m_pWaveManagedObject;

    protected :
    public :
};

}

#endif // LOADOPERATIONALDATASYNCHRONOUSCONTEXT_H
