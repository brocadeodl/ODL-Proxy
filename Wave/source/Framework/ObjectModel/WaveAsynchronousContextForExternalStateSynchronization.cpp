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
 *   Author : Jayanth Venkataraman                                         *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveAsynchronousContextForExternalStateSynchronization.h"

namespace WaveNs
{

WaveAsynchronousContextForExternalStateSynchronization::WaveAsynchronousContextForExternalStateSynchronization (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext (pCaller, pCallback, pCallerContext),
      m_fssStageNumber (0),
      m_serviceType (0)
{
}

WaveAsynchronousContextForExternalStateSynchronization::~WaveAsynchronousContextForExternalStateSynchronization ()
{
}

UI32 WaveAsynchronousContextForExternalStateSynchronization::getFssStageNumber () const
{
    return (m_fssStageNumber);
}

void WaveAsynchronousContextForExternalStateSynchronization::setFssStageNumber (const UI32 fssStageNumber)
{
    m_fssStageNumber = fssStageNumber;
}

ResourceId WaveAsynchronousContextForExternalStateSynchronization::getServiceType () const
{
    return (m_serviceType);
}

void WaveAsynchronousContextForExternalStateSynchronization::setServiceType (const ResourceId serviceType)
{
    m_serviceType = serviceType;
}

}
