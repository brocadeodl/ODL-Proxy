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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef PRISMFRAMEWORKOBJECTMANAGERPOSTPERSISTENTBOOTWORKER_H
#define PRISMFRAMEWORKOBJECTMANAGERPOSTPERSISTENTBOOTWORKER_H

#include "Framework/ObjectModel/WaveWorker.h"
#include "Framework/Postboot/PrismPostPersistentBootMessages.h"

namespace WaveNs
{

class PrismPostPersistenceBootMessage;
class PrismPostBootHardwareSyncMessage;
class PrismPostPersistentBootWorkerClusterContext;

class PrismFrameworkObjectManagerPostPersistentBootWorker : public WaveWorker
{
    private :
                void          postPersistentBootMessageHandler                      (PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage);
                void          postPersistentBootServiceAndWaveSlotsMessageHandler   (PrismPostPersistenceBootServiceAndWaveSlotsMessage *pPrismPostPersistenceBootServiceAndWaveSlotsMessage);
                ResourceId    copyRunningToStart                                    ();
                string        createReplayString                                    (const string &path, const string &params, const string &file);
                void          triggerPostBootPassTable                              (const UI32 &eventId, const UI32 &parameter, PrismPostPersistenceBootMessage *pPrismPostPersistenceBootMessage);
                ResourceId    triggerStartupFileReplay                              ();
                ResourceId    triggerLastConfigReplay                               ();
                void          resetXPathStringsTimestampsInPersistenceObjectManager ();
                ResourceId    setupStartupAfterConfigReplay                             ();

    protected :
    public :
                 PrismFrameworkObjectManagerPostPersistentBootWorker                (WaveObjectManager *pWaveObjectManager);
        virtual ~PrismFrameworkObjectManagerPostPersistentBootWorker                ();
                ResourceId    triggerPostBootPassTableForAll                        (PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext);  
                ResourceId    triggerPostBootPassTableForEvent                      (const PrismPostPersistentBootWorkerClusterContext *pPostPersistentBootWorkerClusterContext);

    // Now the Data Members
    private :
    protected :
    public :
};

}

#endif // PRISMFRAMEWORKOBJECTMANAGERPOSTPERSISTENTBOOTWORKER_H
