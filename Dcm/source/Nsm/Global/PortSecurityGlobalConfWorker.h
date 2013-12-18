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
 *   Author : skrastog                                                     *
 **************************************************************************/

#ifndef PORTSECURITYGLOBALCONFWORKER_H
#define PORTSECURITYGLOBALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Nsm/Local/PortSecurityLocalManagedObject.h"
#include "Nsm/Local/PortSecurityContext.h"
#include "PoIntfManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmGlobalObjectManager;
    class NsmPortSecurityGlobalConfigMessage;
	class NsmEventPortSecGlobalMessage;

    class PortSecurityGlobalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmPortSecurityGlobalConfigMessageHandler( NsmPortSecurityGlobalConfigMessage *pNsmPortSecurityGlobalConfigMessage);
			ResourceId portSecurityValidationStep(PortSecurityContext *pPortSecurityContext);
			ResourceId portSecurityMessageStep(PortSecurityContext *pPortSecurityContext);
			ResourceId portSecurityMessageMOStep(PortSecurityContext *pPortSecurityContext);
			PortSecurityLocalManagedObject *getPortSecurityLocalMO(PoIntfManagedObject *intfMO);
			PortSecOuiLocalManagedObject *getOuiObject(PortSecurityLocalManagedObject *portSecMO, const string &oui);
			PortSecAllowedMacsLocalManagedObject *getAllowedMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId);
			PortSecStickyMacsLocalManagedObject *getStickyMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId);
			void createStickyMacMOs(NsmPortSecurityGlobalConfigMessage *pMsg, PortSecurityLocalManagedObject *portSecMo);
			void NsmEventPortSecGlobalMessageHandler(NsmEventPortSecGlobalMessage *pMsg);
			ResourceId portSecurityStickyMacMOStep(PortSecurityContext *pPortSecurityContext);

        protected:
        public:
            PortSecurityGlobalConfWorker ( NsmGlobalObjectManager *pNsmGlobalObjectManager);
            virtual    ~PortSecurityGlobalConfWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //PORTSECURITYGLOBALCONFWORKER_H
