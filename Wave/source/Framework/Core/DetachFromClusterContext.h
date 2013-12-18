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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef DETACHFROMCLUSTERCONTEXT_H
#define DETACHFROMCLUSTERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Core/FrameworkObjectManagerTypes.h"

namespace WaveNs
{

class DetachFromClusterContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                      DetachFromClusterContext         (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                                      DetachFromClusterContext         (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                                      ~DetachFromClusterContext         ();

                LocationRole                          getThisLocationRoleBeforeDetach  () const;
                void                                  setThisLocationRoleBeforeDetach  (const LocationRole &thisLocationRoleBeforeDetach);
                FrameworkObjectManagerFailoverReason  getReasonForDetachingFromCluster () const;
                void                                  setReasonForDetachingFromCluster (const FrameworkObjectManagerFailoverReason &reasonForDetachingFromCluster = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);

    // Now the data members

    private :
        LocationRole                         m_thisLocationRoleBeforeDetach;
        FrameworkObjectManagerFailoverReason m_reasonForDetachingFromCluster;

    protected :
    public :
};

}

#endif // DETACHFROMCLUSTERCONTEXT_H
