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
 *   Author : pbalacha                                                     *
 **************************************************************************/

#ifndef QOSGLOBALNASMESSAGE_H
#define QOSGLOBALNASMESSAGE_H

#include "ManagementInterface/ManagementInterfaceMessage.h"
#include "Framework/Attributes/AttributeIpV4AddressNetworkMask.h"
#include "Framework/Attributes/Attributes.h"
#include "Qos/Common/QosBaseMessages.h"

using namespace WaveNs;


namespace DcmNs
{
    class QosGlobalNasMessage : public ManagementInterfaceMessage, public QosNasBaseMessage
    {
        private:
        protected:
        public:
            QosGlobalNasMessage ();
    		QosGlobalNasMessage (const UI32 opCode, const UI32 cmdCode, const UI32 cos, const UI32 dscp,
                const IpV4AddressNetworkMask serverIpAddressPrefix, const string vrfName, const UI32 vlanNumber);
            virtual    ~QosGlobalNasMessage ();
			virtual void  setupAttributesForSerialization();

// Now the data members

        private:
        protected:
        public:
    };
}
#endif                                            //QOSGLOBALNASMESSAGE_H
