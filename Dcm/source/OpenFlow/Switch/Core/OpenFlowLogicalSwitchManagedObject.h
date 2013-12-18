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
 *   Copyright (C) 2005-2012 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Vidyasagara Reddy Guntaka                                    *
 ***************************************************************************/

#ifndef OPENFLOWLOGICALSWITCHMANAGEDOBJECT_H
#define OPENFLOWLOGICALSWITCHMANAGEDOBJECT_H

#include "DcmObjectModel/DcmManagedObject.h"
#include "Framework/Types/Uri.h"
#include "OpenFlow/Switch/Core/OpenFlowControllerManagedObject.h"
#include "Framework/ObjectModel/WaveManagedObjectPointer.cpp"
#include "Framework/Types/IpV4AddressNetworkMask.h"
#include "DcmResourceIdEnums.h"

using namespace WaveNs;
using namespace DcmNs;
using namespace std;

namespace OpenFlowNs
{

class OpenFlowLogicalSwitchManagedObject : public DcmManagedObject
{
    private :
        void setupAttributesForPersistence ();
        void setupAttributesForCreate      ();

        void processFlowAddRequest         ();

    protected :
    public :
                        OpenFlowLogicalSwitchManagedObject (WaveObjectManager *pWaveObjectManager);
        virtual        ~OpenFlowLogicalSwitchManagedObject ();

        static  string  getClassName                       ();

                string  getOpenFlowLogicalSwitchName       () const;
                void    setOpenFlowLogicalSwitchName       (const string &openFlowLogicalSwitchName);

                Uri     getOpenFlowLogicalSwitchUri        () const;
                void    setOpenFlowLogicalSwitchUri        (const Uri &openFlowLogicalSwitchUri);

                bool    isAKnownOpenFlowController         (const IpV4AddressNetworkMask &openFlowControllerIpV4AddressNetworkMask, const UI32 &openFlowControllerPort);
                void    addOpenFlowController              (const IpV4AddressNetworkMask &openFlowControllerIpV4AddressNetworkMask, const UI32 &openFlowControllerPort, const OpenFlowChannelConnectionProtocol &openFlowChannelConnectionProtocol);

                void    connectToKnownOpenFlowControllers  ();

    // Now the data members

    private :
        string                                                             m_openFlowLogicalSwitchName;
        Uri                                                                m_openFlowLogicalSwitchUri;

        vector<WaveManagedObjectPointer<OpenFlowControllerManagedObject> > m_openFlowControllers;

    protected :
    public :
};

}

#endif // OPENFLOWLOGICALSWITCHMANAGEDOBJECT_H
