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

#ifndef SPANGLOBALOBJECTMANAGER_H
#define SFLOWGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

	class SpanGlobalWorker;

	class SpanGlobalObjectManager: public WaveObjectManager
	{
	      private:
	         PrismMessage *createMessageInstance(const UI32 &operationCode);
             WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
          protected:
	      public:
	         SpanGlobalObjectManager ();
	         virtual    ~SpanGlobalObjectManager ();
	         static SpanGlobalObjectManager *getInstance();
	         static string getClassName();
             static PrismServiceId getPrismServiceId();
			 void failover(FailoverAsynchronousContext *pFailoverAsynchronousContext);
			 ResourceId SpanGlobalHandleFailoverForLocation (UI32 &location_id,FailoverAsynchronousContext *pFailoverAsynchronousContext);
			 void spanFailoverSendToClusterCallback (WaveSendToClusterContext *ctx_p);
			 ResourceId SpanGlobalCleanupForLocation (UI32 &location_id,FailoverAsynchronousContext *pFailoverAsynchronousContext);
			 void clusterCreateCollectValidationData(WaveObjectManagerCollectValidationDataMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);
             void clusterCreateValidate (WaveObjectManagerValidateClusterCreationMessage *pMessage, PrismAsynchronousContext *pPrismAsynchronousContext);

// Now the data members

         private:
              SpanGlobalWorker  *m_pSpanGlobalWorker;
         protected:
         public:
    };
}
#endif                                            //SFLOWGLOBALOBJECTMANAGER_H
                                         
