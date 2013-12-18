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
 *   Author : Anish Bhaskaran                                              *
 **************************************************************************/

#ifndef ARPDEBUGCLIENTMESSAGE_H
#define ARPDEBUGCLIENTMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"
#include "Framework/Types/IpV4Address.h"

using namespace WaveNs;

namespace DcmNs
{

    class ArpDebugClientMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            ArpDebugClientMessage ();
            ArpDebugClientMessage (const IpV4Address &IpAddress);
            virtual    ~ArpDebugClientMessage ();
            void setIpAddress(const IpV4Address &IpAddress);
            IpV4Address getIpAddress()  const;
            void setNoFlag(const UI32 &NoFlag);
            UI32 getNoFlag()  const;
            virtual const void *getCStructureForInputs ();
            virtual void  loadOutputsFromCStructure (const void *pOutputCStructure);

// Now the data members

        private:
        protected:
        public:
			UI32 m_NoFlag;	
            IpV4Address  m_IpAddress;
    };
}
#endif                                            //ARPDEBUGCLIENTMESSAGE_H
