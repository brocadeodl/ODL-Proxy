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
 *   Author : hzhu                                                     *
 **************************************************************************/

#ifndef RTMLOCALOBJECTMANAGER_H
#define RTMLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/ObjectModel/WaveAsynchronousContextForSlotFailover.h"
#include "RtmConfigLocalManagedObject.h"
#include "RtmNhProtoLocalManagedObject.h"
#include "IfStaticRouteLocalManagedObject.h"
#include "NhStaticRouteLocalManagedObject.h"
#include "Rtm/Local/NhVrfLocalManagedObject.h"
#include "RtmUpdateStaticRouteMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class RtmUpdateRtmConfigMessage;
    class RtmUpdateStaticRouteMessage;
	class DcmManagementInterfaceMessage;
	class RtmLocalShowMessage;

    class RtmLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void RtmUpdateRtmConfigMessageHandler( RtmUpdateRtmConfigMessage *pRtmUpdateRtmConfigMessage);
            void RtmUpdateStaticRouteMessageHandler( RtmUpdateStaticRouteMessage *pRtmUpdateStaticRouteMessage);

            void RtmShowMessageHandler (RtmLocalShowMessage *pMessage);

			ResourceId processResult(PrismMessage* pLocMsg, DcmManagementInterfaceMessage* pClientMsg, ResourceId status);

			ResourceId updateAllRtmConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

			ResourceId updateRtmConfig (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getRtmConfigMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
									  RtmConfigLocalManagedObject **pMo, bool create);

			ResourceId updateNhProto (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getNhProtoMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
									  RtmNhProtoLocalManagedObject **pMo, RtmUpdateRtmConfigMessage *pMsg, bool create);

			ResourceId updateStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

			ResourceId updateNhStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getNhStaticRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
										   NhStaticRouteLocalManagedObject **pMo, RtmUpdateStaticRouteMessage *pMsg, bool create);

			ResourceId updateIfStaticRoute (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
			ResourceId getIfStaticRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext, 
										   IfStaticRouteLocalManagedObject **pMo, 
										   RtmUpdateStaticRouteMessage *pMsg, bool create);

			ResourceId updateAllRtmConfigToBackend (PrismSynchronousLinearSequencerContext 
												   *pPrismSynchronousLinearSequencerContext);
			ResourceId updateStaticRouteToBackend (PrismSynchronousLinearSequencerContext 
												  *pPrismSynchronousLinearSequencerContext);

			ResourceId validateStaticRouteConfig (PrismSynchronousLinearSequencerContext *syncSeqContext);
			ResourceId validateIfName (UI32 ifType, const string &ifName, PrismSynchronousLinearSequencerContext *syncSeqContext);
            ResourceId validateRtmUpdateConfig (PrismSynchronousLinearSequencerContext *syncSeqContext);

			int32_t getSlotFromIfName (UI32 ifType, string ifName);

            ResourceId updateNhVrf(PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext);

            ResourceId getNhVrfRouteMo (PrismSynchronousLinearSequencerContext *pPrismSynchronousLinearSequencerContext,
                                        NhVrfLocalManagedObject **pMo, RtmUpdateStaticRouteMessage *pMsg, bool create);
      virtual void multiPartitionPartialCleanup (WaveAsynchronousContextForMultiPartitionCleanup *pWaveAsynchronousContextForMultiPartitionCleanup);


			void restoreRtmConfig(void);
			void restoreNhStaticRoute(void);
			void restoreIfStaticRoute(UI32 slot);
			void flush_mgmt_routes(void);
			void deleteIfStaticRoute(UI32 slot);
            void restoreNhVrfStaticRoute(void);

        protected:
        public:
            RtmLocalObjectManager ();
            virtual ~RtmLocalObjectManager ();
            static RtmLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

			void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
			void slotFailover(WaveAsynchronousContextForSlotFailover *pWaveAsynchronousContextForSlotFailover);

// Now the data members

        private:

        protected:
        public:
    };
    
    inline string  RtmLocalObjectManager::getClassName()
    {
        return ("Rtm");
    }

}
#endif                                            //RTMLOCALOBJECTMANAGER_H
