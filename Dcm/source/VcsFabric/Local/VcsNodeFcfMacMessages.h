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

#ifndef VCSNODEFCFMACMESSAGES_H
#define VCSNODEFCFMACMESSAGES_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ManagementInterface/ManagementInterfaceServiceIndependentMessage.h"

using namespace WaveNs;

namespace DcmNs
{

class VcsNodeSetFcfMacMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        VcsNodeSetFcfMacMessage();
                        VcsNodeSetFcfMacMessage (const MacAddress &fcf_mac);
        virtual        ~VcsNodeSetFcfMacMessage ();

           void         setFcfMac (const MacAddress &fcf_mac);
           MacAddress   getFcfMac () const;

    // Now the data members

    private :
        MacAddress m_fcfMac;

    protected :
    public :
};

class VcsNodeSetFcfMacMessageServiceIndependent : public ManagementInterfaceServiceIndependentMessage
{   
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
        VcsNodeSetFcfMacMessageServiceIndependent ();
        VcsNodeSetFcfMacMessageServiceIndependent (const MacAddress &fcf_mac);
        virtual        ~VcsNodeSetFcfMacMessageServiceIndependent ();

        void         setFcfMac (const MacAddress &fcf_mac);
        MacAddress   getFcfMac () const;

        // Now the data members

    private :
        MacAddress m_fcfMac;

    protected :
    public :
};

}

#endif // VCSNODEFCFMACMESSAGES_H
