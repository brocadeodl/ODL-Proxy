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
 *   Author : James Chen                                                   *
 ***************************************************************************/

#include "Framework/ObjectModel/WaveSendToClientsContext.h"
#include "Framework/Utils/FrameworkToolKit.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

WaveSendToClientsContext::WaveSendToClientsContext (PrismElement *pCaller, PrismAsynchronousCallback pCallback, void *pCallerContext, const string &clientName, bool isSubInstanceToBeIncluded)
    : PrismAsynchronousContext    (pCaller, pCallback, pCallerContext),
      m_pManagementInterfaceMessageForPhase1    (NULL),
      m_pManagementInterfaceMessageForPhase2    (NULL),
      m_timeoutForPhase1          (0),
      m_timeoutForPhase2          (0),
      m_numberOfFailuresForPhase1 (0),   
      m_numberOfFailuresForPhase2 (0),
      m_instancesToSendToForPhase1(0),
      m_subInstancesToSendToForPhase1(0),
      m_clientName                (clientName),
      m_isSubInstanceToBeIncluded (isSubInstanceToBeIncluded)
{
    if (!m_isSubInstanceToBeIncluded)
    {
        FrameworkToolKit::getConnectedInstances (m_instancesToSendToForPhase1);
    }
    else
    {
        FrameworkToolKit::getConnectedInstancesAndSubInstances (m_instancesToSendToForPhase1, m_subInstancesToSendToForPhase1);
    }
}

WaveSendToClientsContext::~WaveSendToClientsContext ()
{
    map<SI32, ManagementInterfaceMessage *>::iterator       element    = m_resultingMessageForPhase1.begin ();
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement = m_resultingMessageForPhase1.end ();

    while (endElement != element)
    {
        delete (element->second);
        element++;
    }

    map<SI32, ManagementInterfaceMessage *>::iterator       element2    = m_resultingMessageForPhase2.begin ();
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement2 = m_resultingMessageForPhase2.end ();

    while (endElement2 != element2)
    {
        delete (element2->second);
        element2++;
    }
}

ManagementInterfaceMessage *WaveSendToClientsContext::getPManagementInterfaceMessageForPhase1 () const
{
    return (m_pManagementInterfaceMessageForPhase1);
}

void WaveSendToClientsContext::setPManagementInterfaceMessageForPhase1 (ManagementInterfaceMessage *pManagementInterfaceMessageForPhase1)
{
    m_pManagementInterfaceMessageForPhase1 = pManagementInterfaceMessageForPhase1;
}

ManagementInterfaceMessage *WaveSendToClientsContext::getPManagementInterfaceMessageForPhase2 () const
{
    return (m_pManagementInterfaceMessageForPhase2);
}

void WaveSendToClientsContext::setPManagementInterfaceMessageForPhase2 (ManagementInterfaceMessage *pManagementInterfaceMessageForPhase2)
{
    m_pManagementInterfaceMessageForPhase2 = pManagementInterfaceMessageForPhase2;
}

UI32 WaveSendToClientsContext::getTimeoutForPhase1 () const
{
    return (m_timeoutForPhase1);
}

void WaveSendToClientsContext::setTimeoutForPhase1 (const UI32 &timeoutForPhase1)
{
    m_timeoutForPhase1 = timeoutForPhase1;
}

UI32 WaveSendToClientsContext::getTimeoutForPhase2 () const
{
    return (m_timeoutForPhase2);
}

void WaveSendToClientsContext::setTimeoutForPhase2 (const UI32 &timeoutForPhase2)
{
    m_timeoutForPhase2 = timeoutForPhase2;
}

vector<SI32> WaveSendToClientsContext::getInstancesToSendToForPhase1 () const
{
    return (m_instancesToSendToForPhase1);
}

void WaveSendToClientsContext::setInstancesToSendToForPhase1 (const vector<SI32> &instanceToSendToForPhase1)
{
    m_instancesToSendToForPhase1 = instanceToSendToForPhase1;
}

vector<SI32> WaveSendToClientsContext::getSubInstancesToSendToForPhase1 () const
{
    return (m_subInstancesToSendToForPhase1);
}

void WaveSendToClientsContext::setSubInstancesToSendToForPhase1 (const vector<SI32> &subInstanceToSendToForPhase1)
{
    m_subInstancesToSendToForPhase1 = subInstanceToSendToForPhase1;
}

vector<SI32> WaveSendToClientsContext::getSubInstancesToSendToForPhase2 () const
{
    return (m_subInstancesToSendToForPhase2);
}

void WaveSendToClientsContext::setSubInstancesToSendToForPhase2 (const vector<SI32> &subInstanceToSendToForPhase2)
{
    m_subInstancesToSendToForPhase2 = subInstanceToSendToForPhase2;
}

