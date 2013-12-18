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
 *   Author : sshaik                                                     *
 **************************************************************************/

#ifndef DIAGLOCALOBJECTMANAGER_H
#define DIAGLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

#include "Diag/Local/DiagLocalPostManagedObject.h"
#include "Diag/Local/DiagLocalPostMessage.h"
//#include "ClientInterface/DIAG/DiagPostEnqMessage.h"

 
using namespace WaveNs;

namespace DcmNs
{

    class DiagLocalPostWorker;
	class DiagLocalPostMessage;
	class DiagLocalPostManagedObject;


    class DiagLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
			void DiagPostEnableMessageHandler(DiagLocalPostMessage* pDiagPostMessage);
			//void DiagPostEnqMessageHandler(DiagPostEnqMessage* pDiagPostEnqMessage);
			void install( WaveAsynchronousContextForBootPhases * pWaveAsynchronousContextForBootPhases);
			void boot (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
			ResourceId updatePostEnable (PrismSynchronousLinearSequencerContext *pDiagUpdatePostEnableMessageContext);
			//ResourceId enqPostEnable (PrismSynchronousLinearSequencerContext *pDiagEnqPostEnableMessageContext);
			DiagLocalPostManagedObject* QueryDiagLocalPostManagedObject(UI32 rbridgeId);
            void SyncPOSTStatus(void);

        protected:
        public:
            DiagLocalObjectManager ();
            virtual    ~DiagLocalObjectManager ();
            static DiagLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
		//    void install (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

// Now the data members

        private:
            DiagLocalPostWorker  *m_pDiagLocalPostWorker;

        protected:
        public:
    };
}
#endif                                            //DIAGLOCALOBJECTMANAGER_H
