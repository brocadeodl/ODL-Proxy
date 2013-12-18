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

#include "Framework/Core/PrismFrameworkInitializeWorkerStartServicesContext.h"

namespace WaveNs
{

PrismFrameworkInitializeWorkerStartServicesContext::PrismFrameworkInitializeWorkerStartServicesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_prismBootMode                (WAVE_BOOT_FIRST_TIME),
      m_waveBootPhase                (WAVE_BOOT_PHASE_ALL_PHASES),
      m_pPrismBootAgent              (NULL),
      m_isSchemaChangedOnFwdl        (false),
      m_isSchemaUpgradedSuccessfully (false)
{
}

PrismFrameworkInitializeWorkerStartServicesContext::~PrismFrameworkInitializeWorkerStartServicesContext ()
{
    m_pPrismBootAgent = NULL;
}

PrismBootAgent *PrismFrameworkInitializeWorkerStartServicesContext::getPPrismBootAgent ()
{
    return (m_pPrismBootAgent);
}

void PrismFrameworkInitializeWorkerStartServicesContext::setPPrismBootAgent (PrismBootAgent *pPrismBootAgent)
{
    m_pPrismBootAgent = pPrismBootAgent;
}

PrismBootMode PrismFrameworkInitializeWorkerStartServicesContext::getPrismBootMode () const
{
    return (m_prismBootMode);
}

void PrismFrameworkInitializeWorkerStartServicesContext::setPrismBootMode (PrismBootMode prismBootMode)
{
    m_prismBootMode = prismBootMode;
}

WaveBootPhase PrismFrameworkInitializeWorkerStartServicesContext::getWaveBootPhase () const
{
    return (m_waveBootPhase);
}

void PrismFrameworkInitializeWorkerStartServicesContext::setWaveBootPhase (const WaveBootPhase &waveBootPhase)
{
    m_waveBootPhase = waveBootPhase;
}

bool PrismFrameworkInitializeWorkerStartServicesContext::isSchemaChangedOnFwdl () const
{
    return (m_isSchemaChangedOnFwdl);
}

void PrismFrameworkInitializeWorkerStartServicesContext::setSchemaChangedOnFwdl (bool hasChanged)
{
    m_isSchemaChangedOnFwdl = hasChanged;
}

bool PrismFrameworkInitializeWorkerStartServicesContext::getIsSchemaUpgradedSuccessfully () const
{
    return (m_isSchemaUpgradedSuccessfully);
}

void PrismFrameworkInitializeWorkerStartServicesContext::setIsSchemaUpgradedSuccessfully (const bool &isSchemaUpgradedSuccessfully)
{
    m_isSchemaUpgradedSuccessfully = isSchemaUpgradedSuccessfully;
}

}
