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

#ifndef DESTROYCLUSTERCONTEXT_H
#define DESTROYCLUSTERCONTEXT_H

#include "Framework/Core/DeleteNodesFromClusterContext.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"

namespace WaveNs
{

class DestroyClusterContext : public DeleteNodesFromClusterContext
{
    private :
    protected :
    public :
                                                      DestroyClusterContext            (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                      DestroyClusterContext            (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                                      ~DestroyClusterContext            ();

                LocationRole                          getThisLocationRoleBeforeDestroy () const;
                void                                  setThisLocationRoleBeforeDestroy (const LocationRole &thisLocationRoleBeforeDestroy);
                FrameworkObjectManagerFailoverReason  getReasonForDestroyingCluster    () const;
                void                                  setReasonForDestroyingCluster    (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);
                void                                  setIsRebootRequired              (const bool &isRebootRequired);
                bool                                  getIsRebootRequired              () const;

    // Now the data members

    private :
        LocationRole                         m_thisLocationRoleBeforeDestroy;
        FrameworkObjectManagerFailoverReason m_reasonForDestroyingCluster;
        bool                                 m_isRebootRequired;

    protected :
    public :
};

}

#endif // DESTROYCLUSTERCONTEXT_H
