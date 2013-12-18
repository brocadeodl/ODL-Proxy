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
 *   Author : ranantha                                             *
 **************************************************************************/

#ifndef AGPGPLUGINWORKER_H
#define AGPGPLUGINWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "AG/Local/AgNPortMapLocalManagedObject.h"
#include "AG/Local/AgPortGroupLocalManagedObject.h"
#include "AG/Local/AgPGCreateMessage.h"
#include "AG/Local/AgPGAddMessage.h"
#include "AG/Local/AgPGDelMessage.h"
#include "AG/Local/AgPGRenameMessage.h"
#include "AG/Local/AgPGRemoveMessage.h"
#include "AG/Local/AgPGModesMessage.h"


using namespace WaveNs;

namespace DcmNs
{
    class AgLocalObjectManager;
    class AgMappingsMessage;
    class AgFailoverMessage;
    class AgPGModesMessage;


    class AgPGPluginWorker: public WaveWorker
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);




            void AgPGCreateMessageHandler( AgPGCreateMessage *pAgPGCreateMessage);
            ResourceId updateBackendPGCreateStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
    ResourceId validatePGCreateStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);




            void  AgPGAddMessageHandler( AgPGAddMessage *pAgPGAddMessage);
            ResourceId updateBackendPGAddStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGMOForAddStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
  ResourceId validatePGAddStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);





            void  AgPGDelMessageHandler( AgPGDelMessage *pAgPGDelMessage);
            ResourceId updateBackendPGDelStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGMOForDelStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
  ResourceId validatePGDelStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);





            void  AgPGRenameMessageHandler( AgPGRenameMessage *pAgPGRenameMessage);
            ResourceId updateBackendPGRenameStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGMOForRenameStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);





            void  AgPGRemoveMessageHandler( AgPGRemoveMessage *pAgPGRemoveMessage);
            ResourceId updateBackendPGRemoveStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGMOForRemoveStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
  ResourceId validatePGRemoveStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);


            void AgPGModesMessageHandler( AgPGModesMessage *pAgPGModesMessage);
            ResourceId updateBackendPGModesStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
            ResourceId updatePGModesMOStep (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
  ResourceId validatePGModesStep(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);


        protected:
        public:
            AgPGPluginWorker ( AgLocalObjectManager *pAgLocalObjectManager);
            virtual    ~AgPGPluginWorker ();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //AGPGPLUGINWORKER_H
