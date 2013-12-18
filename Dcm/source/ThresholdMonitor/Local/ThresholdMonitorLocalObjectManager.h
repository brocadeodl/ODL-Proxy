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
 *   Author : pahuja                                                     *
 **************************************************************************/

#ifndef THRESHOLDMONITORLOCALOBJECTMANAGER_H
#define THRESHOLDMONITORLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodeFabosDomainEvents.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForConfigReplayEnd.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"


using namespace WaveNs;

namespace DcmNs
{

	class LocalCpuWorker;
	class LocalMemoryWorker;
	class LocalSfpMonitorWorker;
	class LocalSfpConfigWorker;
	class LocalSecWorker;
	class LocalSecConfigWorker;
	class LocalInterfaceMonitorWorker;
	class LocalInterfaceConfigWorker;
	class LocalInterfaceShowWorker;
	class LocalSfpShowWorker;
	class LocalSecShowWorker;

	
	class ThresholdMonitorLocalObjectManager: public WaveLocalObjectManager
	{
		private:
			PrismMessage *createMessageInstance(const UI32 &operationCode);
			WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void  externalStateSynchronization (WaveAsynchronousContextForExternalStateSynchronization
					*pWaveAsynchronousContextForExternalStateSynchronization);
			void configReplayEnd (WaveAsynchronousContextForConfigReplayEnd
					*pWaveAsynchronousContextForConfigReplayEnd);
			void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			void initManagedObjects ();
		protected:
		public:
			ThresholdMonitorLocalObjectManager ();
			virtual    ~ThresholdMonitorLocalObjectManager ();
			static ThresholdMonitorLocalObjectManager *getInstance();
			static string getClassName();
			static PrismServiceId getPrismServiceId();

			// Now the data members

		private:
			LocalCpuWorker  	*m_pLocalCpuWorker;
			LocalMemoryWorker  	*m_pLocalMemoryWorker;
			LocalSfpMonitorWorker 	*m_pLocalSfpMonitorWorker;
			LocalSfpConfigWorker 	*m_pLocalSfpConfigWorker;
			LocalSecWorker		*m_pLocalSecWorker;
			LocalSecConfigWorker	*m_pLocalSecConfigWorker;
			LocalInterfaceMonitorWorker 	*m_pLocalInterfaceMonitorWorker;
			LocalInterfaceConfigWorker 	*m_pLocalInterfaceConfigWorker;
			LocalInterfaceShowWorker 	*m_pLocalInterfaceShowWorker;
			LocalSfpShowWorker 	*m_pLocalSfpShowWorker;
			LocalSecShowWorker 	*m_pLocalSecShowWorker;

		protected:
		public:
	};
}
#endif                                            //THRESHOLDMONITORLOCALOBJECTMANAGER_H
