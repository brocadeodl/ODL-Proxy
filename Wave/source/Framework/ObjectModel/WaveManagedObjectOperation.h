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
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef WAVEMANAGEDOBJECTOPERATION_H
#define WAVEMANAGEDOBJECTOPERATION_H

#include "Framework/Types/Types.h"

namespace WaveNs
{

class WaveManagedObjectOperation
{
    private :
    protected :
    public :
                        WaveManagedObjectOperation ();
                        ~WaveManagedObjectOperation ();

        void            setReason                   (const ResourceId &reason);
        ResourceId      getReason                   () const;

    // Now the data members

    private :
        ResourceId  m_reason;

    protected :
    public :
};

}

#endif // WAVEMANAGEDOBJECTOPERATION_H