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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/MultiThreading/PrismMessageQueue.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Core/PrismFrameworkMessages.h"

using namespace std;

namespace WaveNs
{

template<class T> PrismMessageQueue<T>::PrismMessageQueue ()
{
}

template<class T> PrismMessageQueue<T>::~PrismMessageQueue ()
{
}

template<class T> void PrismMessageQueue<T>::insertAtTheBack (T *&pPrismMessage)
{
    m_prismMutex.lock ();
    m_messageQueue.push_back (pPrismMessage);
    m_prismMutex.unlock ();
}

template<class T> void PrismMessageQueue<T>::insertAtTheFront (T *&pPrismMessage)
{
    m_prismMutex.lock ();
    m_messageQueue.push_front (pPrismMessage);
    m_prismMutex.unlock ();
}

template<class T> void PrismMessageQueue<T>::removeFromFront ()
{
    m_prismMutex.lock ();
    m_messageQueue.pop_front ();
    m_prismMutex.unlock ();
}

template<class T> WaveMessageStatus PrismMessageQueue<T>::remove (T *&pPrismMessage)
{
    m_prismMutex.lock ();

    WaveMessageStatus                   status  = WAVE_MESSAGE_ERROR_UNKNOWN_MESSAGE;
    std::deque<PrismMessage *>::iterator element = m_messageQueue.begin ();
    std::deque<PrismMessage *>::iterator end     = m_messageQueue.end ();

    while (element != end)
    {
        if (pPrismMessage == (*element))
        {
            m_messageQueue.erase (element);
            status = WAVE_MESSAGE_SUCCESS;
            break;
        }

        element++;
    }

    m_prismMutex.unlock ();

    return (status);
}

template<class T> UI32 PrismMessageQueue<T>::removeTimerExpirationsForTimer (const TimerHandle &timerHandle)
{
    m_prismMutex.lock ();

    std::deque<PrismMessage *>::iterator   element                                = m_messageQueue.begin ();
    std::deque<PrismMessage *>::iterator   end                                    = m_messageQueue.end ();
    PrismTimerExpiredObjectManagerMessage *pPrismTimerExpiredObjectManagerMessage = NULL;
    std::deque<PrismMessage *>             tempMessageQueue;
    UI32                                   numberOfExpirationsEncountered         = 0;

    while (element != end)
    {
        pPrismTimerExpiredObjectManagerMessage = dynamic_cast<PrismTimerExpiredObjectManagerMessage *> (*element);

        prismAssert (NULL != pPrismTimerExpiredObjectManagerMessage, __FILE__, __LINE__);

        if ((pPrismTimerExpiredObjectManagerMessage->getTimerId ()) == timerHandle)
        {
            numberOfExpirationsEncountered++;
            delete pPrismTimerExpiredObjectManagerMessage;
        }
        else
        {
            tempMessageQueue.push_back (pPrismTimerExpiredObjectManagerMessage);
        }

        element++;
    }

    m_messageQueue.clear ();

    element = tempMessageQueue.begin ();
    end     = tempMessageQueue.end ();

    while (element != end)
    {
        m_messageQueue.push_back (*element);
        element++;
    }

    tempMessageQueue.clear ();

    m_prismMutex.unlock ();

    return (numberOfExpirationsEncountered);
}

template<class T> T *PrismMessageQueue<T>::getFromFront ()
{
    m_prismMutex.lock ();

    if (true == (m_messageQueue.empty ()))
    {
        m_prismMutex.unlock ();

        return (NULL);
    }
    else
    {
        T *pTemp = m_messageQueue.front ();

        m_prismMutex.unlock ();

        return (pTemp);
    }
}

template<class T> T *PrismMessageQueue<T>::removeAndGetFromFront ()
{
    m_prismMutex.lock ();

    if (true == (m_messageQueue.empty ()))
    {
        m_prismMutex.unlock ();

        return (NULL);
    }
    else
    {
        T *pTemp = m_messageQueue.front ();

        m_messageQueue.pop_front ();

        m_prismMutex.unlock ();

        return (pTemp);
    }
}

template<class T> UI32 PrismMessageQueue<T>::getSize ()
{
    UI32 numberOfEntriesInTheMessageQueue = 0;

    m_prismMutex.lock ();
    numberOfEntriesInTheMessageQueue = m_messageQueue.size ();
    m_prismMutex.unlock ();

    return (numberOfEntriesInTheMessageQueue);
}

template<class T> bool PrismMessageQueue<T>::isEmpty ()
{
    bool isMessageQueueEmpty = true;

    m_prismMutex.lock ();
    isMessageQueueEmpty = m_messageQueue.empty ();
    m_prismMutex.unlock ();

    return (isMessageQueueEmpty);
}

}
