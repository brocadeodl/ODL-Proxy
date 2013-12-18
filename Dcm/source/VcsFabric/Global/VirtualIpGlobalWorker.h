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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.         *
 *   All rights reserved.                                                 *
 *   Author : cshah, Brian Adaniya                                        *
 **************************************************************************/

#ifndef VIRTUALIPGLOBALWORKER_H
#define VIRTUALIPGLOBALWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "VcsFabric/Global/VcsFabObjectManager.h"
#include "VcsFabric/Global/VirtualIpBindTimer.h"
#include "Ipadm/Local/IpadmTypes.h"

using namespace WaveNs;

namespace DcmNs
{

class VirtualIpGlobalConfigVipMessage;
class VirtualIpGlobalDeleteVipMessage;
class VirtualIpGlobalBindVipMessage;
class VirtualIpGlobalUnbindVipMessage;
class VirtualIpBindTimer;
class VirtualIpGlobalWorkerSequencerContext;

class VirtualIpGlobalWorker: public WaveWorker
{
    private:
        PrismMessage       *createMessageInstance                   (const UI32 &operationCode);
        WaveManagedObject  *createManagedObjectInstance             (const string &managedClassName);

        void                postboot                                (WaveAsynchronousContextForPostbootPhase *pWaveAsynchronousContextForPostBootPhase);
        void                shutdown                                (WaveAsynchronousContextForShutDownPhases *pWaveAsynchronousContextForShutDownPhases);

        void                virtualIpGlobalConfigVipMessageHandler  (VirtualIpGlobalConfigVipMessage *pVirtualIpGlobalConfigVipMessage);
        void                virtualIpGlobalDeleteVipMessageHandler  (VirtualIpGlobalDeleteVipMessage *pVirtualIpGlobalDeleteVipMessage);
        void                virtualIpGlobalBindVipMessageHandler    (VirtualIpGlobalBindVipMessage *pVirtualIpGlobalBindVipMessage);
        void                virtualIpGlobalUnbindVipMessageHandler  (VirtualIpGlobalUnbindVipMessage *pVirtualIpGlobalUnbindVipMessage);

        void                queryVirtualIpStep                      (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                validateConfiguringVirtualIpStep        (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                validateDeletingVirtualIpStep           (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                validateBindingVirtualIpStep            (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                validateUnbindingVirtualIpStep          (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                configureVirtualIpManagedObjectStep     (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                deleteVirtualIpManagedObjectStep        (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                bindVirtualIpForConfigurationStep       (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                bindVirtualIpWithTimerRetryStep         (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);
        void                unbindVirtualIpStep                     (VirtualIpGlobalWorkerSequencerContext *pVirtualIpGlobalWorkerSequencerContext);

        int                 virtualIpAddressBindLocal               (const string &ipAddress);
        ResourceId          virtualIpAddressBindWithRetryTimer      (const string &ipAddress);
        void                virtualIpBindTimerHandler               (TimerHandle &timerHandle);
        bool                isVirtualIpBoundToThisNode              (const string &ipAddress);
        bool                isVirtualIpDuplicate                    (const string &ipAddress);

        int                 virtualIpAddressBind                    (const string &ipAddress);
        int                 virtualIpAddressUnBind                  ();

    protected:
    public:
                            VirtualIpGlobalWorker                   (VcsFabObjectManager* pWaveObjectManager);
        virtual            ~VirtualIpGlobalWorker                   ();
 static string              getCurrentVirtualIpAddress              ();  

    // Now the data members

    private:
        VirtualIpBindTimer *m_pVirtualIpBindTimer;

    protected:
    public:
};

}
#endif //VIRTUALIPGLOBALWORKER_H
