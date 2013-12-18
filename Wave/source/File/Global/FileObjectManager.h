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
 *   Author : jiyer                                                        *
 **************************************************************************/

#ifndef FILEOBJECTMANAGER_H
#define FILEOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForBootPhases.h"
#include "Framework/ObjectModel/WaveManagedObjectSynchronousQueryContext.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForShutdownPhases.h"
#include "Framework/ObjectModel/WaveSendToClusterContext.h"


using namespace WaveNs;

namespace WaveNs
{

    class FILESVCAbortFileTransferMessage;

    class FileObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage      *createMessageInstance                       (const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance                 (const string &managedClassName);
            void              FILESVCAbortFileTransferMessageHandler       (FILESVCAbortFileTransferMessage *pFILESVCAbortFileTransferMessage);
            void              boot                                         (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void              install                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
		  void              disable                                      (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);
		  void              shutdown                                     (PrismAsynchronousContext *pPrismAsynchronousContext);
  		  void              failover                                     (FailoverAsynchronousContext *pFailoverAsynchronousContext);
		  void              listenForEvents                              (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
            void              TriggerFailOverCallback                      (WaveSendToClusterContext *pWaveSendToClusterContext );
                        
        protected:
        public:
                                             FileObjectManager             ();
            virtual                         ~FileObjectManager             ();
            static FileObjectManager        *getInstance                   ();
            static string                    getClassName                  ();
            static PrismServiceId            getPrismServiceId             ();

        // Now the data members
        private:

        protected:
        public:
    };
}
#endif    //FILEOBJECTMANAGER_H
