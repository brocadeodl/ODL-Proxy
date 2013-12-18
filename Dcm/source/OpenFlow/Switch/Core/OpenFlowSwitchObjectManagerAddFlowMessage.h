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
 *   Copyright (C) 2013 Brocade Communications Systems, Inc.               *
 *   All rights reserved.                                                  *
 *   Author : Kausik Sridharabalan                                          *
 ***************************************************************************/

#ifndef OPENFLOWSWITCHOBJECTMANAGERADDFLOWMESSAGE_H
#define OPENFLOWSWITCHOBJECTMANAGERADDFLOWMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"

using namespace WaveNs;

namespace OpenFlowNs
{

class OpenFlowSwitchObjectManagerAddFlowMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                        OpenFlowSwitchObjectManagerAddFlowMessage ();
        virtual        ~OpenFlowSwitchObjectManagerAddFlowMessage ();


                UI32        getOpenFlowIngressIfIndex                                    () const;
                void        setOpenFlowIngressIfIndex                                    (const UI32 &OpenFlowIngressIfIndex);

                UI32        getOpenFlowEgressIfIndex                                     () const;
                void        setOpenFlowEgressIfIndex                                     (const UI32 &OpenFlowEgressIfIndex);

                UI32        getOpenFlowSrcIpV4Address                                    () const;
                void        setOpenFlowSrcIpV4Address                                    (const UI32 &OpenFlowSrcIpV4Address); 

                UI32        getOpenFlowSrcIpV4AddressMask                                () const;
                void        setOpenFlowSrcIpV4AddressMask                                (const UI32 &OpenFlowSrcIpV4AddressMask); 

                UI32        getOpenFlowDstIpV4Address                                    () const;
                void        setOpenFlowDstIpV4Address                                    (const UI32 &OpenFlowDstIpV4Address); 
                
                UI32        getOpenFlowDstIpV4AddressMask                                () const;
                void        setOpenFlowDstIpV4AddressMask                                (const UI32 &OpenFlowDstIpV4AddressMask); 

                string      getOpenFlowAppType                                           () const;
                void        setOpenFlowAppType                                           (const string &OpenFlowAppType);

                UI16        getOpenFlowSrcPort                                           () const;
                void        setOpenFlowSrcPort                                           (const UI16 &OpenFlowSrcPort);

                UI16        getOpenFlowDstPort                                           () const;
                void        setOpenFlowDstPort                                           (const UI16 &OpenFlowDstPort);

    // Now the Data Members

    private :
        UI32        m_openFlowIngressIfIndex;
        UI32        m_openFlowEgressIfIndex;
        UI32        m_openFlowSrcIpV4Address;
        UI32        m_openFlowSrcIpV4AddressMask;
        UI32        m_openFlowDstIpV4Address;
        UI32        m_openFlowDstIpV4AddressMask;
        string      m_openFlowAppType;
        UI16        m_openFlowSrcPort;
        UI16        m_openFlowDstPort;

    protected :
    public :
};

}

#endif // OPENFLOWSWITCHOBJECTMANAGERADDFLOWMESSAGE_H 

