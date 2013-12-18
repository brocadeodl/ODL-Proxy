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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODELOCALMESSAGINGTESTOBJECTMANAGER_H
#define VCSNODELOCALMESSAGINGTESTOBJECTMANAGER_H

#include "Regression/PrismTestObjectManager.h"
#include "VcsFabric/Local/VcsNodeVcsIdMessages.h"
#include "VcsFabric/Local/VcsNodeSwitchMacMessages.h"
#include "VcsFabric/Local/VcsNodeMappedIdMessages.h"
#include "VcsFabric/Local/VcsNodeVcsModeMessages.h"
#include "VcsFabric/Local/VcsNodeLocalManagedObject.h"
#include "VcsFabric/Local/VcsFabricLocalObjectManager.h"
#include "VcsFabric/Local/VcsNodePrincipalMessages.h"

namespace DcmNs
{

/*class VcsNodeSetVcsIdMessage;

class FrameworkTestabilityMessage5;
class FrameworkTestabilityMessage6;
class FrameworkLocalMessagingTestContext;
*/

class VcsNodeLocalMessagingTestObjectManager : public PrismTestObjectManager
{
    private :
                            VcsNodeLocalMessagingTestObjectManager ();
        WaveMessageStatus   simpleSynchronousMessageTest (PrismMessage *message);
        WaveMessageStatus   simpleAsynchronousMessageTest (PrismLinearSequencerContext *&pPrismLinearSequencerContext, vector<PrismMessage *>&sagesVector);
        VcsNodeLocalManagedObject* getSwitchObject(const ObjectId &switchObjectId);

    protected :
    public :
        virtual                                          ~VcsNodeLocalMessagingTestObjectManager ();
        static  VcsNodeLocalMessagingTestObjectManager *getInstance                              ();
        static  PrismServiceId                            getPrismServiceId                        ();
                void                                      setVcsNodeMessageCallback                 (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pMessage, PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      setVcsIdMessageCallback                 (FrameworkStatus frameworkStatus, ManagementInterfaceMessage *pMessage, PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      testRequestHandler                        (RegressionTestMessage *pMessage);

                void                                      simpleAsyncSetVcsIdTest                   (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncSetVcsIdTest                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncGetVcsIdTest                   (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncGetVcsIdTest                    (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncSetIsPrincipalTest             (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncSetIsPrincipalTest              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncGetIsPrincipalTest             (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncGetIsPrincipalTest              (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncGetSwitchMacTest               (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncGetSwitchMacTest                (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncGetLocalMappedIdTest           (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncGetLocalMappedIdTest            (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncSetVcsModeTest                 (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncSetVcsModeTest                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleAsyncGetVcsModeTest                 (PrismLinearSequencerContext *pPrismSynchronousLinearSequencerContext);
                void                                      simpleSyncGetVcsModeTest                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleEventTestStep                       (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                      simpleDcmToolKitTestStep                  (PrismLinearSequencerContext *pPrismLinearSequencerContext);


    // Now the data members

    private :
        static VcsNodeLocalMessagingTestObjectManager *m_pVcsNodeLocalMessagingTestObjectManager;

    protected :
    public:
};

}

#endif // VCSNODELOCALMESSAGINGTESTOBJECTMANAGER_H
