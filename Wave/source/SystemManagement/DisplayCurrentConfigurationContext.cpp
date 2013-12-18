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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#include "SystemManagement/DisplayCurrentConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangDisplayConfigurationContext.h"
#include "Modeling/YANG/ObjectModel/YangElement.h"
#include "Framework/ObjectModel/WaveManagedObjectQueryContext.h"
#include "Framework/Utils/AssertUtils.h"

namespace WaveNs
{

DisplayCurrentConfigurationContext::DisplayCurrentConfigurationContext (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps)
    : PrismLinearSequencerContext        (pPrismMessage, pPrismElement, pSteps, numberOfSteps),
      m_pYangDisplayConfigurationContext (NULL),
      m_currentYangElementIndex          (0),
      m_pYangElement                     (NULL),
      m_pWaveManagedObjectQueryContext   (NULL)
{
}

DisplayCurrentConfigurationContext::~DisplayCurrentConfigurationContext ()
{
    if (NULL != m_pYangDisplayConfigurationContext)
    {
        delete m_pYangDisplayConfigurationContext;

        m_pYangDisplayConfigurationContext = NULL;
    }

    if (NULL != m_pWaveManagedObjectQueryContext)
    {
        delete m_pWaveManagedObjectQueryContext;

        m_pWaveManagedObjectQueryContext = NULL;
    }
}

YangDisplayConfigurationContext *DisplayCurrentConfigurationContext::getPYangDisplayConfigurationContext ()
{
    return (m_pYangDisplayConfigurationContext);
}

void DisplayCurrentConfigurationContext::setPYangDisplayConfigurationContext (YangDisplayConfigurationContext *pYangDisplayConfigurationContext)
{
    m_pYangDisplayConfigurationContext = pYangDisplayConfigurationContext;
}

vector<YangElement *> DisplayCurrentConfigurationContext::getYangElements ()
{
    return (m_yangElements);
}

void DisplayCurrentConfigurationContext::setYangElements (vector<YangElement *> &yangElements)
{
    UI32 numberOfYangElements = m_yangElements.size ();

    prismAssert (0 == numberOfYangElements, __FILE__, __LINE__);

    m_yangElements = yangElements;

    m_currentYangElementIndex = 0;

    m_pYangElement = m_yangElements[m_currentYangElementIndex];

    prismAssert (NULL != m_pYangElement, __FILE__, __LINE__);
}

UI32 DisplayCurrentConfigurationContext::getCurrentYangElementIndex () const
{
    return (m_currentYangElementIndex);
}

void DisplayCurrentConfigurationContext::advanceToNextYangElement ()
{
    bool isAtLast = isAtTheLastYangElement ();

    if (true == isAtLast)
    {
        prismAssert (false, __FILE__, __LINE__);
    }
    else
    {
        m_currentYangElementIndex++;

        m_pYangElement = m_yangElements[m_currentYangElementIndex];

        prismAssert (NULL != m_pYangElement, __FILE__, __LINE__);
    }
}

bool DisplayCurrentConfigurationContext::isAtTheLastYangElement ()
{
    UI32 numberOfYangElements = m_yangElements.size ();

    if (0 == numberOfYangElements)
    {
        prismAssert (false, __FILE__, __LINE__);
    }

    if ((numberOfYangElements - 1) == m_currentYangElementIndex)
    {
        return (true);
    }
    else
    {
        return (false);
    }
}

YangElement *DisplayCurrentConfigurationContext::getPYangElement ()
{
    return (m_pYangElement);
}

void DisplayCurrentConfigurationContext::setPYangElement (YangElement *pYangElement)
{
    m_pYangElement = pYangElement;
}

WaveManagedObjectQueryContext *DisplayCurrentConfigurationContext::getPWaveManagedObjectQueryContext ()
{
    return (m_pWaveManagedObjectQueryContext);
}

void DisplayCurrentConfigurationContext::setPWaveManagedObjectQueryContext (WaveManagedObjectQueryContext *pWaveManagedObjectQueryContext)
{
    if (NULL != m_pWaveManagedObjectQueryContext)
    {
        delete m_pWaveManagedObjectQueryContext;
    }

    m_pWaveManagedObjectQueryContext = pWaveManagedObjectQueryContext;
}

}
