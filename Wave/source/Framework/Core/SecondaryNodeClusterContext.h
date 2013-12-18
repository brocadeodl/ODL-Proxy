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
 *   Author : Anand Kumar Subramanian                                      *
 ***************************************************************************/

#ifndef SECONDARYNODECLUSTERCONTEXT_H
#define SECONDARYNODECLUSTERCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"

namespace WaveNs
{

class SecondaryNodeClusterContext: public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                    SecondaryNodeClusterContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool setSecondaryNodeClusterCreationFlag = true, bool clusterHaSyncInProgressFlag = false);
                    SecondaryNodeClusterContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps, bool setSecondaryNodeClusterCreationFlag = true, bool clusterHaSyncInProgressFlag = false);
        virtual    ~SecondaryNodeClusterContext ();

        bool        getClusterHaSyncInProgress  () const;
        void        setClusterHaSyncInProgress  (bool clusterHaSyncInProgressFlag);

        void        setDBEmptyRequired          (const bool &isDBEmptyRequired);
        bool        getIsDBEmptyRequired        () const;

        string      getClusterPrimaryIpAddress  ();
        SI32        getClusterPrimaryPort       ();
        LocationId  getClusterPrimaryLocationId ();

        void        setClusterPrimaryIpAddress  (const string &ipAddress);
        void        setClusterPrimaryPort       (const SI32 &port);
        void        setClusterPrimaryLocationId (const LocationId &locationId);

    // Now the data members

    private :
        bool        m_setSecondaryNodeClusterCreationFlag;
        bool        m_clusterHaSyncInProgressFlag;
        bool        m_isDBEmptyRequired;
        string      m_clusterPrimaryIpAddress;
        SI32        m_clusterPrimaryPort;
        LocationId  m_clusterPrimaryLocationId;

    protected :
    public :
};

}

#endif // SECONDARYNODECLUSTERCONTEXT_H 


