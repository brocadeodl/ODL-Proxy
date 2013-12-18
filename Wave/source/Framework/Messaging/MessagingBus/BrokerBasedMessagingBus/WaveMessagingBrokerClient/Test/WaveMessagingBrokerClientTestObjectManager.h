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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMESSAGINGBROKERCLIENTTESTOBJECTMANAGER_H
#define WAVEMESSAGINGBROKERCLIENTTESTOBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Regression/PrismTestObjectManager.h"
#include "Regression/RegressionTestMessage.h"

namespace WaveNs
{

class PrismLinearSequencerContext;
class WaveBrokerPublishMessage;

class WaveMessagingBrokerClientTestObjectManager : public PrismTestObjectManager
{
    private :
        virtual void testRequestHandler                       (RegressionTestMessage *pRegressionTestMessage);

                void simpleWaveBrokerConnectTestStep          (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void simpleWaveBrokerSubscribeMessageTestStep (PrismLinearSequencerContext *pPrismLinearSequencerContext);
                void simpleWaveBrokerPublishMessageTestStep   (PrismLinearSequencerContext *pPrismLinearSequencerContext);

                void simpleSubscriptionCallback               (const WaveBrokerPublishMessage * const pWaveBrokerPublishMessage);

    protected :
    public :
                                                            WaveMessagingBrokerClientTestObjectManager ();
        virtual                                            ~WaveMessagingBrokerClientTestObjectManager ();

        static  string                                      getServiceName                             ();
        static  PrismServiceId                              getPrismServiceId                          ();
        static  WaveMessagingBrokerClientTestObjectManager *getInstance                                ();

    // Now the data members

    private :
    protected :
    public :
};

}

#endif // WAVEMESSAGINGBROKERCLIENTTESTOBJECTMANAGER_H
