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

/**************************************************************************
*   Copyright (C) 2011 Brocade Communications Systems, Inc.               *
*   All rights reserved.                                                  *
*   Author : Mandar Datar                                                 *
***************************************************************************/

#ifndef VCSFABRICLOCALMESSAGES_H
#define VCSFABRICLOCALMESSAGES_H

#include "Framework/Messaging/Local/PrismMessage.h"
#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

typedef enum
{
    WYSEREA_ASYNC_DEMO_MESSAGE_ASYNC_REPLY_TEST = 0,
    WYSEREA_ASYNC_DEMO_MESSAGE_RECALL_TEST

} WyserEaAsyncDemoMessageCommandCode;

class WyserEaAsyncDemoMessage : public PrismMessage
{
    private :
        void            setupAttributesForSerialization  ();

    protected :
    public :
                        WyserEaAsyncDemoMessage (UI32 numberOfExtraReplies, UI32 delayBetweenReplies, UI32 messageNumber);
                        WyserEaAsyncDemoMessage (UI32 sleepDuration);
                        WyserEaAsyncDemoMessage ();
                       ~WyserEaAsyncDemoMessage ();

    // Now the data members

    public :

        UI32            m_commandCode;          // input
        
        // for m_commandCode as WYSEREA_ASYNC_DEMO_MESSAGE_ASYNC_REPLY_TEST :

        UI32            m_numberOfExtraReplies; // input
        UI32            m_delayBetweenReplies;  // input
        UI32            m_messageNumber;        // input
        string          m_demoString;           // output

        // for m_commandCode as WYSEREA_ASYNC_DEMO_MESSAGE_RECALL_TEST :

        UI32            m_sleepDuration;        // input
};

}
#endif // VCSFABRICLOCALMESSAGES_H
