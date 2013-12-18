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

#ifndef VCSNODEPRINCIPALMESSAGES_H
#define VCSNODEPRINCIPALMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetIsPrincipalMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetIsPrincipalMessage();
                        VcsNodeSetIsPrincipalMessage (const bool &isPrincipal);
        virtual        ~VcsNodeSetIsPrincipalMessage ();

                bool    getIsPrincipal                     () const;

    // Now the data members

    private :
        bool m_isPrincipal;

    protected :
    public :
};

class VcsNodeGetIsPrincipalMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetIsPrincipalMessage();
                        VcsNodeGetIsPrincipalMessage (const UI32 &mappedId);
        virtual        ~VcsNodeGetIsPrincipalMessage ();

                bool    getIsPrincipal               () const;
                UI32    getMappedId            () const;
                void    setIsPrincipal               (const bool &isPrincipal);
                void    setMappedId            (const UI32 &mapped_id);

    // Now the data members

    private :
        UI32 m_mappedId;
        bool m_isPrincipal;

    protected :
    public :
};

class VcsNodeSetPrincipalPriorityMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetPrincipalPriorityMessage ();
                        VcsNodeSetPrincipalPriorityMessage (const UI32 principalPriority);
        virtual        ~VcsNodeSetPrincipalPriorityMessage ();

                UI32    getPrincipalPriority               () const;

    // Now the data members

    private :
        UI32 m_principalPriority;

    protected :
    public :
};

class VcsNodeGetPrincipalPriorityMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeGetPrincipalPriorityMessage ();
        virtual        ~VcsNodeGetPrincipalPriorityMessage ();

                UI32    getPrincipalPriority               () const;
                void    setPrincipalPriority               (const UI32 principalPriority);

    // Now the data members

    private :
        UI32 m_principalPriority;

    protected :
    public :
};

class VcsNodePrincipalTriggerFailover : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodePrincipalTriggerFailover ();
        virtual        ~VcsNodePrincipalTriggerFailover ();

    // Now the data members

    private :
    protected :
    public :

};

}

#endif // VCSNODEPRINCIPALMESSAGES_H
