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
 *   Author : sumank                                                     *
 **************************************************************************/

#ifndef PORTSECURITYLOCALCONFWORKER_H
#define PORTSECURITYLOCALCONFWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "PortSecurityLocalManagedObject.h"
#include "PhyIntfLocalManagedObject.h"
#include "Nsm/Local/PortSecurityContext.h"
#include "Nsm/Local/PortSecAllowedMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecStickyMacsLocalManagedObject.h"
#include "Nsm/Local/PortSecOuiLocalManagedObject.h"
#include "Nsm/Local/NsmEventPortSecLocalMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class NsmLocalObjectManager;
    class NsmPortSecurityConfigMsgMessage;

    class PortSecurityLocalConfWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void NsmPortSecurityConfigMsgMessageHandler( NsmPortSecurityConfigMsgMessage *pNsmPortSecurityConfigMsgMessage);
			ResourceId portSecurityValidationStep(PortSecurityContext *pPortSecurityContext);
			ResourceId portSecurityMessageStep(PortSecurityContext *pPortSecurityContext);
			ResourceId portSecurityMessageMOStep(PortSecurityContext *pPortSecurityContext);

        protected:
        public:
            PortSecurityLocalConfWorker ( NsmLocalObjectManager *pNsmLocalObjectManager);
            virtual    ~PortSecurityLocalConfWorker ();
			PortSecurityLocalManagedObject *getPortSecurityLocalMO(PhyIntfLocalManagedObject *intfMO);
			PortSecAllowedMacsLocalManagedObject *getAllowedMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId);
			PortSecStickyMacsLocalManagedObject *getStickyMacObject(PortSecurityLocalManagedObject *portSecMO, const string &mac, const UI32 &vlanId);
			PortSecOuiLocalManagedObject *getOuiObject(PortSecurityLocalManagedObject *portSecMO, const string &oui);
			void createStickyMacMOs(NsmPortSecurityConfigMsgMessage *pMsg, PortSecurityLocalManagedObject *portSecMo);
			void NsmEventPortSecLocalMessageHandler(NsmEventPortSecLocalMessage *pNsmEventPortSecLocalMessage);
			ResourceId portSecurityStickyMacMOStep(PortSecurityContext *pPortSecurityContext);

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //PORTSECURITYLOCALCONFWORKER_H
