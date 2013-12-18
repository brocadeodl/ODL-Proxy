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
 *   Copyright (C) 2013      Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Kuai Yu                                                      *
 ***************************************************************************/

#include "Framework/Utils/TraceUtils.h"
#include "Framework/CliBlockManagement/CliBlockContext.h"
#include "Framework/CliBlockManagement/CliBlockType.h"

namespace WaveNs
{

CliBlockContext::CliBlockContext ()
{
    m_reason = WAVE_FRAMEWORK_STATE_UNKNOWN_STATE;
}

CliBlockContext::CliBlockContext (ResourceId reason)
{
    m_reason = reason;
}

CliBlockContext::CliBlockContext (const CliBlockContext &context)
{
    m_reason = context.m_reason;
}

CliBlockContext::~CliBlockContext ()
{
}

CliBlockContext& CliBlockContext::operator= (const CliBlockContext &rhs)
{

    if (this == &rhs)
    {
        return (*this); 
    }
    
    m_reason   = rhs.m_reason;

    return (*this);
}

const ResourceId CliBlockContext::getReason () const
{
    return m_reason;
}

void CliBlockContext::setReason (ResourceId reason)
{
    m_reason = reason;
}

}
