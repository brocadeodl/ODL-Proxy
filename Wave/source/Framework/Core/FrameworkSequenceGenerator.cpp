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
 *   Copyright (C) 2005-2006 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Core/FrameworkSequenceGenerator.h"

namespace WaveNs
{

FrameworkSequenceGenerator::FrameworkSequenceGenerator ()
{
}

FrameworkSequenceGenerator::~FrameworkSequenceGenerator ()
{
}

void FrameworkSequenceGenerator::getInitializeSequenceDuringPrePhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToInitializeDuringPrePhase;
}

void FrameworkSequenceGenerator::getEnableSequenceDuringPrePhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToEnableDuringPrePhase;
}

void FrameworkSequenceGenerator::getInstallSequenceDuringPrePhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToInstallDuringPrePhase;
}

void FrameworkSequenceGenerator::getBootSequenceDuringPrePhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToBootDuringPrePhase;
}

void FrameworkSequenceGenerator::getShutdownSequenceDuringPostPhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToShutdownDuringPostPhase;
}

void FrameworkSequenceGenerator::getUninstallSequenceDuringPostPhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToUninstallDuringPostPhase;
}

void FrameworkSequenceGenerator::getUninitializeSequenceDuringPostPhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToUninitializeDuringPostPhase;
}

void FrameworkSequenceGenerator::getDisableSequenceDuringPostPhase (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToDisableDuringPostPhase;
}

void FrameworkSequenceGenerator::getInitializeSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToInitialize;
}

void FrameworkSequenceGenerator::getEnableSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToEnable;
}

void FrameworkSequenceGenerator::getInstallSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToInstall;
}

void FrameworkSequenceGenerator::getBootSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToBoot;
}

void FrameworkSequenceGenerator::getShutdownSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToShutdown;
}

void FrameworkSequenceGenerator::getUninstallSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToUninstall;
}

void FrameworkSequenceGenerator::getUninitializeSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToUninitialize;
}

void FrameworkSequenceGenerator::getDisableSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToDisable;
}

void FrameworkSequenceGenerator::getFailoverSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToFailover;
}

void FrameworkSequenceGenerator::getZeroizeSequence (vector<PrismServiceId> &serviceIds) const
{
    serviceIds = m_prismServiceIdsToZeroize;
}

void FrameworkSequenceGenerator::addPrismServiceIdToAll (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    if (true == isForNormalPhase)
    {
        m_prismServiceIdsToInitialize.push_back (prismServiceId);
        m_prismServiceIdsToEnable.push_back (prismServiceId);
        m_prismServiceIdsToInstall.push_back (prismServiceId);
        m_prismServiceIdsToBoot.push_back (prismServiceId);
        m_prismServiceIdsToShutdown.insert (m_prismServiceIdsToShutdown.begin (), prismServiceId);
        m_prismServiceIdsToUninstall.insert (m_prismServiceIdsToUninstall.begin (), prismServiceId);
        m_prismServiceIdsToUninitialize.insert (m_prismServiceIdsToUninitialize.begin (), prismServiceId);
        m_prismServiceIdsToDisable.insert (m_prismServiceIdsToDisable.begin (), prismServiceId);
    }
    else
    {
        m_prismServiceIdsToInitializeDuringPrePhase.push_back (prismServiceId);
        m_prismServiceIdsToEnableDuringPrePhase.push_back (prismServiceId);
        m_prismServiceIdsToInstallDuringPrePhase.push_back (prismServiceId);
        m_prismServiceIdsToBootDuringPrePhase.push_back (prismServiceId);
        m_prismServiceIdsToShutdownDuringPostPhase.insert (m_prismServiceIdsToShutdownDuringPostPhase.begin (), prismServiceId);
        m_prismServiceIdsToUninstallDuringPostPhase.insert (m_prismServiceIdsToUninstallDuringPostPhase.begin (), prismServiceId);
        m_prismServiceIdsToUninitializeDuringPostPhase.insert (m_prismServiceIdsToUninitializeDuringPostPhase.begin (), prismServiceId);
        m_prismServiceIdsToDisableDuringPostPhase.insert (m_prismServiceIdsToDisableDuringPostPhase.begin (), prismServiceId);
    }

    m_prismServiceIdsToFailover.push_back   (prismServiceId);
    m_prismServiceIdsToZeroize.push_back    (prismServiceId);
    
}

void FrameworkSequenceGenerator::addPrismServiceIdToInitialize (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToInitialize.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToEnable (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToEnable.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToInstall (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToInstall.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToBoot (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToBoot.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToShutdown (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToShutdown.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToUninstall (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToUninstall.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToUninitialize (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToUninitialize.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToDisable (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToDisable.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToFailover (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToFailover.push_back (prismServiceId);
}

void FrameworkSequenceGenerator::addPrismServiceIdToZeroize (const PrismServiceId &prismServiceId, const bool &isForNormalPhase)
{
    m_prismServiceIdsToZeroize.push_back(prismServiceId);
}

}
