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

#ifndef DELETENODESFROMCLUSTERCONTEXT_H
#define DELETENODESFROMCLUSTERCONTEXT_H

#include "Framework/Core/CreateClusterWithNodesContext.h"

namespace WaveNs
{

class DeleteNodesFromClusterContext : public CreateClusterWithNodesContext
{
    private :
    protected :
    public :
                            DeleteNodesFromClusterContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                            DeleteNodesFromClusterContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual            ~DeleteNodesFromClusterContext ();
                void        addLocationId                 (const LocationId &locationId);
                void        removeLocationId              (const LocationId &locationId);
                UI32        getNumberOfLocationIds        ();
                LocationId  getLocationIdAt               (const UI32 &i);
                bool        getIsConfigurationChange      () const;
                void        setIsConfigurationChange      (const bool isConfigurationChange);

    // Now the data members

    private :
                bool        m_isConfigurationChange;
    protected :
    public :
};

}

#endif // DELETENODESFROMCLUSTERCONTEXT_H
