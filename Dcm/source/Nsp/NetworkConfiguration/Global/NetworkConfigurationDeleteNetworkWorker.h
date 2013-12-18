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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef NETWORKCONFIGURATIONDELETENETWORKWORKER_H
#define NETWORKCONFIGURATIONDELETENETWORKWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;
class NetworkConfigurationDeleteNetworkMessage;

class NetworkConfigurationDeleteNetworkWorker : public WaveWorker
{
    private :
                PrismMessage *createMessageInstance                   (const UI32 &operationCode);

                void          deleteNetworkMessageHandler             (NetworkConfigurationDeleteNetworkMessage *pNetworkConfigurationDeleteNetworkMessage);
                void          deleteNetworkValidateStep               (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          deleteNetworkDeleteStep                 (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          deleteNetworkDeactivateInAllNodesStep   (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void          deleteNetworkActivateInAllNodesCallback (WaveSendToClusterContext *pWaveSendToClusterContext);

    protected :
    public :
                 NetworkConfigurationDeleteNetworkWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
        virtual ~NetworkConfigurationDeleteNetworkWorker ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONDELETENETWORKWORKER_H
