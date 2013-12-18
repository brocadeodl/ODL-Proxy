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

#ifndef VCSNODEVCSMODEMESSAGES_H
#define VCSNODEVCSMODEMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetVcsModeMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetVcsModeMessage();
                        VcsNodeSetVcsModeMessage (const UI32 &vcsId);
        virtual        ~VcsNodeSetVcsModeMessage ();

                UI32    getVcsMode                   () const;

    // Now the data members

    private :
        UI32 m_vcsMode;

    protected :
    public :
};

class VcsNodeGetVcsModeMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetVcsModeMessage ();
        virtual        ~VcsNodeGetVcsModeMessage ();

                UI32    getVcsMode             () const;
                void    setVcsMode             (const UI32 &vcs_id);

    // Now the data members

    private :
        UI32 m_vcsMode;

    protected :
    public :
};

class VcsNodeGetClusterTypeMessage : public ManagementInterfaceServiceIndependentMessage 
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetClusterTypeMessage ();
        virtual        ~VcsNodeGetClusterTypeMessage ();

                UI32    getClusterType             () const;
                void    setClusterType             (const UI32 &clusterType);

    // Now the data members

    private :
        UI32 m_clusterType;

    protected :
    public :
};

class VcsNodeEnableValidationMessage : public ManagementInterfaceMessage
{
    private :
    protected :
    public :
                        VcsNodeEnableValidationMessage ();
        virtual        ~VcsNodeEnableValidationMessage ();

    // Now the data members

    private :
    protected :
    public :
};

class VcsNodeSetNewShowVcsFormatMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetNewShowVcsFormatMessage();
                        VcsNodeSetNewShowVcsFormatMessage (const bool &newShowVcs);
        virtual        ~VcsNodeSetNewShowVcsFormatMessage ();

                bool    getNewShowVcs             () const;
                void    setNewShowVcs             (const bool &newShowVcs);
    // Now the data members

    private :
        bool m_newShowVcs;

    protected :
    public :
};

}

#endif // VCSNODEVCSMODEMESSAGES_H
