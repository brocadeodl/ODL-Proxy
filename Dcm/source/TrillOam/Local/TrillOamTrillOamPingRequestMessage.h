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
 *   Author : mmahajan                                                     *
 **************************************************************************/

#ifndef TRILLOAMTRILLOAMPINGREQUESTMESSAGE_H
#define TRILLOAMTRILLOAMPINGREQUESTMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Types/MacAddress.h"
#include "Framework/Types/MacAddress.h"

using namespace WaveNs;

namespace DcmNs
{

    class TrillOamTrillOamPingRequestMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            TrillOamTrillOamPingRequestMessage ();
            TrillOamTrillOamPingRequestMessage (const MacAddress &srcmac,const MacAddress &destmac,const UI32 &vlanid);
            virtual    ~TrillOamTrillOamPingRequestMessage ();
            void setSrcmac(const MacAddress &srcmac);
            MacAddress getSrcmac()  const;
            void setDestmac(const MacAddress &destmac);
            MacAddress getDestmac()  const;
            void setVlanid(const UI32 &vlanid);
            UI32 getVlanid()  const;

// Now the data members

        private:
        protected:
        public:
            MacAddress  m_srcmac;
            MacAddress  m_destmac;
            UI32  m_vlanid;
    };
}
#endif                                            //TRILLOAMTRILLOAMPINGREQUESTMESSAGE_H
