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
 *   Author : kghanta                                                     *
 **************************************************************************/

#ifndef GLOBALAUTHORIZATIONWORKER_H
#define GLOBALAUTHORIZATIONWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"

using namespace WaveNs;

namespace DcmNs
{

	class AAAGlobalObjectManager;
	class AAAGlobalAddRuleMessage;
	class AAAGlobalChangeRuleMessage;
	class AAAGlobalDeleteRuleMessage;

	class GlobalAuthorizationWorker: public WaveWorker
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void AAAGlobalAddRuleMessageHandler( AAAGlobalAddRuleMessage *pAAAGlobalAddRuleMessage);
			void AAAGlobalChangeRuleMessageHandler( AAAGlobalChangeRuleMessage *pAAAGlobalChangeRuleMessage);
 			void AAAGlobalDeleteRuleMessageHandler( AAAGlobalDeleteRuleMessage *pAAAGlobalDeleteRuleMessage);
			void addRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			void changeRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
			void deleteRuleCleanCacheSTCCallback(WaveSendToClusterContext *pWaveSendToClusterContext);
		protected:
		public:
			GlobalAuthorizationWorker ( AAAGlobalObjectManager *pAAAGlobalObjectManager);
			virtual    ~GlobalAuthorizationWorker ();

// Now the data members
		private:
		protected:
		public:
    };
}
#endif                                            //GLOBALAUTHORIZATIONWORKER_H
