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
 *   Copyright (C) 2005-2013 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHMESSAGE_H
#define OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/Uri.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;
using namespace DcmNs;

namespace OpenFlowNs
{

class OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage : public ManagementInterfaceMessage
{
    private :
        void setupAttributesForSerialization ();

    protected :
    public :
                                                   OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage ();
        virtual                                   ~OpenFlowSwitchObjectManagerAddOpenFlowControllerToOpenFlowLogicalSwitchMessage ();

                string                             getOpenFlowLogicalSwitchName                                                   () const;
                void                               setOpenFlowLogicalSwitchName                                                   (const string &openFlowLogicalSwitchName);

                Uri                                getOpenFlowLogicalSwitchUri                                                    () const;
                void                               setOpenFlowLogicalSwitchUri                                                    (const Uri &openLogicalSwitchUri);

                string                             getOpenFlowControllerIpAddress                                                 () const;
                void                               setOpenFlowControllerIpAddress                                                 (const string &openFlowControllerIpAddress);

                UI32                               getOpenFlowControllerPort                                                      () const;
                void                               setOpenFlowControllerPort                                                      (const UI32 &openFlowControllerPort);

                OpenFlowChannelConnectionProtocol  getOpenFlowcontrollerChannelConnectionDefaultProtocol                          () const;
                void                               setOpenFlowcontrollerChannelConnectionDefaultProtocol                          (const OpenFlowChannelConnectionProtocol &openFlowControllerChannelconnectionDefaultProtocol);

    // Now the Data Members

    private :
        string                            m_openFlowLogicalSwitchName;
        Uri                               m_openFlowLogicalSwitchUri;
        string                            m_openFlowControllerIpAddress;
        UI32                              m_openFlowControllerPort;
        OpenFlowChannelConnectionProtocol m_openFlowControllerChannelConnectionDefaultProtocol;

    protected :
    public :
};

}

#endif // OPENFLOWSWITCHOBJECTMANAGERADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCHMESSAGE_H
