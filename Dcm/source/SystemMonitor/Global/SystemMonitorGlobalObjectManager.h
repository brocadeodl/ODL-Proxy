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

#ifndef SYSTEMMONITORGLOBALOBJECTMANAGER_H
#define SYSTEMMONITORGLOBALOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

using namespace WaveNs;

namespace DcmNs
{
    class ShowMonitorGlobalWorker;
    class GlobalFanWorker;
    class GlobalPowerWorker;
    class GlobalTempWorker;
    class GlobalWnnCardWorker;
    class GlobalSfpWorker;
    class GlobalCompactFlashWorker;
    class GlobalFaultyPortsWorker;
    class GlobalFruMailWorker;
	class GlobalSecMailWorker;
    class GlobalSfpMailWorker;
    class GlobalIntMailWorker;
    class GlobalRelayIpWorker;

    class SystemMonitorGlobalObjectManager: public WaveObjectManager
    {
        private:
            PrismMessage *createMessageInstance(const UI32 &operationCode);
            WaveManagedObject *createManagedObjectInstance(const string &managedClassName);
        	void    failover                                (FailoverAsynchronousContext *pFailoverAsynchronousContext);
        protected:
        public:
            SystemMonitorGlobalObjectManager ();
            virtual    ~SystemMonitorGlobalObjectManager ();
            static SystemMonitorGlobalObjectManager *getInstance();
            static string getClassName();
            static PrismServiceId getPrismServiceId();

// Now the data members

        private:
            ShowMonitorGlobalWorker  *m_pShowMonitorGlobalWorker;
            GlobalFanWorker  *m_pGlobalFanWorker;
            GlobalPowerWorker  *m_pGlobalPowerWorker;
            GlobalTempWorker  *m_pGlobalTempWorker;
            GlobalWnnCardWorker  *m_pGlobalWnnCardWorker;
            GlobalSfpWorker  *m_pGlobalSfpWorker;
            GlobalCompactFlashWorker  *m_pGlobalCompactFlashWorker;
            GlobalFaultyPortsWorker  *m_pGlobalFaultyPortsWorker;
            GlobalFruMailWorker  *m_pGlobalFruMailWorker;
			GlobalSecMailWorker  *m_pGlobalSecMailWorker;
            GlobalSfpMailWorker  *m_pGlobalSfpMailWorker;
            GlobalIntMailWorker  *m_pGlobalIntMailWorker;
            GlobalRelayIpWorker  *m_pGlobalRelayIpWorker;

        protected:
        public:
    };
}
#endif                                            //SYSTEMMONITORGLOBALOBJECTMANAGER_H
