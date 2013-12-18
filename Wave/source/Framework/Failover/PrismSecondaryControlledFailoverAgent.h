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

#ifndef PRISMSECONDARYCONTROLLEDFAILOVERAGENT_H
#define PRISMSECONDARYCONTROLLEDFAILOVERAGENT_H

#include "Framework/Failover/PrismFailoverAgent.h"

namespace WaveNs
{

class PrismSecondaryControlledFailoverAgent : public PrismFailoverAgent
{
    private :
        virtual FrameworkObjectManagerFailoverReason getFailoverReason () const;
        virtual WaveBootReason                       getBootReason     () const;

    protected :
    public :
         PrismSecondaryControlledFailoverAgent (WaveObjectManager *pWaveObjectManager, FrameworkSequenceGenerator &currentFrameworkSequenceGenerator);
        ~PrismSecondaryControlledFailoverAgent ();
         void setFailoverDueToPrimaryRemoval   ();
         bool getFailoverDueToPrimaryRemoval   () const;
         virtual void execute (PrismFailoverAgentContext *pPrismFailoverAgentContext);
         virtual bool isToBeExcludedForFailover (const PrismServiceId &prismServiceId);        


    // Now the data members

    private :
        bool m_failoverDueToPrimaryRemoval;
    protected :
    public :
};

}

#endif // PRISMSECONDARYCONTROLLEDFAILOVERAGENT_H
