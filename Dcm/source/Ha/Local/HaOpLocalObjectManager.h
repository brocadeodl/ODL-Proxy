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
 *   October, 2011                                                         *
 *   All rights reserved.                                                  *
  **************************************************************************/

#ifndef HAOPLOCALOBJECTMANAGER_H
#define HAOPLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "DcmCore/DcmToolKit.h"
#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

	class HaOpActionMessage;
	class HaShowLocalMessage;

	class HaOpLocalObjectManager: public WaveLocalObjectManager
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void HaOpActionMessageHandler( HaOpActionMessage *pHaOpActionMessage);
			void HaShowLocalMessageHandler( HaShowLocalMessage *pHaShowLocalMessage);

            // List of over-ridden functions to gain certain priviledges
            bool isOperationAllowedBeforeEnabling (const UI32 &operationCode);

		protected:
		public:
			HaOpLocalObjectManager ();
			virtual    ~HaOpLocalObjectManager ();
			static HaOpLocalObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId();

		private:

		protected:
		public:
	};
}
#endif	//HAOPLOCALOBJECTMANAGER_H
