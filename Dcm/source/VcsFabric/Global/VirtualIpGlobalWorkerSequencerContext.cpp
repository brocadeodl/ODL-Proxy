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

/**
 *@file VirtualIpGlobalWorkerSequencerContext.cpp
 * Copyright (C) 2012 BrocadeCommunications Systems,Inc.
 * All rights reserved. Description: This file extends the
 * LinearSequencerContext for the virtual ip global worker.
 *
 * Author :     Brian Adaniya
 * Date :       12/19/2012
 */

#include "VcsFabric/Global/VirtualIpGlobalWorkerSequencerContext.h"

namespace DcmNs
{

VirtualIpGlobalWorkerSequencerContext::VirtualIpGlobalWorkerSequencerContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps) :
    PrismLinearSequencerContext (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
    m_virtualIpAddress      (""),
    m_boundVirtualIpAddress ("NULL"),
    m_isVirtualIpDuplicate  (false)
{
}

VirtualIpGlobalWorkerSequencerContext::~VirtualIpGlobalWorkerSequencerContext ()
{
}

void VirtualIpGlobalWorkerSequencerContext::setVirtualIpAddress (const string &virtualIpAddress)
{
    m_virtualIpAddress = virtualIpAddress;
}

const string &VirtualIpGlobalWorkerSequencerContext::getVirtualIpAddress ()
{
    return (m_virtualIpAddress);
}

void VirtualIpGlobalWorkerSequencerContext::setBoundVirtualIpAddress (const string &boundVirtualIpAddress)
{
    m_boundVirtualIpAddress = boundVirtualIpAddress;
}

const string &VirtualIpGlobalWorkerSequencerContext::getBoundVirtualIpAddress ()
{
    return (m_boundVirtualIpAddress);
}

void VirtualIpGlobalWorkerSequencerContext::setIsVirtualIpDuplicate (bool isVirtualIpDuplicate)
{
    m_isVirtualIpDuplicate = isVirtualIpDuplicate;
}

bool VirtualIpGlobalWorkerSequencerContext::getIsVirtualIpDuplicate ()
{
    return (m_isVirtualIpDuplicate);
}

}
