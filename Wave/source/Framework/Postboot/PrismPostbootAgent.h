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
 *   Author : Amit Agrawal                                                 *
 ***************************************************************************/

#ifndef PRISMPOSTBOOTAGENT_H
#define PRISMPOSTBOOTAGENT_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Postboot/PrismPostbootTypes.h"

namespace WaveNs
{

class PrismPostbootAgentContext;

class PrismPostbootAgent : protected WaveWorker
{
    private :
    protected :
    public :
                            PrismPostbootAgent            (WaveObjectManager *pWaveObjectManager);
                            PrismPostbootAgent            (WaveObjectManager *pWaveObjectManager, const UI32 &event, const UI32 &parameter, const UI32 &recoveryType);
        virtual            ~PrismPostbootAgent            ();

        virtual ResourceId  execute                       ();
                ResourceId  getListOfEnabledServicesStep  (PrismPostbootAgentContext *pPrismPostbootAgentContext);
                ResourceId  mergeStaticRegistrationsStep  (PrismPostbootAgentContext *pPrismPostbootAgentContext);
                ResourceId  notifyPostbootStartedStep       (PrismPostbootAgentContext *pPrismPostbootAgentContext);
                ResourceId  sendPostbootPassStep            (PrismPostbootAgentContext *pPrismPostbootAgentContext);
                ResourceId  notifyPostbootCompletedStep     (PrismPostbootAgentContext *pPrismPostbootAgentContext);
                bool        requiresPostboot              (const PrismServiceId &prismServiceId);

                void        printRegistrationTable        (void);
        static  void        populateRegistrations         (map <UI32, vector <postbootPass> > registrationsTable);

    // Now the data members

    private :
        // Global table for postboot registrations. The table resizes based on registrations
                map <UI32, vector <postbootPass> >        m_postbootTable;
                UI32        m_eventId;
                UI32        m_parameter;
                UI32        m_recoveryType;
    protected :
    public :
};

}

#endif // PRISMPOSTBOOTAGENT_H
