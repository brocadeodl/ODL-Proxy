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
 *   Author : rhkumar/ranantha                                             *
 **************************************************************************/

#ifndef AGMAPPINGSPLUGINWORKER_H
#define AGMAPPINGSPLUGINWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"

using namespace WaveNs;

namespace DcmNs
{

    typedef enum
    {
        AG_UPDATE_DCMD = 0,
        AG_UPDATE_BACKEND
    }AgActionType;

    class AgLocalObjectManager;
    class AgStaticMappingsMessage;
    class AgMappingsMessage;
    class AgFailoverMessage;
    class AgFailbackMessage;
    class AgVFPortMessage;

    class AgMappingsPluginWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);

            void AgFailoverMessageHandler( AgFailoverMessage *pAgFailoverMessage);

            void AgFailbackMessageHandler( AgFailbackMessage *pAgFailibackMessage);

            void AgMappingsMessageHandler( AgMappingsMessage *pAgMappingsMessage);

            void AgStaticMappingsMessageHandler( AgStaticMappingsMessage *pAgStaticMappingsMessage);

	        void AgVFPortMessageHandler( AgVFPortMessage *pAgVFPortMessage);

            ResourceId updateBackendMappingsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendFailoverStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendFailbackStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateBackendStaticMappingsStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId updateMappingsMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateFailoverMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateFailbackMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updateStaticMappingsMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId validateFailoverStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext); 
            ResourceId validateFailbackStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext); 
            ResourceId validateMappingsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext); 
            ResourceId validateStaticMappingsStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext); 


	        ResourceId sendUpdateBackendMappings(AgNPortMapLocalManagedObject *pAgNPortMapLocalManagedObject);

	        ResourceId updateMOStepByVFEvent(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	        ResourceId updateBackendStepByVFEvent(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
	        ResourceId processVFEvent(AgVFPortMessage *pAgVFPortMessage, AgActionType action);
		
        protected:
        public:
            AgMappingsPluginWorker ( AgLocalObjectManager *pAgLocalObjectManager);
            virtual    ~AgMappingsPluginWorker ();

//	        ResourceId postbootDefaultMapSetup();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //AGMAPPINGSPLUGINWORKER_H
