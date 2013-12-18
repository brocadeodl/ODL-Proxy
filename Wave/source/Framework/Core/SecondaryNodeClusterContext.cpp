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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#include "Framework/Core/SecondaryNodeClusterContext.h"
#include "Framework/Core/PrismFrameworkObjectManager.h"
#include "Framework/Trace/TraceMessages.h"


namespace WaveNs
{

SecondaryNodeClusterContext::SecondaryNodeClusterContext(PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool setSecondaryNodeClusterCreationFlag, bool clusterHaSyncInProgressFlag)
    : PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_isDBEmptyRequired (true),
      m_clusterPrimaryPort (0),
      m_clusterPrimaryLocationId (0)
{
    m_clusterPrimaryIpAddress.clear ();
    m_setSecondaryNodeClusterCreationFlag = setSecondaryNodeClusterCreationFlag;
    
    if (m_setSecondaryNodeClusterCreationFlag)
    {
        (PrismFrameworkObjectManager::getInstance ())->setSecondaryNodeClusterCreationFlag (true);
    }

    m_clusterHaSyncInProgressFlag = clusterHaSyncInProgressFlag;
}

SecondaryNodeClusterContext::SecondaryNodeClusterContext(PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool setSecondaryNodeClusterCreationFlag, bool clusterHaSyncInProgressFlag)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_isDBEmptyRequired (true),
      m_clusterPrimaryPort (0),
      m_clusterPrimaryLocationId (0)
{
    m_clusterPrimaryIpAddress.clear ();
    m_setSecondaryNodeClusterCreationFlag = setSecondaryNodeClusterCreationFlag;
    
    if (m_setSecondaryNodeClusterCreationFlag)
    {
        (PrismFrameworkObjectManager::getInstance ())->setSecondaryNodeClusterCreationFlag (true);
    }

    m_clusterHaSyncInProgressFlag = clusterHaSyncInProgressFlag;
}

SecondaryNodeClusterContext::~SecondaryNodeClusterContext()
{
}

bool SecondaryNodeClusterContext::getClusterHaSyncInProgress () const
{
    return m_clusterHaSyncInProgressFlag;
}

void SecondaryNodeClusterContext::setClusterHaSyncInProgress (bool clusterHaSyncInProgressFlag)
{
    m_clusterHaSyncInProgressFlag = clusterHaSyncInProgressFlag;
}

void SecondaryNodeClusterContext::setDBEmptyRequired (const bool &isDBEmptyRequired)
{
    m_isDBEmptyRequired = isDBEmptyRequired;
}

bool SecondaryNodeClusterContext::getIsDBEmptyRequired () const
{
    return (m_isDBEmptyRequired);
}

string SecondaryNodeClusterContext::getClusterPrimaryIpAddress ()
{
    return (m_clusterPrimaryIpAddress);
}

void SecondaryNodeClusterContext::setClusterPrimaryIpAddress  (const string &ipAddress)
{
    m_clusterPrimaryIpAddress = ipAddress;
}

SI32 SecondaryNodeClusterContext::getClusterPrimaryPort ()
{
    return (m_clusterPrimaryPort);
}

void SecondaryNodeClusterContext::setClusterPrimaryPort (const SI32 &port)
{
    m_clusterPrimaryPort = port;
}

LocationId SecondaryNodeClusterContext::getClusterPrimaryLocationId ()
{
    return (m_clusterPrimaryLocationId);
}

void SecondaryNodeClusterContext::setClusterPrimaryLocationId (const LocationId &locationId)
{
    m_clusterPrimaryLocationId = locationId;
}

}

