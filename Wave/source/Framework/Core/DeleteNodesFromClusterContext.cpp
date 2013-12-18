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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/DeleteNodesFromClusterContext.h"

namespace WaveNs
{

DeleteNodesFromClusterContext::DeleteNodesFromClusterContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : CreateClusterWithNodesContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_isConfigurationChange                (false)
{
}
DeleteNodesFromClusterContext::DeleteNodesFromClusterContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : CreateClusterWithNodesContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_isConfigurationChange                (false)
{
}

DeleteNodesFromClusterContext::~DeleteNodesFromClusterContext ()
{
}

void DeleteNodesFromClusterContext::addLocationId (const LocationId &locationId)
{
    addNewLocationId (locationId);
}

void DeleteNodesFromClusterContext::removeLocationId (const LocationId &locationId)
{
    removeNewLocationId (locationId);
}

UI32 DeleteNodesFromClusterContext::getNumberOfLocationIds ()
{
    return (getNumberOfNewLocationIds ());
}

LocationId DeleteNodesFromClusterContext::getLocationIdAt (const UI32 &i)
{
    return (getNewLocationIdAt (i));
}

bool DeleteNodesFromClusterContext::getIsConfigurationChange () const
{
    return m_isConfigurationChange;
}

void DeleteNodesFromClusterContext::setIsConfigurationChange (const bool isConfigurationChange)
{
    m_isConfigurationChange = isConfigurationChange;
}

}
