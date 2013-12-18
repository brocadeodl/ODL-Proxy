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
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "Framework/Failover/FailoverAsynchronousContext.h"

namespace WaveNs
{

FailoverAsynchronousContext::FailoverAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext, FrameworkObjectManagerFailoverReason failoverReason, vector<LocationId> &failedLocationIds, bool isPrincipalChangedWithThisFailover)
    : PrismAsynchronousContext              (pCaller, pCallback, pCallerContext),
      m_failoverReason                      (failoverReason),
      m_failedLocationIds                   (failedLocationIds),
      m_isPrincipalChangedWithThisFailover  (isPrincipalChangedWithThisFailover)
{
}

FailoverAsynchronousContext::~FailoverAsynchronousContext ()
{
}

FrameworkObjectManagerFailoverReason FailoverAsynchronousContext::getfailoverReason () const
{
    return m_failoverReason;
}

vector<LocationId> FailoverAsynchronousContext::getfailedLocationIds () const
{
    return m_failedLocationIds;
}

bool FailoverAsynchronousContext::getIsPrincipalChangedWithThisFailover () const
{
    return m_isPrincipalChangedWithThisFailover;
}

}
