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
 *   Author : Aparna Kuppachi                                              *
 ***************************************************************************/

#ifndef DHCPRELCLIENTSHOWMESSAGE_H 
#define DHCPRELCLIENTSHOWMESSAGE_H

#include "DcmMessaging/DcmManagementInterfaceMessage.h"

using namespace WaveNs;

namespace DcmNs
{

    class DhcpRelClientShowMessage : public DcmManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            DhcpRelClientShowMessage();
            DhcpRelClientShowMessage(const UI32 &cmdCode, const string &ifName, 
				     const UI32 &ifType, const UI32 &vlanValue);
            virtual ~DhcpRelClientShowMessage();
            void setIfName (const string &ifName);
            string getIfName ()  const;
            void setCmdCode (const UI32 &cmdCode);
            UI32 getCmdCode ()  const;
            void setIfType (const UI32 &ifType);
            UI32 getIfType ()  const;
            void setVlanValue (const UI32 &vlanValue);
            UI32 getVlanValue ()  const;

            virtual const void *getCStructureForInputs ();
            virtual void loadOutputsFromCStructure (const void *pOutputCStructure);

            void getOutputBufferRef (UI32 bufferNum, UI32 &size, void *&pBuffer);

        private:
        protected:
        public:
            UI32    m_cmdCode;
            string  m_ifName;
            UI32    m_ifType;
            UI32    m_vlanValue;
    };
}
#endif  //DHCPRELCLIENTSHOWMESSAGE_H

