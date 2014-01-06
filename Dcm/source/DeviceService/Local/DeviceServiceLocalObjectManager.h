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
 *   Author : dchung                                                     *
 **************************************************************************/

#ifndef DEVICESERVICELOCALOBJECTMANAGER_H
#define DEVICESERVICELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class DeviceServiceGetAllMessage;

    class DeviceServiceLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void DeviceServiceGetAllMessageHandler( DeviceServiceGetAllMessage *pDeviceServiceGetAllMessage);

	        ResourceId getAllEntries (PrismSynchronousLinearSequencerContext *pDeviceServiceGetAllMessageContext);

        protected:
        public:
            DeviceServiceLocalObjectManager ();
            virtual    ~DeviceServiceLocalObjectManager ();
            static DeviceServiceLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //DEVICESERVICELOCALOBJECTMANAGER_H