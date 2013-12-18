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

/* **
 * IntfUdldConfLocalWorker.h
 *
 * Aug 9, 2012
 * Copyright (C) 2012 Brocade Communications Systems, Inc.
 * All rights reserved.
 */

#ifndef __INTFUDLDCONFWORKER_H__
#define __INTFUDLDCONFWORKER_H__

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class PhyIntfLocalManagedObject;
    class UdldIntfConfigMessage;
    class DceSynchronousLinearSeqContext;
    class DcmManagementInterfaceMessage;


    class IntfUdldConfLocalWorker : public WaveWorker
    {
        public:
            IntfUdldConfLocalWorker (NsmLocalObjectManager *om);
            ~IntfUdldConfLocalWorker ();

        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            ResourceId sendToUdldd (DcmManagementInterfaceMessage *pMessage);

        public:
            void UdldIntfConfigHandler (UdldIntfConfigMessage *pMessage);
            ResourceId validateIntfConfigStep (DceSynchronousLinearSeqContext *pContext);
            ResourceId updateIntfConfigStep (DceSynchronousLinearSeqContext *pContext);
            ResourceId updateIntfMoStep (DceSynchronousLinearSeqContext *pContext);

            void UdldResetAllHandler (UdldIntfConfigMessage *pMessage);
            ResourceId clearAllIntfMoStep (DceSynchronousLinearSeqContext *pContext);

            // Postboot
            //ResourceId postbootIntfUdldConfig ();
            ResourceId postbootIntfUdldConfig (PhyIntfLocalManagedObject *intfMo);
    };

}

#endif
