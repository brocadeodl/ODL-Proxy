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

#ifndef HASHOWGLOBALOBJECTMANAGER_H
#define HASHOWGLOBALOBJECTMANAGER_H

#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Ha/Local/HaShowLocalMessage.h"
#include "Ha/Global/HaShowClusterMessage.h"

using namespace WaveNs;

namespace DcmNs
{

	class HaShowClusterMessage;

	class HaShowGlobalObjectManager: public WaveObjectManager
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			vector<WaveManagedObject *> *DoSynchronousQuery (const string className);
			void HaShowClusterMessageHandler( HaShowClusterMessage *pHaShowClusterMessage);
			void HaShowHandler(PrismLinearSequencerContext *pFirmwareContext);
			bool isOperationAllowedBeforeEnabling (const UI32 &operationCode);
		protected:
		public:
			HaShowGlobalObjectManager ();
			virtual    ~HaShowGlobalObjectManager ();
			static HaShowGlobalObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId();

		private:

		protected:
		public:
	};
}
#endif	//HASHOWGLOBALOBJECTMANAGER_H
