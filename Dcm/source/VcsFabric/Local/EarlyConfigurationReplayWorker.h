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
 *   Copyright (C) 2010-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Cindy Hou                                                    *
 ***************************************************************************/

#ifndef EARLYCONFIGURATIONREPLAYWORKER_H
#define EARLYCONFIGURATIONREPLAYWORKER_H

#include <iostream>
#include <fstream>
#include "VcsFabric/Local/VcsNodeFabosInterfaceObjectManager.h"
#include "VcsFabric/Local/EarlyConfigReplayMessagingContext.h"
#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Utils/TraceUtils.h"

using namespace WaveNs;

namespace DcmNs
{

class EarlyConfigurationReplayWorker : public WaveWorker
{
    private :
        void        invalidStartupDBHandlerSuccessStep     (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        void        invalidStartupDBHandlerFailureStep     (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        bool        isBackupStartupConfigFileExist         ();

    protected :
    public :
                    EarlyConfigurationReplayWorker         (VcsNodeFabosInterfaceObjectManager *pVcsNodeFabosInterfaceObjectManager);
        virtual     ~EarlyConfigurationReplayWorker        ();
        static      EarlyConfigurationReplayWorker *getInstance ();

        string      getClassName                           ();

        void        invalidStartupDBHandler                ();
        void        configSnapshotEarlyConfigReplayHandler ();
        ResourceId  extractStartupFile                     (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        ResourceId  extractConfigSnapshotFile              (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext); 
        ResourceId  writeLinecardConfigurationToDB         (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        ResourceId  writeHardwareConnectorConfigToDB       (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        ResourceId  parseStartupFileStep                   (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        UI32        getHwIdFromLinecardType                (const string linecardType);
        UI32        getLinecardModeFromString              (const string linecardMode);
        ResourceId  editSystemMonitorConfig         	   (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        ResourceId  editFcspConfig		                   (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        ResourceId  editSecpolicyConfig                    (EarlyConfigReplayMessagingContext *pEarlyConfigReplayMessagingContext);
        friend class VcsNodeFabosInterfaceObjectManager;
};

}
#endif // EARLYCONFIGURATIONREPLAYWORKER_H 
