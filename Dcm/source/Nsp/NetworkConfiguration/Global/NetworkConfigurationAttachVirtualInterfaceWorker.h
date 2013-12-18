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

#ifndef NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEWORKER_H
#define NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationObjectManager;
class NetworkConfigurationAttachVirtualInterfaceMessage;
class NetworkConfigurationAttachVirtualInterfaceContext;

class NetworkConfigurationAttachVirtualInterfaceWorker : public WaveWorker
{
    private :
        PrismMessage *createMessageInstance                (const UI32 &operationCode);

        void          attachVirtualInterfaceMessageHandler             (NetworkConfigurationAttachVirtualInterfaceMessage *pNetworkConfigurationAttachVirtualInterfaceMessage);
        void          attachVirtualInterfaceValidateStep               (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext);
        void          attachVirtualInterfaceAttachStep                 (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext);
        void          attachVirtualInterfaceActivateInAllNodesStep     (NetworkConfigurationAttachVirtualInterfaceContext *pNetworkConfigurationAttachVirtualInterfaceContext);
        void          attachVirtualInterfaceActivateInAllNodesCallback (WaveSendToClusterContext *pWaveSendToClusterContext);


    protected :
    public :
                 NetworkConfigurationAttachVirtualInterfaceWorker (NetworkConfigurationObjectManager *pNetworkConfigurationObjectManager);
        virtual ~NetworkConfigurationAttachVirtualInterfaceWorker ();

    // Now the Data Members

    private :
    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONATTACHVIRTUALINTERFACEWORKER_H
