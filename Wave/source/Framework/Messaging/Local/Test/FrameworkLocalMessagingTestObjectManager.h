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

#ifndef FRAMEWORKLOCALMESSAGINGTESTOBJECTMANAGER_H
#define FRAMEWORKLOCALMESSAGINGTESTOBJECTMANAGER_H

#include "Regression/PrismTestObjectManager.h"

namespace WaveNs
{

class FrameworkTestabilityMessage1;
class FrameworkTestabilityMessage5;
class FrameworkTestabilityMessage6;
class FrameworkLocalMessagingTestContext;

class FrameworkLocalMessagingTestObjectManager : public PrismTestObjectManager
{
    private :
        FrameworkLocalMessagingTestObjectManager ();
    protected :
    public :
        virtual                                          ~FrameworkLocalMessagingTestObjectManager ();
        static  FrameworkLocalMessagingTestObjectManager *getInstance                              ();
        static  PrismServiceId                            getPrismServiceId                        ();
                void                                      testRequestHandler                       (RegressionTestMessage *pMessage);
                void                                      simpleAsynchronousMessageTestStep        (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      frameworkTestabilityMessage1Callback     (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage1 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleOneWayMessageTestStep              (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleSynchronousMessageTestStep         (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleRecallMessageTestStep              (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleRecallFromFrontMessageTestStep     (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleRecallFromBackMessageTestStep      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      frameworkTestabilityMessage5Callback     (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage5 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      frameworkTestabilityMessage6Callback     (FrameworkStatus frameworkStatus, FrameworkTestabilityMessage6 *pMessage, FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);
                void                                      simpleEventTestStep                      (FrameworkLocalMessagingTestContext *pFrameworkLocalMessagingTestContext);

                PrismEvent                               *createEventInstance                      (const UI32 &eventOperationCode);

    // Now the data members

    private :
        static FrameworkLocalMessagingTestObjectManager *m_pFrameworkLocalMessagingTestObjectManager;

    protected :
    public:
};

}

#endif // FRAMEWORKLOCALMESSAGINGTESTOBJECTMANAGER_H
