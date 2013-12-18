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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Mandar Datar                                                 *
 ***************************************************************************/

#ifndef MESSAGEHISTORY
#define MESSAGEHISTORY

#include "Framework/Types/Types.h"
#include <string>
#include <time.h>

using namespace std;

namespace WaveNs
{
class PrismMessage;
class MessageHistory {

    private :
    protected :
    public :
                MessageHistory              (PrismMessage *pPrismMessage, WaveMessageSendType sendType, WaveMessageHistoryLogType messageHistoryLogType);
                ~MessageHistory             ();
        void    getMessageHistoryString     (string &debugHistoryString);

       // Now the data members

    private :
        UI32                        m_originalMessageId;
        WaveMessageType             m_type;
        WaveMessageSendType         m_sendType;
        LocationId                  m_destinationLocationID;
        PrismServiceId              m_destinationServiceId;
        ResourceId                  m_opcode;
        ResourceId                  m_completionStatus;
        time_t                      m_timeInSeconds;
        UI32                        m_microSeconds;
        WaveMessageHistoryLogType   m_messageHistoryLogType;
    protected :
    public :

};

}

#endif //MESSAGEHISTORY

