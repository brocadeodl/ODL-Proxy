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
 *   Author : noku                                                     *
 **************************************************************************/

#ifndef NTPCLOCKLOCALOBJECTMANAGER_H
#define NTPCLOCKLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockSetSwitchClockMessage;
    class NTPClockSetSwitchZoneMessage;
    class NTPClockGetSwitchClockMessage;
    class NTPClockGetActiveServerMessage;
    class NTPClockSetServersMessage;
	class NTPClockSetServerKeyMessage;

    class NTPClockLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            ResourceId translateTsStatusCode(const int &tsStatus);
            void NTPClockGetActiveServerMessageHandler( NTPClockGetActiveServerMessage *pNTPClockGetActiveServerMessage);
            void NTPClockGetSwitchClockMessageHandler( NTPClockGetSwitchClockMessage *pNTPClockGetSwitchClockMessage);
            void NTPClockSetServerKeyMessageHandler( NTPClockSetServerKeyMessage *pNTPClockSetServerKeyMessage);
            void NTPClockSetServersMessageHandler( NTPClockSetServersMessage *pNTPClockSetServersMessage);
            void NTPClockSetSwitchClockMessageHandler( NTPClockSetSwitchClockMessage *pNTPClockSetSwitchClockMessage);
            void NTPClockSetSwitchZoneMessageHandler( NTPClockSetSwitchZoneMessage *pNTPClockSetSwitchZoneMessage);
        protected:
        public:
            NTPClockLocalObjectManager ();
            virtual    ~NTPClockLocalObjectManager ();
            static NTPClockLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			ResourceId pushBackend();
            void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization *pWaveAsynchronousContextForExternalStateSynchronization);

// Now the data members

        private:

        protected:
        public:
    };
}
#endif                                            //NTPCLOCKLOCALOBJECTMANAGER_H
