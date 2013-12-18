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

#ifndef FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER2_H
#define FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER2_H

#include "Regression/PrismTestObjectManager.h"

namespace WaveNs
{

class FrameworkTestabilityMessage1;
class FrameworkTestabilityMessage3;
class FrameworkTestabilityMessage4;
class FrameworkLocalMessagingTestContext;

class FrameworkRemoteMessagingTestObjectManager2 : public PrismTestObjectManager
{
    private :
        FrameworkRemoteMessagingTestObjectManager2 ();

    protected :
    public :
        virtual                                           ~FrameworkRemoteMessagingTestObjectManager2   ();
        static  FrameworkRemoteMessagingTestObjectManager2 *getInstance                                 ();
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

    // Now the data members

    private :
        time_t  m_startTime;
        time_t  m_endTime;
        UI32    m_numberOfMessagesToSend;
    protected :
    public :
};

}

#endif // FRAMEWORKREMOTEMESSAGINGTESTOBJECTMANAGER2_H
