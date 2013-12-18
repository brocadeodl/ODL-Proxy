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

#ifndef WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGERCANCELREQUESTTOTRYFORCONNECTIONMESSAGE_H
#define WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGERCANCELREQUESTTOTRYFORCONNECTIONMESSAGE_H

#include "Framework/Messaging/Local/PrismMessage.h"

namespace WaveNs
{

class WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage : public PrismMessage
{
    private :
    protected :
    public :
                                WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage (const string &brokerName);
                                WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage (const vector<string> &brokerNames);
        virtual                ~WaveMessagingBrokerConnectionObjectManagerCancelRequestToTryForConnectionMessage ();

                vector<string>  getBrokerNames                                                                   () const;
                void            setBrokerNames                                                                   (const vector<string> &brokerNames);
                void            addBrokerNames                                                                   (const vector<string> &brokerNames);
                void            addBrokerName                                                                    (const string &brokerName);

    // Now the data members

    private :
        vector<string> m_brokerNames;

    protected :
    public :
};
}

#endif //WAVEMESSAGINGBROKERCONNECTIONOBJECTMANAGERCANCELREQUESTTOTRYFORCONNECTIONMESSAGE_H
