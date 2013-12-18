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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKFAILOVERWORKERSEQUENCERCONTEXT_H
#define PRISMFRAMEWORKFAILOVERWORKERSEQUENCERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

namespace WaveNs
{

class PrismFailoverAgent;

class PrismFrameworkFailoverWorkerSequencerContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                                      PrismFrameworkFailoverWorkerSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                                      ~PrismFrameworkFailoverWorkerSequencerContext ();

                LocationRole                          getThisLocationRole                          () const;
                void                                  setThisLocationRole                          (const LocationRole &thisLocationRole);
                FrameworkObjectManagerFailoverReason  getFailoverReason                            () const;
                void                                  setFailoverReason                            (const FrameworkObjectManagerFailoverReason &failoverReason = FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED);
                UI32                                  getNumberOfFailedLocations                   () const;
                LocationId                            getFailedLocationAtIndex                     (const UI32 &index) const;
                void                                  addFailedLocationId                          (const LocationId &locationId);

                PrismFailoverAgent                   *getPPrismFailoverAgent                       () const;
                void                                  setPPrismFailoverAgent                       (PrismFailoverAgent *pPrismFailoverAgent);

                void                                  setServiceToBeIgnored                        (const PrismServiceId &serviceToBeIgnored);
                PrismServiceId                        getServiceToBeIgnored                        () const;

    // Now the data members

    private :
        LocationRole                          m_thisLocationRole;
        FrameworkObjectManagerFailoverReason  m_failoverReason;
        vector<LocationId>                    m_failedLocationIds;

        PrismFailoverAgent                   *m_pPrismFailoverAgent;

        PrismServiceId                        m_serviceToBeIgnored;

    protected :
    public :
};

}

#endif // PRISMFRAMEWORKFAILOVERWORKERSEQUENCERCONTEXT_H
