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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSLOCALOBJECTMANAGER_H
#define QOSLOCALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{

    class QosLocalWorker;
	class CosToCosMutationMapManagedObject;
	class CosToTrafficClassMapManagedObject;
	class DscpToCosMapManagedObject;
	class DscpToTrafficClassMapManagedObject;
	class DscpToDscpMutationMapManagedObject;
	class CeeMapManagedObject;
	class PriorityGroupManagedObject;

    class QosLocalObjectManager: public WaveLocalObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
            void postboot(WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
            void ConfigReplayCosToCosMutationMap();
            void ConfigReplayCosToTrafficClassMap();
            void ConfigReplayQosQueueScheduler();
            void ConfigReplayQosQueueMulticastScheduler();
            void ConfigReplayQosRcvQueueMulticastSetting();
			void ConfigReplayCeeMap();
			void ConfigReplayCeeReemap();
			void ConfigReplayPriorityMap();
			void ConfigReplayPriorityGroup();
			void configReplayPhyQosMaps();
			void configReplayPoQosMaps();
			void configReplayPhyQosPfc();
			void configReplayPoQosPfc();
			void configReplayPhyCee();
			void configReplayPoCee();
            void configReplayPoPriorityTag();
			void configReplayPoQosCosProfile();
			void ConfigReplayDscpMutationMap();
			void ConfigReplayDscpTcMap();
			void ConfigReplayDscpCosMap();
			void ConfigReplayRedProfiles();
			void ConfigReplayAutoNasConfiguration();
			
        protected:
        public:
            QosLocalObjectManager ();
            virtual    ~QosLocalObjectManager ();
            static QosLocalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();
			CosToCosMutationMapManagedObject *getCosToCosMutationMapMOByObjectId(const ObjectId &oId);
			CosToTrafficClassMapManagedObject *getCosToTrafficClassMapMOByObjectId(const ObjectId &oId);
			DscpToCosMapManagedObject *getDscpToCosMapMOByObjectId(const ObjectId &oId);
			DscpToTrafficClassMapManagedObject *getDscpToTrafficClassMapMOByObjectId(const ObjectId &oId);
			DscpToDscpMutationMapManagedObject *getDscpToDscpMutationMapMOByObjectId(const ObjectId &oId);
			CeeMapManagedObject *getCeeMapMOByObjectId(const ObjectId &oId);
			PriorityGroupManagedObject* getPgMoByPgObjectId (const ObjectId &objectId);

// Now the data members

        private:
            QosLocalWorker  *m_pQosLocalWorker;

        protected:
        public:
    };
}
#endif                                            //QOSLOCALOBJECTMANAGER_H
