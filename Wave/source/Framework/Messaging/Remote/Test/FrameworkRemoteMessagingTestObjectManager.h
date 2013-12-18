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
 *   Copyright (C) 2005 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER_H
#define FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER_H

#include "Regression/PrismTestObjectManager.h"

namespace WaveNs
{

class FrameworkTestabilityMessage1;
class FrameworkTestabilityMessage3;
class FrameworkTestabilityMessage4;
class FrameworkLocalMessagingTestContext;
class WaveSendToClusterContext;
class WaveSendMulticastContext;

class FrameworkRemoteMessagingTestObjectManager : public PrismTestObjectManager
{
    private :
        FrameworkRemoteMessagingTestObjectManager ();

    protected :
    public :
        virtual                                           ~FrameworkRemoteMessagingTestObjectManager   ();
        static  FrameworkRemoteMessagingTestObjectManager *getInstance                                 ();
        static  PrismServiceId                             getPrismServiceId                           ();
                void                                       testRequestHandler                          (RegressionTestMessage *pMessage);
                void                                       selectARemoteLocationStep                   (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       simpleAsynchronousMessageTestStep           (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       simpleOneWayMessageTestStep                 (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       frameworkTestabilityMessage1Callback        (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       simpleSynchronousMessageTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       asynchronousMessageWithBuffersTestStep      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       asynchronousMessageWithBuffersCallback      (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage3 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       synchronousMessageWithBuffersTestStep       (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       asynchronousMessageWithLargeBuffersTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       synchronousMessageWithLargeBuffersTestStep  (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       asynchronousMessageAttributeTest            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       asynchronousMessageAttributeTestCallback    (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage4 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       synchronousMessageAttributeTest             (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       messageCloningTestStep                      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       messageCloningWithBuffersTestStep           (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1AllSuccessTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1AllSuccessTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithBuffersPhase1AllFailureTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1AllFailureTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStep (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterPhase1FewSuccessDisconnectnFailWithFlagTestStepCallback           (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStep(FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterPhase1FewSuccessDisconnectnFailButPhase2MessageWithFlagSetTestStepCallback  (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterPhase1FewDisconnectAndFailWithFlagTestStep      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterPhase1FewDisconnectAndFailWithFlagTestStepCallback                (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStep     (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterPhase1FewDisconnectAndFailButPhase2MessageWithFlagSetTestStepCallback       (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithBuffersPhase1SomeSuccessTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1SomeSuccessTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback        (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStep              (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterPhase1SomeDisconnectButPhase2MessageAndFlagSetTestStepCallback      (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithPartialSuccessFlagSetAndMessage1TestStep                        (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithPartialSuccessFlagSetAndMessage1TestStepCallback                (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStep                     (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterWithPartialSuccessAndFailingOverFlagSetTestStepCallback             (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterForSurrogateAllSuccessTestStep                                      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterForSurrogateAllSuccessTestStepCallback                              (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStep         (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterForSurrogateWithPartialSuccessAndFailingOverFlagSetTestStepCallback (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendToClusterForSurrogateWithLocationsSetByUserStep                              (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendToClusterForSurrogateWithLocationsSetByUserStepCallback                      (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       sendOneWayToWaveClusterTestStep                                                  (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       sendOneWayToWaveClusterTestStepCallback                                          (WaveSendToClusterContext *pWaveSendToClusterContext);
                void                                       remoteMessagingPerformanceMeasurementForSentToWaveCluster                        (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       remoteMessagingPerformanceMeasurementForSentToWaveClusterCallback                (WaveSendToClusterContext *pWaveSendToClusterContext);  
                void                                       remoteMessagingPerformanceMeasurementForSentMulticast                            (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                       remoteMessagingPerformanceMeasurementForSentMulticastCallback                    (WaveSendMulticastContext *pWaveSendMulticastContext);

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER_H
