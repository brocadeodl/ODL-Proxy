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

#ifndef DISTRIBUTEDLOGTESTOBJECTMANAGER_H
#define DISTRIBUTEDLOGTESTOBJECTMANAGER_H

#include "Regression/PrismTestObjectManager.h"

namespace WaveNs
{

class DistributedLogInvokeAddLogApiMessage;
class DistributedLogTestContext;

class DistributedLogTestObjectManager : public PrismTestObjectManager
{
    private :
        virtual PrismMessage                       *createMessageInstance                                   (const UI32 &operationCode);
                void                                invokeAddLogApiMessageHandler                           (DistributedLogInvokeAddLogApiMessage *pMessage);
                void                                testRequestHandler                                      (RegressionTestMessage *pMessage);
                void                                dumpAndSaveOriginalStatsTestStep                        (DistributedLogTestContext *pDistributedLogTestContext);
                void                                queryForCurrentConfigInfoTestStep                       (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addMaximumLogEntriesTestStep                            (DistributedLogTestContext *pDistributedLogTestContext);
                void                                verifyMaximumLogEntriesTestStep                         (DistributedLogTestContext *pDistributedLogTestContext);
                void                                rotateLogEntriesTestStep                                (DistributedLogTestContext *pDistributedLogTestContext);
                void                                verifyRotatedLogEntriesTestStep                         (DistributedLogTestContext *pDistributedLogTestContext);
                void                                decreaseMaxAllowedLogEntriesTestStep                    (DistributedLogTestContext *pDistributedLogTestContext);
                void                                verifyDecreasedLogEntriesTestStep                       (DistributedLogTestContext *pDistributedLogTestContext);
                void                                increaseMaxAllowedLogEntriesTestStep                    (DistributedLogTestContext *pDistributedLogTestContext);
                void                                verifyUnchangedLogEntriesTestStep                       (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addMaximumLogEntriesUsingSendOneWayApiTestStep          (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addAuditLog1UsingSendToWaveClusterTestStep              (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addAuditLog2UsingSendToWaveClusterTestStep              (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addAuditLog3UsingSendToWaveClusterTestStep              (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addManagedObjectLog1UsingSendToWaveClusterTestStep      (DistributedLogTestContext *pDistributedLogTestContext);
                void                                addLogUsingSendToWaveClusterTestCallback                (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                restoreMaxAllowedLogEntriesTestStep                     (DistributedLogTestContext *pDistributedLogTestContext);

                ResourceId                          queryForNumberOfLogEntries                              (UI64 &numberOfLogEntries); 
                ResourceId                          queryForFirstLogId                                      (UI64 &firstLogId); 
                ResourceId                          queryForLastLogId                                       (UI64 &lastLogId); 
                ResourceId                          queryForLogEntryInformation                             (UI64 &count, UI64 &firstLogId, UI64 &lastLogId);

    protected :
    public :
                                                    DistributedLogTestObjectManager                         ();
        virtual                                    ~DistributedLogTestObjectManager                         ();

        static  string                              getServiceName                                          ();
        static  DistributedLogTestObjectManager    *getInstance                                             ();
        static  PrismServiceId                      getPrismServiceId                                       ();


    // Now the data members

    private :
    protected :
    public :

};

} 

#endif // DISTRIBUTEDLOGTESTOBJECTMANAGER_H
