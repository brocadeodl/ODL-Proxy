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
 *   Author : Ritesh Madapurath                                            *
 **************************************************************************/

#ifndef EVENTENGINELOCALOBJECTMANAGER_H
#define EVENTENGINELOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "discover/Framework/EventEngineMessages.h"

using namespace WaveNs;

namespace DcmNs {

    /**
     * Primary responsibility is to act as a passthru for the messages being published
     * from Events Global plugin towards the B.E
     *
     */

    class EventEngineLocalObjectManager: public WaveLocalObjectManager {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(
                    const string &managedClassName);
        protected:
        public:
            EventEngineLocalObjectManager();
            virtual ~EventEngineLocalObjectManager();
            static EventEngineLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
            void eventEnginePublishHandler(
                    EventEngineLocalMessage *eventEngineLocalMessage);

// Now the data members

        private:

        protected:
        public:
    };
}
#endif  // EVENTENGINELOCALOBJECTMANAGER_H
