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
 *   Author : Brian Adaniya                                                *
 ***************************************************************************/

#ifndef DISTRIBUTEDLOGOBJECTMANAGER_H
#define DISTRIBUTEDLOGOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveObjectManager.h"

namespace WaveNs
{

class DistributedLogAddLogEntryMessage;
class DistributedLogUpdateMaxLogEntriesMessage;
class DistributedLogSynchronousLinearSequencerContext;
class DistributedLogConfigurationManagedObject;
class DistributedLogUpdateLogIdMessage;

class DistributedLogObjectManager : public WaveObjectManager
{
    private :
                void                            initialize                                                  (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            install                                                     (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);
                void                            haboot                                                      (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

                ResourceId                      distributedLogInstallDefaultConfigurationMOStep             (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogUpdateInMemoryObjectsFromConfigurationMOStep  (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogUpdateInMemoryObjectsFromEntriesMOStep        (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);

                void                            distributedLogAddLogEntryMessageHandler                     (DistributedLogAddLogEntryMessage *pDistributedLogAddLogEntryMessage);
                void                            distributedLogUpdateMaxLogEntriesMessageHandler             (DistributedLogUpdateMaxLogEntriesMessage *pDistributedLogUpdateMaxLogEntriesMessage);
                void                            distributedLogUpdateInMemoryLogIdMessageHandler             (DistributedLogUpdateLogIdMessage *pDistributedLogUpdateLogIdMessage);

                ResourceId                      distributedLogSetupADelayedCommitTransactionIfRequiredStep  (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogAddLogEntryStep                               (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogUpdateFirstAndNextLogIdStep                   (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogRemoveLogEntriesIfRequiredStep                (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);
                ResourceId                      distributedLogUpdateMaxLogEntriesStep                       (DistributedLogSynchronousLinearSequencerContext *pDistributedLogSynchronousLinearSequencerContext);

                UI64                            getNumberOfLogEntries                                       () const; 
                bool                            isLogEmpty                                                  () const;
                bool                            isLogFull                                                   () const;

        static  ResourceId                      distributedLogAddLogEntryDebugHandler                       (UI32 argc, vector<string> argv);
        static  ResourceId                      distributedLogUpdateMaxLogEntriesDebugHandler               (UI32 argc, vector<string> argv);
                void                            dbInconsistencyCheck                                        (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
                                                DistributedLogObjectManager                                 ();
        virtual                                ~DistributedLogObjectManager                                 ();

        virtual PrismMessage                   *createMessageInstance                                       (const UI32 &operationCode);
                WaveManagedObject              *createManagedObjectInstance                                 (const string &managedClassName);

        static  string                          getServiceName                                              ();
        static  DistributedLogObjectManager    *getInstance                                                 ();
        static  PrismServiceId                  getPrismServiceId                                           ();

    // Now the data members

    private :
        UI64                                        m_maxLogEntries;
        UI64                                        m_firstLogId;
        UI64                                        m_nextLogId;

    protected :
    public :
};

}

#endif // DISTRIBUTEDLOGOBJECTMANAGER_H
