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

#ifndef FRAMEWORKTESTABILITY1OBJECTMANAGER_H
#define FRAMEWORKTESTABILITY1OBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "ManagementInterface/ClientInterface/Test/ManagementInterfaceClientTestMessages.h"

namespace WaveNs
{

class PrismLinearSequencerContext;

class FrameworkTestabilityMessage1;
class FrameworkTestabilityMessage2;
class FrameworkTestabilityMessage3;
class FrameworkTestabilityMessage4;
class FrameworkTestabilityMessage5;
class FrameworkTestabilityMessage6;
class FrameworkTestabilityMessage7;
class FrameworkTestabilityEvent1;
class FrameworkTestabilityManagementInterfaceMessage1;
class FrameworkTestabilityManagementInterfaceMessage2;
class FrameworkTestabilityManagementInterfaceMessage3;
class FrameworkTestabilityManagementInterfaceMessage4;
class FrameworkTestabilityManagementInterfaceMessageLeakMessage;
class FrameworkTestabilityManagementInterfaceObjectLeakMessage;
class FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage;
class FrameworkTestabilityManagementInterfaceFileServiceUsageMessage;

class FrameworkTestability1ObjectManager : public WaveLocalObjectManager
{
    private :
                                           FrameworkTestability1ObjectManager       ();
               PrismMessage               *createMessageInstance                    (const UI32 &operationCode);
        static ManagementInterfaceMessage *createManagementInterfaceMessageInstance (const UI32 &operationCode);

        void          listenForEvents                    (WaveAsynchronousContextForBootPhases *pWaveAsynchronousContextForBootPhases);

    protected :
    public :
        virtual                                    ~FrameworkTestability1ObjectManager                            ();
        static  FrameworkTestability1ObjectManager *getInstance                                                   ();
        static  PrismServiceId                      getPrismServiceId                                             ();
        static  string                              getPrismServiceName                                           ();

                void                                frameworkTestabilityMessage1RequestHandler                    (FrameworkTestabilityMessage1 *pMessage);
                void                                frameworkTestabilityMessage2RequestHandler                    (FrameworkTestabilityMessage2 *pMessage);
                void                                frameworkTestabilityMessage3RequestHandler                    (FrameworkTestabilityMessage3 *pMessage);
                void                                frameworkTestabilityMessage3ValidateInputBuffersStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                frameworkTestabilityMessage3AttachOutputBuffersStep           (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                frameworkTestabilityMessage4RequestHandler                    (FrameworkTestabilityMessage4 *pMessage);
                void                                frameworkTestabilityMessage5RequestHandler                    (FrameworkTestabilityMessage5 *pMessage);
                void                                frameworkTestabilityMessage6RequestHandler                    (FrameworkTestabilityMessage6 *pMessage);
                void                                frameworkTestabilityMessage7RequestHandler                    (FrameworkTestabilityMessage7 *pMessage);
                void                                frameworkTestabilityManagementInterfaceMessageLeakMessageRequestHandler (FrameworkTestabilityManagementInterfaceMessageLeakMessage *pMessage);
                void                                frameworkTestabilityManagementInterfaceObjectLeakMessageRequestHandler (FrameworkTestabilityManagementInterfaceObjectLeakMessage *pMessage);
                void                                frameworkTestabilityManagementInterfacFileServiceUsageRequestHandler(FrameworkTestabilityManagementInterfaceFileServiceUsageMessage  *pMessage);
    
        static  UI32                                getNumberOfProcessedMessage2s                                 ();
        static  void                                setNumberOfProcessedMessage2s                                 (const UI32 numberOfProcessedMessage7s);
        static  void                                incrementNumberOfProcessedMessage2s                           ();
        static  bool                                getCanProceedToProcessMessage2s                               ();
        static  void                                setCanProceedToProcessMessage2s                               (const bool &canProceedToProcessMessage2s);

        static  UI32                                getNumberOfProcessedMessage7s                                 ();
        static  void                                setNumberOfProcessedMessage7s                                 (const UI32 numberOfProcessedMessage7s);
        static  void                                incrementNumberOfProcessedMessage7s                           ();

        static  void                                setAreMessage6sReceived                                       (const bool &receivedMessage6s);
        static  bool                                getAndResetAreMessage6sReceived                               ();

                void                                frameworkTestabilityEvent1EventHandler                        (const FrameworkTestabilityEvent1 *&pEvent);

                void                                frameworkTestabilityManagementInterfaceMessage1RequestHandler (FrameworkTestabilityManagementInterfaceMessage1 *pMessage);
                void                                frameworkTestabilityManagementInterfaceMessage2RequestHandler (FrameworkTestabilityManagementInterfaceMessage2 *pMessage);
                void                                frameworkTestabilityManagementInterfaceMessage2RequestCallBack (FrameworkStatus frameworkStatus, ManagementInterfaceClientTestMessage2 *message, void *pcontext);
                void                                frameworkTestabilityManagementInterfaceMessage3RequestHandler (FrameworkTestabilityManagementInterfaceMessage3 *pMessage);
                void                                frameworkTestabilityManagementInterfaceMessage3RequestCallBack (FrameworkStatus frameworkStatus, ManagementInterfaceClientTestMessage3 *message, void *pcontext);
                void                                frameworkTestabilityManagementInterfaceTrackLeaksWithBtMessageRequestHandler (FrameworkTestabilityManagementInterfaceTrackLeaksWithBtMessage *pMessage);
                void                                frameworkTestabilityManagementInterfaceMessage4RequestHandler (FrameworkTestabilityManagementInterfaceMessage4 *pMessage);
                void                                sendToClientsWithBuffersPhase1AllSuccessTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void                                sendToClientsWithBuffersPhase1AllSuccessTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void                                sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		void                                sendToClientsWithBuffersPhase1AllSucesssButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void                                sendToClientsWithBuffersPhase1AllFailureTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		void                                sendToClientsWithBuffersPhase1AllFailureTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void                                sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		void                                sendToClientsWithBuffersPhase1AllFailureButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void                                sendToClientsWithBuffersPhase1SomeSuccessTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		void                                sendToClientsWithBuffersPhase1SomeSuccessTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);
		void                                sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
		void                                sendToClientsWithBuffersPhase1SomeSucesssButPhase2MessageSetTestCallback (WaveSendToClientsContext *pWaveSendToClientsContext);

    // Now the data members

     private :
                UI32                                m_messageSent;
                bool                                m_isStatusFailure;
     protected :
     public :
};

}

#endif // FRAMEWORKTESTABILITY1OBJECTMANAGER_H
