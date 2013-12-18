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

#ifndef NETWORKCONFIGURATIONATTACHVIRTUALINTERFACECONTEXT_H
#define NETWORKCONFIGURATIONATTACHVIRTUALINTERFACECONTEXT_H

#include "Framework/Utils/PrismLinearSequencerContext.h"

using namespace WaveNs;

namespace NspNs
{

class VnicManagedObject;

class NetworkConfigurationAttachVirtualInterfaceContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                   NetworkConfigurationAttachVirtualInterfaceContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
        virtual                   ~NetworkConfigurationAttachVirtualInterfaceContext ();

                VnicManagedObject *getPVnicManagedObject                             ();
                void               setPVnicManagedObject                             (VnicManagedObject *pVnicManagedObject);

                string             getNetworkName                                    () const;
                void               setNetworkName                                    (const string &networkName);

                UI32               getNetworkId                                      () const;
                void               setNetworkId                                      (const UI32 &networkId);

    // Now the Data Members

    private :
        VnicManagedObject *m_pVnicManagedObject;
        string             m_networkName;
        UI32               m_networkId;

    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONATTACHVIRTUALINTERFACECONTEXT_H
