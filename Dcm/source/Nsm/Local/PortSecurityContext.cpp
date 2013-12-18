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
 *   Copyright (C) 2005-2007 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : skrastog                                    				   *
 ***************************************************************************/

#include "Nsm/Local/PortSecurityContext.h"

namespace WaveNs
{

PortSecurityContext::PortSecurityContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_buffer       (NULL),
      m_bufSize 	 (0)
{
}

PortSecurityContext::PortSecurityContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismSynchronousLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismSynchronousLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
      m_buffer       (NULL),
      m_bufSize 	 (0)
{
}

PortSecurityContext::~PortSecurityContext ()
{
	/* TODO, write the code to delete the buffer in this function */
}

void PortSecurityContext::addBuffer(void *data)
{
	m_buffer = data;
}

void *PortSecurityContext::getBuffer() const
{
	return m_buffer;
}

void PortSecurityContext::setBufferSize(const UI32 &size)
{
	m_bufSize = size;
}

UI32 PortSecurityContext::getBufferSize() const
{
	return m_bufSize;
}

}

