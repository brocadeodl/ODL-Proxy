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
 *   Copyright (C) 2011 Brocade Communications Systems, Inc.          *
 *   All rights reserved.                                                  *
 *   Author : Debojyoti                                                     *
 **************************************************************************/

#ifndef IPPOLICYDELETEPBR_H
#define IPPOLICYDELETEPBR_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "ClientInterface/InterfaceCommon.h"

using namespace WaveNs;

namespace DcmNs
{

    class IpPolicyDeletePBRMessage : public ManagementInterfaceMessage
    {
        private:
        protected:
            virtual void setupAttributesForSerialization();
        public:
            IpPolicyDeletePBRMessage ();
            IpPolicyDeletePBRMessage (const string &ifName, UI32 ifType);
            virtual ~IpPolicyDeletePBRMessage ();
            string getIfName();
            UI32 getIfType();

// Now the data members

        private:
        protected:
        public:
            string    m_ifName;
            UI32  m_ifType;
    };
}
#endif                                            //IPPOLICYDELETEPBR_H
