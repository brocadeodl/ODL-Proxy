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

/************************************************************************
  *   Copyright (C) 2012 Brocade Communications Systems, Inc.         	*
  *   All rights reserved.                                              *
  *   Author : Sachin Mishra					   	*
  ***********************************************************************/
#ifndef LOCALSECWORKER_H
#define LOCALSECWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "ThresholdMonitorLocalTypes.h"
using namespace WaveNs;

namespace DcmNs
{


	class ThresholdMonitorLocalObjectManager;
	class SecLocalMessage;
	class LocalSecWorker: public WaveWorker
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void SecLocalMessageHandler(SecLocalMessage *pSecLocalMessage);
		protected:
		public:
			LocalSecWorker ( ThresholdMonitorLocalObjectManager *pThresholdMonitorLocalObjectManager);
			virtual ~LocalSecWorker ();
			int fwSwitchCaseAlert(UI64 optArg, char *ptr);
			int backendSync();
			//Now the data members

		private:
		protected:
		public:
	};
}
#endif 		//LOCALSECWORKER_H
