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

#ifndef NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALWORKER_H
#define NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace NspNs
{

class NetworkConfigurationLocalObjectManager;
class NetworkConfigurationAttachVirtualInterfaceLocalMessage;

class NetworkConfigurationAttachVirtualInterfaceLocalWorker : public WaveWorker
{
    private :
        PrismMessage *createMessageInstance                             (const UI32 &operationCode);
        void          attachVirtualInterfaceLocalMessageHandler         (NetworkConfigurationAttachVirtualInterfaceLocalMessage *pNetworkConfigurationAttachVirtualInterfaceLocalMessage);
        void          attachVirtualInterfaceLocalValidateStep           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
        void          attachVirtualInterfaceLocalActivateInHardWareStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);

    protected :
    public :
                 NetworkConfigurationAttachVirtualInterfaceLocalWorker (NetworkConfigurationLocalObjectManager *pNetworkConfigurationLocalObjectManager);
        virtual ~NetworkConfigurationAttachVirtualInterfaceLocalWorker ();

    // Now the Data Members

    private :
    protected :
    public :
};

}

#endif // NETWORKCONFIGURATIONATTACHVIRTUALINTERFACELOCALWORKER_H
