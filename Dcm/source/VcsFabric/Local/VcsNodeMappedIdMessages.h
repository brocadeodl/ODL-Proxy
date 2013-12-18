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
 *   Copyright (C) 2005-2009 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vipool Prajapati                                             *
 ***************************************************************************/

#ifndef VCSNODEMAPPEDIDMESSAGES_H
#define VCSNODEMAPPEDIDMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeGetLocalMappedIdMessage : public ManagementInterfaceServiceIndependentMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetLocalMappedIdMessage();
        virtual        ~VcsNodeGetLocalMappedIdMessage ();

           UI32         getMappedId () const;
           void         setMappedId (const UI32 &mappedId);

    // Now the data members

    private :
        UI32   m_mappedId;

    protected :
    public :
};

}

#endif // VCSNODEMAPPEDIDMESSAGES_H
