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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef VCSCOPYTOSTARTUPWORKER_H
#define VCSCOPYTOSTARTUPWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/ObjectModel/WaveObjectManager.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "VcsFabric/Local/VcsCopyToStartupMessages.h"
#include "VcsFabric/Local/VcsCopyToStartupMessagingContext.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsCopyToStartupWorker : public WaveWorker
{
    public :
                                        VcsCopyToStartupWorker                      (VcsFabricLocalObjectManager *pVcsFabricLocalObjectManager);
        virtual                        ~VcsCopyToStartupWorker                      ();
                string                  getClassName                                ();

                PrismMessage           *createMessageInstance                       (const UI32 &operationCode);
        static  VcsCopyToStartupWorker *getInstance                                 ();

    protected :
    private :
		        void                    copyRunningToStartupMessageHandler          (VcsCopyRunningToStartupMessage *pVcsCopyRunningToStartupMessage);
		        void                    copyRunningToStartupStep                                (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    sendSyncUpdateMessage                       (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    sendCcmdSyncUpdateMessageStep               (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
                void                    removeCcmdCfgOnStandbyStep                  (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    copyDefaultToStartupMessageHandler          (VcsCopyDefaultToStartupMessage *pVcsCopyDefaultToStartupMessage);
                void                    cleanupPreservedConfigFilesStep             (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    sendCopyDefaultToStartupStep                (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    changeCcmdCfgValidityForDefaultConfigStep   (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);

                ResourceId              sendHaSyncUpdateMessage                                 (const bool &isDbSyncRequired, const UI32 &context);
                void                    copyToStartupAllowedMessageHandler                      (VcsCopyToStartupAllowedMessage *pVcsCopyToStartupAllowedMessage);
                void                    copyToStartupConfigMessageHandler           (VcsCopyToStartupMessage *pVcsCopyToStartupMessage);

		        void                    sendCopyFileToStartupStep                   (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
		        void                    changeCcmdCfgValidityForFileStep            (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
                void                    triggerStandbyCopyRunningToStartupIfPossible            (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
                void                    synchronizeStandbyCopyRunningToStartupStep              (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);
                void                    triggerStandbyCopyRunningToStartupIfPossibleCallback    (FrameworkStatus frameworkStatus, FrameworkObjectManagerHaSyncUpdateMessage *pMessage, VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);

                void                    changeCcmdCfgValidityForPersistRunningConfigStep        (VcsCopyToStartupMessagingContext *pVcsCopyToStartupMessagingContext);

    // Now the Data Members

    private :
    protected :
    public :

    friend class VcsFabricLocalObjectManager;
};

}

#endif // VCSCOPYTOSTARTUPWORKER_H
