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
 *   Copyright (C) 2005-2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTPOINTER_CPP
#define WAVEMANAGEDOBJECTPOINTER_CPP

#include "Framework/ObjectModel/WaveManagedObjectPointer.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/StringUtils.h"

namespace WaveNs
{

template<class T> template<class TX> WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::WaveManagedObjectCountedPointer (TX *pTX)
    : m_pTX            (pTX),
      m_referenceCount (1)
{
}

template<class T> template<class TX> WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::~WaveManagedObjectCountedPointer ()
{
    if (0 != m_referenceCount)
    {
        WaveNs::trace (TRACE_LEVEL_FATAL, string ("WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX> : Trying to Delete the Pointer when there are references pending.  Reference Count : ") + m_referenceCount);
        WaveNs::prismAssert (false, __FILE__, __LINE__);
    }

    delete m_pTX;
}

template<class T> template<class TX> void WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::operator ++ (int)
{
    m_referenceCount++;
}

template<class T> template<class TX> void WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::operator -- (int)
{
    if (0 == m_referenceCount)
    {
        trace (TRACE_LEVEL_FATAL, "WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::operator -- : Trying to decrement reference count beyond 0");
        prismAssert (false, __FILE__, __LINE__);
    }

    m_referenceCount--;
}

template<class T> template<class TX> UI32 WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::getReferenceCount () const
{
    return (m_referenceCount);
}

template<class T> template<class TX> TX *WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::operator -> ()
{
    return (m_pTX);
}

template<class T> template<class TX> bool WaveManagedObjectPointer<T>::WaveManagedObjectCountedPointer<TX>::operator == (const WaveManagedObjectCountedPointer<TX> &rhs) const
{
    if ((NULL != m_pTX) && (NULL != rhs.m_pTX))
    {
        return ((*m_pTX) == (*(rhs.m_pTX)));
    }
    else
    {
        if ((NULL == m_pTX) && (NULL == rhs.m_pTX))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
}

template<class T> WaveManagedObjectPointer<T>::WaveManagedObjectPointer ()
    : m_pWaveManagedObjectCountedPointer (NULL)
{
}

template<class T> WaveManagedObjectPointer<T>::WaveManagedObjectPointer (const WaveManagedObjectPointer<T> &rhs)
    : m_pWaveManagedObjectCountedPointer (NULL)
{
    destroy ();

    m_pWaveManagedObjectCountedPointer = rhs.m_pWaveManagedObjectCountedPointer;

    if (NULL != m_pWaveManagedObjectCountedPointer)
    {
        (*m_pWaveManagedObjectCountedPointer)++;
    }
}

template<class T> WaveManagedObjectPointer<T>::WaveManagedObjectPointer (T *pT)
{
    m_pWaveManagedObjectCountedPointer = new WaveManagedObjectCountedPointer<T> (pT);

    prismAssert (NULL != m_pWaveManagedObjectCountedPointer, __FILE__, __LINE__);
}

template<class T> void WaveManagedObjectPointer<T>::destroy ()
{
    if (NULL != m_pWaveManagedObjectCountedPointer)
    {
        (*m_pWaveManagedObjectCountedPointer)--;

        if (0 == (m_pWaveManagedObjectCountedPointer->getReferenceCount ()))
        {
            delete m_pWaveManagedObjectCountedPointer;

            m_pWaveManagedObjectCountedPointer = NULL;
        }
    }
}

template<class T> WaveManagedObjectPointer<T>::~WaveManagedObjectPointer ()
{
    destroy ();
}

template<class T> T *WaveManagedObjectPointer<T>::operator -> ()
{
    if (NULL != m_pWaveManagedObjectCountedPointer)
    {
        return ((*m_pWaveManagedObjectCountedPointer).operator -> ());
    }
    else
    {
        return (NULL);
    }
}

template<class T> WaveManagedObjectPointer<T> &WaveManagedObjectPointer<T>::operator = (const WaveManagedObjectPointer<T> &rhs)
{
    destroy ();

    m_pWaveManagedObjectCountedPointer = rhs.m_pWaveManagedObjectCountedPointer;

    if (NULL != m_pWaveManagedObjectCountedPointer)
    {
        (*m_pWaveManagedObjectCountedPointer)++;
    }

    return (*this);
}

template<class T> bool WaveManagedObjectPointer<T>::operator == (const WaveManagedObjectPointer<T> &rhs) const
{
    if ((NULL != m_pWaveManagedObjectCountedPointer) && (NULL != rhs.m_pWaveManagedObjectCountedPointer))
    {
        return ((*m_pWaveManagedObjectCountedPointer) == (*(rhs.m_pWaveManagedObjectCountedPointer)));
    }
    else
    {
        if ((NULL == m_pWaveManagedObjectCountedPointer) && (NULL == rhs.m_pWaveManagedObjectCountedPointer))
        {
            return (true);
        }
        else
        {
            return (false);
        }
    }
}

template<class T> bool WaveManagedObjectPointer<T>::operator != (const WaveManagedObjectPointer<T> &rhs) const
{
    return (! (operator == (rhs)));
}

}

#endif // WAVEMANAGEDOBJECTPOINTER_CPP

