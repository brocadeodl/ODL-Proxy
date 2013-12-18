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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef DISTRIBUTEDLOGTESTCONTEXT_H
#define DISTRIBUTEDLOGTESTCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/ObjectId.h"

namespace WaveNs
{

class DistributedLogTestContext : public PrismLinearSequencerContext
{
    private:
    protected:
    public:
                                DistributedLogTestContext       (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                ~DistributedLogTestContext       (); 

                UI64            getOriginalMaxAllowedLogEntries () const;
                void            setOriginalMaxAllowedLogEntries (const UI64 &originalMaxAllowedLogEntries);

                UI64            getCurrentMaxAllowedLogEntries  () const;
                void            setCurrentMaxAllowedLogEntries  (const UI64 &currentMaxAllowedLogEntries);

                UI64            getFirstLogId                   () const;
                void            setFirstLogId                   (const UI64 &firstLogId);

                UI64            getNextLogId                    () const;
                void            setNextLogId                    (const UI64 &nextLogId);

                PrismServiceId  getPrismServiceId               () const;
                void            setPrismServiceId               (const PrismServiceId &prismServiceId);

                ObjectId        getManagedObjectId              () const;
                void            setManagedObjectId              (const ObjectId &managedObjectId);

                UI32            getNumberOfFailures             () const;
                void            setNumberOfFailures             (UI32 numberOfFailures);
                void            incrementNumberOfFailures       ();
        
    // Now the data members
    private:
                UI64            m_originalMaxAllowedLogEntries;
                UI64            m_currentMaxAllowedLogEntries;
                UI64            m_firstLogId;
                UI64            m_nextLogId;
                PrismServiceId  m_prismServiceId;
                ObjectId        m_managedObjectId;
                UI32            m_numberOfFailures;

    protected:
    public:
};

}

#endif // DISTRIBUTEDLOGTESTCONTEXT_H
