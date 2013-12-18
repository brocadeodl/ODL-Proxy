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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.          	   *
 *   All rights reserved.                                                  *
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#ifndef DHCPLOCALWORKER_H
#define DHCPLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Utils/DceSynchronousLinearSeqContext.h"

using namespace WaveNs;

namespace DcmNs
{

class NsmLocalObjectManager;
class DhcpRelIntfLocalConfigMessage;

class DhcpRelayLocalWorker: public WaveWorker
{
	// Class methods
    private:
        PrismMessage *createMessageInstance(const UI32 &operationCode);
        WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        void DhcpRelayIntfLocalConfigMessageHandler(DhcpRelIntfLocalConfigMessage *pDhcpRelIntfLocalConfigMessage);
        ResourceId UpdateDhcpRelIntfLocalConfigStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
        ResourceId UpdateDhcpRelIntfLocalConfigMOStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
		NsmL3ReadyManagedObject* PhyIfNameGetL3ReadyMO(string ifName,PrismSynchronousLinearSequencerContext  *pContext,bool fromLocal);
    protected:
    public:
        DhcpRelayLocalWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
        virtual    ~DhcpRelayLocalWorker ();

	// Class data members
    private:
    protected:
    public:
};
}
#endif                                            //DHCPLOCALWORKER_H
