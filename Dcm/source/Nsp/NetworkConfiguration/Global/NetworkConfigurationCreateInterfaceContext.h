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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef NETWORKCONFIGURATIONCREATEINTERFACECONTEXT_H
#define NETWORKCONFIGURATIONCREATEINTERFACECONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/ObjectModel/ObjectId.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkSegmentManagedObject;

class NetworkConfigurationCreateInterfaceContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                             NetworkConfigurationCreateInterfaceContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                             ~NetworkConfigurationCreateInterfaceContext ();

                ObjectId                     getNetworkObjectId                         () const;
                void                         setNetworkObjectId                         (const ObjectId &networkObjectId);

                ObjectId                     getNetworkSegmentObjectId                  () const;
                void                         setNetworkSegmentObjectId                  (const ObjectId &networkSegmentObjectId);

                NetworkSegmentManagedObject *getPNetworkSegmentManagedObject            ();
                void                         setPNetworkSegmentManagedObject            (NetworkSegmentManagedObject *pNetworkSegmentManagedObject);


    // Now the Data Members

    private :
        ObjectId                     m_networkObjectId;
        ObjectId                     m_networkSegmentObjectId;
        NetworkSegmentManagedObject *m_pNetworkSegmentManagedObject;

    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONCREATEINTERFACECONTEXT_H
