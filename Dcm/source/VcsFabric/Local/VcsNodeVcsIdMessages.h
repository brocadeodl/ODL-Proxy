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

#ifndef VCSNODEVCSIDMESSAGES_H
#define VCSNODEVCSIDMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetVcsIdMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        //PhysicalInterfaceCreateMessage ();
                        VcsNodeSetVcsIdMessage();
                        VcsNodeSetVcsIdMessage (const UI32 &vcsId);
        virtual        ~VcsNodeSetVcsIdMessage ();

                UI32    getVcsId                     () const;

    // Now the data members

    private :
        UI32 m_vcsId;

    protected :
    public :
};

class VcsNodeGetVcsIdMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetVcsIdMessage();
        virtual        ~VcsNodeGetVcsIdMessage ();

                UI32    getVcsId               () const;
                void    setVcsId               (const UI32 &vcs_id);

    // Now the data members

    private :
        UI32 m_vcsId;

    protected :
    public :
};
}

#endif // VCSNODEVCSIDMESSAGES_H
