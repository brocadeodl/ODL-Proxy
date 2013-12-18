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

#include "Framework/Core/DestroyClusterAsynchronousContext.h"

namespace WaveNs
{

DestroyClusterAsynchronousContext::DestroyClusterAsynchronousContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext)
    : PrismAsynchronousContext     (pCaller, pCallback, pCallerContext),
      m_reasonForDestroyingCluster (FRAMEWORK_OBJECT_MANAGER_FAILOVER_REASON_UNCONTROLLED),
      m_originalRequester          (0),
      m_isRebootRequired           (true),
      m_isPreparingForAddNode      (false)
{
}

DestroyClusterAsynchronousContext::~DestroyClusterAsynchronousContext ()
{
}

FrameworkObjectManagerFailoverReason DestroyClusterAsynchronousContext::getReasonForDestroyingCluster () const
{
    return (m_reasonForDestroyingCluster);
}

void DestroyClusterAsynchronousContext::setReasonForDestroyingCluster (const FrameworkObjectManagerFailoverReason &reasonForDestroyingCluster)
{
    m_reasonForDestroyingCluster = reasonForDestroyingCluster;
}

PrismServiceId DestroyClusterAsynchronousContext::getOriginalRequester () const
{
    return (m_originalRequester);
}

void DestroyClusterAsynchronousContext::setOriginalRequester (const PrismServiceId &originalRequester)
{
    m_originalRequester = originalRequester;
}

void DestroyClusterAsynchronousContext::setIsRebootRequired (const bool &isRebootRequired)
{
    m_isRebootRequired = isRebootRequired;
}

bool DestroyClusterAsynchronousContext::getIsRebootRequired () const
{
    return (m_isRebootRequired);
}

void DestroyClusterAsynchronousContext::setPreparingForAddNode (const bool &isPreparingForAddNode)
{
    m_isPreparingForAddNode = isPreparingForAddNode;
}

bool DestroyClusterAsynchronousContext::getIsPreparingForAddNode () const
{
    return (m_isPreparingForAddNode);
}

}
