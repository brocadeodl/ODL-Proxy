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

#ifndef ADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCH_H
#define ADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCH_H

#include "Framework/Utils/PrismLinearSequencerContext.h"
#include "Framework/Types/Uri.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;
using namespace DcmNs;

namespace OpenFlowNs
{

class OpenFlowControllerManagedObject;
class OpenFlowLogicalSwitchManagedObject;

class AddOpenFlowControllerToOpenFlowLogicalSwitchContext : public PrismLinearSequencerContext
{
    private :
    protected :
    public :
                                            AddOpenFlowControllerToOpenFlowLogicalSwitchContext   (PrismMessage *pPrismMessage, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                            AddOpenFlowControllerToOpenFlowLogicalSwitchContext   (PrismAsynchronousContext *pPrismAsynchronousContext, PrismElement *pPrismElement, PrismLinearSequencerStep *pSteps, UI32 numberOfSteps);
                                           ~AddOpenFlowControllerToOpenFlowLogicalSwitchContext   ();

        string                              getOpenFlowLogicalSwitchName                          () const;
        void                                setOpenFlowLogicalSwitchName                          (const string &openFlowLogicalSwitchName);

        Uri                                 getOpenFlowLogicalSwitchUri                           () const;
        void                                setOpenFlowLogicalSwitchUri                           (const Uri &openLogicalSwitchUri);

        string                              getOpenFlowcontrollerIpAddress                        () const;
        void                                setOpenFlowControllerIpAddress                        (const string &openFlowControllerIpAddress);

        UI32                                getOpenFlowControllerPort                             () const;
        void                                setOpenFlowControllerPort                             (const UI32 &openFlowControllerPort);

        OpenFlowControllerManagedObject    *getPOpenFlowControllerManagedObejct                   ();
        void                                setPOpenFlowControllerManagedObject                   (OpenFlowControllerManagedObject *pOpenFlowcontrollerManagedObject);

        OpenFlowLogicalSwitchManagedObject *getPOpenFlowLogicalSwitchManagedObject                ();
        void                                setPOpenFlowLogicalSwitchManagedObject                (OpenFlowLogicalSwitchManagedObject *pOpenFlowLogicalSwitchManagedObject);

        OpenFlowChannelConnectionProtocol   getOpenFlowcontrollerChannelConnectionDefaultProtocol () const;
        void                                setOpenFlowcontrollerChannelConnectionDefaultProtocol (const OpenFlowChannelConnectionProtocol &openFlowControllerChannelconnectionDefaultProtocol);

    // Now the data members

    private :
        string                              m_openFlowLogicalSwitchName;
        Uri                                 m_openFlowLogicalSwitchUri;
        string                              m_openFlowControllerIpAddress;
        UI32                                m_openFlowControllerPort;
        OpenFlowChannelConnectionProtocol   m_openFlowControllerChannelConnectionDefaultProtocol;

        OpenFlowControllerManagedObject    *m_pOpenFlowControllerManagedObject;
        OpenFlowLogicalSwitchManagedObject *m_pOpenFlowLogicalSwitchManagedObject;

    protected :
    public :
};

}

#endif // ADDOPENFLOWCONTROLLERTOOPENFLOWLOGICALSWITCH_H
