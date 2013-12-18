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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef PERSISTENCELOCALEXECUTETRANSACTIONCONTEXT_H
#define PERSISTENCELOCALEXECUTETRANSACTIONCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include "Framework/Types/Types.h"

using namespace std;

namespace WaveNs
{

class PersistenceLocalExecuteTransactionContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                             PersistenceLocalExecuteTransactionContext    (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                             PersistenceLocalExecuteTransactionContext    (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);

        virtual                             ~PersistenceLocalExecuteTransactionContext    ();

                UI32                         getCurrentTransactionId                      () const;
                void                         setCurrentTransactionId                      (const UI32 &currentTransactionId);
                LocationId                   getSenderLocationId                          () const;
                void                         setSenderLocationId                          (const LocationId &locationId);
                bool                         getIsCommit                                  () const;
                void                         setIsCommit                                  (const bool &isCommit);
                bool                         getIsRollback                                () const;
                void                         setIsRollback                                (const bool &isRollback);
                bool                         getIsPrepare                                 () const;
                void                         setIsPrepare                                 (const bool &isPrepare);
                bool                         getIsConfigurationChange                     () const;
                void                         setIsConfigurationChange                     (const bool &isConfigurationChange);
                bool                         getIsConfigurationTimeChange                 () const;
                void                         setIsConfigurationTimeChange                 (const bool &isConfigurationTimeChange);
                ResourceId                   getActiveResult                              () const;
                void                         setActiveResult                              (const ResourceId &result);
                ResourceId                   getStandbyResult                             () const;
                void                         setStandbyResult                             (const ResourceId &result);

    // now the data members

    private :
        UI32                        m_currentTransactionId;
        LocationId                  m_senderLocationId;
        bool                        m_isCommit;
        bool                        m_isRollback;
        bool                        m_isPrepare;
        bool                        m_isConfigurationChange;
        bool                        m_isConfigurationTimeChange;
        ResourceId                  m_activeResult;
        ResourceId                  m_standbyResult;
    protected :
    public :
};

}

#endif // PERSISTENCELOCALEXECUTETRANSACTIONCONTEXT_H
