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
 *   Copyright (C) 2010 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Himanshu Varshney                                            *
 ***************************************************************************/

#include "Framework/Core/ApplicationSpecificServices.h"
#include "Framework/Utils/AssertUtils.h"
#include "Framework/Utils/TraceUtils.h"

namespace WaveNs
{

ApplicationSpecificServices::ApplicationSpecificServices ()
{
}

ApplicationSpecificServices::~ApplicationSpecificServices ()
{
}

ApplicationSpecificServices *ApplicationSpecificServices::getInstance ()
{
    static ApplicationSpecificServices *pApplicationSpecificServices = NULL;

    if (NULL == pApplicationSpecificServices)
    {
        pApplicationSpecificServices = new ApplicationSpecificServices ();
        WaveNs::prismAssert (NULL != pApplicationSpecificServices, __FILE__, __LINE__);
    }

    return (pApplicationSpecificServices);
}

void ApplicationSpecificServices::setPrismServiceId (PrismServiceId prismServiceId)
{
    m_PrismServiceId.push_back (prismServiceId);
}

bool ApplicationSpecificServices::isApplicationSpecificServiceId (PrismServiceId prismServiceId)
{
    vector<PrismServiceId>::iterator element = m_PrismServiceId.begin ();
    vector<PrismServiceId>::iterator end     = m_PrismServiceId.end ();

    while (end != element)
    {
        if (prismServiceId == (*element))
        {
            return (true);
        }
        
        element++;
    }

    return (false);
}

}


