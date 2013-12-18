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
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#ifndef DELETENODEONSECONDARYCONTEXT_H
#define DELETENODEONSECONDARYCONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Utils/PrismAsynchronousContext.h"
#include <vector>
#include <map>

using namespace std;

namespace WaveNs
{

class DeleteNodeOnSecondaryContext : public PrismLinearSequencerContext
{
    private :

    protected :
    public :
                                DeleteNodeOnSecondaryContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                DeleteNodeOnSecondaryContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                ~DeleteNodeOnSecondaryContext   ();

        void                    addKnownLocationId             (const LocationId &locationId);
        vector<LocationId>      getKnownLocationId             ();
        bool                    getIsStartServiceIsRequired    () const;
        void                    setIsStartServiceIsRequired    (const bool &isStartServiceIsRequired);
    // Now the data members

    private :
        vector<LocationId>      m_KnownLocationIdsVector;
        bool                    m_IsStartServiceIsRequired;
    protected :
    public :
};

}

#endif
