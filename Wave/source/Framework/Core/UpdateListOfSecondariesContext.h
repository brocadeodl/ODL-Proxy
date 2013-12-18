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

#ifndef UPDATELISTOFSECONDARIESCONTEXT_H
#define UPDATELISTOFSECONDARIESCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

#include <string>
#include <vector>

using namespace std;

namespace WaveNs
{

class UpdateListOfSecondariesContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                            UpdateListOfSecondariesContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                            UpdateListOfSecondariesContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual            ~UpdateListOfSecondariesContext ();
                void        addSecondaryNodeDetails        (const LocationId &locationId, const string &ipAddress, const UI32 &port, bool isNodeNew);
                void        setIsThisNodeNew               (bool thisNodeNew);
                UI32        getNumberOfSecondaryLocations  () const;
                LocationId  getLocationIdAtIndex           (const UI32 &index) const;
                string      getIpAddressAtIndex            (const UI32 &index) const;
                UI32        getPortAtIndex                 (const UI32 &index) const;
                bool        isNodeNewAtIndex               (const UI32 &index) const;
                bool        isThisNodeNew                  ()                  const;
                void        setConnectedLocationsAtPrimary (vector<LocationId> &locationIds);
                void        getConnectedLocationsAtPrimary (vector<LocationId> &locationIds);
    // Now the data members

    private :
        vector<LocationId> m_locationIdsForSecondaryNodes;
        vector<string>     m_IpAddressesForSecondaryNodes;
        vector<UI32>       m_portsForSecondaryNodes;
        vector<bool>       m_isNodeNewlyAdded;
        bool               m_isThisNodeNew;
        vector<LocationId> m_connectedLocationsAtPrimary;

    protected :
    public :
};

}

#endif // UPDATELISTOFSECONDARIESCONTEXT_H
