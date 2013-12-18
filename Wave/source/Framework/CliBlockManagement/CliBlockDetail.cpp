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
#include "Framework/CliBlockManagement/CliBlockDetail.h"
#include "Framework/CliBlockManagement/CliBlockContext.h"
#include "Framework/CliBlockManagement/CliBlockServiceIndependentMessage.h"
#include "Framework/CliBlockManagement/CliBlockType.h"

namespace WaveNs
{

CliBlockDetail::CliBlockDetail ()
{
    m_prismServiceId    = 0; 
    m_originator        = 0;
    m_cliBlockContext   = CliBlockContext ();
}

CliBlockDetail::CliBlockDetail (PrismServiceId prismServiceId, LocationId originator, const CliBlockContext &cliBlockContext)
{
    m_prismServiceId    = prismServiceId;
    m_originator        = originator;
    m_cliBlockContext   = cliBlockContext;
}

CliBlockDetail::CliBlockDetail (const CliBlockServiceIndependentMessage &cliBlockServiceIndependentMessage)
{
    m_prismServiceId          = cliBlockServiceIndependentMessage.getPrismServiceId ();
    m_originator              = cliBlockServiceIndependentMessage.getOriginator ();
    m_cliBlockContext         = CliBlockContext (cliBlockServiceIndependentMessage.getReason ());
}

CliBlockDetail::CliBlockDetail (const CliBlockDetail &detail) :
     m_prismServiceId (detail.m_prismServiceId), m_originator (detail.m_originator), m_cliBlockContext (detail.m_cliBlockContext)
{
}

CliBlockDetail::~CliBlockDetail ()
{
}

CliBlockDetail& CliBlockDetail::operator= (const CliBlockDetail &rhs)
{

    if (this == &rhs)
    {
        return (*this); 
    }
    
    m_prismServiceId    = rhs.m_prismServiceId;
    m_originator        = rhs.m_originator; 
    m_cliBlockContext   = rhs.m_cliBlockContext;

    return (*this);
}

const PrismServiceId CliBlockDetail::getPrismServiceId () const
{
    return m_prismServiceId;
}

const LocationId CliBlockDetail::getOriginator () const
{
    return m_originator;
}

const CliBlockContext& CliBlockDetail::getCliBlockContext () const
{
    return m_cliBlockContext;
}

}
