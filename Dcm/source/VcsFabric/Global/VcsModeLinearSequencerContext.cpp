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
 *   @file VcsModeLinearSequencerContext.cpp
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.
 *   All rights reserved.
 *   Description:    This file defines the implementation for the VcsModeLinearSequencerContext.
 *   Author:         Brian Adaniya
 *   Date:           12/13/2010
 */

#include "VcsFabric/Global/VcsModeLinearSequencerContext.h"

namespace DcmNs
{

VcsModeLinearSequencerContext::VcsModeLinearSequencerContext (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext (pPrismAsynchronousContext, pPrismElement, pSteps, numberOfSteps),
    m_vcsMode (false),
    m_vcsClusterMode (false)
{
}

VcsModeLinearSequencerContext::~VcsModeLinearSequencerContext ()
{
}

bool VcsModeLinearSequencerContext::getVcsMode () const
{
    return (m_vcsMode);
}

void VcsModeLinearSequencerContext::setVcsMode (bool vcsMode)
{
    m_vcsMode = vcsMode;
}

bool VcsModeLinearSequencerContext::getVcsClusterMode () const
{
    return (m_vcsClusterMode);
}

void VcsModeLinearSequencerContext::setVcsClusterMode (bool vcsClusterMode)
{
    m_vcsClusterMode = vcsClusterMode;
}

}
