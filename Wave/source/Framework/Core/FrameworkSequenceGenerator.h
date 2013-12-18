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

#ifndef FRAMEWORKSEQUENCEGENERATOR_H
#define FRAMEWORKSEQUENCEGENERATOR_H

#include <vector>
#include "Framework/Types/Types.h"

using namespace std;

namespace WaveNs
{

class FrameworkSequenceGenerator
{
    private :
    protected :
        FrameworkSequenceGenerator ();

    public :
        virtual      ~FrameworkSequenceGenerator          ();

        virtual void  getInitializeSequenceDuringPrePhase (vector<PrismServiceId> &serviceIds) const;
        virtual void  getEnableSequenceDuringPrePhase     (vector<PrismServiceId> &serviceIds) const;
        virtual void  getInstallSequenceDuringPrePhase    (vector<PrismServiceId> &serviceIds) const;
        virtual void  getBootSequenceDuringPrePhase       (vector<PrismServiceId> &serviceIds) const;

        virtual void  getInitializeSequence               (vector<PrismServiceId> &serviceIds) const;
        virtual void  getEnableSequence                   (vector<PrismServiceId> &serviceIds) const;
        virtual void  getInstallSequence                  (vector<PrismServiceId> &serviceIds) const;
        virtual void  getBootSequence                     (vector<PrismServiceId> &serviceIds) const;
        virtual void  getShutdownSequence                 (vector<PrismServiceId> &serviceIds) const;
        virtual void  getUninstallSequence                (vector<PrismServiceId> &serviceIds) const;
        virtual void  getUninitializeSequence             (vector<PrismServiceId> &serviceIds) const;
        virtual void  getDisableSequence                  (vector<PrismServiceId> &serviceIds) const;

        virtual void  getShutdownSequenceDuringPostPhase     (vector<PrismServiceId> &serviceIds) const;
        virtual void  getUninstallSequenceDuringPostPhase    (vector<PrismServiceId> &serviceIds) const;
        virtual void  getUninitializeSequenceDuringPostPhase (vector<PrismServiceId> &serviceIds) const;
        virtual void  getDisableSequenceDuringPostPhase      (vector<PrismServiceId> &serviceIds) const;

        virtual void  getFailoverSequence                 (vector<PrismServiceId> &serviceIds) const;
        virtual void  getZeroizeSequence                 (vector<PrismServiceId> &serviceIds) const;

        virtual void  addPrismServiceIdToAll              (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToInitialize       (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToEnable           (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToInstall          (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToBoot             (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToShutdown         (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToUninstall        (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToUninitialize     (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToDisable          (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToFailover         (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);
        virtual void  addPrismServiceIdToZeroize 		  (const PrismServiceId &prismServiceId, const bool &isForNormalPhase = true);

    // Nowe the data members

    private :
    protected :
        vector<PrismServiceId> m_prismServiceIdsToInitializeDuringPrePhase;
        vector<PrismServiceId> m_prismServiceIdsToEnableDuringPrePhase;
        vector<PrismServiceId> m_prismServiceIdsToInstallDuringPrePhase;
        vector<PrismServiceId> m_prismServiceIdsToBootDuringPrePhase;

        vector<PrismServiceId> m_prismServiceIdsToInitialize;
        vector<PrismServiceId> m_prismServiceIdsToEnable;
        vector<PrismServiceId> m_prismServiceIdsToInstall;
        vector<PrismServiceId> m_prismServiceIdsToBoot;
        vector<PrismServiceId> m_prismServiceIdsToShutdown;
        vector<PrismServiceId> m_prismServiceIdsToUninstall;
        vector<PrismServiceId> m_prismServiceIdsToUninitialize;
        vector<PrismServiceId> m_prismServiceIdsToDisable;

        vector<PrismServiceId> m_prismServiceIdsToShutdownDuringPostPhase;
        vector<PrismServiceId> m_prismServiceIdsToUninstallDuringPostPhase;
        vector<PrismServiceId> m_prismServiceIdsToUninitializeDuringPostPhase;
        vector<PrismServiceId> m_prismServiceIdsToDisableDuringPostPhase;

        vector<PrismServiceId> m_prismServiceIdsToFailover;
        vector<PrismServiceId> m_prismServiceIdsToZeroize;

    public :
};

}

#endif // FRAMEWORKSEQUENCEGENERATOR_H
