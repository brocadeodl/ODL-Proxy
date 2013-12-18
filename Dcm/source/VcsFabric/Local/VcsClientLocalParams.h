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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSCLIENTLOCALPARAMS_H
#define VCSCLIENTLOCALPARAMS_H

#include "Framework/Types/Types.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsClientLocalParams
{
    private :
    protected :
    public :
            VcsClientLocalParams    ();
           ~VcsClientLocalParams    ();

    UI32    getMappedId                   () const;
    void    setMappedId                   (const UI32 &mapped_id);

    // Now the Data Members

    private :
        UI32   m_mapped_id;

    protected :
    public :
};

}

#endif // VCSCLIENTLOCALPARAMS_H
