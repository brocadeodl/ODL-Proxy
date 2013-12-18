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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/PrismFrameworkConfigurationContext.h"

namespace WaveNs
{

PrismFrameworkConfigurationContext::PrismFrameworkConfigurationContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps, const string &prismFrameworkConfigurationFileName)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_prismFrameworkConfigurationFileName (prismFrameworkConfigurationFileName),
      m_syncToStandby (false)
{
}

PrismFrameworkConfigurationContext::~PrismFrameworkConfigurationContext ()
{
}

string &PrismFrameworkConfigurationContext::getPrismFrameworkConfigurationFileName ()
{
    return (m_prismFrameworkConfigurationFileName);
}

PrismFrameworkConfiguration &PrismFrameworkConfigurationContext::getPrismFrameworkConfigurationObject ()
{
    return (m_prismFrameworkConfigurationObject);
}

bool PrismFrameworkConfigurationContext::getSyncToStandby ()
{
    return (m_syncToStandby);
}

void PrismFrameworkConfigurationContext::setSyncToStandby (const bool &syncToStandby)
{
    m_syncToStandby = syncToStandby;
}

}