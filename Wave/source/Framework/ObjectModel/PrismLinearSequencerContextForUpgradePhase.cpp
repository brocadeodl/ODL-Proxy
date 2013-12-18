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

#include "Framework/ObjectModel/PrismLinearSequencerContextForUpgradePhase.h"

namespace WaveNs
{

PrismLinearSequencerContextForUpgradePhase::PrismLinearSequencerContextForUpgradePhase (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps)
{
    m_schemaChangeInfo = NULL;    
}

PrismLinearSequencerContextForUpgradePhase::~PrismLinearSequencerContextForUpgradePhase ()
{
    if (m_schemaChangeInfo)
    {
        delete (m_schemaChangeInfo);
    }
}

OMSpecificSchemaChangeInfoForUpgrade* PrismLinearSequencerContextForUpgradePhase::getSchemaChangeInfo () const
{
    return (m_schemaChangeInfo);
}  

void PrismLinearSequencerContextForUpgradePhase::setSchemaChangeInfo (OMSpecificSchemaChangeInfoForUpgrade* pSchemaChangeInfo)
{
    m_schemaChangeInfo = pSchemaChangeInfo;
}

/*
vector<string> PrismLinearSequencerContextForUpgradePhase::getNewManagedObjects () const
{
    return (m_newManagedObjects);
}

void PrismLinearSequencerContextForUpgradePhase::setNewManagedObjects (const vector<string> &newManagedObjects)
{
    m_newManagedObjects = newManagedObjects;
}

vector<string> PrismLinearSequencerContextForUpgradePhase::getChangedManagedObjects () const
{
    return (m_changedManagedObjects);
}

void PrismLinearSequencerContextForUpgradePhase::setChangedManagedObjects (const vector<string> &changedManagedObjects)
{
    m_changedManagedObjects = changedManagedObjects;
}
*/
}
