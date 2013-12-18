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
 *   December, 2012                                                        *
 *   All rights reserved.                                                  *
  **************************************************************************/

#ifndef HAOPGLOBALOBJECTMANAGER_H
#define HAOPGLOBALOBJECTMANAGER_H

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Ha/Local/HaOpActionMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class HaOpGlobalActionMessage;

	class HaOpGlobalObjectManager: public WaveObjectManager
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			vector<WaveManagedObject *> *DoSynchronousQuery (const string className);
			void HaOpActionMessageHandler( HaOpGlobalActionMessage *pHaOpActionMessage);

            // List of over-ridden functions to gain certain priviledges
            bool isOperationAllowedBeforeEnabling (const UI32 &operationCode);

			void HaReloadHandler(PrismLinearSequencerContext *pFirmwareContext);
			void HaReloadCallback(FrameworkStatus frameworkStatus, HaOpActionMessage *pMessage, PrismLinearSequencerContext *pFirmwareContext);
			void prismLinearSequencerFailedStep (PrismLinearSequencerContext *pFirmwareContext);

		protected:
		public:
			HaOpGlobalObjectManager ();
			virtual    ~HaOpGlobalObjectManager ();
			static HaOpGlobalObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId();

		private:

		protected:
		public:
	};
}
#endif	//HAOPGLOBALOBJECTMANAGER_H
