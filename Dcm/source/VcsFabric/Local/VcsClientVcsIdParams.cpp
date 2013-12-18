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

#include "VcsFabric/Local/VcsClientVcsIdParams.h"

namespace DcmNs
{

VcsClientVcsIdParams::VcsClientVcsIdParams ()
{
    m_vcsId = 0;
}

VcsClientVcsIdParams::~VcsClientVcsIdParams ()
{
}

UI32 VcsClientVcsIdParams::getVcsId () const
{
    return (m_vcsId);
}

void VcsClientVcsIdParams::setVcsId (const UI32 &vcsId)
{
    m_vcsId = vcsId;
}

}