void WaveSendToClientsContext::setIsSubInstanceToBeIncluded (bool isSubInstanceToBeIncluded)
{
    m_isSubInstanceToBeIncluded = isSubInstanceToBeIncluded;
}

bool WaveSendToClientsContext::getIsSubInstanceToBeIncluded () const
{
    return (m_isSubInstanceToBeIncluded);
}

WaveMessageStatus WaveSendToClientsContext::getSendStatusForPhase1 (const SI32 &instance)
{
    map<SI32, WaveMessageStatus>::const_iterator element    = m_sendStatusForPhase1.find (instance);
    map<SI32, WaveMessageStatus>::const_iterator endElement = m_sendStatusForPhase1.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_sendStatusForPhase1[instance]);
}

void WaveSendToClientsContext::setSendStatusForPhase1 (const SI32 &instance, const WaveMessageStatus &sendStatusForPhase1)
{
    map<SI32, WaveMessageStatus>::const_iterator element    = m_sendStatusForPhase1.find (instance);
    map<SI32, WaveMessageStatus>::const_iterator endElement = m_sendStatusForPhase1.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_sendStatusForPhase1[instance] = sendStatusForPhase1;
}

FrameworkStatus WaveSendToClientsContext::getFrameworkStatusForPhase1 (const SI32 &instance)
{
    map<SI32, FrameworkStatus>::const_iterator element    = m_frameworkStatusForPhase1.find (instance);
    map<SI32, FrameworkStatus>::const_iterator endElement = m_frameworkStatusForPhase1.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_frameworkStatusForPhase1[instance]);
}

void WaveSendToClientsContext::setFrameworkStatusForPhase1 (const SI32 &instance, const FrameworkStatus frameworkStatusForPhase1)
{
    map<SI32, FrameworkStatus>::const_iterator element    = m_frameworkStatusForPhase1.find (instance);
    map<SI32, FrameworkStatus>::const_iterator endElement = m_frameworkStatusForPhase1.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_frameworkStatusForPhase1[instance] = frameworkStatusForPhase1;
}

ResourceId WaveSendToClientsContext::getCompletionStatusForPhase1 (const SI32 &instance)
{
    map<SI32, ResourceId>::const_iterator element    = m_completionStatusForPhase1.find (instance);
    map<SI32, ResourceId>::const_iterator endElement = m_completionStatusForPhase1.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_completionStatusForPhase1[instance]);
}

void WaveSendToClientsContext::setCompletionStatusForPhase1 (const SI32 &instance, const ResourceId &completionStatusForPhase1)
{
    map<SI32, ResourceId>::const_iterator element    = m_completionStatusForPhase1.find (instance);
    map<SI32, ResourceId>::const_iterator endElement = m_completionStatusForPhase1.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_completionStatusForPhase1[instance] = completionStatusForPhase1;
}

ManagementInterfaceMessage *WaveSendToClientsContext::getResultingMessageForPhase1 (const SI32 &instance)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator element    = m_resultingMessageForPhase1.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement = m_resultingMessageForPhase1.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_resultingMessageForPhase1[instance]);
}

void WaveSendToClientsContext::setResultingMessageForPhase1 (const SI32 &instance, ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator element    = m_resultingMessageForPhase1.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement = m_resultingMessageForPhase1.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_resultingMessageForPhase1[instance] = pManagementInterfaceMessage;
}

ManagementInterfaceMessage *WaveSendToClientsContext::transferResultingMessageForPhase1 (const SI32 &instance)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator  element       = m_resultingMessageForPhase1.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator  endElement    = m_resultingMessageForPhase1.end ();
    ManagementInterfaceMessage                                    *pManagementInterfaceMessage = NULL;

    prismAssert (endElement != element, __FILE__, __LINE__);

    pManagementInterfaceMessage = m_resultingMessageForPhase1[instance];

   m_resultingMessageForPhase1.erase (instance);

    return (pManagementInterfaceMessage);
}

UI32 WaveSendToClientsContext::getNumberOfFailuresForPhase1 () const
{
    return (m_numberOfFailuresForPhase1);
}

void WaveSendToClientsContext::setNumberOfFailuresForPhase1 (const UI32 &numberOfFailuresForPhase1)
{
    m_numberOfFailuresForPhase1 = numberOfFailuresForPhase1;
}

vector<SI32> WaveSendToClientsContext::getInstancesToSendToForPhase2 () const
{
    return (m_instancesToSendToForPhase2);
}

