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

#include "Framework/Utils/FixedSizeBuffer.h"
#include "Framework/Utils/TraceUtils.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

FixedSizeBuffer::FixedSizeBuffer (const UI32 maximumSize)
    : m_maximumSize (maximumSize),
      m_isBufferOwnedByMe (true)
{
    if (0 == m_maximumSize)
    {
		trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::FixedSizeBuffer : A FixedSizeBuffer of length 0 is not possible.");
		prismAssert (false, __FILE__,__LINE__);
	}

    m_pRawBuffer  = new UI8[m_maximumSize + 1];
    m_currentSize = 0;

    if (NULL == m_pRawBuffer)
    {
        trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::FixedSizeBuffer : Could not allocate a buffer.");
        prismAssert (false, __FILE__,__LINE__);
    }

    m_pRawBuffer[m_maximumSize] = '\0';
}

FixedSizeBuffer::FixedSizeBuffer (const UI32 maximumSize, void *pBuffer, bool isBufferUsed)
    : m_maximumSize (maximumSize),
      m_isBufferOwnedByMe (false)
{
    if (0 == m_maximumSize)
    {
		trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::FixedSizeBuffer : A FixedSizeBuffer of length 0 is not possible.");
		prismAssert (false, __FILE__,__LINE__);
	}

    m_pRawBuffer  = reinterpret_cast<UI8 *> (pBuffer);

    if (true == isBufferUsed)
    {
        m_currentSize = m_maximumSize;
    }
    else
    {
        m_currentSize = 0;
    }

    if (NULL == m_pRawBuffer)
    {
        trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::FixedSizeBuffer : Invalid buffer supplied");
        prismAssert (false, __FILE__,__LINE__);
    }
}

FixedSizeBuffer::~FixedSizeBuffer ()
{
    if (true == m_isBufferOwnedByMe)
    {
        if (NULL != m_pRawBuffer)
        {
            delete[] m_pRawBuffer;
        }
    }

    m_pRawBuffer  = NULL;
    m_currentSize = 0;
}

UI8 *FixedSizeBuffer::getPRawBuffer ()
{
    return (m_pRawBuffer);
}

UI8 *FixedSizeBuffer::getPRawBuffer () const
{
    return (m_pRawBuffer);
}

UI8 *FixedSizeBuffer::getPCurrentRawBuffer ()
{
    return (m_pRawBuffer + m_currentSize);
}

const UI32 FixedSizeBuffer::getMaximumSize () const
{
    return (m_maximumSize);
}

const UI32 FixedSizeBuffer::getCurrentSize () const
{
    return (m_currentSize);
}

const UI32 FixedSizeBuffer::getRemainingSize () const
{
    if (m_currentSize > m_maximumSize)
    {
        trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::getRemainingSize : Current size exceeded Maximum Size.  Something went wrong.");
        prismAssert (false, __FILE__,__LINE__);
    }

    if (m_currentSize <= m_maximumSize)
    {
        return (m_maximumSize - m_currentSize);
    }
    else
    {
        return (0);
    }
}

void FixedSizeBuffer::incrementCurrentSize (const UI32 incrementSize)
{
    UI32 remainingSize = m_maximumSize - m_currentSize;

    if (remainingSize < incrementSize)
    {
        trace (TRACE_LEVEL_ERROR, "FixedSizeBuffer::getRemainingSize : Increment Size causes Current size to exceeded Maximum Size.  Something went wrong.");
        trace (TRACE_LEVEL_FATAL, "FixedSizeBuffer::getRemainingSize : Not incrementing the current size.");
        prismAssert (false, __FILE__,__LINE__);
    }
    else
    {
        m_currentSize += incrementSize;
    }
}

bool FixedSizeBuffer::isFull ()
{
    return (m_maximumSize == m_currentSize);
}

const void FixedSizeBuffer::toString (string &resultString)
{
    m_pRawBuffer[m_currentSize] = '\0';

    resultString.assign ((const char *) m_pRawBuffer, m_currentSize);
}

void  FixedSizeBuffer::reset ()
{
    m_currentSize = 0;
}

UI8 *FixedSizeBuffer::transferBufferToUser (UI32 &currentSize, UI32 &maximumSize)
{
    if (false == m_isBufferOwnedByMe)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    m_isBufferOwnedByMe = false;

    currentSize = m_currentSize;
    maximumSize = m_maximumSize;

    return (m_pRawBuffer);
}

}

