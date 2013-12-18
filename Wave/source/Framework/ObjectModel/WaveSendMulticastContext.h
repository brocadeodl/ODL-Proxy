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
 *   Copyright (C) 2008-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef WAVESENDMULTICASTCONTEXT_H
#define WAVESENDMULTICASTCONTEXT_H

#include "Framework/Utils/PrismAsynchronousContext.h"

#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class PrismMessage;

class WaveSendMulticastContext : public PrismAsynchronousContext
{
    private:
    protected:
    public:
                                            WaveSendMulticastContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext = NULL);
        virtual                            ~WaveSendMulticastContext ();
                void                        setPrismMessage          (PrismMessage *pPrismMessage);
                PrismMessage*               getPrismMessage          ();
                vector<LocationId>          getAllLocationsToSent    () const;
                void                        setAllLocationsToSent    (vector<LocationId> &locationIds);
                ResourceId                  getStatusForALocation    (LocationId &loctionId);
                void                        setStatusForALocation    (LocationId &locationId, ResourceId &locationStatus);
       
                ResourceId                  getOverallMulticastStatus ();
                void                        setMulticastStatus        (ResourceId &status);
    // Data Members
    private:
        PrismMessage                   *m_pPrismMessage;
        vector<LocationId>              m_connectedLocationIds;
        map<LocationId, ResourceId>     m_locationStatus;
        ResourceId                      m_multicastStatus;
 
    protected:
    public:
};

}

#endif // WAVESENDMULTICASTCONTEXT_H

