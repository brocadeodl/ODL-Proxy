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
 *   Author : Mobin Mohan                                                  *
 **************************************************************************/

#ifndef APPMVNMLOCALWORKER_H
#define APPMVNMLOCALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class APPMLocalObjectManager;
    class APPMVnmLocalCdpNotifyMessage;

    class APPMVnmLocalWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void APPMVnmLocalCdpNotifyMessageHandler(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage);
            ResourceId handleCdpNotification(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId putPoIntfToPortProfilePort(string poIfName);
            ResourceId putPhyIntfToPortProfilePort(string ifName, InterfaceType type);
            ResourceId putIntfToPortProfilePort(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage);
            ResourceId removePoIntfFromPortProfilePort(string poIfName);
            ResourceId removePhyIntfFromPortProfilePort(string ifName, InterfaceType type);
            ResourceId removeIntfFromPortProfilePort(APPMVnmLocalCdpNotifyMessage *pAPPMVnmLocalCdpNotifyMessage);
            ResourceId isIntfReceiveCdp(string ifName, bool &isMemberHasCdp);
        protected:
        public:
            APPMVnmLocalWorker (APPMLocalObjectManager *pAPPMLocalObjectManager);
            virtual    ~APPMVnmLocalWorker ();

// Now the data members
        private:
        protected:
        public:
    };
}
#endif //APPMVNMLOCALWORKER_H
