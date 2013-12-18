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

#ifndef WAVEBROKERPUBLISHMESSAGEFACTORY_H
#define WAVEBROKERPUBLISHMESSAGEFACTORY_H

#include "Framework/Types/Types.h"
#include "Framework/Utils/PrismMutex.h"

#include <map>

using namespace std;

namespace WaveNs
{

class WaveBrokerPublishMessage;

typedef WaveBrokerPublishMessage *(* WaveBrokerPublishMessageInstantiator) ();

class WaveBrokerPublishMessageFactory
{
    private :
    protected :
    public :
                                         WaveBrokerPublishMessageFactory              ();
                                        ~WaveBrokerPublishMessageFactory              ();

        static void                      registerWaveBrokerPublishMessageInstantiator (const string &topicName, WaveBrokerPublishMessageInstantiator waveBrokerPublishMessageInstantiator);
        static WaveBrokerPublishMessage *createWaveBrokerPublishMessageInstance       (const string &topicName);

    // Now the data members

    private :
        static map<string, WaveBrokerPublishMessageInstantiator> m_waveBrokerPublishMessageInstatiators;
        static PrismMutex                                        m_waveBrokerPublishMessageInstatiatorsMutex;

    protected :
    public :
};

}

#endif // WAVEBROKERPUBLISHMESSAGEFACTORY_H
