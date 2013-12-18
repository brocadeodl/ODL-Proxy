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
 *   Copyright (C) 2005-2010 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "WaveCliReplyThread.h"
#include "Framework/Trace/TraceObjectManager.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"
#include "ManagementInterface/ClientInterface/WaveUserInterfaceObjectManager.h"

namespace WaveNs
{

WaveCliReplyThread::WaveCliReplyThread (ManagementInterfaceMessage *pManagementInterfaceMessage)
    : PrismPosixThread (),
      m_pManagementInterfaceMessage (pManagementInterfaceMessage)
{
}

WaveThreadStatus WaveCliReplyThread::start ()
{
    trace (TRACE_LEVEL_DEBUG, string ("WaveCliReplyThread::start \n\r"));

    UI32 noOfReply = 1000;
    UI32 i;
    bool lastReply;

    for (i = 0; i < noOfReply - 1; i++)
    {
        lastReply = false;
        m_pManagementInterfaceMessage->setIsLastReply (lastReply);
        WaveUserInterfaceObjectManager::replyToWaveServer (m_pManagementInterfaceMessage);
    }

    lastReply = true;

    m_pManagementInterfaceMessage->setCompletionStatus (WAVE_MESSAGE_SUCCESS);
    m_pManagementInterfaceMessage->setIsLastReply (lastReply);
    WaveUserInterfaceObjectManager::replyToWaveServer (m_pManagementInterfaceMessage);

    delete this;

    return (WAVE_THREAD_SUCCESS);

}

}

