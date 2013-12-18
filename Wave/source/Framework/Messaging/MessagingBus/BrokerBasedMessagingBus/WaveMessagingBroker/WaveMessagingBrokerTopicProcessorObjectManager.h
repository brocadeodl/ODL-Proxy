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

#ifndef WAVEMESSAGINGBROKERTOPICPROCESSOROBJECTMANAGER_H
#define WAVEMESSAGINGBROKERTOPICPROCESSOROBJECTMANAGER_H

#include "Framework/ObjectModel/WaveLocalObjectManager.h"
#include "Framework/Utils/PrismMutex.h"

namespace WaveNs
{

class WaveMessagingBrokerTopicProcessorPublishWorker;

class WaveMessagingBrokerTopicProcessorObjectManager : public WaveLocalObjectManager
{
    private :
        WaveMessagingBrokerTopicProcessorObjectManager (const string &topicName);

    protected :
    public :
        virtual                                                ~WaveMessagingBrokerTopicProcessorObjectManager ();

        static  WaveMessagingBrokerTopicProcessorObjectManager *createInstance                                 (const string &topicName);
        static  string                                          getServiceNamePrefix                           ();

                string                                          getTopicName                                   () const;

        static PrismServiceId                                   getPrismServiceIdByTopicName                   (const string &topicName);

    // Now the data members

    private :
               string                                          m_topicName;

               WaveMessagingBrokerTopicProcessorPublishWorker *m_pWaveMessagingBrokerTopicProcessorPublishWorker;

        static map<string, PrismServiceId>                     m_topicNameToPrismServiceIdMap;
        static PrismMutex                                      m_topicNameToPrismServiceIdMapMutex;

    protected :
    public :

    friend class WaveMessagingBrokerTopic;
};

}

#endif // WAVEMESSAGINGBROKERTOPICPROCESSOROBJECTMANAGER_H
