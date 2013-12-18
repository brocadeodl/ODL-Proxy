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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#ifndef PRISMLINEARSEQUENCERCONTEXTFORUPGRADEPHASE_H
#define PRISMLINEARSEQUENCERCONTEXTFORUPGRADEPHASE_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/ObjectModel/ObjectId.h"
#include "Framework/ObjectRelationalMapping/OMSpecificSchemaChangeInfoForUpgrade.h"

#include <vector>

using namespace std;

namespace WaveNs
{

class PrismLinearSequencerContextForUpgradePhase : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                           PrismLinearSequencerContextForUpgradePhase (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual           ~PrismLinearSequencerContextForUpgradePhase ();

/*
        vector<string>     getNewManagedObjects         () const;
        void               setNewManagedObjects         (const vector<string> &newManagedObjects);

        vector<string>     getChangedManagedObjects     () const;
        void               setChangedManagedObjects     (const vector<string> &changedManagedObjects);
*/
        OMSpecificSchemaChangeInfoForUpgrade* getSchemaChangeInfo () const;  
        void                                  setSchemaChangeInfo (OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo);   
    // Now the data members

    private :
/*
        vector<string>     m_newManagedObjects;
        vector<string>     m_changedManagedObjects;
*/
        OMSpecificSchemaChangeInfoForUpgrade* m_schemaChangeInfo;

    protected :
    public :
};

}

#endif // PRISMLINEARSEQUENCERCONTEXTFORUPGRADEPHASE_H 