void WaveSendToClientsContext::setInstancesToSendToForPhase2 (const vector<SI32> &instanceToSendToForPhase2)
{
    m_instancesToSendToForPhase2 = instanceToSendToForPhase2;
}

WaveMessageStatus WaveSendToClientsContext::getSendStatusForPhase2 (const SI32 &instance)
{
    map<SI32, WaveMessageStatus>::const_iterator element    = m_sendStatusForPhase2.find (instance);
    map<SI32, WaveMessageStatus>::const_iterator endElement = m_sendStatusForPhase2.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_sendStatusForPhase2[instance]);
}

void WaveSendToClientsContext::setSendStatusForPhase2 (const SI32 &instance, const WaveMessageStatus &sendStatusForPhase2)
{
    map<SI32, WaveMessageStatus>::const_iterator element    = m_sendStatusForPhase2.find (instance);
    map<SI32, WaveMessageStatus>::const_iterator endElement = m_sendStatusForPhase2.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_sendStatusForPhase2[instance] = sendStatusForPhase2;
}

FrameworkStatus WaveSendToClientsContext::getFrameworkStatusForPhase2 (const SI32 &instance)
{
    map<SI32, FrameworkStatus>::const_iterator element    = m_frameworkStatusForPhase2.find (instance);
    map<SI32, FrameworkStatus>::const_iterator endElement = m_frameworkStatusForPhase2.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_frameworkStatusForPhase2[instance]);
}

void WaveSendToClientsContext::setFrameworkStatusForPhase2 (const SI32 &instance, const FrameworkStatus frameworkStatusForPhase2)
{
    map<SI32, FrameworkStatus>::const_iterator element    = m_frameworkStatusForPhase2.find (instance);
    map<SI32, FrameworkStatus>::const_iterator endElement = m_frameworkStatusForPhase2.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_frameworkStatusForPhase2[instance] = frameworkStatusForPhase2;
}

ResourceId WaveSendToClientsContext::getCompletionStatusForPhase2 (const SI32 &instance)
{
    map<SI32, ResourceId>::const_iterator element    = m_completionStatusForPhase2.find (instance);
    map<SI32, ResourceId>::const_iterator endElement = m_completionStatusForPhase2.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_completionStatusForPhase2[instance]);
}

void WaveSendToClientsContext::setCompletionStatusForPhase2 (const SI32 &instance, const ResourceId &completionStatusForPhase2)
{
    map<SI32, ResourceId>::const_iterator element    = m_completionStatusForPhase2.find (instance);
    map<SI32, ResourceId>::const_iterator endElement = m_completionStatusForPhase2.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_completionStatusForPhase2[instance] = completionStatusForPhase2;
}

ManagementInterfaceMessage *WaveSendToClientsContext::getResultingMessageForPhase2 (const SI32 &instance)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator element    = m_resultingMessageForPhase2.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement = m_resultingMessageForPhase2.end ();

    prismAssert (endElement != element, __FILE__, __LINE__);

    return (m_resultingMessageForPhase2[instance]);
}

void WaveSendToClientsContext::setResultingMessageForPhase2 (const SI32 &instance, ManagementInterfaceMessage *pManagementInterfaceMessage)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator element    = m_resultingMessageForPhase2.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator endElement = m_resultingMessageForPhase2.end ();

    prismAssert (endElement == element, __FILE__, __LINE__);

    m_resultingMessageForPhase2[instance] = pManagementInterfaceMessage;
}

ManagementInterfaceMessage *WaveSendToClientsContext::transferResultingMessageForPhase2 (const SI32 &instance)
{
    map<SI32, ManagementInterfaceMessage *>::const_iterator  element       = m_resultingMessageForPhase2.find (instance);
    map<SI32, ManagementInterfaceMessage *>::const_iterator  endElement    = m_resultingMessageForPhase2.end ();
    ManagementInterfaceMessage         *pManagementInterfaceMessage = NULL;

    prismAssert (endElement != element, __FILE__, __LINE__);

    pManagementInterfaceMessage = m_resultingMessageForPhase2[instance];

    m_resultingMessageForPhase2.erase (instance);

    return (pManagementInterfaceMessage);
}

UI32 WaveSendToClientsContext::getNumberOfFailuresForPhase2 () const
{
    return (m_numberOfFailuresForPhase2);
}

void WaveSendToClientsContext::setNumberOfFailuresForPhase2 (const UI32 &numberOfFailuresForPhase2)
{
    m_numberOfFailuresForPhase2 = numberOfFailuresForPhase2;
}

string WaveSendToClientsContext::getClientName () const
{
    return (m_clientName);
}

void WaveSendToClientsContext::setClientName (const string &clientName)
{
    m_clientName = clientName;
}

}
