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

#ifndef NTPCLOCKGLOBALOBJECTMANAGER_H
#define NTPCLOCKGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class NTPClockGlobalAddServerMessage;
    class NTPClockGlobalChangeServerMessage;
    class NTPClockGlobalRemoveServerMessage;
    class NTPClockGlobalGetServerMessage;
    class NTPClockGlobalSetClockMessage;
    class NTPClockGlobalGetClockMessage;
    class NTPClockGlobalSetZoneMessage;
	class NTPServerManagedObject;
    class NTPClockGlobalAddKeyMessage;
    class NTPClockGlobalRemoveKeyMessage;
    class NTPKeyManagedObject;
	// class TimeZoneManagedObject;

    class NTPClockGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NTPClockGlobalAddServerMessageHandler( NTPClockGlobalAddServerMessage *pNTPClockGlobalAddServerMessage);
            void addServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalChangeServerMessageHandler( NTPClockGlobalChangeServerMessage *pNTPClockGlobalChangeServerMessage);
            void changeServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalGetClockMessageHandler( NTPClockGlobalGetClockMessage *pNTPClockGlobalGetClockMessage);
			void getClockSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalGetServerMessageHandler( NTPClockGlobalGetServerMessage *pNTPClockGlobalGetServerMessage);
			void getServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalRemoveServerMessageHandler( NTPClockGlobalRemoveServerMessage *pNTPClockGlobalRemoveServerMessage);
            void removeServerSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalSetClockMessageHandler( NTPClockGlobalSetClockMessage *pNTPClockGlobalSetClockMessage);
            void setClockSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalSetZoneMessageHandler( NTPClockGlobalSetZoneMessage *pNTPClockGlobalSetZoneMessage);
            void setZoneSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
            void NTPClockGlobalAddKeyMessageHandler( NTPClockGlobalAddKeyMessage *pNTPClockGlobalAddKeyMessage);
            void NTPClockGlobalRemoveKeyMessageHandler( NTPClockGlobalRemoveKeyMessage *pNTPClockGlobalRemoveKeyMessage);	
            ResourceId traceSTCErrors(WaveSendToClusterContext *pWaveSTCContext, const string &callerName, string &formattedIds);

        protected:
        public:
            NTPClockGlobalObjectManager ();
            virtual    ~NTPClockGlobalObjectManager ();
            static NTPClockGlobalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //NTPCLOCKGLOBALOBJECTMANAGER_H
