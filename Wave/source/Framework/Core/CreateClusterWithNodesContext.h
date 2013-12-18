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

#ifndef CREATECLUSTERWITHNODESCONTEXT_H
#define CREATECLUSTERWITHNODESCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class CreateClusterWithNodesContext : public PrismLinearSequencerContext
{
    private :
        bool doesLocationExist (const LocationId &locationId);

    protected :
    public :
                                CreateClusterWithNodesContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                CreateClusterWithNodesContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                ~CreateClusterWithNodesContext   ();
        void                    addNewLocationId                (const LocationId &locationId);
        void                    removeNewLocationId             (const LocationId &locationId);
        UI32                    getNumberOfNewLocationIds       ();
        LocationId              getNewLocationIdAt              (const UI32 &i);
        void                    addValidationDetailsForService  (const PrismServiceId &prismServiceId, void *pValidationDetials, const UI32 size);
        vector<PrismServiceId> &getPrismServiceIdsVector        ();
        vector<void *>         &getValidationDetailsVector      ();
        vector<UI32>           &getValidationDetailsSizesVector ();
        void                    addValidaionResultsLocation     (const string &ipAddress, const SI32 &port);
        UI32                    getNumberOfResultsLocations     ();
        void                    getResultsLocationAt            (const UI32 &i, string &ipAddress, SI32 &port);
        void                    addValidationResultsForService  (const string &ipAddress, const SI32 &port, const PrismServiceId &prismServiceId, void *const &pValidationResults, const UI32 &validationResultsSize);
        void                    getValidationResultsForService  (const string &ipAddress, const SI32 &port, const PrismServiceId &prismServiceId, void *&pValidationResults, UI32 &validationResultsSize);
        vector<PrismServiceId> &getPrismServiceIdsToCommunicate ();

        void                    addToSuccessfullyAddedLocationIdVector (LocationId locationId);
        vector<LocationId>      getSuccessfullyAddedLocationIdVector   () const ;
        void                    addToFailedLocationIdVector            (LocationId locationId);
        vector<LocationId>      getFailedLocationIdVector              () const ;
        bool                    isAFailedLocation                      (LocationId locationId);
        void                    clearFailedLocationIdVector            ();
        bool                    getIsDeletion                          ()    const;
        void                    setIsDeletion                          (bool deletion);
        bool                    getIsCreateClusterContext              () const;
        void                    setIsCreateClusterContext              (const bool &isCreateClusterContext);
    // Now the data members

    private :
        vector<LocationId>     m_newLocationIdsVector;
        vector<PrismServiceId> m_prismServiceIdsVector;
        vector<void *>         m_validationDetailsVector;
        vector<UI32>           m_validationDetailsSizesVector;

        vector<PrismServiceId> m_prismServiceIdsToCommunicate;

        vector<string>         m_prismLocationIpAddressesForValidationResultsVector;
        vector<SI32>           m_prismLocationPortsForValidationResultsVector;
        map<string, void *>    m_prismLocationValidationResultsVector;
        map<string, UI32>      m_prismLocationValidationResultsSizesVector;
        bool                   m_isDeletion;
        vector<LocationId>     m_successfullyAddedLocationIdVector;
        vector<LocationId>     m_failedLocationIds;
        bool                   m_isCreateClusterContext;

    protected :
    public :
};

}

#endif // CREATECLUSTERWITHNODESCONTEXT_H
